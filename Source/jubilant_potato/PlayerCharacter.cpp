
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

APlayerCharacter::APlayerCharacter( const FObjectInitializer& ObjectInitializer )
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

    // GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, GetVelocity().ToString() );
    if ( !movement->bOrientRotationToMovement ) {
        SetActorRotation( camera_root->GetRelativeRotation() );
    }

    // if ( is_running ) {
    //     time_running = FMath::Clamp( time_running + DeltaTime, 0.f, time_to_reach_max_run );

    //     movement->MaxWalkSpeed = FMath::Lerp( norm_run_speed, fast_run_speed,
    //                                           time_running / time_to_reach_max_run );
    //     GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
    //                                       FString::SanitizeFloat( movement->MaxWalkSpeed ) );
    //     GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red,
    //                                       FString::SanitizeFloat( time_running ) );
    // }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) {
    // Get the player controller
    const APlayerController* pc = Cast< APlayerController >( GetController() );

    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( pc->GetLocalPlayer() );
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext( input_mapping, 0 );

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI = Cast< UEnhancedInputComponent >( PlayerInputComponent );

    // Bind the actions
    PEI->BindAction( input_move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move );
    PEI->BindAction( input_look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look );

    TArray< UAction* > action_components;
    GetComponents< UAction >( action_components );
    for ( UAction* action : action_components ) {
        action->RegisterComponent();
        action->BindAction( PEI );
    }
}

void APlayerCharacter::Move( const FInputActionValue& value ) {
    const FVector2D input_value = value.Get< FVector2D >();
    last_movement_input = FVector( input_value.X, input_value.Y, 0.f );

    if ( !can_walk ) {
        is_running = false;
        movement->MaxWalkSpeed = norm_run_speed;
        time_running = 0.f;
        return;
    }

    if ( input_value.SquaredLength() <= 0.f ) {
        is_running = false;
        movement->MaxWalkSpeed = norm_run_speed;
        time_running = 0.f;
    } else {
        is_running = true;
    }

    AddMovementInput( gimbal->GetForwardVector(), input_value.Y, false );
    AddMovementInput( gimbal->GetRightVector(), input_value.X, false );
}

void APlayerCharacter::Look( const FInputActionValue& value ) {
    const FVector2D input_value = value.Get< FVector2D >();
    last_camera_input = input_value;

    const float input_yaw = gimbal->GetRelativeRotation().Yaw + ( input_value.X * sensitivity );
    const FRotator new_yaw{ 0.0, input_yaw, 0.0 };
    gimbal->SetRelativeRotation( new_yaw );

    const float input_pitch = spring_arm->GetRelativeRotation().Pitch +
                              ( input_value.Y * sensitivity );
    const float clamped_pitch = FMath::Clamp( input_pitch, -80, 80 );

    const FRotator new_pitch{ clamped_pitch, 0.0, 0.0 };
    spring_arm->SetRelativeRotation( new_pitch );
}
