
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

    // Create and attach each piece of the camera system
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

    // Add listener for changing movement mode
    MovementModeChangedDelegate.AddUniqueDynamic(
        this, &AGravPlayerCharacter::MovementModeChanged );

    // Init target camera rotation to current rotation
    targetCameraOrientation = cameraRoot->GetComponentQuat();
}

void AGravPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    // Store the movement component
    gravMovement = Cast< UGravMovementComponent >( GetCharacterMovement() );
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    // Update player rotation to match gravity
    gravMovement->UpdateRotation( DeltaTime );

    // Update camera orientation if allowed
    if ( canUpdateCamera ) {
        UpdateCameraOrientation( DeltaTime );
    }
}

void AGravPlayerCharacter::UpdateCameraOrientation( float DeltaTime ) {
    const FVector gravity = gravMovement->GetGravityDirection();
    const FVector inverseGravity = gravMovement->GetGravityDirection() * -1.f;
    const FVector rootUp = cameraRoot->GetUpVector();

    const float result = FVector::DotProduct( inverseGravity, rootUp );

    // If the camera is aligned with the new gravity
    if ( FMath::IsNearlyEqual( result, 1.f ) ) {
        updatingCamera = false;
        canUpdateCamera = false;

        return;
    }

    // Get current camera rotation
    const FQuat startRot = cameraRoot->GetComponentQuat();

    // If camera hasn't started updating
    if ( !updatingCamera ) {
        updatingCamera = true;
        targetUp = inverseGravity;

        // Calculate difference between two rotations
        const FQuat addQuat = FQuat::FindBetweenVectors(
                                  cameraRoot->GetUpVector(), inverseGravity )
                                  .GetNormalized();

        // Apply change to start rotation
        targetCameraOrientation = addQuat * startRot;
        targetCameraOrientation.Normalize();
    } else {
        const float updateResult =
            FVector::DotProduct( inverseGravity, targetUp );

        // Check if the gravity has changed since starting update
        if ( FMath::IsNearlyEqual( updateResult, 1.f ) ) {
            targetUp = inverseGravity;

            // Calculate difference between two rotations
            const FQuat addQuat =
                FQuat::FindBetweenVectors( cameraRoot->GetUpVector(),
                                           inverseGravity )
                    .GetNormalized();

            // Apply change to start rotation
            targetCameraOrientation = addQuat * startRot;
            targetCameraOrientation.Normalize();
        }
    }

    // Update camera root rotation with lerp
    cameraRoot->SetWorldRotation( FQuat::FastLerp(
        startRot.GetNormalized(), targetCameraOrientation, 12.0 * DeltaTime ) );
}

void AGravPlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void AGravPlayerCharacter::MovementModeChanged( ACharacter* Character,
                                                EMovementMode PrevMovementMode,
                                                uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling &&
         gravMovement->MovementMode == MOVE_Walking ) {
        canUpdateCamera = true;
    }
}

void AGravPlayerCharacter::SetCanUpdateCamera( bool Value ) {
    canUpdateCamera = Value;
}

const FQuat& AGravPlayerCharacter::GetTargetCameraOrientation() const {
    return targetCameraOrientation;
}
