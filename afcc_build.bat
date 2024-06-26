@echo off
echo Compiling afcc_main.c, sock_create.c, header.c, and heartbeat.c...
gcc .\afcc_main.c .\sock_create.c .\header.c .\heartbeat.c -o afcc -lws2_32

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

echo Compilation successful.
