# X-CSL-Updater command-line guide

Command-line options let you choose a target folder, start indexing, and enable detailed logging. The application still requires a desktop session: select packages and click **Update** in its window. Automatic updates and automatic exit are not supported.

For installation instructions, see [English](Readme-eng.txt) or [Русский](Readme-ru.txt).

## Examples

These commands select an X-Plane installation and start indexing. Replace the paths with your own; use absolute paths and quote those containing spaces.

**Windows** — run PowerShell in the updater folder:

```powershell
.\X-CSL-Updater.exe -t "C:\Games\X-Plane 12" -i
```

**Linux** — run from the extracted updater folder:

```sh
./X-CSL-Updater.sh -t "$HOME/X-Plane 12" -i
```

**macOS** — run in Terminal:

```sh
/Applications/X-CSL-Updater.app/Contents/MacOS/X-CSL-Updater -t "$HOME/X-Plane 12" -i
```

## Options

| Option | Short form | Purpose |
| --- | --- | --- |
| `--help` | `-h` | Show help and exit. |
| `--version` | `-v` | Show the version and exit. |
| `--sim-target-dir <directory>` | `-t <directory>` | Use an X-Plane root folder; determine the ALTITUDE library location automatically. |
| `--custom-target-dir <directory>` | — | Use this folder directly as the CSL library destination. |
| `--index-auto-start` | `-i` | Start indexing automatically; otherwise click **Index**. |
| `--verbose` | `-d` | Add debug details to `log.txt` and standard output. |

On Windows, help, version information, and command-line errors appear in dialogs. On Linux and macOS, help and version information appear in the terminal.

## Target folders

For `--sim-target-dir`, supply the X-Plane root folder containing `Resources/plugins`. If it is invalid, the application asks you to select the X-Plane executable.

For a custom library destination:

```powershell
.\X-CSL-Updater.exe --custom-target-dir "D:\CSL\X-CSL-Package" -i
```

Custom mode skips X-Plane checks and installs only library files, excluding additional client files. It uses the supplied folder directly, takes precedence over `--sim-target-dir`, and disables folder controls for that run. Choose a writable destination and back up any existing library before experimenting.

The target and its mode are saved for future launches. To return to a normal installation, use `--sim-target-dir`. The `-i` and `-d` flags apply only to the current run.

## Logs and settings

`log.txt`, `settings.ini`, and `tmp/` are stored beside the executable and require write access. On macOS, they are inside `X-CSL-Updater.app/Contents/MacOS/`.

Add `-d` for troubleshooting. Each launch overwrites `log.txt`, so copy it first if you need to keep it.
