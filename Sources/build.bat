@echo off
color 7

rem ---------------------------------------------------------------------------------------------------------#
rem  SPDX-License-Identifier: GPL-2.0                                                                        #
rem  Copyright (c) 2024 by Nuvoton Technology Corporation                                                    #
rem  All rights reserved                                                                                     #
rem ---------------------------------------------------------------------------------------------------------#
rem  File Contents:                                                                                          #
rem    build.bat                                                                                             #
rem             batch for building code, for Windows                                                         #
rem   Project:  Arbel BootBlock for A35                                                                      #
rem ---------------------------------------------------------------------------------------------------------#
set TARGET=arbel
set ENCLAVE=%1
@set DELIV_DIR=%WORK_CD%\Images\%ENCLAVE%


@echo ========================================
@echo = build: Building %TARGET% %ENCLAVE%
@echo ========================================

@set ERRORLEVEL=0

@set PATH_BACKUP=%PATH%

@set CROSS_COMPILER=
@set CROSS_COMPILER_PATH=
@set MAKE_BIN_PATH=
@set MAKE_MSYS_PATH=C:\MinGW\msys\1.0\bin
@set MAKE_PATH=

rem Honor CC_PATH when it points to either the toolchain root or its bin directory.
if defined CC_PATH if exist "%CC_PATH%\bin\aarch64-none-elf-gcc.exe" set CROSS_COMPILER_PATH=%CC_PATH%\bin\
if defined CC_PATH if exist "%CC_PATH%\aarch64-none-elf-gcc.exe" set CROSS_COMPILER_PATH=%CC_PATH%\

rem Prefer PATH (where) so user environment is the primary source of truth.
if not defined CROSS_COMPILER_PATH for /f "delims=" %%I in ('where aarch64-none-elf-gcc.exe 2^>nul') do (
    set CROSS_COMPILER_PATH=%%~dpI
    goto cross_compiler_found
)

rem Fallback for common local install locations.
for %%R in (
    "C:\cross-compiler\arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-aarch64-none-elf"
) do (
    if not defined CROSS_COMPILER_PATH if exist "%%~R\bin\aarch64-none-elf-gcc.exe" set CROSS_COMPILER_PATH=%%~R\bin\
)

:cross_compiler_found
if defined CROSS_COMPILER_PATH (
    for %%R in ("%CROSS_COMPILER_PATH%..") do set CROSS_COMPILER=%%~fR
)

@set CROSS_COMPILER_LIB=%CROSS_COMPILER%\lib\
@set CROSS_COMPILER_INC=%CROSS_COMPILER%\include\

rem Prefer make from PATH, then fall back to the standard MinGW location.
for /f "delims=" %%I in ('where make.exe 2^>nul') do (
    set MAKE_BIN_PATH=%%~dpI
    goto make_found
)
if exist "C:\MinGW\bin\make.exe" set MAKE_BIN_PATH=C:\MinGW\bin\
if not defined MAKE_BIN_PATH if exist "%MAKE_MSYS_PATH%\make.exe" set MAKE_BIN_PATH=%MAKE_MSYS_PATH%\

:make_found
if not defined CROSS_COMPILER_PATH goto missing_tools
if not defined MAKE_BIN_PATH goto missing_tools

@set MAKE_PATH=%MAKE_BIN_PATH%
if exist "%MAKE_MSYS_PATH%\sh.exe" @set MAKE_PATH=%MAKE_PATH%;%MAKE_MSYS_PATH%
     
echo ================================
echo === using GNU tools locally ====
echo ================================
@set PATH=%CROSS_COMPILER_PATH%;%CROSS_COMPILER_LIB%;%MAKE_PATH%;%PATH%


rem pushd \\tanap1\proj_sw\sw_dev_tools
rem set SW_DEV_TOOLS_DIR=%CD%
rem cd /D %BASE_DIR%

rem external flags can be:
rem SET OPTIONAL_FLAGS=-D_SORT_ -DBOOTBLOCK_STACK_PROFILER
SET OPTIONAL_FLAGS=

@echo ==========================================
@echo = Building Arbel %TARGET% %ENCLAVE% code
@echo ==========================================
if "%1%" EQU "no_tip" (
	make arbel_a35_bootblock_no_tip CROSS_COMPILE=%CROSS_COMPILER_PATH% OPTIONAL_FLAGS=%OPTIONAL_FLAGS% ENCLAVE=%ENCLAVE% || goto ERROR_COMPILATION
) else if "%1%" EQU "sip" (
	make arbel_a35_bootblock_sip_test CROSS_COMPILE=%CROSS_COMPILER_PATH% OPTIONAL_FLAGS=%OPTIONAL_FLAGS% ENCLAVE=%ENCLAVE% || goto ERROR_COMPILATION
) else (
	make arbel_a35_bootblock CROSS_COMPILE=%CROSS_COMPILER_PATH% OPTIONAL_FLAGS=%OPTIONAL_FLAGS% ENCLAVE=%ENCLAVE% || goto ERROR_COMPILATION
)

:continue
@set PATH=%PATH_BACKUP%
rem  %TARGET% || goto ERROR_COMPILATION
echo Compile done


@rem Make sure outputs are ready
if "%1%" EQU "sip" (
    if not exist .\%DELIV_DIR%\Arbel_a35_bootblock_sip.bin set ERRORLEVEL=1
) else if "%1%" EQU "no_tip" (
    if not exist .\%DELIV_DIR%\Arbel_a35_bootblock_no_tip.bin set ERRORLEVEL=1
) else (
    if not exist .\%DELIV_DIR%\Arbel_a35_bootblock.bin set ERRORLEVEL=1
)

IF ERRORLEVEL 1 goto ERROR_COMPILATION

goto pass

@rem ============================================================================================================================
@rem Error handling

:missing_tools
set ERRORLEVEL=1
@echo.
@echo ============================== ERROR ===============================
if not defined CROSS_COMPILER_PATH @echo Missing aarch64-none-elf GCC toolchain. Add its bin directory to PATH or set CC_PATH.
if not defined MAKE_BIN_PATH @echo Missing GNU Make. Add make.exe to PATH or install it under C:\MinGW\bin.
@echo ====================================================================
goto end

:ERROR_COMPILATION
rem color C
set ERRORLEVEL=1
@echo.
@echo ============================== ERROR ===============================
@echo Error building Arbel %TARGET% %SECURITY%
@echo ====================================================================
type log.txt
goto end

@rem ============================================================================================================================
@rem END

:pass

color A
set ERRORLEVEL=0
@echo.
@echo ====================================================================
@echo      build.bat: Arbel %TARGET% %SECURITY% %ENCLAVE% build ended successfully
@echo ====================================================================


:end
@set PATH=%PATH_BACKUP%
EXIT /B %ERRORLEVEL%
