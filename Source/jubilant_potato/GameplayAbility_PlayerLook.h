// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_BaseTriggeredInputActionAbility.h"
#include "GameplayAbility_PlayerLook.generated.h"

/**
 *
 */
UCLASS()
class JUBILANT_POTATO_API UGameplayAbility_PlayerLook
    : public UGameplayAbility_BaseTriggeredInputActionAbility {
    GENERATED_BODY()

protected: // Functions
    void OnTriggeredInputAction( const FInputActionValue& Value );
};
