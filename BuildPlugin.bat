@echo off
echo Building OpenClawUE Plugin...
echo.

REM Check if we're in the right directory
if not exist "OpenClawUE.uplugin" (
    echo Error: Not in OpenClawUE plugin directory!
    echo Please run this script from the OpenClawUE plugin folder.
    pause
    exit /b 1
)

REM Find Unreal Engine installation
set UE_PATH=
if exist "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.7
) else if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.6
) else if exist "C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.5
) else if exist "C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.4
) else if exist "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.3
)

if "%UE_PATH%"=="" (
    echo Error: Could not find Unreal Engine installation!
    echo Please install Unreal Engine 5.3 or later.
    pause
    exit /b 1
)

echo Found Unreal Engine at: %UE_PATH%
echo.

REM Generate project files
echo Generating project files...
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%cd%\..\.." -game -rocket -progress
if errorlevel 1 (
    echo Error generating project files!
    pause
    exit /b 1
)

echo.
echo Project files generated successfully.
echo.
echo Next steps:
echo 1. Open your Unreal Engine project
echo 2. Go to Edit -> Plugins
echo 3. Find "OpenClawUE" and enable it
echo 4. Restart the editor
echo.
echo For manual building:
echo 1. Open Visual Studio solution
echo 2. Build the OpenClawUE modules
echo 3. Copy built plugin to your project's Plugins folder
echo.

pause