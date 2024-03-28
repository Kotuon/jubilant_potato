#pragma once

#include "CoreMinimal.h"
#include "ActionTypes.generated.h"

UENUM( BlueprintType )
enum class EAction : uint8 {
    A_None = 0 UMETA( DisplayName = "None" ),
    A_Sprint = 1 UMETA( DisplayName = "Sprint" ),
    A_Jump = 1 UMETA( DisplayName = "Jump" )
};