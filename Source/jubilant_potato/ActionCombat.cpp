// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionCombat.h"
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "ActionManager.h"                            // UActionManager class
#include "Animation/AnimMontage.h"                    // UAnimMontage class
#include "TimerManager.h"                             // SetTimer()
#include "TargetSystem.h"                             // UTargetSystem class
#include "Enemy.h"                                    // AEnemy class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "ActionAim.h"                                // UActionAim class
#include "Base_Projectile.h"                          // ABase_Projectile class
#include "Camera/CameraComponent.h"                   // UCameraComponent class
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h" // UProjectileMovementComponent class

#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"  // UEnhancedInputComponent class

#include "AttackData.h"

// Sets default values for this component's properties
UActionCombat::UActionCombat() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_Attack;
    // ...
}

void UActionCombat::BeginPlay() {
    Super::BeginPlay();
    // ...

    targetSystem = parent->FindComponentByClass< UTargetSystem >();

    world = GetWorld();
    movement = parent->GetCharacterMovement();

    TArray< UAction* > action_components;
    parent->GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        if ( action->type == EAction::A_Aim ) {
            aimAction = Cast< UActionAim >( action );
        }
    }
}

void UActionCombat::BindAction( UEnhancedInputComponent* PEI ) {
    AttackList = AttackDataAsset->GetAttackInfo().Array();

    unsigned i = 0;
    for ( auto& Attack : AttackList ) {
        if ( Attack.InputAction ) {
            PEI->BindActionInstanceLambda(
                Attack.InputAction, ETriggerEvent::Triggered,
                [this, Attack,
                 i]( const FInputActionInstance& ActionInstance ) {
                    if ( onCooldown ||
                         ( currAttackId != -1 && currAttackId != i ) ) {
                        return;
                    }

                    auto& AttackInfo = AttackList[i];
                    if ( AttackInfo.hasTriggerConditions ) {
                        if ( AttackInfo.hasVelocityCondition &&
                             AttackInfo.minVelocity >
                                 parent->GetVelocity().Length() ) {
                            return;
                        } else if ( AttackInfo.hasInAirCondition &&
                                    !movement->IsFalling() ) {
                            return;
                        }
                    }

                    currAttackId = i;
                    Start( ActionInstance.GetValue() );
                } );
        }
        ++i;
    }
}

void UActionCombat::Start( const FInputActionValue& Value ) {
    if ( onCooldown || movement->IsFalling() || aimAction->GetIsAiming() ) {
        return;
    }

    if ( ( attackCount == 0 || canCombo ) &&
         AttackList[currAttackId].attacks.Num() >= attackCount + 1 ) {
        attackCount += 1;
        canCombo = false;
        isAttacking = true;

        UpdatePlayerRotation();

        if ( attackCount == 1 ) {
            parent->PlayAnimMontage( AttackList[currAttackId]
                                         .attacks[attackCount - 1]
                                         .attackMontage );
        }
    }
}

void UActionCombat::End() {
    Super::End();

    // parent->SetCanWalk( true );]

    if ( IsValid( currTarget ) ) {
        currTarget->EndTarget();
        currTarget = nullptr;
    }

    attackCount = 0;
    canCombo = false;
    isAttacking = false;

    onCooldown = true;
    parent->GetWorldTimerManager().SetTimer(
        cooldownTimer, this, &UActionCombat::EndCooldown,
        AttackList[currAttackId].cooldown, false );

    currAttackId = -1;
}

void UActionCombat::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                      FString::FromInt( attackCount ) );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red,
                                      FString::FromInt( currAttackId ) );
}

void UActionCombat::StartCanComboWindow() {
    startComboCount = attackCount;
    startComboId = currAttackId;
    canCombo = true;

    parent->GetWorldTimerManager().SetTimer(
        comboWindowTimer, this, &UActionCombat::EndCanComboWindow,
        AttackList[currAttackId].comboWindow, false );
}

void UActionCombat::EndCanComboWindow() {
    const int32 comboLength = AttackList[currAttackId].attacks.Num();

    const bool shouldEndAttack =
        ( startComboCount == attackCount || attackCount > comboLength );

    if ( startComboId != currAttackId || ( isAttacking && shouldEndAttack ) ) {
        End();
        return;
    }

    parent->PlayAnimMontage(
        AttackList[currAttackId].attacks[attackCount - 1].attackMontage );

    canCombo = false;
}

void UActionCombat::SetCanCombo( bool CanCombo ) {
    const int32 comboLength = AttackList[currAttackId].attacks.Num();

    if ( CanCombo ) {
        startComboCount = attackCount;
        startComboId = currAttackId;
    } else if ( startComboId != currAttackId ||
                ( isAttacking && ( startComboCount == attackCount ||
                                   attackCount > comboLength ) ) ) {
        End();
        return;
    } else if ( !CanCombo ) {
        parent->PlayAnimMontage(
            AttackList[currAttackId].attacks[attackCount - 1].attackMontage );
    }

    canCombo = CanCombo;
}

int UActionCombat::GetAttackCount() const { return attackCount; }

bool UActionCombat::GetIsAttacking() const { return isAttacking; }

void UActionCombat::EndCooldown() { onCooldown = false; }

void UActionCombat::TargetEnemy( AEnemy* Target ) {
    if ( currTarget != Target && IsValid( currTarget ) ) {
        currTarget->EndTarget();
    }
    currTarget = Target;
    Target->StartTarget();
}

void UActionCombat::UpdatePlayerRotation() {
    FVector input = parent->GetLastMovementInput();
    FVector searchDirection;

    if ( abs( input.X ) + abs( input.Y ) > 0.f ) {
        searchDirection = ( ( parent->gimbal->GetForwardVector() * input.Y ) +
                            ( parent->gimbal->GetRightVector() * input.X ) )
                              .GetSafeNormal();
    } else {
        searchDirection = parent->gimbal->GetForwardVector();
    }
    parent->SetActorRotation( searchDirection.Rotation() );
}

void UActionCombat::HitTargets() {

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Cyan,
                                      "Current attack: " +
                                          FString::FromInt( currAttackId ) );

    targetSystem->UpdateTarget(
        AttackList[currAttackId].attacks[attackCount - 1].hitSize,
        AttackList[currAttackId].attacks[attackCount - 1].hitRange, false );

    TArray< AEnemy* >& targets = targetSystem->GetTargets();

    for ( auto* target : targets ) {
        target->ApplyDamage( damage_amount );
    }

    targetSystem->ClearTargets();
}
