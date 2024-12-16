// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAim.h"
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "SmartSpringArm.h"                           // USmartSpringArm class
#include "ActionManager.h"                            // UActionManager class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class

UActionAim::UActionAim() { type = EAction::A_Aim; }

void UActionAim::BeginPlay() {
    Super::BeginPlay();
    //...

    springArm = parent->springArm;
}

void UActionAim::Start( const FInputActionValue& value ) {
    if ( value.Get< bool >() ) {
        isAiming = true;
        springArm->SetIsAiming( true );
        parent->SetStrafe( true );
        parent->SetActorRotation( parent->gimbal->GetComponentRotation() );

        GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                          "Holding aim." );
    } else {
        End();
    }
}

void UActionAim::End() {
    Super::End();

    isAiming = false;
    springArm->SetIsAiming( false );
    parent->SetStrafe( false );
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "Released aim." );
}

void UActionAim::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {}

bool UActionAim::GetIsAiming() const { return isAiming; }
