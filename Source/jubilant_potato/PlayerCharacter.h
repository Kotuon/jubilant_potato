// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GravPlayerCharacter.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UAction;
class UActionManager;

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public AGravPlayerCharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    APlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

    UFUNCTION( BlueprintCallable )
    void SetCanWalk( bool canWalk ) {
        can_walk = canWalk;
    }

    bool GetCanWalk() const {
        return can_walk;
    }

    UFUNCTION( BlueprintCallable )
    const FVector& GetLastMovementInput() const {
        return last_movement_input;
    }

    UFUNCTION( BlueprintCallable )
    const FVector2D& GetLastCameraInput() const {
        return last_camera_input;
    }

    void SetLastMovementZInput( const float input_value ) {
        last_movement_input.Z = input_value;
    }

public: // Variables
    UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Actions" )
    UActionManager* action_manager;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* input_mapping;

    // Input Actions
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction* input_move;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions" )
    UInputAction* input_look;

    // Running
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Jump" )
    UAnimMontage* run_to_stop_animation;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

private: // Functions
    void Move( const FInputActionValue& value );
    void Look( const FInputActionValue& value );

private: // Variables
    FVector last_movement_input;
    FVector2D last_camera_input;

    float time_running;

    float norm_run_speed = 600.f;
    float fast_run_speed = 2400.f;
    const float time_to_reach_max_run = 5.f;

    bool can_walk = true;
    bool is_running = false;
};
