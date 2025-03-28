// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetSystem.generated.h"

class AEnemy;
class APlayerCharacter;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UTargetSystem : public UActorComponent {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UTargetSystem();

    virtual void
    TickComponent( float DeltaTime, ELevelTick TickType,
                   FActorComponentTickFunction* ThisTickFunction ) override;

    UFUNCTION( BlueprintCallable )
    void UpdateTarget( float Width = 0.1f, float Range = 500.f,
                                     bool SingleTarget = true );

    UFUNCTION( BlueprintCallable )
    TArray< AEnemy* >& GetTargets();

    UFUNCTION( BlueprintCallable )
    void ClearTargets();

private: // Functions
public:  // Variables
private: // Variables
    TArray< AEnemy* > curr_targets;
    APlayerCharacter* parent;

    UWorld* world;
};
