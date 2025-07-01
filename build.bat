@echo off
cl -nologo nob.c
nob %*

:: LOCK?
if exist nob.obj del nob.obj
if exist nob.exe del nob.exe
