// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionCombat.generated.h"

class UAnimMontage;
struct FTimerHandle;
class UActionAim;
class AEnemy;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionCombat : public UAction {
    GENERATED_BODY()

protected: // Functions
    // Called when the game starts
    virtual void BeginPlay() override;

public: // Functions
    UActionCombat();
    virtual void Start( const FInputActionValue& Value );
    virtual void End();
    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

    UFUNCTION( BlueprintCallable )
    void SetCanCombo( bool CanCombo );

    UFUNCTION( BlueprintCallable )
    int GetAttackCount() const;

    UFUNCTION( BlueprintCallable )
    bool GetIsAttacking() const;

    UFUNCTION( BlueprintCallable )
    void SpawnProjectile( FVector SocketLocation, FRotator SocketNormal );

private: // Functions
    void EndCooldown();

public: // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
    TArray< UAnimMontage* > attack_montages;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
    UAnimMontage* jump_attack_montage;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectile" )
    TSubclassOf< class ABase_Projectile > projectile_class;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectile" )
    float start_distance_projectile = 100.f;

    UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "General" )
    float range = 2000.f;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    float cooldown = 0.1f;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    int damage_amount = 1;

private: // Variables
    UActionAim* aimAction;
    UWorld* world;

    AEnemy* currTarget = nullptr;

    FTimerHandle cooldownTimer;

    int attackCount = 0;
    int startComboCount = 0;

    bool canCombo = false;
    bool isAttacking = false;
    bool onCooldown = false;
};
