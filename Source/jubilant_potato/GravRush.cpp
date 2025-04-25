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

    parent->ResourceEmptyDelegate.AddUObject( this, &UGravRush::ResourceEmpty );

    movement = Cast< UGravMovementComponent >( parent->GetCharacterMovement() );
    originalGrav = movement->GetGravityDirection();

    camera = parent->GetCamera();
}

void UGravRush::Start( const FInputActionValue& Value ) {
    // Get new gravity direction and trigger the change
    const FVector nextGrav = camera->GetForwardVector();
    TriggerGravShift( nextGrav );
}

void UGravRush::End() {
    Super::End();
    //...

    GEngine->AddOnScreenDebugMessage( -1, 4.f, FColor::Green, "Cancel grav." );

    // Reset gravity to default direction
    movement->SetGravityDirection( originalGrav.GetSafeNormal() );
    movement->SetMovementMode( MOVE_Falling );
    parent->SetCanMove( true );

    hasClicked = false;

    // Reset timer
    parent->ClearResourceTimer();
}

void UGravRush::TickComponent( float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...
}

void UGravRush::MovementModeChanged( ACharacter* Character,
                                     EMovementMode PrevMovementMode,
                                     uint8 PrevCustomMode ) {}

void UGravRush::TriggerGravShift( const FVector direction ) {
    // Check if there is resource remaining
    if ( !parent->UseResource( startCost ) ) {
        return;
    }

    // Set new direction of gravity
    movement->SetGravityDirection( direction.GetSafeNormal() );
    hasClicked = true;

    // Start tick resource use
    parent->UseResourceOnTimer( tickCost );
}

void UGravRush::ResourceEmpty() {
    // Callback for when resource runs out
    if ( hasClicked ) {
        End();
    }
}

void UGravRush::InvertGrav() {
    GEngine->AddOnScreenDebugMessage( -1, 4.f, FColor::Green, "Invert grav." );
    TriggerGravShift( movement->GetGravityDirection() * -1.f );
}

void UGravRush::LeftGrav() {
    GEngine->AddOnScreenDebugMessage( -1, 4.f, FColor::Green, "Left grav." );
    TriggerGravShift( camera->GetRightVector() * -1.f );
}

void UGravRush::RightGrav() {
    GEngine->AddOnScreenDebugMessage( -1, 4.f, FColor::Green, "Right grav." );
    TriggerGravShift( camera->GetRightVector() );
}

void UGravRush::BackGrav() {
    TriggerGravShift( camera->GetForwardVector() * -1.f );
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
