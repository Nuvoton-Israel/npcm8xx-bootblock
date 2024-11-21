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

@set CROSS_COMPILER=C:\\cross-compiler\\arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-aarch64-none-elf

@set CROSS_COMPILER_PATH=%CROSS_COMPILER%\\bin\\
@set CROSS_COMPILER_LIB=%CROSS_COMPILER%\\lib\\
@set CROSS_COMPILER_INC=%CROSS_COMPILER%\\include\\

@set MAKE_PATH=C:\MinGW\bin;C:\MinGW\msys\1.0\bin


IF NOT EXIST %CROSS_COMPILER_PATH% goto missing_tools
IF NOT EXIST %MAKE_PATH%           goto missing_tools
     
echo ================================
echo === using GNU tools locally ====
echo ================================
@set PATH=%CROSS_COMPILER_PATH%;%CROSS_COMPILER_LIB%;%MAKE_PATH%;%PATH%


rem pushd \\tanap1\proj_sw\sw_dev_tools
rem set SW_DEV_TOOLS_DIR=%CD%
rem cd /D %BASE_DIR%

rem external flags can be:
rem SET OPTIONAL_FLAGS=-D_SORT_ -DBOOTBLOCK_STACK_PROFILER
SET OPTIONAL_FLAGS=''

@echo ==========================================
@echo = Building Arbel %TARGET% %ENCLAVE% code
@echo ==========================================
if "%1%" EQU "no_tip" (
	make arbel_a35_bootblock_no_tip CROSS_COMPILE=%CROSS_COMPILER_PATH% OPTIONAL_FLAGS=%OPTIONAL_FLAGS% || goto ERROR_COMPILATION
) else (
	make arbel_a35_bootblock CROSS_COMPILE=%CROSS_COMPILER_PATH% OPTIONAL_FLAGS=%OPTIONAL_FLAGS% || goto ERROR_COMPILATION
)

:continue
@set PATH=%PATH_BACKUP%
rem  %TARGET% || goto ERROR_COMPILATION
echo Compile done


@rem Make sure outputs are ready
if not exist .\%DELIV_DIR%\Arbel_a35_bootblock.bin set ERRORLEVEL=1

IF ERRORLEVEL 1 goto ERROR_COMPILATION

goto pass

@rem ============================================================================================================================
@rem Error handling

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
