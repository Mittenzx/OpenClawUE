#!/usr/bin/env python3
"""
Basic OpenClawUE Usage Example

This script demonstrates basic usage of the OpenClawUE Python API.
"""

import sys
import os

# Add the Python module to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'Python'))

try:
    import openclawue
    print(f"OpenClawUE version: {openclawue.__version__}")
except ImportError as e:
    print(f"Error importing OpenClawUE: {e}")
    print("Make sure the plugin is installed and Python path is set correctly.")
    sys.exit(1)

def test_basic_functionality():
    """Test basic OpenClawUE functionality."""
    print("\n=== Testing Basic Functionality ===")
    
    # Check if Unreal Engine is available
    if openclawue.is_unreal_available():
        print("✓ Unreal Engine Python API is available")
    else:
        print("✗ Unreal Engine Python API not available (running in stub mode)")
    
    # Test service instantiation
    try:
        blueprint_service = openclawue.BlueprintService
        print("✓ BlueprintService initialized")
    except Exception as e:
        print(f"✗ Failed to initialize BlueprintService: {e}")
    
    # Test Python execution
    print("\n=== Testing Python Execution ===")
    result = openclawue.execute_python("print('Hello from OpenClawUE!')")
    if result.get("success"):
        print(f"✓ Python execution successful")
        if result.get("stdout"):
            print(f"  Output: {result['stdout']}")
    else:
        print(f"✗ Python execution failed: {result.get('stderr', 'Unknown error')}")
    
    # List subsystems
    print("\n=== Testing Subsystem Discovery ===")
    subsystems = openclawue.list_subsystems()
    if subsystems:
        print(f"✓ Found {len(subsystems)} subsystems:")
        for subsystem in subsystems:
            print(f"  - {subsystem}")
    else:
        print("✗ No subsystems found (may be in stub mode)")

def test_blueprint_creation():
    """Test Blueprint creation functionality."""
    print("\n=== Testing Blueprint Creation ===")
    
    try:
        service = openclawue.BlueprintService
        
        # Create a blueprint
        result = service.create_blueprint(
            name="BP_TestActor",
            parent_class="Actor",
            path="/Game/Blueprints"
        )
        
        if result.get("success"):
            print(f"✓ Blueprint creation successful")
            print(f"  Asset path: {result.get('asset_path')}")
            print(f"  Message: {result.get('message')}")
        else:
            print(f"✗ Blueprint creation failed: {result.get('error', 'Unknown error')}")
        
        # Add a variable
        if result.get("success"):
            var_result = service.add_variable(
                blueprint_path=result["asset_path"],
                name="Health",
                var_type="Float",
                default_value="100.0"
            )
            
            if var_result.get("success"):
                print(f"✓ Variable added successfully")
                print(f"  Variable: {var_result.get('variable_name')}")
                print(f"  Type: {var_result.get('variable_type')}")
            else:
                print(f"✗ Failed to add variable: {var_result.get('error', 'Unknown error')}")
        
        # Compile blueprint
        if result.get("success"):
            compile_result = service.compile_blueprint(result["asset_path"])
            if compile_result.get("success"):
                print(f"✓ Blueprint compiled successfully")
            else:
                print(f"✗ Failed to compile blueprint: {compile_result.get('error', 'Unknown error')}")
                
    except Exception as e:
        print(f"✗ Exception during blueprint test: {e}")

def test_openclaw_integration():
    """Test OpenClaw integration."""
    print("\n=== Testing OpenClaw Integration ===")
    
    try:
        service = openclawue.OpenClawService
        
        # Connect to OpenClaw
        connect_result = service.connect_to_openclaw()
        if connect_result.get("success"):
            print(f"✓ Connected to OpenClaw Gateway")
            print(f"  URL: {connect_result.get('url')}")
        else:
            print(f"✗ Failed to connect to OpenClaw: {connect_result.get('error', 'Unknown error')}")
        
        # Send a test command
        if connect_result.get("success"):
            command_result = service.send_to_openclaw({
                "command": "test",
                "data": {"message": "Hello from OpenClawUE"}
            })
            
            if command_result.get("success"):
                print(f"✓ Command sent successfully")
                print(f"  Response: {command_result.get('response', {})}")
            else:
                print(f"✗ Failed to send command: {command_result.get('error', 'Unknown error')}")
        
        # Create dashboard
        dashboard_result = service.create_dashboard()
        if dashboard_result.get("success"):
            print(f"✓ Dashboard created")
            print(f"  URL: {dashboard_result.get('dashboard_url')}")
        else:
            print(f"✗ Failed to create dashboard: {dashboard_result.get('error', 'Unknown error')}")
            
    except Exception as e:
        print(f"✗ Exception during OpenClaw test: {e}")

def main():
    """Main function."""
    print("OpenClawUE Basic Usage Example")
    print("=" * 50)
    
    test_basic_functionality()
    test_blueprint_creation()
    test_openclaw_integration()
    
    print("\n" + "=" * 50)
    print("Example completed!")
    print("\nNext steps:")
    print("1. Install the OpenClawUE plugin in your Unreal Engine project")
    print("2. Enable the plugin and restart the editor")
    print("3. Run this script with Unreal Engine Python environment")
    print("4. Explore the full API with discover_module() and discover_class()")

if __name__ == "__main__":
    main()