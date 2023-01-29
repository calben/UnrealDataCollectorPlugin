#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "DataCollectorBPLibrary.generated.h"

UCLASS()
class UDataCollectorBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

public:
    // FJsonObjectConverter::CustomExportCallback EnumOverrideExportCallback;

    // based on https://github.com/getnamo/SocketIOClient-Unreal
    static TSharedPtr<FJsonObject> ToJsonObject(
        UStruct* Struct, void* StructPtr, bool IsBlueprintStruct = false, bool BinaryStructCppSupport = false);
    static void TrimValueKeyNames(const TSharedPtr<FJsonValue>& JsonValue);
    static bool TrimKey(const FString& InLongKey, FString& OutTrimmedKey);
};
