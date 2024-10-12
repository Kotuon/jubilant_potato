
#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"  // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"   // UEnhancedInputComponent class
#include "Kismet/KismetMathLibrary.h" // GetForwardVector(), GetRightVector()

APlayerCharacter::APlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : AMotionMatchingCharacter( ObjectInitializer ) {
    PrimaryActorTick.bCanEverTick = true;

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
}

void APlayerCharacter::Move( const FInputActionValue& value ) {
    const FRotator rotation = GetControlRotation();

    AddMovementInput( UKismetMathLibrary::GetForwardVector( rotation ),
                      value.Get< FVector2D >().Y );

    AddMovementInput( UKismetMathLibrary::GetRightVector( rotation ),
                      value.Get< FVector2D >().X );
}

void APlayerCharacter::Look( const FInputActionValue& value ) {
    AddControllerPitchInput( value.Get< FVector2D >().Y * sensitivity );
    AddControllerYawInput( value.Get< FVector2D >().X * sensitivity );
}
