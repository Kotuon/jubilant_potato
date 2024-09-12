// Fill out your copyright notice in the Description page of Project Settings.

#include "Jump.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "Components/CapsuleComponent.h"              // UCapsuleComponent class
#include "Kismet/KismetMathLibrary.h"                 // MapRangeClamped
#include "Animation/AnimMontage.h"                    // UAnimMontage class

UJump::UJump() {
    type = EAction::A_Jump;
}

void UJump::BeginPlay() {
    Super::BeginPlay();

    movement = parent->GetCharacterMovement();
    timer_manager = &( parent->GetWorldTimerManager() );

    parent->MovementModeChangedDelegate.AddUniqueDynamic( this, &UJump::MovementModeChanged );
    parent->LandedDelegate.AddUniqueDynamic( this, &UJump::OnLanded );
}

void UJump::Start( const FInputActionValue& value ) {
    if ( value.Get< bool >() ) {
        parent->SetLastMovementZInput( 1.f );
    } else {
        parent->SetLastMovementZInput( 0.f );
        End();
        return;
    }

    if ( movement->IsFalling() && can_coyote ) {
        can_coyote = false;
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Purple, "Has used Coyote" );
    } else if ( movement->IsFalling() && !jump_memory ) {
        jump_memory = true;
        timer_manager->SetTimer( jump_memory_handle,
                                 FTimerDelegate::CreateLambda( [this] { jump_memory = false; } ),
                                 0.1f, false, jump_memory_time );
        return;
    }
    has_jumped = true;
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "JUMP!" );
    parent->PlayAnimMontage( jump_montage );
}

void UJump::End() {
    Super::End();

    parent->StopJumping();
}

void UJump::JumpTakeOff() {
    const FVector velocity = movement->Velocity;
    const float velocity_xy = velocity.Size2D();

    movement->JumpZVelocity = UKismetMathLibrary::MapRangeClamped( velocity_xy,
                                                                   400.f, 800.f, 700.f, 900.f );
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "JUMPING" );
    parent->Jump();

    movement->bNotifyApex = true;
}

void UJump::MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode,
                                 uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling && movement->MovementMode == MOVE_Walking ) {
        has_jumped = false;
        End();
        if ( jump_memory )
            Start( FInputActionValue() );
    } else if ( PrevMovementMode == MOVE_Walking && movement->MovementMode == MOVE_Falling ) {
        can_coyote = true;
        timer_manager->SetTimer( coyote_time_handle,
                                 FTimerDelegate::CreateLambda( [this] { can_coyote = false;GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "Can not Coyote" ); } ),
                                 0.1f, false, coyote_time );
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Can Coyote" );
    }
}

void UJump::SetHasPlayedAnimation() {
    has_played_jump_animation = true;
}

bool UJump::GetHasPlayedAnimation() const {
    return has_played_jump_animation;
}

// Landing
void UJump::OnLanded( const FHitResult& Hit ) {
    has_played_jump_animation = false;
}
