#!/bin/bash

QT_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt5"
PROJ_DIR=build

if [ ! -d "$PROJ_DIR" ]; then
    mkdir $PROJ_DIR 
fi
cd $PROJ_DIR
# configure
cmake -G "Unix Makefiles" ../ -DCMAKE_PREFIX_PATH=$QT_PATH -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../Package

# build
cmake --build . --clean-first --config Release

# install
cmake --build . --target install --config Release

cd ../