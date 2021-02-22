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
@rem *  make_packages.bat
@rem *    Batch file for creating the Regshot release packages
@rem *
@rem ***************************************************************************

@rem Helpful links:
@rem Batch files: http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/batch.mspx



@rem *** Display script title
@ECHO MAKE_PACKAGES.BAT of Regshot - to create binary and symbol packages

@rem *** Make all changes local to the script, switch to the script's directory and set generic variables
SETLOCAL
CD /D "%~dp0"

@rem *** Configurable important settings
SET COMPRESSION_EXE=7z
SET COMPRESSION_TYPE=7z
SET SOURCE_DEFAULT=VS2008

@rem *** Generic variables
PATH %~dp0;%PATH%

SET VS_PROJECT_NAME=Regshot
SET VS_SOURCEFILE_BASENAME=%VS_PROJECT_NAME%

SET LEAVE_SCRIPT=



@rem *** Check for the help switches as first parameter
IF /I "%~1" == "help"   GOTO ShowHelp
IF /I "%~1" == "/help"  GOTO ShowHelp
IF /I "%~1" == "-help"  GOTO ShowHelp
IF /I "%~1" == "--help" GOTO ShowHelp
IF /I "%~1" == "/?"     GOTO ShowHelp



@rem ***
@rem *** Check parameters and prerequisites
@rem ***

@rem *** Check the first parameter for the source of the binaries and symbols
SET SOURCE=
IF "%~1" == "" (
  @ECHO.
  @ECHO Using default: %SOURCE_DEFAULT%
  SET SOURCE=%SOURCE_DEFAULT%
) ELSE (
  IF /I "%~1" == "WDK" (
    SET SOURCE=WDK
  )
  IF /I "%~1" == "/WDK" (
    SET SOURCE=WDK
  )
  IF /I "%~1" == "-WDK" (
    SET SOURCE=WDK
  )
  IF /I "%~1" == "--WDK" (
    SET SOURCE=WDK
  )
  IF /I "%~1" == "VS6" (
    SET SOURCE=VS6
  )
  IF /I "%~1" == "/VS6" (
    SET SOURCE=VS6
  )
  IF /I "%~1" == "-VS6" (
    SET SOURCE=VS6
  )
  IF /I "%~1" == "--VS6" (
    SET SOURCE=VS6
  )
  IF /I "%~1" == "VS2008" (
    SET SOURCE=VS2008
  )
  IF /I "%~1" == "/VS2008" (
    SET SOURCE=VS2008
  )
  IF /I "%~1" == "-VS2008" (
    SET SOURCE=VS2008
  )
  IF /I "%~1" == "--VS2008" (
    SET SOURCE=VS2008
  )
  IF /I "%~1" == "VS2010" (
    SET SOURCE=VS2010
  )
  IF /I "%~1" == "/VS2010" (
    SET SOURCE=VS2010
  )
  IF /I "%~1" == "-VS2010" (
    SET SOURCE=VS2010
  )
  IF /I "%~1" == "--VS2010" (
    SET SOURCE=VS2010
  )
  IF /I "%~1" == "ICL12" (
    SET SOURCE=ICL12
  )
  IF /I "%~1" == "/ICL12" (
    SET SOURCE=ICL12
  )
  IF /I "%~1" == "-ICL12" (
    SET SOURCE=ICL12
  )
  IF /I "%~1" == "--ICL12" (
    SET SOURCE=ICL12
  )
  IF /I "%~1" == "GCC" (
    SET SOURCE=GCC
  )
  IF /I "%~1" == "/GCC" (
    SET SOURCE=GCC
  )
  IF /I "%~1" == "-GCC" (
    SET SOURCE=GCC
  )
  IF /I "%~1" == "--GCC" (
    SET SOURCE=GCC
  )
)
@rem *** Check if unknown parameter value was given
IF "%SOURCE%" == "" (
  CALL :SubMsg "ERROR" "Unsupported commandline parameter '%1'!"
  @ECHO Run "%~nx0 help" for details about the commandline switches.
  SET LEAVE_SCRIPT=X
)

@rem *** Check if needed tools are present
%COMPRESSION_EXE% 1>NUL 2>NUL
IF %ERRORLEVEL% NEQ 0 (
  CALL :SubMsg "ERROR" "Prerequisite 7-Zip missing!"
  @ECHO %COMPRESSION_EXE% wasn't found in your path!
  CALL :Sub7ZipNote
  SET LEAVE_SCRIPT=X
)

@rem *** Exit script if any error occurred
IF NOT "%LEAVE_SCRIPT%" == "" GOTO End



@rem ***
@rem *** Set variables and check for binaries
@rem ***

@rem *** Set variables according to selected source
SET SOURCEFILEx86A=
SET SOURCEFILEx86U=
SET SOURCEFILEx64A=
SET SOURCEFILEx64U=
SET SUFFIX=
SET SYMBOLS_SUFFIX=
IF "%SOURCE%" == "WDK" (
  SET SOURCEFILEx86A=bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SOURCEFILEx86U=bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release_Unicode\%VS_SOURCEFILE_BASENAME%-x86-Unicode.exe
  SET SOURCEFILEx64A=bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release\%VS_SOURCEFILE_BASENAME%-x64-ANSI.exe
  SET SOURCEFILEx64U=bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release_Unicode\%VS_SOURCEFILE_BASENAME%-x64-Unicode.exe
  SET SUFFIX=_WDK
  SET SYMBOLS_SUFFIX=pdb
)
IF "%SOURCE%" == "VS6" (
  SET SOURCEFILEx86A=bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SUFFIX=_VS6
  SET SYMBOLS_SUFFIX=pdb
)
IF "%SOURCE%" == "VS2008" (
  SET SOURCEFILEx86A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SOURCEFILEx86U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release Unicode\%VS_SOURCEFILE_BASENAME%-x86-Unicode.exe
  SET SOURCEFILEx64A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release\%VS_SOURCEFILE_BASENAME%-x64-ANSI.exe
  SET SOURCEFILEx64U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release Unicode\%VS_SOURCEFILE_BASENAME%-x64-Unicode.exe
  SET SUFFIX=_VS2008
  SET SYMBOLS_SUFFIX=pdb
)
IF "%SOURCE%" == "VS2010" (
  SET SOURCEFILEx86A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SOURCEFILEx86U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release Unicode\%VS_SOURCEFILE_BASENAME%-x86-Unicode.exe
  SET SOURCEFILEx64A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release\%VS_SOURCEFILE_BASENAME%-x64-ANSI.exe
  SET SOURCEFILEx64U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release Unicode\%VS_SOURCEFILE_BASENAME%-x64-Unicode.exe
  SET SUFFIX=_VS2010
  SET SYMBOLS_SUFFIX=pdb
)
IF "%SOURCE%" == "VS2012" (
  SET SOURCEFILEx86A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SOURCEFILEx86U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release Unicode\%VS_SOURCEFILE_BASENAME%-x86-Unicode.exe
  SET SOURCEFILEx64A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release\%VS_SOURCEFILE_BASENAME%-x64-ANSI.exe
  SET SOURCEFILEx64U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release Unicode\%VS_SOURCEFILE_BASENAME%-x64-Unicode.exe
  SET SUFFIX=_VS2012
  SET SYMBOLS_SUFFIX=pdb
)
IF "%SOURCE%" == "ICL12" (
  SET SOURCEFILEx86A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release\%VS_SOURCEFILE_BASENAME%-x86-ANSI.exe
  SET SOURCEFILEx86U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\Win32\Release Unicode\%VS_SOURCEFILE_BASENAME%-x86-Unicode.exe
  SET SOURCEFILEx64A=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release\%VS_SOURCEFILE_BASENAME%-x64-ANSI.exe
  SET SOURCEFILEx64U=%VS_OUTPUT%bin\%SOURCE%\%VS_PROJECT_NAME%\x64\Release Unicode\%VS_SOURCEFILE_BASENAME%-x64-Unicode.exe
  SET SUFFIX=_ICL12
)
IF "%SOURCE%" == "GCC" (
  SET SOURCEFILEx86A=bin\GCC\Release_Win32
  SET SOURCEFILEx64A=bin\GCC\Release_x64
  SET SUFFIX=_GCC
)
@rem *** Check if source is not defined in script
IF "%SOURCEFILEx86A%" == "" (
  CALL :SubMsg "ERROR" "Unsupported source '%SOURCE%'!"
  @ECHO Please check the script for errors.
  @ECHO Run "%~nx0 help" for details about the commandline switches.
  GOTO End
)

@rem *** Check if binaries are present
IF NOT "%SOURCEFILEx86A%" == "" (
  IF NOT EXIST "%SOURCEFILEx86A%" (
    CALL :SubMsg "ERROR" "Compile Regshot Win32 ANSI first!"
    SET LEAVE_SCRIPT=X
  )
)
IF NOT "%SOURCEFILEx86U%" == "" (
  IF NOT EXIST "%SOURCEFILEx86U%" (
    CALL :SubMsg "ERROR" "Compile Regshot Win32 Unicode first!"
    SET LEAVE_SCRIPT=X
  )
)
IF NOT "%SOURCEFILEx64A%" == "" (
  IF NOT EXIST "%SOURCEFILEx64A%" (
    CALL :SubMsg "ERROR" "Compile Regshot x64 ANSI first!"
    SET LEAVE_SCRIPT=X
  )
)
IF NOT "%SOURCEFILEx64U%" == "" (
  IF NOT EXIST "%SOURCEFILEx64U%" (
    CALL :SubMsg "ERROR" "Compile Regshot x64 Unicode first!"
    SET LEAVE_SCRIPT=X
  )
)

@rem *** Exit script if any error occurred
IF NOT "%LEAVE_SCRIPT%" == "" GOTO End



@rem ***
@rem *** Create packages
@rem ***

CALL :SubGetVersion

@rem *** binary package
SET PACKAGE_FILENAME_BASE=%VS_SOURCEFILE_BASENAME%-%REGSHOTVER%
SET PACKAGE_DIR=temp_bin

CALL :SubCreatePackDir

PUSHD "%PACKAGE_DIR%\"
CALL svn_changes.bat ..\src
POPD

@ECHO. >"%PACKAGE_DIR%\License.txt"
@XCOPY /V /F /Y "files\license_lgpl-2.1.txt" "%PACKAGE_DIR%\License.txt"
@XCOPY /V /F /Y "files\History.txt"          "%PACKAGE_DIR%\"
@XCOPY /V /F /Y "files\language.ini"         "%PACKAGE_DIR%\"
@XCOPY /V /F /Y "files\ReadMe.txt"           "%PACKAGE_DIR%\"
@XCOPY /V /F /Y "files\regshot.ini"          "%PACKAGE_DIR%\"

IF NOT "%SOURCEFILEx86A%" == ""  @XCOPY /V /F /Y "%SOURCEFILEx86A%" "%PACKAGE_DIR%\"
IF NOT "%SOURCEFILEx86U%" == ""  @XCOPY /V /F /Y "%SOURCEFILEx86U%" "%PACKAGE_DIR%\"
IF NOT "%SOURCEFILEx64A%" == ""  @XCOPY /V /F /Y "%SOURCEFILEx64A%" "%PACKAGE_DIR%\"
IF NOT "%SOURCEFILEx64U%" == ""  @XCOPY /V /F /Y "%SOURCEFILEx64U%" "%PACKAGE_DIR%\"

CALL :SubDoCompression

IF EXIST "%PACKAGE_DIR%" RMDIR /S /Q "%PACKAGE_DIR%"

@rem *** Exit script if any error occurred
IF NOT "%LEAVE_SCRIPT%" == "" GOTO End

@rem *** symbols package
SET PACKAGE_FILENAME_BASE=%VS_SOURCEFILE_BASENAME%-%REGSHOTVER%-symbols
SET PACKAGE_DIR=temp_sym

CALL :SubCreatePackDir

IF NOT "%SOURCEFILEx86A%" == "" CALL :SubCopyFile "%%SOURCEFILEx86A:.exe=.%SYMBOLS_SUFFIX%%%"
IF NOT "%SOURCEFILEx86U%" == "" CALL :SubCopyFile "%%SOURCEFILEx86U:.exe=.%SYMBOLS_SUFFIX%%%"
IF NOT "%SOURCEFILEx64A%" == "" CALL :SubCopyFile "%%SOURCEFILEx64A:.exe=.%SYMBOLS_SUFFIX%%%"
IF NOT "%SOURCEFILEx64U%" == "" CALL :SubCopyFile "%%SOURCEFILEx64U:.exe=.%SYMBOLS_SUFFIX%%%"

CALL :SubDoCompression

IF EXIST "%PACKAGE_DIR%" RMDIR /S /Q "%PACKAGE_DIR%"

@rem *** Exit script if any error occurred
IF NOT "%LEAVE_SCRIPT%" == "" GOTO End

@rem *** All done
CALL :SubMsg "INFO" "All done"
GOTO End



@rem *** Jump point (GOTO) to show script help/usage and exit
:ShowHelp
@ECHO.
@ECHO Usage:   %~nx0 [GCC^|ICL12^|VS2008^|VS2010^|WDK]
@ECHO Notes:   You can also prefix the commands with "-", "--" or "/".
@ECHO          The arguments are not case sensitive.
@ECHO.
@ECHO Executing "%~nx0" will use the defaults: "%~nx0 %SOURCE_DEFAULT%"
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

@rem *** Sub routine: create package directory
:SubCreatePackDir
CALL :SubMsg "INFO" "Creating %PACKAGE_FILENAME_BASE% %COMPRESSION_TYPE% file..."
IF EXIST "%PACKAGE_DIR%" RMDIR /S /Q "%PACKAGE_DIR%"
MKDIR "%PACKAGE_DIR%"
EXIT /B


@rem *** Sub routine: copy file to package directory
:SubCopyFile
XCOPY /V /F /Y "%~1" "%PACKAGE_DIR%\"
EXIT /B


@rem *** Sub routine: create compressed file from package directory
:SubDoCompression
DEL "%PACKAGE_FILENAME_BASE%.%COMPRESSION_TYPE%" 2>NUL
PUSHD "%PACKAGE_DIR%\"
%COMPRESSION_EXE% a -t%COMPRESSION_TYPE% -mx=9 "..\%PACKAGE_FILENAME_BASE%.%COMPRESSION_TYPE%"
IF %ERRORLEVEL% NEQ 0 (
  CALL :SubMsg "ERROR" "Compression failed!"
  CALL :Sub7ZipNote
  SET LEAVE_SCRIPT=X
)
POPD
EXIT /B


@rem *** Sub routine: display 7-zip information
:Sub7ZipNote
@ECHO You should ^(re^)install 7-Zip ^( http://www.7-zip.org/ ^)
@ECHO and/or add its folder to your path.
@ECHO Alternative you could copy 7z.exe and 7z.dll to the script's folder.
EXIT /B


@rem *** Sub routine: determine Regshot version
:SubGetVersion
SET REGSHOTVER=
SET VerType=
SET VerMajor=
SET VerMinor=
SET VerPatch=
SET VerRev=
@rem *** These FOR loops call a sub routine due to variable expansion when the FOR loop is read by the interpreter
@rem *** The variable expansion in the called sub routine is executed when it is called
FOR /F "tokens=3,4 delims= " %%X IN ( 'FINDSTR /I /L /C:"define REGSHOT_VERSION_TYPE" "src\version.rc.h"' ) DO (
  Call :SubVerType "%%X"
)
FOR /F "tokens=3,4 delims= " %%X IN ( 'FINDSTR /I /L /C:"define REGSHOT_VERSION_MAJOR" "src\version.rc.h"' ) DO (
  Call :SubVerMajor "%%X"
)
FOR /F "tokens=3,4 delims= " %%X IN ( 'FINDSTR /I /L /C:"define REGSHOT_VERSION_MINOR" "src\version.rc.h"' ) DO (
  Call :SubVerMinor "%%X"
)
FOR /F "tokens=3,4 delims= " %%X IN ( 'FINDSTR /I /L /C:"define REGSHOT_VERSION_PATCH" "src\version.rc.h"' ) DO (
  Call :SubVerPatch "%%X"
)
FOR /F "tokens=3,4 delims= " %%X IN ( 'FINDSTR /I /L /C:"define REGSHOT_REVISION_NUM" "src\revision.h"' ) DO (
  Call :SubVerRev "%%X"
)
IF "%VerType%" == "trunk" (
  IF "%VerRev%" == "" (
    SET REGSHOTVER=%VerType%
  ) ELSE (
    SET REGSHOTVER=%VerType%_r%VerRev%
  )
) ELSE (
  IF "%VerType%" == "beta" (
    IF "%VerRev%" == "" (
      SET REGSHOTVER=%VerMajor%.%VerMinor%.%VerPatch%-%VerType%
    ) ELSE (
      SET REGSHOTVER=%VerMajor%.%VerMinor%.%VerPatch%-%VerType%_r%VerRev%
    )
  ) ELSE (
    SET REGSHOTVER=%VerMajor%.%VerMinor%.%VerPatch%
  )
)
EXIT /B

:SubVerType
SET VerType=%~1
EXIT /B

:SubVerMajor
SET VerMajor=%~1
EXIT /B

:SubVerMinor
SET VerMinor=%~1
EXIT /B

:SubVerPatch
SET VerPatch=%~1
EXIT /B

:SubVerRev
IF NOT "%~1" == "0" SET VerRev=%VerRev%%~1
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
