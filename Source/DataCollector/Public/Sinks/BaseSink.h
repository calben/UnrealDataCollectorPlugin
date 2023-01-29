#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "BaseSink.generated.h"

UCLASS()
class DATACOLLECTOR_API ABaseSink : public AInfo
{
    GENERATED_BODY()

public:
    TArray<TSharedPtr<FJsonObject>> Data;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int MaxObjectsInQueue = 128;

    void Add(TSharedPtr<FJsonObject> InObject)
    {
        Data.Add(InObject);
        if (Data.Num() > MaxObjectsInQueue)
        {
            Dump();
        }
    }

    virtual void Dump()
    {
    }

    virtual void DestroyComponent(bool bPromoteChildren) override
    {
        Dump();
    }
};
