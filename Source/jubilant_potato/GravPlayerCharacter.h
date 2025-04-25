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

    const FQuat& GetTargetCameraOrientation() const;

    USmartSpringArm* GetSpringArm() const;
    USceneComponent* GetGimbal() const;
    UCameraComponent* GetCamera() const;

private: // Functions
    void UpdateCameraOrientation( float DeltaTime );
    void SetCameraUpdateValues( const FVector& inverseGravity,
                                const FQuat& startRot );

protected: // Variables
    // Camera components
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera",
               meta = ( AllowPrivateAccess = "true" ) )
    USceneComponent* cameraRoot;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera",
               meta = ( AllowPrivateAccess = "true" ) )
    USceneComponent* gimbal;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera",
               meta = ( AllowPrivateAccess = "true" ) )
    USmartSpringArm* springArm;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera",
               meta = ( AllowPrivateAccess = "true" ) )
    UCameraComponent* camera;

    UGravMovementComponent* gravMovement;

private: // Variables
    // FRotator targetRot;
    FQuat targetCameraOrientation; //!< Assumed to be normalized
    FVector targetUp;

    FRotator lastGimbalRot;
    FRotator lastSaRot;

    bool updatingCamera;
    bool canUpdateCamera = false;
};
