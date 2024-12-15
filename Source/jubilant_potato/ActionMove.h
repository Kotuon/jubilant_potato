// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionMove.generated.h"

class USceneComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionMove : public UAction {
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

public:
    UActionMove();
    virtual void Start( const FInputActionValue& value );

private:
    USceneComponent* gimbal;
};
