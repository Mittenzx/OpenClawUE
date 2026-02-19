"""
OpenClawUE - Unreal Engine Python API for OpenClaw

This module provides Python access to Unreal Engine functionality
for integration with OpenClaw AI assistants.
"""

__version__ = "1.0.0"
__author__ = "OpenClawUE Contributors"
__license__ = "MIT"

import sys
import json
from typing import Any, Dict, List, Optional, Union

# Try to import Unreal Engine Python API
try:
    import unreal
    _UNREAL_AVAILABLE = True
except ImportError:
    _UNREAL_AVAILABLE = False
    print("Warning: Unreal Engine Python API not available. Running in stub mode.")

# Service imports
from .services import (
    BlueprintService,
    MaterialService,
    WidgetService,
    ActorService,
    AnimationService,
    NiagaraService,
    InputService,
    AssetService,
    OpenClawService
)

# Export services
__all__ = [
    'BlueprintService',
    'MaterialService',
    'WidgetService',
    'ActorService',
    'AnimationService',
    'NiagaraService',
    'InputService',
    'AssetService',
    'OpenClawService',
    'discover_module',
    'discover_class',
    'execute_python',
    'list_subsystems',
    'is_unreal_available'
]

# Service instances
_blueprint_service = None
_material_service = None
_widget_service = None
_actor_service = None
_animation_service = None
_niagara_service = None
_input_service = None
_asset_service = None
_openclaw_service = None

def _get_service(service_class, instance_attr):
    """Lazy-load service instance."""
    global _blueprint_service, _material_service, _widget_service
    global _actor_service, _animation_service, _niagara_service
    global _input_service, _asset_service, _openclaw_service
    
    instance = globals().get(instance_attr)
    if instance is None:
        instance = service_class()
        globals()[instance_attr] = instance
    return instance

# Service properties
BlueprintService = property(lambda self: _get_service(BlueprintService, '_blueprint_service'))
MaterialService = property(lambda self: _get_service(MaterialService, '_material_service'))
WidgetService = property(lambda self: _get_service(WidgetService, '_widget_service'))
ActorService = property(lambda self: _get_service(ActorService, '_actor_service'))
AnimationService = property(lambda self: _get_service(AnimationService, '_animation_service'))
NiagaraService = property(lambda self: _get_service(NiagaraService, '_niagara_service'))
InputService = property(lambda self: _get_service(InputService, '_input_service'))
AssetService = property(lambda self: _get_service(AssetService, '_asset_service'))
OpenClawService = property(lambda self: _get_service(OpenClawService, '_openclaw_service'))

def is_unreal_available() -> bool:
    """
    Check if Unreal Engine Python API is available.
    
    Returns:
        bool: True if Unreal Engine Python API is available
    """
    return _UNREAL_AVAILABLE

def discover_module(module_name: str, **kwargs) -> Dict[str, Any]:
    """
    Discover contents of a Python module.
    
    Args:
        module_name: Name of the module to discover
        **kwargs: Additional discovery parameters
        
    Returns:
        Dict containing module information
    """
    if not _UNREAL_AVAILABLE:
        return {"error": "Unreal Engine Python API not available"}
    
    # This would be implemented by the C++ side
    # For now, return stub response
    return {
        "module": module_name,
        "classes": [],
        "functions": [],
        "constants": []
    }

def discover_class(class_name: str, **kwargs) -> Dict[str, Any]:
    """
    Discover methods and properties of a Python class.
    
    Args:
        class_name: Fully qualified class name
        **kwargs: Additional discovery parameters
        
    Returns:
        Dict containing class information
    """
    if not _UNREAL_AVAILABLE:
        return {"error": "Unreal Engine Python API not available"}
    
    # This would be implemented by the C++ side
    # For now, return stub response
    return {
        "class": class_name,
        "methods": [],
        "properties": [],
        "inheritance": []
    }

def execute_python(code: str) -> Dict[str, Any]:
    """
    Execute Python code in Unreal Engine context.
    
    Args:
        code: Python code to execute
        
    Returns:
        Dict containing execution results
    """
    if not _UNREAL_AVAILABLE:
        return {
            "stdout": "",
            "stderr": "Unreal Engine Python API not available",
            "success": False
        }
    
    try:
        # Execute the code
        exec_globals = {'unreal': unreal}
        exec(code, exec_globals)
        
        return {
            "stdout": "Execution completed",
            "stderr": "",
            "success": True
        }
    except Exception as e:
        return {
            "stdout": "",
            "stderr": str(e),
            "success": False
        }

def list_subsystems() -> List[str]:
    """
    List available Unreal Engine editor subsystems.
    
    Returns:
        List of subsystem names
    """
    if not _UNREAL_AVAILABLE:
        return []
    
    try:
        # This is a simplified version
        # In reality, we'd query the editor for all subsystems
        subsystems = [
            "LevelEditorSubsystem",
            "EditorAssetLibrary",
            "EditorLevelLibrary",
            "EditorUtilityLibrary"
        ]
        return subsystems
    except Exception:
        return []

# Initialize services on module import
if _UNREAL_AVAILABLE:
    print(f"OpenClawUE {__version__} initialized with Unreal Engine Python API")
else:
    print(f"OpenClawUE {__version__} initialized in stub mode")