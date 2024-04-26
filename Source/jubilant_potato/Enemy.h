// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UUserWidget;

UCLASS()
class JUBILANT_POTATO_API AEnemy : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AEnemy();

    virtual void Tick( float DeltaTime ) override;

    UFUNCTION( BlueprintCallable )
    void StartTarget();

    UFUNCTION( BlueprintCallable )
    void EndTarget();

private: // Functions
public:  // Variables
private: // Variables
    UUserWidget *target_marker;
};
