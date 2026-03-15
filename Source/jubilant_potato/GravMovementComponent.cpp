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

void UGravMovementComponent::StartRotation() {
    // Calculate delta rotaiton between current and new gravity
    const FQuat deltaRot =
        FQuat::FindBetweenVectors( -1.f * parent->GetActorUpVector(),
                                   GetGravityDirection() )
            .GetNormalized();

    currRotation = CharacterOwner->GetActorQuat();

    // Calculate new rotation
    desiredRotation = deltaRot * currRotation;

    timer = 0.f;
    hasStartedRotation = true;
}

void UGravMovementComponent::SetNewRotation( const float alpha ) {
    // Get the new rotation
    const FQuat newRotation =
        FQuat::FastLerp( currRotation, desiredRotation, alpha ).GetNormalized();

    // Update character rotation
    CharacterOwner->SetActorRotation( newRotation );
}

void UGravMovementComponent::UpdateRotationFlags() {
    // Update flags
    if ( !hasUpdatedRotationForNewGravity ) {
        hasUpdatedRotationForNewGravity = true;
        currentlyUpdatingRotation = true;
    }
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

        if ( overLand && !hasStartedRotation ) {
            timeToRotate = 0.25f;
        } else if ( !hasStartedRotation ) {
            timeToRotate = 1.f;
        }

        { // DEBUG ///////////////////////////////////
            DrawDebugLine( GetWorld(), start, end, FColor::Green, false, 0.f,
                           ( uint8 )0U, 2.f );
            ///////////////////////////////////////////}
        }

        if ( !hasStartedRotation ) StartRotation();

        timer += DeltaTime;

        const float alpha = FMath::Clamp( timer / timeToRotate, 0.f, 1.f );

        SetNewRotation( alpha );

        UpdateRotationFlags();

        if ( timer >= timeToRotate ) currentlyUpdatingRotation = false;
    }
}

void UGravMovementComponent::SetGravityDirection( const FVector& GravityDir ) {
    // Local reference to current gravity
    lastGrav = GetGravityDirection();

    // Checking if the gravity has changed
    if ( GravityDir.Equals( lastGrav ) ) return;

    hasStartedRotation = false;

    // Update flags
    hasUpdatedRotationForNewGravity = false;
    gravIsDirty = true;

    Super::SetGravityDirection( GravityDir );
    //...
}
