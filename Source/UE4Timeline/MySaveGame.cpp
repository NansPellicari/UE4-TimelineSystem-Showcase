// Fill out your copyright notice in the Description page of Project Settings.

#include "MySaveGame.h"

#include "Engine.h"
#include "TimelineClient.h"
#include "TimelineGameSubsystem.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"

UMySaveGame::UMySaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UMySaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	SaveTimeline(Ar);
}

void UMySaveGame::SaveTimeline(FArchive& Ar)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	UE_LOG(LogTemp, Warning, TEXT("%s - saving"), ANSI_TO_TCHAR(__FUNCTION__));
	if (WorldContextObject == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("%s World Context"), ANSI_TO_TCHAR(__FUNCTION__));
	if (WorldContextObject->GetWorld() == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("%s World"), ANSI_TO_TCHAR(__FUNCTION__));
	if (WorldContextObject->GetWorld()->GetGameInstance() == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("%s GameInstance"), ANSI_TO_TCHAR(__FUNCTION__));

	UGameInstance* GI = WorldContextObject->GetWorld()->GetGameInstance();
	if (!ensure(IsValid(GI))) return;

	UTimelineGameSubsystem* MySubsystem = GI->GetSubsystem<UTimelineGameSubsystem>();
	if (!ensure(IsValid(MySubsystem))) return;

	UE_LOG(LogTemp, Warning, TEXT("%s INTimelineGameInstance"), ANSI_TO_TCHAR(__FUNCTION__));

	MySubsystem->GetTimelineClient()->Serialize(Ar);

	if (Ar.IsLoading())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - loading"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	else if (Ar.IsSaving())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - saving"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - other"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}
