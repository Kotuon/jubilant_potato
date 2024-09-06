// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"

void UGravMovementComponent::OnMovementUpdated( float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity ) {
    Super::OnMovementUpdated( DeltaSeconds, OldLocation, OldVelocity );
    //...

    UpdateGravity();
}

void UGravMovementComponent::UpdateGravity() {
    if ( !CurrentFloor.HitResult.ImpactNormal.IsZero() ) {
        SetGravityDirection( CurrentFloor.HitResult.ImpactNormal * -1.f );
    }
}
