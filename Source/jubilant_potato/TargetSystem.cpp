
#include "TargetSystem.h"
#include "PlayerCharacter.h"        // APlayerCharacter class
#include "Enemy.h"                  // AEnemy class
#include "Kismet/GameplayStatics.h" // GetAllActorsOfClass()

UTargetSystem::UTargetSystem() {
    PrimaryComponentTick.bCanEverTick = false;
    // ...
}

void UTargetSystem::BeginPlay() {
    Super::BeginPlay();
    // ...

    parent = Cast< APlayerCharacter >( GetOwner() );
    world = GetWorld();
}

void UTargetSystem::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}

TArray< AEnemy* >& UTargetSystem::UpdateTarget( float Width, float Range, bool SingleTarget ) {
    float search_value = 1.f - Width;
    FVector input = parent->GetLastMovementInput();

    FVector search_direction;

    if ( abs( input.X ) + abs( input.Y ) > 0.f ) {
        search_direction = ( ( parent->gimbal->GetForwardVector() * input.Y ) + ( parent->gimbal->GetRightVector() * input.X ) ).GetSafeNormal();
    } else {
        search_direction = parent->GetActorForwardVector();
    }

    DrawDebugDirectionalArrow( GetWorld(), parent->GetActorLocation(),
                               parent->GetActorLocation() + ( search_direction * 200.f ), 10.f,
                               FColor::Green, false, 1.f, ( uint8 )0U, 2.f );

    // DrawDebugDirectionalArrow( GetWorld(), parent->GetActorLocation(),
    //                            parent->GetActorLocation() + ( search_direction * 200.f ),
    //                            2.f, );

    TArray< AActor* > found_enemies;
    UGameplayStatics::GetAllActorsOfClass( world, AEnemy::StaticClass(), found_enemies );

    TArray< float > found_dot_result;
    found_dot_result.Init( -1.f, 1 );
    TArray< int > found_enemy_index;
    found_enemy_index.Init( -1, 1 );
    for ( int i = 0; i < found_enemies.Num(); ++i ) {
        AEnemy* enemy = Cast< AEnemy >( found_enemies[i] );

        float distance = FVector::Distance( enemy->GetActorLocation(), parent->GetActorLocation() );

        if ( distance > Range || enemy->IsDead() ) {
            if ( enemy->GetIsTargeted() ) {
                enemy->EndTarget();
            }

            continue;
        }

        FVector enemy_direction = ( enemy->GetActorLocation() - parent->GetActorLocation() ).GetSafeNormal();

        float dot_result = FVector::DotProduct( search_direction, enemy_direction );
        if ( abs( dot_result ) > search_value ) {
            if ( SingleTarget ) {
                if ( dot_result > found_dot_result[0] ) {
                    found_dot_result[0] = dot_result;
                    found_enemy_index[0] = i;
                }
                if ( curr_targets.Num() > 0 && enemy == curr_targets[0] ) {
                    found_dot_result[0] = dot_result;
                    found_enemy_index[0] = i;
                    break;
                }
            } else if ( found_enemy_index[0] == -1 ) {
                found_dot_result[0] = dot_result;
                found_enemy_index[0] = i;
            } else {
                found_dot_result.Emplace( dot_result );
                found_enemy_index.Emplace( i );
            }
        } else {
            if ( enemy->GetIsTargeted() ) {
                enemy->EndTarget();
            }
        }
    }

    curr_targets.Empty();
    if ( found_enemy_index[0] == -1 ) {
        return curr_targets;
    }

    for ( int index : found_enemy_index ) {
        curr_targets.Add( Cast< AEnemy >( found_enemies[index] ) );
        curr_targets.Last()->StartTarget();
    }

    return curr_targets;
}

void UTargetSystem::ClearTargets() {
    for ( AEnemy* target : curr_targets ) {
        target->EndTarget();
    }

    curr_targets.Empty();
}
