@echo off
echo Building Full-Stack Student Management System...
gcc -c include/sqlite3.c -o sqlite3.o
if %ERRORLEVEL% NEQ 0 (
    echo SQLite compilation failed!
    exit /b %ERRORLEVEL%
)
g++ -o output\main.exe main.cpp sqlite3.o -Iinclude -lws2_32 -std=c++11
if %ERRORLEVEL% == 0 (
    echo Build successful! Run .\output\main.exe to start.
) else (
    echo Build failed!
)
