#!/bin/bash

python ci/configure.py cleanup
python ci/configure.py debug
python ci/build.py debug