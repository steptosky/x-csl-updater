#!/usr/bin/python

__author__ = 'StepToSky'

# No arguments = build all

import os
import sys
import platform
import argparse
import shutil

# --------------------------------------------------------#
# definitions

argBuildFolder = 'build'
argTestBuilding = False
argSts = False
argIsCleanUpInstallFolderFirst = False
argDestPath = ''
argGenerator = ''
argBuildType = 'Release'

# --------------------------------------------------------#
# Utilities

def createBuildFolderIfNotExist(inFolder):
    if not os.path.exists(inFolder):
        os.makedirs(inFolder)
        print('cis: <' + inFolder + '> has been created.')


def deleteFolder(inFolder):
    if os.path.exists(inFolder):
        shutil.rmtree(inFolder)
        print('cis: <' + inFolder + '> has been deleted.')


def deleteFile(inFile):
    if os.path.exists(inFile):
        os.remove(inFile)
        print('cis: <' + inFile + '> has been deleted.')


def getCmakeConfigure():
    createBuildFolderIfNotExist(argBuildFolder)
    os.chdir(argBuildFolder)
    cmakeCommand = 'cmake -G ' + argGenerator + ' ../  -DCMAKE_BUILD_TYPE=' + argBuildType

    if argIsCleanUpInstallFolderFirst:
        cmakeCommand = cmakeCommand + ' -DCLEANUP_INSTALL_FIRST=ON'
    else:
        cmakeCommand = cmakeCommand + ' -DCLEANUP_INSTALL_FIRST=OFF'

    if argTestBuilding:
        cmakeCommand = cmakeCommand + ' -DBUILD_TESTS=ON'
    else:
        cmakeCommand = cmakeCommand + ' -DBUILD_TESTS=OFF'

    if argDestPath:
        if not argSts:
            cmakeCommand = cmakeCommand + ' -DCMAKE_INSTALL_PREFIX=' + argDestPath
        else:
            cmakeCommand = cmakeCommand + ' -DSTS_ENV_INSTALL=' + argDestPath

    print('cis: Run cmake command <' + cmakeCommand + '>')
    if os.system(cmakeCommand):
        sys.exit('cis: Can not configure project')

    os.chdir('../')

# --------------------------------------------------------#

mainActionHelp = '\n\
debug = configure debug configuration.\n\
release = configure release configuration.\n\
cleanup = deletes all generated configuration artifacts.\n'

parser = argparse.ArgumentParser(description='Configuration')
parser.add_argument('action', choices=['debug', 'release', 'cleanup'], help=mainActionHelp)
parser.add_argument('--test', action='store_true', help='building tests.')
parser.add_argument('-t', '--tests', action='store_true', help='building tests.')
parser.add_argument('-p', '--prefix', help='Destination path where artifacts will be installed')
parser.add_argument('-c', '--cleanup_install_folder_first', action='store_true', help='Will cleanup the install folder first')
parser.add_argument('-s', '--sts', action='store_true',
                    help='Enable StepToSky environment folders structure for installation target.')

print('cis: Configure script')
args = parser.parse_args(sys.argv[1:])

if args.action == 'debug':
    argBuildType = 'Debug'

if args.action == 'release':
    argBuildType = 'Release'

if args.action == 'cleanup':
    deleteFolder(argBuildFolder)
    sys.exit(0)

if args.test:
    argTestBuilding = True

if args.tests:
    argTestBuilding = True

if args.sts:
    argSts = True

if args.cleanup_install_folder_first:
    argIsCleanUpInstallFolderFirst = True

argDestPath = args.prefix

# --------------------------------------------------------#

argGenerator = '"Unix Makefiles"'

if platform.system() == "Windows":
    argGenerator = '"Visual Studio 14 Win64"'
    getCmakeConfigure()
elif platform.system() == "Linux":
    getCmakeConfigure()
elif platform.system() == "Darwin":
    getCmakeConfigure()
else:
    sys.exit("wf:Unknown OS")

# --------------------------------------------------------#