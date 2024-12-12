// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayAbilityTask_Aim.generated.h"

class UGameplayAbility_Aim;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnTickTaskDelegate, float,
                                             DeltaTime );


UCLASS()
class JUBILANT_POTATO_API UGameplayAbilityTask_Aim : public UAbilityTask {
    GENERATED_BODY()

    UPROPERTY( BlueprintAssignable )
    FOnTickTaskDelegate OnTick;

public:
    UGameplayAbilityTask_Aim( const FObjectInitializer& ObjectInitializer );

    UFUNCTION( BlueprintCallable, Category = "Ability|Tasks",
               meta = ( HidePin = "OwningAbility",
                        DefaultToSelf = "OwningAbility",
                        BlueprintInternalUseOnly = "TRUE" ) )
    static UGameplayAbilityTask_Aim*
    AbilityTaskOnTick( UGameplayAbility* OwningAbility,
                       FName TaskInstanceName );

    virtual void Activate() override;
    virtual void TickTask( float DeltaTime ) override;

private:
    float TaskDeltaTime;
};
