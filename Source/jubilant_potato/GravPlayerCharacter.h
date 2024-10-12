// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionMatchingCharacter.h"
#include "GravPlayerCharacter.generated.h"

class USmartSpringArm;
class UCameraComponent;
class UGravMovementComponent;

UCLASS()
class JUBILANT_POTATO_API AGravPlayerCharacter
    : public AMotionMatchingCharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AGravPlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode, uint8 PrevCustomMode );

private:   // Functions
protected: // Variables
    UGravMovementComponent* movement;

public: // Variables
    // Camera components
    // UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    // USceneComponent* camera_root;
    // UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    // USceneComponent* gimbal;
    // UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    // USmartSpringArm* spring_arm;
    // UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    // UCameraComponent* camera;

private: // Variables
    FRotator target_rot;
    FVector target_up;

    FRotator last_gimbal_rot;
    FRotator last_sa_rot;

    bool updating_camera;
    bool can_update_camera = false;
};
