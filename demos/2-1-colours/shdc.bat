@echo off

if [%1]==[] goto fail
:usage
C:\Users\Sam\myprojects\learn-sokol\fips-deploy\sokol-tools\win64-vstudio-debug\sokol-shdc -i %1.glsl -o %1.glsl.h -l glsl430:glsl300es
goto eof
:fail
@echo ERROR::SHDC::BAT: NO_FILE
@echo Usage: ./shdc filename (do not inlcude extention)
:eof

