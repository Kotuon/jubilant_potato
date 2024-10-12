// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat.h"
#include "PlayerCharacter.h"                          // APlayerCharacter class
#include "ActionManager.h"                            // UActionManager class
#include "Animation/AnimMontage.h"                    // UAnimMontage class
#include "TimerManager.h"                             // SetTimer()
#include "TargetSystem.h"                             // UTargetSystem class
#include "Enemy.h"                                    // AEnemy class
#include "GameFramework/CharacterMovementComponent.h" // UCharacterMovementComponent class
#include "Aim.h"                                      // UAim class
#include "Base_Projectile.h"                          // ABase_Projectile class
#include "Camera/CameraComponent.h"                   // UCameraComponent class

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
    world = GetWorld();

    TArray< UAction* > action_components;
    parent->GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        if ( action->type == EAction::A_Aim ) {
            aim_action = Cast< UAim >( action );
        }
    }
}

void UCombat::Start( const FInputActionValue& Value ) {
    if ( on_cooldown || parent->GetCharacterMovement()->IsFalling() ) {
        return;
    }

    if ( !manager->StartAction( type ) && attack_count == 0 ) {
        return;
    }

    // if ( parent->GetCharacterMovement()->IsFalling() ) {
    //     can_combo = false;
    //     is_attacking = true;

    //     parent->SetCanWalk( false );

    //     return;
    // }

    if ( aim_action->GetIsAiming() ) {
        SpawnProjectile();

        End();
        return;
    }

    if ( attack_count == 0 || can_combo ) {
        attack_count += 1;
        can_combo = false;
        is_attacking = true;

        TArray< AEnemy* > targets = target_system->UpdateTarget( 0.15f, 500.f, false );
        int target_num = targets.Num();
        if ( target_num > 0 ) {
            FVector average_position = FVector( 0 );
            for ( AEnemy* target : targets ) {
                average_position += target->GetActorLocation();
                target->ApplyDamage( damage_amount );
            }

            average_position /= target_num;
            parent->SetActorRotation( ( average_position - parent->GetActorLocation() ).Rotation() );
        }

        if ( attack_count == 1 ) {
            // parent->SetCanWalk( false );
            parent->PlayAnimMontage( attack_montages[attack_count - 1] );
        }

        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Start attack " + FString::FromInt( attack_count ) + "." );
    }
}

void UCombat::End() {
    Super::End();

    target_system->ClearTargets();

    // parent->SetCanWalk( true );

    attack_count = 0;
    can_combo = false;
    is_attacking = false;

    on_cooldown = true;
    parent->GetWorldTimerManager().SetTimer( cooldown_timer, this, &UCombat::EndCooldown, cooldown, false );

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "End attack." );
}

void UCombat::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
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

void UCombat::SpawnProjectile() {
    FVector playerPosition = parent->GetActorLocation() + FVector{ 0.f, 0.f, 40.f };
    FVector cameraForward = Cast< USceneComponent >( parent->camera )->GetForwardVector();
    FRotator playerRotation = parent->GetActorRotation();

    FVector initialLocation = FVector( 100000000000, 10000000000000, 10000000000 );

    FVector traceStart = parent->camera->GetComponentLocation();
    FVector traceEnd = traceStart + ( cameraForward * 10000.f );

    // DrawDebugLine( world, traceStart, traceEnd, FColor::Green, false, 2.f );

    FCollisionShape sphere;
    sphere.SetSphere( 20.f );

    FHitResult hitResult;
    world->SweepSingleByChannel( hitResult, traceStart, traceEnd, FQuat::Identity,
                                 ECollisionChannel::ECC_PhysicsBody, sphere );

    if ( hitResult.IsValidBlockingHit() ) {
        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Yellow, hitResult.GetActor()->GetName() );
        traceEnd = hitResult.ImpactPoint;
    }

    FVector shotLocation = playerPosition + ( cameraForward * start_distance_projectile );
    FVector launchDirection = ( traceEnd - shotLocation ).GetSafeNormal();

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = parent;
    spawnParams.Instigator = parent->GetInstigator();
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABase_Projectile* newProjectile = world->SpawnActor< ABase_Projectile >( projectile_class,
                                                                             initialLocation,
                                                                             FRotationMatrix::MakeFromX( launchDirection ).Rotator(),
                                                                             spawnParams );
    if ( !newProjectile ) {
        return;
    }
    newProjectile->parent = parent;
    newProjectile->Tags.Add( parent->Tags[0] );
    newProjectile->start_location = shotLocation;

    newProjectile->SetActorLocation( shotLocation );
    newProjectile->FireInDirection( launchDirection );
}
