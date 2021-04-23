// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TOPDOWN_Take1 : ModuleRules
{
	public TOPDOWN_Take1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore", "NavigationSystem", "AIModule", "GameplayTasks", "GameplayAbilities", "GameplayTags" });
    }
}
