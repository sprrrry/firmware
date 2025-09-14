# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Development Commands

### Prerequisites
- PlatformIO development environment
- Git
- Visual Studio Code (recommended)

### Initial Setup

**For pbxlora_v2 Custom Hardware:**
1. **Clone this repository**: `git clone --recursive https://github.com/sprrrry/firmware.git`
2. **Enter directory**: `cd firmware`
3. **Switch to custom branch**: `git checkout pbxlora-v2-custom`
4. **Build for pbxlora_v2**: `pio run -e pbxlora-v2`
5. **Open in VS Code**: Open the firmware folder in Visual Studio Code

**For standard Meshtastic development:**
1. **Clone repository**: `git clone https://github.com/meshtastic/firmware.git`
2. **Update submodules**: `cd firmware && git submodule update --init`
3. **Open in VS Code**: Open the firmware folder in Visual Studio Code

### Building Firmware
**Recommended Method (VS Code + PlatformIO):**
1. Open Command Palette (Ctrl+Shift+P / Cmd+Shift+P)
2. Select "PlatformIO: Pick Project Environment"
3. Choose your target device
4. Run "PlatformIO: Build"
5. Optional: Run "PlatformIO: Upload" to flash directly

**Command Line Methods:**
- **Default build**: `pio run` (builds default target: tbeam)
- **Build specific target**: `pio run -e <target>` (e.g., `pio run -e heltec-v3`)
- **Clean build**: `pio run -t clean`
- **Upload firmware**: `pio run -e <target> -t upload`

**Batch Build Scripts** (in `bin/` directory):
- `bin/build-esp32.sh` - ESP32 targets
- `bin/build-nrf52.sh` - nRF52 targets
- `bin/build-native.sh` - Native/Linux targets
- `bin/build-rp2xx0.sh` - RP2040/RP2350 targets

### Code Formatting and Linting
- **Format code**: `trunk fmt` (uses Trunk.io for consistent formatting)
- **Check code quality**: `trunk check` (runs all configured linters)
- **Install formatters**: Install Trunk VSCode extension

### Testing
- **Unit tests**: Located in `test/` directory with platform-specific subdirectories
- **Native testing**: `pio test -e native`
- **End-to-end tests**: Located in `meshtestic/` directory (Node.js based)
- **Run e2e tests**: `cd meshtestic && pnpm run test`

### Firmware Installation/Flashing

**Recommended: Web Flasher** (for end users)
- Use https://flasher.meshtastic.org
- Works with Chrome or Edge browsers
- Easy to use, especially for new users

**Development Scripts** (for developers)
- **Device install script**: `bin/device-install.sh` or `bin/device-install.bat`
- **Device update script**: `bin/device-update.sh` or `bin/device-update.bat`

**PlatformIO Upload** (during development)
- **Direct upload**: `pio run -e <target> -t upload`
- **Upload via VS Code**: Use "PlatformIO: Upload" command

## Architecture Overview

### Core Structure
- **`src/main.cpp`**: Main application entry point and initialization
- **`src/mesh/`**: Core mesh networking implementation
  - Router classes (FloodingRouter)
  - Channels and encryption (Channels.cpp, CryptoEngine.cpp)
  - Protocol definitions and generated protobuf code
- **`src/modules/`**: Modular feature implementations
  - AdminModule, CannedMessageModule, ExternalNotificationModule, etc.
  - Each module handles specific device functionality
- **`src/platform/`**: Platform-specific code for different hardware
- **`src/gps/`**: GPS functionality and location services
- **`src/graphics/`**: Display and UI implementations

### Key Components
- **Power Management**: `Power.cpp`, `PowerFSM.cpp` - handles device power states
- **Concurrency**: `src/concurrency/` - threading and task management
- **Configuration**: `configuration.h` - compile-time configuration system
- **Hardware Abstraction**: Platform-specific variants in `variants/` directory

### Hardware Support
- **ESP32**: Primary platform with WiFi/Bluetooth
- **nRF52**: Nordic semiconductor chips for low power
- **RP2040/RP2350**: Raspberry Pi Pico chips
- **Native**: Linux/desktop builds for development/simulation

### Build System
- **PlatformIO**: Primary build system configured in `platformio.ini`
- **Multiple environments**: Each hardware variant has its own build environment
- **Conditional compilation**: Uses preprocessor flags for feature selection
- **Library management**: Dependencies managed through PlatformIO

### Protobuf Integration
- Protocol definitions in `protobufs/` subdirectory (git submodule)
- Generated code in `src/mesh/generated/`
- Regenerate with `bin/regen-protos.sh`

## Development Guidelines

### Code Standards
- Follow existing code formatting (enforced by Trunk)
- Use consistent naming conventions matching the codebase
- Add appropriate platform guards for hardware-specific code
- Follow the modular architecture pattern for new features

### Adding New Hardware
- Create variant in `variants/` directory
- Add platformio.ini environment configuration
- Implement platform-specific code in `src/platform/`
- Update build scripts if needed

**Custom Hardware Guidelines:**
- Create a new directory in `variants/` (copy existing configuration)
- Modify `platformio.ini` and `variant.h` for your hardware
- Use `PRIVATE_HW` model number for custom hardware
- Test and iterate - community hardware has limited official support
- First-time builds may take longer as PlatformIO downloads tooling

### Module Development
- Extend from base Module class
- Follow existing module patterns for message handling
- Add configuration options to protobuf definitions
- Include appropriate platform guards for optional features

### Important Build Notes
- First-time setup downloads required tooling and may take significant time
- Submodule updates are required after cloning (`git submodule update --init`)
- Web client at meshtastic.local only updates with full device wipe/reinstall
- Export configuration before full reinstalls to preserve settings

## pbxlora_v2 Custom Hardware

This firmware includes custom support for pbxlora_v2 hardware with the following features:

### Hardware Specifications
- **MCU**: ESP32-WROOM-32E
- **LoRa Radio**: SX1262
- **Display**: 128x64 OLED (I2C)
- **Sensor**: TF-02pro lidar (40m range)
- **I2C Pins**: SDA=17, SCL=16
- **Power**: Battery voltage monitoring on GPIO39

### Custom Features
- **TF-02pro Integration**: Uses RCWL9620Sensor interface for compatibility
- **Environmental Telemetry Fix**: Remote nodes environmental data properly stored in NodeDB
- **Dynamic Update Intervals**: iOS app update rate matches environmental telemetry interval
- **Custom Channel Support**: Bypass 30-minute minimum interval restriction
- **OLED Display**: Automatic detection and configuration

### Clone Instructions for Others
```bash
# Clone with all submodules
git clone --recursive https://github.com/sprrrry/firmware.git
cd firmware

# Switch to pbxlora_v2 branch
git checkout pbxlora-v2-custom

### Submodule Notes
This fork uses a custom protobufs submodule (`sprrrry/protobufs`) that includes:
- `EnvironmentMetrics` field added to `NodeInfoLite` structure
- Enables proper environmental telemetry storage for remote nodes
- Compatible with standard Meshtastic apps and web interface

### Configuration Commands
```bash
# Set 60-second environmental telemetry
meshtastic --port COM29 --set telemetry.environment_update_interval 60
```