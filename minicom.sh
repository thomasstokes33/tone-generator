#!/bin/bash

PROJECT_DIR=~/Downloads/tcp-attempt2/pico-tcp-server

cd ${PROJECT_DIR} && minicom -b 115200 -o -D /dev/ttyACM0