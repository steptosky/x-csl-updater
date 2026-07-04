#!/bin/bash

source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/setup-qt-env.sh" || exit 1
echo "QT_PATH=$QT_PATH"
PROJ_DIR=build

if [ ! -d "$PROJ_DIR" ]; then
    mkdir $PROJ_DIR 
fi
cd $PROJ_DIR
# configure
cmake -G "Unix Makefiles" ../ \
        -DQT_PATH="$QT_PATH" \
        -DCMAKE_PREFIX_PATH="$QT_PATH" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=../Package \
        -DCLEANUP_INSTALL_FIRST=ON

# build
cmake --build . --clean-first --config Release

# install
cmake --build . --target install --config Release

cd ../
