#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Collector.generated.h"

UCLASS()
class DATACOLLECTOR_API ACollector : public AInfo
{
    GENERATED_BODY()
public:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // class ADataSink* Sink;

    UFUNCTION(BlueprintCallable)
    void Dump()
    {
    }
};
