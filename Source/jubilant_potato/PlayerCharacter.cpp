#include "PlayerCharacter.h"
#include "DefaultASC.h"
#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class

#include "AbilitySystemComponent.h"

#include "JPAttributeSet.h"

#include "SmartSpringArm.h"
#include "Camera/CameraComponent.h" // UCameraComponent class

#include "EnhancedInputComponent.h"   // UEnhancedInputComponent class
#include "Kismet/KismetMathLibrary.h" // GetForwardVector(), GetRightVector()
#include "PlayerGameplayAbilitiesDataAsset.h"

#include "GameFramework/CharacterMovementComponent.h"

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

    AbilitySystemComponent = CreateDefaultSubobject< UDefaultASC >(
        TEXT( "AbilitySystemComponent" ) );
    AbilitySystemComponent->SetIsReplicated( true );

    AttributeSet =
        CreateDefaultSubobject< UJPAttributeSet >( TEXT( "AttributeSet" ) );

    Tags.Add( FName( "Player" ) );
}

void APlayerCharacter::BeginPlay() {
    InitAbilitySystem();

    if ( const APlayerController* PlayerController =
             Cast< APlayerController >( Controller ) ) {
        if ( UEnhancedInputLocalPlayerSubsystem* Subsystem =
                 ULocalPlayer::GetSubsystem<
                     UEnhancedInputLocalPlayerSubsystem >(
                     PlayerController->GetLocalPlayer() ) ) {
            constexpr int32 Priority = 0;
            Subsystem->AddMappingContext( inputMapping, Priority );
        }
    }

    Super::BeginPlay();

    UWorld* world = GetWorld();

    const UPrimitiveComponent* MovementBase = GetMovementBase();
    if ( !MovementBaseUtility::UseRelativeLocation( MovementBase ) ) {
        return;
    }
}

void APlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    UpdateRotation( DeltaTime );

    GEngine->AddOnScreenDebugMessage(
        -1, 0.f, FColor::Green,
        FString::SanitizeFloat( GetCharacterMovement()->MaxWalkSpeed ) );
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent ) {

    // Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI =
        Cast< UEnhancedInputComponent >( PlayerInputComponent );

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

void APlayerCharacter::SetStrafe( bool NewStrafe ) {
    ShouldStrafe = NewStrafe;
    GetCharacterMovement()->bOrientRotationToMovement = !NewStrafe;
}

void APlayerCharacter::UpdateRotation( float DeltaTime ) {
    if ( !ShouldStrafe ) return;

    UCharacterMovementComponent* Movement = GetCharacterMovement();

    FQuat DeltaQuat = FQuat::Identity;
    FVector DeltaPosition = FVector::ZeroVector;

    FRotator CurrentRotation =
        Movement->UpdatedComponent->GetComponentRotation();
    CurrentRotation.DiagnosticCheckNaN( TEXT(
        "CharacterMovementComponent::PhysicsRotation(): CurrentRotation" ) );

    FRotator DesiredRotation = gimbal->GetComponentRotation();
    DesiredRotation.Pitch = 0.f;
    DesiredRotation.Roll = 0.f;

    Movement->MoveUpdatedComponent( FVector::ZeroVector, DesiredRotation,
                                    false );
}
