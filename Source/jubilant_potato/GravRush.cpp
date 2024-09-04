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
    if ( !is_floating && !manager->StartAction( type ) ) {
        return;
    }

    // if ( is_floating ) {
        has_moved = true;
        const FVector last_grav = movement->GetGravityDirection();
        const FVector next_grav = parent->camera->GetForwardVector();

        movement->SetGravityDirection( next_grav );
        movement->SetMovementMode( MOVE_Falling );
        parent->SetCanWalk( true );

        // return;
    // }

    // if ( movement->MovementMode != MOVE_Falling ) {
    //     const FVector up = parent->GetActorUpVector() * 800.f;
    //     movement->Velocity += up;
    // }

    is_floating = true;
    // movement->SetMovementMode( MOVE_Flying );
    // parent->SetCanWalk( false );
    // GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start grav rush." );
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start grav rush." );
}

void UGravRush::End() {
    Super::End();
    //...

    if ( !is_floating ) {
        return;
    }

    is_floating = false;
    movement->SetGravityDirection( original_grav );
    movement->SetMovementMode( MOVE_Falling );
    parent->SetCanWalk( true );
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "End grav rush." );
}

void UGravRush::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    //...

    const float result = FVector::DotProduct( parent->GetActorUpVector(), parent->gimbal->GetUpVector() );
    // GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, FString::SanitizeFloat( result ) );
    if ( result < 0.999f ) {
        parent->camera_root->SetWorldRotation(
            FMath::RInterpTo( parent->camera_root->GetComponentRotation(), parent->GetActorRotation(),
                              DeltaTime, 1.0f ) );
    }

    if ( !is_floating ) {
        return;
    }

    if ( !has_moved ) {
        if ( movement->Velocity.Length() > 0.f ) {
            movement->Velocity = FMath::Lerp( movement->Velocity, FVector::ZeroVector, 0.05f );
            if ( movement->Velocity.Length() < 5.f ) {
                movement->Velocity = FVector::ZeroVector;
            }
        }
    }
}

void UGravRush::MovementModeChanged( ACharacter* Character, EMovementMode PrevMovementMode, uint8 PrevCustomMode ) {
    // if ( PrevMovementMode == MOVE_Falling && parent->GetCharacterMovement()->MovementMode == MOVE_Walking ) {
    // parent->camera_root->SetWorldRotation( parent->GetActorRotation() );
    // }
}

void UGravRush::BindAction( UEnhancedInputComponent* PEI ) {
    Super::BindAction( PEI );

    if ( cancel_action ) {
        PEI->BindAction( cancel_action, ETriggerEvent::Triggered, this, &UAction::End );
    }
}

bool UGravRush::GetIsFloating() const {
    return is_floating;
}
