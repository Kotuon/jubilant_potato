// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionJump.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "Components/CapsuleComponent.h"              // UCapsuleComponent class
#include "Kismet/KismetMathLibrary.h"                 // MapRangeClamped
#include "Animation/AnimMontage.h"                    // UAnimMontage class

UActionJump::UActionJump() {
    type = EAction::A_Jump;
}

void UActionJump::BeginPlay() {
    Super::BeginPlay();

    movement = parent->GetCharacterMovement();
    timer_manager = &( parent->GetWorldTimerManager() );

    parent->MovementModeChangedDelegate.AddUniqueDynamic(
        this, &UActionJump::MovementModeChanged );
    parent->LandedDelegate.AddUniqueDynamic( this, &UActionJump::OnLanded );
}

void UActionJump::Start( const FInputActionValue& value ) {
    if ( !movement->IsMovingOnGround() )
        return;

    parent->Jump();

    // if ( value.Get< bool >() ) {
    //     parent->SetLastMovementZInput( 1.f );
    // } else {
    //     parent->SetLastMovementZInput( 0.f );
    //     End();
    //     return;
    // }

    // if ( movement->IsFalling() && can_coyote ) {
    //     can_coyote = false;
    // } else if ( movement->IsFalling() && !jump_memory ) {
    //     jump_memory = true;
    //     timer_manager->SetTimer( jump_memory_handle,
    //                              FTimerDelegate::CreateLambda(
    //                                  [this] { jump_memory = false; } ),
    //                              0.1f, false, jump_memory_time );
    //     return;
    // }
    // has_jumped = true;
    // parent->PlayAnimMontage( jump_montage );
}

void UActionJump::End() {
    Super::End();

    // parent->StopJumping();
}

void UActionJump::JumpTakeOff() {
    // const FVector velocity = movement->Velocity;
    // const float velocity_xy = velocity.Size2D();

    // movement->JumpZVelocity = UKismetMathLibrary::MapRangeClamped(
    //     velocity_xy, 400.f, 800.f, 700.f, 900.f );
    // parent->Jump();

    // movement->bNotifyApex = true;
}

void UActionJump::MovementModeChanged( ACharacter* Character,
                                 EMovementMode PrevMovementMode,
                                 uint8 PrevCustomMode ) {
    // if ( PrevMovementMode == MOVE_Falling &&
    //      movement->MovementMode == MOVE_Walking ) {
    //     has_jumped = false;
    //     End();
    //     if ( jump_memory )
    //         Start( FInputActionValue() );
    // } else if ( PrevMovementMode == MOVE_Walking &&
    //             movement->MovementMode == MOVE_Falling ) {
    //     can_coyote = true;
    //     timer_manager->SetTimer( coyote_time_handle,
    //                              FTimerDelegate::CreateLambda(
    //                                  [this] { can_coyote = false; } ),
    //                              0.1f, false, coyote_time );
    // }
}

void UActionJump::SetHasPlayedAnimation() {
    // has_played_jump_animation = true;
}

bool UActionJump::GetHasPlayedAnimation() const {
    // return has_played_jump_animation;
    return false;
}

// Landing
void UActionJump::OnLanded( const FHitResult& Hit ) {
    // has_played_jump_animation = false;
}
