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

    void SetLastCameraInput(const FVector2D newInput);
    const FVector2D GetLastCameraInput() const;

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* inputMapping;

    UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Actions" )
    UActionManager* actionManager;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

private: // Functions
private: // Variables
    FVector lastMovementInput;
    FVector2D lastCameraInput;

    bool shouldStrafe = false;
    bool canMove = true;
};