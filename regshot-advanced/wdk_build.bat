@ECHO OFF

@rem ***************************************************************************
@rem * Copyright 2010-2011 XhmikosR
@rem * Copyright 2011-2012 Regshot Team
@rem *
@rem * This file is part of Regshot.
@rem *
@rem * Regshot is free software: you can redistribute it and/or modify
@rem * it under the terms of the GNU Lesser General Public License as published by
@rem * the Free Software Foundation, either version 2.1 of the License, or
@rem * (at your option) any later version.
@rem *
@rem * Regshot is distributed in the hope that it will be useful,
@rem * but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@rem * GNU Lesser General Public License for more details.
@rem *
@rem * You should have received a copy of the GNU Lesser General Public License
@rem * along with Regshot.  If not, see <http://www.gnu.org/licenses/>.
@rem *
@rem *
@rem *  wdk_build.bat
@rem *    Batch file "wrapper" for regshot_wdk.mak, used to build Regshot with WDK
@rem *
@rem ***************************************************************************

@rem Helpful links:
@rem Batch files: http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/batch.mspx
@rem Windows Driver Kit: http://www.microsoft.com/en-us/download/details.aspx?id=11800



@rem *** Display script title
@ECHO WDK_BUILD.BAT of Regshot - to compile with Windows Driver Kit

@rem *** Make all changes local to the script, switch to the script's directory and set generic variables
SETLOCAL ENABLEEXTENSIONS
CD /D "%~dp0"

@rem *** Generic variables
PATH %~dp0;%PATH%
SET LEAVE_SCRIPT=



@rem ***
@rem *** Check parameters and prerequisites
@rem ***
SET SHOWHELP=
SET DO_CLEAN=
SET DO_BUILD=
SET DO_X86=
SET DO_X64=
FOR %%Z IN (%*) DO call :CheckParameter "%%~Z"

@rem *** React on help switch
IF NOT "%SHOWHELP%" == "" GOTO ShowHelp

@rem *** Check the build type
SET BUILDTYPE=
IF NOT "%DO_BUILD%" == "" (
  IF NOT "%DO_CLEAN%" == "" (
    SET BUILDTYPE=rebuild
  ) ELSE (
    SET BUILDTYPE=all
  )
) ELSE (
  IF NOT "%DO_CLEAN%" == "" (
    SET BUILDTYPE=clean
  ) ELSE (
    @ECHO Using default: build
    SET BUILDTYPE=all
  )
)

@rem *** Check the architecture
IF "%DO_X86%" == "" (
  IF "%DO_X64%" == "" (
    @ECHO Using default: all
    SET DO_X86=X
    SET DO_X64=X
  )
)

@rem *** Check the WDK directory
IF NOT DEFINED WDKBASEDIR SET "WDKBASEDIR=C:\WinDDK\7600.16385.1"
IF NOT EXIST "%WDKBASEDIR%" (
  CALL :SubMsg "ERROR" "Specify your WDK directory in env var WDKBASEDIR!"
  SET LEAVE_SCRIPT=X
)

@rem *** Exit script if any error occurred
IF NOT "%LEAVE_SCRIPT%" == "" GOTO End



@rem ***
@rem *** Set variables
@rem ***
set ORGPATH=%PATH%



@rem ***
@rem *** Build Regshot
@rem ***

@rem *** x86 build
IF "%DO_X86%" == "" GOTO x86_end
CALL :SubMsg "INFO" "Building Regshot x86 with WDK"
SET INCLUDE=%WDKBASEDIR%\inc\api;%WDKBASEDIR%\inc\api\crt\stl60;%WDKBASEDIR%\inc\crt;%WDKBASEDIR%\inc\ddk
SET LIB=%WDKBASEDIR%\lib\crt\i386;%WDKBASEDIR%\lib\win7\i386
PATH %WDKBASEDIR%\bin\x86\x86;%WDKBASEDIR%\bin\x86;%ORGPATH%
CALL :SubMsg "INFO" "Building Regshot x86 ANSI"
CALL :NMake CFG="Regshot - Win32 Release"
CALL :SubMsg "INFO" "Building Regshot x86 Unicode"
CALL :NMake CFG="Regshot - Win32 Release Unicode"
:x86_end

@rem *** x64 build
IF "%DO_X64%" == "" GOTO x64_end
CALL :SubMsg "INFO" "Building Regshot x64 with WDK"
SET INCLUDE=%WDKBASEDIR%\inc\api;%WDKBASEDIR%\inc\api\crt\stl60;%WDKBASEDIR%\inc\crt;%WDKBASEDIR%\inc\ddk
SET LIB=%WDKBASEDIR%\lib\crt\amd64;%WDKBASEDIR%\lib\win7\amd64
PATH %WDKBASEDIR%\bin\x86\amd64;%WDKBASEDIR%\bin\x86;%ORGPATH%
CALL :SubMsg "INFO" "Building Regshot x64 ANSI"
CALL :NMake CFG="Regshot - x64 Release"
CALL :SubMsg "INFO" "Building Regshot x64 Unicode"
CALL :NMake CFG="Regshot - x64 Release Unicode"
:x64_end

@rem *** All done
:done
CALL :SubMsg "INFO" "All done"
GOTO End



@rem *** Jump point (GOTO) to show script help/usage and exit
:ShowHelp
@ECHO.
@ECHO Usage:   %~nx0 [clean^|build^|rebuild] [x86^|x64^|all]
@ECHO Notes:   You can also prefix the commands with "-", "--" or "/".
@ECHO          The arguments are not case sensitive.
@ECHO.
@ECHO Prerequisites:
@ECHO Set your WDK directory with WDK_PREPARE.BAT in env var WDKBASEDIR.
@ECHO WDKBASEDIR is currently ^"%WDKBASEDIR%^"
@ECHO.
@ECHO Executing "%~nx0" will use the defaults: "build all"
GOTO End


@rem *** Jump point (GOTO) to cleanly end script
:End
ENDLOCAL
@ECHO.
PAUSE
EXIT /B



@rem ***************************************************************************
@rem *** Sub Routines - Begin
@rem *** Usage: CALL :<name> [<parameter> <parameter> ...]
@rem ***************************************************************************

@rem *** Sub routines: checking parameter
:CheckParameter
@rem *** Check for help switches
SET PARAMETER_OK=
IF /I "%~1" == "HELP"   call :PARA_HELP
IF /I "%~1" == "/HELP"  call :PARA_HELP
IF /I "%~1" == "-HELP"  call :PARA_HELP
IF /I "%~1" == "--HELP" call :PARA_HELP
IF /I "%~1" == "/?"     call :PARA_HELP

IF /I "%~1" == "BUILD"   call :PARA_BUILD
IF /I "%~1" == "/BUILD"  call :PARA_BUILD
IF /I "%~1" == "-BUILD"  call :PARA_BUILD
IF /I "%~1" == "--BUILD" call :PARA_BUILD

IF /I "%~1" == "CLEAN"   call :PARA_CLEAN
IF /I "%~1" == "/CLEAN"  call :PARA_CLEAN
IF /I "%~1" == "-CLEAN"  call :PARA_CLEAN
IF /I "%~1" == "--CLEAN" call :PARA_CLEAN

IF /I "%~1" == "REBUILD"   call :PARA_REBUILD
IF /I "%~1" == "/REBUILD"  call :PARA_REBUILD
IF /I "%~1" == "-REBUILD"  call :PARA_REBUILD
IF /I "%~1" == "--REBUILD" call :PARA_REBUILD

IF /I "%~1" == "x86"   call :PARA_X86
IF /I "%~1" == "/x86"  call :PARA_X86
IF /I "%~1" == "-x86"  call :PARA_X86
IF /I "%~1" == "--x86" call :PARA_X86

IF /I "%~1" == "x64"   call :PARA_X64
IF /I "%~1" == "/x64"  call :PARA_X64
IF /I "%~1" == "-x64"  call :PARA_X64
IF /I "%~1" == "--x64" call :PARA_X64

IF /I "%~1" == "all"   call :PARA_ALL
IF /I "%~1" == "/all"  call :PARA_ALL
IF /I "%~1" == "-all"  call :PARA_ALL
IF /I "%~1" == "--all" call :PARA_ALL

IF NOT "%PARAMETER_OK%" == "" EXIT /B

CALL :SubMsg "ERROR" "Unsupported commandline parameter '%~1'!"
@ECHO Run "%~nx0 help" for details about the commandline switches.
SET LEAVE_SCRIPT=X

EXIT /B

:PARA_HELP
SET SHOWHELP=X
SET PARAMETER_OK=X
EXIT /B

:PARA_BUILD
SET DO_BUILD=X
SET PARAMETER_OK=X
EXIT /B

:PARA_CLEAN
SET DO_CLEAN=X
SET PARAMETER_OK=X
EXIT /B

:PARA_REBUILD
SET DO_CLEAN=X
SET DO_BUILD=X
SET PARAMETER_OK=X
EXIT /B

:PARA_X86
SET DO_X86=X
SET PARAMETER_OK=X
EXIT /B

:PARA_X64
SET DO_X64=X
SET PARAMETER_OK=X
EXIT /B

:PARA_ALL
SET DO_X86=X
SET DO_X64=X
SET PARAMETER_OK=X
EXIT /B


@rem *** Sub routine: display a message
:NMake
nmake /NOLOGO /f "regshot_wdk.mak" %BUILDTYPE% %*
IF %ERRORLEVEL% NEQ 0 CALL :SubMsg "ERROR" "Compilation failed!"
EXIT /B


@rem *** Sub routine: display a message
:SubMsg
@ECHO.
@ECHO ____________________________________________________________
@ECHO [%~1] %~2
@ECHO ____________________________________________________________
EXIT /B

@rem ***************************************************************************
@rem *** Sub Routines - End
@rem ***************************************************************************
