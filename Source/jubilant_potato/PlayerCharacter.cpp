
#include "PlayerCharacter.h"
#include "SmartSpringArm.h"                  // USmartSpringArm class
#include "EnhancedInputSubsystems.h"         // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"          // UEnhancedInputComponent class
#include "Kismet/KismetMathLibrary.h"        // GetForwardVector(), GetRightVector()
#include "GravMovementComponent.h"           // UGravMovementComponent
#include "Action.h"                          // UAction class
#include "ActionManager.h"                   // UActionManager class
#include "GameFramework/MovementComponent.h" //
#include "Math/UnrealMathUtility.h"          // Lerp

APlayerCharacter::APlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : AGravPlayerCharacter( ObjectInitializer ) {
    PrimaryActorTick.bCanEverTick = true;

    action_manager = Cast< UActionManager >(
        CreateDefaultSubobject< UActionManager >( FName( "ActionManager" ) ) );

    Tags.Add( FName( "Player" ) );
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    UWorld* world = GetWorld();

    APlayerController* pc = world->GetFirstPlayerController();
    if ( !pc )
        return;

    pc->bShowMouseCursor = false;
}

void APlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    if ( !movement->bOrientRotationToMovement ) {
        SetActorRotation( camera_root->GetRelativeRotation() );
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    // Get the player controller
    const APlayerController* pc = Cast< APlayerController >( GetController() );

    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >(
            pc->GetLocalPlayer() );
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext( input_mapping, 0 );

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI = Cast< UEnhancedInputComponent >(
        PlayerInputComponent );

    // Bind the actions
    PEI->BindAction( input_move, ETriggerEvent::Triggered, this,
                     &APlayerCharacter::Move );
    PEI->BindAction( input_look, ETriggerEvent::Triggered, this,
                     &APlayerCharacter::Look );

    TArray< UAction* > action_components;
    GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        action->RegisterComponent();
        action->BindAction( PEI );
    }
}

void APlayerCharacter::Move( const FInputActionValue& value ) {
    const FVector2D inputValue = value.Get< FVector2D >();
    lastMovementInput = FVector( inputValue.X, inputValue.Y, 0.f );

    if ( !can_walk ) return;

    AddMovementInput( gimbal->GetForwardVector(), inputValue.Y, false );
    AddMovementInput( gimbal->GetRightVector(), inputValue.X, false );
}

void APlayerCharacter::Look( const FInputActionValue& value ) {
    const FVector2D inputValue = value.Get< FVector2D >();
    lastCameraInput = inputValue;

    const float inputYaw = gimbal->GetRelativeRotation().Yaw +
                           ( inputValue.X * sensitivity );
    const FRotator newYaw{ 0.0, inputYaw, 0.0 };
    gimbal->SetRelativeRotation( newYaw );

    const float inputPitch = spring_arm->GetRelativeRotation().Pitch +
                             ( inputValue.Y * sensitivity );
    const float clampedPitch = FMath::Clamp( inputPitch, -80, 80 );

    const FRotator newPitch{ clampedPitch, 0.0, 0.0 };
    spring_arm->SetRelativeRotation( newPitch );
}
