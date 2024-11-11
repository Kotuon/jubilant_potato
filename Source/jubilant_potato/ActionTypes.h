#pragma once

#include "CoreMinimal.h"
#include "ActionTypes.generated.h"

UENUM( BlueprintType )
enum class EActionInputIDs : uint8 {
    None UMETA( DisplayName = "None" ),
    Confirm UMETA( DisplayName = "Confirm" ),
    Cancel UMETA( DisplayName = "Cancel" ),
    Jump UMETA( DisplayName = "Jump" ),
};