@echo off
echo Compiling ucs_main.c, sock_create.c, header.c, and heartbeat.c...
gcc .\ucs_main.c .\sock_create.c .\header.c .\heartbeat.c .\includes.c .\validation.c .\acknowledgement.c .\cucs.c .\vehicleid.c -o ucs -lws2_32

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

echo Compilation successful.
