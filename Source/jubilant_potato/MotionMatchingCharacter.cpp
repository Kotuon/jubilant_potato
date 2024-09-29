
#include "MotionMatchingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AMotionMatchingCharacter::AMotionMatchingCharacter(
    const FObjectInitializer& ObjectInitializer )
    : ACharacter( ObjectInitializer ) {
    PrimaryActorTick.bCanEverTick = true;

    LandedDelegate.AddUniqueDynamic( this, &AMotionMatchingCharacter::OnLanded );
}

void AMotionMatchingCharacter::BeginPlay() {
    Super::BeginPlay();
    //...

    movement = GetCharacterMovement();
}

void AMotionMatchingCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    UpdateMovement();
}

void AMotionMatchingCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}

///
//@brief Updates Gait and sets max walk speed for movement component.
//
///
void AMotionMatchingCharacter::UpdateMovement() {
    gait = GetDesiredGait();
    float maxMoveSpeed = CalculateMaxSpeed();

    movement->MaxWalkSpeed = maxMoveSpeed;
    movement->MaxWalkSpeedCrouched = maxMoveSpeed;
}

///
//@brief Uses WantsToStrafe to update character rotation. Setting RotationRate
// to -1 causes instant rotation.
//
///
void AMotionMatchingCharacter::UpdateRotation() {

    if ( wantsToStrafe ) {
        movement->bUseControllerDesiredRotation = true;
        movement->bOrientRotationToMovement = false;
    } else {
        movement->bUseControllerDesiredRotation = false;
        movement->bOrientRotationToMovement = true;
    }

    if ( movement->IsFalling() ) {
        movement->RotationRate = FRotator( 0.f, 0.f, 200.f );
    } else {
        movement->RotationRate = FRotator( 0.f, 0.f, -1.f );
    }
}

EGait AMotionMatchingCharacter::GetDesiredGait() const {
    if ( wantsToSprint ) {
        return EGait::ERun;
    } else if ( wantsToWalk ) {
        return EGait::EWalk;
    } else {
        return EGait::ERun;
    }
}

float AMotionMatchingCharacter::CalculateMaxSpeed() const {
    const FVector velocity = movement->Velocity;
    const FRotator characterRotation = GetActorRotation();

    float direction = UKismetAnimationLibrary::CalculateDirection(
        velocity, characterRotation );
    direction = abs( direction );

    const float strafeSpeedMap =
        strafeSpeedMapCurve->GetFloatValue( direction );

    FVector speedToUse;

    switch ( gait ) {
    case EGait::EWalk:
        speedToUse = walkSpeeds;
        break;
    case EGait::ERun:
        speedToUse = runSpeeds;
        break;
    case EGait::ESprint:
        speedToUse = sprintSpeeds;
        break;
    }

    if ( movement->IsCrouching() ) {
        speedToUse = crouchSpeeds;
    }

    if ( strafeSpeedMap < 0.f ) {
        return UKismetMathLibrary::MapRangeClamped(
            strafeSpeedMap, 0.f, 1.f, speedToUse.X, speedToUse.Y );
    } else {
        return UKismetMathLibrary::MapRangeClamped(
            strafeSpeedMap, 1.f, 2.f, speedToUse.Y, speedToUse.Z );
    }
}

void AMotionMatchingCharacter::OnLanded( const FHitResult& Hit ) {
    landVelocity = movement->Velocity;
    justLanded = true;
    timerManager->SetTimer( landedHandle,
                            FTimerDelegate::CreateLambda(
                                [this] { justLanded = false; } ),
                            0.1f, false, 0.3f );
}
