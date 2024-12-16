// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionDash.h"
#include "PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"

UActionDash::UActionDash() { type = EAction::A_Dash; }

void UActionDash::BeginPlay() {
    Super::BeginPlay();
    //...

    world = GetWorld();
}

void UActionDash::Start( const FInputActionValue& value ) {
    const FVector parentLocation = parent->GetActorLocation();

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
}

void UActionDash::End() {}

void UActionDash::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    if ( dashTimer >= dashDuration ) {
        End();
        return;
    }
    dashTimer += DeltaTime;

    FVector nextLocation =
        FMath::InterpEaseOut( parent->GetActorLocation(), endLocation,
                              dashTimer / dashDuration, 0.25 );
    parent->SetActorLocation( nextLocation );
}
