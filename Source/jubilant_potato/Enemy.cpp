// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/WidgetComponent.h"  // UUserWidget class and UWidgetComponent class
#include "AIController.h"                // AAIController class
#include "BrainComponent.h"              // UBrainComponent class
#include "Components/CapsuleComponent.h" // UCapsuleComponent class

AEnemy::AEnemy() {
    PrimaryActorTick.bCanEverTick = true;
    //...
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    //...

    TArray< UWidgetComponent * > widgets;
    GetComponents< UWidgetComponent >( widgets );

    for ( UWidgetComponent *widget : widgets ) {
        if ( widget->GetName() == "TargetMarker" ) {
            target_marker = Cast< UUserWidget >( widget->GetWidget() );
            target_marker->SetRenderOpacity( 0.f );
            continue;
        }
    }
}

void AEnemy::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...
}

void AEnemy::StartTarget() {
    is_targeted = true;
    target_marker->SetRenderOpacity( 1.f );
}

void AEnemy::EndTarget() {
    is_targeted = false;
    target_marker->SetRenderOpacity( 0.f );
}

bool AEnemy::GetIsTargeted() const {
    return is_targeted;
}

void AEnemy::ApplyDamage( int DamageAmount ) {
    health -= DamageAmount;
    if ( health <= 0 ) {
        Kill();
    }
}

void AEnemy::Kill() {
    is_dead = true;

    EndTarget();

    AAIController *controller = Cast< AAIController >( GetController() );
    if ( IsValid( controller ) ) {
        controller->GetBrainComponent()->StopLogic( "Dead" );
    }

    UCapsuleComponent *bodyHitbox = FindComponentByClass< UCapsuleComponent >();
    if ( IsValid( bodyHitbox ) ) {
        bodyHitbox->SetCollisionEnabled( ECollisionEnabled::NoCollision );
    }

    // TODO: Stop any animation montages playing when dying (once/if there are any added)

    controller->UnPossess();
}

void AEnemy::StartRagdoll() {
    USkeletalMeshComponent *mesh = GetMesh();

    mesh->SetCollisionObjectType( ECollisionChannel::ECC_PhysicsBody );
    mesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
    mesh->SetSimulatePhysics( true );
    mesh->SetAllBodiesSimulatePhysics( true );
}

bool AEnemy::IsDead() const {
    return is_dead;
}
