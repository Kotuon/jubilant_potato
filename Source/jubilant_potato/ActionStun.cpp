// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionStun.h"

#include "PlayerCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Enemy.h" // AEnemy class
#include "GameFramework/CharacterMovementComponent.h"

UActionStun::UActionStun() { type = EAction::A_Attack; }

void UActionStun::BeginPlay() {
    Super::BeginPlay();
    //...

    OriginalJumpSpeed = parent->GetCharacterMovement()->JumpZVelocity;
    world = GetWorld();
}

void UActionStun::Start( const FInputActionValue& Value ) {
    if ( onCooldown ) return;
    onCooldown = true;

    parent->GetCharacterMovement()->JumpZVelocity = 400.f;
    parent->Jump();

    parent->PlayAnimMontage( stunAttackMontage );
}

void UActionStun::End() { Super::End(); }

void UActionStun::EndCooldown() { onCooldown = false; }

void UActionStun::TriggerDamage() {
    parent->GetCharacterMovement()->JumpZVelocity = OriginalJumpSpeed;

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = parent;
    spawnParams.Instigator = parent->GetInstigator();
    spawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* effect = world->SpawnActor< AActor >(
        effect_class, parent->GetActorLocation() - FVector::UpVector * 88.f,
        FVector::UpVector.Rotation(), spawnParams );

    TArray< TEnumAsByte< EObjectTypeQuery > > objectTypes;
    objectTypes.Add( UEngineTypes::ConvertToObjectType( ECC_Pawn ) );

    TArray< AActor*, FDefaultAllocator > ignoreTypes;
    ignoreTypes.Add( parent );

    TArray< AActor* > targets;
    UKismetSystemLibrary::SphereOverlapActors(
        world, parent->GetActorLocation(), range, objectTypes,
        TSubclassOf< class AEnemy >(), ignoreTypes, targets );

    DrawDebugSphere( world, parent->GetActorLocation(), range / 2.f, 16,
                     FColor::Green, false, 2.f, ( uint8 )0U, 1.f );

    for ( AActor* target : targets ) {
        AEnemy* enemy = Cast< AEnemy >( target );
        enemy->ApplyDamage( 1 );
    }

    parent->GetWorldTimerManager().SetTimer(
        cooldownTimer, this, &UActionStun::EndCooldown, cooldown, false );
}
