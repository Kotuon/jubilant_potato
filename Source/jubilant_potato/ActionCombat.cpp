// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionCombat.h"
#include "PlayerCharacter.h"       // APlayerCharacter class
#include "ActionManager.h"         // UActionManager class
#include "Animation/AnimMontage.h" // UAnimMontage class
#include "TimerManager.h"          // SetTimer()
#include "TargetSystem.h"          // UTargetSystem class
#include "Enemy.h"                 // AEnemy class
#include "GravMovementComponent.h"
#include "ActionAim.h"              // UActionAim class
#include "Base_Projectile.h"        // ABase_Projectile class
#include "Camera/CameraComponent.h" // UCameraComponent class
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
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
    movement = Cast< UGravMovementComponent >( parent->GetCharacterMovement() );
    gimbal = parent->GetGimbal();

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
    // Getting input vector {x: left/right, y: forward/backward, z: isJumping}
    FVector input = parent->GetLastMovementInput();

    // fwd vector equals (0.f, 1.0)
    float dot = input.Y; // input.X * fwd.x + input.Y * fwd.y
    float det = input.X; // input.X * fwd.y - input.Y * fwd.x

    float angle = UKismetMathLibrary::Atan2( det, dot );

    // Up vector from the player model.
    const FQuat& gimbalQuat = gimbal->GetComponentQuat();

    // Quaternion for applying yaw rotation to player
    const FQuat addQuat =
        FQuat( gimbal->GetUpVector().GetSafeNormal(), angle ).GetNormalized();

    // Calculate new player rotation
    const FQuat outputQuat = addQuat * gimbalQuat;

    // Debug messaging
    {
        // GEngine->AddOnScreenDebugMessage(
        //     -1, 5.f, FColor::Green, "Adding rotation: " + addQuat.ToString()
        //     );

        // UE_LOG( LogTemp, Display, TEXT( "Adding quaternion: %s" ),
        //         *addQuat.ToString() );
        // UE_LOG( LogTemp, Display, TEXT( "Gimbal quaternion: %s" ),
        //         *gimbalQuat.ToString() );
        // UE_LOG( LogTemp, Display, TEXT( "Output quaternion: %s" ),
        //         *outputQuat.ToString() );
        // UE_LOG( LogTemp, Display, TEXT( "Gimbal rotator: %s" ),
        //         *parent->gimbal->GetComponentRotation().ToString() );
    }

    // Update player rotation
    parent->SetActorRotation( outputQuat );
}

void UActionCombat::HitTargets() {
    targetSystem->UpdateTarget(
        AttackList[currAttackId].attacks[attackCount - 1].hitSize,
        AttackList[currAttackId].attacks[attackCount - 1].hitRange, false );

    TArray< AEnemy* >& targets = targetSystem->GetTargets();

    for ( auto* target : targets ) {
        target->ApplyDamage( damage_amount );
    }

    targetSystem->ClearTargets();
}
