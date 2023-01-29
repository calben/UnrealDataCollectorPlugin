#include "Sinks/CsvSink.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"

FString ACsvSink::Header(TSharedPtr<FJsonObject> Object)
{
    TArray<FString> Keys;
    Object->Values.GetKeys(Keys);
    Keys.Sort();
    return FString::Join(Keys, TEXT(","));
}

FString ACsvSink::Line(TSharedPtr<FJsonObject> Object)
{
    TArray<FString> CsvValues;
    auto            JsonValues = Object->Values;
    JsonValues.KeySort([](FString A, FString B) { return A < B; });
    for (auto JsonValue : JsonValues)
    {
        CsvValues.Add(JsonValue.Value.Get()->AsString());
    }
    return FString::Join(CsvValues, TEXT(","));
}

// void ACsvSink::OpenFile()
//{
//     IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
//     if (PlatformFile.CreateDirectoryTree(*FileDirectory))
//     {
//         FString AbsoluteFilePath = FileDirectory + "/" + FileName;
//         FileHandle               = PlatformFile.OpenWrite(*AbsoluteFilePath);
//     }
// }

void ACsvSink::Dump()
{
    TArray<FString> Buffer;
    for (const auto& Object : Data)
    {
        if (!bHaveWrittenHeader)
        {
            Buffer.Add(Header(Object));
            bHaveWrittenHeader = true;
        }
        Buffer.Add(Line(Object));
    }
    Data.Empty();
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.CreateDirectoryTree(*FileDirectory))
    {
        FString AbsoluteFilePath = FileDirectory + "/" + FileName;
        FFileHelper::SaveStringToFile(FString::Join(Buffer, TEXT("\n")), *AbsoluteFilePath,
            FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
        FFileHelper::SaveStringToFile(TEXT("\n"), *AbsoluteFilePath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(),
            EFileWrite::FILEWRITE_Append);
    }
}
