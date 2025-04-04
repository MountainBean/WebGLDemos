@echo off

mkdir ..\..\build
pushd ..\..\build
cl ../demos/1-hellowindow/hellowindow.cpp /DSOKOL_GLCORE /I%HOME%/myprojects/learn-sokol/sokol kernel32.lib user32.lib gdi32.lib -std:c++20
popd
