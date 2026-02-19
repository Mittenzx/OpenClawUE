// Copyright (c) 2026 OpenClawUE Contributors. All Rights Reserved.

#include "OpenClawUEModule.h"
#include "OpenClawMCP/Public/OpenClawMCPServer.h"
#include "OpenClawPython/Public/OpenClawPythonExecutor.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FOpenClawUEModule"

void FOpenClawUEModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("OpenClawUE Module Starting Up..."));
	
	// Initialize Python executor
	PythonExecutor = MakeShared<FOpenClawPythonExecutor>();
	
	// Check if we should auto-start MCP server
	// This could be configurable via project settings
	bool bAutoStartMCP = true;
	int32 MCPPort = 8089;
	
	if (bAutoStartMCP)
	{
		StartMCPServer(MCPPort);
	}
	
	UE_LOG(LogTemp, Log, TEXT("OpenClawUE Module Started Successfully"));
}

void FOpenClawUEModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("OpenClawUE Module Shutting Down..."));
	
	// Stop MCP server if running
	StopMCPServer();
	
	// Clean up Python executor
	PythonExecutor.Reset();
	
	UE_LOG(LogTemp, Log, TEXT("OpenClawUE Module Shut Down"));
}

FString FOpenClawUEModule::GetPluginVersion() const
{
	return PluginVersion;
}

bool FOpenClawUEModule::IsMCPServerRunning() const
{
	return MCPServer.IsValid() && MCPServer->IsRunning();
}

bool FOpenClawUEModule::StartMCPServer(int32 Port)
{
	if (IsMCPServerRunning())
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP server is already running"));
		return true;
	}
	
	MCPServer = MakeShared<FOpenClawMCPServer>(PythonExecutor);
	bool bStarted = MCPServer->Start(Port);
	
	if (bStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("MCP server started on port %d"), Port);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to start MCP server on port %d"), Port);
	}
	
	return bStarted;
}

void FOpenClawUEModule::StopMCPServer()
{
	if (MCPServer.IsValid())
	{
		MCPServer->Stop();
		MCPServer.Reset();
		UE_LOG(LogTemp, Log, TEXT("MCP server stopped"));
	}
}

FString FOpenClawUEModule::ExecutePython(const FString& PythonCode)
{
	if (!PythonExecutor.IsValid())
	{
		return TEXT("Python executor not available");
	}
	
	return PythonExecutor->Execute(PythonCode);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOpenClawUEModule, OpenClawUE)