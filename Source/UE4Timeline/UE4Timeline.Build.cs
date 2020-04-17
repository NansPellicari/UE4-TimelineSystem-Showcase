// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4Timeline : ModuleRules
{
	public UE4Timeline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Kismet",
			"Slate",
			"SlateCore",
			"NansCoreHelpers",
			"NansTimelineSystemCore",
			"NansTimelineSystemUE4"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}