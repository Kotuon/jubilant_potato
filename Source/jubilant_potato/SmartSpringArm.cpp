// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartSpringArm.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"

void USmartSpringArm::BeginPlay() {
    Super::BeginPlay();

    parent = Cast< ACharacter >( GetOwner() );

    TArray< USceneComponent* > parents;

    GetParentComponents( parents );

    gimbal = parents[0];

    gimbal->SetWorldRotation( parent->GetActorRotation() );

    TargetArmLength = default_length;
}

void USmartSpringArm::UpdateFromVelocity( float DeltaTime ) {
    // Updating length from velocity
    if ( parent->GetVelocity().Length() > start_velocity ) {
        curr_time_velocity = FMath::Clamp( curr_time_velocity + DeltaTime, 0, lerp_time );
    } else {
        curr_time_velocity = FMath::Clamp( curr_time_velocity - DeltaTime, 0, lerp_time );
    }

    float t = FMath::Clamp( curr_time_velocity / lerp_time, 0.f, 1.f );

    float new_length = FMath::Lerp( TargetArmLength,
                                    default_length + default_length * ( length_multiplier_velocity * t ),
                                    0.25f );

    TargetArmLength = new_length;
}

bool USmartSpringArm::UpdateFromAiming( float DeltaTime ) {
    if ( !is_aiming && curr_time_aiming <= 0.f ) {
        return false;
    }

    if ( !is_aiming ) {
        curr_time_aiming = FMath::Clamp( curr_time_aiming - DeltaTime, 0, lerp_time );
    } else {
        curr_time_aiming = FMath::Clamp( curr_time_aiming + DeltaTime, 0, lerp_time );
    }

    float t = FMath::Clamp( curr_time_aiming / lerp_time, 0.f, 1.f );

    float new_length = FMath::Lerp( TargetArmLength,
                                    default_length - default_length * ( length_multiplier_aiming * t ),
                                    0.25f );

    TargetArmLength = new_length;

    return true;
}

void USmartSpringArm::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if ( !IsValid( parent ) )
        return;

    if ( !UpdateFromAiming( DeltaTime ) ) {
        // UpdateFromVelocity( DeltaTime );
    }
}

void USmartSpringArm::SetIsAiming( bool IsAiming ) {
    is_aiming = IsAiming;
}
