/*
    Copyright 2011-2015 Regshot Team
    Copyright 2011-2012 XhmikosR

    This file is part of Regshot.

    Regshot is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    Regshot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Regshot.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGSHOT_VERSION_RC_H
#define REGSHOT_VERSION_RC_H


#include "revision.h"

#define DO_STRINGIFY(x) #x
#define STRINGIFY(x)    DO_STRINGIFY(x)


/*
    Version and Revision

    trunk  = Regshot |       | x86/x64 | ANSI/Unicode | [Dbg] | (<type> rXXX[M])
    branch = Regshot | N.M.P | x86/x64 | ANSI/Unicode | [Dbg] | (<type> rXXX[M])
    tags   = Regshot | N.M.P | x86/x64 | ANSI/Unicode | [Dbg] |
*/
#define release   0
#define trunk     1
#define beta      2

#define REGSHOT_VERSION_TYPE    release
#define REGSHOT_VERSION_MAJOR   2
#define REGSHOT_VERSION_MINOR   0
#define REGSHOT_VERSION_PATCH   7

#define REGSHOT_VERSION_NUM_DOTS  REGSHOT_VERSION_MAJOR.REGSHOT_VERSION_MINOR.REGSHOT_VERSION_PATCH

#if REGSHOT_VERSION_TYPE == trunk
#   define REGSHOT_VERSION_NUM_DOTS2
#else
#   define REGSHOT_VERSION_NUM_DOTS2   TEXT(" ") TEXT(STRINGIFY(REGSHOT_VERSION_NUM_DOTS))
#endif

#if REGSHOT_REVISION_NUM != 0
#   define REGSHOT_REVISION_NUM2   TEXT(STRINGIFY(REGSHOT_REVISION_NUM))
#else
#   define REGSHOT_REVISION_NUM2
#endif

#if REGSHOT_REVISION_MOD != 0
#   define REGSHOT_REVISION_NUM_SUFFIX2   TEXT(STRINGIFY(REGSHOT_REVISION_NUM_SUFFIX))
#else
#   define REGSHOT_REVISION_NUM_SUFFIX2
#endif

#undef REGSHOT_VERSION_NAME
#if REGSHOT_VERSION_TYPE == trunk
#   define REGSHOT_VERSION_NAME   TEXT("trunk")
#elif REGSHOT_VERSION_TYPE == beta
#   define REGSHOT_VERSION_NAME   TEXT("beta")
#endif
#if defined(REGSHOT_VERSION_NAME)
#   define REGSHOT_VERSION_NAME2   REGSHOT_VERSION_NAME TEXT(" r") REGSHOT_REVISION_NUM2 REGSHOT_REVISION_NUM_SUFFIX2
#   define REGSHOT_VERSION_NAME3   TEXT(" (") REGSHOT_VERSION_NAME2 TEXT(")")
#   define REGSHOT_VERSION_NAME4   TEXT(" ") REGSHOT_VERSION_NAME
#else
#   define REGSHOT_VERSION_NAME2
#   define REGSHOT_VERSION_NAME3
#   define REGSHOT_VERSION_NAME4
#endif

#if REGSHOT_VERSION_TYPE == trunk
#   define REGSHOT_FILEVERSION       0,0,0,REGSHOT_REVISION_NUM
#   define REGSHOT_PRODUCTVERSION   REGSHOT_VERSION_NAME2
#else
#   define REGSHOT_FILEVERSION      REGSHOT_VERSION_MAJOR,REGSHOT_VERSION_MINOR,REGSHOT_VERSION_PATCH,REGSHOT_REVISION_NUM
#   define REGSHOT_PRODUCTVERSION   TEXT(STRINGIFY(REGSHOT_VERSION_NUM_DOTS)) REGSHOT_VERSION_NAME3
#endif


/*
    ANSI/Unicode
*/
#if defined(_UNICODE)
#define REGSHOT_CODEPAGE   TEXT("Unicode")
#else
#define REGSHOT_CODEPAGE   TEXT("ANSI")
#endif


/*
    Build types
*/
#undef REGSHOT_BUILDTYPE
#if defined(_DEBUG)
#   define REGSHOT_BUILDTYPE    TEXT("Debug")
#   define REGSHOT_BUILDTYPE2   TEXT(" Dbg")
#else
#   define REGSHOT_BUILDTYPE2
#endif

#if defined(REGSHOT_VERSION_NAME) || defined(REGSHOT_BUILDTYPE)
#   if defined(REGSHOT_VERSION_NAME) && defined(REGSHOT_BUILDTYPE)
#       define REGSHOT_SPECIALBUILD   REGSHOT_VERSION_NAME TEXT(" ") REGSHOT_BUILDTYPE
#   elif defined(REGSHOT_VERSION_NAME)
#       define REGSHOT_SPECIALBUILD   REGSHOT_VERSION_NAME
#   elif defined(REGSHOT_BUILDTYPE)
#       define REGSHOT_SPECIALBUILD   REGSHOT_BUILDTYPE
#   endif
#endif


/*
    Copyright
*/
//#define REGSHOT_VERSION_COPYRIGHT   TEXT("Copyright (C) 1999-2021, Patrick (skydive241@gmx.de), Regshot Team and all contributors")
#define REGSHOT_VERSION_COPYRIGHT   TEXT("Copyright (C) 1999-2021, Patrick (skydive241@gmx.de), Regshot Team and all contributors")


/*
    URL
*/
#define REGSHOT_URL   TEXT("https://github.com/skydive241/Regshot-Advanced")


/*
    Define title and result file name according to platform target and processor architecture
    For predefined macros see http://msdn.microsoft.com/en-us/library/b0084kay.aspx
    and http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
    Note that these predefined macros have to be set manually for the resource compiler
*/
#define REGSHOT_BASENAME   TEXT("Regshot")

#if defined(_WIN64)
#   if (defined(_M_IA64) || defined(__ia64__))  /* Itanium (IA64) was first supported on Windows 2000 */
#       define REGSHOT_VERSION_PLATFORM   TEXT("IA64")
#       define REGSHOT_RESULT_FILE        TEXT("~res-IA64")
#       if defined(_UNICODE)
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot IA64 Unicode" "Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot IA64 Unicode")
#           endif
#       else
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot IA64 ANSI Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot IA64 ANSI")
#           endif
#       endif
#   elif (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64__))  /* AMD64 was first supported on Windows XP, called "x64" by Microsoft */
#       define REGSHOT_VERSION_PLATFORM   TEXT("x64")
#       define REGSHOT_RESULT_FILE        TEXT("~res-x64")
#       if defined(_UNICODE)
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot x64 Unicode Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot x64 Unicode")
#           endif
#       else
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot x64 ANSI Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot x64 ANSI")
#           endif
#       endif
#   else
#       define REGSHOT_VERSION_PLATFORM   TEXT("64-Bit unknown architecture")
#       define REGSHOT_RESULT_FILE        TEXT("~res-64-Bit")
#       if defined(_UNICODE)
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot 64-Bit unknown architecture Unicode Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot 64-Bit unknown architecture Unicode")
#           endif
#       else
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot 64-Bit unknown architecture ANSI Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot 64-Bit unknown architecture ANSI")
#           endif
#       endif
#   endif
#elif defined(_WIN32)
#   if (defined(_M_IX86) || defined(_X86_) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__))
#       define REGSHOT_VERSION_PLATFORM   TEXT("x86")
#       define REGSHOT_RESULT_FILE        TEXT("~res-x86")
#       if defined(_UNICODE)
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot x86 Unicode Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot x86 Unicode")
#           endif
#       else
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot x86 ANSI Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot x86 ANSI")
#           endif
#       endif
#   else
#       define REGSHOT_VERSION_PLATFORM   TEXT("32-Bit unknown architecture")
#       define REGSHOT_RESULT_FILE        TEXT("~res-32-Bit")
#       if defined(_UNICODE)
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot 32-Bit unknown architecture Unicode Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot 32-Bit unknown architecture Unicode")
#           endif
#       else
#           if defined(_DEBUG)
#               define REGSHOT_CAPTION    TEXT("Regshot 32-Bit unknown architecture ANSI Debug")
#           else
#               define REGSHOT_CAPTION    TEXT("Regshot 32-Bit unknown architecture ANSI")
#           endif
#       endif
#   endif
#else
#   define REGSHOT_VERSION_PLATFORM   TEXT("unknown platform")
#   define REGSHOT_RESULT_FILE        TEXT("~res-unknown-platform")
#   if defined(_UNICODE)
#       if defined(_DEBUG)
#           define REGSHOT_CAPTION    TEXT("Regshot unknown platform Unicode Debug")
#       else
#           define REGSHOT_CAPTION    TEXT("Regshot unknown platform Unicode")
#       endif
#   else
#       if defined(_DEBUG)
#           define REGSHOT_CAPTION    TEXT("Regshot unknown platform ANSI Debug")
#       else
#           define REGSHOT_CAPTION    TEXT("Regshot unknown platform ANSI")
#       endif
#   endif
#endif // _WINxx

#define REGSHOT_TITLE   REGSHOT_BASENAME REGSHOT_VERSION_NUM_DOTS2 TEXT(" ") REGSHOT_VERSION_PLATFORM TEXT(" ") REGSHOT_CODEPAGE REGSHOT_BUILDTYPE2 REGSHOT_VERSION_NAME3


#endif // REGSHOT_VERSION_RC_H
