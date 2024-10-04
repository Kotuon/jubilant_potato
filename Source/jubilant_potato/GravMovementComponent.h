// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GravMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class JUBILANT_POTATO_API UGravMovementComponent :
public UCharacterMovementComponent {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    virtual void OnMovementUpdated( float DeltaSeconds, 
    const FVector& OldLocation,
                                    const FVector& OldVelocity ) override;

    virtual void UpdateGravity();

    virtual bool ShouldRemainVertical() const;

    void UpdateRotation( float DeltaTime );

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character, 
    EMovementMode PrevMovementMode,
                              uint8 PrevCustomMode );

    virtual void SetGravityDirection( const FVector& GravityDir );

private: // Variables
    FQuat LastGravityToWorldTransform;
    FQuat LastWorldToGravityTransform;

    FRotator currentLastGravRotation;
    FRotator currentRotation;
    FRotator desiredRotation;

    float startDistance;

    bool gravIsDirty = false;
    bool hasUpdatedRotationForNewGravity = true;
    bool currentlyUpdatingRotation = false;
};
