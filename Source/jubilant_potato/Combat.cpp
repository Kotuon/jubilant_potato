// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat.h"
#include "PlayerCharacter.h"       // APlayerCharacter class
#include "ActionManager.h"         // UActionManager class
#include "Animation/AnimMontage.h" // UAnimMontage class
#include "TimerManager.h"          // SetTimer()

// Sets default values for this component's properties
UCombat::UCombat() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_Attack;
    // ...
}

void UCombat::BeginPlay() {
    Super::BeginPlay();
    // ...
}

void UCombat::Start( const FInputActionValue &value ) {
    if ( on_cooldown ) {
        return;
    }

    if ( !manager->StartAction( type ) && attack_count == 0 ) {
        return;
    }

    if ( attack_count == 0 || can_combo ) {
        attack_count += 1;
        can_combo = false;
        is_attacking = true;

        if ( attack_count == 1 ) {
            parent->PlayAnimMontage( attack_montages[attack_count - 1] );
        }

        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start attack " + FString::FromInt( attack_count ) + "." );
    }
}

void UCombat::End() {
    Super::End();

    attack_count = 0;
    can_combo = false;
    is_attacking = false;

    on_cooldown = true;
    parent->GetWorldTimerManager().SetTimer( cooldown_timer, this, &UCombat::EndCooldown, cooldown, false );

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "End attack." );
}

void UCombat::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}

void UCombat::SetCanCombo( bool canCombo ) {
    if ( canCombo ) {
        start_combo_count = attack_count;
    } else if ( start_combo_count == attack_count || attack_count > attack_montages.Num() ) {
        End();
    } else if ( !canCombo ) {
        if ( attack_count <= attack_montages.Num() ) {
            parent->PlayAnimMontage( attack_montages[attack_count - 1] );
        }
    }

    can_combo = canCombo;
}

int UCombat::GetAttackCount() const {
    return attack_count;
}

bool UCombat::GetIsAttacking() const {
    return is_attacking;
}

void UCombat::EndCooldown() {
    on_cooldown = false;

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "End cooldown." );
}
