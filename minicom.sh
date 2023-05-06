#!/bin/bash

PROJECT_DIR=~/Downloads/

cd ${PROJECT_DIR} && minicom -b 115200 -o -D /dev/ttyACM0
