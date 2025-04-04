@echo off

if [%1]==[] goto fail

:usage
SET CODEDIR="%cd%"
mkdir ..\..\build
pushd ..\..\build
cl %CODEDIR%/%1.cpp /I%HOME%/myprojects/learn-sokol/sokol -I%HOME%/OpenGL/include /I../include -std:c++20
popd
goto eof

:fail
@echo ERROR::BUILD::BAT: NO_FILE
@echo Usage: ./shdc filename (do not inlcude extention)

:eof

