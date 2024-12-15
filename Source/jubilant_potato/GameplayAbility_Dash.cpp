// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_Dash.h"
#include "GameFramework/Character.h"

UGameplayAbility_Dash::UGameplayAbility_Dash(
    class FObjectInitializer const& ) {}

bool UGameplayAbility_Dash::CanActivateAbility(
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
    return ( IsValid( Character ) );
}

void UGameplayAbility_Dash::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData ) {
    if ( HasAuthorityOrPredictionKey( ActorInfo, &ActivationInfo ) ) {
        if ( !CommitAbility( Handle, ActorInfo, ActivationInfo ) ) return;

        ACharacter* Character =
            CastChecked< ACharacter >( ActorInfo->AvatarActor.Get() );
        // TODO: Implement dash

    }
}

void UGameplayAbility_Dash::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo ) {

    if ( ActorInfo != NULL && ActorInfo->AvatarActor != NULL ) {
        CancelAbility( Handle, ActorInfo, ActivationInfo, true );
    }
}

void UGameplayAbility_Dash::CancelAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility ) {
    if ( ScopeLockCount > 0 ) {
        WaitingToExecute.Add( FPostLockDelegate::CreateUObject(
            this, &UGameplayAbility_Dash::CancelAbility, Handle, ActorInfo,
            ActivationInfo, bReplicateCancelAbility ) );
        return;
    }

    Super::CancelAbility( Handle, ActorInfo, ActivationInfo,
                          bReplicateCancelAbility );

    ACharacter* Character =
        CastChecked< ACharacter >( ActorInfo->AvatarActor.Get() );
    // TODO: Implement dash
}