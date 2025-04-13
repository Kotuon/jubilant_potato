#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h" // UCameraComponent class
#include "SmartSpringArm.h"

#include "EnhancedInputSubsystems.h" // UEnhancedInputLocalPlayerSubsystem class
#include "EnhancedInputComponent.h"  // UEnhancedInputComponent class

#include "ActionManager.h"
#include "Action.h"

#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

APlayerCharacter::APlayerCharacter(
    const FObjectInitializer& ObjectInitializer )
    : AGravPlayerCharacter( ObjectInitializer ) {
    PrimaryActorTick.bCanEverTick = true;

    actionManager = Cast< UActionManager >(
        CreateDefaultSubobject< UActionManager >( FName( "ActionManager" ) ) );

    Tags.Add( FName( "Player" ) );
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
    //...

    UWorld* world = GetWorld();

    resourceCurrAmount = resourceTotalAmount;

    TArray< UWidgetComponent* > widgets;
    GetComponents< UWidgetComponent >( widgets );
    UWidgetComponent* hud = nullptr;

    for ( UWidgetComponent* widget : widgets ) {
        if ( widget->GetName() == "Hud" ) {
            hud = widget;
            break;
        }
    }

    TArray< UWidget* > child_widgets;
    hud->GetWidget()->WidgetTree->GetAllWidgets( child_widgets );
    for ( UWidget* child : child_widgets ) {
        if ( child->GetName() == "ProgressBar" ) {
            resourceBar = Cast< UProgressBar >( child );
            resourceBar->SetPercent( resourceCurrAmount / resourceTotalAmount );
        }
    }
}

#include "GravMovementComponent.h"

void APlayerCharacter::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    //...

    float angle = FMath::DegreesToRadians( gimbal->GetRelativeRotation().Yaw );
    if ( angle < 0.f ) {
        angle = 2 * PI + angle;
    }

    const FQuat& targetQuat = GetTargetCameraOrientation();

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green,
                                      "UpVector: " +
                                          cameraRoot->GetUpVector().ToString() );

    GEngine->AddOnScreenDebugMessage(
        -1, 0.f, FColor::Green,
        "GravUp  : " + ( GetCharacterMovement()->GetGravityDirection() * -1.f )
                           .ToString() );

    GEngine->AddOnScreenDebugMessage(
        -1, 0.f, FColor::Green, "Yaw: " + FString::SanitizeFloat( angle ) );

    GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Cyan,
                                      "TargetQuat: " + targetQuat.ToString() );

    DrawDebugDirectionalArrow(
        GetWorld(), GetActorLocation(),
        GetActorLocation() + ( targetQuat.GetUpVector() * 500.f ), 10.f,
        FColor::Red, false, 0.f, ( uint8 )0U, 2.f );

    DrawDebugDirectionalArrow(
        GetWorld(), GetActorLocation(),
        GetActorLocation() + ( targetQuat.GetForwardVector() * 500.f ), 10.f,
        FColor::Green, false, 0.f, ( uint8 )0U, 2.f );

    const FQuat rotatedTarget =
        FQuat( targetQuat.GetUpVector(), angle ).GetNormalized();

    const FQuat outputQuat = rotatedTarget * targetQuat;

    DrawDebugDirectionalArrow(
        GetWorld(), GetActorLocation(),
        GetActorLocation() + ( outputQuat.GetForwardVector() * 500.f ), 10.f,
        FColor::Cyan, false, 0.f, ( uint8 )0U, 2.f );
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

void APlayerCharacter::SetCanMove( bool CanMove_ ) { canMove = CanMove_; }

bool APlayerCharacter::GetCanMove() const { return canMove; }

void APlayerCharacter::SetLastMovementInput( const FVector newInput ) {
    lastMovementInput = newInput;
}

const FVector APlayerCharacter::GetLastMovementInput() const {
    return lastMovementInput;
}

void APlayerCharacter::SetLastCameraInput( const FVector2D newInput ) {
    lastCameraInput = newInput;
}

const FVector2D APlayerCharacter::GetLastCameraInput() const {
    return lastCameraInput;
}

bool APlayerCharacter::UseResource( const float Amount ) {
    if ( resourceCurrAmount - Amount <= 0.f ) return false;

    resourceCurrAmount =
        FMath::Clamp( resourceCurrAmount - Amount, 0.f, resourceTotalAmount );
    resourceBar->SetPercent( resourceCurrAmount / resourceTotalAmount );

    return true;
}

void APlayerCharacter::UseResourceOnTimer( const float Amount ) {
    FTimerDelegate resourceTickDelegate;

    resourceTickDelegate.BindUFunction( this, FName( "TickResource" ), Amount );

    GetWorldTimerManager().SetTimer( resourceTimer, resourceTickDelegate, 0.01f,
                                     true );
}

void APlayerCharacter::ClearResourceTimer() {
    GetWorldTimerManager().ClearTimer( resourceTimer );
}

void APlayerCharacter::TickResource( const float Amount ) {

    if ( !UseResource( Amount ) ) {
        ClearResourceTimer();
        ResourceEmptyDelegate.Broadcast();
    }
}

bool APlayerCharacter::AddResource( const float Amount ) {
    if ( resourceCurrAmount >= resourceTotalAmount ) {
        return false;
    }

    resourceCurrAmount =
        FMath::Clamp( resourceCurrAmount + Amount, 0.f, resourceTotalAmount );
    resourceBar->SetPercent( resourceCurrAmount / resourceTotalAmount );

    return true;
}
