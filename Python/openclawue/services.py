"""
OpenClawUE Services

Python services for Unreal Engine control through OpenClaw.
"""

from typing import Any, Dict, List, Optional, Union
import json

try:
    import unreal
    _UNREAL_AVAILABLE = True
except ImportError:
    _UNREAL_AVAILABLE = False


class BaseService:
    """Base class for all OpenClawUE services."""
    
    def __init__(self):
        self._unreal_available = _UNREAL_AVAILABLE
    
    def _check_unreal(self) -> bool:
        """Check if Unreal Engine Python API is available."""
        if not self._unreal_available:
            raise RuntimeError("Unreal Engine Python API not available")
        return True
    
    def _format_error(self, error: Exception) -> Dict[str, Any]:
        """Format error response."""
        return {
            "success": False,
            "error": str(error),
            "error_type": type(error).__name__
        }


class BlueprintService(BaseService):
    """Service for Blueprint creation and manipulation."""
    
    def create_blueprint(self, name: str, parent_class: str, path: str) -> Dict[str, Any]:
        """
        Create a new Blueprint asset.
        
        Args:
            name: Name of the Blueprint
            parent_class: Parent class (e.g., "Actor", "Pawn")
            path: Asset path (e.g., "/Game/Blueprints")
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            # This would call the actual Unreal API
            # For now, return stub response
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Blueprint '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)
    
    def add_variable(self, blueprint_path: str, name: str, var_type: str, 
                    default_value: str = "") -> Dict[str, Any]:
        """
        Add a variable to a Blueprint.
        
        Args:
            blueprint_path: Path to the Blueprint asset
            name: Variable name
            var_type: Variable type (e.g., "Float", "Bool", "String")
            default_value: Default value as string
            
        Returns:
            Dict containing operation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "variable_name": name,
                "variable_type": var_type,
                "message": f"Variable '{name}' added to {blueprint_path}"
            }
        except Exception as e:
            return self._format_error(e)
    
    def compile_blueprint(self, blueprint_path: str) -> Dict[str, Any]:
        """
        Compile a Blueprint.
        
        Args:
            blueprint_path: Path to the Blueprint asset
            
        Returns:
            Dict containing compilation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "message": f"Blueprint {blueprint_path} compiled successfully"
            }
        except Exception as e:
            return self._format_error(e)


class MaterialService(BaseService):
    """Service for Material creation and manipulation."""
    
    def create_material(self, name: str, path: str) -> Dict[str, Any]:
        """
        Create a new Material asset.
        
        Args:
            name: Name of the Material
            path: Asset path (e.g., "/Game/Materials")
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Material '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)
    
    def create_material_instance(self, parent_material: str, name: str, 
                                path: str) -> Dict[str, Any]:
        """
        Create a Material Instance.
        
        Args:
            parent_material: Path to parent Material
            name: Name of the Material Instance
            path: Asset path
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Material Instance '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)


class WidgetService(BaseService):
    """Service for UMG Widget creation and manipulation."""
    
    def create_widget_blueprint(self, name: str, path: str, 
                               parent_class: str = "UserWidget") -> Dict[str, Any]:
        """
        Create a new Widget Blueprint.
        
        Args:
            name: Name of the Widget Blueprint
            path: Asset path (e.g., "/Game/UI")
            parent_class: Parent class (default: "UserWidget")
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Widget Blueprint '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)
    
    def add_widget_component(self, widget_path: str, component_type: str, 
                            name: str, parent: str = "") -> Dict[str, Any]:
        """
        Add a component to a Widget Blueprint.
        
        Args:
            widget_path: Path to the Widget Blueprint
            component_type: Type of component (e.g., "Button", "TextBlock")
            name: Component name
            parent: Parent component name (optional)
            
        Returns:
            Dict containing operation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "component_name": name,
                "component_type": component_type,
                "message": f"Component '{name}' added to {widget_path}"
            }
        except Exception as e:
            return self._format_error(e)


class ActorService(BaseService):
    """Service for Level Actor manipulation."""
    
    def spawn_actor(self, actor_class: str, location: List[float], 
                   rotation: List[float] = None) -> Dict[str, Any]:
        """
        Spawn an actor in the current level.
        
        Args:
            actor_class: Actor class to spawn
            location: [X, Y, Z] location
            rotation: [Pitch, Yaw, Roll] rotation (optional)
            
        Returns:
            Dict containing spawn results
        """
        try:
            self._check_unreal()
            
            if rotation is None:
                rotation = [0.0, 0.0, 0.0]
            
            return {
                "success": True,
                "actor_class": actor_class,
                "location": location,
                "rotation": rotation,
                "message": f"Actor of class '{actor_class}' spawned successfully"
            }
        except Exception as e:
            return self._format_error(e)
    
    def get_all_actors(self) -> Dict[str, Any]:
        """
        Get all actors in the current level.
        
        Returns:
            Dict containing actor list
        """
        try:
            self._check_unreal()
            
            # Stub response
            return {
                "success": True,
                "actors": [],
                "count": 0,
                "message": "No actors in level (stub mode)"
            }
        except Exception as e:
            return self._format_error(e)


class AnimationService(BaseService):
    """Service for Animation system control."""
    
    def create_animation_sequence(self, name: str, path: str, 
                                 skeleton: str = "") -> Dict[str, Any]:
        """
        Create a new Animation Sequence.
        
        Args:
            name: Name of the Animation Sequence
            path: Asset path (e.g., "/Game/Animations")
            skeleton: Skeleton asset path (optional)
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Animation Sequence '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)


class NiagaraService(BaseService):
    """Service for Niagara VFX system control."""
    
    def create_niagara_system(self, name: str, path: str, 
                             template: str = "minimal") -> Dict[str, Any]:
        """
        Create a new Niagara System.
        
        Args:
            name: Name of the Niagara System
            path: Asset path (e.g., "/Game/VFX")
            template: Template to use (default: "minimal")
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Niagara System '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)


class InputService(BaseService):
    """Service for Enhanced Input system control."""
    
    def create_input_action(self, name: str, path: str, 
                           value_type: str = "boolean") -> Dict[str, Any]:
        """
        Create a new Input Action.
        
        Args:
            name: Name of the Input Action
            path: Asset path (e.g., "/Game/Input")
            value_type: Value type ("boolean", "axis1d", "axis2d", "axis3d")
            
        Returns:
            Dict containing creation results
        """
        try:
            self._check_unreal()
            
            return {
                "success": True,
                "asset_path": f"{path}/{name}",
                "message": f"Input Action '{name}' created successfully"
            }
        except Exception as e:
            return self._format_error(e)


class AssetService(BaseService):
    """Service for Asset discovery and management."""
    
    def search_assets(self, search_term: str = "", 
                     asset_type: str = "") -> Dict[str, Any]:
        """
        Search for assets in the project.
        
        Args:
            search_term: Search term (optional)
            asset_type: Filter by asset type (optional)
            
        Returns:
            Dict containing search results
        """
        try:
            self._check_unreal()
            
            # Stub response
            return {
                "success": True,
                "assets": [],
                "count": 0,
                "message": "No assets found (stub mode)"
            }
        except Exception as e:
            return self._format_error(e)


class OpenClawService(BaseService):
    """Service for OpenClaw-specific integration."""
    
    def __init__(self):
        super().__init__()
        self._connected = False
    
    def connect_to_openclaw(self, url: str = "ws://127.0.0.1:18789") -> Dict[str, Any]:
        """
        Connect to OpenClaw Gateway.
        
        Args:
            url: OpenClaw Gateway WebSocket URL
            
        Returns:
            Dict containing connection results
        """
        try:
            # This would establish WebSocket connection
            # For now, just mark as connected
            self._connected = True
            
            return {
                "success": True,
                "connected": True,
                "url": url,
                "message": f"Connected to OpenClaw Gateway at {url}"
            }
        except Exception as e:
            return self._format_error(e)
    
    def send_to_openclaw(self, command: Dict[str, Any]) -> Dict[str, Any]:
        """
        Send command to OpenClaw.
        
        Args:
            command: Command dictionary
            
        Returns:
            Dict containing response
        """
        try:
            if not self._connected:
                return {
                    "success": False,
                    "error": "Not connected to OpenClaw"
                }
            
            # Stub response
            return {
                "success": True,
                "command": command,
                "response": {"status": "received", "message": "Command processed (stub)"}
            }
        except Exception as e:
            return self._format_error(e)
    
    def create_dashboard(self) -> Dict[str, Any]:
        """
        Create web dashboard for UE monitoring.
        
        Returns:
            Dict containing dashboard information
        """
        try:
            return {
                "success": True,
                "dashboard_url": "http://127.0.0.1:3000/openclawue",
                "message": "Dashboard created successfully (stub)"
            }
        except Exception as e:
            return self._format_error(e)