// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionJump.generated.h"

class UCharacterMovementComponent;
class UAnimMontage;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API UActionJump : public UAction {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    UActionJump();
    virtual void Start( const FInputActionValue& value );
    virtual void End();

    UFUNCTION( BlueprintCallable )
    void JumpTakeOff();

    UFUNCTION()
    void MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode, uint8 PrevCustomMode );

    // Landing
    UFUNCTION()
    void OnLanded( const FHitResult& Hit );

    UFUNCTION( BlueprintCallable )
    void SetHasPlayedAnimation();

    UFUNCTION( BlueprintCallable )
    bool GetHasPlayedAnimation() const;

public: // Variables
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
    UAnimMontage* jump_montage;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General|Jump Memory" )
    float jump_memory_time = 0.25f;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General|Coyote Time" )
    float coyote_time = 0.25f;

private: // Variables
    UCharacterMovementComponent* movement;
    FTimerManager* timer_manager;

    FTimerHandle jump_memory_handle;
    FTimerHandle coyote_time_handle;

    bool has_jumped = false;
    bool jump_memory = false;
    bool can_coyote = false;
    bool has_played_jump_animation = false;
};
