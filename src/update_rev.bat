@ECHO OFF

@rem ***************************************************************************
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
@rem ***************************************************************************

@rem Helpful links:
@rem http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/batch.mspx

SETLOCAL
PUSHD %~dp0

DEL "revision.h"
SubWCRev .. "revision.h.in" "revision.h" -f 2>NUL
IF %ERRORLEVEL% NEQ 0 GOTO NoSubWCRev
GOTO END


:NoSubWCRev
@ECHO SubWCRev, which is part of TortoiseSVN, wasn't found in your path!
@ECHO You should ^(re^)install TortoiseSVN ^( http://tortoisesvn.net/ ^).
@ECHO Revision "Unknown" is used for now.

(
ECHO #ifndef REGSHOT_REVISION_H
ECHO #define REGSHOT_REVISION_H
ECHO.
ECHO #define REGSHOT_REVISION_NUM          0
ECHO #define REGSHOT_REVISION_NUM_SUFFIX   Unknown
ECHO #define REGSHOT_REVISION_MOD          1
ECHO.
ECHO #endif // REGSHOT_REVISION_H
) > "revision.h"

GOTO END


:END
POPD
ENDLOCAL
EXIT /B
