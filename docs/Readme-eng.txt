Thank you for choosing X-CSL-Package!

X-CSL-Package is a library of CSL aircraft models used to display IVAO traffic in X-Plane.
The project is developed and maintained by the X-AiR and StepToSky Teams.
Use X-CSL-Updater to install the library and keep it up to date.

--------------------------------------------------------------------------------
Official X-CSL-Package project website (https://x-csl.ru/?lang_id=43)
Copyright © 2009-2026 X-AiR Team (https://x-air.ru)
Copyright © 2009-2026 StepToSky Team (https://steptosky.com)

Use this form to contact us: https://x-csl.ru/contact/?lang_id=43

--------------------------------------------------------------------------------
Support and compatibility

- Using the X-CSL-Package alongside other CSL libraries or individual CSL models is not officially supported.
You may try this at your own risk, but the X-AiR and StepToSky Teams are not responsible for any resulting problems.
For a supported setup, use a CSL library folder containing only the X-CSL-Package.
Back up your existing CSL library before replacing it or testing a mixed setup.

- Using the X-CSL-Package with other virtual aviation network clients is not officially supported.
You may try it at your own risk, but the X-AiR and StepToSky Teams are not responsible for any resulting problems.
To specify a custom CSL library location, choose Menu -> Set Custom Path.
Back up your existing CSL library before testing this setup.

--------------------------------------------------------------------------------
Installation and startup

Windows:
1. Download the Windows .zip archive.
2. Extract the archive to a folder of your choice.
3. Open the extracted folder and run X-CSL-Updater.exe.

Linux:
1. Download the Linux .tar.gz archive.
2. Extract the archive to a folder of your choice.
3. Open a terminal in the extracted folder containing X-CSL-Updater.sh and run:
   ./X-CSL-Updater.sh

macOS:
1. Download the macOS .dmg disk image.
2. Open the disk image and drag X-CSL-Updater.app to the Applications folder.
3. Launch X-CSL-Updater from the Applications folder.

--------------------------------------------------------------------------------
Installing or updating CSL packages (all systems)

1. Click "Browse" and open your X-Plane installation folder. Select:
   - Windows: the X-Plane executable (.exe).
   - Linux: the X-Plane executable.
   - macOS: the X-Plane application (.app).
   If you need a custom CSL library location, choose Menu -> Set Custom Path instead.
2. Click "Index" to compare your local CSL files with the package index on the server.
3. Wait for indexing to finish. The package list will show which packages need to be installed or updated.
4. Select the packages you want to install or update. To select multiple packages,
   hold Ctrl on Windows or Linux, or Command on macOS, while clicking them.
   You can also click "Select All" to select every package.
5. Click "Update" and wait for the operation to finish.

--------------------------------------------------------------------------------
Command-line reference

  -h, --help                         Show help and exit.
  -v, --version                      Show the version and exit.
  -t, --sim-target-dir <directory>    Set the X-Plane root folder.
      --custom-target-dir <directory>
                                    Set the CSL library folder directly.
  -i, --index-auto-start             Start indexing automatically.
  -d, --verbose                      Enable detailed logging.

Quote paths containing spaces. The target folder is saved for future launches.
Custom mode skips X-Plane checks, excludes additional client files, and takes
precedence over --sim-target-dir. Folder controls are disabled for that run.
Indexing does not start an update: select packages and click "Update" in the window.
The -i and -d flags apply only to the current run. Each launch overwrites log.txt
beside the executable.
On Windows, --help and --version display a message box instead of terminal output.
Click "OK" to close the message box and exit the application.
