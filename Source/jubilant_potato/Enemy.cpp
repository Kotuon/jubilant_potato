// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/WidgetComponent.h" // UUserWidget class and UWidgetComponent class

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
    target_marker->SetRenderOpacity( 1.f );
}

void AEnemy::EndTarget() {
    target_marker->SetRenderOpacity( 0.f );
}
