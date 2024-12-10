// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UDefaultASC;
class UAbilitySystemComponent;
class UPlayerGameplayAbilitiesDataAsset;
class UAbilitySystemComponent;
class USmartSpringArm;
class UCameraComponent;

UCLASS()
class JUBILANT_POTATO_API APlayerCharacter : public ACharacter,
                                             public IAbilitySystemInterface {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    APlayerCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

    void PossessedBy( AController* NewController ) override;

    void AbilityInputPressed( int32 InputID );
    void AbilityInputReleased( int32 InputID );

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    FORCEINLINE UPlayerGameplayAbilitiesDataAsset*
    GetPlayerGameplayAbilitiesDataAsset() const {
        return PlayerGameplayAbilitiesDataAsset;
    }

    void SetStrafe( bool NewStrafe );

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Input" )
    class UInputMappingContext* inputMapping;

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

    UPROPERTY( EditDefaultsOnly, Category = "Gameplay Abilities" )
    UDefaultASC* AbilitySystemComponent;

    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem",
               meta = ( AllowPrivateAccess = "true" ) )
    TObjectPtr< UPlayerGameplayAbilitiesDataAsset >
        PlayerGameplayAbilitiesDataAsset;

    UPROPERTY()
    class UJPAttributeSet* AttributeSet;

private: // Functions
    void InitAbilitySystem();

    void UpdateRotation( float DeltaTime );

private: // Variables
    bool ShouldStrafe = false;
};