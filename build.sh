#!/bin/bash

cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -B ./build
if [ "$?" -eq "0" ]
then
	cd build
	make
fi