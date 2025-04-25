// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "GravRush.generated.h"

class UGravMovementComponent;
class UEnhancedInputComponent;
class UInputAction;
class UCameraComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UGravRush : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UGravRush();
    virtual void Start( const FInputActionValue& Value );
    virtual void End();
    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character,
                              EMovementMode PrevMovementMode,
                              uint8 PrevCustomMode );

    virtual void InvertGrav();
    virtual void LeftGrav();
    virtual void RightGrav();
    virtual void BackGrav();

    virtual void GroundGrav();

    virtual void BindAction( UEnhancedInputComponent* PEI ) override;

private: // Functions
    void TriggerGravShift( const FVector direction );
    void ResourceEmpty();

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* invertAction;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* leftAction;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* rightAction;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* backAction;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* cancelAction;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* groundAction;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    float startCost = 20.f;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    float tickCost = 1.f;

private: // Variables
    UCameraComponent* camera;

    UGravMovementComponent* movement;
    FVector originalGrav;

    FTimerHandle resourceCostTimer;

    bool hasClicked = false;
    bool groundGrav = false;
};
