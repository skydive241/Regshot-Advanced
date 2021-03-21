[$$SETUP$$]
Extension=.iss
UseLongRegHead=0
LogUNLOrder=0
OutSeparateObjs=0
NoVals=0
LogEnvironmentStrings=0

[$$HEADER$$] 
#define MyAppName "Regshot's uninstaller for ""{{##TITLE}}""
#define MyAppVersion "1.0"
{{##LINEFEED}}
{{##LEFTBRACKET}}Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
ArchitecturesAllowed=x64 arm64 ia64
CreateAppDir=no
OutputBaseFilename={{##SCRIPTNAME}}
Compression=lzma
SolidCompression=yes
Uninstallable=no
SetupLogging=True
OutputDir={{##OUTDIR}}
{{##LINEFEED}}
{{##LEFTBRACKET}}Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
{{##LINEFEED}}
{{##LEFTBRACKET}}Types]
Name: "full"; Description: "Full maintenance"; Flags: iscustom
Name: "filesystem"; Description: "Filesystem maintenance"
Name: "registry"; Description: "Registry maintenance"
{{##LINEFEED}}
{{##LEFTBRACKET}}Components]
Name: "Filesystem"; Types: full filesystem; Description: "Filesystem maintenance"
Name: "Registry"; Types: full registry; Description: "Registry maintenance"

[$$DATALINES$$]
RegDefaultVal=""
MultiSzStringDivider={break}
;MultiSzStringDivider={break}
;Root: "HKLM"; Subkey: "SOFTWARE\Microsoft\Windows\Windows Error Reporting\TermReason\19476"; ValueName: "Terminator"; Components: registry ; Flags: deletevalue
;Root: "HKCU"; Subkey: "SOFTWARE\Seifert\WinDirStat\Settings"; Flags: deletekey; Components: registry 
;Root: "HKU"; Subkey: "S-1-5-19\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\Wpad\34-31-c4-66-c2-38"; ValueType: binary; ValueName: "WpadDecisionTime"; Components: registry ; ValueData: 
VALADDNormal=            Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; ValueType: {{##VALTYPE}}; Components: registry; ValueData:{{##BLANK}}
VALADDShort=             Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; ValueType: {{##VALTYPE}}; Components: registry; ValueData:{{##BLANK}}
VALCHANGENormal=         Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; ValueType: {{##VALTYPE}}; Components: registry; ValueData:{{##BLANK}}
VALCHANGEShort=          Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; ValueType: {{##VALTYPE}}; Components: registry; ValueData:{{##BLANK}}
VALDELETENormal=         Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; Components: registry ; Flags: deletevalue
VALDELETEShort =         Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; ValueName: "{{##VALNAME}}"; Components: registry ; Flags: deletevalue
KEYADDNormal=            Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; Components: registry
KEYDELETENormal=         Root: "{{##ROOTKEYNAME}}"; Subkey: "{{##KEYNAME}}"; Flags: deletekey; Components: registry
;Type: files; Name: "{{##FILENAME}}"; Components: filesystem 
FILEADDNormal=           Source: "{{##FILENAME}}"; DestDir:"{{##DIROFFILENAME}}"; Components: filesystem
FILEDELETENormal=        Type: files; Name: "{{##FILENAME}}"; Components: filesystem
FILEDELETEDeleteReadOnly=Type: files; Name: "{{##FILENAME}}"; Components: filesystem
DIRADDNormal=            Name: "{{##DIRNAME}}"; Components: filesystem
DIRDELETENormal=         Type: dirifempty; Name: "{{##DIRNAME}}"; Components: filesystem
DIRDELETEDeleteRecurse=  Type: filesandordirs; Name: "{{##DIRNAME}}"; Components: filesystem

[$$REGPREFS$$]
NameREG_SZ=string
NameREG_EXPAND_SZ=expandsz
NameREG_MULTI_SZ=multisz
NameREG_DWORD_LITTLE_ENDIAN=dword
NameREG_DWORD_BIG_ENDIAN=dword
NameREG_QWORD_LITTLE_ENDIAN=qword
NameREG_NONE=none
NameREG_BINARY=binary
PrefixREG_SZ=
PrefixREG_EXPAND_SZ=
PrefixREG_MULTI_SZ=
PrefixREG_DWORD_LITTLE_ENDIAN=
PrefixREG_DWORD_BIG_ENDIAN=
PrefixREG_QWORD_LITTLE_ENDIAN=
PrefixREG_NONE=
PrefixREG_BINARY=

[$$STARTREGSECTION$$] 
{{##LINEFEED}}
{{##LEFTBRACKET}}Registry]

[$$STARTFILEADDPART$$]
{{##LINEFEED}}
{{##LEFTBRACKET}}Files]

[$$STARTDIRADDPART$$]
{{##LINEFEED}}
{{##LEFTBRACKET}}Dirs]

[$$STARTFILEDELPART$$]
{{##LINEFEED}}
{{##LEFTBRACKET}}InstallDelete]

[$$STARTDIRDELPART$$]
{{##LINEFEED}}
{{##LEFTBRACKET}}InstallDelete]

[$$FOOTER$$]
{{##LEFTBRACKET}}Messages]
ReadyLabel1=[name].
ReadyLabel2b=Click (Un)install to continue.
ButtonInstall=(Un)install
