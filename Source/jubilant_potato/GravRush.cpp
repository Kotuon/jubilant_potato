// Fill out your copyright notice in the Description page of Project Settings.

#include "GravRush.h"
#include "ActionManager.h"                            // UActionManager class
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "Math/UnrealMathUtility.h"                   // Lerp
#include "EnhancedInputSubsystems.h"                  // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"                   // UEnhancedInputComponent class
#include "Camera/CameraComponent.h"                   // UCameraComponent class
#include "MatrixTypes.h"                              // FMatrix3d

UGravRush::UGravRush() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_GravRush;
    //...
}

void UGravRush::BeginPlay() {
    Super::BeginPlay();
    //...

    movement = parent->GetCharacterMovement();
    original_grav = movement->GetGravityDirection();
}

void UGravRush::Start( const FInputActionValue& Value ) {
    const FVector last_grav = movement->GetGravityDirection();
    const FVector next_grav = parent->camera->GetForwardVector();

    movement->SetGravityDirection( next_grav );
    movement->SetMovementMode( MOVE_Falling );
    parent->SetCanWalk( true );

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start grav rush." );
}

void UGravRush::End() {
    Super::End();
    //...
    movement->SetGravityDirection( original_grav );
    movement->SetMovementMode( MOVE_Falling );
    parent->SetCanWalk( true );

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "End grav rush." );
}

void UGravRush::TickComponent( float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...
}

void UGravRush::MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode,
                                     uint8 PrevCustomMode ) {
}

void UGravRush::InvertGrav() {
    movement->SetGravityDirection( movement->GetGravityDirection() * -1.f );
}

void UGravRush::LeftGrav() {
    movement->SetGravityDirection( parent->camera->GetRightVector() * -1.f );
}

void UGravRush::RightGrav() {
    movement->SetGravityDirection( parent->camera->GetRightVector() );
}

void UGravRush::BackGrav() {
    movement->SetGravityDirection( parent->camera->GetForwardVector() * -1.f );
}

void UGravRush::BindAction( UEnhancedInputComponent* PEI ) {
    Super::BindAction( PEI );

    if ( invert_action ) {
        PEI->BindAction( invert_action, ETriggerEvent::Triggered, this, &UGravRush::InvertGrav );
    }
    if ( left_action ) {
        PEI->BindAction( left_action, ETriggerEvent::Triggered, this, &UGravRush::LeftGrav );
    }
    if ( right_action ) {
        PEI->BindAction( right_action, ETriggerEvent::Triggered, this, &UGravRush::RightGrav );
    }
    if ( back_action ) {
        PEI->BindAction( back_action, ETriggerEvent::Triggered, this, &UGravRush::BackGrav );
    }
    if ( cancel_action ) {
        PEI->BindAction( cancel_action, ETriggerEvent::Triggered, this, &UAction::End );
    }
}
