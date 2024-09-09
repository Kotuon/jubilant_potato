
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

    // const float result = FVector::DotProduct( gravity, gimbal->GetUpVector() );
    // GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, gravity.To_String() );
    // GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, GetActorUpVector() );
    // // GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, FString::SanitizeFloat( result ) );
    // if ( result < 0.999f ) {
    //     const FRotator gravity_rot = gravity.Rotation();

    //     camera_root->SetWorldRotation(
    //         FMath::RInterpTo( camera_root->GetComponentRotation(), gravity_rot,
    //                           DeltaTime, 1.0f ) );
    // }

    const float result = FVector::DotProduct( GetActorUpVector(), gimbal->GetUpVector() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, gravity.ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, GetActorUpVector().ToString() );
    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Yellow, FString::SanitizeFloat( result ) );
    if ( result < 0.999f ) {
        if ( !has_started ) {
            has_started = true;
            target_rot = GetActorRotation();
            target_up = GetActorUpVector();
        } else {
            const float update_result = FVector::DotProduct( GetActorUpVector(), target_up );
            if ( result != 1.f ) {
                target_rot = GetActorRotation();
                target_up = GetActorUpVector();
            }
        }
        camera_root->SetWorldRotation(
            FMath::RInterpTo( camera_root->GetComponentRotation(), target_rot,
                              DeltaTime, 1.0f ) );
    } else {
        has_started = false;
    }
}

void AGravPlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}
