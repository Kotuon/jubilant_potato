
#include "GravPlayerCharacter.h"
#include "GravMovementComponent.h"  // UGravMovementComponent
#include "SmartSpringArm.h"         // USmartSpringArm class
#include "Camera/CameraComponent.h" // UCameraComponent class

AGravPlayerCharacter::AGravPlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : ACharacter(
          ObjectInitializer.SetDefaultSubobjectClass< UGravMovementComponent >(
              ACharacter::CharacterMovementComponentName ) ) {
    PrimaryActorTick.bCanEverTick = true;

    cameraRoot = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "CameraRoot" ) ) );
    cameraRoot->SetupAttachment( GetRootComponent() );
    gimbal = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "Gimbal" ) ) );
    gimbal->SetupAttachment( cameraRoot );
    springArm = Cast< USmartSpringArm >(
        CreateDefaultSubobject< USmartSpringArm >( FName( "SpringArm" ) ) );
    springArm->SetupAttachment( gimbal );
    camera = CreateDefaultSubobject< UCameraComponent >( FName( "Camera" ) );
    camera->SetupAttachment( springArm );

    MovementModeChangedDelegate.AddUniqueDynamic(
        this, &AGravPlayerCharacter::MovementModeChanged );
}

void AGravPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    movement = Cast< UGravMovementComponent >( GetCharacterMovement() );
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    movement->UpdateRotation( DeltaTime );

    if ( !canUpdateCamera ) return;

    const FVector inverseGravity = movement->GetGravityDirection() * -1.f;
    const FVector gimbalUp = gimbal->GetUpVector();

    const float result = FVector::DotProduct( inverseGravity, gimbalUp );

    // If the camera is not aligned with the new gravity
    // if ( result < 0.9999999f ) {
    if ( !FMath::IsNearlyEqual( result, 1.f ) ) {
        // If camera hasn't started updating
        if ( !updatingCamera ) {
            updatingCamera = true;
            targetUp = inverseGravity;
            targetRot = GetActorQuat();
        } else {
            const float update_result =
                FVector::DotProduct( inverseGravity, targetUp );

            // Check if the gravity has changed since starting update
            if ( FMath::IsNearlyEqual( update_result, 1.f ) ) {
                targetUp = inverseGravity;
                targetRot = GetActorQuat();
            }
        }

        // Setup quaternions for lerp
        const FQuat startRot = cameraRoot->GetComponentQuat();
        const FQuat endRot = targetRot;

        // Update camera root rotation with lerp
        cameraRoot->SetWorldRotation(
            FQuat::FastLerp( startRot, endRot, 12.0 * DeltaTime ) );
    } else {
        updatingCamera = false;
        canUpdateCamera = false;
    }
}

void AGravPlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void AGravPlayerCharacter::MovementModeChanged( ACharacter* Character,
                                                EMovementMode PrevMovementMode,
                                                uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling &&
         movement->MovementMode == MOVE_Walking ) {
        canUpdateCamera = true;
    }
}

void AGravPlayerCharacter::SetCanUpdateCamera( bool Value ) {
    canUpdateCamera = Value;
}

const FQuat& AGravPlayerCharacter::GetTargetQuat() const { return targetRot; }
