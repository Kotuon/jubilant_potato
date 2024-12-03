// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_Aim.h"

#include "PlayerCharacter.h"
#include "SmartSpringArm.h"

UGameplayAbility_Aim::UGameplayAbility_Aim( class FObjectInitializer const& ) {
    const APlayerCharacter* character = CastChecked< APlayerCharacter >(
        ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed );

    springArm = Character->springArm;
}

bool UGameplayAbility_Aim::CanActivateAbility(
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

void UGameplayAbility_Aim::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData ) {
    if ( HasAuthorityOrPredictionKey( ActorInfo, &ActivationInfo ) ) {
        if ( !CommitAbility( Handle, ActorInfo, ActivationInfo ) ) return;

        // TODO: Implement aim
        springArm->SetIsAiming( true );
    }
}

void UGameplayAbility_Aim::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo ) {

    if ( ActorInfo != NULL && ActorInfo->AvatarActor != NULL ) {
        CancelAbility( Handle, ActorInfo, ActivationInfo, true );
    }
}

void UGameplayAbility_Aim::CancelAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility ) {
    if ( ScopeLockCount > 0 ) {
        WaitingToExecute.Add( FPostLockDelegate::CreateUObject(
            this, &UGameplayAbility_Aim::CancelAbility, Handle, ActorInfo,
            ActivationInfo, bReplicateCancelAbility ) );
        return;
    }

    Super::CancelAbility( Handle, ActorInfo, ActivationInfo,
                          bReplicateCancelAbility );

    // TODO: Implement aim end

    springArm->SetIsAiming( false );
}