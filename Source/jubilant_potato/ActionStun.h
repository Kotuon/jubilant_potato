// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionStun.generated.h"

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionStun : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UActionStun();
    virtual void Start( const FInputActionValue& Value );
    virtual void End();

    UFUNCTION( BlueprintCallable )
    void TriggerDamage();

private: // Functions
    void EndCooldown();

public: // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
    UAnimMontage* stunAttackMontage;
    // UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "VFX" )
    // class UNiagaraSystem* effect;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "VFX" )
    TSubclassOf< class AActor > effect_class;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float cooldown = 2.f;

    UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "General" )
    float range = 1000.f;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    float stunDuration = 1.f;

private: // Variables
    UWorld* world;

    FTimerHandle cooldownTimer;

    float OriginalJumpSpeed;

    bool onCooldown = false;
};
