#******************************************************************************
#* Copyright 2010-2011 XhmikosR
#* Copyright 2011-2012 Regshot Team
#*
#* This file is part of Regshot.
#*
#* Regshot is free software: you can redistribute it and/or modify
#* it under the terms of the GNU Lesser General Public License as published by
#* the Free Software Foundation, either version 2.1 of the License, or
#* (at your option) any later version.
#*
#* Regshot is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU Lesser General Public License for more details.
#*
#* You should have received a copy of the GNU Lesser General Public License
#* along with Regshot.  If not, see <http://www.gnu.org/licenses/>.
#*
#*
#*  regshot_wdk.mak
#*    makefile for building Regshot with WDK's NMAKE
#*
#*    Use wdk_prepare.bat to set WDK directory and extra flags.
#*    Use wdk_build.bat to finally create builds.
#******************************************************************************

# Useful links:
# Windows Driver Kit: http://www.microsoft.com/en-us/download/details.aspx?id=11800
# NMake reference: http://msdn.microsoft.com/en-us/library/dd9y37ha%28v=vs.90%29.aspx


# Comment out the .SILENT directive in order to display all the commands
.SILENT:

!IF "$(CFG)" == ""
CFG=Regshot - Win32 Release
!MESSAGE No configuration specified. Defaulting to Regshot - Win32 Release.
!ENDIF

!IF "$(CFG)" != "Regshot - Win32 Release" && "$(CFG)" != "Regshot - Win32 Release Unicode" && "$(CFG)" != "Regshot - x64 Release" && "$(CFG)" != "Regshot - x64 Release Unicode"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "regshot_wdk.mak" CFG="Regshot - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Regshot - Win32 Release" (based on "Application (.exe)")
!MESSAGE "Regshot - Win32 Release Unicode" (based on "Application (.exe)")
!MESSAGE "Regshot - x64 Release" (based on "Application (.exe)")
!MESSAGE "Regshot - x64 Release Unicode" (based on "Application (.exe)")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF


#######################
##  Global settings  ##
#######################

ProjectName=Regshot

SRCDIR=.\src
OUTDIR=$(VS_OUTPUT)bin\WDK\$(ProjectName)\$(PlatformName)\$(ConfigurationName)
INTDIR=$(VS_OUTPUT)obj\WDK\$(ProjectName)\$(PlatformName)\$(ConfigurationName)

LD=link.exe
LDOBJS= \
	"$(INTDIR)\fileshot.obj" \
	"$(INTDIR)\language.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\output.obj" \
	"$(INTDIR)\regshot.obj" \
	"$(INTDIR)\setup.obj" \
	"$(INTDIR)\ui.obj" \
	"$(INTDIR)\winmain.obj" \
	"$(INTDIR)\regshot.res"


######################
##  Configurations  ##
######################

!IF  "$(CFG)" == "Regshot - Win32 Release"

PlatformName=Win32
ConfigurationName=Release
TargetName=$(ProjectName)-x86-ANSI
CFLAGS=/O2 /GL /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_MBCS" /FD /EHsc /MT /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W4 /nologo /c /Zi /TC /errorReport:prompt
RFLAGS=/d "WIN32" /d "_M_IX86" /d "TARGETFILENAME=$(TargetName).exe" /l 0x409 /fo"$(INTDIR)\regshot.res"
LDLIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
LDFLAGS=/OUT:"$(OUTDIR)\$(TargetName).exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /DEBUG /PDB:"$(OUTDIR)\$(TargetName).pdb" /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE /OPT:REF /OPT:ICF /LTCG /RELEASE /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT

!ELSEIF  "$(CFG)" == "Regshot - Win32 Release Unicode"

PlatformName=Win32
ConfigurationName=Release_Unicode
TargetName=$(ProjectName)-x86-Unicode
CFLAGS=/O2 /GL /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /FD /EHsc /MT /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W4 /nologo /c /Zi /TC /errorReport:prompt
RFLAGS=/d "WIN32" /d "_M_IX86" /d "TARGETFILENAME=$(TargetName).exe" /d "_UNICODE" /d "UNICODE" /l 0x409 /fo"$(INTDIR)\regshot.res"
LDLIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
LDFLAGS=/OUT:"$(OUTDIR)\$(TargetName).exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /DEBUG /PDB:"$(OUTDIR)\$(TargetName).pdb" /SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE /OPT:REF /OPT:ICF /LTCG /RELEASE /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT

!ELSEIF  "$(CFG)" == "Regshot - x64 Release"

PlatformName=x64
ConfigurationName=Release
TargetName=$(ProjectName)-x64-ANSI
CFLAGS=/O2 /GL /D "_WIN64" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_MBCS" /FD /EHsc /MT /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W4 /nologo /c /Zi /TC /errorReport:prompt
RFLAGS=/d "_WIN64" /d "_M_AMD64" /d "TARGETFILENAME=$(TargetName).exe" /l 0x409 /fo"$(INTDIR)\regshot.res"
LDLIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
LDFLAGS=/OUT:"$(OUTDIR)\$(TargetName).exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /DEBUG /PDB:"$(OUTDIR)\$(TargetName).pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /RELEASE /DYNAMICBASE /NXCOMPAT /MACHINE:X64 /ERRORREPORT:PROMPT

!ELSEIF  "$(CFG)" == "Regshot - x64 Release Unicode"

PlatformName=x64
ConfigurationName=Release_Unicode
TargetName=$(ProjectName)-x64-Unicode
CFLAGS=/O2 /GL /D "_WIN64" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /FD /EHsc /MT /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W4 /nologo /c /Zi /TC /errorReport:prompt
RFLAGS=/d "_WIN64" /d "_M_AMD64" /d "TARGETFILENAME=$(TargetName).exe" /d "_UNICODE" /d "UNICODE" /l 0x409 /fo"$(INTDIR)\regshot.res"
LDLIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
LDFLAGS=/OUT:"$(OUTDIR)\$(TargetName).exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /DEBUG /PDB:"$(OUTDIR)\$(TargetName).pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /RELEASE /DYNAMICBASE /NXCOMPAT /MACHINE:X64 /ERRORREPORT:PROMPT

!ENDIF


################
###  Targets  ##
################

ALL : "$(OUTDIR)\$(TargetName).exe"

CLEAN :
	@ECHO Cleaning...
	-@erase $(LDOBJS)
	-@erase "$(INTDIR)\vc*.idb"
	-@erase "$(INTDIR)\vc*.pdb"
	-@rmdir "$(INTDIR)"
	-@erase "$(OUTDIR)\$(TargetName).exe"
	-@erase "$(OUTDIR)\$(TargetName).pdb"
	-@rmdir "$(OUTDIR)"

PREBUILD $(SRCDIR)\revision.h :
	@ECHO Generating revision number...
	$(SRCDIR)\update_rev.bat

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

"$(OUTDIR)\$(TargetName).exe" : PREBUILD "$(OUTDIR)" $(LDOBJS)
    $(LD) $(LDLIBS) $(EXTRA_LDLIBS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(LDOBJS)

REBUILD : CLEAN ALL


##############
###  Rules  ##
##############

# adapted from http://msdn.microsoft.com/en-us/library/cx06ysxh%28v=vs.90%29.aspx

{$(SRCDIR)}.c{$(INTDIR)}.obj :
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) /c $<

{$(SRCDIR)}.rc{$(INTDIR)}.res :
	$(RC) $(RFLAGS) $(EXTRA_RFLAGS) /r $<


###########################
##  Source Dependencies  ##
###########################

$(SRCDIR)\global.h : \
	"$(SRCDIR)\resource.h"\


$(SRCDIR)\version.h : \
	"$(SRCDIR)\version.rc.h"\


$(SRCDIR)\version.rc.h : \
	"$(SRCDIR)\revision.h"\


$(SRCDIR)\revision.h : \
	"$(SRCDIR)\revision.h.in"\


$(SRCDIR)\fileshot.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\language.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\misc.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\output.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\regshot.c : \
	"$(SRCDIR)\global.h"\
	"$(SRCDIR)\version.h"\


$(SRCDIR)\setup.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\ui.c : \
	"$(SRCDIR)\global.h"\


$(SRCDIR)\winmain.c : \
	"$(SRCDIR)\global.h"\
	"$(SRCDIR)\version.h"\


$(SRCDIR)\regshot.rc : \
	"$(SRCDIR)\res\Regshot.exe.manifest"\
	"$(SRCDIR)\res\regshot.ico"\
	"$(SRCDIR)\resource.h"\
	"$(SRCDIR)\version.rc.h"\


###########################
##  Object Dependencies  ##
###########################

$(LDOBJS) : \
	"$(INTDIR)"\

