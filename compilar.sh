#!/bin/sh

g++ `pkg-config --cflags opencv` cromaticidad.cpp -o croma_art `pkg-config --libs opencv` -std=c++11

