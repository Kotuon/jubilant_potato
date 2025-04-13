// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class
#include "GravPlayerCharacter.h"

void UGravMovementComponent::BeginPlay() {
    // LastGravityToWorldTransform = GetGravityToWorldTransform();
    // LastWorldToGravityTransform = GetWorldToGravityTransform();

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
        FHitResult hitResult;
        const FVector start = GetActorLocation();
        const FVector end = start + GetGravityDirection() * 400.f;
        bool overLand = GetWorld()->LineTraceSingleByChannel(
            hitResult, start, end, ECollisionChannel::ECC_Visibility );

        if ( overLand ) {
            DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 0.f,
                           ( uint8 )0U, 2.f );

            const float alpha =
                1.f - FMath::Clamp( ( ( hitResult.Distance - 20.f ) / 400.f ),
                                    0.f, 1.f );

            const FQuat newRotation =
                FQuat::FastLerp( currRotation, desiredRotation, alpha )
                    .GetNormalized();

            CharacterOwner->SetActorRotation( newRotation );

            if ( !hasUpdatedRotationForNewGravity ) {
                hasUpdatedRotationForNewGravity = true;
                currentlyUpdatingRotation = true;
            }

            if ( ( hitResult.Distance / ( 400.f ) ) <= 0.f )
                currentlyUpdatingRotation = false;

        } else {
            DrawDebugLine( GetWorld(), start, end, FColor::Green, false, 0.f,
                           ( uint8 )0U, 2.f );
        }
    }
}

void UGravMovementComponent::SetGravityDirection( const FVector& GravityDir ) {
    const FVector& currGravityDir = GetGravityDirection();

    if ( GravityDir.Equals( currGravityDir ) ) return;

    // const FQuat WorldToNegativeGravityTransform =
    //     FQuat::FindBetweenNormals( FVector::UpVector, GravityDir );

    // const FQuat NegativeGravityToWorldTransform =
    //     WorldToNegativeGravityTransform.Inverse();

    // currRotation = CharacterOwner->GetActorQuat();

    // if ( hasUpdatedRotationForNewGravity ) {
    //     currLastGravRotation = ( GetWorldToGravityTransform() * currRotation
    //     );
    // }

    // desiredRotation =
    //     ( NegativeGravityToWorldTransform * currLastGravRotation );

    const FQuat deltaRot =
        FQuat::FindBetweenVectors( currGravityDir, GravityDir ).GetNormalized();

    currRotation = CharacterOwner->GetActorQuat();

    desiredRotation = deltaRot * currRotation;

    hasUpdatedRotationForNewGravity = false;
    gravIsDirty = true;

    Super::SetGravityDirection( GravityDir );
    //...
}

// const FQuat& UGravMovementComponent::GetLastGravityToWorldTransform() const {
//     return LastGravityToWorldTransform;
// }

// const FQuat& UGravMovementComponent::GetLastWorldToGravityTransform() const {
//     return LastWorldToGravityTransform;
// }
