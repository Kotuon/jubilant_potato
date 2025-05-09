// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class
#include "GravPlayerCharacter.h"

void UGravMovementComponent::BeginPlay() {
    CharacterOwner->MovementModeChangedDelegate.AddUniqueDynamic(
        this, &UGravMovementComponent::MovementModeChanged );

    ProcessRootMotionPostConvertToWorld.BindUObject(
        this, &UGravMovementComponent::ProcessPostRootMotion );

    parent = Cast< AGravPlayerCharacter >( CharacterOwner );
}

void UGravMovementComponent::OnMovementUpdated( float DeltaSeconds,
                                                const FVector& OldLocation,
                                                const FVector& OldVelocity ) {
    Super::OnMovementUpdated( DeltaSeconds, OldLocation, OldVelocity );
    //...
}

FTransform UGravMovementComponent::ProcessPostRootMotion(
    const FTransform& WorldRootMotionTransform,
    UCharacterMovementComponent* movement, float DeltaSeconds ) {

    return WorldRootMotionTransform;
}

void UGravMovementComponent::UpdateGravity() {
    // if ( !gravIsDirty ) return;

    SetGravityToGround();

    gravIsDirty = false;
}

void UGravMovementComponent::SetGravityToGround() {
    if ( !CurrentFloor.HitResult.ImpactNormal.IsNearlyZero() ) {
        SetGravityDirection( CurrentFloor.HitResult.ImpactNormal * -1.f );

        hasUpdatedRotationForNewGravity = true;
        currentlyUpdatingRotation = false;
    }
}

void UGravMovementComponent::MovementModeChanged(
    ACharacter* Character, EMovementMode PrevMovementMode,
    uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling && MovementMode == MOVE_Walking )
        UpdateGravity();
}

bool UGravMovementComponent::ShouldRemainVertical() const {
    return IsMovingOnGround();
}

void UGravMovementComponent::UpdateRotation( float DeltaTime ) {
    if ( MovementMode == MOVE_Falling &&
         ( !hasUpdatedRotationForNewGravity || currentlyUpdatingRotation ) ) {

        // Search ray in direction of gravity
        FHitResult hitResult;
        const FVector start = GetActorLocation();
        const FVector end = start + GetGravityDirection() * 400.f;
        bool overLand = GetWorld()->LineTraceSingleByChannel(
            hitResult, start, end, ECollisionChannel::ECC_Visibility );

        // Rotate player model to match floor direction
        if ( overLand ) {
            if ( !hasStartedRotation ) {
                // Calculate delta rotaiton between current and new gravity
                const FQuat deltaRot = FQuat::FindBetweenVectors(
                                           -1.f * parent->GetActorUpVector(),
                                           GetGravityDirection() )
                                           .GetNormalized();

                currRotation = CharacterOwner->GetActorQuat();

                // Calculate new rotation
                desiredRotation = deltaRot * currRotation;

                hasStartedRotation = true;
            }

            DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 0.f,
                           ( uint8 )0U, 2.f );

            // Calculate rotation delta based on distance from floor
            const float alpha =
                1.f - FMath::Clamp( ( ( hitResult.Distance - 20.f ) / 400.f ),
                                    0.f, 1.f );

            // Get the new rotation
            const FQuat newRotation =
                FQuat::FastLerp( currRotation, desiredRotation, alpha )
                    .GetNormalized();

            // Update character rotation
            CharacterOwner->SetActorRotation( newRotation );

            // Update flags
            if ( !hasUpdatedRotationForNewGravity ) {
                hasUpdatedRotationForNewGravity = true;
                currentlyUpdatingRotation = true;
            }

            // Check if player has rotated enough
            if ( ( hitResult.Distance / ( 400.f ) ) <= 0.f )
                currentlyUpdatingRotation = false;

        } else {
            DrawDebugLine( GetWorld(), start, end, FColor::Green, false, 0.f,
                           ( uint8 )0U, 2.f );
        }
    }
}

void UGravMovementComponent::SetGravityDirection( const FVector& GravityDir ) {
    // Local reference to current gravity
    // const FVector& currGravityDir = GetGravityDirection();
    lastGrav = GetGravityDirection();

    // Checking if the gravity has changed
    if ( GravityDir.Equals( lastGrav ) ) return;

    hasStartedRotation = false;

    // // Calculate delta rotaiton between current and new gravity
    // const FQuat deltaRot =
    //     FQuat::FindBetweenVectors( currGravityDir, GravityDir
    //     ).GetNormalized();

    // currRotation = CharacterOwner->GetActorQuat();

    // // Calculate new rotation
    // desiredRotation = deltaRot * currRotation;

    // Update flags
    hasUpdatedRotationForNewGravity = false;
    gravIsDirty = true;

    Super::SetGravityDirection( GravityDir );
    //...
}
