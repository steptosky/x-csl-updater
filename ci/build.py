#!/usr/bin/python

__author__ = 'StepToSky'

# No arguments = build all

import os
import sys
import argparse
import shutil

# --------------------------------------------------------#
# definitions

argBuildType = 'Release'
argBuildFolder = 'build'

# --------------------------------------------------------#
# Utilities

def deleteFolder(inFolder):
    if os.path.exists(inFolder):
        shutil.rmtree(inFolder)
        print('cis: <' + inFolder + '> has been deleted.')


def deleteFile(inFile):
    if os.path.exists(inFile):
        os.remove(inFile)
        print('cis: <' + inFile + '> has been deleted.')


def deleteFolderTree(inFolder):
    if os.path.exists(inFolder):
        for root, dirs, files in os.walk(inFolder):
            for f in files:
                deleteFile(os.path.join(root, f))
            for d in dirs:
                deleteFolder(os.path.join(root, d))

def clearFolderTree(inFolder):
    if os.path.exists(inFolder):
        for root, dirs, files in os.walk(inFolder):
            for f in files:
                deleteFile(os.path.join(root, f))

def deletePossibleFolders():
    clearFolderTree('bin')
    clearFolderTree('lib')
    clearFolderTree('lib_static')

# --------------------------------------------------------#
# Builder

def runBuild():
    if not os.path.exists(argBuildFolder):
        sys.exit('cis: Configure the project first with configure.py')

    os.chdir(argBuildFolder)
    cmakeCommand = 'cmake --build . --clean-first --config ' + argBuildType
    print('cis: Run cmake command <' + cmakeCommand + '>')
    if os.system(cmakeCommand):
        sys.exit('cis: Can not build project')
    os.chdir('../')

# --------------------------------------------------------#

mainActionHelp = '\n\
debug = build debug.\n\
release = build release.\n\
cleanup = deletes all generated artifacts.\n'

parser = argparse.ArgumentParser(description='Build')
parser.add_argument('action', choices=['debug', 'release', 'cleanup'], help=mainActionHelp)

print('cis: Build script')
args = parser.parse_args(sys.argv[1:])

if args.action == 'debug':
    argBuildType = 'Debug'

if args.action == 'release':
    argBuildType = 'Release'

if args.action == 'cleanup':
    deletePossibleFolders()
    sys.exit(0)

runBuild()

# --------------------------------------------------------#
