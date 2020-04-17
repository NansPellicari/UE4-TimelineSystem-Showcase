// Fill out your copyright notice in the Description page of Project Settings.

#include "MySaveGameManager.h"

#include "Misc/EngineVersion.h"
#include "MySaveGame.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

///////////////////////////////////////////////////////////////////////////////
// Such as chame to copy Aaaall of this from Runtime/Engine/Private/GameplayStatics.cpp ...
// TODO ask epic why they are not in the header file?
///////////////////////////////////////////////////////////////////////////////
static const int UE4_SAVEGAME_FILE_TYPE_TAG = 0x53415647;	 // "sAvG"

struct FSaveGameFileVersion
{
	enum Type
	{
		InitialVersion = 1,
		// serializing custom versions into the savegame data to handle that type of versioning
		AddedCustomVersions = 2,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
};

struct FSaveGameHeader
{
	FSaveGameHeader();
	FSaveGameHeader(TSubclassOf<UMySaveGame> ObjectType);

	void Empty();

	void Read(FMemoryReader& MemoryReader);

	int32 FileTypeTag;
	int32 SaveGameFileVersion;
	int32 PackageFileUE4Version;
	FEngineVersion SavedEngineVersion;
	int32 CustomVersionFormat;
	FCustomVersionContainer CustomVersions;
	FString SaveGameClassName;
};

FSaveGameHeader::FSaveGameHeader()
	: FileTypeTag(0)
	, SaveGameFileVersion(0)
	, PackageFileUE4Version(0)
	, CustomVersionFormat(static_cast<int32>(ECustomVersionSerializationFormat::Unknown))
{
}

FSaveGameHeader::FSaveGameHeader(TSubclassOf<UMySaveGame> ObjectType)
	: FileTypeTag(UE4_SAVEGAME_FILE_TYPE_TAG)
	, SaveGameFileVersion(FSaveGameFileVersion::LatestVersion)
	, PackageFileUE4Version(GPackageFileUE4Version)
	, SavedEngineVersion(FEngineVersion::Current())
	, CustomVersionFormat(static_cast<int32>(ECustomVersionSerializationFormat::Latest))
	, CustomVersions(FCurrentCustomVersions::GetAll())
	, SaveGameClassName(ObjectType->GetPathName())
{
}

void FSaveGameHeader::Empty()
{
	FileTypeTag = 0;
	SaveGameFileVersion = 0;
	PackageFileUE4Version = 0;
	SavedEngineVersion.Empty();
	CustomVersionFormat = (int32) ECustomVersionSerializationFormat::Unknown;
	CustomVersions.Empty();
	SaveGameClassName.Empty();
}

void FSaveGameHeader::Read(FMemoryReader& MemoryReader)
{
	Empty();

	MemoryReader << FileTypeTag;

	if (FileTypeTag != UE4_SAVEGAME_FILE_TYPE_TAG)
	{
		// this is an old saved game, back up the file pointer to the beginning and assume version 1
		MemoryReader.Seek(0);
		SaveGameFileVersion = FSaveGameFileVersion::InitialVersion;

		// Note for 4.8 and beyond: if you get a crash loading a pre-4.8 version of your savegame file and
		// you don't want to delete it, try uncommenting these lines and changing them to use the version
		// information from your previous build. Then load and resave your savegame file.
		// MemoryReader.SetUE4Ver(MyPreviousUE4Version);				// @see GPackageFileUE4Version
		// MemoryReader.SetEngineVer(MyPreviousEngineVersion);		// @see FEngineVersion::Current()
	}
	else
	{
		// Read version for this file format
		MemoryReader << SaveGameFileVersion;

		// Read engine and UE4 version information
		MemoryReader << PackageFileUE4Version;

		MemoryReader << SavedEngineVersion;

		MemoryReader.SetUE4Ver(PackageFileUE4Version);
		MemoryReader.SetEngineVer(SavedEngineVersion);

		if (SaveGameFileVersion >= FSaveGameFileVersion::AddedCustomVersions)
		{
			MemoryReader << CustomVersionFormat;

			CustomVersions.Serialize(MemoryReader, static_cast<ECustomVersionSerializationFormat::Type>(CustomVersionFormat));
			MemoryReader.SetCustomVersions(CustomVersions);
		}
	}

	// Get the class name
	MemoryReader << SaveGameClassName;
}

///////////////////////////////////////////////////////////////////////////////
// END
///////////////////////////////////////////////////////////////////////////////

// @see GameplayStatics::LoadGameFromSlot()
UMySaveGame* UMySaveGameManager::LoadGame(UObject* WorldContextObject, const FString& SlotName, const int32 UserIndex)
{
	// This is a wrapper around the functions reading to/from a byte array
	TArray<uint8> ObjectBytes;
	UMySaveGame* LoadedGame = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (UGameplayStatics::LoadDataFromSlot(ObjectBytes, SlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 2"), ANSI_TO_TCHAR(__FUNCTION__));
		LoadedGame = LoadGameFromMemory(ObjectBytes, WorldContextObject);
	}

	return LoadedGame;
}

UMySaveGame* UMySaveGameManager::LoadGameFromMemory(const TArray<uint8>& InSaveData, UObject* WorldContextObject)
{
	if (InSaveData.Num() == 0)
	{
		// Empty buffer, return instead of causing a bad serialize that could crash
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	UMySaveGame* OutSaveGameObject = nullptr;

	FMemoryReader MemoryReader(InSaveData, true);

	FSaveGameHeader SaveHeader;
	SaveHeader.Read(MemoryReader);

	// Try and find it, and failing that, load it
	UClass* SaveGameClass = FindObject<UClass>(ANY_PACKAGE, *SaveHeader.SaveGameClassName);
	if (SaveGameClass == nullptr)
	{
		SaveGameClass = LoadObject<UClass>(nullptr, *SaveHeader.SaveGameClassName);
	}

	// If we have a class, try and load it.
	if (SaveGameClass != nullptr)
	{
		OutSaveGameObject = NewObject<UMySaveGame>(GetTransientPackage(), SaveGameClass);
		OutSaveGameObject->WorldContextObject = WorldContextObject;

		FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
		OutSaveGameObject->Serialize(Ar);
	}

	return OutSaveGameObject;
}