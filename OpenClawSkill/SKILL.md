# Unreal Engine Control Skill

Control Unreal Engine 5.7+ through OpenClawUE plugin. Create blueprints, materials, widgets, and manage level actors via natural language commands.

## 🎯 Features

- **Blueprint Creation**: Create, modify, and compile blueprints
- **Material System**: Build material graphs and instances
- **UMG Widgets**: Create and modify user interface elements
- **Level Management**: Spawn, modify, and control level actors
- **Animation Control**: Manage animations and montages
- **Niagara VFX**: Create and modify visual effects
- **Real-time Monitoring**: Monitor UE performance and status

## 🔧 Prerequisites

1. **Unreal Engine 5.7+** with OpenClawUE plugin installed
2. **OpenClawUE Plugin** enabled in your UE project
3. **MCP Server** running on port 8089 (default)

## 🚀 Installation

### 1. Install OpenClawUE Plugin
```bash
# Clone the plugin
cd YourProject/Plugins
git clone https://github.com/Mittenzx/OpenClawUE.git

# Build the plugin
cd OpenClawUE
BuildPlugin.bat  # Windows
# or ./BuildPlugin.sh  # Linux/Mac

# Enable in Unreal Editor:
# 1. Edit → Plugins
# 2. Search for "OpenClawUE"
# 3. Enable and restart editor
```

### 2. Configure MCP Server
In Unreal Editor:
1. **Project Settings → Plugins → OpenClawUE**
2. Enable MCP Server (port 8089)
3. Configure authentication if needed

### 3. Test Connection
```bash
# Check if MCP server is running
curl http://127.0.0.1:8089/health
```

## 📖 Usage

### Basic Commands

```javascript
// Import the skill
const unreal = require('./unreal-control');

// Create a blueprint
const result = await unreal.createBlueprint({
  name: "BP_Player",
  parentClass: "Actor",
  path: "/Game/Blueprints"
});

// Add variables to blueprint
await unreal.addBlueprintVariable({
  blueprintPath: result.assetPath,
  name: "Health",
  type: "Float",
  defaultValue: "100.0"
});

// Compile blueprint
await unreal.compileBlueprint(result.assetPath);
```

### Material Creation
```javascript
// Create material
const material = await unreal.createMaterial({
  name: "M_Glowing",
  path: "/Game/Materials"
});

// Create material instance
const instance = await unreal.createMaterialInstance({
  parentMaterial: material.assetPath,
  name: "MI_Glowing_Red",
  path: "/Game/Materials/Instances"
});
```

### Level Actor Management
```javascript
// Spawn actor in level
const actor = await unreal.spawnActor({
  actorClass: "StaticMeshActor",
  location: [0, 0, 100],
  rotation: [0, 0, 0]
});

// Get all actors in level
const actors = await unreal.getAllActors();

// Modify actor properties
await unreal.setActorProperty({
  actorPath: actor.path,
  property: "bHidden",
  value: "false"
});
```

## 🔌 MCP Integration

The skill communicates with OpenClawUE MCP server using JSON-RPC:

### Available MCP Tools:
- `discover_python_module` - Explore UE Python modules
- `discover_python_class` - Get class methods and properties
- `execute_python_code` - Run Python in UE context
- `list_python_subsystems` - List UE editor subsystems

### Example MCP Request:
```json
{
  "jsonrpc": "2.0",
  "method": "execute_python_code",
  "params": {
    "code": "import unreal\nunreal.BlueprintService.create_blueprint('BP_Test', 'Actor', '/Game/Blueprints')"
  },
  "id": 1
}
```

## 🎮 Service Reference

### BlueprintService
```javascript
// Create blueprint
unreal.createBlueprint(options)

// Add variable
unreal.addBlueprintVariable(options)

// Compile blueprint
unreal.compileBlueprint(blueprintPath)

// Add function
unreal.addBlueprintFunction(options)

// Add component
unreal.addBlueprintComponent(options)
```

### MaterialService
```javascript
// Create material
unreal.createMaterial(options)

// Create material instance
unreal.createMaterialInstance(options)

// Set material parameter
unreal.setMaterialParameter(options)

// Compile material
unreal.compileMaterial(materialPath)
```

### ActorService
```javascript
// Spawn actor
unreal.spawnActor(options)

// Get all actors
unreal.getAllActors()

// Set actor transform
unreal.setActorTransform(options)

// Destroy actor
unreal.destroyActor(actorPath)
```

### WidgetService
```javascript
// Create widget blueprint
unreal.createWidgetBlueprint(options)

// Add widget component
unreal.addWidgetComponent(options)

// Set widget property
unreal.setWidgetProperty(options)
```

## 🖥️ Dashboard

Access the web dashboard at: `http://127.0.0.1:3000/unreal`

Features:
- Real-time UE performance monitoring
- Blueprint and asset browser
- Level editor with live preview
- Material graph visualizer
- Animation timeline viewer

## 🔒 Security

- **Local network only**: MCP server runs on localhost by default
- **Authentication**: Optional API key authentication
- **Rate limiting**: Prevents excessive requests
- **Command validation**: All commands are validated before execution

## 🐛 Troubleshooting

### Common Issues:

1. **MCP server not responding**
   ```bash
   # Check if server is running
   curl http://127.0.0.1:8089/health
   
   # Check UE editor logs
   # Look for "OpenClawUE" in Output Log
   ```

2. **Python API not available**
   - Ensure PythonScriptPlugin is enabled
   - Check UE Python installation
   - Verify Python path in project settings

3. **Permission errors**
   - Check file permissions for asset paths
   - Verify UE project is writable
   - Check antivirus/firewall settings

### Logs:
- **UE Editor Log**: `Saved/Logs/YourProject.log`
- **MCP Server Log**: Console output in UE editor
- **OpenClaw Skill Log**: `openclaw-skills/unreal-control/logs/`

## 📚 Examples

See `examples/` directory for complete examples:
- `basic-blueprint.js` - Basic blueprint creation
- `material-system.js` - Material creation and editing
- `level-editor.js` - Level actor management
- `animation-system.js` - Animation control
- `dashboard-integration.js` - Web dashboard integration

## 🔄 Updates

Check for updates:
```bash
cd openclaw-skills/unreal-control
git pull origin main
```

## 🤝 Contributing

1. Fork the repository
2. Create feature branch
3. Make changes
4. Submit pull request

See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## 📄 License

MIT License - see [LICENSE](LICENSE) file.

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/Mittenzx/OpenClawUE/issues)
- **Discord**: [OpenClaw Discord](https://discord.gg/clawd)
- **Documentation**: [OpenClawUE Docs](https://docs.openclaw.ai/plugins/openclawue)

---

**Made with ❤️ for the OpenClaw community**