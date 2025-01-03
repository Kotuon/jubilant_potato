#pragma once

#include "CoreMinimal.h"
#include "ActionTypes.generated.h"

UENUM( BlueprintType )
enum class EAction : uint8 {
    A_None = 0 UMETA( DisplayName = "None" ),
    A_Sprint = 1 UMETA( DisplayName = "Sprint" ),
    A_Attack = 3 UMETA( DisplayName = "Attack" ),
    A_Jump = 2 UMETA( DisplayName = "Jump" ),
    A_Aim = 2 UMETA( DisplayName = "Aim" ),
    A_GravRush = 3 UMETA( DisplayName = "GravRush" ),
    A_Move = 3 UMETA( DisplayName = "Move" ),
    A_Look = 3 UMETA( DisplayName = "Look" ),
    A_Dash = 3 UMETA( DisplayName = "Dash" ),
};