// Fill out your copyright notice in the Description page of Project Settings.

#include "GravRush.h"
#include "ActionManager.h"           // UActionManager class
#include "PlayerCharacter.h"         // APlayerCharacter class
#include "GravMovementComponent.h"   // UGravMovementComponent class
#include "Math/UnrealMathUtility.h"  // Lerp
#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"  // UEnhancedInputComponent class
#include "Camera/CameraComponent.h"  // UCameraComponent class
#include "MatrixTypes.h"             // FMatrix3d

UGravRush::UGravRush() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_GravRush;
    //...
}

void UGravRush::BeginPlay() {
    Super::BeginPlay();
    //...

    movement = Cast< UGravMovementComponent >( parent->GetCharacterMovement() );
    originalGrav = movement->GetGravityDirection();
}

void UGravRush::Start( const FInputActionValue& Value ) {
    const FVector lastGrav = movement->GetGravityDirection();
    const FVector nextGrav = parent->camera->GetForwardVector();

    TriggerGravShift( nextGrav );
}

void UGravRush::End() {
    Super::End();
    //...
    movement->SetGravityDirection( originalGrav );
    movement->SetMovementMode( MOVE_Falling );
    parent->SetCanMove( true );

    hasClicked = false;

    parent->ClearResourceTimer();
}

void UGravRush::TickComponent( float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...
    if ( movement->MovementMode == MOVE_Walking ) {
        movement->SetGravityDirection(
            movement->CurrentFloor.HitResult.ImpactNormal * -1.f );
        parent->SetCanUpdateCamera( true );
    }

    // movement->SetGravityToGround();
}

void UGravRush::MovementModeChanged( ACharacter* Character,
                                     EMovementMode PrevMovementMode,
                                     uint8 PrevCustomMode ) {}

void UGravRush::TriggerGravShift( const FVector direction ) {
    if ( !parent->UseResource( startCost ) ) {
        return;
    }
    movement->SetGravityDirection( direction );
    hasClicked = true;

    parent->UseResourceOnTimer( tickCost );
}

void UGravRush::InvertGrav() {
    TriggerGravShift( movement->GetGravityDirection() * -1.f );
}

void UGravRush::LeftGrav() {
    TriggerGravShift( parent->camera->GetRightVector() * -1.f );
}

void UGravRush::RightGrav() {
    TriggerGravShift( parent->camera->GetRightVector() );
}

void UGravRush::BackGrav() {
    TriggerGravShift( parent->camera->GetForwardVector() * -1.f );
}

void UGravRush::GroundGrav() {
    if ( groundGrav ) {
        groundGrav = false;
        End();
    } else {
        groundGrav = true;
    }
}

void UGravRush::BindAction( UEnhancedInputComponent* PEI ) {
    Super::BindAction( PEI );

    if ( invertAction ) {
        PEI->BindAction( invertAction, ETriggerEvent::Triggered, this,
                         &UGravRush::InvertGrav );
    }
    if ( leftAction ) {
        PEI->BindAction( leftAction, ETriggerEvent::Triggered, this,
                         &UGravRush::LeftGrav );
    }
    if ( rightAction ) {
        PEI->BindAction( rightAction, ETriggerEvent::Triggered, this,
                         &UGravRush::RightGrav );
    }
    if ( backAction ) {
        PEI->BindAction( backAction, ETriggerEvent::Triggered, this,
                         &UGravRush::BackGrav );
    }
    if ( cancelAction ) {
        PEI->BindAction( cancelAction, ETriggerEvent::Triggered, this,
                         &UAction::End );
    }
    if ( groundAction ) {
        PEI->BindAction( groundAction, ETriggerEvent::Triggered, this,
                         &UGravRush::GroundGrav );
    }
}
