// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionMove.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GravMovementComponent.h"
#include "Camera/CameraComponent.h" // UCameraComponent class

UActionMove::UActionMove() { type = EAction::A_Move; }

void UActionMove::BeginPlay() {
    Super::BeginPlay();
    //...

    gimbal = parent->GetGimbal();
    movement = Cast< UGravMovementComponent >( parent->GetCharacterMovement() );
}

void UActionMove::Start( const FInputActionValue& value ) {
    if ( !parent->GetCanMove() ) return;

    // Get and store most recent movement inputs
    const FVector2D inputValue = value.Get< FVector2D >();
    parent->SetLastMovementInput( FVector( inputValue.X, inputValue.Y, 0.0f ) );

    // Get the intended orientation of camera
    const FQuat& targetQuat = parent->GetTargetCameraOrientation();

    // Calculate the yaw angle of camera in radians
    float angle = FMath::DegreesToRadians( gimbal->GetRelativeRotation().Yaw );
    if ( angle < 0.f ) {
        angle = 2 * PI + angle;
    }

    // Calculate the rotation quaternion to apply yaw
    const FQuat addQuat =
        FQuat( targetQuat.GetUpVector().GetSafeNormal(), angle )
            .GetNormalized();

    // Apply yaw to camera orientation
    const FQuat outputQuat = addQuat * targetQuat;

    // Get forward and right vectors
    const FVector fwd = outputQuat.GetForwardVector();
    const FVector right = outputQuat.GetRightVector();

    if ( movement->MovementMode != MOVE_Flying ) {

        // Apply movement to character
        parent->AddMovementInput( fwd, inputValue.Y, false );
        parent->AddMovementInput( right, inputValue.X, false );
    } else {
        movement->AddImpulse( parent->GetCamera()->GetForwardVector() * 50.f,
                              true );
    }

    GEngine->AddOnScreenDebugMessage(
        -1, 0.f, FColor::Red,
        FString::SanitizeFloat( movement->Velocity.Length() ) );
}
