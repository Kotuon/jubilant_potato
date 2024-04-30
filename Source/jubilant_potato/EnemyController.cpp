// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "Enemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyController::AEnemyController( const FObjectInitializer &ObjectInitializer )
    : Super( ObjectInitializer.SetDefaultSubobjectClass< UCrowdFollowingComponent >( TEXT( "PathFollowingComponent" ) ) ) {}

void AEnemyController::OnPossess( APawn *InPawn ) {
    Super::OnPossess( InPawn );

    controlledPawn = InPawn;

    UBehaviorTree *behavior_tree_to_run = ( Cast< AEnemy >( InPawn ) )->enemy_behavior_tree;

    if ( !behavior_tree_to_run ) {
        GEngine->AddOnScreenDebugMessage( -1, 50.f, FColor::Green, "BAD" );
        return;
    }

    RunBehaviorTree( behavior_tree_to_run );

    GetWorld()->GetTimerManager().SetTimer( find_target_time_handle, this, &AEnemyController::FindTarget, 0.1f, false );
}

void AEnemyController::FindTarget() {
    AActor *player_character = UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 );

    UBlackboardComponent *blackboard = UAIBlueprintHelperLibrary::GetBlackboard( controlledPawn );
    blackboard->SetValueAsObject( "Target", player_character );
    blackboard->SetValueAsObject( "SelfActor", controlledPawn );
}
