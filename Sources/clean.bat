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

@set MAKE_BIN_PATH=
@set MAKE_MSYS_PATH=C:\MinGW\msys\1.0\bin
@set MAKE_PATH=

rem Prefer PATH (where) so user environment is the primary source of truth.
for /f "delims=" %%I in ('where make.exe 2^>nul') do (
    set MAKE_BIN_PATH=%%~dpI
    goto make_found
)

rem Fallback for common local MinGW location.
if exist "C:\MinGW\bin\make.exe" set MAKE_BIN_PATH=C:\MinGW\bin\
if not defined MAKE_BIN_PATH if exist "%MAKE_MSYS_PATH%\make.exe" set MAKE_BIN_PATH=%MAKE_MSYS_PATH%\

:make_found
if not defined MAKE_BIN_PATH goto missing_tools
@set MAKE_PATH=%MAKE_BIN_PATH%
if exist "%MAKE_MSYS_PATH%\sh.exe" @set MAKE_PATH=%MAKE_PATH%;%MAKE_MSYS_PATH%
      
echo ================================
echo === using GNU tools locally ====
echo ================================
@set PATH=%MAKE_PATH%;%PATH%


@echo ==========================================
@echo = Clean Arbel %TARGET% code
@echo ==========================================
del /s /q *.o
del /s /q *.d
rd /s /q .\Images


make clean-all || goto ERROR_CLEAN
@set PATH=%PATH_BACKUP%

if exist .\Src\*.o          goto ERROR_CLEAN
if exist .\Src\App\*\*.o    goto ERROR_CLEAN
if exist .\SWC_HAL\*.o      goto ERROR_CLEAN
if exist .\Images\%TARGET%  goto ERROR_CLEAN

IF ERRORLEVEL 1 goto ERROR_CLEAN

goto pass

@rem ============================================================================================================================
@rem Error handling

:missing_tools
color C
set ERRORLEVEL=1
@echo.
@echo ============================== ERROR ===============================
@echo Missing GNU Make. Add make.exe to PATH or install it under C:\MinGW\bin.
@echo ====================================================================
goto end

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
