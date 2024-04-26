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

    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

    UFUNCTION( BlueprintCallable )
    void UpdateTarget();

private: // Functions
public:  // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    float detection_size = 5.f;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
    float detection_range = 500.f;

private: // Variables
    AEnemy *curr_target = nullptr;
    APlayerCharacter *parent;

    UWorld *world;

    float search_value;
};
