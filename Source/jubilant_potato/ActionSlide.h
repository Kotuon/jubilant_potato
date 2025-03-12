// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionSlide.generated.h"

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionSlide : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UActionSlide();
    virtual void Start( const FInputActionValue& value );
    virtual void End();

    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
    float speed = 1000.f;

private: // Variables
    bool isRunning = false;
};
