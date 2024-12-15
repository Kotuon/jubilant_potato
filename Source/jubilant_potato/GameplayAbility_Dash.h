// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Dash.generated.h"

/**
 *
 */
UCLASS()
class JUBILANT_POTATO_API UGameplayAbility_Dash : public UGameplayAbility {
    GENERATED_UCLASS_BODY()

public:
    virtual bool
    CanActivateAbility( const FGameplayAbilitySpecHandle Handle,
                        const FGameplayAbilityActorInfo* ActorInfo,
                        const FGameplayTagContainer* SourceTags = nullptr,
                        const FGameplayTagContainer* TargetTags = nullptr,
                        OUT FGameplayTagContainer* OptionalRelevantTags =
                            nullptr ) const override;

    virtual void
    ActivateAbility( const FGameplayAbilitySpecHandle Handle,
                     const FGameplayAbilityActorInfo* ActorInfo,
                     const FGameplayAbilityActivationInfo ActivationInfo,
                     const FGameplayEventData* TriggerEventData ) override;

    virtual void InputReleased(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo ) override;

    virtual void
    CancelAbility( const FGameplayAbilitySpecHandle Handle,
                   const FGameplayAbilityActorInfo* ActorInfo,
                   const FGameplayAbilityActivationInfo ActivationInfo,
                   bool bReplicateCancelAbility ) override;
};
