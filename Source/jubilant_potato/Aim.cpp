// Fill out your copyright notice in the Description page of Project Settings.

#include "Aim.h"
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "SmartSpringArm.h"                           // USmartSpringArm class
#include "ActionManager.h"                            // UActionManager class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class

UAim::UAim() {
    type = EAction::A_Aim;
}

void UAim::BeginPlay() {
    Super::BeginPlay();
    //...

    // spring_arm = parent->spring_arm;
}

void UAim::Start( const FInputActionValue& value ) {
    if ( value.Get< bool >() ) {
        is_aiming = true;
        spring_arm->SetIsAiming( true );

        // parent->GetCharacterMovement()->bOrientRotationToMovement = false;

        GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, "Holding aim." );
    } else {
        End();
    }
}

void UAim::End() {
    Super::End();

    is_aiming = false;
    spring_arm->SetIsAiming( false );
    // parent->GetCharacterMovement()->bOrientRotationToMovement = true;
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "Released aim." );
}

void UAim::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
}

bool UAim::GetIsAiming() const {
    return is_aiming;
}
