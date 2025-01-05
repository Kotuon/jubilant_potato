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

// Sets default values for this component's properties
UActionCombat::UActionCombat() {
    PrimaryComponentTick.bCanEverTick = true;
    type = EAction::A_Attack;
    // ...
}

void UActionCombat::BeginPlay() {
    Super::BeginPlay();
    // ...

    world = GetWorld();

    TArray< UAction* > action_components;
    parent->GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        if ( action->type == EAction::A_Aim ) {
            aimAction = Cast< UActionAim >( action );
        }
    }
}

void UActionCombat::Start( const FInputActionValue& Value ) {
    if ( onCooldown || parent->GetCharacterMovement()->IsFalling() ) {
        return;
    }

    // if ( isAttacking && attackCount == 0 ) {
    //     return;
    // }

    // if ( parent->GetCharacterMovement()->IsFalling() ) {
    //     canCombo = false;
    //     isAttacking = true;

    //     parent->SetCanWalk( false );

    //     return;
    // }

    // if ( aimAction->GetIsAiming() ) {
    //     SpawnProjectile();

    //     End();
    //     return;
    // }

    if ( attackCount == 0 || canCombo ) {
        attackCount += 1;
        canCombo = false;
        isAttacking = true;

        TArray< TEnumAsByte< EObjectTypeQuery > > objectTypes;
        objectTypes.Add( UEngineTypes::ConvertToObjectType( ECC_Pawn ) );

        TArray< AActor*, FDefaultAllocator > ignoreTypes;
        ignoreTypes.Add( parent );

        FVector start = parent->camera->GetComponentLocation();
        FVector end = start + ( parent->camera->GetForwardVector() * 2000.f );

        FHitResult hitResult;
        UKismetSystemLibrary::SphereTraceSingleForObjects(
            world, start, end, 50.f, objectTypes, false, ignoreTypes,
            EDrawDebugTrace::None, hitResult, false );

        if ( hitResult.bBlockingHit ) {
            if ( hitResult.GetActor()->ActorHasTag( "Enemy" ) ) {
                AEnemy* target = Cast< AEnemy >( hitResult.GetActor() );
                // target->ApplyDamage( damage_amount );
                if ( currTarget != target && IsValid( currTarget ) ) {
                    currTarget->EndTarget();
                }
                currTarget = target;
                target->StartTarget();
            }

            parent->SetActorRotation( end.Rotation() );
        } else {
            if ( IsValid( currTarget ) ) {
                currTarget->EndTarget();
                currTarget = nullptr;
            }

            FVector input = parent->GetLastMovementInput();

            FVector searchDirection;

            if ( abs( input.X ) + abs( input.Y ) > 0.f ) {
                searchDirection =
                    ( ( parent->gimbal->GetForwardVector() * input.Y ) +
                      ( parent->gimbal->GetRightVector() * input.X ) )
                        .GetSafeNormal();
            } else {
                searchDirection = parent->gimbal->GetForwardVector();
            }
            parent->SetActorRotation( searchDirection.Rotation() );
        }

        if ( attackCount == 1 ) {
            // parent->SetCanWalk( false );
            parent->PlayAnimMontage( attack_montages[attackCount - 1] );
        }

        GEngine->AddOnScreenDebugMessage(
            -1, 5.f, FColor::Green,
            "Start attack " + FString::FromInt( attackCount ) + "." );
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
        cooldownTimer, this, &UActionCombat::EndCooldown, cooldown, false );

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "End attack." );
}

void UActionCombat::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}

void UActionCombat::SetCanCombo( bool CanCombo ) {
    if ( CanCombo ) {
        startComboCount = attackCount;
    } else if ( startComboCount == attackCount ||
                attackCount > attack_montages.Num() ) {
        End();
    } else if ( !CanCombo ) {
        if ( attackCount <= attack_montages.Num() ) {
            parent->PlayAnimMontage( attack_montages[attackCount - 1] );
        }
    }

    canCombo = CanCombo;
}

int UActionCombat::GetAttackCount() const { return attackCount; }

bool UActionCombat::GetIsAttacking() const { return isAttacking; }

void UActionCombat::EndCooldown() {
    onCooldown = false;

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "End cooldown." );
}

void UActionCombat::SpawnProjectile( FVector SocketLocation,
                                     FRotator SocketNormal ) {
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = parent;
    spawnParams.Instigator = parent->GetInstigator();
    spawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABase_Projectile* newProjectile = world->SpawnActor< ABase_Projectile >(
        projectile_class, SocketLocation, SocketNormal, spawnParams );
    if ( !newProjectile ) {
        return;
    }
    newProjectile->parent = parent;
    newProjectile->Tags.Add( parent->Tags[0] );
    newProjectile->start_location = SocketLocation;

    newProjectile->SetActorLocation( SocketLocation );
    newProjectile->FireInDirection( SocketNormal.Vector() );

    if ( IsValid( currTarget ) ) {
        newProjectile->projectile_movement_component->bIsHomingProjectile =
            true;

        newProjectile->projectile_movement_component->HomingTargetComponent =
            currTarget->GetRootComponent();

        GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Yellow, "HOMING" );
    }

    /////////////////////////////////
    // FVector playerPosition =
    //     parent->GetActorLocation() + FVector{ 0.f, 0.f, 40.f };
    // FVector cameraForward =
    //     Cast< USceneComponent >( parent->camera )->GetForwardVector();
    // FRotator playerRotation = parent->GetActorRotation();

    // FVector initialLocation =
    //     FVector( 100000000000, 10000000000000, 10000000000 );

    // FVector traceStart = parent->camera->GetComponentLocation();
    // FVector traceEnd = traceStart + ( cameraForward * 10000.f );

    // // DrawDebugLine( world, traceStart, traceEnd, FColor::Green, false, 2.f
    // // );

    // FCollisionShape sphere;
    // sphere.SetSphere( 20.f );

    // FHitResult hitResult;
    // world->SweepSingleByChannel( hitResult, traceStart, traceEnd,
    //                              FQuat::Identity,
    //                              ECollisionChannel::ECC_PhysicsBody, sphere
    //                              );

    // if ( hitResult.IsValidBlockingHit() ) {
    //     GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Yellow,
    //                                       hitResult.GetActor()->GetName() );
    //     traceEnd = hitResult.ImpactPoint;
    // }

    // FVector shotLocation =
    //     playerPosition + ( cameraForward * start_distance_projectile );
    // FVector launchDirection = ( traceEnd - shotLocation ).GetSafeNormal();

    // FActorSpawnParameters spawnParams;
    // spawnParams.Owner = parent;
    // spawnParams.Instigator = parent->GetInstigator();
    // spawnParams.SpawnCollisionHandlingOverride =
    //     ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // ABase_Projectile* newProjectile = world->SpawnActor< ABase_Projectile >(
    //     projectile_class, initialLocation,
    //     FRotationMatrix::MakeFromX( launchDirection ).Rotator(), spawnParams
    //     );
    // if ( !newProjectile ) {
    //     return;
    // }
    // newProjectile->parent = parent;
    // newProjectile->Tags.Add( parent->Tags[0] );
    // newProjectile->start_location = shotLocation;

    // newProjectile->SetActorLocation( shotLocation );
    // newProjectile->FireInDirection( launchDirection );
}
