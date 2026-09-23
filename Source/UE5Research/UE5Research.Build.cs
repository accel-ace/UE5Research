// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5Research : ModuleRules
{
	public UE5Research(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UE5Research",
			"UE5Research/Variant_Platforming",
			"UE5Research/Variant_Platforming/Animation",
			"UE5Research/Variant_Combat",
			"UE5Research/Variant_Combat/AI",
			"UE5Research/Variant_Combat/Animation",
			"UE5Research/Variant_Combat/Gameplay",
			"UE5Research/Variant_Combat/Interfaces",
			"UE5Research/Variant_Combat/UI",
			"UE5Research/Variant_SideScrolling",
			"UE5Research/Variant_SideScrolling/AI",
			"UE5Research/Variant_SideScrolling/Gameplay",
			"UE5Research/Variant_SideScrolling/Interfaces",
			"UE5Research/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
