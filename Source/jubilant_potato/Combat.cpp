// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat.h"
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "ActionManager.h"                            // UActionManager class
#include "Animation/AnimMontage.h"                    // UAnimMontage class
#include "TimerManager.h"                             // SetTimer()
#include "TargetSystem.h"                             // UTargetSystem class
#include "Enemy.h"                                    // AEnemy class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class

// Sets default values for this component's properties
UCombat::UCombat() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_Attack;
    // ...
}

void UCombat::BeginPlay() {
    Super::BeginPlay();
    // ...

    target_system = parent->FindComponentByClass< UTargetSystem >();
}

void UCombat::Start( const FInputActionValue &Value ) {
    if ( on_cooldown ) {
        return;
    }

    if ( !manager->StartAction( type ) && attack_count == 0 ) {
        return;
    }

    if ( parent->GetCharacterMovement()->IsFalling() ) {
        can_combo = false;
        is_attacking = true;

        parent->SetCanWalk( false );

        return;
    }

    if ( attack_count == 0 || can_combo ) {
        attack_count += 1;
        can_combo = false;
        is_attacking = true;

        TArray< AEnemy * > targets = target_system->UpdateTarget( 0.1f, 1000.f, true );
        int target_num = targets.Num();
        if ( target_num > 0 ) {
            FVector average_position = FVector( 0 );
            for ( AEnemy *target : targets ) {
                average_position += target->GetActorLocation();
                target->ApplyDamage( damage_amount );
            }

            average_position /= target_num;
            parent->SetActorRotation( ( average_position - parent->GetActorLocation() ).Rotation() );
        }

        if ( attack_count == 1 ) {
            parent->SetCanWalk( false );
            parent->PlayAnimMontage( attack_montages[attack_count - 1] );
        }

        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start attack " + FString::FromInt( attack_count ) + "." );
    }
}

void UCombat::End() {
    Super::End();

    target_system->ClearTargets();

    parent->SetCanWalk( true );

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

void UCombat::SetCanCombo( bool CanCombo ) {
    if ( CanCombo ) {
        start_combo_count = attack_count;
    } else if ( start_combo_count == attack_count || attack_count > attack_montages.Num() ) {
        End();
    } else if ( !CanCombo ) {
        if ( attack_count <= attack_montages.Num() ) {
            parent->PlayAnimMontage( attack_montages[attack_count - 1] );
        }
    }

    can_combo = CanCombo;
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
