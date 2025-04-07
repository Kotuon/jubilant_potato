// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GravPlayerCharacter.generated.h"

class USmartSpringArm;
class UCameraComponent;
class UGravMovementComponent;

UCLASS()
class JUBILANT_POTATO_API AGravPlayerCharacter : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AGravPlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character,
                              EMovementMode PrevMovementMode,
                              uint8 PrevCustomMode );

    void SetCanUpdateCamera( bool Value );

    const FQuat& GetTargetQuat() const;

private:   // Functions
protected: // Variables
    UGravMovementComponent* movement;

public: // Variables
    // Camera components
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent* cameraRoot;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent* gimbal;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USmartSpringArm* springArm;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    UCameraComponent* camera;

private: // Variables
    // FRotator targetRot;
    FQuat targetRot;
    FVector targetUp;

    FRotator lastGimbalRot;
    FRotator lastSaRot;

    bool updatingCamera;
    bool canUpdateCamera = false;
};
