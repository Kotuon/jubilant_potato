// Fill out your copyright notice in the Description page of Project Settings.

#include "NoRestrictPlayerCameraManager.h"

void ANoRestrictPlayerCameraManager::ProcessViewRotation( float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot ) {
}

void ANoRestrictPlayerCameraManager::LimitViewPitch( FRotator&, float, float ) {
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Limit pitch." );
}

void ANoRestrictPlayerCameraManager::LimitViewRoll( FRotator&, float, float ) {
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Limit roll." );
}

void ANoRestrictPlayerCameraManager::LimitViewYaw( FRotator&, float, float ) {
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Limit yaw." );
}
