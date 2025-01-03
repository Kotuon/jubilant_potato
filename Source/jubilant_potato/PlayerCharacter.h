// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class USmartSpringArm;
class UCameraComponent;
class UActionManager;

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public ACharacter {
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

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* inputMapping;

    UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Actions" )
    UActionManager* actionManager;

    // Camera components
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent* cameraRoot;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USceneComponent* gimbal;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USmartSpringArm* springArm;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    UCameraComponent* camera;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float sensitivity = 1.f;

private: // Functions
private: // Variables
    FVector lastMovementInput;

    bool shouldStrafe = false;
    bool canMove = true;
};