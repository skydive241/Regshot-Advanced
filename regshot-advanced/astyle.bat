@ECHO OFF
SETLOCAL

PUSHD %~dp0

AStyle.exe --version 1>&2 2>NUL

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Astyle wasn't found!
  ECHO Visit http://astyle.sourceforge.net/ for download and details.
  GOTO END
)

AStyle.exe --style=kr^
 --indent=spaces=4 --indent-switches --indent-namespaces --indent-col1-comments^
 --add-brackets^
 --pad-header --pad-oper --unpad-paren^
 --align-pointer=name^
 --preserve-date^
 --recursive src\*.c src\*.h src\*.in

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Something went wrong!
  GOTO END
)

:END
POPD
PAUSE
ENDLOCAL
EXIT /B
