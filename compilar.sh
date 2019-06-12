#!/bin/sh

g++ `pkg-config --cflags opencv` histograms.cpp -o croma_art `pkg-config --libs opencv` -std=c++11

