#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h" // UCameraComponent class
#include "SmartSpringArm.h"

#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"  // UEnhancedInputComponent class

#include "ActionManager.h"
#include "Action.h"

APlayerCharacter::APlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : ACharacter( ObjectInitializer ) {
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

    actionManager = Cast< UActionManager >(
        CreateDefaultSubobject< UActionManager >( FName( "ActionManager" ) ) );

    Tags.Add( FName( "Player" ) );
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
    //...

    UWorld* world = GetWorld();
}

void APlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    GEngine->AddOnScreenDebugMessage(
        -1, 0.f, FColor::Green,
        FString::SanitizeFloat( GetCharacterMovement()->MaxWalkSpeed ) );
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
    Subsystem->AddMappingContext( inputMapping, 0 );

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI =
        Cast< UEnhancedInputComponent >( PlayerInputComponent );

    // Bind the actions
    TArray< UAction* > action_components;
    GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        action->RegisterComponent();
        action->BindAction( PEI );
    }
}

void APlayerCharacter::SetStrafe( bool NewStrafe ) {
    shouldStrafe = NewStrafe;
    GetCharacterMovement()->bOrientRotationToMovement = !NewStrafe;
}

bool APlayerCharacter::GetCanMove() const {return canMove;}
