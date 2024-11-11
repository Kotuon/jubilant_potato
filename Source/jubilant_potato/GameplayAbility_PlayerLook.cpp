// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_PlayerLook.h"
#include "PlayerCharacter.h"
#include "SmartSpringArm.h"
#include "EnhancedInputComponent.h" // FInputActionValue class

void UGameplayAbility_PlayerLook::OnTriggeredInputAction(
    const FInputActionValue& Value ) {
    APlayerCharacter* character =
        Cast< APlayerCharacter >( GetAvatarActorFromActorInfo() );

    USmartSpringArm* springArm = character->springArm;
    USceneComponent* gimbal = character->gimbal;

    const FVector2D inputValue = Value.Get< FVector2D >();

    const float inputYaw = gimbal->GetRelativeRotation().Yaw +
                           ( inputValue.X * character->sensitivity );
    const FRotator newYaw{ 0.0, inputYaw, 0.0 };
    gimbal->SetRelativeRotation( newYaw );

    const float inputPitch = springArm->GetRelativeRotation().Pitch +
                             ( inputValue.Y * character->sensitivity );
    const float clampedPitch = FMath::Clamp( inputPitch, -80, 80 );

    const FRotator newPitch{ clampedPitch, 0.0, 0.0 };
    springArm->SetRelativeRotation( newPitch );
}