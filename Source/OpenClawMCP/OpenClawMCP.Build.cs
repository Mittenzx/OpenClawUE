using UnrealBuildTool;

public class OpenClawMCP : ModuleRules
{
	public OpenClawMCP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"HTTP",
			"Json",
			"JsonUtilities",
			"WebSockets",
			"Sockets",
			"Networking"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OpenClawPython"
		});
		
		// Enable exceptions
		bEnableExceptions = true;
		
		// Use RTTI
		bUseRTTI = true;
		
		// Add include paths
		PublicIncludePaths.AddRange(new string[]
		{
			"OpenClawMCP/Public"
		});
		
		PrivateIncludePaths.AddRange(new string[]
		{
			"OpenClawMCP/Private"
		});
	}
}