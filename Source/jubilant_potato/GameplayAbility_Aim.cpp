// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility_Aim.h"

#include "PlayerCharacter.h"
#include "SmartSpringArm.h"

#include "Abilities/Tasks/AbilityTask_Repeat.h"

#include "Components/SplineMeshComponent.h"

UGameplayAbility_Aim::UGameplayAbility_Aim( class FObjectInitializer const& ) {}

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

        // TODO: Implement camera
        if ( VerifySpringArm( ActorInfo ) ) springArm->SetIsAiming( true );

        if ( VerifyLaserSpline( ActorInfo ) ) {
            laserSpline->SetVisibleFlag( true );
            laserSpline->SetHiddenInGame( false );
        }

        if ( VerifyCharacter( ActorInfo ) ) character->SetStrafe( true );
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

    // TODO: Implement camera end
    if ( VerifySpringArm( ActorInfo ) ) springArm->SetIsAiming( false );

    if ( VerifyLaserSpline( ActorInfo ) ) {
        laserSpline->SetVisibleFlag( false );
        laserSpline->SetHiddenInGame( true );
    }

    if ( VerifyCharacter( ActorInfo ) ) character->SetStrafe( false );

    // UAbilityTask_Repeat* task = UAbilityTask_Repeat::();
}

bool UGameplayAbility_Aim::VerifySpringArm(
    const FGameplayAbilityActorInfo* ActorInfo ) {
    if ( IsValid( springArm ) ) return true;

    VerifyCharacter( ActorInfo );

    springArm = character->springArm;

    if ( IsValid( springArm ) )
        return true;
    else
        return false;
}

bool UGameplayAbility_Aim::VerifyLaserSpline(
    const FGameplayAbilityActorInfo* ActorInfo ) {
    if ( IsValid( laserSpline ) )
        return true;
    else
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "BAD" );

    VerifyCharacter( ActorInfo );

    TArray< USplineMeshComponent* > Splines;
    character->GetComponents< USplineMeshComponent >( Splines );

    for ( USplineMeshComponent* Spline : Splines ) {
        if ( Spline->ComponentHasTag( "Laser" ) ) laserSpline = Spline;
    }

    // UActorComponent* comp = character->FindComponentByClass(
    //     TSubclassOf< USplineMeshComponent >() );

    // laserSpline = Cast< USplineMeshComponent >( comp );

    if ( IsValid( laserSpline ) )
        return true;
    else
        return false;
}

bool UGameplayAbility_Aim::VerifyCharacter(
    const FGameplayAbilityActorInfo* ActorInfo ) {

    character = CastChecked< APlayerCharacter >(
        ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed );

    if ( IsValid( character ) )
        return true;
    else
        return false;
}
