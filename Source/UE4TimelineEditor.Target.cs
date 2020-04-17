// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4TimelineEditorTarget : TargetRules
{
	public UE4TimelineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[]
		{
			"UE4Timeline",
			"NansCoreHelpers",
			"NansTimelineSystemCore",
			"NansTimelineSystemUE4"
		});
	}
}