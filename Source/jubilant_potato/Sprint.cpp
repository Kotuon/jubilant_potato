// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprint.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class

USprint::USprint() : UAction() {
    type = EAction::A_Sprint;
}

void USprint::BeginPlay() {
    Super::BeginPlay();
    //...

    SetComponentTickEnabled( true );

    character_movement = parent->GetCharacterMovement();

    original_walk_speed = character_movement->MaxWalkSpeed;
}

void USprint::Start( const FInputActionValue &value ) {
    if ( !value.Get< bool >() ) {
        return;
    }

    if ( !manager->StartAction( type ) && !is_sprinting ) {
        return;
    }

    if ( is_sprinting ) {
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Stop sprinting." );
        End();
    } else {
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start sprinting." );
        is_sprinting = true;
        character_movement->MaxWalkSpeed = sprint_speed;
    }
}

void USprint::End() {
    Super::End();

    is_sprinting = false;
    character_movement->MaxWalkSpeed = original_walk_speed;
}

void USprint::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...

    const FVector last_input = parent->GetLastMovementInput();
    if ( abs( last_input.X ) + abs( last_input.Y ) <= 0.f ) {
        End();
    }
}
