#!/bin/bash

python ci/configure.py cleanup
python ci/configure.py release
python ci/build.py release
