// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RenderLab : ModuleRules
{
	public RenderLab(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		//PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		//PublicIncludePaths.AddRange(
		//	new string[] {
		//		"Core"
		//	}
		//	);
				
		
		//PrivateIncludePaths.AddRange(
		//	new string[] {
		//		"CoreUObject",
  //              "Engine",
  //              "RenderCore",
  //              "RHI"
		//	}
		//	);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"RenderCore",
				"RHI",
				"Projects",
				"Renderer",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
