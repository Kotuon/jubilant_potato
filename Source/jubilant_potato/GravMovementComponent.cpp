// Fill out your copyright notice in the Description page of Project Settings.

#include "GravMovementComponent.h"
#include "GameFramework/Character.h" // ACharacter class

void UGravMovementComponent::BeginPlay() {
    CharacterOwner->MovementModeChangedDelegate.AddUniqueDynamic(
        this, &UGravMovementComponent::MovementModeChanged );
}

void UGravMovementComponent::OnMovementUpdated( float DeltaSeconds, const FVector& OldLocation,
                                                const FVector& OldVelocity ) {
    Super::OnMovementUpdated( DeltaSeconds, OldLocation, OldVelocity );
    //...
}

void UGravMovementComponent::UpdateGravity() {
    if ( !CurrentFloor.HitResult.ImpactNormal.IsZero() ) {
        SetGravityDirection( CurrentFloor.HitResult.ImpactNormal * -1.f );
    }
}

void UGravMovementComponent::MovementModeChanged( ACharacter* Character,
                                                  EMovementMode PrevMovementMode,
                                                  uint8 PrevCustomMode ) {
    if ( PrevMovementMode == MOVE_Falling && MovementMode == MOVE_Walking ) {
        UpdateGravity();
    }
}

bool UGravMovementComponent::ShouldRemainVertical() const {
    return IsMovingOnGround();
}

void UGravMovementComponent::UpdateRotation( float DeltaTime ) {
    if ( MovementMode == MOVE_Falling ) {
        FHitResult hitResult;
        const FVector start = GetActorLocation();
        const FVector end = start + GetGravityDirection() * 400.f;
        bool overLand = GetWorld()->LineTraceSingleByChannel( hitResult, start, end,
                                                              ECollisionChannel::ECC_Visibility );

        if ( overLand ) {
            DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 0.f, ( uint8 )0U, 2.f );

            FRotator deltaRotation = GetDeltaRotation( DeltaTime );

            FRotator currentRotation = CharacterOwner->GetActorRotation();

            FRotator desiredRotation = ComputeOrientToMovementRotation(
                currentRotation, DeltaTime, deltaRotation );

            FRotator gravityRelativeDesiredRotation =
                ( GetGravityToWorldTransform() * desiredRotation.Quaternion() ).Rotator();

            gravityRelativeDesiredRotation.Pitch = 0.f;
            gravityRelativeDesiredRotation.Yaw = FRotator::NormalizeAxis(
                gravityRelativeDesiredRotation.Yaw );
            gravityRelativeDesiredRotation.Roll = 0.f;

            desiredRotation = ( GetWorldToGravityTransform() *
                                gravityRelativeDesiredRotation.Quaternion() )
                                  .Rotator();

            // if ( CharacterMovementCVars::bPreventNonVerticalOrientationBlock ) {
            //     if ( FMath::IsNearlyZero( deltaRotation.Pitch ) ) {
            //         deltaRotation.Pitch = 360.0;
            //     }
            //     if ( FMath::IsNearlyZero( deltaRotation.Roll ) ) {
            //         deltaRotation.Roll = 360.0;
            //     }
            // }

            const float angleTolerance = 1e-3f;

            FRotator gravityRelativeCurrentRotation =
                ( GetGravityToWorldTransform() * currentRotation.Quaternion() ).Rotator();

            if ( !FMath::IsNearlyEqual( gravityRelativeCurrentRotation.Pitch,
                                        gravityRelativeDesiredRotation.Pitch, angleTolerance ) ) {
                gravityRelativeDesiredRotation.Pitch = FMath::FixedTurn(
                    gravityRelativeCurrentRotation.Pitch, gravityRelativeDesiredRotation.Pitch,
                    deltaRotation.Pitch );
            }

            if ( !FMath::IsNearlyEqual( gravityRelativeCurrentRotation.Yaw,
                                        gravityRelativeDesiredRotation.Yaw, angleTolerance ) ) {
                gravityRelativeDesiredRotation.Yaw = FMath::FixedTurn(
                    gravityRelativeCurrentRotation.Yaw, gravityRelativeDesiredRotation.Yaw,
                    deltaRotation.Yaw );
            }

            if ( !FMath::IsNearlyEqual( gravityRelativeCurrentRotation.Roll,
                                        gravityRelativeDesiredRotation.Roll, angleTolerance ) ) {
                gravityRelativeDesiredRotation.Roll = FMath::FixedTurn(
                    gravityRelativeCurrentRotation.Roll, gravityRelativeDesiredRotation.Roll,
                    deltaRotation.Roll );
            }

            desiredRotation = ( GetWorldToGravityTransform() *
                                gravityRelativeDesiredRotation.Quaternion() )
                                  .Rotator();

            desiredRotation =
                ( GetGravityToWorldTransform() * currentRotation.Quaternion() ).Rotator();

            desiredRotation.DiagnosticCheckNaN( TEXT( "CharacterMovementComponent::PhysicsRotation(): DesiredRotation" ) );
            MoveUpdatedComponent( FVector::ZeroVector, desiredRotation, false );

            GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Green, desiredRotation.ToString() );
            GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, currentRotation.ToString() );

            // CharacterOwner->SetActorRotation( FMath::Lerp( CharacterOwner->GetActorRotation(), ) )
        } else {
            DrawDebugLine( GetWorld(), start, end, FColor::Green, false, 0.f, ( uint8 )0U, 2.f );
        }
    }
}
