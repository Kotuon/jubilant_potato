// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionMatchingCharacter.generated.h"

UENUM( BlueprintType )
enum class EGait : uint8 {
    ERun,
    EWalk
};

class UCharacterMovementComponent;

UCLASS()
class JUBILANT_POTATO_API AMotionMatchingCharacter : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AMotionMatchingCharacter();

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

private: // Functions
    void UpdateMovement();
    void UpdateRotation();

    EGait GetDesiredGait() const;
    float CalculateMaxSpeed() const;

public: // Variables
    EGait gait;
    bool wantsToStrafe = false;

private: // Variables
    UCharacterMovementComponent* movemnet;
};
