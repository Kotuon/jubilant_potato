// Fill out your copyright notice in the Description page of Project Settings.

#include "Aim.h"
#include "PlayerCharacter.h" // APlayerCharacter class
#include "SmartSpringArm.h"  // USmartSpringArm class

UAim::UAim() {
    type = EAction::A_Aim;
}

void UAim::BeginPlay() {
    Super::BeginPlay();
    //...

    spring_arm = parent->spring_arm;
}

void UAim::Start( const FInputActionValue &value ) {
}

void UAim::End() {
}

void UAim::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) {
}
