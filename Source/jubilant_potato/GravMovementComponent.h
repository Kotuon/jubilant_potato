// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class JUBILANT_POTATO_API UGravMovementComponent : public UCharacterMovementComponent {
    GENERATED_BODY()

public:
    virtual void OnMovementUpdated( float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity ) override;

    virtual void UpdateGravity();
};
