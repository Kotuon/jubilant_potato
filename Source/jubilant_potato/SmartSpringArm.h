// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "SmartSpringArm.generated.h"

class ACharacter;

UCLASS( ClassGroup = "Camera", meta = ( BlueprintSpawnableComponent ) )
class JUBILANT_POTATO_API USmartSpringArm : public USpringArmComponent {
    GENERATED_BODY()
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    void SetIsAiming( bool IsAiming );

private: // Functions
    void UpdateFromVelocity( float DeltaTime );

    bool UpdateFromAiming( float DeltaTime );

public:
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arm Lerp" )
    float default_length = 300.f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arm Lerp" )
    float length_multiplier_velocity = 1.5f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arm Lerp" )
    float length_multiplier_aiming = 0.5f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arm Lerp" )
    float lerp_time = 0.25f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arm Lerp" )
    float start_velocity = 601.f;

private:
    ACharacter* parent;
    USceneComponent* gimbal;
    float curr_time_velocity = 0.f;
    float curr_time_aiming = 0.f;

    bool is_aiming = false;
};