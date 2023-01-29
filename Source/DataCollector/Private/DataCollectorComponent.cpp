#include "DataCollectorComponent.h"

UDataCollectorComponent::UDataCollectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDataCollectorComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UDataCollectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
