// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class JUBILANT_POTATO_API AEnemyController : public AAIController {
    GENERATED_BODY()

public:
    AEnemyController( const FObjectInitializer &ObjectInitializer );

public:
    void OnPossess( APawn *InPawn ) override;
    void FindTarget();

private:
    FTimerHandle find_target_time_handle;
    APawn *controlledPawn;
};
