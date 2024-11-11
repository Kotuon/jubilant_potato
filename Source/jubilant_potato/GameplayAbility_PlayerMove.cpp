// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_PlayerMove.h"
#include "Kismet/KismetMathLibrary.h" // GetForwardVector(), GetRightVector()
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h" // FInputActionValue class

void UGameplayAbility_PlayerMove::OnTriggeredInputAction(
    const FInputActionValue& Value ) {
    APlayerCharacter* character =
        Cast< APlayerCharacter >( GetAvatarActorFromActorInfo() );
    USceneComponent* gimbal = character->gimbal;

    const FVector2D inputValue = Value.Get< FVector2D >();

    character->AddMovementInput( gimbal->GetForwardVector(), inputValue.Y,
                                 false );
    character->AddMovementInput( gimbal->GetRightVector(), inputValue.X,
                                 false );
}
