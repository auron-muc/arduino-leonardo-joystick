#!/bin/bash
mv TestBitArray.cpp_test TestBitArray.cpp
g++ TestBitArray.cpp -o TestBitArray
./TestBitArray.exe
rm -f ./TestBitArray.exe
mv TestBitArray.cpp TestBitArray.cpp_test
