// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Sprint.generated.h"

class UCharacterMovementComponent;
class UAim;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API USprint : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    USprint();
    virtual void Start( const FInputActionValue& value );
    virtual void End();

    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float sprint_speed = 800.f;

private: // Variables
    UCharacterMovementComponent* character_movement;

    UAim* aim_action;

    float original_walk_speed;

    bool is_sprinting = false;
};
