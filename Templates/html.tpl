[$$SETUP$$]
Extension=.htm
NoVals=0
Logger=1

[$$HEADER$$] 

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

[$$TABLEHEAD$$]
<tr class="c">
{{##BLANK}} <td><a name="c"></a>&nbsp;</td>
</tr>
<tr class="h">
{{##BLANK}} <td>{{##PARTCOUNTTEXT}} {{##PARTCOUNT}}</td>
</tr>

[$$STARTREGSECTION$$] 
<table id="hive" cellspacing="1">

[$$STARTFILEADDPART$$]

[$$STARTDIRADDPART$$]

[$$STARTFILEDELPART$$]

[$$STARTDIRDELPART$$]

[$$FOOTER$$]
{{##BLANK}} </BODY>
</html>

[$$CSS$$]
body, table  { background-color: #FFFFFF; font-family: Tahoma; font-size: 10pt }
div.m        { background-color: #FFFFE0; width: 100%; text-align: center; border: 1px solid #808080; padding-top: 1px; padding-bottom: 3px }
h2           { font-size: 11pt; text-align: center }
td           { vertical-align: top; padding-left: 8px; padding-right: 8px; padding-top: 2px; padding-bottom: 2px }
#info        { border: 1px solid #E0E0E0; background-color: #F8F8F4 }
#info tr.a   { background-color: #FFFFFF }
#info tr.b   { background-color: #F0F0EC }
#info tr.h   { background-color: #DBE8F0; text-align: center; font-weight: bold }
#info td.a   { text-align: left }
#info td.b   { text-align: right }
#info td.c   { text-align: justify }
#info td.h   { text-align: center }
#info span.i { font-style: italic; color: #800000 }
#hive        { width: 100% }
#hive tr.a   { background-color: #FFFFFF }
#hive tr.b   { background-color: #F4F4F4 }
#hive tr.c   { background-color: #FFFFFF; font-family: Courier New }
#hive tr.d   { background-color: #F0F4F4; font-family: Courier New }
#hive tr.e   { background-color: #DAEAD5; font-weight: bold }
#hive tr.f   { background-color: #FFFFFF; font-family: Courier New }
#hive tr.g   { background-color: #F4F8F4; font-family: Courier New }
#hive tr.h   { background-color: #DBE8F0; font-weight: bold }
#hive span.r { color: #FF0000 }
#file        { width: 100% }
#file tr.a   { background-color: #FFFFFF }
#file tr.b   { background-color: #F4F8FC }
#file tr.h   { background-color: #DBE8F0; font-weight: bold }
.hash td     { font-family: Courier New; font-size: 9pt; padding-top: 0px; padding-bottom: 0px; text-align:right }
a            { color: #0000FF }
a:active     { color: #FF0000 }
a:hover      { color: #FF0000 }
a:link       { color: #0000FF }
a:visited    { color: #800080 }
