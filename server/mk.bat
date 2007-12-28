@echo off
PATH=%PATH%;c:\programs\dev-cpp\bin\
make -f Makefile WINLIB=-wsock32
pause
