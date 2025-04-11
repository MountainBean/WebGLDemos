@echo off

if [%1]==[] goto fail

:usage
SET CODEDIR="%cd%"

mkdir ..\..\embuild
pushd ..\..\embuild
em++ %CODEDIR%/%1.cpp -o %1.html -I%HOME%/OpenGL/include -I../include -sUSE_WEBGL2 -s TOTAL_STACK=32MB -s INITIAL_HEAP=64MB -g -std=c++20 --shell-file ..\code\shell.html --embed-file ".\data\container2.png" --embed-file ".\data\container2_specular.png"
popd
goto eof

:fail
@echo ERROR::EMBUILD::BAT: NO_FILE
@echo Usage: ./shdc filename (do not inlcude extention)

:eof

