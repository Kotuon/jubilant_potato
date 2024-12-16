// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionSprint.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "ActionAim.h"                                // UActionAim class

UActionSprint::UActionSprint() : UAction() { type = EAction::A_Sprint; }

void UActionSprint::BeginPlay() {
    Super::BeginPlay();
    //...

    SetComponentTickEnabled( true );

    character_movement = parent->GetCharacterMovement();

    original_walk_speed = character_movement->MaxWalkSpeed;

    TArray< UAction* > action_components;
    parent->GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        if ( action->type == EAction::A_Aim ) {
            aim_action = Cast< UActionAim >( action );
        }
    }
}

void UActionSprint::Start( const FInputActionValue& value ) {
    if ( !value.Get< bool >() ) {
        return;
    }

    if ( !manager->StartAction( type ) && !is_sprinting ) {
        return;
    }

    if ( is_sprinting ) {
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green,
                                          "Stop sprinting." );
        End();
    } else {
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green,
                                          "Start sprinting." );
        is_sprinting = true;
        character_movement->MaxWalkSpeed = sprint_speed;
    }
}

void UActionSprint::End() {
    Super::End();

    is_sprinting = false;
    character_movement->MaxWalkSpeed = original_walk_speed;
}

void UActionSprint::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...

    const FVector last_input = parent->GetLastMovementInput();
    if ( ( last_input.Length() <= 0.f ) || aim_action->GetIsAiming() ) {
        End();
    }
}
