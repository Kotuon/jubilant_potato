// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class

void UGravMovementComponent::BeginPlay() {
    CharacterOwner->MovementModeChangedDelegate.AddUniqueDynamic(
        this, &UGravMovementComponent::MovementModeChanged );
}

void UGravMovementComponent::OnMovementUpdated( float DeltaSeconds, const FVector& OldLocation,
                                                const FVector& OldVelocity ) {
    Super::OnMovementUpdated( DeltaSeconds, OldLocation, OldVelocity );
    //...
}

void UGravMovementComponent::UpdateGravity() {
    if ( !CurrentFloor.HitResult.ImpactNormal.IsZero() ) {
        SetGravityDirection( CurrentFloor.HitResult.ImpactNormal * -1.f );
    }
}

void UGravMovementComponent::MovementModeChanged( ACharacter* Character,
                                                  EMovementMode PrevMovementMode,
                                                  uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling && MovementMode == MOVE_Walking ) {
        UpdateGravity();
    }
}
