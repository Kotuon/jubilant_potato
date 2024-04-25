// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Aim.generated.h"

class USmartSpringArm;

UCLASS()
class JUBILANT_POTATO_API UAim : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UAim();
    virtual void Start( const FInputActionValue &value );
    virtual void End();

    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

public:  // Variables
private: // Variables
    USmartSpringArm *spring_arm;
};
