// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbilityTask_Aim.h"
#include "GameplayAbility_Aim.h"

UGameplayAbilityTask_Aim::UGameplayAbilityTask_Aim(
    const FObjectInitializer& ObjectInitializer ) {
    bTickingTask = true;
}

UGameplayAbilityTask_Aim*
UGameplayAbilityTask_Aim::AbilityTaskOnTick( UGameplayAbility* OwningAbility,
                                             FName TaskInstanceName ) {

    GEngine->AddOnScreenDebugMessage( -1, 2.f, FColor::Yellow,
                                      "Created task." );
    return NewAbilityTask< UGameplayAbilityTask_Aim >( OwningAbility );
}

void UGameplayAbilityTask_Aim::Activate() {
    GEngine->AddOnScreenDebugMessage( -1, 2.f, FColor::Yellow,
                                      "Activate task." );
    Super::Activate();
}

void UGameplayAbilityTask_Aim::TickTask( float DeltaTime ) {
    GEngine->AddOnScreenDebugMessage( -1, 2.f, FColor::Yellow,
                                      "Ticking task." );
    Super::TickTask( DeltaTime );
    //...
    if ( ShouldBroadcastAbilityTaskDelegates() ) {
        OnTick.Broadcast( DeltaTime );
    }
}
