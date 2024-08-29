// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "NoRestrictPlayerCameraManager.generated.h"

/**
 *
 */
UCLASS()
class JUBILANT_POTATO_API ANoRestrictPlayerCameraManager : public APlayerCameraManager {
    GENERATED_BODY()

public: // Functions
    virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

    virtual void LimitViewPitch( FRotator& ViewRotation, float InViewPitchMin, float InViewPitchMax ) override;

    virtual void LimitViewRoll( FRotator& ViewRotation, float InViewRollMin, float InViewRollMax ) override;

    virtual void LimitViewYaw( FRotator& ViewRotation, float InViewYawMin, float InViewYawMax ) override;
};
