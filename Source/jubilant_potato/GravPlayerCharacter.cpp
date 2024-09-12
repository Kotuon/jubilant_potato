
#include "GravPlayerCharacter.h"
#include "GravMovementComponent.h"  // UGravMovementComponent
#include "SmartSpringArm.h"         // USmartSpringArm class
#include "Camera/CameraComponent.h" // UCameraComponent class

AGravPlayerCharacter::AGravPlayerCharacter( const FObjectInitializer& ObjectInitializer )
    : ACharacter( ObjectInitializer.SetDefaultSubobjectClass< UGravMovementComponent >(
          ACharacter::CharacterMovementComponentName ) ) {
    PrimaryActorTick.bCanEverTick = true;

    camera_root = Cast< USceneComponent >( CreateDefaultSubobject< USceneComponent >( FName( "CameraRoot" ) ) );
    camera_root->SetupAttachment( GetRootComponent() );
    gimbal = Cast< USceneComponent >( CreateDefaultSubobject< USceneComponent >( FName( "Gimbal" ) ) );
    gimbal->SetupAttachment( camera_root );
    spring_arm = Cast< USmartSpringArm >( CreateDefaultSubobject< USmartSpringArm >( FName( "SpringArm" ) ) );
    spring_arm->SetupAttachment( gimbal );
    camera = CreateDefaultSubobject< UCameraComponent >( FName( "Camera" ) );
    camera->SetupAttachment( spring_arm );
}

void AGravPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    movement = Cast< UGravMovementComponent >( GetCharacterMovement() );
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    const FVector gravity = movement->GetGravityDirection() * -1.f;

    const float result = FVector::DotProduct( gravity, gimbal->GetUpVector() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, gravity.ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, GetActorUpVector().ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Yellow, FString::SanitizeFloat( result ) );
    if ( result < 0.999f ) {
        if ( !has_started ) {
            has_started = true;
            target_up = gravity;
            target_rot = GetActorRotation();
        } else {
            const float update_result = FVector::DotProduct( gravity, target_up );
            if ( result != 1.f ) {
                target_up = gravity;
                target_rot = GetActorRotation();
            }
        }

        camera_root->SetWorldRotation(
            FMath::RInterpTo( camera_root->GetComponentRotation(), target_rot,
                              DeltaTime, 2.f ) );

        const FRotator new_gimbal_rot = FMath::RInterpTo( gimbal->GetRelativeRotation(), FRotator( 0.f ),
                                                          DeltaTime, 2.f );
        if ( !new_gimbal_rot.IsNearlyZero() ) {
            gimbal->SetRelativeRotation( new_gimbal_rot );
        }
        const FRotator new_sa_rot = FMath::RInterpTo( spring_arm->GetRelativeRotation(), FRotator( 0.f ),
                                                      DeltaTime, 2.f );
        if ( !new_sa_rot.IsNearlyZero() ) {
            spring_arm->SetRelativeRotation( new_sa_rot );
        }

    } else {
        has_started = false;
    }
}

void AGravPlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}
