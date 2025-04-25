
#include "TargetSystem.h"
#include "PlayerCharacter.h"        // APlayerCharacter class
#include "Enemy.h"                  // AEnemy class
#include "Kismet/GameplayStatics.h" // GetAllActorsOfClass()
#include "Camera/CameraComponent.h" // UCameraComponent class

UTargetSystem::UTargetSystem() {
    PrimaryComponentTick.bCanEverTick = false;
    // ...
}

void UTargetSystem::BeginPlay() {
    Super::BeginPlay();
    // ...

    parent = Cast< APlayerCharacter >( GetOwner() );
    world = GetWorld();

    gimbal = parent->GetGimbal();
}

void UTargetSystem::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}

void UTargetSystem::UpdateTarget( float Width, float Range,
                                  bool SingleTarget ) {
    //
    float searchValue = 1.f - Width;

    FVector input = parent->GetLastMovementInput();

    // Determine the direction to look based off movement inputs (if given)
    FVector searchDirection = gimbal->GetForwardVector();
    if ( abs( input.X ) + abs( input.Y ) > 0.f ) {
        searchDirection = ( ( gimbal->GetForwardVector() * input.Y ) +
                            ( gimbal->GetRightVector() * input.X ) )
                              .GetSafeNormal();
    }

    // Draw search direction
    DrawDebugDirectionalArrow(
        GetWorld(), parent->GetActorLocation(),
        parent->GetActorLocation() + ( searchDirection * Range ), 10.f,
        FColor::Green, false, 1.f, ( uint8 )0U, 2.f );

    // Get list of all enemies in world
    TArray< AActor* > foundEnemies;
    UGameplayStatics::GetAllActorsOfClass( world, AEnemy::StaticClass(),
                                           foundEnemies );

    // Determine the dot between search direction and direction to enemy
    TArray< float > found_dot_result;
    found_dot_result.Init( -1.f, 1 );
    TArray< int > found_enemy_index;
    found_enemy_index.Init( -1, 1 );
    for ( int i = 0; i < foundEnemies.Num(); ++i ) {
        AEnemy* enemy = Cast< AEnemy >( foundEnemies[i] );

        float distance = FVector::Distance( enemy->GetActorLocation(),
                                            parent->GetActorLocation() );

        if ( distance > Range || enemy->IsDead() ) {
            if ( enemy->GetIsTargeted() ) {
                enemy->EndTarget();
            }

            continue;
        }

        FVector enemyDirection =
            ( enemy->GetActorLocation() - parent->GetActorLocation() )
                .GetSafeNormal();

        float dot_result =
            FVector::DotProduct( searchDirection, enemyDirection );

        if ( dot_result > searchValue ) {
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
        return;
    }

    for ( int index : found_enemy_index ) {
        curr_targets.Add( Cast< AEnemy >( foundEnemies[index] ) );
        curr_targets.Last()->StartTarget();
    }
}

TArray< AEnemy* >& UTargetSystem::GetTargets() { return curr_targets; }

void UTargetSystem::ClearTargets() {
    for ( AEnemy* target : curr_targets ) {
        target->EndTarget();
    }

    curr_targets.Empty();
}
