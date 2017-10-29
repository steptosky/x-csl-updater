#!/usr/bin/python

__author__ = 'StepToSky'

# No arguments = build all

import os
import sys
import platform
import argparse

# --------------------------------------------------------#
# definitions

argBuildType = 'Release'
argBuildFolder = 'build'

# --------------------------------------------------------#
# Utilities

def isConfigured():
    if not os.path.exists(argBuildFolder):
        sys.exit('cis: Configure and build the project first.')

# --------------------------------------------------------#


mainActionHelp = '\n\
debug = install debug version (has effect only on windows).\n\
release = install release version (has effect only on windows).\n'

parser = argparse.ArgumentParser(description='Install')
parser.add_argument('action', choices=['debug', 'release'], help=mainActionHelp)

print('cis: Install script')
args = parser.parse_args(sys.argv[1:])

if args.action == 'debug':
    argBuildType = 'Debug'

if args.action == 'release':
    argBuildType = 'Release'

# --------------------------------------------------------#
# Testers

def installWin():
    os.chdir(argBuildFolder)
    cmakeCommand = 'cmake --build . --target INSTALL --config ' + argBuildType
    print('cis: Run cmake command <' + cmakeCommand + '>')
    if os.system(cmakeCommand):
        sys.exit('cis: Can not install')
    os.chdir('../')

def installLin():
    print('cis: Note: Switching for the Release and Debug mode does work only under Windows')
    os.chdir(argBuildFolder)
    cmakeCommand = 'make install'
    print('cis: Run make command <' + cmakeCommand + '>')
    if os.system(cmakeCommand):
        sys.exit('cis: Can not install')
    os.chdir('../')

# --------------------------------------------------------#
# Run

isConfigured()

print('cis: Start installing ...')

if platform.system() == "Windows":
    installWin()
elif platform.system() == "Linux":
    installLin()
elif platform.system() == "Darwin":
    installLin()
else:
    sys.exit("wf:Unknown OS")

print('cis: End installing')

# --------------------------------------------------------#