// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class JUBILANT_POTATO_API ABase_Projectile : public AActor {
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    ABase_Projectile();

    virtual void Tick( float DeltaTime ) override;

    void FireInDirection( const FVector& ShootDirection );

    UFUNCTION()
    void OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

    UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
    USphereComponent* collision_component;

    UPROPERTY( VisibleAnywhere, Category = Movement )
    UProjectileMovementComponent* projectile_movement_component;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Attack" )
    int damage = 1;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|General" )
    float life_time = 2.f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Effects" )
    class UNiagaraSystem* hit_VFX;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Effects" )
    class UNiagaraSystem* muzzle_VFX;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Effects" )
    float hit_size = 1.f;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Effects" )
    float muzzle_size = 0.25f;

    // UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Sound" )
    // UMetaSoundSource* spawn_sound;

    // UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Sound" )
    // UMetaSoundSource* in_air_sound;

    // UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Sound" )
    // UMetaSoundSource* impact_sound;

    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats|Sound" )
    float in_air_start_time = 0.f;

    AActor* parent;
    FVector start_location;

    bool has_muzzle = false;
};
