// Copyright (c) 2026 OpenClawUE Contributors. All Rights Reserved.

#include "OpenClawMCPServer.h"
#include "OpenClawPython/Public/OpenClawPythonExecutor.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "WebSockets/Public/WebSocket.h"
#include "WebSockets/Public/WebSocketServer.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformTime.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FOpenClawMCPServer"

FOpenClawMCPServer::FOpenClawMCPServer(TSharedPtr<IOpenClawPythonExecutor> InPythonExecutor)
	: PythonExecutor(InPythonExecutor)
	, ServerPort(8089)
	, bIsRunning(false)
{
	UE_LOG(LogTemp, Log, TEXT("OpenClawMCPServer created"));
}

FOpenClawMCPServer::~FOpenClawMCPServer()
{
	Stop();
}

bool FOpenClawMCPServer::Start(int32 Port)
{
	if (bIsRunning)
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP server already running"));
		return true;
	}
	
	ServerPort = Port;
	
	// Create HTTP router
	HttpRouter = FHttpServerModule::Get().GetHttpRouter(ServerPort);
	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create HTTP router on port %d"), ServerPort);
		return false;
	}
	
	// Bind MCP endpoint
	HttpRouter->BindRoute(FHttpPath(TEXT("/mcp")), EHttpServerRequestVerbs::VERB_POST,
		[this](const FHttpServerRequest& Request, const FHttpServerResponse& Response)
		{
			return HandleHttpRequest(Request, Response);
		});
	
	// Bind health check endpoint
	HttpRouter->BindRoute(FHttpPath(TEXT("/health")), EHttpServerRequestVerbs::VERB_GET,
		[this](const FHttpServerRequest& Request, const FHttpServerResponse& Response)
		{
			TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			JsonObject->SetStringField(TEXT("status"), TEXT("healthy"));
			JsonObject->SetStringField(TEXT("service"), TEXT("OpenClawUE MCP Server"));
			JsonObject->SetNumberField(TEXT("port"), ServerPort);
			JsonObject->SetBoolField(TEXT("python_available"), PythonExecutor.IsValid());
			
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject, Writer);
			
			Response->SetContentAsString(JsonString);
			Response->SetContentType(TEXT("application/json"));
			Response->SetStatusCode(EHttpServerResponseCodes::Ok);
			return true;
		});
	
	// Start WebSocket server
	WebSocketServer = FWebSocketServer::Create(ServerPort);
	if (WebSocketServer.IsValid())
	{
		WebSocketServer->OnWebSocketConnection().BindLambda([this](TSharedPtr<FWebSocket> WebSocket)
		{
			HandleWebSocketConnection(WebSocket);
		});
		
		if (WebSocketServer->Start())
		{
			UE_LOG(LogTemp, Log, TEXT("WebSocket server started on port %d"), ServerPort);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to start WebSocket server"));
			WebSocketServer.Reset();
		}
	}
	
	// Start tick for periodic updates
	TickDelegate = FTickerDelegate::CreateRaw(this, &FOpenClawMCPServer::Tick);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 1.0f);
	
	bIsRunning = true;
	UE_LOG(LogTemp, Log, TEXT("MCP server started on port %d"), ServerPort);
	UE_LOG(LogTemp, Log, TEXT("HTTP endpoint: http://127.0.0.1:%d/mcp"), ServerPort);
	UE_LOG(LogTemp, Log, TEXT("Health check: http://127.0.0.1:%d/health"), ServerPort);
	
	return true;
}

void FOpenClawMCPServer::Stop()
{
	if (!bIsRunning)
	{
		return;
	}
	
	// Stop tick
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}
	
	// Close WebSocket connections
	for (TSharedPtr<FWebSocket> WebSocket : WebSocketConnections)
	{
		if (WebSocket.IsValid())
		{
			WebSocket->Close();
		}
	}
	WebSocketConnections.Empty();
	
	// Stop WebSocket server
	if (WebSocketServer.IsValid())
	{
		WebSocketServer->Stop();
		WebSocketServer.Reset();
	}
	
	// Clear HTTP router
	if (HttpRouter.IsValid())
	{
		HttpRouter.Reset();
	}
	
	bIsRunning = false;
	UE_LOG(LogTemp, Log, TEXT("MCP server stopped"));
}

bool FOpenClawMCPServer::IsRunning() const
{
	return bIsRunning;
}

int32 FOpenClawMCPServer::GetPort() const
{
	return ServerPort;
}

FString FOpenClawMCPServer::GetServerURL() const
{
	return FString::Printf(TEXT("http://127.0.0.1:%d"), ServerPort);
}

bool FOpenClawMCPServer::HandleHttpRequest(const TSharedRef<IHttpRequest>& Request, const TSharedRef<IHttpResponse>& Response)
{
	FString RequestBody = Request->GetContentAsString();
	
	// Process JSON-RPC request
	FString ResponseBody = ProcessJsonRpcRequest(RequestBody);
	
	Response->SetContentAsString(ResponseBody);
	Response->SetContentType(TEXT("application/json"));
	Response->SetStatusCode(EHttpServerResponseCodes::Ok);
	
	return true;
}

void FOpenClawMCPServer::HandleWebSocketConnection(TSharedPtr<FWebSocket> WebSocket)
{
	if (!WebSocket.IsValid())
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("WebSocket connection established"));
	
	WebSocketConnections.Add(WebSocket);
	
	// Set up message handler
	WebSocket->OnMessage().BindLambda([this, WebSocket](const FString& Message)
	{
		HandleWebSocketMessage(WebSocket, Message);
	});
	
	// Set up close handler
	WebSocket->OnClosed().BindLambda([this, WebSocket](int32 StatusCode, const FString& Reason)
	{
		HandleWebSocketClose(WebSocket, StatusCode, Reason);
	});
	
	// Send welcome message
	TSharedRef<FJsonObject> WelcomeMessage = MakeShared<FJsonObject>();
	WelcomeMessage->SetStringField(TEXT("type"), TEXT("welcome"));
	WelcomeMessage->SetStringField(TEXT("service"), TEXT("OpenClawUE MCP Server"));
	WelcomeMessage->SetNumberField(TEXT("port"), ServerPort);
	WelcomeMessage->SetStringField(TEXT("version"), TEXT("1.0.0"));
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(WelcomeMessage, Writer);
	
	SendWebSocketMessage(WebSocket, JsonString);
}

void FOpenClawMCPServer::HandleWebSocketMessage(TSharedPtr<FWebSocket> WebSocket, const FString& Message)
{
	if (!WebSocket.IsValid())
	{
		return;
	}
	
	// Process JSON-RPC request
	FString Response = ProcessJsonRpcRequest(Message);
	
	// Send response back
	SendWebSocketMessage(WebSocket, Response);
}

void FOpenClawMCPServer::HandleWebSocketClose(TSharedPtr<FWebSocket> WebSocket, int32 StatusCode, const FString& Reason)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket connection closed: %s"), *Reason);
	WebSocketConnections.Remove(WebSocket);
}

FString FOpenClawMCPServer::ProcessJsonRpcRequest(const FString& RequestJson)
{
	TSharedPtr<FJsonObject> RequestObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RequestJson);
	
	if (!FJsonSerializer::Deserialize(Reader, RequestObject))
	{
		// Invalid JSON
		TSharedRef<FJsonObject> ErrorResponse = MakeShared<FJsonObject>();
		ErrorResponse->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));
		ErrorResponse->SetObjectField(TEXT("error"), MakeShared<FJsonObject>());
		ErrorResponse->GetObjectField(TEXT("error"))->SetNumberField(TEXT("code"), -32700);
		ErrorResponse->GetObjectField(TEXT("error"))->SetStringField(TEXT("message"), TEXT("Parse error"));
		ErrorResponse->SetNumberField(TEXT("id"), 0);
		
		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(ErrorResponse, Writer);
		return JsonString;
	}
	
	// Extract JSON-RPC fields
	FString JsonRpcVersion = RequestObject->GetStringField(TEXT("jsonrpc"));
	FString Method = RequestObject->GetStringField(TEXT("method"));
	TSharedPtr<FJsonObject> Params = RequestObject->GetObjectField(TEXT("params"));
	int32 Id = RequestObject->GetIntegerField(TEXT("id"));
	
	// Handle tool call
	TSharedPtr<FJsonObject> Result = HandleToolCall(Method, Params);
	
	// Build response
	TSharedRef<FJsonObject> ResponseObject = MakeShared<FJsonObject>();
	ResponseObject->SetStringField(TEXT("jsonrpc"), JsonRpcVersion);
	ResponseObject->SetObjectField(TEXT("result"), Result);
	ResponseObject->SetNumberField(TEXT("id"), Id);
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(ResponseObject, Writer);
	
	return JsonString;
}

TSharedPtr<FJsonObject> FOpenClawMCPServer::HandleToolCall(const FString& Method, const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	
	if (Method == TEXT("discover_python_module"))
	{
		return HandleDiscoverPythonModule(Params);
	}
	else if (Method == TEXT("discover_python_class"))
	{
		return HandleDiscoverPythonClass(Params);
	}
	else if (Method == TEXT("execute_python_code"))
	{
		return HandleExecutePythonCode(Params);
	}
	else if (Method == TEXT("list_python_subsystems"))
	{
		return HandleListPythonSubsystems(Params);
	}
	else if (Method == TEXT("manage_skills"))
	{
		return HandleManageSkills(Params);
	}
	else if (Method == TEXT("read_logs"))
	{
		return HandleReadLogs(Params);
	}
	else
	{
		// Unknown method
		Result->SetBoolField(TEXT("success"), false);
		Result->SetStringField(TEXT("error"), FString::Printf(TEXT("Unknown method: %s"), *Method));
		return Result;
	}
}

TSharedPtr<FJsonObject> FOpenClawMCPServer::HandleDiscoverPythonModule(const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	
	if (!PythonExecutor.IsValid())
	{
		Result->SetBoolField(TEXT("success"), false);
		Result->SetStringField(TEXT("error"), TEXT("Python executor not available"));
		return Result;
	}
	
	FString ModuleName = Params->GetStringField(TEXT("module_name"));
	FString NameFilter = Params->GetStringField(TEXT("name_filter"));
	bool IncludeClasses = Params->GetBoolField(TEXT("include_classes"));
	bool IncludeFunctions = Params->GetBoolField(TEXT("include_functions"));
	int32 MaxItems = Params->GetIntegerField(TEXT("max_items"));
	
	// This would call Python executor to discover module
	// For now, return stub response
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("module"), ModuleName);
	
	TArray<TSharedPtr<FJsonValue>> ClassesArray;
	TArray<TSharedPtr<FJsonValue>> FunctionsArray;
	TArray<TSharedPtr<FJsonValue>> ConstantsArray;
	
	if (IncludeClasses)
	{
		// Add stub classes
		TSharedPtr<FJsonObject> Class1 = MakeShared<FJsonObject>();
		Class1->SetStringField(TEXT("name"), TEXT("BlueprintService"));
		Class1->SetStringField(TEXT("description"), TEXT("Blueprint creation and manipulation"));
		ClassesArray.Add(MakeShared<FJsonValueObject>(Class1));
	}
	
	if (IncludeFunctions)
	{
		// Add stub functions
		TSharedPtr<FJsonObject> Func1 = MakeShared<FJsonObject>();
		Func1->SetStringField(TEXT("name"), TEXT("create_blueprint"));
		Func1->SetStringField(TEXT("description"), TEXT("Create a new blueprint"));
		FunctionsArray.Add(MakeShared<FJsonValueObject>(Func1));
	}
	
	Result->SetArrayField(TEXT("classes"), ClassesArray);
	Result->SetArrayField(TEXT("functions"), FunctionsArray);
	Result->SetArrayField(TEXT("constants"), ConstantsArray);
	
	return Result;
}

TSharedPtr<FJsonObject> FOpenClawMCPServer::HandleDiscoverPythonClass(const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	
	FString ClassName = Params->GetStringField(TEXT("class_name"));
	FString MethodFilter = Params->GetStringField(TEXT("method_filter"));
	bool IncludeInherited = Params->GetBoolField(TEXT("include_inherited"));
	bool IncludePrivate = Params->GetBoolField(TEXT("include_private"));
	int32 MaxMethods = Params->GetIntegerField(TEXT("max_methods"));
	
	// Stub response
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("class"), ClassName);
	
	TArray<TSharedPtr<FJsonValue>> MethodsArray;
	TArray<TSharedPtr<FJsonValue>> PropertiesArray;
	TArray<TSharedPtr<FJsonValue>> InheritanceArray;
	
	// Add stub methods
	TSharedPtr<FJsonObject> Method1 = MakeShared<FJsonObject>();
	Method1->SetStringField(TEXT("name"), TEXT("create_blueprint"));
	Method1->SetStringField(TEXT("signature"), TEXT("(name: str, parent_class: str, path: str) -> Dict[str, Any]"));
	Method1->SetStringField(TEXT("description"), TEXT("Create a new blueprint asset"));
	MethodsArray.Add(MakeShared<FJsonValueObject>(Method1));
	
	TSharedPtr<FJsonObject> Method2 = MakeShared<FJsonObject>();
	Method2->SetStringField(TEXT("name"), TEXT("add_variable"));
	Method2->SetStringField(TEXT("signature"), TEXT("(blueprint_path: str, name: str, var_type: str, default_value: str = '') -> Dict[str, Any]"));
	Method2->SetStringField(TEXT("description"), TEXT("Add a variable to a blueprint"));
	MethodsArray.Add(MakeShared<FJsonValueObject>(Method2));
	
	Result->SetArrayField(TEXT("methods"), MethodsArray);
	Result->SetArrayField(TEXT("properties"), PropertiesArray);
	Result->SetArrayField(TEXT("inheritance"), InheritanceArray);
	
	return Result;
}

TSharedPtr<FJsonObject> FOpenClawMCPServer::HandleExecutePythonCode(const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	
	if (!PythonExecutor.IsValid())
	{
		Result->SetBoolField(TEXT("success"), false);
		Result->SetStringField(TEXT("error"), TEXT("Python executor not available"));
		return Result;
	}
	
	FString Code = Params->GetStringField(TEXT("code"));
	
	// Execute Python code
	FString Output = PythonExecutor->Execute(Code);
	
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("stdout"), Output);
	Result->SetStringField(TEXT("stderr"), TEXT(""));
	
	return Result;
}

TSharedPtr<FJsonObject> FOpenClawMCPServer::HandleListPythonSubsystems(const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	
	// Stub response with common UE subsystems
	TArray<TSharedPtr<FJsonValue>> SubsystemsArray;
	
	TArray<FString> SubsystemNames = {
		T