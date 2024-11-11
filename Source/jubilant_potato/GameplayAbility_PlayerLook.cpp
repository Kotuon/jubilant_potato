// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_PlayerLook.h"
#include "GameFramework/Character.h"  // ACharacter
#include "EnhancedInputComponent.h"   // FInputActionValue class

void UGameplayAbility_PlayerLook::OnTriggeredInputAction(
    const FInputActionValue& Value ) {
    ACharacter* character = Cast< ACharacter >( GetAvatarActorFromActorInfo() );

    character->AddControllerPitchInput( Value.Get< FVector2D >().Y );
    character->AddControllerYawInput( Value.Get< FVector2D >().X );
}