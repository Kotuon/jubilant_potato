// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/WidgetComponent.h" // UUserWidget class and UWidgetComponent class
#include "AIController.h"               // AAIController class
#include "BrainComponent.h"             // UBrainComponent class
#include "Components/CapsuleComponent.h" // UCapsuleComponent class
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

AEnemy::AEnemy() {
    PrimaryActorTick.bCanEverTick = true;
    //...

    Tags.Add( "Enemy" );
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    //...

    maxHealth = health;
    currHealth = maxHealth;

    TArray< UWidgetComponent* > widgets;
    GetComponents< UWidgetComponent >( widgets );

    for ( UWidgetComponent* widget : widgets ) {
        if ( widget->GetName() == "TargetMarker" ) {
            target_marker = Cast< UUserWidget >( widget->GetWidget() );
            target_marker->SetRenderOpacity( 0.f );
            continue;
        } else if ( widget->GetName() == "HealthBar" ) {
            healthBar = Cast< UProgressBar >(
                widget->GetWidget()->GetWidgetFromName( "HealthBar" ) );
            healthBar->SetPercent( static_cast< float >( currHealth ) /
                                   static_cast< float >( maxHealth ) );
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

bool AEnemy::GetIsTargeted() const { return is_targeted; }

void AEnemy::ApplyDamage( int DamageAmount ) {
    currHealth -= DamageAmount;

    healthBar->SetPercent( static_cast< float >( currHealth ) /
                           static_cast< float >( maxHealth ) );

    if ( !is_dead && currHealth <= 0 ) {
        Kill();
    }
}

void AEnemy::Kill() {
    is_dead = true;

    EndTarget();

    AAIController* controller = Cast< AAIController >( GetController() );
    if ( IsValid( controller ) ) {
        controller->GetBrainComponent()->StopLogic( "Dead" );
    }

    UCapsuleComponent* bodyHitbox = FindComponentByClass< UCapsuleComponent >();
    if ( IsValid( bodyHitbox ) ) {
        bodyHitbox->SetCollisionEnabled( ECollisionEnabled::NoCollision );
    }

    // TODO: Stop any animation montages playing when dying (once/if there are
    // any added)

    controller->UnPossess();
}

void AEnemy::StartRagdoll() {
    USkeletalMeshComponent* mesh = GetMesh();

    mesh->SetCollisionObjectType( ECollisionChannel::ECC_PhysicsBody );
    mesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
    mesh->SetSimulatePhysics( true );
    mesh->SetAllBodiesSimulatePhysics( true );
}

bool AEnemy::IsDead() const { return is_dead; }
