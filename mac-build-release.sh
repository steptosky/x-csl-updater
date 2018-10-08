#!/bin/bash

export QT_PATH="/Volumes/El Capitan/Users/den_rain/Qt-last/5.11.2/clang_64/"
PROJ_DIR=build

if [ ! -d "$PROJ_DIR" ]; then
    mkdir $PROJ_DIR 
fi
cd $PROJ_DIR
# configure
cmake -G "Unix Makefiles" ../ -DCMAKE_PREFIX_PATH="$QT_PATH" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../Package

# build
cmake --build . --clean-first --config Release

# install
cmake --build . --target install --config Release

cd ../