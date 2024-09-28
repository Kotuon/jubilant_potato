// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionMatchingCharacter.generated.h"

UENUM( BlueprintType )
enum class EGait : uint8 {
    ERun,
    EWalk,
    ESprint
};

class UCharacterMovementComponent;

UCLASS()
class JUBILANT_POTATO_API AMotionMatchingCharacter : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AMotionMatchingCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

    UFUNCTION( BlueprintCallable )
    const FVector& GetLastMovementInput() const {
        return lastMovementInput;
    }

    UFUNCTION( BlueprintCallable )
    const FVector2D& GetLastCameraInput() const {
        return lastCameraInput;
    }

    void SetLastMovementZInput( const float inputValue ) {
        lastMovementInput.Z = inputValue;
    }

private: // Functions
    void UpdateMovement();
    void UpdateRotation();

    EGait GetDesiredGait() const;
    float CalculateMaxSpeed() const;

protected: // Variables
    FVector lastMovementInput;
    FVector2D lastCameraInput;

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
               Category = "MotionMatching" )
    UCurveFloat* strafeSpeedMapCurve;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
               Category = "MotionMatching" )
    FVector walkSpeeds = { 200.f, 175.f, 150.f };

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
               Category = "MotionMatching" )
    FVector runSpeeds = { 500.f, 350.f, 300.f };

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
               Category = "MotionMatching" )
    FVector sprintSpeeds = { 700.f, 700.f, 700.f };

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
               Category = "MotionMatching" )
    FVector crouchSpeeds = { 200.f, 150.f, 150.f };

    EGait gait;

    bool wantsToStrafe = false;
    bool wantsToSprint = false;
    bool wantsToWalk = false;

private: // Variables
    UCharacterMovementComponent* movement;
};
