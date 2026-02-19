using UnrealBuildTool;

public class OpenClawUE : ModuleRules
{
	public OpenClawUE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HTTP",
			"Json",
			"JsonUtilities",
			"WebSockets",
			"PythonScriptPlugin",
			"EditorScriptingUtilities"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OpenClawMCP",
			"OpenClawPython"
		});
		
		// Enable Python support
		bEnablePythonSupport = true;
		
		// Enable exceptions
		bEnableExceptions = true;
		
		// Use RTTI
		bUseRTTI = true;
		
		// Enable undefined behavior sanitizer
		bEnableUndefinedBehaviorSanitizer = false;
		
		// Enable address sanitizer
		bEnableAddressSanitizer = false;
	}
}