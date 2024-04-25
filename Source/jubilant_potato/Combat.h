// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Combat.generated.h"

class UAnimMontage;
struct FTimerHandle;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UCombat : public UAction {
    GENERATED_BODY()

protected: // Functions
    // Called when the game starts
    virtual void BeginPlay() override;

public: // Functions
    UCombat();
    virtual void Start( const FInputActionValue &value );
    virtual void End();
    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

    UFUNCTION( BlueprintCallable )
    void SetCanCombo( bool canCombo );

    UFUNCTION( BlueprintCallable )
    int GetAttackCount() const;

    UFUNCTION( BlueprintCallable )
    bool GetIsAttacking() const;

private: // Functions
    void EndCooldown();

public: // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
    TArray< UAnimMontage * > attack_montages;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    float cooldown = 0.5f;

private: // Variables
    FTimerHandle cooldown_timer;

    int attack_count = 0;
    int start_combo_count = 0;

    bool can_combo = false;
    bool is_attacking = false;
    bool on_cooldown = false;
};
