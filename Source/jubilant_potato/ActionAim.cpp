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

    // Getting references to different camera components
    springArm = parent->GetSpringArm();
    gimbal = parent->GetGimbal();
}

void UActionAim::Start( const FInputActionValue& value ) {
    // If aim input is triggered
    if ( value.Get< bool >() ) {
        // Notify camera of aiming
        isAiming = true;
        springArm->SetIsAiming( true );

        // Make player strafe
        parent->SetStrafe( true );
        parent->SetActorRotation( gimbal->GetComponentRotation() );
    } else {
        End();
    }
}

void UActionAim::End() {
    Super::End();

    // Notify camera of cancelled aiming
    isAiming = false;
    springArm->SetIsAiming( false );

    // Stop strafing
    parent->SetStrafe( false );
}

void UActionAim::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {}

bool UActionAim::GetIsAiming() const { return isAiming; }
