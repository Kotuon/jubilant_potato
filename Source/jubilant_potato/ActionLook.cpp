// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionLook.h"
#include "PlayerCharacter.h"
#include "SmartSpringArm.h"

UActionLook::UActionLook() { type = EAction::A_Look; }

void UActionLook::BeginPlay() {
    Super::BeginPlay();
    //...

    gimbal = parent->gimbal;
    springArm = parent->springArm;
}

void UActionLook::Start( const FInputActionValue& value ) {
    const FVector2D inputValue = value.Get< FVector2D >();
    parent->SetLastCameraInput( inputValue );

    const float inputYaw = gimbal->GetRelativeRotation().Yaw +
                           ( inputValue.X * parent->sensitivity );
    const FRotator newYaw{ 0.0, inputYaw, 0.0 };
    gimbal->SetRelativeRotation( newYaw );

    const float inputPitch = springArm->GetRelativeRotation().Pitch +
                             ( inputValue.Y * parent->sensitivity );
    const float clampedPitch = FMath::Clamp( inputPitch, -80, 80 );

    const FRotator newPitch{ clampedPitch, 0.0, 0.0 };
    springArm->SetRelativeRotation( newPitch );
}
