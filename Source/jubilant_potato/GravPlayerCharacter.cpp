
#include "GravPlayerCharacter.h"
#include "GravMovementComponent.h"  // UGravMovementComponent
#include "SmartSpringArm.h"         // USmartSpringArm class
#include "Camera/CameraComponent.h" // UCameraComponent class

AGravPlayerCharacter::AGravPlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : ACharacter(
          ObjectInitializer.SetDefaultSubobjectClass< UGravMovementComponent >(
              ACharacter::CharacterMovementComponentName ) ) {
    PrimaryActorTick.bCanEverTick = true;

    cameraRoot = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "CameraRoot" ) ) );
    cameraRoot->SetupAttachment( GetRootComponent() );
    gimbal = Cast< USceneComponent >(
        CreateDefaultSubobject< USceneComponent >( FName( "Gimbal" ) ) );
    gimbal->SetupAttachment( cameraRoot );
    springArm = Cast< USmartSpringArm >(
        CreateDefaultSubobject< USmartSpringArm >( FName( "SpringArm" ) ) );
    springArm->SetupAttachment( gimbal );
    camera = CreateDefaultSubobject< UCameraComponent >( FName( "Camera" ) );
    camera->SetupAttachment( springArm );

    MovementModeChangedDelegate.AddUniqueDynamic(
        this, &AGravPlayerCharacter::MovementModeChanged );
}

void AGravPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    movement = Cast< UGravMovementComponent >( GetCharacterMovement() );
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    movement->UpdateRotation( DeltaTime );

    if ( !canUpdateCamera ) return;

    const FVector gravity = movement->GetGravityDirection() * -1.f;

    const float result = FVector::DotProduct( gravity, gimbal->GetUpVector() );

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                      gravity.ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red,
                                      GetActorUpVector().ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Yellow,
                                      FString::SanitizeFloat( result ) );

    if ( result < 0.999f ) {
        if ( !updatingCamera ) {
            updatingCamera = true;
            targetUp = gravity;
            targetRot = GetActorRotation();
            // targetRot = ( movement->GetGravityToWorldTransform() *
            //                FRotator( 0.f ).Quaternion() )
            //                  .Rotator();
        } else {
            const float update_result =
                FVector::DotProduct( gravity, targetUp );
            if ( result != 1.f ) {
                targetUp = gravity;
                targetRot = GetActorRotation();
                // targetRot = ( movement->GetGravityToWorldTransform() *
                //                FRotator( 0.f ).Quaternion() )
                //                  .Rotator();
            }
        }

        cameraRoot->SetWorldRotation( FMath::RInterpTo(
            cameraRoot->GetComponentRotation(), targetRot, DeltaTime, 2.f ) );

        if ( lastGimbalRot == gimbal->GetRelativeRotation() ) {
            lastGimbalRot = FMath::RInterpTo( gimbal->GetRelativeRotation(),
                                              FRotator( 0.f ), DeltaTime, 2.f );
            if ( !lastGimbalRot.IsNearlyZero() ) {
                gimbal->SetRelativeRotation( lastGimbalRot );
            }
        }

        if ( lastSaRot == springArm->GetRelativeRotation() ) {
            lastSaRot = FMath::RInterpTo( springArm->GetRelativeRotation(),
                                          FRotator( 0.f ), DeltaTime, 2.f );
            if ( !lastSaRot.IsNearlyZero() ) {
                springArm->SetRelativeRotation( lastSaRot );
            }
        }

    } else {
        updatingCamera = false;
        canUpdateCamera = false;
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
        canUpdateCamera = true;
    }
}
