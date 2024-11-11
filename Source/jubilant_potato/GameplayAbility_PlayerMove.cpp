// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_PlayerMove.h"
#include "Kismet/KismetMathLibrary.h" // GetForwardVector(), GetRightVector()
#include "GameFramework/Character.h"  // ACharacter
#include "EnhancedInputComponent.h"   // FInputActionValue class

void UGameplayAbility_PlayerMove::OnTriggeredInputAction(
    const FInputActionValue& Value ) {
    ACharacter* character = Cast< ACharacter >( GetAvatarActorFromActorInfo() );

    const FRotator rotation = character->GetControlRotation();

    character->AddMovementInput(
        UKismetMathLibrary::GetForwardVector( rotation ),
        Value.Get< FVector2D >().Y );
    character->AddMovementInput( UKismetMathLibrary::GetRightVector( rotation ),
                                 Value.Get< FVector2D >().X );
}
