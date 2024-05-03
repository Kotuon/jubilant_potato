// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

class UAIPerceptionComponent;
struct FAIStimulus;

UCLASS()
class JUBILANT_POTATO_API AEnemyController : public AAIController {
    GENERATED_BODY()

protected: // Functions
    // Called when the game starts
    virtual void BeginPlay() override;

public: // Functions
    AEnemyController( const FObjectInitializer &ObjectInitializer );

    void OnPossess( APawn *InPawn ) override;
    void FindTarget();

    UFUNCTION()
    void OnPerceptionTargetUpdate( AActor *actor, FAIStimulus stimulus);

private: // Functions
    void StartEnemyTimer();

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Perception" )
    UAIPerceptionComponent *sight;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Perception" )
    float line_of_sight_timer = 4.f;

private: // Variables
    FTimerHandle find_target_time_handle;
    FTimerHandle check_los_countdown;

    UBlackboardComponent *blackboard;

    UWorld *world;

    APawn *controlledPawn;
};
