[$$SETUP$$]
Extension=.nsi
NoVals=0
LogUNLOrder=0
LogEnvironmentStrings=0
OutSeparateObjs=0

[$$HEADER$$] 
!include "MUI2.nsh"
{{##LINEFEED}}
Name "{{##SCRIPTNAME}}"
OutFile "{{##OUTFILE}}.exe"
RequestExecutionLevel admin
Unicode True
ShowInstDetails Show
InstallDir "$EXEDIR"
{{##LINEFEED}}
{{##COMMENT}}--------------------------------
{{##COMMENT}} Pages
{{##COMMENT}}--------------------------------
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
{{##LINEFEED}}
{{##COMMENT}}--------------------------------
{{##COMMENT}} Languages
{{##COMMENT}}--------------------------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"

[$$STARTREGSECTION$$] 
{{##LINEFEED}}
Section "Registry"
{{##COMMENT}}----------------------------------------------------------
{{##COMMENT}} Switch LogSet On if NSIS_CONFIG_LOG build setting is used
{{##COMMENT}}  LogSet On
{{##COMMENT}}----------------------------------------------------------
{{##BLANK}} SetRegView 64

[$$ENDREGSECTION$$] 
{{##LINEFEED}}
SectionEnd

[$$STARTFSSECTION$$] 
{{##LINEFEED}}
Section "Filesystem"
{{##COMMENT}}----------------------------------------------------------
{{##COMMENT}} Switch LogSet On if NSIS_CONFIG_LOG build setting is used
{{##COMMENT}}  LogSet On
{{##COMMENT}}----------------------------------------------------------

[$$ENDFSSECTION$$] 
{{##LINEFEED}}
SectionEnd

[$$DATALINES$$]
RegDefaultVal=""
VALADDNormal=            {{##BLANK}} WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALADDShort=             {{##BLANK}} WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALCHANGENormal=         {{##BLANK}} WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALCHANGEShort=          {{##BLANK}} WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALDELETENormal=         {{##BLANK}} DeleteRegValue {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"
VALDELETEShort=          {{##BLANK}} DeleteRegValue {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"
KEYADDNormal=            {{##BLANK}} WriteRegNone {{##ROOTKEYNAME}} "{{##KEYNAME}}" ""
KEYDELETENormal=         {{##BLANK}} DeleteRegKey {{##ROOTKEYNAME}} "{{##KEYNAME}}"
FILEADDNormal=           {{##BLANK}} File "/oname={{##FILENAME}}" "{{##FILENAME}}"
FILEDELETENormal=        {{##BLANK}} Delete "{{##FILENAME}}"
FILEDELETEDeleteReadOnly={{##BLANK}} Delete "{{##FILENAME}}"
DIRADDNormal=            {{##BLANK}} CreateDirectory "{{##DIRNAME}}"
DIRDELETENormal=         {{##BLANK}} RMDir "{{##DIRNAME}}"
DIRDELETEDeleteRecurse=  {{##BLANK}} RMDir /r "{{##DIRNAME}}"
; Type specific macros
VALADDNormalQWORD=       ;  WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALADDShortQWORD=        ;  WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALCHANGENormalQWORD=    ;  WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}
VALCHANGEShortQWORD=     ;  WriteReg{{##VALTYPE}} {{##ROOTKEYNAME}} "{{##KEYNAME}}" "{{##VALNAME}}"{{##BLANK}}

[$$REGPREFS$$]
NameREG_SZ=Str
NameREG_EXPAND_SZ=ExpandStr
NameREG_MULTI_SZ=MultiStr /REGEDIT5
NameREG_DWORD_LITTLE_ENDIAN=DWORD
NameREG_DWORD_BIG_ENDIAN=DWORD
NameREG_QWORD_LITTLE_ENDIAN=QWORD
NameREG_NONE=None
NameREG_BINARY=Bin
PrefixREG_SZ=
PrefixREG_EXPAND_SZ=
PrefixREG_MULTI_SZ=
PrefixREG_DWORD_LITTLE_ENDIAN=
PrefixREG_DWORD_BIG_ENDIAN=
PrefixREG_QWORD_LITTLE_ENDIAN=
PrefixREG_NONE=
PrefixREG_BINARY=