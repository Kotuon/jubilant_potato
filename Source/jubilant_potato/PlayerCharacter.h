// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class USmartSpringArm;
class UCameraComponent;
class UAction;
class UActionManager;

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    APlayerCharacter( const FObjectInitializer &ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent( class UInputComponent *PlayerInputComponent ) override;

    UFUNCTION( BlueprintCallable )
    void SetCanWalk( bool canWalk );

    bool GetCanWalk() const;

    UFUNCTION( BlueprintCallable )
    const FVector &GetLastMovementInput() const;

    void SetLastMovementZInput( const float input_value );

public: // Variables
    UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Actions" )
    UActionManager *action_manager;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext *input_mapping;

    // Input Actions
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction *input_move;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction *input_look;

    // Camera components
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent *camera_root;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent *gimbal;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USmartSpringArm *spring_arm;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    UCameraComponent *camera;

    // Running
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jump" )
    UAnimMontage *run_to_stop_animation;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

private: // Functions
    void Move( const FInputActionValue &value );
    void Look( const FInputActionValue &value );

private: // Variables
    UCharacterMovementComponent *character_movement;

    FVector last_movement_input;

    bool can_walk = true;
};
