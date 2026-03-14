// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionFly.h"
#include "GravMovementComponent.h"
#include "PlayerCharacter.h"

UActionFly::UActionFly() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_Fly;
    // ...
}

void UActionFly::BeginPlay() {
    Super::BeginPlay();
    // ...

    world = GetWorld();
    movement = Cast< UGravMovementComponent >( parent->GetCharacterMovement() );
}

void UActionFly::Start( const FInputActionValue& Value ) {
    if ( flying ) {
        movement->MovementMode = MOVE_Falling;
        flying = false;
    } else {
        movement->MovementMode = MOVE_Flying;
        flying = true;
    }

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Fly" );
}

void UActionFly::End() { Super::End(); }

void UActionFly::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}
