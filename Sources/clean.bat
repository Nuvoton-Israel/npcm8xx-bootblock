@echo off
color 7

rem ---------------------------------------------------------------------------------------------------------#
rem  SPDX-License-Identifier: GPL-2.0                                                                        #
rem  Copyright (c) 2022 by Nuvoton Technology Corporation                                                    #
rem  All rights reserved                                                                                     #
rem ---------------------------------------------------------------------------------------------------------#
rem  File Contents:                                                                                          #
rem    clean.bat                                                                                             #
rem             batch for cleaning objects, for Windows                                                      #
rem   Project:  Arbel                                                                                        #
rem ---------------------------------------------------------------------------------------------------------#
set TARGET=arbel_a35_bootlbock

if not "%1"=="" set TARGET=%1

@set ERRORLEVEL=0
@set PATH_BACKUP=%PATH%

@set CROSS_COMPILER_PATH=C:\cross-compiler\gcc-linaro-7.5.0-2019.12-i686-mingw32_aarch64-elf\bin\
@set CROSS_COMPILER_LIB= C:\cross-compiler\gcc-linaro-7.5.0-2019.12-i686-mingw32_aarch64-elf\aarch64-elf\libc\usr\lib\
@set MAKE_PATH=C:\MinGW\bin;C:\MinGW\msys\1.0\bin

IF NOT EXIST %CROSS_COMPILER_PATH% goto missing_tools
IF NOT EXIST %MAKE_PATH%           goto missing_tools
     
echo ================================
echo === using GNU tools locally ====
echo ================================
@set PATH=%CROSS_COMPILER_PATH%;%MAKE_PATH%;%PATH%


@echo ==========================================
@echo = Clean Arbel %TARGET% code
@echo ==========================================
del /s /q *.o
del /s /q *.d
rd /s /q .\Images


make clean-all
@set PATH=%PATH_BACKUP%

if exist .\Src\*.o          goto ERROR_CLEAN
if exist .\Src\App\*\*.o    goto ERROR_CLEAN
if exist .\SWC_HAL\*.o      goto ERROR_CLEAN
if exist .\Images\%TARGET%  goto ERROR_CLEAN

IF ERRORLEVEL 1 goto ERROR_CLEAN

goto pass

@rem ============================================================================================================================
@rem Error handling

:ERROR_CLEAN
color C
set ERRORLEVEL=1
@echo.
@echo ============================== ERROR ===============================
@echo Error cleaning %TARGET%
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
@echo %TARGET% clean ended successfully
@echo ====================================================================

:end
@set PATH=%PATH_BACKUP%
EXIT /B %ERRORLEVEL%
