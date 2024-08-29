// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Projectile.h"
#include "NiagaraFunctionLibrary.h"                    // SpawnSystemAtLocation()
#include "Components/SphereComponent.h"                //USphereComponent class
#include "GameFramework/ProjectileMovementComponent.h" // UProjectileMovementComponent class
#include "NiagaraComponent.h"                          // UNiagaraComponent class
#include "PlayerCharacter.h"                           // APlayerCharacter class
#include "Kismet/KismetMathLibrary.h"                  // MakeRotFromX()
#include "Enemy.h"                                     // AEnemy class

// Sets default values
ABase_Projectile::ABase_Projectile() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    if ( !RootComponent ) {
        RootComponent = CreateDefaultSubobject< USceneComponent >( TEXT( "ProjectileSceneComponent" ) );
    }

    if ( !collision_component ) {
        // Use a sphere as a simple collision representation.
        collision_component = CreateDefaultSubobject< USphereComponent >( TEXT( "SphereComponent" ) );
        // Set the sphere's collision radius.
        collision_component->InitSphereRadius( 15.0f );
        // Set the root component to be the collision component.
        RootComponent = collision_component;

        collision_component->OnComponentHit.AddDynamic( this, &ABase_Projectile::OnHit );
    }

    if ( !projectile_movement_component ) {
        // Use this component to drive this projectile's movement.
        projectile_movement_component = CreateDefaultSubobject< UProjectileMovementComponent >( TEXT( "projectile_movement_component" ) );
        projectile_movement_component->SetUpdatedComponent( collision_component );
        projectile_movement_component->InitialSpeed = 3000.0f;
        projectile_movement_component->MaxSpeed = 3000.0f;
        projectile_movement_component->bRotationFollowsVelocity = true;
        projectile_movement_component->bShouldBounce = true;
        projectile_movement_component->Bounciness = 0.3f;
        projectile_movement_component->ProjectileGravityScale = 0.0f;
    }
}

// Called when the game starts or when spawned
void ABase_Projectile::BeginPlay() {
    Super::BeginPlay();

    SetLifeSpan( life_time );
    has_muzzle = false;
    // UGameplayStatics::SpawnSoundAttached(InAirSound, GetRootComponent(), "None", FVector(0), FRotator(0));
    // UGameplayStatics::SpawnSoundAttached( InAirSound, GetRootComponent(), "None", FVector( 0 ), FRotator( 0 ), EAttachLocation::KeepRelativeOffset, false, 1.f, 1.f, InAirStartTime );
}

// Called every frame
void ABase_Projectile::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );

    if ( has_muzzle )
        return;

    UWorld* world = GetWorld();
    if ( !world )
        return;

    UNiagaraComponent* newEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation( world, hit_VFX,
                                                                                   start_location,
                                                                                   GetActorRotation() );
    if ( newEffect ) {
        newEffect->SetVariableFloat( "Scale", muzzle_size );
    }

    has_muzzle = true;
}

void ABase_Projectile::FireInDirection( const FVector& ShootDirection ) {
    projectile_movement_component->Velocity = ShootDirection * projectile_movement_component->InitialSpeed;
}

void ABase_Projectile::OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit ) {

    if ( OtherActor == this || OtherActor == parent ) {
        return;
    }

    GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, OtherActor->GetName() );

    // TArray< UAudioComponent* > audioComponents;
    // GetComponents< UAudioComponent >( audioComponents );

    // for ( int i = 0; i < audioComponents.Num(); ++i ) {
    //     audioComponents[i]->FadeOut( 1.f, 0.f, EAudioFaderCurve::Linear );
    // }

    UWorld* world = GetWorld();
    if ( !world )
        return;

    AEnemy* enemy = Cast< AEnemy >( OtherActor );
    if ( IsValid( enemy ) ) {
        enemy->ApplyDamage( 1 );
    }

    UNiagaraComponent* newEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation( world, hit_VFX,
                                                                                   Hit.ImpactPoint,
                                                                                   UKismetMathLibrary::MakeRotFromX( Hit.ImpactNormal ) );

    if ( newEffect ) {
        newEffect->SetVariableFloat( "Scale", hit_size );
    }

    Destroy();
}