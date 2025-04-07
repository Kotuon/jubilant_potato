// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class
#include "GravPlayerCharacter.h"

void UGravMovementComponent::BeginPlay() {
    LastGravityToWorldTransform = GetGravityToWorldTransform();
    LastWorldToGravityTransform = GetWorldToGravityTransform();

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
    // GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
    //                                   "ROOT MOTION TEST" );

    // GEngine->AddOnScreenDebugMessage(
    //     -1, 0.f, FColor::Cyan,
    //     WorldRootMotionTransform.GetRotation().ToString() );

    // FTransform newTransform = WorldRootMotionTransform;
    FTransform newTransform( FVector::ZeroVector );

    // FQuat cameraRot = Cast< AGravPlayerCharacter >( CharacterOwner )
    //                       ->cameraRoot->GetComponentRotation()
    //                       .Quaternion().GetNormalized();

    // cameraRot.
    ///////////////////////////////////
    newTransform.AddToTranslation( WorldRootMotionTransform.GetTranslation() );

    // MoveUpdatedComponent( FVector::ZeroVector, -desiredRotation, true );

    // newTransform.SetRotation(
    // FQuat::MakeFromEuler( FVector( 1.f, 0.f, 0.f ) ) );
    ////////////////////////////////////
    // newTransform.ConcatenateRotation(
    // FQuat::MakeFromEuler( FVector( 0.f, 0.f, 90.f ) ) );

    // newTransform.ConcatenateRotation( desiredRotation );
    // newTransform.ConcatenateRotation( -UpdatedComponent->GetComponentQuat()
    // );

    // newTransform.SetRotation( WorldRootMotionTransform.GetRotation() );
    // newTransform.SetRotation( desiredRotation );
    // newTransform.SetRotation( Cast< AGravPlayerCharacter >(
    // CharacterOwner )
    //                               ->cameraRoot->GetComponentRotation()
    //                               .Quaternion() );

    // MoveUpdatedComponent

    // newTransform.SetRotation( desiredRotation *
    //   -GetLastUpdateRotation().Quaternion() );

    // return newTransform;
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

            FQuat newRotation = FQuat::FastLerp(
                currentRotation, desiredRotation,
                1.f -
                    FMath::Clamp( ( ( hitResult.Distance - 20.f ) / ( 400.f ) ),
                                  0.f, 1.f ) );

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
    if ( FMath::IsNearlyZero(
             1.f - FVector::DotProduct( GravityDir, GetGravityDirection() ) ) )
        return;

    // GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Set gravity" );

    const FQuat WorldToNegativeGravityTransform =
        FQuat::FindBetweenNormals( FVector::UpVector, GravityDir );
    LastWorldToGravityTransform = WorldToNegativeGravityTransform;

    const FQuat NegativeGravityToWorldTransform =
        WorldToNegativeGravityTransform.Inverse();
    LastGravityToWorldTransform = NegativeGravityToWorldTransform;

    currentRotation = CharacterOwner->GetActorQuat();

    if ( hasUpdatedRotationForNewGravity ) {
        currentLastGravRotation =
            ( GetWorldToGravityTransform() * currentRotation );
    }

    desiredRotation =
        ( NegativeGravityToWorldTransform * currentLastGravRotation );

    // GEngine->AddOnScreenDebugMessage( -1, 20.f, FColor::Green,
    //                                   desiredRotation.ToString() );

    hasUpdatedRotationForNewGravity = false;

    gravIsDirty = true;

    Super::SetGravityDirection( GravityDir );
    //...
}

const FQuat& UGravMovementComponent::GetLastGravityToWorldTransform() const {
    return LastGravityToWorldTransform;
}

const FQuat& UGravMovementComponent::GetLastWorldToGravityTransform() const {
    return LastWorldToGravityTransform;
}
