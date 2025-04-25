// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionDash.h"
#include "PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UActionDash::UActionDash() { type = EAction::A_Dash; }

void UActionDash::BeginPlay() {
    Super::BeginPlay();
    //...

    world = GetWorld();
    gimbal = parent->GetGimbal();
}

void UActionDash::Start( const FInputActionValue& value ) {
    // Update player variables
    parent->SetCanMove( false );
    isRunning = true;

    // Get intial location
    startLocation = parent->GetActorLocation();

    // Find direction to dash using movement inputs
    FVector dashDirection =
        parent->GetLastMovementInput().Y * gimbal->GetForwardVector() +
        parent->GetLastMovementInput().X * gimbal->GetRightVector();

    // Default dash direction if no movement inputs
    if ( dashDirection.Length() <= 0.f ) {
        dashDirection = gimbal->GetForwardVector();
    }

    // Get end location
    endLocation = startLocation + ( dashDirection * distance );

    // Search for any obstacles to the dash
    TArray< TEnumAsByte< EObjectTypeQuery > > objectTypes;
    objectTypes.Add( UEngineTypes::ConvertToObjectType( ECC_WorldStatic ) );

    TArray< AActor*, FDefaultAllocator > ignoreTypes;
    ignoreTypes.Add( parent );

    FHitResult hitResult;
    UKismetSystemLibrary::SphereTraceSingleForObjects(
        world, startLocation, endLocation, 50.f, objectTypes, false,
        ignoreTypes, EDrawDebugTrace::None, hitResult, false );

    // Update end location if there is an obstacle
    if ( hitResult.bBlockingHit ) {
        endLocation =
            hitResult.ImpactPoint -
            ( ( hitResult.ImpactPoint - startLocation ).Normalize() * 50.f );
    }

    dashTimer = 0.f;

    // Trigger VFX
    UNiagaraComponent* effectComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            world, effect, startLocation, dashDirection.Rotation(),
            FVector( distance / 800.f, 0.25f, 0.25f ) );
}

void UActionDash::End() {
    // Update player variables
    parent->SetCanMove( true );
    isRunning = false;
}

void UActionDash::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    if ( !isRunning ) return;

    // Update the dash timer
    if ( dashTimer >= dashDuration - ( dashDuration * 0.2f ) ) {
        End();
        return;
    }
    dashTimer += DeltaTime;

    // Continue interpolating the players location along dash vector
    FVector nextLocation =
        FMath::InterpEaseOut( parent->GetActorLocation(), endLocation,
                              dashTimer / dashDuration, 0.25 );
    parent->SetActorLocation( nextLocation );
}
