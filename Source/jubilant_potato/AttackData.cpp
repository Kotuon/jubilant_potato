// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackData.h"

UAttackData::UAttackData( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer ) {}

const TSet< FAttackInfo >& UAttackData::GetAttackInfo() const {
    return Attacks;
}

// #if WITH_EDITOR
// void UAttackData::PostEditChangeProperty(
//     FPropertyChangedEvent& PropertyChangedEvent ) {
//     Super::PostEditChangeProperty( PropertyChangedEvent );

//     const FProperty* Property = PropertyChangedEvent.Property;
//     if ( Property &&
//          Property->GetFName() ==
//              GET_MEMBER_NAME_CHECKED( UAttackData, Attacks ) &&
//          !Attacks.IsEmpty() ) {
//         TArray< FAttackInfo > AttacksArray = Attacks.Array();

//         for ( int32 i = AttacksArray.Num() - 1; i > 0; --i ) {
//             const int32 PrevIndex = i - 1;
//             if ( AttacksArray[i] == AttacksArray[PrevIndex] ) {
//                 AttacksArray.RemoveAtSwap( i );
//             }
//         }

//         Attacks.Reset();

//         for ( int32 i = 0; i < AttacksArray.Num(); ++i ) {
//             FAttackInfo& Info = AttacksArray[i];
//             Info.InputID = i;
//             Attacks.Add( Info );
//         }
//     }
// }
// #endif
