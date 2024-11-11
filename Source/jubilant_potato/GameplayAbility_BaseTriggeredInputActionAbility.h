#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_BaseTriggeredInputActionAbility.generated.h"

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class JUBILANT_POTATO_API UGameplayAbility_BaseTriggeredInputActionAbility :
    public
    UGameplayAbility {
    GENERATED_UCLASS_BODY()
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Ability" )
    bool bCancelAbilityOnInputReleased;

protected:
    UPROPERTY()
    TObjectPtr< UEnhancedInputComponent > EnhancedInputComponent;

    TArray< uint32 > TriggeredEventHandles;

    //~ Begin UGameplayAbility Interface
    virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData*
                                  TriggerEventData ) override;
    virtual void EndAbility( const FGameplayAbilitySpecHandle Handle,
                             const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo
                             ActivationInfo, bool bReplicateEndAbility,
                             bool bWasCancelled ) override;

public:
    virtual void InputReleased( const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo
                                ActivationInfo ) override;
    //~ End UGameplayAbility Interface

protected:
    //~ Begin UGameplayAbility_BaseTriggeredInputActionAbility Interface
    virtual void OnTriggeredInputAction( const FInputActionValue& Value );
    //~ End UGameplayAbility_BaseTriggeredInputActionAbility Interface
};