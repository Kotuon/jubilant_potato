#include "PlayerCharacter.h"
#include "DefaultASC.h"
#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class

#include "AbilitySystemComponent.h"

#include "JPAttributeSet.h"

#include "EnhancedInputComponent.h"   // UEnhancedInputComponent class
#include "Kismet/KismetMathLibrary.h" // GetForwardVector(), GetRightVector()
#include "PlayerGameplayAbilitiesDataAsset.h"

APlayerCharacter::APlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : AMotionMatchingCharacter( ObjectInitializer ) {
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject< UDefaultASC >(
        TEXT( "AbilitySystemComponent" ) );
    AbilitySystemComponent->SetIsReplicated( true );

    AttributeSet =
        CreateDefaultSubobject< UJPAttributeSet >( TEXT( "AttributeSet" ) );

    Tags.Add( FName( "Player" ) );
}

void APlayerCharacter::BeginPlay() {
    // AbilitySystemComponent->InitAbilityActorInfo( this, this );
    InitAbilitySystem();

    if ( const APlayerController* PlayerController =
             Cast< APlayerController >( Controller ) ) {
        if ( UEnhancedInputLocalPlayerSubsystem* Subsystem =
                 ULocalPlayer::GetSubsystem<
                     UEnhancedInputLocalPlayerSubsystem >(
                     PlayerController->GetLocalPlayer() ) ) {
            constexpr int32 Priority = 0;
            Subsystem->AddMappingContext( input_mapping, Priority );
        }
    }

    Super::BeginPlay();

    UWorld* world = GetWorld();
}

void APlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {
    // Get the player controller
    // const APlayerController* pc = Cast< APlayerController >( GetController()
    // );
    //
    // // Get the local player subsystem
    // UEnhancedInputLocalPlayerSubsystem* Subsystem =
    //     ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >(
    //         pc->GetLocalPlayer() );
    // // Clear out existing mapping, and add our mapping
    // Subsystem->ClearAllMappings();
    // Subsystem->AddMappingContext( input_mapping, 0 );

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI =
        Cast< UEnhancedInputComponent >( PlayerInputComponent );

    // Bind the actions
    // PEI->BindAction( input_move, ETriggerEvent::Triggered, this,
    //                  &APlayerCharacter::Move );
    // PEI->BindAction( input_look, ETriggerEvent::Triggered, this,
    //                  &APlayerCharacter::Look );

    if ( !PlayerGameplayAbilitiesDataAsset ) return;

    const TSet< FGameplayInputAbilityInfo >& InputAbilities =
        PlayerGameplayAbilitiesDataAsset->GetInputAbilities();

    for ( const auto& It : InputAbilities ) {
        if ( !It.IsValid() ) continue;

        const UInputAction* InputAction = It.InputAction;
        const int32 InputID = It.InputID;

        PEI->BindAction( InputAction, ETriggerEvent::Started, this,
                         &APlayerCharacter::AbilityInputPressed, InputID );
        PEI->BindAction( InputAction, ETriggerEvent::Completed, this,
                         &APlayerCharacter::AbilityInputReleased, InputID );
    }
}

void APlayerCharacter::PossessedBy( AController* NewController ) {
    Super::PossessedBy( NewController );

    if ( AbilitySystemComponent )
        AbilitySystemComponent->InitAbilityActorInfo( this, this );

    SetOwner( NewController );
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

void APlayerCharacter::AbilityInputPressed( int32 InputID ) {
    if ( AbilitySystemComponent ) {
        AbilitySystemComponent->AbilityLocalInputPressed( InputID );
    }
}

void APlayerCharacter::AbilityInputReleased( int32 InputID ) {
    if ( AbilitySystemComponent ) {
        AbilitySystemComponent->AbilityLocalInputReleased( InputID );
    }
}

void APlayerCharacter::InitAbilitySystem() {
    if ( !PlayerGameplayAbilitiesDataAsset ) return;

    const TSet< FGameplayInputAbilityInfo > InputAbilities =
        PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
    constexpr int32 AbilityLevel = 1;

    for ( const auto& It : InputAbilities ) {
        if ( !It.IsValid() ) continue;

        const FGameplayAbilitySpec AbilitySpec =
            FGameplayAbilitySpec( It.GameplayAbilityClass, 1, It.InputID );
        AbilitySystemComponent->GiveAbility( AbilitySpec );
    }
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}