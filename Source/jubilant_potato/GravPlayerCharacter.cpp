
#include "GravPlayerCharacter.h"
#include "GravMovementComponent.h"
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
}

void AGravPlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    const float result = FVector::DotProduct( GetActorUpVector(), gimbal->GetUpVector() );
    if ( result < 0.999f ) {
        camera_root->SetWorldRotation(
            FMath::RInterpTo( camera_root->GetComponentRotation(), GetActorRotation(),
                              DeltaTime, 1.0f ) );
    }
}

void AGravPlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) {
    Super::SetupPlayerInputComponent( PlayerInputComponent );
}
