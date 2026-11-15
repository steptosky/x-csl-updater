# Building X-CSL-Updater

## Requirements

- Git, CMake 3.25 or newer, and Qt 5.15.2 with Widgets and Network.
- Windows: Visual Studio 2019 with C++ tools and the Windows SDK; Qt `msvc2019_64`.
- Linux: GCC/G++, Make, and Qt `gcc_64`.
- macOS: Xcode command-line tools, Make, and Qt `clang_64` (Intel/x86_64).

Run commands from the repository root and adjust the Qt paths to your installation.

## Build

### Windows (PowerShell)

```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DQT_PATH="C:/Qt/5.15.2/msvc2019_64" -DCMAKE_INSTALL_PREFIX=Package
cmake --build build --config Release --target X-CSL-Updater
```

### Linux / macOS (terminal)

```sh
export QT_PATH="$HOME/Qt/5.15.2/gcc_64"  # Linux; use clang_64 for macOS
cmake -S . -B build -G "Unix Makefiles" -DQT_PATH="$QT_PATH" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=Package
cmake --build build --target X-CSL-Updater
```

On macOS, add `-DCMAKE_OSX_ARCHITECTURES=x86_64` to the configure command for the Intel Qt kit.

For Debug: use `--config Debug` on Windows or configure with `-DCMAKE_BUILD_TYPE=Debug` on Linux/macOS.

## Package a Release build

After building Release, package with Qt's deployment tools and, on Linux/macOS, `cqtdeployer` / `create-dmg` on `PATH`:

```sh
cmake --install build --config Release
```

Creates packages in `Package/Packaged/`. Installation is Release-only and clears `Package/Deployed/` and `Package/Packaged/` by default.

Or configure, rebuild, and package using a script:

- Windows: `.\win-build-release.bat`
- Linux: `bash unix-build-release.sh`
- macOS: `bash mac-build-release.sh`

Scripts use `QT_PATH`, falling back to `qt-paths.json`.
