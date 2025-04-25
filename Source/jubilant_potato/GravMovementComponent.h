// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravMovementComponent.generated.h"

class AGravPlayerCharacter;

UCLASS()
class JUBILANT_POTATO_API UGravMovementComponent
    : public UCharacterMovementComponent {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    virtual void OnMovementUpdated( float DeltaSeconds,
                                    const FVector& OldLocation,
                                    const FVector& OldVelocity ) override;

    FTransform
    ProcessPostRootMotion( const FTransform& WorldRootMotionTransform,
                           UCharacterMovementComponent* movement,
                           float DeltaSeconds );

    virtual void UpdateGravity();

    void SetGravityToGround();

    virtual bool ShouldRemainVertical() const;

    void UpdateRotation( float DeltaTime );

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character,
                              EMovementMode PrevMovementMode,
                              uint8 PrevCustomMode );

    virtual void SetGravityDirection( const FVector& GravityDir );

private: // Functions
private: // Variables
    AGravPlayerCharacter* parent;

    FQuat currLastGravRotation;
    FQuat currRotation;
    FQuat desiredRotation;

    float startDistance;

    bool gravIsDirty = false;
    bool hasUpdatedRotationForNewGravity = true;
    bool currentlyUpdatingRotation = false;
};
