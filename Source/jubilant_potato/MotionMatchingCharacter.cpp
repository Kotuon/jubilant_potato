
#include "MotionMatchingCharacter.h"

#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"  // UEnhancedInputComponent class
AMotionMatchingCharacter::AMotionMatchingCharacter() {
    PrimaryActorTick.bCanEverTick = true;
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
    float MaxMoveSpeed = CalculateMaxSpeed();

    movement->MaxWalkSpeed = MaxMoveSpeed;
    movement->MaxWalkSpeedCrouched = MaxMoveSpeed;
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
