/*
    Copyright 2021-     Patrick (skydive241@gmx.de)

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

#include "global.h"

// Some strings used to write to UNL file
LPTSTR lpszUNLEnclosing         = TEXT("\"");
LPTSTR lpszUNLPartsDivider      = TEXT(",");

LPTSTR lpszUNLKeyAdd            = TEXT("Registry=");
LPTSTR lpszUNLKeyDel            = TEXT("; Registry=+");
LPTSTR lpszUNLValAdd            = TEXT("Registry=");
LPTSTR lpszUNLValDel            = TEXT("; Registry=+");
LPTSTR lpszUNLValModi           = TEXT("; Registry=");
LPTSTR lpszUNLValDefault        = TEXT("{Standard}");

LPTSTR lpszUNLDirAdd            = TEXT("Directory=");
LPTSTR lpszUNLDirDel            = TEXT("; Directory=+");
LPTSTR lpszUNLDirModi           = TEXT("; Directory=");
LPTSTR lpszUNLFileAdd           = TEXT("File=");
LPTSTR lpszUNLFileDel           = TEXT("; File=+");
LPTSTR lpszUNLFileModi          = TEXT("; File=");

// Some strings used to write to BAT file
LPTSTR lpszBATEnclosing         = TEXT("\"");
LPTSTR lpszBATPartsDivider      = TEXT(" ");
LPTSTR lpszBATKeyDelete         = TEXT("REG DELETE");
LPTSTR lpszBATValDelete         = TEXT("REG DELETE");
LPTSTR lpszBATFileDelete        = TEXT("DEL");
LPTSTR lpszBATDirDelete         = TEXT("RMDIR");

// Some strings used to write to ISS file
LPTSTR lpszREGKeySeparator      = TEXT("\r\n");
LPTSTR lpszREGKeyEnclosingLeft  = TEXT("[");
LPTSTR lpszREGKeyEnclosingRight = TEXT("]\r\n");
LPTSTR lpszREGValueEnclosing    = TEXT("\"");
LPTSTR lpszREGVal4Default       = TEXT("");
LPTSTR lpszREGVal5Default       = TEXT("");
LPTSTR lpszREGValDelete         = TEXT("=-");
LPTSTR lpszREGPartsDivider      = TEXT("=");

// Some strings used to write to ISS file
LPTSTR lpszISSEnclosing         = TEXT("\"");
LPTSTR lpszISSPartsDivider      = TEXT("; ");

LPTSTR lpszISSValueTypeString   = TEXT("string");
LPTSTR lpszISSValueTypeExpandsz = TEXT("expandsz");
LPTSTR lpszISSValueTypeMultisz  = TEXT("multisz");
LPTSTR lpszISSValueTypeDword    = TEXT("dword");
LPTSTR lpszISSValueTypeQword    = TEXT("qword");
LPTSTR lpszISSValueTypeNone     = TEXT("none");
LPTSTR lpszISSValueTypeBinary   = TEXT("binary");
LPTSTR lpszISSRoot              = TEXT("Root: ");
LPTSTR lpszISSSubkey            = TEXT("Subkey: ");
LPTSTR lpszISSValueType         = TEXT("ValueType: ");
LPTSTR lpszISSValueName         = TEXT("ValueName: ");
LPTSTR lpszISSValueData         = TEXT("ValueData: ");
LPTSTR lpszISSFlags             = TEXT("Flags: ");
LPTSTR lpszISSDeletekey         = TEXT("deletekey");
LPTSTR lpszISSDeletevalue       = TEXT("deletevalue");
LPTSTR lpszISSCreatevalue       = TEXT("createvalueifdoesntexist");
LPTSTR lpszISSValDefault        = TEXT("");
LPTSTR lpszISSFileAdd           = TEXT("Source: ");
LPTSTR lpszISSDirAdd            = TEXT("Name: ");
LPTSTR lpszISSDestDir           = TEXT("DestDir:");
LPTSTR lpszISSFileDelete        = TEXT("Type: files; Name: ");
LPTSTR lpszISSDirDelete         = TEXT("Type: dirifempty; Name: ");
LPTSTR lpszISSFileOrDirDelete   = TEXT("Type: filesandordirs; Name: ");
LPTSTR lpszISSComponentRegistry = TEXT("; Components: registry ");
LPTSTR lpszISSComponentFilesystem = TEXT("; Components: filesystem ");

LPTSTR lpszNSIPartsDivider      = TEXT(" ");
LPTSTR lpszNSIEnclosing         = TEXT("\"");
LPTSTR lpszNSIValDefault        = TEXT("");
LPTSTR lpszNSIValueTypeString   = TEXT("  WriteRegStr ");
LPTSTR lpszNSIValueTypeExpandsz = TEXT("  WriteRegExpandStr ");
LPTSTR lpszNSIValueTypeMultisz  = TEXT("  WriteRegMultiStr /REGEDIT5 ");
LPTSTR lpszNSIValueTypeDword    = TEXT("  WriteRegDWORD ");
LPTSTR lpszNSIValueTypeQword    = TEXT(";  WriteRegQWORD ");
LPTSTR lpszNSIValueTypeNone     = TEXT("  WriteRegNone ");
LPTSTR lpszNSIValueTypeBinary   = TEXT("  WriteRegBin ");
LPTSTR lpszNSIDeletevalue       = TEXT("  DeleteRegValue ");
LPTSTR lpszNSIDeletekey         = TEXT("  DeleteRegKey ");
LPTSTR lpszNSIAddkeyStr         = TEXT("  WriteRegStr ");
LPTSTR lpszNSIFileAdd           = TEXT("  File \"/oname=");
LPTSTR lpszNSIFileDelete        = TEXT("  Delete ");
LPTSTR lpszNSIDirAdd            = TEXT("  CreateDirectory ");
LPTSTR lpszNSIDirDelete         = TEXT("  RMDir ");
// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR CorrectISSOutputString(LPTSTR lpszText)
{
    LPTSTR pos = NULL;
    int i = 0;

    if (lpszText != NULL)
        pos = _tcschr(lpszText, _T('{'));
    while (pos) {
        i++;
        pos = _tcschr(++pos, _T('{'));
    }

    if (i > 0) {
        LPTSTR lpszOutputStringCorrected = MYALLOC0((_tcslen(lpszText) + i + 1) * sizeof(TCHAR));
        i = 0;

        for (int j = 0; j <= _tcslen(lpszText); j++) {
            if (lpszOutputStringCorrected + j + i != NULL)
                _tcsncpy(lpszOutputStringCorrected + j + i, lpszText + j, 1);
            if (_tcsncmp(lpszText + j, _TEXT("{"), 1) == 0) {
                if (lpszOutputStringCorrected + j + i + 1 != NULL)
                    _tcsncpy(lpszOutputStringCorrected + j + i + 1, _TEXT("{"), 1);
                i++;
            }
        }
        MYFREE(lpszText);
        return lpszOutputStringCorrected;
    }

    return lpszText;
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildNSIOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType, BOOL bDeInstall)
{
    LPTSTR lpszOutputString = NULL;
    LPTSTR lpszTempString;
    size_t cchStringLen = 1;    // terminating \0
    LPTSTR pos = NULL;

    if ((nActionType == VALADD) || (nActionType == VALDEL) || (nActionType == VALMODI)) {
        pos = _tcschr(lpszText, _T('\\'));
        if (pos != NULL)
            _tcscpy(pos, TEXT("\0"));
// DeleteRegValue HKLM "Software\My Company\My Software" "some value"
// WriteRegStr HKCU "Software\My Company\My Software" "" "Test"
        cchStringLen += 6 * _tcslen(lpszNSIEnclosing) + _tcslen(lpszText) +
            2 * _tcslen(lpszNSIPartsDivider) + (pos != NULL ? _tcslen(++pos) : 0);
        if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
            cchStringLen += _tcslen(((LPVALUECONTENT)lpContent)->lpszValueName);
        else
            cchStringLen += _tcslen(lpszNSIValDefault);
        
        if ((bDeInstall && (nActionType == VALADD)) || (!bDeInstall && (nActionType == VALDEL))) {
            cchStringLen += _tcslen(lpszNSIDeletevalue);
        }
        if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
            switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
            case REG_SZ:
                cchStringLen += _tcslen(lpszNSIValueTypeString);
                break;
            case REG_EXPAND_SZ:
                cchStringLen += _tcslen(lpszNSIValueTypeExpandsz);
                break;
            case REG_MULTI_SZ:
                cchStringLen += _tcslen(lpszNSIValueTypeMultisz);
                break;
            case REG_DWORD_LITTLE_ENDIAN:
            case REG_DWORD_BIG_ENDIAN:
                cchStringLen += _tcslen(lpszNSIValueTypeDword);
                break;
            case REG_QWORD_LITTLE_ENDIAN:
                cchStringLen += _tcslen(lpszNSIValueTypeQword);
                break;
            case REG_NONE:
                cchStringLen += _tcslen(lpszNSIValueTypeNone);
                break;
            default:
                cchStringLen += _tcslen(lpszNSIValueTypeBinary);
                break;
            }
            cchStringLen += _tcslen(lpszNSIPartsDivider);
        }

        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
                switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
                case REG_SZ:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeString);
                    break;
                case REG_EXPAND_SZ:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeExpandsz);
                    break;
                case REG_MULTI_SZ:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeMultisz);
                    break;
                case REG_DWORD_LITTLE_ENDIAN:
                case REG_DWORD_BIG_ENDIAN:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeDword);
                    break;
                case REG_QWORD_LITTLE_ENDIAN:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeQword);
                    break;
                case REG_NONE:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeNone);
                    break;
                default:
                    _tcscpy(lpszOutputString, lpszNSIValueTypeBinary);
                    break;
                }
            }
            else
                _tcscpy(lpszOutputString, lpszNSIDeletevalue);

            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszNSIPartsDivider);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
            if (pos != NULL)
                _tcscat(lpszOutputString, pos);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
            _tcscat(lpszOutputString, lpszNSIPartsDivider);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
            if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
                _tcscat(lpszOutputString, ((LPVALUECONTENT)lpContent)->lpszValueName);
            else
                _tcscat(lpszOutputString, lpszNSIValDefault);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
//Root: "HKCU"; Subkey: "RegTypes"; ValueType: dword; ValueName: """REG_DWORD_BIG_ENDIAN_EMPTY"""; ValueData: "hex(5):0x00000000"
            if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
                _tcscat(lpszOutputString, lpszNSIPartsDivider);
            }
        }
    }
    
    else if ((nActionType == KEYADD) || (nActionType == KEYDEL)) {
        pos = _tcschr(lpszText, _T('\\'));
        if (pos != NULL)
            _tcscpy(pos, TEXT("\0"));                               
// DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Example2"
// WriteRegStr HKCU "Software\My Company\My Software" ""
        cchStringLen += _tcslen(lpszText) + 2 * _tcslen(lpszNSIEnclosing)
            + (pos != NULL ? _tcslen(++pos) : 0) + _tcslen(lpszNSIPartsDivider);
        // Add key
        if ((!bDeInstall && (nActionType == KEYADD)) || (bDeInstall && (nActionType == KEYDEL))) {
            cchStringLen += _tcslen(lpszNSIAddkeyStr) + 2 * _tcslen(lpszNSIPartsDivider) + 4 * _tcslen(lpszNSIEnclosing);
        }
        // Delete key
        else {
            cchStringLen += _tcslen(lpszNSIDeletekey);
        }
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
        // Add key
            if ((!bDeInstall && (nActionType == KEYADD)) || (bDeInstall && (nActionType == KEYDEL))) 
                _tcscpy(lpszOutputString, lpszNSIAddkeyStr);
            else 
            // Delete key
                _tcscpy(lpszOutputString, lpszNSIDeletekey);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszNSIPartsDivider);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
            if (pos != NULL)
                _tcscat(lpszOutputString, pos);
            _tcscat(lpszOutputString, lpszNSIEnclosing);
        // Add key
            if ((!bDeInstall && (nActionType == KEYADD)) || (bDeInstall && (nActionType == KEYDEL))) {
                _tcscat(lpszOutputString, lpszNSIPartsDivider);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszNSIPartsDivider);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
            }
        }
    }

    else if ((nActionType == DIRDEL) || (nActionType == DIRMODI) || (nActionType == DIRADD) || 
        (nActionType == FILEDEL) || (nActionType == FILEMODI) || (nActionType == FILEADD)) {
        
        lpszTempString = GetWholeFileName(lpContent, 0, TRUE);
        cchStringLen += 2 * _tcslen(lpszNSIEnclosing) + _tcslen(lpszTempString);

        if ((bDeInstall && (nActionType == DIRADD)) || (!bDeInstall && (nActionType == DIRDEL))) {
        //  RMDir /r "C:\Program Files (x86)\My Program\test"
            cchStringLen += _tcslen(lpszNSIDirDelete);
            if (fDeleteDirNotEmpty)
                cchStringLen += 3;
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszNSIDirDelete);
                if (fDeleteDirNotEmpty) {
                    cchStringLen += 3;
                    _tcscat(lpszOutputString, TEXT("/r "));
                }
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
            }
        }
        else if ((bDeInstall && (nActionType == DIRDEL)) || (!bDeInstall && (nActionType == DIRADD))) {
        //  CreateDirectory "c:\Temp2"
            cchStringLen += _tcslen(lpszNSIDirAdd);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszNSIDirAdd);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
            }
        }
        else if ((bDeInstall && (nActionType == FILEADD)) || (!bDeInstall && (nActionType == FILEDEL))) {
        // Delete "C:\Program Files (x86)\My Program\MyProg.chm"
            cchStringLen += _tcslen(lpszNSIFileDelete);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszNSIFileDelete);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
            }
        }
        else if ((bDeInstall && (nActionType == FILEDEL)) || (!bDeInstall && (nActionType == FILEADD))) {
        //  File "/oname=c:\temp\example2.nsi" "C:\Temp\nsis-3.06.1-setup\Examples\example2.nsi"
                cchStringLen += _tcslen(lpszNSIFileAdd) + _tcslen(lpszNSIPartsDivider) +
                3 * _tcslen(lpszNSIEnclosing) + 2 * _tcslen(lpszTempString);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszNSIFileAdd);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszNSIPartsDivider);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszNSIEnclosing);
            }
        }
        
        MYFREE(lpszTempString);
    }

//    lpszOutputString = CorrectISSOutputString(lpszOutputString);

    return lpszOutputString;
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildISSOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType, BOOL bDeInstall)
{
    LPTSTR lpszOutputString = NULL;
    LPTSTR lpszTempString;
    size_t cchStringLen = 1;    // terminating \0
    LPTSTR pos = NULL;

//    LPTSTR lpszISSComponentRegistry = TEXT("; Components: registry ");
//    LPTSTR lpszISSComponentFilesystem = TEXT("; Components: filesystem ");

    if ((nActionType == VALADD) || (nActionType == VALDEL) || (nActionType == VALMODI)) {
        pos = _tcschr(lpszText, _T('\\'));
        if (pos != NULL)
            _tcscpy(pos, TEXT("\0"));
//Root: "HKLM"; Subkey: "xxx"; ValueName: "aaaaa"; Flags: deletevalue
        cchStringLen += _tcslen(lpszISSRoot) + 6 * _tcslen(lpszISSEnclosing) + _tcslen(lpszText) +
            2 * _tcslen(lpszISSPartsDivider) + _tcslen(lpszISSSubkey) + _tcslen(lpszISSValueName) + (pos != NULL ? _tcslen(++pos) : 0) + _tcslen(lpszISSComponentRegistry);
        if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
            cchStringLen += _tcslen(((LPVALUECONTENT)lpContent)->lpszValueName);
        else
            cchStringLen += _tcslen(lpszISSValDefault);
        if ((bDeInstall && (nActionType == VALADD)) || (!bDeInstall && (nActionType == VALDEL))) {
            cchStringLen += _tcslen(lpszISSPartsDivider) + _tcslen(lpszISSFlags) + _tcslen(lpszISSDeletevalue);
        }
        if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
            cchStringLen += _tcslen(lpszISSValueType) + _tcslen(lpszISSPartsDivider);
            cchStringLen += _tcslen(lpszISSValueData) + _tcslen(lpszISSPartsDivider);
            
            switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
            case REG_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeString);
                break;
            case REG_EXPAND_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeExpandsz);
                break;
            case REG_MULTI_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeMultisz);
                break;
            case REG_DWORD_LITTLE_ENDIAN:
            case REG_DWORD_BIG_ENDIAN:
                cchStringLen += _tcslen(lpszISSValueTypeDword);
                break;
            case REG_QWORD_LITTLE_ENDIAN:
                cchStringLen += _tcslen(lpszISSValueTypeQword);
                break;
            case REG_NONE:
                cchStringLen += _tcslen(lpszISSValueTypeNone);
                break;
            default:
                cchStringLen += _tcslen(lpszISSValueTypeBinary);
                break;
            }
        }

        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszISSRoot);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszISSPartsDivider);
            _tcscat(lpszOutputString, lpszISSSubkey);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            if (pos != NULL)
                _tcscat(lpszOutputString, pos);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszISSPartsDivider);
            if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
                _tcscat(lpszOutputString, lpszISSValueType);
                
                switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
                case REG_SZ:
                    _tcscat(lpszOutputString, lpszISSValueTypeString);
                    break;
                case REG_EXPAND_SZ:
                    _tcscat(lpszOutputString, lpszISSValueTypeExpandsz);
                    break;
                case REG_MULTI_SZ:
                    _tcscat(lpszOutputString, lpszISSValueTypeMultisz);
                    break;
                case REG_DWORD_LITTLE_ENDIAN:
                case REG_DWORD_BIG_ENDIAN:
                    _tcscat(lpszOutputString, lpszISSValueTypeDword);
                    break;
                case REG_QWORD_LITTLE_ENDIAN:
                    _tcscat(lpszOutputString, lpszISSValueTypeQword);
                    break;
                case REG_NONE:
                    _tcscat(lpszOutputString, lpszISSValueTypeNone);
                    break;
                default:
                    _tcscat(lpszOutputString, lpszISSValueTypeBinary);
                    break;
                }
                _tcscat(lpszOutputString, lpszISSPartsDivider);
            }
            _tcscat(lpszOutputString, lpszISSValueName);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
                _tcscat(lpszOutputString, ((LPVALUECONTENT)lpContent)->lpszValueName);
            else
                _tcscat(lpszOutputString, lpszISSValDefault);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszISSComponentRegistry);
            if ((bDeInstall && (nActionType == VALADD)) || (!bDeInstall && (nActionType == VALDEL))) {
                _tcscat(lpszOutputString, lpszISSPartsDivider);
                _tcscat(lpszOutputString, lpszISSFlags);
                _tcscat(lpszOutputString, lpszISSDeletevalue);
            }
            //Root: "HKCU"; Subkey: "RegTypes"; ValueType: dword; ValueName: """REG_DWORD_BIG_ENDIAN_EMPTY"""; ValueData: "hex(5):0x00000000"
            if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
                _tcscat(lpszOutputString, lpszISSPartsDivider);
                _tcscat(lpszOutputString, lpszISSValueData);
            }
        }
    }
    
    else if ((nActionType == KEYADD) || (nActionType == KEYDEL)) {
        pos = _tcschr(lpszText, _T('\\'));
        if (pos != NULL)
            _tcscpy(pos, TEXT("\0"));                               
//Root: "HKCR"; Subkey: "xxx"[; Flags: deletekey]
        cchStringLen += _tcslen(lpszISSRoot) + 4 * _tcslen(lpszISSEnclosing) + _tcslen(lpszText) +
             _tcslen(lpszISSPartsDivider) + _tcslen(lpszISSSubkey) + (pos != NULL ? _tcslen(++pos) : 0) + _tcslen(lpszISSComponentRegistry);
        if ((bDeInstall && (nActionType == KEYADD)) || (!bDeInstall && (nActionType == KEYDEL))) {
            cchStringLen += _tcslen(lpszISSPartsDivider) + _tcslen(lpszISSFlags) + _tcslen(lpszISSDeletekey);
        }
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszISSRoot);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            _tcscat(lpszOutputString, lpszISSPartsDivider);
            _tcscat(lpszOutputString, lpszISSSubkey);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            if (pos != NULL)
                _tcscat(lpszOutputString, pos);
            _tcscat(lpszOutputString, lpszISSEnclosing);
            if ((bDeInstall && (nActionType == KEYADD)) || (!bDeInstall && (nActionType == KEYDEL))) {
                _tcscat(lpszOutputString, lpszISSPartsDivider);
                _tcscat(lpszOutputString, lpszISSFlags);
                _tcscat(lpszOutputString, lpszISSDeletekey);
            }
            _tcscat(lpszOutputString, lpszISSComponentRegistry);
        }
    }

    else if ((nActionType == DIRDEL) || (nActionType == DIRMODI) || (nActionType == DIRADD) || 
        (nActionType == FILEDEL) || (nActionType == FILEMODI) || (nActionType == FILEADD)) {
        
        lpszTempString = GetWholeFileName(lpContent, 0, TRUE);
        cchStringLen += 2 * _tcslen(lpszISSEnclosing) + _tcslen(lpszTempString) + _tcslen(lpszISSComponentFilesystem);

        if ((bDeInstall && (nActionType == DIRADD)) || (!bDeInstall && (nActionType == DIRDEL))) {
        //    [InstallDelete]
        //    Type: dirifempty; Name: "C:\Program Files (x86)\My Program\test"
        //    Type: filesandordirs; Name: "C:\Program Files (x86)\My Program\test"
            if (fDeleteDirNotEmpty)
                cchStringLen += _tcslen(lpszISSFileOrDirDelete);
            else
                cchStringLen += _tcslen(lpszISSDirDelete);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                if (fDeleteDirNotEmpty)
                    _tcscpy(lpszOutputString, lpszISSFileOrDirDelete);
                else
                    _tcscpy(lpszOutputString, lpszISSDirDelete);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszISSComponentFilesystem);
            }
        }
        else if ((bDeInstall && (nActionType == DIRDEL)) || (!bDeInstall && (nActionType == DIRADD))) {
        //    [Dirs]
        //    Name: "c:\Temp2"
            cchStringLen += _tcslen(lpszISSDirAdd) + _tcslen(lpszISSComponentFilesystem);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszISSDirAdd);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszISSComponentFilesystem);
            }
        }
        else if ((bDeInstall && (nActionType == FILEADD)) || (!bDeInstall && (nActionType == FILEDEL))) {
        //    [InstallDelete]
        //    Type: files; Name: "C:\Program Files (x86)\My Program\MyProg.chm"
            cchStringLen += _tcslen(lpszISSFileDelete) + _tcslen(lpszISSComponentFilesystem);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszISSFileDelete);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszISSComponentFilesystem);
            }
        }
        else if ((bDeInstall && (nActionType == FILEDEL)) || (!bDeInstall && (nActionType == FILEADD))) {
        //    [Files]
        //    Source: "C:\Temp\win32.chm"; DestDir: "c:\temp"
            cchStringLen += _tcslen(lpszISSFileAdd) + _tcslen(lpszISSPartsDivider) + _tcslen(lpszISSDestDir) + 
                2 * _tcslen(lpszISSEnclosing) + _tcslen(lpszTempString) + _tcslen(lpszISSComponentFilesystem);
            lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
            if (lpszOutputString != NULL) {
                _tcscpy(lpszOutputString, lpszISSFileAdd);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszISSPartsDivider);
                _tcscat(lpszOutputString, lpszISSDestDir);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                pos = _tcsrchr(lpszTempString, _T('\\'));
                if (pos != NULL)
                    _tcscpy(pos, TEXT("\0"));
                cchStringLen += _tcslen(lpszTempString);
                _tcscat(lpszOutputString, lpszTempString);
                _tcscat(lpszOutputString, lpszISSEnclosing);
                _tcscat(lpszOutputString, lpszISSComponentFilesystem);
            }
        }
        MYFREE(lpszTempString);
    }

    lpszOutputString = CorrectISSOutputString(lpszOutputString);

    //if (lpszOutputString != NULL)
    //    pos = _tcschr(lpszOutputString, _T('{'));
    //int i = 0;
    //while (pos) {
    //    i++;
    //    pos = _tcschr(++pos, _T('{'));
    //}
    //if (i > 0) {
    //    LPTSTR lpszOutputStringCorrected = MYALLOC0((_tcslen(lpszOutputString) + i + 1) * sizeof(TCHAR));
    //    i = 0;
    //    
    //    for (int j = 0; j <= _tcslen(lpszOutputString); j++) {
    //        if (lpszOutputStringCorrected + j + i != NULL)
    //            _tcsncpy(lpszOutputStringCorrected + j + i, lpszOutputString+j, 1);
    //        if (_tcsncmp(lpszOutputString + j, _TEXT("{"), 1) == 0) {
    //            if (lpszOutputStringCorrected + j + i + 1 != NULL)
    //                _tcsncpy(lpszOutputStringCorrected + j + i + 1, _TEXT("{"), 1);
    //            i++;
    //        }
    //    }
    //    MYFREE(lpszOutputString);
    //    return lpszOutputStringCorrected;
    //}
    
    return lpszOutputString;
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildREGOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType, BOOL bSuppressKey, BOOL bDeInstall)
{
    LPTSTR lpszOutputString = NULL;
    //LPTSTR lpszTempString;
    size_t cchStringLen = 1;    // terminating \0
    //LPTSTR pos = NULL;
// TODO: L�ngenberechnung!!!
    if ((nActionType == VALADD) || (nActionType == VALDEL) || (nActionType == VALMODI)) {
//Root: "HKLM"; Subkey: "xxx"; ValueName: "aaaaa"; Flags: deletevalue
        if (!bSuppressKey) {
            cchStringLen += _tcslen(lpszREGKeySeparator) + _tcslen(lpszREGKeyEnclosingLeft) + _tcslen(lpszText) +
                _tcslen(lpszREGKeyEnclosingRight);
        } 
        cchStringLen += 2 * _tcslen(lpszREGValueEnclosing);
        
        if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
            cchStringLen += _tcslen(((LPVALUECONTENT)lpContent)->lpszValueName);
        else
            cchStringLen += _tcslen((fREG5 ? lpszREGVal5Default : lpszREGVal4Default));
        if ((bDeInstall && (nActionType == VALADD)) || (!bDeInstall && (nActionType == VALDEL))) {
            cchStringLen += _tcslen(lpszREGValDelete);
        }
        if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
            cchStringLen += _tcslen(lpszREGPartsDivider);
            
            switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
            case REG_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeString);
                break;
            case REG_EXPAND_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeExpandsz);
                break;
            case REG_MULTI_SZ:
                cchStringLen += _tcslen(lpszISSValueTypeMultisz);
                break;
            case REG_DWORD_LITTLE_ENDIAN:
            case REG_DWORD_BIG_ENDIAN:
                cchStringLen += _tcslen(lpszISSValueTypeDword);
                break;
            case REG_QWORD_LITTLE_ENDIAN:
                cchStringLen += _tcslen(lpszISSValueTypeQword);
                break;
            case REG_NONE:
                cchStringLen += _tcslen(lpszISSValueTypeNone);
                break;
            default:
                cchStringLen += _tcslen(lpszISSValueTypeBinary);
                break;
            }
        }

        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            if (!bSuppressKey) {
                _tcscat(lpszOutputString, lpszREGKeySeparator);
                _tcscat(lpszOutputString, lpszREGKeyEnclosingLeft);
                _tcscat(lpszOutputString, lpszText);
                _tcscat(lpszOutputString, lpszREGKeyEnclosingRight);
            }
            _tcscat(lpszOutputString, lpszREGValueEnclosing);
            if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
                _tcscat(lpszOutputString, ((LPVALUECONTENT)lpContent)->lpszValueName);
            else
                _tcscat(lpszOutputString, (fREG5 ? lpszREGVal5Default : lpszREGVal4Default));
            _tcscat(lpszOutputString, lpszREGValueEnclosing);
            if ((bDeInstall && (nActionType == VALADD)) || (!bDeInstall && (nActionType == VALDEL))) {
                _tcscat(lpszOutputString, lpszREGValDelete);
            }
            //Root: "HKCU"; Subkey: "RegTypes"; ValueType: dword; ValueName: """REG_DWORD_BIG_ENDIAN_EMPTY"""; ValueData: "hex(5):0x00000000"
            if ((bDeInstall && (nActionType == VALDEL)) || (!bDeInstall && (nActionType == VALADD)) || (nActionType == VALMODI)) {
            }
        }
    }
    
    else if ((nActionType == KEYADD) || (nActionType == KEYDEL)) {
//Root: "HKCR"; Subkey: "xxx"[; Flags: deletekey]
        cchStringLen += _tcslen(lpszREGKeyEnclosingLeft) + _tcslen(lpszText) + _tcslen(lpszREGKeyEnclosingRight);
        if ((bDeInstall && (nActionType == KEYADD)) || (!bDeInstall && (nActionType == KEYDEL))) {
            cchStringLen += 1;
        }
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscat(lpszOutputString, lpszREGKeyEnclosingLeft);
            if ((bDeInstall && (nActionType == KEYADD)) || (!bDeInstall && (nActionType == KEYDEL))) {
                _tcscat(lpszOutputString, TEXT("-"));
            }
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszREGKeyEnclosingRight);
        }
    }

//    lpszOutputString = CorrectISSOutputString(lpszOutputString);

    return lpszOutputString;
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildUNLOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType)
{
    LPTSTR lpszOutputString = NULL;
    LPTSTR lpszTempString;
    size_t cchStringLen = 1;    // terminating \0
    LPTSTR lpszPrefix = NULL;

    if ((nActionType == VALADD) || (nActionType == VALDEL) || (nActionType == VALMODI)) {
        if (nActionType == VALADD) {
            lpszPrefix = lpszUNLValAdd;
        }
        else if (nActionType == VALDEL) {
            lpszPrefix = lpszUNLValDel;
        }
        else if (nActionType == VALMODI) {
            lpszPrefix = lpszUNLValModi;
        }
        
        cchStringLen += _tcslen(lpszPrefix) + 4 * _tcslen(lpszUNLEnclosing) + _tcslen(lpszText) + _tcslen(lpszUNLPartsDivider);
        if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
            cchStringLen += _tcslen(((LPVALUECONTENT)lpContent)->lpszValueName);
        else
            cchStringLen += _tcslen(lpszUNLValDefault);
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));

        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszPrefix);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
            _tcscat(lpszOutputString, lpszUNLPartsDivider);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
            if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL)
                _tcscat(lpszOutputString, ((LPVALUECONTENT)lpContent)->lpszValueName);
            else
                _tcscat(lpszOutputString, lpszUNLValDefault);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
        }
    }
    
    else if ((nActionType == KEYADD) || (nActionType == KEYDEL)) {
        if (nActionType == KEYADD) {
            lpszPrefix = lpszUNLKeyAdd;
        }
        else if (nActionType == KEYDEL) {
            lpszPrefix = lpszUNLKeyDel;
        }
        cchStringLen += _tcslen(lpszPrefix) + 2 * _tcslen(lpszUNLEnclosing) + _tcslen(lpszText);
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszPrefix);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
        }
    }

    else if ((nActionType == DIRDEL) || (nActionType == DIRMODI) || (nActionType == DIRADD) || 
        (nActionType == FILEDEL) || (nActionType == FILEMODI) || (nActionType == FILEADD)) {
        lpszTempString = GetWholeFileName(lpContent, 0, TRUE);
        cchStringLen += _tcslen(lpszTempString) + 2 * _tcslen(lpszUNLEnclosing);
        if (nActionType == DIRDEL) {
            lpszPrefix = lpszUNLDirDel;
        }
        else if (nActionType == DIRMODI) {
            lpszPrefix = lpszUNLDirModi;
        }
        else if (nActionType == DIRADD) {
            lpszPrefix = lpszUNLDirAdd;
        }
        else if (nActionType == FILEDEL) {
            lpszPrefix = lpszUNLFileDel;
        }
        else if (nActionType == FILEMODI) {
            lpszPrefix = lpszUNLFileModi;
        }
        else if (nActionType == FILEADD) {
            lpszPrefix = lpszUNLFileAdd;
        }
        cchStringLen += _tcslen(lpszPrefix);
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszPrefix);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
            _tcscat(lpszOutputString, lpszTempString);
            _tcscat(lpszOutputString, lpszUNLEnclosing);
        }
        MYFREE(lpszTempString);
    }
    
    return lpszOutputString;
}
// ----------------------------------------------------------------------

// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildBATOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType)
{
    LPTSTR lpszOutputString = NULL;
    LPTSTR lpszTempString;
    size_t cchStringLen = 1;

    if (nActionType == VALADD) {
        //REG DELETE HKCU\RegTypes /v REG_DWORD_EMPTY /f
        cchStringLen += _tcslen(lpszBATValDelete) + 3 * _tcslen(lpszBATPartsDivider) + _tcslen(lpszText) + 4 + 2 * _tcslen(lpszBATEnclosing);
        if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL) {
            cchStringLen += _tcslen(lpszBATPartsDivider) + _tcslen(((LPVALUECONTENT)lpContent)->lpszValueName) + 2 * _tcslen(lpszBATEnclosing);
        }
        else {
            cchStringLen += 1;
        }
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszBATValDelete);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            _tcscat(lpszOutputString, TEXT("/v"));

            if (((LPVALUECONTENT)lpContent)->lpszValueName != NULL) {
                _tcscat(lpszOutputString, lpszBATPartsDivider);
                _tcscat(lpszOutputString, lpszBATEnclosing);
                _tcscat(lpszOutputString, ((LPVALUECONTENT)lpContent)->lpszValueName);
                _tcscat(lpszOutputString, lpszBATEnclosing);
            }
            else
                _tcscat(lpszOutputString, TEXT("e"));

            _tcscat(lpszOutputString, lpszBATPartsDivider);
            _tcscat(lpszOutputString, TEXT("/f"));
        }
    }

    else if (KEYADD == nActionType) {
        //REG DELETE HKCU\RegTypes /f
        cchStringLen += _tcslen(lpszBATKeyDelete) + 2 * _tcslen(lpszBATPartsDivider) + 2 * _tcslen(lpszBATEnclosing) + _tcslen(lpszText) + 2;
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszBATKeyDelete);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszText);
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            _tcscat(lpszOutputString, TEXT("/f"));
        }
    }
    
    else if (FILEADD == nActionType) {
        //DEL "C:\Users\All Users\Blumentals\phpxdebug\php722\libsasl.dll"
        lpszTempString = GetWholeFileName(lpContent, 0, TRUE);
        cchStringLen += _tcslen(lpszBATFileDelete) + _tcslen(lpszBATPartsDivider) + 2 * _tcslen(lpszBATEnclosing) + _tcslen(lpszTempString);
        if (fDeleteReadOnly) {
            cchStringLen += _tcslen(lpszBATPartsDivider) + 2;
        }
        
        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            _tcscpy(lpszOutputString, lpszBATFileDelete);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            if (fDeleteReadOnly) {
                _tcscat(lpszOutputString, TEXT("/F"));
                _tcscat(lpszOutputString, lpszBATPartsDivider);
            }
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszTempString);
            _tcscat(lpszOutputString, lpszBATEnclosing);
        }
        MYFREE(lpszTempString);
    }
    
    else if (DIRADD == nActionType) {
//        RMDIR "C:\Program Files (x86)\WeBuilder 2020\templates\ASP"
        lpszTempString = GetWholeFileName(lpContent, 0, TRUE);
        cchStringLen += _tcslen(lpszBATDirDelete) + _tcslen(lpszBATPartsDivider) + 2 * _tcslen(lpszBATEnclosing) + _tcslen(lpszTempString);
        if (fDeleteDirNotEmpty) {
            cchStringLen += 2 * _tcslen(lpszBATPartsDivider) + 4;
        }

        lpszOutputString = MYALLOC0(cchStringLen * sizeof(TCHAR));
        if (lpszOutputString != NULL) {
            //LPTSTR lpszBATDirDelete = TEXT("RMDIR");
            _tcscpy(lpszOutputString, lpszBATDirDelete);
            _tcscat(lpszOutputString, lpszBATPartsDivider);
            if (fDeleteReadOnly) {
                _tcscat(lpszOutputString, TEXT("/S"));
                _tcscat(lpszOutputString, lpszBATPartsDivider);
                _tcscat(lpszOutputString, TEXT("/Q"));
                _tcscat(lpszOutputString, lpszBATPartsDivider);
            }
            _tcscat(lpszOutputString, lpszBATEnclosing);
            _tcscat(lpszOutputString, lpszTempString);
            _tcscat(lpszOutputString, lpszBATEnclosing);
        }
        MYFREE(lpszTempString);
    }
    
    return lpszOutputString;
}
