// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "Enemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h" // UAIPerceptionComponent class
#include "Perception/AISense_Sight.h"         // UAISense_Sight class

AEnemyController::AEnemyController( const FObjectInitializer &ObjectInitializer )
    : Super( ObjectInitializer.SetDefaultSubobjectClass< UCrowdFollowingComponent >( TEXT( "PathFollowingComponent" ) ) ) {

    sight = Cast< UAIPerceptionComponent >( CreateDefaultSubobject< UAIPerceptionComponent >( FName( "Sight" ) ) );
    sight->SetDominantSense( UAISense_Sight::StaticClass() );

    sight->OnTargetPerceptionUpdated.AddUniqueDynamic( this, &AEnemyController::OnPerceptionTargetUpdate );
}

void AEnemyController::BeginPlay() {
    Super::BeginPlay();
    //...

    world = GetWorld();
    blackboard = UAIBlueprintHelperLibrary::GetBlackboard( controlledPawn );
}

void AEnemyController::OnPossess( APawn *InPawn ) {
    Super::OnPossess( InPawn );

    controlledPawn = InPawn;

    UBehaviorTree *behavior_tree_to_run = ( Cast< AEnemy >( InPawn ) )->enemy_behavior_tree;

    if ( !behavior_tree_to_run ) {
        GEngine->AddOnScreenDebugMessage( -1, 50.f, FColor::Green, "BAD" );
        return;
    }

    RunBehaviorTree( behavior_tree_to_run );

    if ( !IsValid( world ) ) {
        world = GetWorld();
    }
    world->GetTimerManager().SetTimer( find_target_time_handle, this, &AEnemyController::FindTarget, 0.1f, false );
}

void AEnemyController::FindTarget() {
    AActor *player_character = UGameplayStatics::GetPlayerCharacter( world, 0 );

    blackboard->SetValueAsObject( "Target", player_character );
    blackboard->SetValueAsObject( "SelfActor", controlledPawn );
}

void AEnemyController::OnPerceptionTargetUpdate( AActor *actor, FAIStimulus stimulus ) {

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, "Updating perception." );

    if ( actor->ActorHasTag( "Player" ) && stimulus.WasSuccessfullySensed() ) {
        world->GetTimerManager().ClearTimer( check_los_countdown );
        blackboard->SetValueAsBool( "HasLineOfSight", true );
    } else {
        world->GetTimerManager().SetTimer( check_los_countdown, this, &AEnemyController::StartEnemyTimer, line_of_sight_timer, false );
    }
}

void AEnemyController::StartEnemyTimer() {
    blackboard->SetValueAsBool( "HasLineOfSight", false );
}
