// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/R1AssetData.h"
#include "UObject/ObjectSaveContext.h"

void UR1AssetData::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
    Super::PreSave(ObjectSaveContext);

    AssetNameToPath.Empty();
    AssetLabelToSet.Empty();

    AssetGroupNameToSet.KeySort([](const FName& A, const FName& B)
        {
            return (A.Compare(B) < 0);
        });

    for (const auto& Pair : AssetGroupNameToSet)
    {
        const FAssetSet& AssetSet = Pair.Value;
        for (FAssetEntry AssetEntry : AssetSet.AssetEntries)
        {
            FSoftObjectPath& AssetPath = AssetEntry.AssetPath;
            const FString& AssetName = AssetPath.GetAssetName();

            /*if (AssetName.StartsWith(TEXT("BP_")) || AssetName.StartsWith(TEXT("B_")) || )*/

            AssetNameToPath.Emplace(AssetEntry.AssetName, AssetEntry.AssetPath);
            for (const FName& Label : AssetEntry.AssetLabels)
            {
                AssetLabelToSet.FindOrAdd(Label).AssetEntries.Emplace(AssetEntry);
            }

        }
    }
}

FSoftObjectPath UR1AssetData::GetAssetPathByName(const FName& AssetName)
{
    FSoftObjectPath* AssetPath = AssetNameToPath.Find(AssetName);
    ensureAlwaysMsgf(AssetPath, TEXT("Can't find Asset Path From Asset Name [%s]."), *AssetName.ToString());

    return *AssetPath;
}

const FAssetSet& UR1AssetData::GetAssetSetByLabel(const FName& Label)
{
    const FAssetSet* AssetSet = AssetLabelToSet.Find(Label);
    ensureAlwaysMsgf(AssetSet, TEXT("Can't find Asset Set from Label [%s]."), *Label.ToString());
    return *AssetSet;
}
