// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionLook.generated.h"

class USceneComponent;
class USmartSpringArm;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionLook : public UAction {
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

public:
    UActionLook();
    virtual void Start( const FInputActionValue& value );

private:
    USceneComponent* gimbal;
    USmartSpringArm* springArm;
};
