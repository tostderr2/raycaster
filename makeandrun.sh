#!/bin/bash
cmake --build build/ 
echo "---- build done ----"
echo "---- running wolfenstein_raycaster ----"
./build/wolfenstein_raycaster
