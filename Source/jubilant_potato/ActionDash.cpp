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
}

void UActionDash::Start( const FInputActionValue& value ) {
    parent->SetCanMove( false );
    isRunning = true;

    const FVector parentLocation = parent->GetActorLocation();
    startLocation = parentLocation;

    FVector dashDirection =
        parent->GetLastMovementInput().Y * parent->gimbal->GetForwardVector() +
        parent->GetLastMovementInput().X * parent->gimbal->GetRightVector();

    if ( dashDirection.Length() <= 0.f ) {
        dashDirection = parent->gimbal->GetForwardVector();
    }

    endLocation = parentLocation + ( dashDirection * distance );

    TArray< TEnumAsByte< EObjectTypeQuery > > objectTypes;
    objectTypes.Add( UEngineTypes::ConvertToObjectType( ECC_WorldStatic ) );

    TArray< AActor*, FDefaultAllocator > ignoreTypes;
    ignoreTypes.Add( parent );

    FHitResult hitResult;
    UKismetSystemLibrary::SphereTraceSingleForObjects(
        world, parentLocation, endLocation, 50.f, objectTypes, false,
        ignoreTypes, EDrawDebugTrace::None, hitResult, false );

    if ( hitResult.bBlockingHit ) {
        endLocation =
            hitResult.ImpactPoint -
            ( ( hitResult.ImpactPoint - parentLocation ).Normalize() * 50.f );
    }

    dashTimer = 0.f;

    // VFX
    UNiagaraComponent* effectComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            world, effect, startLocation, dashDirection.Rotation(),
            FVector( 1.f, 0.25f, 0.25f ) );
}

void UActionDash::End() {
    parent->SetCanMove( true );
    isRunning = false;
}

void UActionDash::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    if ( !isRunning ) return;

    if ( dashTimer >= dashDuration - ( dashDuration * 0.2f ) ) {
        End();
        return;
    }
    dashTimer += DeltaTime;

    FVector nextLocation =
        FMath::InterpEaseOut( parent->GetActorLocation(), endLocation,
                              dashTimer / dashDuration, 0.25 );
    parent->SetActorLocation( nextLocation );
}
