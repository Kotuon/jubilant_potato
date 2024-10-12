// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class jubilant_potato : ModuleRules
{
    public jubilant_potato(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "UMG", "AIModule", "Niagara", "NinjaCharacter", });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "AnimGraphRuntime", "GameplayAbilities", "GameplayTags", "GameplayTasks", });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
