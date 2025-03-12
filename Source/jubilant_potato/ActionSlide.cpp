
#include "ActionSlide.h"

UActionSlide::UActionSlide() { type = EAction::A_Dash; }

void UActionSlide::BeginPlay() {
    Super::BeginPlay();
    //...
}

void UActionSlide::Start( const FInputActionValue& value ) {}

void UActionSlide::End() {}

void UActionSlide::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction ) {
    if ( !isRunning ) return;
}
