
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
    search_value = 1.f - detection_size;
}

void UTargetSystem::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    // ...
}

void UTargetSystem::UpdateTarget() {
    FVector input = parent->GetLastMovementInput();

    FVector search_direction;

    if ( abs( input.X ) + abs( input.Y ) > 0.f ) {
        search_direction = ( ( parent->gimbal->GetForwardVector() * input.Y ) + ( parent->gimbal->GetRightVector() * input.X ) ).GetSafeNormal();
    } else {
        search_direction = parent->gimbal->GetForwardVector();
    }

    TArray< AActor * > found_enemies;
    UGameplayStatics::GetAllActorsOfClass( world, AEnemy::StaticClass(), found_enemies );

    float found_dot_result = -1.f;
    int found_enemy_index = -1;
    for ( int i = 0; i < found_enemies.Num(); ++i ) {
        AActor *enemy = found_enemies[i];
        FVector enemy_direction = ( enemy->GetActorLocation() - parent->GetActorLocation() ).GetSafeNormal();

        float dot_result = FVector::DotProduct( search_direction, enemy_direction );
        if ( dot_result > search_value ) {
            if ( dot_result > found_dot_result ) {
                found_dot_result = dot_result;
                found_enemy_index = i;
            }
            if ( enemy == curr_target ) {
                found_dot_result = dot_result;
                found_enemy_index = i;
                break;
            }
        }
    }

    if ( found_enemy_index == -1 ) {
        if ( curr_target ) {
            curr_target->EndTarget();
        }
        curr_target = nullptr;

        return;
    }

    if ( curr_target != found_enemies[found_enemy_index] ) {
        if ( curr_target ) {
            curr_target->EndTarget();
        }

        curr_target = Cast< AEnemy >( found_enemies[found_enemy_index] );
        curr_target->StartTarget();
    }
}
