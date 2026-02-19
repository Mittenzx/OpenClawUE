/**
 * OpenClaw Unreal Engine Control Skill
 * 
 * Control Unreal Engine 5.7+ through OpenClawUE plugin.
 * Provides natural language interface for UE5 automation.
 */

const axios = require('axios');
const WebSocket = require('ws');

class UnrealControlSkill {
  constructor(config = {}) {
    this.config = {
      mcpServerUrl: 'http://127.0.0.1:8089',
      mcpWebSocketUrl: 'ws://127.0.0.1:8089',
      openclawGatewayUrl: 'ws://127.0.0.1:18789',
      apiKey: '',
      timeout: 30000,
      ...config
    };
    
    this.mcpClient = null;
    this.openclawConnection = null;
    this.isConnected = false;
    this.requestId = 1;
  }
  
  /**
   * Initialize the skill
   */
  async initialize() {
    console.log('Initializing Unreal Engine Control Skill...');
    
    // Test MCP server connection
    try {
      const health = await this.checkHealth();
      if (health.status === 'healthy') {
        console.log(`✅ Connected to MCP server on port ${health.port}`);
        this.isConnected = true;
      } else {
        console.log('❌ MCP server not healthy');
        return false;
      }
    } catch (error) {
      console.log(`❌ Failed to connect to MCP server: ${error.message}`);
      return false;
    }
    
    // Connect to OpenClaw Gateway
    try {
      await this.connectToOpenClaw();
      console.log('✅ Connected to OpenClaw Gateway');
    } catch (error) {
      console.log(`⚠️ Could not connect to OpenClaw Gateway: ${error.message}`);
      console.log('Skill will work in standalone mode');
    }
    
    console.log('✅ Unreal Engine Control Skill initialized');
    return true;
  }
  
  /**
   * Check MCP server health
   */
  async checkHealth() {
    try {
      const response = await axios.get(`${this.config.mcpServerUrl}/health`, {
        timeout: this.config.timeout
      });
      return response.data;
    } catch (error) {
      throw new Error(`Health check failed: ${error.message}`);
    }
  }
  
  /**
   * Connect to OpenClaw Gateway
   */
  async connectToOpenClaw() {
    return new Promise((resolve, reject) => {
      this.openclawConnection = new WebSocket(this.config.openclawGatewayUrl);
      
      this.openclawConnection.on('open', () => {
        console.log('OpenClaw WebSocket connection established');
        resolve();
      });
      
      this.openclawConnection.on('error', (error) => {
        reject(new Error(`WebSocket error: ${error.message}`));
      });
      
      this.openclawConnection.on('close', () => {
        console.log('OpenClaw WebSocket connection closed');
        this.openclawConnection = null;
      });
      
      // Handle incoming messages
      this.openclawConnection.on('message', (data) => {
        this.handleOpenClawMessage(data);
      });
    });
  }
  
  /**
   * Handle OpenClaw Gateway messages
   */
  handleOpenClawMessage(data) {
    try {
      const message = JSON.parse(data);
      console.log('Received OpenClaw message:', message);
      
      // Handle different message types
      switch (message.type) {
        case 'command':
          this.handleCommand(message);
          break;
        case 'status':
          this.handleStatus(message);
          break;
        case 'error':
          console.error('OpenClaw error:', message);
          break;
      }
    } catch (error) {
      console.error('Error parsing OpenClaw message:', error);
    }
  }
  
  /**
   * Handle commands from OpenClaw
   */
  async handleCommand(command) {
    try {
      let result;
      
      switch (command.action) {
        case 'create_blueprint':
          result = await this.createBlueprint(command.params);
          break;
        case 'create_material':
          result = await this.createMaterial(command.params);
          break;
        case 'spawn_actor':
          result = await this.spawnActor(command.params);
          break;
        case 'execute_python':
          result = await this.executePython(command.params);
          break;
        case 'discover':
          result = await this.discoverModule(command.params);
          break;
        default:
          result = {
            success: false,
            error: `Unknown command: ${command.action}`
          };
      }
      
      // Send response back to OpenClaw
      if (this.openclawConnection) {
        this.openclawConnection.send(JSON.stringify({
          type: 'response',
          commandId: command.id,
          result: result
        }));
      }
      
      return result;
    } catch (error) {
      console.error('Error handling command:', error);
      
      if (this.openclawConnection) {
        this.openclawConnection.send(JSON.stringify({
          type: 'error',
          commandId: command.id,
          error: error.message
        }));
      }
      
      return {
        success: false,
        error: error.message
      };
    }
  }
  
  /**
   * Handle status updates
   */
  handleStatus(status) {
    console.log('OpenClaw status:', status);
    // Could update dashboard or log status changes
  }
  
  /**
   * Send MCP request
   */
  async sendMCPRequest(method, params = {}) {
    const requestId = this.requestId++;
    
    const request = {
      jsonrpc: '2.0',
      method: method,
      params: params,
      id: requestId
    };
    
    try {
      const response = await axios.post(`${this.config.mcpServerUrl}/mcp`, request, {
        headers: {
          'Content-Type': 'application/json'
        },
        timeout: this.config.timeout
      });
      
      return response.data.result;
    } catch (error) {
      throw new Error(`MCP request failed: ${error.message}`);
    }
  }
  
  /**
   * Execute Python code in UE context
   */
  async executePython(params) {
    const { code } = params;
    
    const result = await this.sendMCPRequest('execute_python_code', {
      code: code
    });
    
    return {
      success: result.success || false,
      stdout: result.stdout || '',
      stderr: result.stderr || '',
      ...result
    };
  }
  
  /**
   * Discover Python module
   */
  async discoverModule(params) {
    const { module_name, name_filter = '', include_classes = true, include_functions = true } = params;
    
    const result = await this.sendMCPRequest('discover_python_module', {
      module_name: module_name,
      name_filter: name_filter,
      include_classes: include_classes,
      include_functions: include_functions
    });
    
    return result;
  }
  
  /**
   * Discover Python class
   */
  async discoverClass(params) {
    const { class_name, method_filter = '', include_inherited = false, include_private = false } = params;
    
    const result = await this.sendMCPRequest('discover_python_class', {
      class_name: class_name,
      method_filter: method_filter,
      include_inherited: include_inherited,
      include_private: include_private
    });
    
    return result;
  }
  
  /**
   * Create a blueprint
   */
  async createBlueprint(params) {
    const { name, parent_class = 'Actor', path = '/Game/Blueprints' } = params;
    
    const pythonCode = `
import unreal

try:
    # Create blueprint
    result = unreal.BlueprintService.create_blueprint(
        "${name}",
        "${parent_class}",
        "${path}"
    )
    
    # Save the asset
    unreal.EditorAssetLibrary.save_asset(result)
    
    print(f"Blueprint created: {result}")
    return {"success": True, "asset_path": result}
    
except Exception as e:
    print(f"Error creating blueprint: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        assetPath: result.asset_path || `${path}/${name}`,
        message: `Blueprint '${name}' created successfully`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error creating blueprint'
      };
    }
  }
  
  /**
   * Add variable to blueprint
   */
  async addBlueprintVariable(params) {
    const { blueprint_path, name, type, default_value = '' } = params;
    
    const pythonCode = `
import unreal

try:
    # Add variable to blueprint
    unreal.BlueprintService.add_variable(
        "${blueprint_path}",
        "${name}",
        "${type}",
        "${default_value}"
    )
    
    # Compile blueprint (required before adding variable nodes)
    unreal.BlueprintService.compile_blueprint("${blueprint_path}")
    
    print(f"Variable '${name}' added to ${blueprint_path}")
    return {"success": True}
    
except Exception as e:
    print(f"Error adding variable: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        variableName: name,
        variableType: type,
        message: `Variable '${name}' added to blueprint`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error adding variable'
      };
    }
  }
  
  /**
   * Compile blueprint
   */
  async compileBlueprint(blueprintPath) {
    const pythonCode = `
import unreal

try:
    unreal.BlueprintService.compile_blueprint("${blueprintPath}")
    print(f"Blueprint compiled: ${blueprintPath}")
    return {"success": True}
    
except Exception as e:
    print(f"Error compiling blueprint: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        message: `Blueprint '${blueprintPath}' compiled successfully`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error compiling blueprint'
      };
    }
  }
  
  /**
   * Create material
   */
  async createMaterial(params) {
    const { name, path = '/Game/Materials' } = params;
    
    const pythonCode = `
import unreal

try:
    # Create material
    result = unreal.MaterialService.create_material(
        "${name}",
        "${path}"
    )
    
    # Save the asset
    unreal.EditorAssetLibrary.save_asset(result)
    
    print(f"Material created: {result}")
    return {"success": True, "asset_path": result}
    
except Exception as e:
    print(f"Error creating material: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        assetPath: result.asset_path || `${path}/${name}`,
        message: `Material '${name}' created successfully`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error creating material'
      };
    }
  }
  
  /**
   * Spawn actor in level
   */
  async spawnActor(params) {
    const { actor_class, location = [0, 0, 0], rotation = [0, 0, 0] } = params;
    
    const [x, y, z] = location;
    const [pitch, yaw, roll] = rotation;
    
    const pythonCode = `
import unreal

try:
    # Get level editor subsystem
    level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    
    # Spawn actor
    transform = unreal.Transform(
        location=unreal.Vector(${x}, ${y}, ${z}),
        rotation=unreal.Rotator(${pitch}, ${yaw}, ${roll}),
        scale=unreal.Vector(1.0, 1.0, 1.0)
    )
    
    actor = level_subsystem.spawn_actor_from_class(
        unreal.load_class(None, "${actor_class}"),
        transform
    )
    
    if actor:
        actor_path = actor.get_path_name()
        print(f"Actor spawned: {actor_path}")
        return {"success": True, "actor_path": actor_path}
    else:
        return {"success": False, "error": "Failed to spawn actor"}
    
except Exception as e:
    print(f"Error spawning actor: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        actorPath: result.actor_path,
        location: location,
        rotation: rotation,
        message: `Actor of class '${actor_class}' spawned successfully`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error spawning actor'
      };
    }
  }
  
  /**
   * Get all actors in level
   */
  async getAllActors() {
    const pythonCode = `
import unreal

try:
    # Get all actors in current level
    level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    actors = level_subsystem.get_all_level_actors()
    
    actor_list = []
    for actor in actors:
        actor_info = {
            "name": actor.get_name(),
            "class": actor.get_class().get_name(),
            "path": actor.get_path_name(),
            "location": {
                "x": actor.get_actor_location().x,
                "y": actor.get_actor_location().y,
                "z": actor.get_actor_location().z
            }
        }
        actor_list.append(actor_info)
    
    print(f"Found {len(actor_list)} actors")
    return {"success": True, "actors": actor_list, "count": len(actor_list)}
    
except Exception as e:
    print(f"Error getting actors: {e}")
    return {"success": False, "error": str(e)}
`;
    
    const result = await this.executePython({ code: pythonCode });
    
    if (result.success) {
      return {
        success: true,
        actors: result.actors || [],
        count: result.count || 0,
        message: `Found ${result.count || 0} actors in level`
      };
    } else {
      return {
        success: false,
        error: result.stderr || 'Unknown error getting actors'
      };
    }
  }
  
  /**
   * Send notification to OpenClaw
   */
  async sendNotification(message, type = 'info') {
    if (!this.openclawConnection) {
      return false;
    }
    
    try {
      this.openclawConnection.send(JSON.stringify({
        type: 'notification',
        notification: {
          message: message,
          type: type,
          timestamp: new Date().toISOString()
        }
      }));
      return true;
    } catch (error) {
      console.error('Error sending notification:', error);
      return false;
    }
  }
  
  /**
   * Clean up resources
   */
  async cleanup() {
    console.log('Cleaning up Unreal Engine Control Skill...');
    
    // Close OpenClaw connection
    if (this.openclawConnection) {
      this.openclawConnection.close();
      this.openclawConnection = null;
    }
    
    this.isConnected = false;
    console.log('✅ Unreal Engine Control Skill cleaned up');
  }
}

// Export the skill
module.exports = UnrealControlSkill;

// If running as standalone script
if (require.main === module) {
  (async () => {
    const skill = new UnrealControlSkill();
    
    try {
      const initialized = await skill.initialize();
      if (!initialized) {
        console.error('Failed to initialize skill');
        process.exit(1);
      }
      
      console.log('Unreal Engine Control Skill running...');
      console.log('Press Ctrl+C to exit');
      
      // Keep process alive
      process.on('SIGINT', async () => {
        console.log('\nShutting down...');
        await skill.cleanup();
        process.exit(0);
      });
      
    } catch (error) {
      console.error('Error:', error);
      process.exit(1);
    }
  })();
}