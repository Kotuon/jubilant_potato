// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionMatchingCharacter.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UDefaultASC;
class UAbilitySystemComponent;
class UPlayerGameplayAbilitiesDataAsset;
class UAbilitySystemComponent;

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public AMotionMatchingCharacter,
                                             public IAbilitySystemInterface {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    APlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

    void PossessedBy( AController* NewController ) override;

    void AbilityInputPressed( int32 InputID );
    void AbilityInputReleased( int32 InputID );

    virtual UAbilitySystemComponent*
    GetAbilitySystemComponent() const override;

    FORCEINLINE UPlayerGameplayAbilitiesDataAsset*
    GetPlayerGameplayAbilitiesDataAsset() const {
        return PlayerGameplayAbilitiesDataAsset;
    }

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* input_mapping;

    // Input Actions
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction* input_move;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction* input_look;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

    UPROPERTY( EditDefaultsOnly, Category = "Gameplay Abilities" )
    UDefaultASC* AbilitySystemComponent;

    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem",
        meta = ( AllowPrivateAccess = "true" ) )
    TObjectPtr< UPlayerGameplayAbilitiesDataAsset >
    PlayerGameplayAbilitiesDataAsset;

    UPROPERTY()
    class UJPAttributeSet* AttributeSet;

private: // Functions
    void Move( const FInputActionValue& value );
    void Look( const FInputActionValue& value );
    void InitAbilitySystem();

private: // Variables
};