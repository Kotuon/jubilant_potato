// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionSprint.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class

UActionSprint::UActionSprint() : UAction() { type = EAction::A_Sprint; }

void UActionSprint::BeginPlay() {
    Super::BeginPlay();
    //...

    character_movement = parent->GetCharacterMovement();
    original_walk_speed = character_movement->MaxWalkSpeed;
}

void UActionSprint::Start( const FInputActionValue& value ) {
    if ( value.Get< bool >() ) {
        if ( !is_sprinting ) {
            is_sprinting = true;
            character_movement->MaxWalkSpeed = sprint_speed;
        }
    } else {
        is_sprinting = false;
        character_movement->MaxWalkSpeed = original_walk_speed;
    }
}
