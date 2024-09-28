
#include "GravPlayerCharacter.h"
#include "GravMovementComponent.h"  // UGravMovementComponent
#include "SmartSpringArm.h"         // USmartSpringArm class
#include "Camera/CameraComponent.h" // UCameraComponent class

AGravPlayerCharacter::AGravPlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : AMotionMatchingCharacter(
          ObjectInitializer.SetDefaultSubobjectClass< UGravMovementComponent >(
              ACharacter::CharacterMovementComponentName ) ) {
    PrimaryActorTick.bCanEverTick = true;

    camera_root = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "CameraRoot" ) ) );
    camera_root->SetupAttachment( GetRootComponent() );
    gimbal = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "Gimbal" ) ) );
    gimbal->SetupAttachment( camera_root );
    spring_arm = Cast< USmartSpringArm >(
        CreateDefaultSubobject< USmartSpringArm >( FName( "SpringArm" ) ) );
    spring_arm->SetupAttachment( gimbal );
    camera = CreateDefaultSubobject< UCameraComponent >( FName( "Camera" ) );
    camera->SetupAttachment( spring_arm );

    MovementModeChangedDelegate.AddUniqueDynamic(
        this,
        &AGravPlayerCharacter::MovementModeChanged );
}

void AGravPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    movement = Cast< UGravMovementComponent >( GetCharacterMovement() );
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    movement->UpdateRotation( DeltaTime );

    if ( !can_update_camera ) return;

    const FVector gravity = movement->GetGravityDirection() * -1.f;

    const float result = FVector::DotProduct( gravity, gimbal->GetUpVector() );

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                      gravity.ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red,
                                      GetActorUpVector().ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Yellow,
                                      FString::SanitizeFloat( result ) );

    if ( result < 0.999f ) {
        if ( !updating_camera ) {
            updating_camera = true;
            target_up = gravity;
            target_rot = GetActorRotation();
            // target_rot = ( movement->GetGravityToWorldTransform() *
            //                FRotator( 0.f ).Quaternion() )
            //                  .Rotator();
        } else {
            const float update_result = FVector::DotProduct( gravity,
                                                             target_up );
            if ( result != 1.f ) {
                target_up = gravity;
                target_rot = GetActorRotation();
                // target_rot = ( movement->GetGravityToWorldTransform() *
                //                FRotator( 0.f ).Quaternion() )
                //                  .Rotator();
            }
        }

        camera_root->SetWorldRotation(
            FMath::RInterpTo( camera_root->GetComponentRotation(), target_rot,
                              DeltaTime, 2.f ) );

        if ( last_gimbal_rot == gimbal->GetRelativeRotation() ) {
            last_gimbal_rot = FMath::RInterpTo( gimbal->GetRelativeRotation(),
                                                FRotator( 0.f ),
                                                DeltaTime, 2.f );
            if ( !last_gimbal_rot.IsNearlyZero() ) {
                gimbal->SetRelativeRotation( last_gimbal_rot );
            }
        }

        if ( last_sa_rot == spring_arm->GetRelativeRotation() ) {
            last_sa_rot = FMath::RInterpTo( spring_arm->GetRelativeRotation(),
                                            FRotator( 0.f ), DeltaTime, 2.f );
            if ( !last_sa_rot.IsNearlyZero() ) {
                spring_arm->SetRelativeRotation( last_sa_rot );
            }
        }

    } else {
        updating_camera = false;
        can_update_camera = false;
    }
}

void AGravPlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void AGravPlayerCharacter::MovementModeChanged( ACharacter* Character,
                                                EMovementMode PrevMovementMode,
                                                uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling &&
         movement->MovementMode == MOVE_Walking ) {
        can_update_camera = true;
    }
}
