// Copyright (c) 2026 OpenClawUE Contributors. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * OpenClawUE Module
 * 
 * Main module for OpenClaw Unreal Engine integration.
 * Provides AI assistant control over UE5 through MCP protocol.
 */
class FOpenClawUEModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/**
	 * Singleton-like access to this module's interface.
	 * This is useful for calling some functions without needing to get a reference.
	 */
	static inline FOpenClawUEModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FOpenClawUEModule>("OpenClawUE");
	}
	
	/**
	 * Checks if this module is available.
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("OpenClawUE");
	}
	
	/**
	 * Get the plugin version.
	 */
	FString GetPluginVersion() const;
	
	/**
	 * Check if MCP server is running.
	 */
	bool IsMCPServerRunning() const;
	
	/**
	 * Start the MCP server.
	 * @param Port The port to listen on (default: 8089)
	 * @return True if server started successfully
	 */
	bool StartMCPServer(int32 Port = 8089);
	
	/**
	 * Stop the MCP server.
	 */
	void StopMCPServer();
	
	/**
	 * Execute Python code in UE context.
	 * @param PythonCode The Python code to execute
	 * @return Output from execution
	 */
	FString ExecutePython(const FString& PythonCode);
	
private:
	/** Plugin version */
	FString PluginVersion = TEXT("1.0.0");
	
	/** MCP server instance */
	TSharedPtr<class IOpenClawMCPServer> MCPServer;
	
	/** Python executor */
	TSharedPtr<class IOpenClawPythonExecutor> PythonExecutor;
};