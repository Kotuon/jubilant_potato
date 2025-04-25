// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.generated.h"

class UInputAction;

USTRUCT()
struct FAttackComboInfo {
    GENERATED_USTRUCT_BODY()

    bool operator==( const FAttackComboInfo& Other ) const {
        return attackMontage == Other.attackMontage;
    }

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    UAnimMontage* attackMontage;

    UPROPERTY( EditAnywhere, Category = "AttackInfo",
               meta = ( ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0",
                        UIMax = "2.0" ) )
    float hitSize = 0.1f;

    UPROPERTY( EditAnywhere, Category = "AttackInfo",
               meta = ( ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0" ) )
    float hitRange = 200.f;

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    float hitDamage = 1.f;
};

USTRUCT() struct FAttackInfo {
    GENERATED_USTRUCT_BODY()

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    TArray< FAttackComboInfo > attacks;

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    TObjectPtr< UInputAction > InputAction;

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    float comboWindow = 0.4f;

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    float cooldown = 0.1f;

    UPROPERTY( EditAnywhere, Category = "AttackInfo" )
    bool hasTriggerConditions = false;

    UPROPERTY( EditAnywhere, Category = "AttackInfo|Conditions",
               meta = ( EditCondition = "hasTriggerConditions" ) )
    bool hasVelocityCondition = false;

    UPROPERTY( EditAnywhere, Category = "AttackInfo|Conditions",
               meta = ( EditCondition = "hasVelocityCondition" ) )
    float minVelocity = 0.f;

    UPROPERTY( EditAnywhere, Category = "AttackInfo|Conditions",
               meta = ( EditCondition = "hasTriggerConditions" ) )
    bool hasInAirCondition = false;

    UPROPERTY( VisibleAnywhere, Category = "AttackInfo" )
    int32 InputID;

    bool IsValid() const { return attacks[0].attackMontage && InputAction; }

    bool operator==( const FAttackInfo& Other ) const {
        return attacks[0] == Other.attacks[0] &&
               InputAction == Other.InputAction;
    }

    bool operator!=( const FAttackInfo& Other ) const {
        return !operator==( Other );
    }

    friend uint32 GetTypeHash( const FAttackInfo& Item ) {
        return GetTypeHash( Item.InputAction );
    }

    FAttackInfo() : InputID( INDEX_NONE ) {}
};

UCLASS()
class JUBILANT_POTATO_API UAttackData : public UDataAsset {
    GENERATED_UCLASS_BODY()

protected:
    UPROPERTY( EditAnywhere, Category = "Attacks" )
    TSet< FAttackInfo > Attacks;

public:
    const TSet< FAttackInfo >& GetAttackInfo() const;

    // #if WITH_EDITOR
    //     virtual void PostEditChangeProperty(
    //         FPropertyChangedEvent& PropertyChangedEvent ) override;
    // #endif
};
