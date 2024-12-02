// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_Sprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGameplayAbility_Sprint::UGameplayAbility_Sprint(
    class FObjectInitializer const& ) {}

bool UGameplayAbility_Sprint::CanActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags,
    OUT FGameplayTagContainer* OptionalRelevantTags ) const {
    if ( !Super::CanActivateAbility( Handle, ActorInfo, SourceTags, TargetTags,
                                     OptionalRelevantTags ) )
        return false;

    const ACharacter* Character = CastChecked< ACharacter >(
        ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed );
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red,
                                      "Check can activate sprint." );
    return ( IsValid( Character ) );
}

void UGameplayAbility_Sprint::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData ) {
    if ( HasAuthorityOrPredictionKey( ActorInfo, &ActivationInfo ) ) {
        if ( !CommitAbility( Handle, ActorInfo, ActivationInfo ) ) return;

        ACharacter* Character =
            CastChecked< ACharacter >( ActorInfo->AvatarActor.Get() );
        // TODO: Implement sprint
        UCharacterMovementComponent* Movement =
            Character->GetCharacterMovement();

        Movement->MaxWalkSpeed = 800.f;

        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red,
                                          "Press sprint." );
    }
}

void UGameplayAbility_Sprint::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo ) {

    if ( ActorInfo != NULL && ActorInfo->AvatarActor != NULL ) {
        CancelAbility( Handle, ActorInfo, ActivationInfo, true );
    }
}

void UGameplayAbility_Sprint::CancelAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility ) {
    if ( ScopeLockCount > 0 ) {
        WaitingToExecute.Add( FPostLockDelegate::CreateUObject(
            this, &UGameplayAbility_Sprint::CancelAbility, Handle, ActorInfo,
            ActivationInfo, bReplicateCancelAbility ) );
        return;
    }

    Super::CancelAbility( Handle, ActorInfo, ActivationInfo,
                          bReplicateCancelAbility );

    ACharacter* Character =
        CastChecked< ACharacter >( ActorInfo->AvatarActor.Get() );
    // TODO: Implement sprint
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

    Movement->MaxWalkSpeed = 600.f;
    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "Release sprint." );
}
