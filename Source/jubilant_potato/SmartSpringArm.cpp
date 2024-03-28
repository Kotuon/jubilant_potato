// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartSpringArm.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"

void USmartSpringArm::BeginPlay() {
    Super::BeginPlay();

    parent = Cast< ACharacter >( GetOwner() );

    TArray< USceneComponent * > parents;

    GetParentComponents( parents );

    gimbal = parents[0];

    gimbal->SetWorldRotation( parent->GetActorRotation() );

    TargetArmLength = lerpPoints[0];
}

void USmartSpringArm::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if ( !IsValid( parent ) )
        return;

    if ( parent->GetVelocity().Length() > startVelocity ) {
        currTime = FMath::Clamp( currTime + DeltaTime, 0, lerpTime );
    } else {
        currTime = FMath::Clamp( currTime - DeltaTime, 0, lerpTime );
    }

    float t = FMath::Clamp( currTime / lerpTime, 0.f, 1.f );

    // Cubic bezier curve
    float new_length = FMath::Pow( 1 - t, 3 ) * lerpPoints[0] +
                       3 * FMath::Pow( 1 - t, 2 ) * t * lerpPoints[1] +
                       3 * ( 1 - t ) * FMath::Pow( t, 2 ) * lerpPoints[2] +
                       FMath::Pow( t, 3 ) * lerpPoints[3];

    TargetArmLength = new_length;
}