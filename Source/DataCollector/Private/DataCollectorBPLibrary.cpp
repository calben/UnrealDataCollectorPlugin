#include "DataCollectorBPLibrary.h"

#include "DataCollector.h"
#include "JsonObjectConverter.h"

UDataCollectorBPLibrary::UDataCollectorBPLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

TSharedPtr<FJsonObject> UDataCollectorBPLibrary::ToJsonObject(
    UStruct* StructDefinition, void* StructPtr, bool IsBlueprintStruct, bool BinaryStructCppSupport /*= false */)
{
    TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    if (IsBlueprintStruct || BinaryStructCppSupport)
    {
        //// Handle BP enum override
        // if (!EnumOverrideExportCallback.IsBound())
        //{
        //     EnumOverrideExportCallback.BindLambda(
        //         [](FProperty* Property, const void* Value)
        //         {
        //             if (FByteProperty* BPEnumProperty = CastField<FByteProperty>(Property))
        //             {
        //                 // Override default enum behavior by fetching display name text
        //                 UEnum* EnumDef = BPEnumProperty->Enum;

        //                uint8 IntValue = *(uint8*) Value;

        //                // It's an enum byte
        //                if (EnumDef)
        //                {
        //                    FString StringValue = EnumDef->GetDisplayNameTextByIndex(IntValue).ToString();
        //                    return (TSharedPtr<FJsonValue>) MakeShared<FJsonValueString>(StringValue);
        //                }
        //                // it's a regular byte, convert to number
        //                else
        //                {
        //                    return (TSharedPtr<FJsonValue>) MakeShared<FJsonValueNumber>(IntValue);
        //                }
        //            }
        //            // byte array special case
        //            else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
        //            {
        //                // is it a byte array?
        //                if (ArrayProperty->Inner->IsA<FByteProperty>())
        //                {
        //                    FScriptArrayHelper ArrayHelper(ArrayProperty, Value);
        //                    TArray<uint8>      ByteArray(ArrayHelper.GetRawPtr(), ArrayHelper.Num());
        //                    return USIOJConvert::ToJsonValue(ByteArray);
        //                }
        //            }

        //            // invalid
        //            return TSharedPtr<FJsonValue>();
        //        });
        //}

        // Get the object keys
        FJsonObjectConverter::UStructToJsonObject(StructDefinition, StructPtr, JsonObject, 0, 0 /*, &EnumOverrideExportCallback */);

        // Wrap it into a value and pass it into the trimmer
        TSharedPtr<FJsonValue> JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
        UDataCollectorBPLibrary::TrimValueKeyNames(JsonValue);

        // Return object with trimmed names
        return JsonValue->AsObject();
    }
    else
    {
        FJsonObjectConverter::UStructToJsonObject(StructDefinition, StructPtr, JsonObject, 0, 0);
        return JsonObject;
    }
}

void UDataCollectorBPLibrary::TrimValueKeyNames(const TSharedPtr<FJsonValue>& JsonValue)
{
    // Array?
    if (JsonValue->Type == EJson::Array)
    {
        auto Array = JsonValue->AsArray();

        for (auto SubValue : Array)
        {
            TrimValueKeyNames(SubValue);
        }
    }
    // Object?
    else if (JsonValue->Type == EJson::Object)
    {
        auto JsonObject = JsonValue->AsObject();
        for (auto Pair : JsonObject->Values)
        {
            const FString& Key = Pair.Key;
            FString        TrimmedKey;

            bool DidNeedTrimming = TrimKey(Key, TrimmedKey);

            // keep attempting sub keys even if we have a valid string
            auto SubValue = Pair.Value;
            TrimValueKeyNames(SubValue);

            if (DidNeedTrimming)
            {
                // Replace field names with the trimmed key
                JsonObject->SetField(TrimmedKey, SubValue);
                JsonObject->RemoveField(Key);
            }
        }
    }
    else
    {
        // UE_LOG(LogTemp, Warning, TEXT("TrimValueKeyNames:: uncaught type is: %d"), (int)JsonValue->Type);
    }
}

bool UDataCollectorBPLibrary::TrimKey(const FString& InLongKey, FString& OutTrimmedKey)
{
    // Look for the position of the 2nd '_'
    int32 LastIndex = InLongKey.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    LastIndex       = InLongKey.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, LastIndex);

    if (LastIndex >= 0)
    {
        OutTrimmedKey = InLongKey.Mid(0, LastIndex);
        ;
        return true;
    }
    else
    {
        return false;
    }
}
