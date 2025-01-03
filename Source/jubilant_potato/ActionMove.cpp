// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionMove.h"
#include "PlayerCharacter.h"

UActionMove::UActionMove() { type = EAction::A_Move; }

void UActionMove::BeginPlay() {
    Super::BeginPlay();
    //...

    gimbal = parent->gimbal;
}

void UActionMove::Start( const FInputActionValue& value ) {
    if ( !parent->GetCanMove() ) return;

    const FVector2D inputValue = value.Get< FVector2D >();
    parent->SetLastMovementInput( FVector( inputValue.X, inputValue.Y, 0.0f ) );

    parent->AddMovementInput( gimbal->GetForwardVector(), inputValue.Y, false );
    parent->AddMovementInput( gimbal->GetRightVector(), inputValue.X, false );
}
