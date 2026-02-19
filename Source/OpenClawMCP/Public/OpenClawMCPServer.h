// Copyright (c) 2026 OpenClawUE Contributors. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "WebSockets/Public/WebSocket.h"

class IOpenClawPythonExecutor;

/**
 * MCP (Model Context Protocol) Server for OpenClawUE
 * 
 * Provides HTTP/WebSocket server for AI assistants to control Unreal Engine.
 * Implements JSON-RPC 2.0 protocol.
 */
class OPENCLAWMCP_API FOpenClawMCPServer : public TSharedFromThis<FOpenClawMCPServer>
{
public:
	/**
	 * Constructor.
	 * @param PythonExecutor Shared pointer to Python executor
	 */
	FOpenClawMCPServer(TSharedPtr<IOpenClawPythonExecutor> InPythonExecutor);
	
	/**
	 * Destructor.
	 */
	virtual ~FOpenClawMCPServer();
	
	/**
	 * Start the MCP server.
	 * @param Port The port to listen on
	 * @return True if server started successfully
	 */
	bool Start(int32 Port = 8089);
	
	/**
	 * Stop the MCP server.
	 */
	void Stop();
	
	/**
	 * Check if server is running.
	 * @return True if server is running
	 */
	bool IsRunning() const;
	
	/**
	 * Get the server port.
	 * @return Current port number
	 */
	int32 GetPort() const;
	
	/**
	 * Get the server URL.
	 * @return Server URL as string
	 */
	FString GetServerURL() const;
	
private:
	/** Python executor for running Python code */
	TSharedPtr<IOpenClawPythonExecutor> PythonExecutor;
	
	/** HTTP server handle */
	TSharedPtr<class IHttpRouter> HttpRouter;
	
	/** WebSocket server */
	TSharedPtr<class FWebSocketServer> WebSocketServer;
	
	/** Active WebSocket connections */
	TArray<TSharedPtr<FWebSocket>> WebSocketConnections;
	
	/** Server port */
	int32 ServerPort = 8089;
	
	/** Is server running */
	bool bIsRunning = false;
	
	/** Tick handle for periodic updates */
	FTickerDelegate TickDelegate;
	FTSTicker::FDelegateHandle TickHandle;
	
	/**
	 * Handle HTTP request.
	 * @param Request HTTP request
	 * @param Response HTTP response
	 * @return True if request was handled
	 */
	bool HandleHttpRequest(const TSharedRef<IHttpRequest>& Request, const TSharedRef<IHttpResponse>& Response);
	
	/**
	 * Handle WebSocket connection.
	 * @param WebSocket WebSocket connection
	 */
	void HandleWebSocketConnection(TSharedPtr<FWebSocket> WebSocket);
	
	/**
	 * Handle WebSocket message.
	 * @param WebSocket WebSocket connection
	 * @param Message Received message
	 */
	void HandleWebSocketMessage(TSharedPtr<FWebSocket> WebSocket, const FString& Message);
	
	/**
	 * Handle WebSocket close.
	 * @param WebSocket WebSocket connection
	 * @param StatusCode Close status code
	 * @param Reason Close reason
	 */
	void HandleWebSocketClose(TSharedPtr<FWebSocket> WebSocket, int32 StatusCode, const FString& Reason);
	
	/**
	 * Process JSON-RPC request.
	 * @param RequestJson JSON request
	 * @return JSON response
	 */
	FString ProcessJsonRpcRequest(const FString& RequestJson);
	
	/**
	 * Handle MCP tool call.
	 * @param Method Method name
	 * @param Params Method parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleToolCall(const FString& Method, const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle discover_python_module tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleDiscoverPythonModule(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle discover_python_class tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleDiscoverPythonClass(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle execute_python_code tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleExecutePythonCode(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle list_python_subsystems tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleListPythonSubsystems(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle manage_skills tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleManageSkills(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Handle read_logs tool.
	 * @param Params Tool parameters
	 * @return Result JSON
	 */
	TSharedPtr<FJsonObject> HandleReadLogs(const TSharedPtr<FJsonObject>& Params);
	
	/**
	 * Send WebSocket message.
	 * @param WebSocket WebSocket connection
	 * @param Message Message to send
	 */
	void SendWebSocketMessage(TSharedPtr<FWebSocket> WebSocket, const FString& Message);
	
	/**
	 * Broadcast message to all WebSocket connections.
	 * @param Message Message to broadcast
	 */
	void BroadcastWebSocketMessage(const FString& Message);
	
	/**
	 * Server tick function.
	 * @param DeltaTime Time since last tick
	 * @return True to continue ticking
	 */
	bool Tick(float DeltaTime);
};