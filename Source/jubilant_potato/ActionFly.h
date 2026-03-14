// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionFly.generated.h"

class UGravMovementComponent;
class UWorld;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionFly : public UAction {
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

public:
    UActionFly();
    virtual void Start( const FInputActionValue& Value );
    virtual void End();
    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

private:
    UWorld* world;
    UGravMovementComponent* movement;

    bool flying = false;
};
