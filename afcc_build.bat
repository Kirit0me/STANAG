@echo off
echo Compiling afcc_main.c, sock_create.c, header.c, and ack...
gcc .\afcc_main.c .\sock_create.c .\header.c .\heartbeat.c .\validation.c .\acknowledgement.c .\includes.c -o afcc -lws2_32

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

echo Compilation successful.
