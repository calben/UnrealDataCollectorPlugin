#pragma once

#include "BaseSink.h"
#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "CsvSink.generated.h"

UCLASS()
class DATACOLLECTOR_API ACsvSink : public ABaseSink
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FileDirectory = FString("/");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FileName = FString("Default.csv");

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bHaveWrittenHeader = false;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    // IFileHandle* FileHandle;

    FString Header(TSharedPtr<FJsonObject> Object);

    FString Line(TSharedPtr<FJsonObject> Object);

    // void OpenFile();

    virtual void Dump() override;
};
