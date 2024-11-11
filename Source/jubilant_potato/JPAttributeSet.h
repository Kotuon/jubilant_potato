#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "JPAttributeSet.generated.h"

#define JP_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UJPAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    UPROPERTY()
    FGameplayAttributeData Hp;
    JP_ATTRIBUTE_ACCESSORS(UJPAttributeSet, Hp);
};
