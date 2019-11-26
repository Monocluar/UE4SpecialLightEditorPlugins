// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CurveRouteCreate : ModuleRules
{
	public CurveRouteCreate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				
				//"Editor/UnrealEd/Private"
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				//"TimeManagement",
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				//"Curves",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.bBuildEditor == true)
		{
			PublicIncludePaths.AddRange(
			new string[] {
				
				// ... add public include paths required here ...
			}
			);

			PrivateIncludePaths.AddRange(
			new string[] {
				
				//"Editor/UnrealEd/Private"
				// ... add other private include paths required here ...
			}
			);
			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"InputCore",
					"UnrealEd",
					"LevelEditor",
				}
			);  // @todo api: Only public because of WITH_EDITOR and UNREALED_API

		}
	}
}
