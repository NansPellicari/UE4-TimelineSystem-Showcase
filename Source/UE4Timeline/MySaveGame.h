// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "MySaveGame.generated.h"

class UNRealLifeTimelineManager;

/**
 *
 */
UCLASS()
class UE4TIMELINE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, Category = Basic)
	UObject* WorldContextObject;

	UMySaveGame();

	virtual void Serialize(FArchive& Ar) override;
	void SaveTimeline(FArchive& Ar);
};
