// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionMatchingCharacter.generated.h"

USTRUCT( BlueprintType )
struct FSimpleCameraParams {
    GENERATED_BODY()

    float SpringArmLength = 300.f;
    FVector SocketOffset = FVector( 0.f );
    float TranslationLagSpeed = 10.f;
    float FieldOfView = 90.f;
    float TransitionSpeed = 5.f;
};

UENUM( BlueprintType )
enum class EGait : uint8 {
    ERun,
    EWalk,
    ESprint
};

class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class JUBILANT_POTATO_API AMotionMatchingCharacter : public ACharacter {
    GENERATED_BODY()

protected: // Functions
    virtual void BeginPlay() override;

public: // Functions
    AMotionMatchingCharacter( const FObjectInitializer& ObjectInitializer );

    virtual void Tick( float DeltaTime ) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent ) override;

private: // Functions
    void UpdateMovement();
    void UpdateRotation();
    void UpdateCamera( float DeltaTime, bool Interpolate );

    EGait GetDesiredGait() const;
    float CalculateMaxSpeed() const;

    void OnLanded( const FHitResult& Hit );

public: // Variables
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
        Category = "MotionMatching" )
    UCurveFloat* strafeSpeedMapCurve;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
        Category = "MotionMatching" )
    FVector walkSpeeds = {200.f, 175.f, 150.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
        Category = "MotionMatching" )
    FVector runSpeeds = {500.f, 350.f, 300.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
        Category = "MotionMatching" )
    FVector sprintSpeeds = {700.f, 700.f, 700.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly,
        Category = "MotionMatching" )
    FVector crouchSpeeds = {200.f, 150.f, 150.f};

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    FVector landVelocity;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    EGait gait;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    bool wantsToStrafe = true;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    bool wantsToSprint = false;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    bool wantsToWalk = false;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    bool wantsToAim = false;

    UPROPERTY( BlueprintReadOnly, Category = "MotionMatching" )
    bool justLanded = false;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    FSimpleCameraParams CamStyle_Far{
        300.f, FVector( 0.f, 0.f, 20.f ),
        6.f, 90.f, 2.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    FSimpleCameraParams CamStyle_Close{
        225.f, FVector( 0.f, 45.f, 20.f ),
        10.f, 90.f, 5.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    FSimpleCameraParams CamStyle_Aim{
        200.f, FVector( 0.f, 50.f, 30.f ),
        15.f, 80.f, 5.f};

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    float CameraDistanceMultiplier = 1.f;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    USpringArmComponent* spring_arm;
    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Camera" )
    UCameraComponent* camera;

private: // Variables
    UCharacterMovementComponent* movement;
    FTimerManager* timerManager;

    FTimerHandle landedHandle;
};