// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MySaveGameManager.generated.h"

class UMySaveGame;

/**
 *
 */
UCLASS()
class UE4TIMELINE_API UMySaveGameManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// clang-format off
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "Nans Load Game", Keywords = "SaveGame load"), Category = "SaveGame")
	static UMySaveGame* LoadGame(UObject* WorldContextObject, const FString& SlotName, const int32 UserIndex);
	// clang-format on

	static UMySaveGame* LoadGameFromMemory(const TArray<uint8>& InSaveData, UObject* WorldContextObject);
};
