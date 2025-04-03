// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionCombat.generated.h"

struct FTimerHandle;
class UAnimMontage;
class UActionAim;
class AEnemy;
class UGravMovementComponent;
class UTargetSystem;
class UAttackData;
class UEnhancedInputComponent;
struct FAttackInfo;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionCombat : public UAction {
    GENERATED_BODY()

protected: // Functions
    // Called when the game starts
    virtual void BeginPlay() override;

public: // Functions
    UActionCombat();

    virtual void BindAction( UEnhancedInputComponent* PEI ) override;

    virtual void Start( const FInputActionValue& Value );
    virtual void End();
    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

    UFUNCTION( BlueprintCallable )
    void SetCanCombo( bool CanCombo );

    UFUNCTION( BlueprintCallable )
    void StartCanComboWindow();

    UFUNCTION( BlueprintCallable )
    void EndCanComboWindow();

    UFUNCTION( BlueprintCallable )
    int GetAttackCount() const;

    UFUNCTION( BlueprintCallable )
    bool GetIsAttacking() const;

    UFUNCTION( BlueprintCallable )
    void HitTargets();

private: // Functions
    void EndCooldown();
    void TargetEnemy( AEnemy* Target );
    void UpdatePlayerRotation();

public: // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    int damage_amount = 1;

    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attacks",
               meta = ( AllowPrivateAccess = "true" ) )
    TObjectPtr< UAttackData > AttackDataAsset;

private: // Variables
    TArray< FAttackInfo > AttackList;

    UActionAim* aimAction;
    UWorld* world;

    UGravMovementComponent* movement;
    UTargetSystem* targetSystem;

    AEnemy* currTarget = nullptr;

    FTimerHandle comboWindowTimer;
    FTimerHandle cooldownTimer;

    int currAttackId = -1;

    int attackCount = 0;
    int startComboCount = 0;
    int startComboId = 0;

    bool canCombo = false;
    bool isAttacking = false;
    bool onCooldown = false;
};
