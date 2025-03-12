// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class

void UGravMovementComponent::BeginPlay() {
    LastGravityToWorldTransform = GetGravityToWorldTransform();
    LastWorldToGravityTransform = GetWorldToGravityTransform();

    CharacterOwner->MovementModeChangedDelegate.AddUniqueDynamic(
        this, &UGravMovementComponent::MovementModeChanged );
}

void UGravMovementComponent::OnMovementUpdated( float DeltaSeconds, const FVector& OldLocation,
                                                const FVector& OldVelocity ) {
    Super::OnMovementUpdated( DeltaSeconds, OldLocation, OldVelocity );
    //...
}

void UGravMovementComponent::UpdateGravity() {
    if ( !gravIsDirty )
        return;

    if ( !CurrentFloor.HitResult.ImpactNormal.IsNearlyZero() ) {
        SetGravityDirection( CurrentFloor.HitResult.ImpactNormal * -1.f );

        hasUpdatedRotationForNewGravity = true;
        currentlyUpdatingRotation = false;
    }

    gravIsDirty = false;
}

void UGravMovementComponent::MovementModeChanged( ACharacter* Character,
                                                  EMovementMode PrevMovementMode,
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
        bool overLand = GetWorld()->LineTraceSingleByChannel( hitResult, start, end,
                                                              ECollisionChannel::ECC_Visibility );

        if ( overLand ) {
            DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 0.f, ( uint8 )0U, 2.f );

            FRotator newRotation = FMath::Lerp(
                currentRotation, desiredRotation,
                1.f - FMath::Clamp(
                          ( hitResult.Distance / ( 400.f ) ), 0.f, 1.f ) );

            CharacterOwner->SetActorRotation( newRotation );

            if ( !hasUpdatedRotationForNewGravity ) {
                hasUpdatedRotationForNewGravity = true;
                currentlyUpdatingRotation = true;
            }

            if ( ( hitResult.Distance / ( 400.f ) ) <= 0.f )
                currentlyUpdatingRotation = false;

        } else {
            DrawDebugLine( GetWorld(), start, end, FColor::Green, false, 0.f, ( uint8 )0U, 2.f );
        }
    }
}

void UGravMovementComponent::SetGravityDirection( const FVector& GravityDir ) {
    if ( FMath::IsNearlyZero( 1.f - FVector::DotProduct( GravityDir, GetGravityDirection() ) ) )
        return;

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Set gravity" );

    const FQuat WorldToNegativeGravityTransform = FQuat::FindBetweenNormals(
        FVector::UpVector, GravityDir );
    const FQuat NegativeGravityToWorldTransform = WorldToNegativeGravityTransform.Inverse();

    currentRotation = CharacterOwner->GetActorRotation();

    if ( hasUpdatedRotationForNewGravity ) {
        currentLastGravRotation =
            ( GetWorldToGravityTransform() * currentRotation.Quaternion() ).Rotator();
    }

    desiredRotation = ( NegativeGravityToWorldTransform *
                        ( currentLastGravRotation + FRotator( 0.f, 0.f, 0.f ) ).Quaternion() )
                          .Rotator();
    GEngine->AddOnScreenDebugMessage( -1, 20.f, FColor::Green, desiredRotation.ToString() );

    hasUpdatedRotationForNewGravity = false;

    gravIsDirty = true;

    Super::SetGravityDirection( GravityDir );
    //...
}
