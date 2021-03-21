[$$SETUP$$]
Comment=;
Extension=.unl
OutSeparateObjs=0
Logger=1

[$$HEADER$$]
{{##LINEFEED}}
Title={{##TITLE}}
Log=1
{{##COMMENT}} NewFileLog=1
{{##COMMENT}} LogOnlyErrors=1
{{##COMMENT}} Simulation=1
DeleteReadOnly={{##DELETEREADONLY}}
DeleteRecurse={{##DELETERECURSE}}

[$$DATALINES$$]
RegDefaultVal="{Standard}"
VALADDNormal=            ; Registry=+"{{##WHOLEKEYNAME}}","{{##VALNAME}}"=
VALADDShort =            ; Registry=+"{{##WHOLEKEYNAME}}","{{##VALNAME}}"
VALCHANGENormal=         ; Registry="{{##WHOLEKEYNAME}}","{{##VALNAME}}"=
VALCHANGEShort=          ; Registry="{{##WHOLEKEYNAME}}","{{##VALNAME}}"
VALDELETENormal=         Registry="{{##WHOLEKEYNAME}}","{{##VALNAME}}"=
VALDELETEShort=          Registry="{{##WHOLEKEYNAME}}","{{##VALNAME}}"
KEYADDNormal=            ; Registry=+"{{##WHOLEKEYNAME}}"
KEYDELETENormal=         Registry="{{##WHOLEKEYNAME}}"
FILEADDNormal=           ; File=+"{{##FILENAME}}"
FILEADDDeleteReadOnly=   ; File=+"{{##FILENAME}}"
FILECHANGENormal=        ; File="{{##FILENAME}}"
FILEDELETENormal=        File="{{##FILENAME}}"
FILEDELETEDeleteReadOnly=File="{{##FILENAME}}"
DIRCHANGENormal=         ; Directory="{{##DIRNAME}}"
DIRADDNormal=            ; Directory=+"{{##DIRNAME}}"
DIRADDDeleteRecurse=     ; Directory=+"{{##DIRNAME}}"
DIRDELETENormal=         Directory="{{##DIRNAME}}"
DIRDELETEDeleteRecurse=  Directory="{{##DIRNAME}}"

