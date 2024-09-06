// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "GravRush.generated.h"

class UCharacterMovementComponent;
class UEnhancedInputComponent;
class UInputAction;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UGravRush : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UGravRush();
    virtual void Start( const FInputActionValue& Value );
    virtual void End();
    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode, uint8 PrevCustomMode );

    virtual void BindAction( UEnhancedInputComponent* PEI ) override;

private: // Functions
public:  // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    UInputAction* cancel_action;

private: // Variables
    UCharacterMovementComponent* movement;
    FVector original_grav;

    bool has_clicked = false;
};
