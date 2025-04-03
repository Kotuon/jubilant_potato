// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GravPlayerCharacter.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class USmartSpringArm;
class UCameraComponent;
class UActionManager;
class UProgressBar;

DECLARE_MULTICAST_DELEGATE( FResourceEmptyDelegate );

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public AGravPlayerCharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    APlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

    void SetStrafe( bool NewStrafe );

    void SetCanMove( bool CanMove_ );
    bool GetCanMove() const;

    void SetLastMovementInput( const FVector newInput );
    const FVector GetLastMovementInput() const;

    void SetLastCameraInput( const FVector2D newInput );
    const FVector2D GetLastCameraInput() const;

    bool UseResource( const float Amount );

    void UseResourceOnTimer( const float Amount );
    void ClearResourceTimer();

    UFUNCTION( BlueprintCallable )
    bool AddResource( const float Amount );

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* inputMapping;

    UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Actions" )
    UActionManager* actionManager;

    FResourceEmptyDelegate ResourceEmptyDelegate;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Resource" )
    float resourceTotalAmount = 1000.f;

private: // Functions
    UFUNCTION()
    void TickResource( const float Amount );

private: // Variables
    FVector lastMovementInput;
    FVector2D lastCameraInput;

    UProgressBar* resourceBar;

    FTimerHandle resourceTimer;

    float resourceCurrAmount;

    bool shouldStrafe = false;
    bool canMove = true;
};