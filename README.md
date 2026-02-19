# OpenClawUE - Unreal Engine Plugin for OpenClaw

![OpenClawUE Logo](https://img.shields.io/badge/OpenClaw-UE5-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-green)
![Unreal Engine 5.7+](https://img.shields.io/badge/UE-5.7%2B-orange)

A powerful Unreal Engine plugin that enables seamless integration between Unreal Engine and OpenClaw, allowing AI assistants to control UE5 through natural language and automated workflows.

## ✨ Features

### 🤖 AI-First Architecture
- **MCP (Model Context Protocol) Server** - HTTP/WebSocket server for AI assistant integration
- **Python API Services** - 20+ specialized services with 600+ methods
- **Discovery Tools** - Explore UE modules, classes, and functions dynamically
- **Skills System** - Lazy-loaded domain knowledge for efficient AI context

### 🎮 Unreal Engine Control
- **Blueprint Creation & Editing** - Create, modify, and compile blueprints
- **Material System** - Build material graphs and instances
- **UMG Widgets** - Create and modify user interface elements
- **Animation Systems** - Control animations, montages, and state machines
- **Niagara VFX** - Create and modify visual effects systems
- **Level & Actor Management** - Spawn, modify, and control level actors

### 🔌 OpenClaw Integration
- **Native OpenClaw Protocol** - Direct integration with OpenClaw Gateway
- **Real-time Monitoring** - Monitor UE performance and status
- **Automated Workflows** - Batch operations and scheduled tasks
- **Web Dashboard** - Built-in web interface for UE control

## 🚀 Quick Start

### Prerequisites
- Unreal Engine 5.7+
- Python 3.8+ (for Python API)
- OpenClaw (for full integration)

### Installation

1. **Clone the plugin:**
   ```bash
   cd YourProject/Plugins
   git clone https://github.com/Mittenzx/OpenClawUE.git
   ```

2. **Build the plugin:**
   ```bash
   # Windows
   Plugins/OpenClawUE/BuildPlugin.bat
   
   # Linux/Mac
   Plugins/OpenClawUE/BuildPlugin.sh
   ```

3. **Enable in Unreal Editor:**
   - Open your project
   - Go to **Edit → Plugins**
   - Search for "OpenClawUE"
   - Enable the plugin
   - Restart the editor

4. **Configure OpenClaw integration:**
   - Open **Project Settings → Plugins → OpenClawUE**
   - Enable MCP Server (default port: 8089)
   - Configure OpenClaw Gateway connection

## 📖 Usage

### Basic Python API
```python
import openclawue

# Create a blueprint
bp_path = openclawue.BlueprintService.create_blueprint(
    "BP_Player", 
    "Actor", 
    "/Game/Blueprints"
)

# Add variables
openclawue.BlueprintService.add_variable(bp_path, "Health", "Float", "100.0")
openclawue.BlueprintService.compile_blueprint(bp_path)
```

### MCP Server (for AI Assistants)
```json
// MCP request to discover UE modules
{
  "jsonrpc": "2.0",
  "method": "discover_python_module",
  "params": {
    "module_name": "unreal"
  },
  "id": 1
}
```

### OpenClaw Integration
```javascript
// OpenClaw skill for UE control
const openclawUE = {
  async createMaterial(name, path) {
    return await this.executePython(`
import openclawue
openclawue.MaterialService.create_material("${name}", "${path}")
`);
  }
};
```

## 🏗️ Architecture

```
OpenClawUE/
├── Source/                    # C++ plugin source
│   ├── OpenClawMCP/          # MCP server implementation
│   ├── OpenClawServices/     # UE service wrappers
│   └── OpenClawPython/       # Python integration
├── Python/                   # Python API
│   └── openclawue/          # Python package
├── Config/                   # Configuration files
├── Content/                  # UE assets and instructions
└── Examples/                 # Example projects and scripts
```

## 🔧 Available Services

| Service | Methods | Description |
|---------|---------|-------------|
| `BlueprintService` | 75+ | Blueprint creation, variables, functions, nodes |
| `MaterialService` | 29+ | Materials and material instances |
| `WidgetService` | 16+ | UMG widget blueprints and components |
| `ActorService` | 24+ | Level actor management |
| `AnimationService` | 89+ | Animation sequences and montages |
| `NiagaraService` | 37+ | Niagara VFX systems |
| `InputService` | 23+ | Enhanced Input actions and contexts |
| `AssetService` | 19+ | Asset discovery and management |
| `OpenClawService` | 15+ | OpenClaw integration and monitoring |

## 🌐 MCP Server Endpoints

The plugin includes a full MCP server for AI assistant integration:

- **HTTP:** `http://127.0.0.1:8089/mcp`
- **WebSocket:** `ws://127.0.0.1:8089/mcp/ws`

### Available MCP Tools:
- `discover_python_module` - Explore UE Python modules
- `discover_python_class` - Get class methods and properties
- `execute_python_code` - Run Python in UE context
- `list_python_subsystems` - List UE editor subsystems
- `manage_skills` - Load domain-specific knowledge
- `read_logs` - Access UE log files

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by [VibeUE](https://github.com/kevinpbuckley/VibeUE)
- Built for the OpenClaw ecosystem
- Community-driven development

## 📞 Support

- **Issues:** [GitHub Issues](https://github.com/Mittenzx/OpenClawUE/issues)
- **Discord:** [OpenClaw Discord](https://discord.gg/clawd)
- **Documentation:** [OpenClawUE Docs](https://docs.openclaw.ai/plugins/openclawue)

---

**Made with ❤️ for the OpenClaw community**