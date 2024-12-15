// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionAim.generated.h"

class USmartSpringArm;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionAim : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UActionAim();
    virtual void Start( const FInputActionValue& value );
    virtual void End();

    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    bool GetIsAiming() const;

public:  // Variables
private: // Variables
    USmartSpringArm* spring_arm;

    bool is_aiming = false;
};
