#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DataCollectorBPLibrary.h"
#include "JsonObjectConverter.h"
#include "Sinks/BaseSink.h"
#include "Templates/Casts.h"
#include "UObject/UnrealTypePrivate.h"

#include "DataCollectorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DATACOLLECTOR_API UDataCollectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDataCollectorComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ABaseSink* Sink;

    UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "AnyStruct"))
    void AddInfo(UProperty* AnyStruct);

    DECLARE_FUNCTION(execAddInfo)
    {
        Stack.Step(Stack.Object, NULL);
        UStructProperty* StructProperty = (UStructProperty*) (Stack.MostRecentProperty);
        void*            StructPtr      = Stack.MostRecentPropertyAddress;

        P_FINISH;
        TSharedPtr<FJsonObject> JsonObject = UDataCollectorBPLibrary::ToJsonObject(StructProperty->Struct, StructPtr, true);
        if (P_THIS->Sink != nullptr)
        {
            P_THIS->Sink->Add(JsonObject);
        }
    }

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
