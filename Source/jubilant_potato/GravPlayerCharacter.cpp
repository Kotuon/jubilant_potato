
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

    const FVector inverseGravity = movement->GetGravityDirection() * -1.f;

    const float result =
        FVector::DotProduct( inverseGravity, gimbal->GetUpVector() );

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                      inverseGravity.ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red,
                                      GetActorUpVector().ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Yellow,
                                      FString::SanitizeFloat( result ) );

    if ( result < 0.9999999f ) {
        if ( !updatingCamera ) {
            updatingCamera = true;
            targetUp = inverseGravity;
            targetRot = GetActorRotation();
        } else {
            const float update_result =
                FVector::DotProduct( inverseGravity, targetUp );
            if ( result != 1.f ) {
                targetUp = inverseGravity;
                targetRot = GetActorRotation();
            }
        }

        const FQuat startRot = cameraRoot->GetComponentRotation().Quaternion();
        const FQuat endRot = targetRot.Quaternion();

        // cameraRoot->SetWorldRotation(
        //     FQuat::Slerp( startRot, endRot, 3.0 * DeltaTime ) );

        cameraRoot->SetWorldRotation(
            FQuat::FastLerp( startRot, endRot, 3.0 * DeltaTime ) );

        // cameraRoot->SetWorldRotation( FMath::RInterpTo(
        //     cameraRoot->GetComponentRotation(), targetRot, DeltaTime, 4.f )
        //     );

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

void AGravPlayerCharacter::SetCanUpdateCamera( bool Value ) {
    canUpdateCamera = Value;
}

const FRotator AGravPlayerCharacter::GetTargetRotation() const {
    return targetRot;
}
