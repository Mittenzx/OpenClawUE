// Copyright (c) 2026 OpenClawUE Contributors. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OpenClawPythonExecutor.generated.h"

/**
 * Interface for Python execution in Unreal Engine context.
 * Provides methods to execute Python code and interact with UE Python API.
 */
class OPENCLAWPYTHON_API IOpenClawPythonExecutor
{
public:
	virtual ~IOpenClawPythonExecutor() = default;
	
	/**
	 * Execute Python code in Unreal Engine context.
	 * @param PythonCode The Python code to execute
	 * @return Output from execution
	 */
	virtual FString Execute(const FString& PythonCode) = 0;
	
	/**
	 * Check if Python is available.
	 * @return True if Python is available
	 */
	virtual bool IsPythonAvailable() const = 0;
	
	/**
	 * Get Python version.
	 * @return Python version string
	 */
	virtual FString GetPythonVersion() const = 0;
	
	/**
	 * Discover Python module contents.
	 * @param ModuleName Name of the module to discover
	 * @param NameFilter Filter by name substring
	 * @param IncludeClasses Include classes in results
	 * @param IncludeFunctions Include functions in results
	 * @param MaxItems Maximum items to return
	 * @return JSON string with module information
	 */
	virtual FString DiscoverModule(
		const FString& ModuleName,
		const FString& NameFilter = TEXT(""),
		bool IncludeClasses = true,
		bool IncludeFunctions = true,
		int32 MaxItems = 100
	) = 0;
	
	/**
	 * Discover Python class methods and properties.
	 * @param ClassName Fully qualified class name
	 * @param MethodFilter Filter methods by name substring
	 * @param IncludeInherited Include inherited methods
	 * @param IncludePrivate Include private methods
	 * @param MaxMethods Maximum methods to return
	 * @return JSON string with class information
	 */
	virtual FString DiscoverClass(
		const FString& ClassName,
		const FString& MethodFilter = TEXT(""),
		bool IncludeInherited = false,
		bool IncludePrivate = false,
		int32 MaxMethods = 0
	) = 0;
	
	/**
	 * List available Unreal Engine editor subsystems.
	 * @return JSON string with subsystem list
	 */
	virtual FString ListSubsystems() = 0;
};

/**
 * Concrete implementation of Python executor for Unreal Engine.
 */
class OPENCLAWPYTHON_API FOpenClawPythonExecutor : public IOpenClawPythonExecutor
{
public:
	FOpenClawPythonExecutor();
	virtual ~FOpenClawPythonExecutor();
	
	// IOpenClawPythonExecutor interface
	virtual FString Execute(const FString& PythonCode) override;
	virtual bool IsPythonAvailable() const override;
	virtual FString GetPythonVersion() const override;
	virtual FString DiscoverModule(
		const FString& ModuleName,
		const FString& NameFilter = TEXT(""),
		bool IncludeClasses = true,
		bool IncludeFunctions = true,
		int32 MaxItems = 100
	) override;
	virtual FString DiscoverClass(
		const FString& ClassName,
		const FString& MethodFilter = TEXT(""),
		bool IncludeInherited = false,
		bool IncludePrivate = false,
		int32 MaxMethods = 0
	) override;
	virtual FString ListSubsystems() override;
	
private:
	/** Check if Unreal Engine Python API is available */
	bool CheckPythonAvailability() const;
	
	/** Execute Python code and capture output */
	FString ExecutePythonInternal(const FString& PythonCode);
	
	/** Python availability flag */
	bool bPythonAvailable;
	
	/** Python version */
	FString PythonVersion;
};