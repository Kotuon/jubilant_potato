// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionDash.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionDash : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UActionDash();
    virtual void Start( const FInputActionValue& value );
    virtual void End();

    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float distance = 1500.f;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float dashDuration = 0.1f;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float exitSpeed = 800.f;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    class UNiagaraSystem* effect;

private: // Variables
    UWorld* world;
    USceneComponent* gimbal;

    FVector endLocation;
    FVector startLocation;
    float dashTimer;

    bool isRunning = false;
};
