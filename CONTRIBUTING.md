# Contributing to OpenClawUE

Thank you for your interest in contributing to OpenClawUE! This document provides guidelines and instructions for contributing to the project.

## 🎯 Development Philosophy

OpenClawUE follows these principles:

1. **AI-First Design**: Everything should be accessible and controllable by AI assistants
2. **Simplicity**: Keep APIs simple and intuitive
3. **Extensibility**: Easy to add new services and features
4. **Documentation**: Code should be self-documenting with clear examples

## 🚀 Getting Started

### Prerequisites
- Unreal Engine 5.7+
- Visual Studio 2019+ (Windows) or Xcode (macOS)
- Python 3.8+
- Git

### Development Setup

1. **Fork the repository** on GitHub
2. **Clone your fork**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/OpenClawUE.git
   cd OpenClawUE
   ```

3. **Set up upstream remote**:
   ```bash
   git remote add upstream https://github.com/Mittenzx/OpenClawUE.git
   ```

4. **Create a development branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

5. **Build the plugin**:
   ```bash
   # Windows
   BuildPlugin.bat
   
   # Linux/Mac
   chmod +x BuildPlugin.sh
   ./BuildPlugin.sh
   ```

## 🏗️ Project Structure

```
OpenClawUE/
├── Source/                    # C++ source code
│   ├── OpenClawUE/           # Main module
│   ├── OpenClawMCP/          # MCP server implementation
│   └── OpenClawPython/       # Python integration
├── Python/                   # Python API
│   └── openclawue/          # Python package
├── Config/                   # Configuration files
├── Content/                  # UE assets and instructions
├── Examples/                 # Example projects and scripts
└── Tests/                    # Unit and integration tests
```

## 📝 Code Style

### C++ Code
- Follow Unreal Engine coding standards
- Use `PascalCase` for classes and `camelCase` for functions/methods
- Include Doxygen-style comments for public APIs
- Use `#pragma once` for header guards

### Python Code
- Follow PEP 8 style guide
- Use type hints for function signatures
- Include docstrings for all public functions
- Use `snake_case` for functions and variables

### Commit Messages
Follow conventional commits format:
```
type(scope): description

[optional body]

[optional footer]
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or fixing tests
- `chore`: Maintenance tasks

## 🧪 Testing

### Running Tests
```bash
# Run Python tests
python -m pytest Tests/Python/

# Run C++ tests (in Unreal Editor)
# Enable Test Automation plugin and run tests
```

### Writing Tests
- Write unit tests for new functionality
- Include integration tests for MCP server
- Test both success and error cases
- Mock external dependencies when appropriate

## 🔧 Adding New Services

To add a new service:

1. **Create C++ service class** in `Source/OpenClawServices/`
2. **Create Python wrapper** in `Python/openclawue/services/`
3. **Add MCP tool handlers** in `Source/OpenClawMCP/`
4. **Write documentation** in `Docs/`
5. **Add examples** in `Examples/`

## 📚 Documentation

### API Documentation
- Document all public classes and functions
- Include usage examples
- Document error conditions and return values

### User Documentation
- Write clear installation instructions
- Include step-by-step tutorials
- Add troubleshooting guide

## 🐛 Reporting Issues

When reporting issues, include:

1. **Description**: Clear description of the issue
2. **Steps to Reproduce**: Step-by-step reproduction instructions
3. **Expected Behavior**: What you expected to happen
4. **Actual Behavior**: What actually happened
5. **Environment**: UE version, OS, plugin version
6. **Logs**: Relevant log output

## 🔄 Pull Request Process

1. **Ensure tests pass** before submitting PR
2. **Update documentation** for new features
3. **Add examples** if applicable
4. **Follow the PR template**
5. **Request review** from maintainers

### PR Checklist
- [ ] Tests added/updated
- [ ] Documentation updated
- [ ] Examples added/updated
- [ ] Code follows style guidelines
- [ ] Commit messages follow convention

## 🏆 Recognition

Contributors will be:
- Listed in the README.md
- Acknowledged in release notes
- Given credit for their contributions

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

## ❓ Questions?

- Join our [Discord](https://discord.gg/clawd)
- Check the [documentation](https://docs.openclaw.ai/plugins/openclawue)
- Open an issue for questions

---

Thank you for contributing to OpenClawUE! 🎉