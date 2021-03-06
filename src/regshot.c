/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
    Copyright 1999-2003,2007,2011 TiANWEi
    Copyright 2004 tulipfan

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
#include "version.h"

BOOL ConvertDataToBinaryFormat(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, DWORD nConversionType, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType);
BOOL ConvertMultiSZ2Strings(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType);
BOOL ConvertQWORD2LongString(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, LPQWORD lpQword, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType);
BOOL ConvertDWORD2LongString(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, LPDWORD lpDword, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType);

LPTSTR lpszResultFileBaseName = REGSHOT_RESULT_FILE;

LPTSTR lpszFilter =
#ifdef _UNICODE
    TEXT("Regshot Unicode hive files (*.hivu)\0*.hivu\0All files\0*.*\0\0");
#else
    TEXT("Regshot ANSI hive files (*.hiv)\0*.hiv\0All files\0*.*\0\0");
#endif

// SBCS/MBCS signature (even in Unicode builds for backwards compatibility)
char szRegshotFileSignatureSBCS[] = "REGSHOTHIVE";
char szRegshotFileSignatureUTF16[] = "REGSHOTHIV2";  // use a number to define a file format not compatible with older releases (e.g. "3" could be UTF-32 or Big Endian)
#ifdef _UNICODE
#define szRegshotFileSignature szRegshotFileSignatureUTF16
#else
#define szRegshotFileSignature szRegshotFileSignatureSBCS
#endif

LPTSTR lpszRegshotFileDefExt =
#ifdef _UNICODE
    TEXT("hivu");
#else
    TEXT("hiv");
#endif

//LPTSTR lpszValueDataIsNULL = TEXT(": (NULL!)");
LPTSTR lpszValueDataIsNULL = TEXT("");

LPTSTR lpszEmpty = TEXT("");

REGSHOT Shot1;
REGSHOT Shot2;

FILEHEADER fileheader;
FILEEXTRADATA fileextradata;
SAVEKEYCONTENT sKC;
SAVEVALUECONTENT sVC;

LPBYTE lpFileBuffer;
LPTSTR lpStringBuffer;
size_t nStringBufferSize;
LPBYTE lpDataBuffer;
size_t nDataBufferSize;
size_t nSourceSize;

#define MAX_SIGNATURE_LENGTH 12
#define REGSHOT_READ_BLOCK_SIZE 8192

// Compare result
COMPRESULTS CompareResult;
BOOL bNoOutput = FALSE;

DWORD NBW;               // that is: NumberOfBytesWritten

FILETIME ftLastWrite;    // Filetime struct

// in regshot.ini, "UseLongRegHead" to control this
// 1.6 using long name, so in 1.8.1 add an option
LPTSTR lpszHKLMShort = TEXT("HKLM");
LPTSTR lpszHKLMLong  = TEXT("HKEY_LOCAL_MACHINE");
#define CCH_HKLM_LONG  18

LPTSTR lpszHKUShort  = TEXT("HKU");
LPTSTR lpszHKULong   = TEXT("HKEY_USERS");
#define CCH_HKU_LONG  10

LPTSTR lpszHKCUShort = TEXT("HKCU");
LPTSTR lpszHKCULong = TEXT("HKEY_CURRENT_USER");
#define CCH_HKCU_LONG  17

LONG nErrNo;


// ----------------------------------------------------------------------
// Get whole registry key name from KEYCONTENT
// ----------------------------------------------------------------------
LPTSTR GetWholeKeyName(LPKEYCONTENT lpStartKC, BOOL fUseLongNames)
{
    LPKEYCONTENT lpKC;
    LPTSTR lpszName;
    LPTSTR lpszTail = NULL;
    size_t cchName;
    LPTSTR lpszKeyName;

    cchName = 0;
    for (lpKC = lpStartKC; NULL != lpKC; lpKC = lpKC->lpFatherKC) {
        if (NULL != lpKC->lpszKeyName) {
            if ((fUseLongNames) && (lpszHKLMShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKLM_LONG;
            } else if ((fUseLongNames) && (lpszHKUShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKU_LONG;
            } else if ((fUseLongNames) && (lpszHKCUShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKCU_LONG;
            } else {
                cchName += lpKC->cchKeyName;
            }
            cchName++;  // +1 char for backslash or NULL char
        }
    }
    if (0 == cchName) {  // at least create an empty string with NULL char
        cchName++;
    }

    lpszName = MYALLOC(cchName * sizeof(TCHAR));
    if (lpszName) {
        lpszTail = &lpszName[cchName - 1];
        lpszTail[0] = (TCHAR)'\0';
    }

    for (lpKC = lpStartKC; NULL != lpKC; lpKC = lpKC->lpFatherKC) {
        if (NULL != lpKC->lpszKeyName) {
            if ((fUseLongNames) && (lpszHKLMShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKLM_LONG;
                lpszKeyName = lpszHKLMLong;
            } else if ((fUseLongNames) && (lpszHKUShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKU_LONG;
                lpszKeyName = lpszHKULong;
            } else if ((fUseLongNames) && (lpszHKCUShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKCU_LONG;
                lpszKeyName = lpszHKCULong;
            } else {
                cchName = lpKC->cchKeyName;
                lpszKeyName = lpKC->lpszKeyName;
            }
            if (lpszTail - cchName != NULL) {
                _tcsncpy(lpszTail -= cchName, lpszKeyName, cchName);
                if (lpszTail > lpszName) {
                    *--lpszTail = (TCHAR)'\\';
                }
            }
        }
    }

    return lpszName;
}


// ----------------------------------------------------------------------
// Get whole value name from VALUECONTENT
// ----------------------------------------------------------------------
LPTSTR GetWholeValueName(LPVALUECONTENT lpVC, BOOL fUseLongNames)
{
    LPKEYCONTENT lpKC;
    LPTSTR lpszName;
    LPTSTR lpszTail = NULL;
    size_t cchName;
    size_t cchValueName;
    LPTSTR lpszKeyName;

    cchValueName = 0;
    if (NULL != lpVC->lpszValueName) {
        cchValueName = lpVC->cchValueName;
    }
    cchName = cchValueName + 1;  // +1 char for NULL char

    for (lpKC = lpVC->lpFatherKC; NULL != lpKC; lpKC = lpKC->lpFatherKC) {
        if (NULL != lpKC->lpszKeyName) {
            if ((fUseLongNames) && (lpszHKLMShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKLM_LONG;
            } else if ((fUseLongNames) && (lpszHKUShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKU_LONG;
            } else if ((fUseLongNames) && (lpszHKCUShort == lpKC->lpszKeyName)) {
                cchName += CCH_HKCU_LONG;
            } else {
                cchName += lpKC->cchKeyName;
            }
            cchName++;  // +1 char for backslash
        }
    }

    lpszName = MYALLOC0(cchName * sizeof(TCHAR));
    if (lpszName) {
        lpszTail = &lpszName[cchName - 1];
        lpszTail[0] = (TCHAR)'\0';
    }

    if (NULL != lpVC->lpszValueName) {
        if (lpszTail - cchValueName != NULL) {
            _tcsncpy(lpszTail -= cchValueName, lpVC->lpszValueName, cchValueName);
        }
    }
    if (lpszTail > lpszName) {
        *--lpszTail = (TCHAR)'\\';
    }

    for (lpKC = lpVC->lpFatherKC; NULL != lpKC; lpKC = lpKC->lpFatherKC) {
        if (NULL != lpKC->lpszKeyName) {
            if ((fUseLongNames) && (lpszHKLMShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKLM_LONG;
                lpszKeyName = lpszHKLMLong;
            } else if ((fUseLongNames) && (lpszHKUShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKU_LONG;
                lpszKeyName = lpszHKULong;
            } else if ((fUseLongNames) && (lpszHKCUShort == lpKC->lpszKeyName)) {
                cchName = CCH_HKCU_LONG;
                lpszKeyName = lpszHKCULong;
            } else {
                cchName = lpKC->cchKeyName;
                lpszKeyName = lpKC->lpszKeyName;
            }
            if (lpszTail - cchName != NULL) {
                _tcsncpy(lpszTail -= cchName, lpszKeyName, cchName);
            }
            if (lpszTail > lpszName) {
                *--lpszTail = (TCHAR)'\\';
            }
        }
    }

    return lpszName;
}

// ----------------------------------------------------------------------
// Transform value content data from binary into string according
// to given conversion type (may differ from original type)
// Called by GetWholeValueData()
// ----------------------------------------------------------------------
LPTSTR TransData(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nConversionType, DWORD nActionType, size_t iNameLength, int iOutputType)
{
    LPDWORD lpDword;
    LPQWORD lpQword;
    DWORD nDwordCpu;

    LPTSTR lpszEnclosing = TEXT("\"");
    LPTSTR lpszDivider = TEXT(",");
    LPTSTR lpszValueType = MYALLOC(9 * sizeof(TCHAR));
    if (lpszValueType == NULL)
        return lpszValueData[0];

    _tcscpy(lpszValueType, lpszEmpty);
    if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL)) {
        _tcscpy(lpszDivider, TEXT(" "));
        _tcscpy(lpszValueType, lpszEmpty);
    }
    else if ((iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_NSI_INSTALL)) {
        _tcscpy(lpszDivider, lpszEmpty);
        _tcscpy(lpszValueType, lpszEmpty);
    }
    else {
        _tcscpy(lpszDivider, TEXT(","));
    }

    switch (nConversionType) {
        case REG_NONE:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, TEXT("$"));
            else if ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL))
                _tcscat(lpszValueType, lpszEmpty);
            else
                _tcscat(lpszValueType, TEXT("hex(0):"));
            break;
        case REG_SZ:
            _tcscat(lpszValueType, lpszEmpty);
            break;
        case REG_EXPAND_SZ:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, TEXT("$"));
            else if ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL))
                _tcscat(lpszValueType, lpszEmpty);
            else
                _tcscat(lpszValueType, TEXT("hex(2):"));
            break;
        case REG_BINARY:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, lpszEmpty);
            else if ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL))
                _tcscat(lpszValueType, lpszEmpty);
            else
                _tcscat(lpszValueType, TEXT("hex:"));
            //    _tcscat(lpszValueType, TEXT("$"));
            break;
        case REG_DWORD:
//        case REG_DWORD_LITTLE_ENDIAN:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, TEXT("$"));
            else if ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL))
                _tcscat(lpszValueType, TEXT("0x"));
            else
                _tcscat(lpszValueType, TEXT("dword:"));
            break;
        case REG_DWORD_BIG_ENDIAN:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, TEXT("$"));
            else
                _tcscat(lpszValueType, TEXT("hex(5):"));
            break;
//        case REG_LINK:
//            break;
        case REG_MULTI_SZ:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL))
                _tcscat(lpszValueType, TEXT("$"));
            else if ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL)) {
                _tcscat(lpszValueType, lpszEmpty);
                _tcscpy(lpszDivider, TEXT(","));
            }
            else
                _tcscat(lpszValueType, TEXT("hex(7):"));
            break;
//        case REG_RESOURCE_LIST:
//            break;
//        case REG_FULL_RESOURCE_DESCRIPTOR:
//            break;
//        case REG_RESOURCE_REQUIREMENTS_LIST:
//            break;
        case REG_QWORD:
            if ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_ISS_DEINSTALL)) {
                _tcscat(lpszValueType, TEXT("$"));
                _tcscpy(lpszDivider, lpszEmpty);
            }
            else
                _tcscat(lpszValueType, TEXT("hex(b):"));
//        case REG_QWORD_LITTLE_ENDIAN:
            break;
        }
    
#if 1 == __LITTLE_ENDIAN__
    // no REG_QWORD_BIG_ENDIAN yet
#elif
    QWORD nQwordCpu;
#endif

    lpszValueData[0] = NULL;
    lpDword = NULL;
    lpQword = NULL;
    if (NULL == lpVC->lpValueData) {
        lpszValueData[0] = MYALLOC((_tcslen(lpszValueType) + _tcslen(lpszValueDataIsNULL) + 1) * sizeof(TCHAR));
        if (lpszValueData[0] != NULL) {
            _tcscpy(lpszValueData[0], lpszValueType);
            _tcscat(lpszValueData[0], lpszValueDataIsNULL);
        }
    } else {
        DWORD cbData;
        DWORD ibCurrent;
        size_t nChars = 0;

        cbData = lpVC->cbData;

//"REG_NONE_EMPTY"=hex(0):00,00
//"REG_BINARY_EMPTY"=hex:
//"REG_DWORD_EMPTY"=dword:00000000
//"REG_DWORD_LITTLE_ENDIAN_EMPTY"=dword:00000000
//"REG_DWORD_BIG_ENDIAN_EMPTY"=hex(5):00,00,00,00
//"REG_QWORD_EMPTY"=hex(b):00,00,00,00,00,00,00,00
//"REG_SZ_EMPTY"=""
//"REG_EXPAND_SZ_EMPTY"=hex(2):00,00
//"REG_MULTI_SZ_EMPTY"=hex(7):00,00
//"REG_NONE"=hex(0):46,00,61,00,6b,00,65,00,53,00,74,00,72,00,69,00,6e,00,67,00,\
//  00,00
//"REG_BINARY"=hex:12,34,56,78
//"REG_DWORD"=dword:12345678
//"REG_DWORD_LITTLE_ENDIAN"=dword:12345678
//"REG_DWORD_BIG_ENDIAN"=hex(5):12,34,56,78
//"REG_QWORD"=hex(b):f0,de,bc,9a,78,56,34,12
//"REG_SZ"="String"
//"REG_EXPAND_SZ"=hex(2):53,00,74,00,72,00,69,00,6e,00,67,00,00,00
//"REG_MULTI_SZ"=hex(7):53,00,74,00,72,00,69,00,6e,00,67,00,31,00,00,00,53,00,74,\
//  00,72,00,69,00,6e,00,67,00,32,00,00,00,53,00,74,00,72,00,69,00,6e,00,67,00,\
//  33,00,00,00,00,00

 //REG_NONE                         ( 0ul ) 
 //REG_SZ                           ( 1ul ) 
 //REG_EXPAND_SZ                    ( 2ul ) 
 //REG_BINARY                       ( 3ul ) 
 //REG_DWORD                        ( 4ul ) 
 //REG_DWORD_LITTLE_ENDIAN          ( 4ul ) 
 //REG_DWORD_BIG_ENDIAN             ( 5ul ) 
 //REG_LINK                         ( 6ul ) 
 //REG_MULTI_SZ                     ( 7ul ) 
 //REG_RESOURCE_LIST                ( 8ul ) 
 //REG_FULL_RESOURCE_DESCRIPTOR     ( 9ul ) 
 //REG_RESOURCE_REQUIREMENTS_LIST   ( 10ul )
 //REG_QWORD                        ( 11ul )
 //REG_QWORD_LITTLE_ENDIAN          ( 11ul )
        size_t iOffset = _tcslen(lpszValueType);
        switch (nConversionType) {
            case REG_SZ:
                // string value that can be displayed as a string
                // format  "=\"<string>\"\0"
                nChars = iOffset + 2 * _tcslen(lpszEnclosing) + (size_t)cbData + 1;
                lpszValueData[0] = MYALLOC0(nChars * sizeof(TCHAR));
                // TODO: ISS-String-Korrekturen
                if (lpszValueData[0] != NULL) {
                    _tcscpy(lpszValueData[0], lpszValueType);
                    _tcscat(lpszValueData[0], lpszEnclosing);

                    LPTSTR strTemp = MYALLOC0((nChars + 1) * sizeof(TCHAR));
                    if (strTemp != NULL)
                        memcpy(strTemp, lpVC->lpValueData, cbData);

                    if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL)) {
                        strTemp = EscapeSpecialCharacters(strTemp, _T('"'), TEXT("\""));
                        strTemp = EscapeSpecialCharacters(strTemp, _T('{'), TEXT("{"));
                    }
                    else if ((iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_NSI_INSTALL)) {
                        strTemp = EscapeSpecialCharacters(strTemp, _T('"'), TEXT("$\\"));
                        strTemp = EscapeSpecialCharacters(strTemp, _T('%'), TEXT("$\\"));
                    }
                    else if ((iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_REG_INSTALL)) {
                        strTemp = EscapeSpecialCharacters(strTemp, _T('"'), TEXT("\\"));
                        strTemp = EscapeSpecialCharacters(strTemp, _T('\\'), TEXT("\\"));
                    }
                    
                    if (strTemp != NULL)
                        _tcscat(lpszValueData[0], strTemp);
                    
                    MYFREE(strTemp);

                    //else {
                    //    if (NULL != lpVC->lpValueData) {
                    //        memcpy(&lpszValueData[0][1 + iOffset], lpVC->lpValueData, cbData);
                    //    }
                    //}
                    _tcscat(lpszValueData[0], lpszEnclosing);
                }
                break;

            case REG_MULTI_SZ:
                if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL))
                    ConvertMultiSZ2Strings(lpszValueData, lpVC, nActionType, iNameLength, lpszValueType, lpszDivider, iOutputType);
                else
                    ConvertDataToBinaryFormat(lpszValueData, lpVC, nActionType, iNameLength, nConversionType, lpszValueType, lpszDivider, iOutputType);
                break;

#if 1 == __LITTLE_ENDIAN__
            case REG_DWORD_BIG_ENDIAN:
#elif
            case REG_DWORD_LITTLE_ENDIAN:
#endif
                // convert DWORD with different endianness
                lpDword = &nDwordCpu;
                for (ibCurrent = 0; ibCurrent < sizeof(DWORD); ibCurrent++) {
                    ((LPBYTE)&nDwordCpu)[ibCurrent] = lpVC->lpValueData[sizeof(DWORD) - 1 - ibCurrent];
                }
                if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL))
                    ConvertDWORD2LongString(lpszValueData, lpVC, lpDword, nActionType, iNameLength, lpszValueType, lpszDivider, iOutputType);
                else
                    ConvertDataToBinaryFormat(lpszValueData, lpVC, nActionType, iNameLength, nConversionType, lpszValueType, lpszDivider, iOutputType);
                break;

#if 1 == __LITTLE_ENDIAN__
            case REG_DWORD_LITTLE_ENDIAN:
#elif
            case REG_DWORD_BIG_ENDIAN:
#endif
                ConvertDWORD2LongString(lpszValueData, lpVC, lpDword, nActionType, iNameLength, lpszValueType, lpszDivider, iOutputType);
                break;

#if 1 == __LITTLE_ENDIAN__
                //case REG_QWORD_BIG_ENDIAN:
#elif
            case REG_QWORD_LITTLE_ENDIAN:
#endif
#if 1 == __LITTLE_ENDIAN__
                // no REG_QWORD_BIG_ENDIAN yet
#elif
                // convert QWORD with different endianness
                lpQword = &nQwordCpu;
                for (ibCurrent = 0; ibCurrent < sizeof(QWORD); i++) {
                    ((LPBYTE)&nQwordCpu)[ibCurrent] = lpVC->lpValueData[sizeof(QWORD) - ibCurrent - ibCurrent];
                }
#endif

#if 1 == __LITTLE_ENDIAN__
            case REG_QWORD_LITTLE_ENDIAN:
#elif
            //case REG_QWORD_BIG_ENDIAN:
#endif
                if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL))
                    ConvertQWORD2LongString(lpszValueData, lpVC, lpQword, nActionType, iNameLength, lpszValueType, lpszDivider, iOutputType);
                else
                    ConvertDataToBinaryFormat(lpszValueData, lpVC, nActionType, iNameLength, nConversionType, lpszValueType, lpszDivider, iOutputType);
                break;

            case REG_EXPAND_SZ:
            case REG_BINARY:
            case REG_NONE:
            default: {
                ConvertDataToBinaryFormat(lpszValueData, lpVC, nActionType, iNameLength, nConversionType, lpszValueType, lpszDivider, iOutputType);
            }
        }
    }

    return lpszValueData[0];
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
BOOL ConvertMultiSZ2Strings(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType)
{
    UNREFERENCED_PARAMETER(iOutputType);
    UNREFERENCED_PARAMETER(lpszDivider);
    UNREFERENCED_PARAMETER(iNameLength);
    UNREFERENCED_PARAMETER(lpszValueType);
    UNREFERENCED_PARAMETER(nActionType);

    //size_t nChars = 0;
    //size_t iOffset = _tcslen(lpszValueType);
    DWORD cbData = lpVC->cbData;
    size_t cchChars = 0;
    size_t cchActual;
    LPTSTR lpszSrc;
    LPTSTR lpszDst;
    size_t cchToGo;
    size_t cchString;

    // multi string value that can be displayed as strings
    // format  "=\"<string>\", \"<string>\", \"<string>\", ...\0"
    // see http://msdn.microsoft.com/en-us/library/windows/desktop/ms724884.aspx
    cchChars = 10 + 2 * (size_t)cbData;
    //                nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, cchChars, REGSHOT_BUFFER_BLOCK_BYTES);
    nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, cchChars, REGSHOT_BUFFER_BLOCK_BYTES);
    ZeroMemory(lpStringBuffer, nStringBufferSize);
    lpszDst = lpStringBuffer;
//    _tcscpy(lpszDst, lpszValueType);
//    _tcscpy(lpszDst, TEXT(": \""));
    if ((iOutputType != OUT_ISS_DEINSTALL) && (iOutputType != OUT_ISS_INSTALL)) {
        _tcscpy(lpszDst, TEXT("\""));
        lpszDst += 1;
    }
//    lpszDst += 3 + _tcslen(lpszValueType);
//    lpszDst += 3;
    cchActual = 0;
    if (NULL != lpVC->lpValueData) {
        lpszSrc = (LPTSTR)lpVC->lpValueData;
        cchToGo = cbData / sizeof(TCHAR);  // convert byte count to char count
        while ((cchToGo > 0) && (*lpszSrc)) {
            if (0 != cchActual) {
//                if ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL)) {
                if (lpszMultiSzStringDivider != NULL) {
                    //_tcscpy(lpszDst, TEXT("}break}"));
                    //lpszDst += 7;
                    //cchActual += 7;
                    _tcscpy(lpszDst, lpszMultiSzStringDivider);
                    lpszDst += _tcslen(lpszMultiSzStringDivider);
                    cchActual += _tcslen(lpszMultiSzStringDivider);
                }
                else {
                    _tcscpy(lpszDst, TEXT("\", \""));
                    lpszDst += 4;
                    cchActual += 4;
                }
            }
            cchString = _tcsnlen(lpszSrc, cchToGo);
            _tcsncpy(lpszDst, lpszSrc, cchString);
            lpszDst += cchString;
            cchActual += cchString;

            cchToGo -= cchString;
            if (cchToGo == 0) {
                break;
            }

            // account for null char
            lpszSrc += cchString + 1;
            cchToGo -= 1;
        }
    }
//    _tcsncpy(lpszDst, TEXT("\""), 1);
    if ((iOutputType != OUT_ISS_DEINSTALL) && (iOutputType != OUT_ISS_INSTALL)) {
        lpStringBuffer[cchActual + 1] = _T('\"');
    }
    lpStringBuffer[cchActual+2] = _T('\0');
    cchActual += 3 + 1 + 1;  // account for null char
    lpszValueData[0] = MYALLOC(cchActual * sizeof(TCHAR));
    if (lpszValueData[0] != NULL)
        _tcscpy(lpszValueData[0], lpStringBuffer);

    return TRUE;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
BOOL ConvertDWORD2LongString(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, LPDWORD lpDword, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType)
{
    UNREFERENCED_PARAMETER(iOutputType);
    UNREFERENCED_PARAMETER(lpszDivider);
    UNREFERENCED_PARAMETER(iNameLength);
    UNREFERENCED_PARAMETER(nActionType);
    
    size_t nChars = 0;
    size_t iOffset = _tcslen(lpszValueType);
    DWORD cbData = lpVC->cbData;

    // native DWORD that can be displayed as DWORD
    if (NULL == lpDword) {
        lpDword = (LPDWORD)lpVC->lpValueData;
    }
    // format  ": 0xXXXXXXXX\0"
    nChars = iOffset + 2 * (size_t)cbData + 1;
    lpszValueData[0] = MYALLOC0((nChars) * sizeof(TCHAR));
    if (lpszValueData[0] != NULL) {
        _tcscpy(lpszValueData[0], lpszValueType);
        if (NULL != lpVC->lpValueData) {
            _sntprintf(lpszValueData[0] + _tcslen(lpszValueType), (8 + 1), TEXT("%08X\0"), *lpDword);
        }
    }

    return TRUE;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
BOOL ConvertQWORD2LongString(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, LPQWORD lpQword, DWORD nActionType, size_t iNameLength, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType)
{
    UNREFERENCED_PARAMETER(iOutputType);
    UNREFERENCED_PARAMETER(lpszDivider);
    UNREFERENCED_PARAMETER(iNameLength);
    UNREFERENCED_PARAMETER(nActionType);

    size_t nChars = 0;
    size_t iOffset = _tcslen(lpszValueType);
    DWORD cbData = lpVC->cbData;
    
    // native QWORD that can be displayed as QWORD
    if (NULL == lpQword) {
        lpQword = (LPQWORD)lpVC->lpValueData;
    }
    // format  ": 0xXXXXXXXXXXXXXXXX\0"
    nChars = iOffset + 2 * (size_t)cbData + 1;
    lpszValueData[0] = MYALLOC0(nChars * sizeof(TCHAR));
    if (lpszValueData[0] != NULL) {
        _tcscpy(lpszValueData[0], lpszValueType);
        if (NULL != lpVC->lpValueData) {
            _sntprintf(lpszValueData[0] + _tcslen(lpszValueType), (16 + 1), TEXT("%016I64X\0"), *lpQword);
        }
    }

    return TRUE;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
BOOL ConvertDataToBinaryFormat(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, DWORD nConversionType, LPTSTR lpszValueType, LPTSTR lpszDivider, int iOutputType)
{
    DWORD ibCurrent;
    size_t nChars = 0;
    int iLineCount = 0, iBytesCurrentLine = 0;
    size_t iBytesCount = _tcslen(lpszValueType);
    size_t iOffset = _tcslen(lpszValueType);
    DWORD cbData = lpVC->cbData;
    LPTSTR lpszActualByte = MYALLOC0(3 * sizeof(TCHAR));
    int iTrailingZeroes = 0;

    // display value as hex bytes
    // format ":[ xx][ xx]...[ xx][ ...]\0"
    if (lpszValueData[0] == NULL) {
        nChars = iOffset + (2 + _tcslen(lpszDivider)) * (size_t)cbData + 4 + 12;
        lpszValueData[0] = MYALLOC0(nChars * sizeof(TCHAR));
        if (lpszValueData[0] != NULL)
            _tcscpy(lpszValueData[0], lpszValueType);
        lpszValueData[1] = NULL;
        for (ibCurrent = 0; ibCurrent < cbData; ibCurrent++) {
            if (cbData - ibCurrent >= 2) {
                // Zeilenaufbau .REG-File:
                // Schl�sselname komplett
                // Wertname plus Werte-Bytes bis max. 80 Zeichen, aber Werte-Name immer komplett plus EIN Werte-Byte
                // Wertezeilen 80 Zeichen
                // "{S38OS404-1Q43-42S2-9305-67QR0O28SP23}\\rkcybere.rkr"=hex:03,00,00,00,00,00,\
                // "{1NP14R77-02R7-4R5Q-O744-2RO1NR5198O7}\\FlfgrzCebcregvrfPbzchgreAnzr.rkr"=hex:03,\
                //   00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,5c,d7,dc,3b,00,00,00,00,00,00,\

                if (((iLineCount == 0) && (iBytesCount + iNameLength >= 77) && ibCurrent > 0) || (iBytesCount >= 75)) {
                    //                            if (iLineCount + 1 == MAX_RESULT_STRINGS)
                    if (iLineCount + 1 == nOutMaxResultLines)
                        break;
                    _sntprintf(lpszValueData[iLineCount] + iBytesCount, 2, TEXT("%s"), TEXT("\\\0"));
                    iLineCount++;
                    iBytesCurrentLine = 0;

                    lpszValueData[iLineCount] = MYALLOC0(nChars * sizeof(TCHAR));
                    lpszValueData[iLineCount + 1] = NULL;
                    iBytesCount = 2;
                    if ((iOutputType == OUT_UNL) && (nActionType != VALADD))
                        _sntprintf(lpszValueData[iLineCount], 2, TEXT("%s"), TEXT("; "));
                    else if (((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL)) && 
                        (nConversionType == REG_QWORD)) {
                        // QWORD not supported by nsis
                        _sntprintf(lpszValueData[iLineCount], 3, TEXT("%s"), TEXT(";  "));
                        iBytesCount++;
                    }
                    else
                        _sntprintf(lpszValueData[iLineCount], 2, TEXT("%s"), TEXT("  "));
                }

                // _sntprintf(lpszValueData[iLineCount] + (_tcslen(lpszValueType) + ((size_t)ibCurrent * 3)), 4, TEXT("%02X%s"), *(lpVC->lpValueData + ibCurrent), lpszDivider);
                _sntprintf(lpszActualByte, 2, TEXT("%02X"), *(lpVC->lpValueData + ibCurrent));
                lpszActualByte[2] = _T('\0');
//                _sntprintf(lpszValueData[iLineCount] + iBytesCount, 4, TEXT("%02X%s"), *(lpVC->lpValueData + ibCurrent), lpszDivider);
                if (lpszValueData[iLineCount] != NULL) {
                    _tcscat(lpszValueData[iLineCount], lpszActualByte);
                    _tcscat(lpszValueData[iLineCount], lpszDivider);
                }
                iTrailingZeroes = (_tcscmp(lpszActualByte, TEXT("00")) == 0 ? iTrailingZeroes + 1 : 0);
                iBytesCount += 2 + _tcslen(lpszDivider);
                iBytesCurrentLine++;
            }
            else {
                _sntprintf(lpszActualByte, 2, TEXT("%02X"), *(lpVC->lpValueData + ibCurrent));
                lpszActualByte[2] = _T('\0');
                if (lpszValueData[iLineCount] != NULL)
                    _tcscat(lpszValueData[iLineCount], lpszActualByte);
                //_sntprintf(lpszValueData[iLineCount] + iBytesCount, 4, TEXT("%02X"), *(lpVC->lpValueData + ibCurrent));
                iTrailingZeroes = (_tcscmp(lpszActualByte, TEXT("00")) == 0 ? iTrailingZeroes + 1 : 0);
                iBytesCurrentLine++;
            }
            // _sntprintf(lpszValueData[iLineCount] + (_tcslen(lpszValueType) + ((size_t)ibCurrent * 3)), 4, TEXT("%02X"), *(lpVC->lpValueData + ibCurrent));
        }
        // if (cbData != lpVC->cbData) {
        if (ibCurrent != lpVC->cbData) {
            // _sntprintf(lpszValueData[iLineCount] + (_tcslen(lpszValueType) + ((size_t)ibCurrent * 3)), 4, TEXT("...\0"));
            _sntprintf(lpszValueData[iLineCount] + (2 + ((size_t)iBytesCurrentLine * 3)), 4, TEXT("...\0"));
        } 
        else {
            // nsis needs 4 zeroes at the end of a multisz... workaround...
            if ((iTrailingZeroes < 4) && ((iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_NSI_DEINSTALL)) && (nConversionType == REG_MULTI_SZ)) {
                for (int i = 0; i < (4 - iTrailingZeroes); i++) {
                    if (lpszValueData[iLineCount] != NULL)
                        _tcscat(lpszValueData[iLineCount], TEXT(",00"));
                }
            }
        }
    }

    MYFREE(lpszActualByte);
    return TRUE;
}

// ----------------------------------------------------------------------
// Get value data from value content as string
// Check for special cases to call TransData() properly
// ----------------------------------------------------------------------
LPTSTR GetWholeValueData(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, int iOutputType)
{
    lpszValueData[0] = NULL;
    lpszValueData[1] = NULL;
    if (NULL == lpVC->lpValueData) {
//        lpszValueData = TransData(lpVC, REG_BINARY, fForOutput, iOutputType);
        lpszValueData[0] = TransData(lpszValueData, lpVC, REG_BINARY, nActionType, iNameLength, iOutputType);
    } else {
        DWORD cbData;
        size_t cchMax;
        size_t cchActual;

        cbData = lpVC->cbData;

        switch (lpVC->nTypeCode) {
            case REG_SZ:
            case REG_EXPAND_SZ:
            case REG_MULTI_SZ:
                // string values
                // check for hidden bytes after string[s]
                cchMax = cbData / sizeof(TCHAR);  // convert byte count to char count
                if (REG_MULTI_SZ == lpVC->nTypeCode) {
                    // search double null chars
                    for (cchActual = 0; cchActual < cchMax; cchActual++) {
                        if (0 != ((LPTSTR)lpVC->lpValueData)[cchActual]) {
                            continue;
                        }

                        cchActual++;
                        if (cchActual >= cchMax) {  // special case: incorrectly terminated
                            break;
                        }

                        if (0 != ((LPTSTR)lpVC->lpValueData)[cchActual]) {
                            continue;
                        }

                        // found
                        cchActual++;
                        break;
                    }
                } else {
                    cchActual = _tcsnlen((LPTSTR)lpVC->lpValueData, cchMax);
                    if (cchActual < cchMax) {
                        cchActual++;  // account for null char
                    }
                }
                // TODO: Pr�fung hier (REG_MULTI_SZ vs. REG_BINARY) �berdenken....
//                if ((cchActual * sizeof(TCHAR)) == cbData) {
                    lpszValueData[0] = TransData(lpszValueData, lpVC, lpVC->nTypeCode, nActionType, iNameLength, iOutputType);
//                } else {
//                    lpszValueData[0] = TransData(lpszValueData, lpVC, REG_BINARY, nActionType, iNameLength, iOutputType);
//                }
                break;

            case REG_DWORD_LITTLE_ENDIAN:
            case REG_DWORD_BIG_ENDIAN:
                // DWORD values
                if (sizeof(DWORD) == cbData) {
                    lpszValueData[0] = TransData(lpszValueData, lpVC, lpVC->nTypeCode, nActionType, iNameLength, iOutputType);
                } else {
                    lpszValueData[0] = TransData(lpszValueData, lpVC, REG_BINARY, nActionType, iNameLength, iOutputType);
                }
                break;

            case REG_QWORD_LITTLE_ENDIAN:
                //case REG_QWORD_BIG_ENDIAN:
                // QWORD values
                if (sizeof(QWORD) == cbData) {
                    lpszValueData[0] = TransData(lpszValueData, lpVC, lpVC->nTypeCode, nActionType, iNameLength, iOutputType);
                } else {
                    lpszValueData[0] = TransData(lpszValueData, lpVC, REG_BINARY, nActionType, iNameLength, iOutputType);
                }
                break;

            case REG_NONE:
                lpszValueData[0] = TransData(lpszValueData, lpVC, REG_NONE, nActionType, iNameLength, iOutputType);
                break;

            default:
                lpszValueData[0] = TransData(lpszValueData, lpVC, REG_BINARY, nActionType, iNameLength, iOutputType);
        }
    }

    return lpszValueData[0];
}


//-------------------------------------------------------------
// Routine to create new comparison result, distribute to different pointers
//-------------------------------------------------------------
VOID CreateNewResult(DWORD nActionType, LPVOID lpContentOld, LPVOID lpContentNew)
{
    LPCOMPRESULT lpCR;
    lpCR = (LPCOMPRESULT)MYALLOC0(sizeof(COMPRESULT));
    if (lpCR != NULL) {
        lpCR->lpContentOld = lpContentOld;
        lpCR->lpContentNew = lpContentNew;
    }

    LPCOMPRESULTNEW lpCRNew;
    lpCRNew = (LPCOMPRESULTNEW)MYALLOC0(sizeof(COMPRESULTNEW));
    if (lpCRNew != NULL) {
        lpCRNew->nActionType = nActionType;
        lpCRNew->lpContentOld = lpContentOld;
        lpCRNew->lpContentNew = lpContentNew;
    }
    else
        return;

    switch (nActionType) {
        case KEYDEL:
        case KEYADD:
        case VALDEL:
        case VALADD:
        case VALMODI:
            if (CompareResult.stCRCurrent.lpCRRegistry != NULL)
                lpCRNew->lpPrevCRNew = CompareResult.stCRCurrent.lpCRRegistry;
            (NULL == CompareResult.stCRHeads.lpCRRegistry) ? (CompareResult.stCRHeads.lpCRRegistry = lpCRNew) : (CompareResult.stCRCurrent.lpCRRegistry->lpNextCRNew = lpCRNew);
            CompareResult.stCRHeads.lpCRRegistryLast = lpCRNew;
            CompareResult.stCRCurrent.lpCRRegistry = lpCRNew;
            break;
        case DIRDEL:
        case DIRADD:
        case DIRMODI:
        case FILEDEL:
        case FILEADD:
        case FILEMODI:
            if (CompareResult.stCRCurrent.lpCRFilesystem != NULL)
                lpCRNew->lpPrevCRNew = CompareResult.stCRCurrent.lpCRFilesystem;
            (NULL == CompareResult.stCRHeads.lpCRFilesystem) ? (CompareResult.stCRHeads.lpCRFilesystem = lpCRNew) : (CompareResult.stCRCurrent.lpCRFilesystem->lpNextCRNew = lpCRNew);
            CompareResult.stCRHeads.lpCRFilesystemLast = lpCRNew;
            CompareResult.stCRCurrent.lpCRFilesystem = lpCRNew;
            break;
    }
    switch (nActionType) {
        case KEYDEL:
            (NULL == CompareResult.stCRHeads.lpCRKeyDeleted) ? (CompareResult.stCRHeads.lpCRKeyDeleted = lpCR) : (CompareResult.stCRCurrent.lpCRKeyDeleted->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRKeyDeleted = lpCR;
            break;
        case KEYADD:
            (NULL == CompareResult.stCRHeads.lpCRKeyAdded) ? (CompareResult.stCRHeads.lpCRKeyAdded = lpCR) : (CompareResult.stCRCurrent.lpCRKeyAdded->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRKeyAdded = lpCR;
            break;
        case VALDEL:
            (NULL == CompareResult.stCRHeads.lpCRValDeleted) ? (CompareResult.stCRHeads.lpCRValDeleted = lpCR) : (CompareResult.stCRCurrent.lpCRValDeleted->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRValDeleted = lpCR;
            break;
        case VALADD:
            (NULL == CompareResult.stCRHeads.lpCRValAdded) ? (CompareResult.stCRHeads.lpCRValAdded = lpCR) : (CompareResult.stCRCurrent.lpCRValAdded->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRValAdded = lpCR;
            break;
        case VALMODI:
            (NULL == CompareResult.stCRHeads.lpCRValModified) ? (CompareResult.stCRHeads.lpCRValModified = lpCR) : (CompareResult.stCRCurrent.lpCRValModified->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRValModified = lpCR;
            break;
        case DIRDEL:
            (NULL == CompareResult.stCRHeads.lpCRDirDeleted) ? (CompareResult.stCRHeads.lpCRDirDeleted = lpCR) : (CompareResult.stCRCurrent.lpCRDirDeleted->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRDirDeleted = lpCR;
            break;
        case DIRADD:
            (NULL == CompareResult.stCRHeads.lpCRDirAdded) ? (CompareResult.stCRHeads.lpCRDirAdded = lpCR) : (CompareResult.stCRCurrent.lpCRDirAdded->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRDirAdded = lpCR;
            break;
        case DIRMODI:
            (NULL == CompareResult.stCRHeads.lpCRDirModified) ? (CompareResult.stCRHeads.lpCRDirModified = lpCR) : (CompareResult.stCRCurrent.lpCRDirModified->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRDirModified = lpCR;
            break;
        case FILEDEL:
            (NULL == CompareResult.stCRHeads.lpCRFileDeleted) ? (CompareResult.stCRHeads.lpCRFileDeleted = lpCR) : (CompareResult.stCRCurrent.lpCRFileDeleted->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRFileDeleted = lpCR;
            break;
        case FILEADD:
            (NULL == CompareResult.stCRHeads.lpCRFileAdded) ? (CompareResult.stCRHeads.lpCRFileAdded = lpCR) : (CompareResult.stCRCurrent.lpCRFileAdded->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRFileAdded = lpCR;
            break;
        case FILEMODI:
            (NULL == CompareResult.stCRHeads.lpCRFileModified) ? (CompareResult.stCRHeads.lpCRFileModified = lpCR) : (CompareResult.stCRCurrent.lpCRFileModified->lpNextCR = lpCR);
            CompareResult.stCRCurrent.lpCRFileModified = lpCR;
            break;
    }
}


//-------------------------------------------------------------
// Convert content to result strings
//-------------------------------------------------------------
size_t ResultToString(LPTSTR rgszResultStrings[], size_t iResultStringsMac, size_t iLinesWrittenOldPart, DWORD nActionType, LPVOID lpContent, BOOL fNewContent, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    LPTSTR lpszName;
    LPTSTR lpszData = NULL;
    LPTSTR lpszOldData;
    size_t cchData;
    size_t iResultStringsNew;
    size_t iResultStringsTemp1;
    LPTSTR lpszIntermediateResultString;
    LPTSTR* lpszValueData = MYALLOC0(((size_t)nOutMaxResultLines+1) * sizeof(LPTSTR));
//    for (int i = 0; i <= MAX_RESULT_STRINGS; i++)
    for (int i = 0; i <= nOutMaxResultLines; i++)
        lpszValueData[i] = NULL;

    iResultStringsNew = iResultStringsMac;

    if ((KEYDEL == nActionType) || (KEYADD == nActionType)) {
        // create result
        // name only
//        if (iResultStringsNew < MAX_RESULT_STRINGS) {
        if (iResultStringsNew < nOutMaxResultLines) {
            
            lpszIntermediateResultString = GetWholeKeyName(lpContent, pOutputFileDescription->fUseLongRegHead);
            if (pOutputFileDescription->iOutputType == OUT_UNL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if ((pOutputFileDescription->iOutputType == OUT_BAT) && (KEYADD == nActionType)) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if ((pOutputFileDescription->iOutputType == OUT_AU3) && (KEYADD == nActionType)) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else if (pOutputFileDescription->iOutputType == OUT_REG_INSTALL) {
                rgszResultStrings[iResultStringsNew] = BuildOutputString(lpszIntermediateResultString, NULL, nActionType, FALSE, pOutputFileDescription);
                MYFREE(lpszIntermediateResultString);
            }
            else
                rgszResultStrings[iResultStringsNew] = lpszIntermediateResultString;
            
            iResultStringsNew++;
        }
    } 
    else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
        // name
        lpszIntermediateResultString = GetWholeKeyName(((LPVALUECONTENT)lpContent)->lpFatherKC, pOutputFileDescription->fUseLongRegHead);
//        lpszIntermediateResultString = EscapeSpecialCharacters(lpszIntermediateResultString, _T('{'), TEXT("{"));

//        LPVALUECONTENT test = (LPVALUECONTENT)lpContent;
        if (pOutputFileDescription->iOutputType == OUT_UNL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if ((pOutputFileDescription->iOutputType == OUT_BAT) && (VALADD == nActionType)) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if ((pOutputFileDescription->iOutputType == OUT_AU3) && (VALADD == nActionType)) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else if (pOutputFileDescription->iOutputType == OUT_REG_INSTALL) {
            lpszName = BuildOutputString(lpszIntermediateResultString, lpContent, nActionType, bSuppressKey, pOutputFileDescription);
            MYFREE(lpszIntermediateResultString);
        }
        else
            lpszName = GetWholeValueName(lpContent, pOutputFileDescription->fUseLongRegHead);
        
        cchData = 0;
        if (((pOutputFileDescription->iOutputType == OUT_UNL) || (pOutputFileDescription->iOutputType == OUT_HTML) || (pOutputFileDescription->iOutputType == OUT_TXT) ||
            ((((pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL)) && ((nActionType == VALDEL) || (nActionType == VALMODI))) ||
            ((((pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_INSTALL)) && ((nActionType == VALADD)) || (nActionType == VALMODI)))))
            && !pOutputFileDescription->fNoVals)
        {
            // data
//            lpszData = GetWholeValueData(lpContent, TRUE, iOutputType);
            cchData = _tcslen(lpszName);
            if ((pOutputFileDescription->iOutputType == OUT_REG_INSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL)) {
//                LPVALUECONTENT lpTest = lpContent;
                cchData = ((LPVALUECONTENT)lpContent)->cchValueName + 2; // *_tcslen(lpszREGValueEnclosing);
            }
            GetWholeValueData(lpszValueData, lpContent, nActionType, cchData, pOutputFileDescription->iOutputType);
//            if (NULL != lpszData) {
//                cchData = _tcslen(lpszData);
//            }
        }

        // create result
        if (iResultStringsNew - iLinesWrittenOldPart < nOutMaxResultLines) {
            for (int i = 0; iResultStringsNew - iLinesWrittenOldPart < nOutMaxResultLines; i++) {
                if ((i > 0) && (lpszValueData[i] == NULL))
                    break;
                cchData = (i == 0 ? _tcslen(lpszName) : 0) + 1;
                if (lpszValueData[i] != NULL)
                    cchData += _tcslen(lpszValueData[i]);
                rgszResultStrings[iResultStringsNew] = MYALLOC(cchData * sizeof(TCHAR));
                if (i == 0) {
                    _tcscpy(rgszResultStrings[iResultStringsNew], lpszName);
                    if (lpszValueData[i] != NULL)
                        _tcscat(rgszResultStrings[iResultStringsNew], lpszValueData[i]);
                }
                else {
                    _tcscpy(rgszResultStrings[iResultStringsNew], lpszValueData[i]);
                }
                
//                cchData = _tcslen(rgszResultStrings[iResultStringsNew]);
                MYFREE(lpszValueData[i]);
                lpszValueData[i] = NULL;
                iResultStringsNew++;
            }
        }
        //for (int i = 0; i < MAX_RESULT_STRINGS; i++) {
        //    lpszValueData[i] = NULL;
        //}
        MYFREE(lpszName);
        //if (NULL != lpszValueData) {
        //    MYFREE(lpszValueData);
        //}
        //if (NULL != lpszData) {
        //    MYFREE(lpszData);
        //}
    } 
    else if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType)) {
        // create result (1st line)
        // name only
        if ((!fNewContent) || (0 >= iResultStringsMac)) {
//            if (iResultStringsNew < MAX_RESULT_STRINGS) {
            if (iResultStringsNew < nOutMaxResultLines) {
                if (pOutputFileDescription->iOutputType == OUT_UNL) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_BAT) && (DIRADD == nActionType)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_AU3) && (DIRADD == nActionType)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) || (pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else
                    rgszResultStrings[iResultStringsNew] = GetWholeFileName(lpContent, 0, pOutputFileDescription);
                iResultStringsNew++;
            }
        }
        
        if ((pOutputFileDescription->iOutputType != OUT_UNL) && (pOutputFileDescription->iOutputType != OUT_BAT) && (pOutputFileDescription->iOutputType != OUT_AU3) &&
            (pOutputFileDescription->iOutputType != OUT_ISS_INSTALL) && (pOutputFileDescription->iOutputType != OUT_ISS_DEINSTALL) &&
            (pOutputFileDescription->iOutputType != OUT_NSI_INSTALL) && (pOutputFileDescription->iOutputType != OUT_NSI_DEINSTALL)
            ) {
            // attributes
            lpszData = MYALLOC0(SIZEOF_RESULT_DATA * sizeof(TCHAR));
            cchData = _sntprintf(lpszData, SIZEOF_RESULT_DATA, TEXT("0x%08X\0"), ((LPFILECONTENT)lpContent)->nFileAttributes);
            // create result (2nd line)
            // add to previous line if old and new data present
            iResultStringsTemp1 = iResultStringsNew;
            lpszOldData = NULL;
            if ((fNewContent) && (0 < iResultStringsMac)) {
                iResultStringsTemp1 = iResultStringsMac - 1;
                lpszOldData = rgszResultStrings[iResultStringsTemp1];
                cchData += _tcslen(lpszOldData) + 5;  // length in chars of separator between old and new content
            }
//            if (iResultStringsTemp1 < MAX_RESULT_STRINGS) {
            if (iResultStringsTemp1 < nOutMaxResultLines) {
                rgszResultStrings[iResultStringsTemp1] = MYALLOC((cchData + 1) * sizeof(TCHAR));
                if ((fNewContent) && (0 < iResultStringsMac) && (NULL < rgszResultStrings[iResultStringsTemp1])) {
                    _tcscpy(rgszResultStrings[iResultStringsTemp1], lpszOldData);
                    _tcscat(rgszResultStrings[iResultStringsTemp1], TEXT(" --> "));
                }
                else {
                    rgszResultStrings[iResultStringsNew][0] = (TCHAR)'\0';
                }
                _tcscat(rgszResultStrings[iResultStringsTemp1], lpszData);

                if (iResultStringsTemp1 >= iResultStringsMac) {
                    iResultStringsNew++;
                }
            }
            MYFREE(lpszData);
            if (NULL != lpszOldData) {
                MYFREE(lpszOldData);
            }
        }
    } 
    else if ((FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
        SYSTEMTIME stFile;
        FILETIME ftFile;
        LARGE_INTEGER cbFile;
        // create result (1st line)
        // name only
        if ((!fNewContent) || (0 >= iResultStringsMac)) {
//            if (iResultStringsNew < MAX_RESULT_STRINGS) {
            if (iResultStringsNew < nOutMaxResultLines) {
                if (pOutputFileDescription->iOutputType == OUT_UNL) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_BAT) && (FILEADD == nActionType)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_AU3) && (FILEADD == nActionType)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) || (pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else if ((pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL)) {
                    rgszResultStrings[iResultStringsNew] = BuildOutputString(NULL, lpContent, nActionType, FALSE, pOutputFileDescription);
                }
                else
                    rgszResultStrings[iResultStringsNew] = GetWholeFileName(lpContent, 0, pOutputFileDescription);
                iResultStringsNew++;
            }
        }
        
        if ((pOutputFileDescription->iOutputType != OUT_UNL) && (pOutputFileDescription->iOutputType != OUT_BAT) && (pOutputFileDescription->iOutputType != OUT_AU3) &&
            (pOutputFileDescription->iOutputType != OUT_ISS_INSTALL) && (pOutputFileDescription->iOutputType != OUT_ISS_DEINSTALL) &&
            (pOutputFileDescription->iOutputType != OUT_NSI_INSTALL) && (pOutputFileDescription->iOutputType != OUT_NSI_DEINSTALL)
            ) {
            // last write time, attributes, size
            ZeroMemory(&ftFile, sizeof(ftFile));
            ftFile.dwLowDateTime = ((LPFILECONTENT)lpContent)->nWriteDateTimeLow;
            ftFile.dwHighDateTime = ((LPFILECONTENT)lpContent)->nWriteDateTimeHigh;
            ZeroMemory(&stFile, sizeof(stFile));
            FileTimeToSystemTime(&ftFile, &stFile);
            ZeroMemory(&cbFile, sizeof(cbFile));
            cbFile.LowPart = ((LPFILECONTENT)lpContent)->nFileSizeLow;
            cbFile.HighPart = ((LPFILECONTENT)lpContent)->nFileSizeHigh;
            lpszData = MYALLOC0(SIZEOF_RESULT_DATA * sizeof(TCHAR));
            cchData = _sntprintf(lpszData, SIZEOF_RESULT_DATA, TEXT("%04d-%02d-%02d %02d:%02d:%02d, 0x%08X, %lld\0"),
                stFile.wYear, stFile.wMonth, stFile.wDay,
                stFile.wHour, stFile.wMinute, stFile.wSecond,
                ((LPFILECONTENT)lpContent)->nFileAttributes, cbFile.QuadPart);
            // create result (2nd/3rd line)
//            if (iResultStringsNew < MAX_RESULT_STRINGS) {
            if (iResultStringsNew < nOutMaxResultLines) {
                rgszResultStrings[iResultStringsNew] = lpszData;
                iResultStringsNew++;
            }
        }
    } 
    else {
        // TODO: error message and handling
    }

    if (NULL != lpszValueData) {
        MYFREE(lpszValueData);
    }
    return iResultStringsNew;
}


//-------------------------------------------------------------
// Routine to free all comparison results (release memory)
//-------------------------------------------------------------
VOID FreeAllCompResults(LPCOMPRESULT lpCR)
{
    LPCOMPRESULT lpNextCR;

    for (; NULL != lpCR; lpCR = lpNextCR) {
        // Save pointer in local variable
        lpNextCR = lpCR->lpNextCR;

        // Increase count
        cCurrent++;

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }

        // Free compare result itself
        MYFREE(lpCR);
    }
}

//-------------------------------------------------------------
// Routine to free all comparison results (release memory)
//-------------------------------------------------------------
VOID FreeAllCompResultsNew(LPCOMPRESULTNEW lpCR)
{
    LPCOMPRESULTNEW lpNextCR;

    for (; NULL != lpCR; lpCR = lpNextCR) {
        // Save pointer in local variable
        lpNextCR = lpCR->lpNextCRNew;

        // Increase count
        cCurrent++;

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }

        // Free compare result itself
        MYFREE(lpCR);
    }
}


// ----------------------------------------------------------------------
// Free all compare results
// ----------------------------------------------------------------------
VOID FreeCompareResult(void)
{
    FreeAllCompResults(CompareResult.stCRHeads.lpCRKeyDeleted);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRKeyAdded);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRValDeleted);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRValAdded);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRValModified);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRDirDeleted);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRDirAdded);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRDirModified);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRFileDeleted);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRFileAdded);
    FreeAllCompResults(CompareResult.stCRHeads.lpCRFileModified);
    FreeAllCompResultsNew(CompareResult.stCRHeads.lpCRFilesystem);
    FreeAllCompResultsNew(CompareResult.stCRHeads.lpCRRegistry);

    ZeroMemory(&CompareResult, sizeof(CompareResult));
}


//-------------------------------------------------------------
// Registry comparison engine
//-------------------------------------------------------------
VOID CompareRegKeys(LPKEYCONTENT lpStartKC1, LPKEYCONTENT lpStartKC2)
{
    LPKEYCONTENT lpKC1;
    LPKEYCONTENT lpKC2;
    LPTSTR lpszFullName;

    // Compare keys
    for (lpKC1 = lpStartKC1; NULL != lpKC1; lpKC1 = lpKC1->lpBrotherKC) {
        // TODO: Filter when comparing
        lpszFullName = GetWholeKeyName(lpKC1, FALSE);
//        if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
        if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
            MYFREE(lpszFullName);
            continue;
        }
        MYFREE(lpszFullName);

        CompareResult.stcCompared.cKeys++;
        // Find a matching key for KC1
        for (lpKC2 = lpStartKC2; NULL != lpKC2; lpKC2 = lpKC2->lpBrotherKC) {

            // Update counters display
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                lpszUIBuffer = GetWholeKeyName(lpKC2, FALSE);
                if (lpszUIBuffer != NULL) {
                    UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, CompareResult.stcCompared.cKeys, CompareResult.stcCompared.cValues, asLangTexts[iszTextKey].lpszText, lpszUIBuffer);
                    MYFREE(lpszUIBuffer);
                }
            }
            if (!bRunning)
                break;

// skip KC2 if already matched
            if (NOMATCH != lpKC2->fKeyMatch) {
                continue;
            }
            // skip KC2 if names do *not* match (ATTENTION: test for match, THEN negate)
            if (!(
                        (lpKC1->lpszKeyName == lpKC2->lpszKeyName)
                        || ((NULL != lpKC1->lpszKeyName) && (NULL != lpKC2->lpszKeyName) && (0 == _tcscmp(lpKC1->lpszKeyName, lpKC2->lpszKeyName)))  // TODO: case-insensitive compare?
                    )) {
                continue;
            }

            // Same key name of KC1 found in KC2! Mark KC2 as matched to skip it for the next KC1, then compare their values and sub keys!
            lpKC2->fKeyMatch = ISMATCH;

            // Extra local block to reduce stack usage due to recursive calls
            {
                LPVALUECONTENT lpVC1;
                LPVALUECONTENT lpVC2;

                // Compare values
                for (lpVC1 = lpKC1->lpFirstVC; NULL != lpVC1; lpVC1 = lpVC1->lpBrotherVC) {
                    CompareResult.stcCompared.cValues++;
                    // Find a matching value for VC1
                    for (lpVC2 = lpKC2->lpFirstVC; NULL != lpVC2; lpVC2 = lpVC2->lpBrotherVC) {
                        // skip VC2 if already matched
                        if (NOMATCH != lpVC2->fValueMatch) {
                            continue;
                        }
                        // skip VC2 if types do *not* match (even if same name then interpret as deleted+added)
                        if (lpVC1->nTypeCode != lpVC2->nTypeCode) {
                            continue;
                        }
                        // skip VC2 if names do *not* match (ATTENTION: test for match, THEN negate)
                        if (!(
                                    (lpVC1->lpszValueName == lpVC2->lpszValueName)
                                    || ((NULL != lpVC1->lpszValueName) && (NULL != lpVC2->lpszValueName) && (0 == _tcscmp(lpVC1->lpszValueName, lpVC2->lpszValueName)))  // TODO: case-insensitive compare?
                                )) {
                            continue;
                        }

                        // Same value type and name of VC1 found in VC2, so compare their size and data
                        if ((lpVC1->cbData == lpVC2->cbData)
                                && ((lpVC1->lpValueData == lpVC2->lpValueData)
                                    || ((NULL != lpVC1->lpValueData) && (NULL != lpVC2->lpValueData) && (0 == memcmp(lpVC1->lpValueData, lpVC2->lpValueData, lpVC1->cbData)))
                                   )) {
                            // Same value data of VC1 found in VC2
                            lpVC2->fValueMatch = ISMATCH;
                        } else {
                            // Value data differ, so value is modified
                            lpVC2->fValueMatch = ISMODI;
                            CompareResult.stcChanged.cValues++;
                            CompareResult.stcModified.cValues++;
                            CreateNewResult(VALMODI, lpVC1, lpVC2);
                        }
                        break;
                    }
                    if (NULL == lpVC2) {
                        // VC1 has no match in KC2, so VC1 is a deleted value
                        CompareResult.stcChanged.cValues++;
                        CompareResult.stcDeleted.cValues++;
                        CreateNewResult(VALDEL, lpVC1, NULL);
                    }
                }
                // After looping all values of KC1, do an extra loop over all KC2 values and check previously set match flags to determine added values
                for (lpVC2 = lpKC2->lpFirstVC; NULL != lpVC2; lpVC2 = lpVC2->lpBrotherVC) {
                    // skip VC2 if already matched
                    if (NOMATCH != lpVC2->fValueMatch) {
                        continue;
                    }

                    CompareResult.stcCompared.cValues++;

                    // VC2 has no match in KC1, so VC2 is an added value
                    CompareResult.stcChanged.cValues++;
                    CompareResult.stcAdded.cValues++;
                    CreateNewResult(VALADD, NULL, lpVC2);
                }
            }  // End of extra local block

            // Compare sub keys if any
            if ((NULL != lpKC1->lpFirstSubKC) || (NULL != lpKC2->lpFirstSubKC)) {
                CompareRegKeys(lpKC1->lpFirstSubKC, lpKC2->lpFirstSubKC);
            }
            break;
        }
        if (NULL == lpKC2) {
            // KC1 has no matching KC2, so KC1 is a deleted key
            CompareResult.stcChanged.cKeys++;
            CompareResult.stcDeleted.cKeys++;
            CreateNewResult(KEYDEL, lpKC1, NULL);
            // Extra local block to reduce stack usage due to recursive calls
            {
                LPVALUECONTENT lpVC1;

                for (lpVC1 = lpKC1->lpFirstVC; NULL != lpVC1; lpVC1 = lpVC1->lpBrotherVC) {
                    CompareResult.stcCompared.cValues++;
                    CompareResult.stcChanged.cValues++;
                    CompareResult.stcDeleted.cValues++;
                    CreateNewResult(VALDEL, lpVC1, NULL);
                }
            }  // End of extra local block

            // "Compare"/Log sub keys if any
            if (NULL != lpKC1->lpFirstSubKC) {
                CompareRegKeys(lpKC1->lpFirstSubKC, NULL);
            }
        }
    }
    // After looping all KC1 keys, do an extra loop over all KC2 keys and check previously set match flags to determine added keys
    for (lpKC2 = lpStartKC2; NULL != lpKC2; lpKC2 = lpKC2->lpBrotherKC) {
        // skip KC2 if already matched
        if (NOMATCH != lpKC2->fKeyMatch) {
            continue;
        }
        // TODO: Filter when comparing
        lpszFullName = GetWholeKeyName(lpKC2, FALSE);
        // if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
        if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
            MYFREE(lpszFullName);
            continue;
        }
        MYFREE(lpszFullName);

        // KC2 has no matching KC1, so KC2 is an added key
        CompareResult.stcCompared.cKeys++;
        CompareResult.stcChanged.cKeys++;
        CompareResult.stcAdded.cKeys++;
        CreateNewResult(KEYADD, NULL, lpKC2);
        // Extra local block to reduce stack usage due to recursive calls
        {
            LPVALUECONTENT lpVC2;

            for (lpVC2 = lpKC2->lpFirstVC; NULL != lpVC2; lpVC2 = lpVC2->lpBrotherVC) {
                CompareResult.stcCompared.cValues++;
                CompareResult.stcChanged.cValues++;
                CompareResult.stcAdded.cValues++;
                CreateNewResult(VALADD, NULL, lpVC2);
            }
        }  // End of extra local block

        // "Compare"/Log sub keys if any
        if (NULL != lpKC2->lpFirstSubKC) {
            CompareRegKeys(NULL, lpKC2->lpFirstSubKC);
        }
    }

    // Update counters display
    nCurrentTime = GetTickCount64();
    if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, CompareResult.stcCompared.cKeys, CompareResult.stcCompared.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }
}


//------------------------------------------------------------
// Routine to call registry/file comparison engine
//------------------------------------------------------------
VOID CompareShots(VOID)
{
    if (!DirChainMatch(Shot1.lpHF, Shot2.lpHF)) {
        MessageBox(hMainWnd, TEXT("Found two shots with different DIR chain! (or with different order)\r\nYou can continue, but file comparison result would be abnormal!"), asLangTexts[iszTextWarning].lpszText, MB_ICONWARNING);  //TODO: I18N, create text index and translate
    }

    // Initialize result and markers
    cEnd = 0;
    FreeCompareResult();
    CompareResult.lpShot1 = &Shot1;
    CompareResult.lpShot2 = &Shot2;
    ClearRegKeyMatchFlags(Shot1.lpHKLM);
    ClearRegKeyMatchFlags(Shot2.lpHKLM);
    ClearRegKeyMatchFlags(Shot1.lpHKU);
    ClearRegKeyMatchFlags(Shot2.lpHKU);
    ClearRegKeyMatchFlags(Shot1.lpHKCU);
    ClearRegKeyMatchFlags(Shot2.lpHKCU);
    ClearHeadFileMatchFlags(Shot1.lpHF);
    ClearHeadFileMatchFlags(Shot2.lpHF);

    // Setup GUI for comparing...
    UI_InitCounters();

    // Compare HKLM
    if ((NULL != CompareResult.lpShot1->lpHKLM) || (NULL != CompareResult.lpShot2->lpHKLM)) {
        CompareRegKeys(CompareResult.lpShot1->lpHKLM, CompareResult.lpShot2->lpHKLM);
        if (!bRunning) {
            return;
        }

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, CompareResult.stcCompared.cKeys, CompareResult.stcCompared.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Compare HKU
    if ((NULL != CompareResult.lpShot1->lpHKU) || (NULL != CompareResult.lpShot2->lpHKU)) {
        CompareRegKeys(CompareResult.lpShot1->lpHKU, CompareResult.lpShot2->lpHKU);
        if (!bRunning) {
            return;
        }

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, CompareResult.stcCompared.cKeys, CompareResult.stcCompared.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Compare HKCU
    if ((NULL != CompareResult.lpShot1->lpHKCU) || (NULL != CompareResult.lpShot2->lpHKCU)) {
        CompareRegKeys(CompareResult.lpShot1->lpHKCU, CompareResult.lpShot2->lpHKCU);
        if (!bRunning) {
            return;
        }

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, CompareResult.stcCompared.cKeys, CompareResult.stcCompared.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Compare HEADFILEs v1.8.1
    if ((NULL != CompareResult.lpShot1->lpHF) || (NULL != CompareResult.lpShot2->lpHF)) {
        CompareHeadFiles(CompareResult.lpShot1->lpHF, CompareResult.lpShot2->lpHF);
        if (!bRunning) {
            return;
        }

        // Update counters display (dirs/files final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextDir].lpszText, asLangTexts[iszTextFile].lpszText, CompareResult.stcCompared.cDirs, CompareResult.stcCompared.cFiles, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Get total count of all items
    CompareResult.stcCompared.cAll = CompareResult.stcCompared.cKeys
                                     + CompareResult.stcCompared.cValues
                                     + CompareResult.stcCompared.cDirs
                                     + CompareResult.stcCompared.cFiles;
    CompareResult.stcChanged.cAll = CompareResult.stcChanged.cKeys
                                    + CompareResult.stcChanged.cValues
                                    + CompareResult.stcChanged.cDirs
                                    + CompareResult.stcChanged.cFiles;
    CompareResult.stcDeleted.cAll = CompareResult.stcDeleted.cKeys
                                    + CompareResult.stcDeleted.cValues
                                    + CompareResult.stcDeleted.cDirs
                                    + CompareResult.stcDeleted.cFiles;
    CompareResult.stcAdded.cAll = CompareResult.stcAdded.cKeys
                                  + CompareResult.stcAdded.cValues
                                  + CompareResult.stcAdded.cDirs
                                  + CompareResult.stcAdded.cFiles;
    CompareResult.stcModified.cAll = CompareResult.stcModified.cKeys
                                     + CompareResult.stcModified.cValues
                                     + CompareResult.stcModified.cDirs
                                     + CompareResult.stcModified.cFiles;
    CompareResult.fFilled = TRUE;

    UI_ShowHideCounters(SW_HIDE);
}


//------------------------------------------------------------
// Routine to output comparison result
//------------------------------------------------------------
BOOL OutputComparisonResult(VOID)
{
    LPTSTR lpszBuffer;
    LPTSTR lpszFilenameInstallerSuffix = TEXT("-Installer");
    LPTSTR lpszFilenameUninstallerSuffix = TEXT("-Uninstaller");
    LPTSTR lpszFilenameLogSuffix = TEXT("-log");
    LPTSTR lpszDestFileName;
    DWORD  nBufferSize = 2048;
    size_t cchString;
    
    // read common flags for output from gui (reset flags if they are wrong for certain output type)
    fNoVals = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOVALS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    fOnlyNewEntries = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_ONLYADDED), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    fNoDeletedEntries = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOTDELETED), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    int nResult = (int)SendDlgItemMessage(hMainWnd, IDC_COMBO_MAINCP, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
    if (CB_ERR == nResult) {
        nCodePage = 65001;
    }
    else if (nResult == 0)
        nCodePage = 0;
    else if (nResult == 1)
        nCodePage = -1;
    else
        nCodePage = 65001;
    if ((BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_RESETADHOC), BM_GETCHECK, (WPARAM)0, (LPARAM)0)) {
        ResetAdhocFilters();
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_RESETADHOC), BM_SETCHECK, (WPARAM)0, (LPARAM)0);
    }

    // TODO: max. Anzahl �nderungen < 5000 (?)
    cEnd = (fOnlyNewEntries ? CompareResult.stcAdded.cAll : CompareResult.stcChanged.cAll);
    cEnd = (fNoDeletedEntries ? CompareResult.stcAdded.cAll + CompareResult.stcModified.cAll : cEnd);
    if ((BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_RESULT), BM_GETCHECK, (WPARAM)0, (LPARAM)0) && (cEnd <= nMaxNodes)) {
        DoTVPropertySheet(hMainWnd, (bCompareReg ? PROP_TVREGS : PROP_TVDIRS));
        if (bNoOutput) {
            bNoOutput = FALSE;
            return TRUE;
        }
        bNoOutput = FALSE;
    }
    
    // Setup GUI for saving...
    BOOL bNoResetOutputTypes = (cEnd > nMaxLines ? FALSE : TRUE);
    cEnd = CompareResult.stcChanged.cAll;

    UI_InitProgressBar();

    BOOL nOutputType[OUTPUTTYPES];
    for (int i = OUT_UNL; i < OUTPUTTYPES; i++)
        nOutputType[i] = FALSE;

    // Check which output types are selected
    if (fOutputBATfile)
        nOutputType[OUT_BAT] = bNoResetOutputTypes;
    if (fOutputUNLfile || !bNoResetOutputTypes)
        nOutputType[OUT_UNL] = TRUE;
    if (fOutputHTMfile)
        nOutputType[OUT_HTML] = bNoResetOutputTypes;
    if (fOutputTXTfile)
        nOutputType[OUT_TXT] = bNoResetOutputTypes;
    if (fOutputAU3file)
        nOutputType[OUT_AU3] = bNoResetOutputTypes;
    if (fISSDeinstallFile)
        nOutputType[OUT_ISS_DEINSTALL] = bNoResetOutputTypes;
    if (fISSInstallFile)
        nOutputType[OUT_ISS_INSTALL] = bNoResetOutputTypes;
    if (fNSIDeinstallFile)
        nOutputType[OUT_NSI_DEINSTALL] = bNoResetOutputTypes;
    if (fNSIInstallFile)
        nOutputType[OUT_NSI_INSTALL] = bNoResetOutputTypes;
    if (fRegDel)
        nOutputType[OUT_REG_DEINSTALL] = bNoResetOutputTypes;
    if (fRegIns)
        nOutputType[OUT_REG_INSTALL] = bNoResetOutputTypes;

    LPTSTR lpszValue = MYALLOC0(5 * sizeof(TCHAR));
    if (lpszValue != NULL) {
        GetDlgItemText(hMainWnd, IDC_EDIT_OUTPUTLINES, lpszValue, 4);
        lpszValue[4] = (TCHAR)'\0';
        nOutMaxResultLines = _tstoi(lpszValue);
    }
    MYFREE(lpszValue);

    DWORD cchDataline;
    DWORD nSize = MAX_PATH;
    int   iIniValue = 0;
    OUTPUTFILEDESCRIPTION OutputFileDescription;
    for (int i = OUT_UNL; i < OUTPUTTYPES; i++) {
        // Schalter Installer/Uninstaller, Logger und Format auf den GUI
        OutputFileDescription.lpszFormat = MYALLOC0(5 * sizeof(TCHAR));
        OutputFileDescription.bDeInstaller = TRUE;
        OutputFileDescription.lpszOutDir = NULL;
        OutputFileDescription.lpszOutFile = NULL;
        if (nOutputType[i]) {
            OutputFileDescription.iOutputType = i;
            switch (i) {
            case OUT_UNL:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("UNL"));
                break;
            
            case OUT_AU3:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("AU3"));
                break;
            
            case OUT_TXT:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("TXT"));
                break;

            case OUT_HTML:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("HTML"));
                break;
            
            case OUT_BAT:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("CMD"));
                break;

            case OUT_REG_DEINSTALL:
            case OUT_REG_INSTALL:
                _tcscpy(OutputFileDescription.lpszFormat, (fREG5 ? TEXT("REG5") : TEXT("REG4")));
                OutputFileDescription.bDeInstaller = (i == OUT_REG_DEINSTALL ? TRUE : FALSE);
                break;

            case OUT_ISS_DEINSTALL:
            case OUT_ISS_INSTALL:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("ISS"));
                OutputFileDescription.bDeInstaller = (i == OUT_ISS_DEINSTALL ? TRUE : FALSE);
                if (fUseDifferentISSOutputFolder)
                    OutputFileDescription.lpszOutDir = lpszISSOutputFolder;
                break;

            case OUT_NSI_DEINSTALL:
            case OUT_NSI_INSTALL:
                _tcscpy(OutputFileDescription.lpszFormat, TEXT("NSI"));
                OutputFileDescription.bDeInstaller = (i == OUT_NSI_DEINSTALL ? TRUE : FALSE);
                
                OutputFileDescription.lpszOutFile = MYALLOC(MAX_PATH * sizeof(TCHAR));
                if (fUseDifferentNSIOutputFolder) {
                    _tcscpy(OutputFileDescription.lpszOutFile, lpszNSIOutputFolder);
                    if (_tcsrchr(OutputFileDescription.lpszOutFile, _T('\\')) != OutputFileDescription.lpszOutFile + _tcslen(OutputFileDescription.lpszOutFile) - 1)
                        _tcscat(OutputFileDescription.lpszOutFile, TEXT("\\"));
                    _tcscat(OutputFileDescription.lpszOutFile, lpszTitle);
                }
                else
                    _tcscpy(OutputFileDescription.lpszOutFile, lpszTitle);
                _tcscat(OutputFileDescription.lpszOutFile, OutputFileDescription.lpszFilenameSuffix);
                
                break;
            }
            if (lpszMacroFileName != NULL) {
                GetModuleFileNameW(NULL, lpszMacroFileName, MAX_PATH);
                PathRemoveFileSpec(lpszMacroFileName);
                _tcscat(lpszMacroFileName, TEXT("\\Templates\\"));
                _tcscat(lpszMacroFileName, OutputFileDescription.lpszFormat);
                _tcscat(lpszMacroFileName, TEXT(".tpl"));
            }
            OutputFileDescription.lpszComment = TEXT(";");
            cchDataline = GetPrivateProfileString(TEXT("$$SETUP$$"), TEXT("Comment"), TEXT(";"), OutputFileDescription.lpszComment, nSize, lpszMacroFileName);
            OutputFileDescription.lpszExtension = MYALLOC0(20 * sizeof(TCHAR));
            cchDataline = GetPrivateProfileString(TEXT("$$SETUP$$"), TEXT("Extension"), TEXT(".XXX"), OutputFileDescription.lpszExtension, nSize, lpszMacroFileName);

            OutputFileDescription.lpszFilenameSuffix = (OutputFileDescription.bDeInstaller ? lpszFilenameUninstallerSuffix : lpszFilenameInstallerSuffix);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("Logger"), 0, lpszMacroFileName);
            OutputFileDescription.bLogger = (iIniValue == 1 ? TRUE : FALSE);
            OutputFileDescription.lpszFilenameSuffix = (OutputFileDescription.bLogger ? lpszFilenameLogSuffix : OutputFileDescription.lpszFilenameSuffix);
            
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("NoVals"), (fNoVals ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fNoVals = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("OnlyNewEntries"), (fOnlyNewEntries ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fOnlyNewEntries = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("LogUNLOrder"), (fLogUNLOrder ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fLogUNLOrder = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("OutSeparateObjs"), (fOutSeparateObjs ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fOutSeparateObjs = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("UseLongRegHead"), (fUseLongRegHead ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fUseLongRegHead = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("GroupRegKeys"), 0, lpszMacroFileName);
            OutputFileDescription.fGroupRegKeys = (iIniValue == 1 ? TRUE : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("LogEnvironmentStrings"), (fLogEnvironmentStrings ? 1 : 0), lpszMacroFileName);
            OutputFileDescription.fLogEnvironmentStrings = (iIniValue == 1 ? TRUE : FALSE);

            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("Filesystem"), 1, lpszMacroFileName);
            OutputFileDescription.fFilesystem = (iIniValue == 1 ? TRUE : FALSE);
            OutputFileDescription.fFilesystem = (bCompareFS ? OutputFileDescription.fFilesystem : FALSE);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("Registry"), 1, lpszMacroFileName);
            OutputFileDescription.fRegistry = (iIniValue == 1 ? TRUE : FALSE);
            OutputFileDescription.fRegistry = (bCompareReg ? OutputFileDescription.fRegistry : FALSE);
            
            OutputFileDescription.nCodePage = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("Codepage"), nCodePage, lpszMacroFileName);
            iIniValue = GetPrivateProfileInt(TEXT("$$SETUP$$"), TEXT("BOM"), 0, lpszMacroFileName);
            OutputFileDescription.bBOM = (iIniValue == 1 ? TRUE : FALSE);
            
            OutputFileDescription.lpszScriptFile = NULL;
            OutputFileDescription.lpszPartCountText = NULL;
            OutputFileDescription.nPartCount = 0;
        }
        else
            continue;

        lpszDestFileName = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
        lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
        GetDlgItemText(hMainWnd, IDC_EDITCOMMENT, lpszBuffer, COMMENTLENGTH);  // length incl. NULL character

        LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
        GetDlgItemText(hMainWnd, IDC_EDITPATH, lpszOutputPath, MAX_PATH);  // length incl. NULL character
        ExpandEnvironmentStrings(lpszOutputPath, lpszTempDirBuffer, MAX_PATH);
        _tcscpy(lpszOutputPath, lpszTempDirBuffer);
        MYFREE(lpszTempDirBuffer);
        // TODO: Subdirectory TITLE
        GetDlgItemText(hMainWnd, IDC_EDITTITLE, lpszTitle, MAX_PATH);  // length incl. NULL character

        cchString = _tcslen(lpszOutputPath);
        if ((0 < cchString) && ((TCHAR)'\\' != *(lpszOutputPath + cchString - 1))) {
            *(lpszOutputPath + cchString) = (TCHAR)'\\';
            *(lpszOutputPath + cchString + 1) = (TCHAR)'\0';
            cchString++;
        }
        ReplaceInvalidFileNameChars(lpszTitle);
        _tcscat(lpszOutputPath, lpszTitle);
        cchString = _tcslen(lpszOutputPath);
        if ((0 < cchString) && ((TCHAR)'\\' != *(lpszOutputPath + cchString - 1))) {
            *(lpszOutputPath + cchString) = (TCHAR)'\\';
            *(lpszOutputPath + cchString + 1) = (TCHAR)'\0';
            cchString++;
        }
        _tcscpy(lpszDestFileName, lpszOutputPath);
        CreateDirectory(lpszDestFileName, 0);
        if (i == OUT_HTML) {
            _tcscat(lpszDestFileName, TEXT("report.css"));
            DeleteFile(lpszDestFileName);
            HANDLE hFile = CreateFile(lpszDestFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (OutputFileDescription.bBOM)
                WriteBOM(hFile, OutputFileDescription.nCodePage);
            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$CSS$$"), &OutputFileDescription);
            CloseHandle(hFile);
        }

        _tcscpy(lpszDestFileName, lpszOutputPath);
        if (ReplaceInvalidFileNameChars(lpszTitle)) {
            _tcscat(lpszDestFileName, lpszTitle);
        } else {
            _tcscat(lpszDestFileName, lpszResultFileBaseName);
        }

        _tcscat(lpszDestFileName, OutputFileDescription.lpszFilenameSuffix);
        cchString = _tcslen(lpszDestFileName);
        _tcscat(lpszDestFileName, OutputFileDescription.lpszExtension);

        HANDLE hFile = CreateFile(lpszDestFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            DWORD filetail;

            for (filetail = 1; MAXAMOUNTOFFILE > filetail; filetail++) {
                _sntprintf(lpszDestFileName + cchString, 6, TEXT("_%04u\0"), filetail);
                //*(lpszDestFileName+cchString + 5) = 0x00;
                _tcscpy(lpszDestFileName + cchString + 5, OutputFileDescription.lpszExtension);

                hFile = CreateFile(lpszDestFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                if (INVALID_HANDLE_VALUE == hFile) {
                    if (ERROR_FILE_EXISTS == GetLastError()) {  // My God! I use stupid ERROR_ALREADY_EXISTS first!!
                        continue;
                    } else {
                        ErrMsg(asLangTexts[iszTextErrorCreateFile].lpszText);
                        return FALSE;
                    }
                } else {
                    break;
                }
            }
            if (filetail >= MAXAMOUNTOFFILE) {
                ErrMsg(asLangTexts[iszTextErrorCreateFile].lpszText);
                return FALSE;
            }
        }
        OutputFileDescription.lpszScriptFile = lpszDestFileName;
        ReadOutputMacros(&OutputFileDescription);

        if (OutputFileDescription.bBOM)
            WriteBOM(hFile, OutputFileDescription.nCodePage);
        
        // Outputfile-Header
        if (i == OUT_HTML)
            WriteHTMLHeader(hFile, OutputFileDescription.nCodePage);
        else
            WritePartTemplate(hFile, TEXT("COMMON"), TEXT("$$HEADER$$"), &OutputFileDescription);
        
        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$HEADER$$"), &OutputFileDescription);
        if (OutputFileDescription.fLogUNLOrder) {
            int fsChanges = CompareResult.stcChanged.cDirs + CompareResult.stcChanged.cFiles;
            if (OutputFileDescription.fFilesystem == TRUE) {
                if (OutputFileDescription.fOnlyNewEntries)
                    fsChanges = CompareResult.stcAdded.cDirs + CompareResult.stcAdded.cFiles;
                if ((i != OUT_REG_INSTALL) && (i != OUT_REG_DEINSTALL)) {
                    if (fsChanges > 0) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFSSECTION$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextFilesystemModi].lpszText, fsChanges, &OutputFileDescription);
                        WritePartNew(hFile, ALLCHANGES, CompareResult.stCRHeads.lpCRFilesystemLast, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFSSECTION$$"), &OutputFileDescription);
                    }
                }
            }
        
            if (OutputFileDescription.fRegistry == TRUE) {
                fsChanges = CompareResult.stcChanged.cKeys + CompareResult.stcChanged.cValues;
                if (OutputFileDescription.fOnlyNewEntries)
                    fsChanges = CompareResult.stcAdded.cKeys + CompareResult.stcAdded.cValues;
                if (fsChanges > 0) {
                    WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTREGSECTION$$"), &OutputFileDescription);
                    WriteTableHead(hFile, asLangTexts[iszTextRegistryModi].lpszText, fsChanges, &OutputFileDescription);
                    WritePartNew(hFile, ALLCHANGES, CompareResult.stCRHeads.lpCRRegistryLast, &OutputFileDescription);
                    WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFSSECTION$$"), &OutputFileDescription);
                }
            }
        }
        else {
            if (OutputFileDescription.fRegistry == TRUE) {
                WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTREGSECTION$$"), &OutputFileDescription);

                // Installer (-install.reg, -install.iss): KeyAdd/ValAdd/ValModi/ValDel/KeyDel
                if (!OutputFileDescription.bDeInstaller) {
                    // Write keyadd part
                    if (0 != CompareResult.stcAdded.cKeys) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTKEYADDPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextKeyAdd].lpszText, CompareResult.stcAdded.cKeys, &OutputFileDescription);
                        WritePart(hFile, KEYADD, CompareResult.stCRHeads.lpCRKeyAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDKEYADDPART$$"), &OutputFileDescription);
                    }
                    // Write valadd part
                    if (0 != CompareResult.stcAdded.cValues) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTVALADDPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextValAdd].lpszText, CompareResult.stcAdded.cValues, &OutputFileDescription);
                        WritePart(hFile, VALADD, CompareResult.stCRHeads.lpCRValAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDVALADDPART$$"), &OutputFileDescription);
                    }
                }
                // Uninstaller (.html, .unl, .txt, .cmd, -uninstall.reg, -uninstall.iss, -uninstall.nsi): ValAdd/KeyAdd/ValModi/KeyDel/ValDel
                else {
                    // Write valadd part
                    if (0 != CompareResult.stcAdded.cValues) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTVALDELPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextValAdd].lpszText, CompareResult.stcAdded.cValues, &OutputFileDescription);
                        WritePart(hFile, VALADD, CompareResult.stCRHeads.lpCRValAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDVALDELPART$$"), &OutputFileDescription);
                    }
                    // Write keyadd part
                    if (0 != CompareResult.stcAdded.cKeys) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTKEYDELPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextKeyAdd].lpszText, CompareResult.stcAdded.cKeys, &OutputFileDescription);
                        WritePartNew(hFile, KEYADD, CompareResult.stCRHeads.lpCRRegistryLast, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDKEYDELPART$$"), &OutputFileDescription);
                    }
                }

                // Write valmodi part
                if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcModified.cValues)) {
                    WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTVALMODIPART$$"), &OutputFileDescription);
                    WriteTableHead(hFile, asLangTexts[iszTextValModi].lpszText, CompareResult.stcModified.cValues, &OutputFileDescription);
                    WritePart(hFile, VALMODI, CompareResult.stCRHeads.lpCRValModified, &OutputFileDescription);
                    WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDVALMODIPART$$"), &OutputFileDescription);
                }

                // Installer (-install.reg, -install.iss, -install.nsi): KeyAdd/ValAdd/ValModi/ValDel/KeyDel
                if (!fNoDeletedEntries) {
                    if (!OutputFileDescription.bDeInstaller) {
                        // Write valdel part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cValues)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTVALDELPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextValDel].lpszText, CompareResult.stcDeleted.cValues, &OutputFileDescription);
                            WritePart(hFile, VALDEL, CompareResult.stCRHeads.lpCRValDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDVALDELPART$$"), &OutputFileDescription);
                        }
                        // Write keydel part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cKeys)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTKEYDELPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextKeyDel].lpszText, CompareResult.stcDeleted.cKeys, &OutputFileDescription);
                            WritePart(hFile, KEYDEL, CompareResult.stCRHeads.lpCRKeyDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDKEYDELPART$$"), &OutputFileDescription);
                        }
                    }
                    // Uninstaller (.html, .unl, .txt, .cmd, -uninstall.reg, -uninstall.iss, -uninstall.nsi): ValAdd/KeyAdd/ValModi/KeyDel/ValDel
                    else {
                        // Write keydel part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cKeys)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTKEYADDPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextKeyDel].lpszText, CompareResult.stcDeleted.cKeys, &OutputFileDescription);
                            WritePart(hFile, KEYDEL, CompareResult.stCRHeads.lpCRKeyDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDKEYADDPART$$"), &OutputFileDescription);
                        }
                        // Write valdel part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cValues)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTVALADDPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextValDel].lpszText, CompareResult.stcDeleted.cValues, &OutputFileDescription);
                            WritePart(hFile, VALDEL, CompareResult.stCRHeads.lpCRValDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDVALADDPART$$"), &OutputFileDescription);
                        }
                    }
                }
                WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDREGSECTION$$"), &OutputFileDescription);
            }

            if (OutputFileDescription.fFilesystem) {
                WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFSSECTION$$"), &OutputFileDescription);
                // Installer (-install.reg, -install.iss): DirAdd/FileAdd/FileModi/DirModi/FileDel/DirDel
                if (!OutputFileDescription.bDeInstaller) {
                    // Write directory add part
                    if (0 != CompareResult.stcAdded.cDirs) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTDIRADDPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextDirAdd].lpszText, CompareResult.stcAdded.cDirs, &OutputFileDescription);
                        WritePart(hFile, DIRADD, CompareResult.stCRHeads.lpCRDirAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDDIRADDPART$$"), &OutputFileDescription);
                    }
                    // Write file add part
                    if (0 != CompareResult.stcAdded.cFiles) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFILEADDPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextFileAdd].lpszText, CompareResult.stcAdded.cFiles, &OutputFileDescription);
                        WritePart(hFile, FILEADD, CompareResult.stCRHeads.lpCRFileAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFILEADDPART$$"), &OutputFileDescription);
                    }
                }
// Uninstaller (.html, .unl, .txt, .cmd, -uninstall.reg, -uninstall.iss): FileAdd/DirAdd/FileModi/DirModi/DirDel/FileDel
                else {
                    // Write file add part
                    if (0 != CompareResult.stcAdded.cFiles) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFILEDELPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextFileAdd].lpszText, CompareResult.stcAdded.cFiles, &OutputFileDescription);
                        WritePart(hFile, FILEADD, CompareResult.stCRHeads.lpCRFileAdded, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFILEDELPART$$"), &OutputFileDescription);
                    }
                    // Write directory add part
                    if (0 != CompareResult.stcAdded.cDirs) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTDIRDELPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextDirAdd].lpszText, CompareResult.stcAdded.cDirs, &OutputFileDescription);
                        WritePartNew(hFile, DIRADD, CompareResult.stCRHeads.lpCRFilesystemLast, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDDIRDELPART$$"), &OutputFileDescription);
                    }
                }
                
                // Write file modi part
                if (OutputFileDescription.bLogger) {
                    if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcModified.cFiles)) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFILEMODIPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextFileModi].lpszText, CompareResult.stcModified.cFiles, &OutputFileDescription);
                        WritePart(hFile, FILEMODI, CompareResult.stCRHeads.lpCRFileModified, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFILEMODIPART$$"), &OutputFileDescription);
                    }
                }
                
// Installer (-install.reg, -install.iss): DirAdd/FileAdd/FileModi/DirModi/FileDel/DirDel
                if (!fNoDeletedEntries) {
                    if (!OutputFileDescription.bDeInstaller) {
                        // Write file del part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cFiles)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFILEDELPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextFileDel].lpszText, CompareResult.stcDeleted.cFiles, &OutputFileDescription);
                            WritePart(hFile, FILEDEL, CompareResult.stCRHeads.lpCRFileDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFILEDELPART$$"), &OutputFileDescription);
                        }
                        // Write directory del part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cDirs)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTDIRDELPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextDirDel].lpszText, CompareResult.stcDeleted.cDirs, &OutputFileDescription);
                            WritePart(hFile, DIRDEL, CompareResult.stCRHeads.lpCRDirDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDDIRDELPART$$"), &OutputFileDescription);
                        }
                    }
                    // Uninstaller (.html, .unl, .txt, .cmd, -uninstall.reg, -uninstall.iss, -uninstall.nsi): FileAdd/DirAdd/FileModi/DirModi/DirDel/FileDel
                    else {
                        // Write directory del part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cDirs)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTDIRADDPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextDirDel].lpszText, CompareResult.stcDeleted.cDirs, &OutputFileDescription);
                            WritePart(hFile, DIRDEL, CompareResult.stCRHeads.lpCRDirDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDDIRADDPART$$"), &OutputFileDescription);
                        }
                        // Write file del part
                        if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcDeleted.cFiles)) {
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTFILEADDPART$$"), &OutputFileDescription);
                            WriteTableHead(hFile, asLangTexts[iszTextFileDel].lpszText, CompareResult.stcDeleted.cFiles, &OutputFileDescription);
                            WritePart(hFile, FILEDEL, CompareResult.stCRHeads.lpCRFileDeleted, &OutputFileDescription);
                            WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFILEADDPART$$"), &OutputFileDescription);
                        }
                    }
                }
                // Write directory modi part
                if (OutputFileDescription.bLogger) {
                    if (!OutputFileDescription.fOnlyNewEntries && (0 != CompareResult.stcModified.cDirs)) {
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$STARTDIRMODIPART$$"), &OutputFileDescription);
                        WriteTableHead(hFile, asLangTexts[iszTextDirModi].lpszText, CompareResult.stcModified.cDirs, &OutputFileDescription);
                        WritePart(hFile, DIRMODI, CompareResult.stCRHeads.lpCRDirModified, &OutputFileDescription);
                        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDDIRMODIPART$$"), &OutputFileDescription);
                    }
                }
                WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$ENDFSSECTION$$"), &OutputFileDescription);
            }
        }
        if (i == OUT_HTML) {
            WriteFileCP(hFile, OutputFileDescription.nCodePage, TEXT("<br>\r\n"), (DWORD)(_tcslen(TEXT("<br>\r\n")) * sizeof(TCHAR)), &NBW, NULL);
        }
        WriteTableHead(hFile, asLangTexts[iszTextTotal].lpszText, (OutputFileDescription.fOnlyNewEntries ? CompareResult.stcAdded.cAll : CompareResult.stcChanged.cAll), &OutputFileDescription);
        WritePartTemplate(hFile, OutputFileDescription.lpszFormat, TEXT("$$FOOTER$$"), &OutputFileDescription);
        CloseHandle(hFile);

        fOpenEditor = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_OPENEDITOR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
        if (fOpenEditor) {
            if (i == OUT_HTML) {
                if ((32 >= (size_t)ShellExecute(hMainWnd, TEXT("open"), lpszDestFileName, NULL, NULL, SW_SHOW))) 
                    ErrMsg(asLangTexts[iszTextErrorExecViewer].lpszText);
            }
            else {
                lpszBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
                if (lpszBuffer != NULL) {
                    _tcscpy(lpszBuffer, TEXT("\""));
                    _tcscat(lpszBuffer, lpszDestFileName);
                    _tcscat(lpszBuffer, TEXT("\""));
                    if (((i == OUT_ISS_DEINSTALL) || (i == OUT_ISS_INSTALL)) && fUseDifferentISSEditor) {
                        _tspawnlp(_P_NOWAIT, lpszISSEditor, lpszBuffer, lpszBuffer, NULL);
                    }
                    else if ((i == OUT_NSI_DEINSTALL) || (i == OUT_NSI_INSTALL)) {
                        _tspawnlp(_P_NOWAIT, lpszEditor, lpszBuffer, lpszBuffer, NULL);
                        if (fUseDifferentNSIEditor)
                            _tspawnlp(_P_NOWAIT, lpszNSIEditor, lpszBuffer, lpszBuffer, NULL);
                    }
                    else
                        _tspawnlp(_P_NOWAIT, lpszEditor, lpszBuffer, lpszBuffer, NULL);
                }
                MYFREE(lpszBuffer);
            }
        }
        MYFREE(lpszDestFileName);

        MYFREE(OutputFileDescription.lpszFormat);
        MYFREE(OutputFileDescription.lpszExtension);
        if (OutputFileDescription.lpszOutFile != NULL)
            MYFREE(OutputFileDescription.lpszOutFile);
    }

    UI_ShowHideProgressBar(SW_HIDE);
    //TODO: l�schen nach Ausgabe des Vergleichs
    BOOL  fDelShots = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_DELSHOTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);    // 1.9.1: Separate objects in output with empty line
    if (fDelShots) {
        if (CompareResult.lpShot1->fLoaded) {
            DeleteFile(CompareResult.lpShot1->lpszFileName);
        }
        if (CompareResult.lpShot2->fLoaded) {
            DeleteFile(CompareResult.lpShot2->lpszFileName);
        }
    }
    
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOFILTERS), BM_SETCHECK, (WPARAM)FALSE, (LPARAM)0);

    return TRUE;
}


// ----------------------------------------------------------------------
// Clear comparison match flags of registry keys and values
// ----------------------------------------------------------------------
VOID ClearRegKeyMatchFlags(LPKEYCONTENT lpKC)
{
    LPVALUECONTENT lpVC;

    for (; NULL != lpKC; lpKC = lpKC->lpBrotherKC) {
        lpKC->fKeyMatch = NOMATCH;
        for (lpVC = lpKC->lpFirstVC; NULL != lpVC; lpVC = lpVC->lpBrotherVC) {
            lpVC->fValueMatch = NOMATCH;
        }
        ClearRegKeyMatchFlags(lpKC->lpFirstSubKC);
    }
}


// ----------------------------------------------------------------------
// Free all registry values
// ----------------------------------------------------------------------
VOID FreeAllValueContents(LPVALUECONTENT lpVC)
{
    LPVALUECONTENT lpBrotherVC;

    for (; NULL != lpVC; lpVC = lpBrotherVC) {
        // Save pointer in local variable
        lpBrotherVC = lpVC->lpBrotherVC;

        // Increase count
        cCurrent++;

        // Free value name
        if (NULL != lpVC->lpszValueName) {
            MYFREE(lpVC->lpszValueName);
        }

        // Free value data
        if (NULL != lpVC->lpValueData) {
            MYFREE(lpVC->lpValueData);
        }

        // Free entry itself
        MYFREE(lpVC);
    }
}


// ----------------------------------------------------------------------
// Free all registry keys and values
// ----------------------------------------------------------------------
VOID FreeAllKeyContents(LPKEYCONTENT lpKC)
{
    LPKEYCONTENT lpBrotherKC;

    for (; NULL != lpKC; lpKC = lpBrotherKC) {
        // Save pointer in local variable
        lpBrotherKC = lpKC->lpBrotherKC;

        // Increase count
        cCurrent++;

        // Free key name
        if (NULL != lpKC->lpszKeyName) {
            if ((NULL != lpKC->lpFatherKC)  // only the top KC can have HKLM/HKU, so ignore the sub KCs
                    || ((lpKC->lpszKeyName != lpszHKLMShort)
                        && (lpKC->lpszKeyName != lpszHKLMLong)
                        && (lpKC->lpszKeyName != lpszHKUShort)
                        && (lpKC->lpszKeyName != lpszHKULong) 
                        && (lpKC->lpszKeyName != lpszHKCUShort)
                        && (lpKC->lpszKeyName != lpszHKCULong))) {
                MYFREE(lpKC->lpszKeyName);
            }
        }

        // If the entry has values, then do a call for the first value
        if (NULL != lpKC->lpFirstVC) {
            FreeAllValueContents(lpKC->lpFirstVC);
        }

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }

        // If the entry has childs, then do a recursive call for the first child
        if (NULL != lpKC->lpFirstSubKC) {
            FreeAllKeyContents(lpKC->lpFirstSubKC);
        }

        // Free entry itself
        MYFREE(lpKC);
    }
}


// ----------------------------------------------------------------------
// Free shot completely and initialize
// ----------------------------------------------------------------------
VOID FreeShot(LPREGSHOT lpShot)
{
    if (NULL != lpShot->lpszComputerName) {
        MYFREE(lpShot->lpszComputerName);
    }

    if (NULL != lpShot->lpszUserName) {
        MYFREE(lpShot->lpszUserName);
    }

    if (NULL != lpShot->lpszFileName) {
        MYFREE(lpShot->lpszFileName);
    }

    if (NULL != lpShot->lpszTitle) {
        MYFREE(lpShot->lpszTitle);
    }

    if (NULL != lpShot->lpHKLM) {
        FreeAllKeyContents(lpShot->lpHKLM);
    }

    if (NULL != lpShot->lpHKU) {
        FreeAllKeyContents(lpShot->lpHKU);
    }

    if (NULL != lpShot->lpHKCU) {
        FreeAllKeyContents(lpShot->lpHKCU);
    }

    if (NULL != lpShot->lpHF) {
        FreeAllHeadFiles(lpShot->lpHF);
    }

    ZeroMemory(lpShot, sizeof(REGSHOT));

}


// ----------------------------------------------------------------------
// Get registry snap shot
// ----------------------------------------------------------------------
LPKEYCONTENT GetRegistrySnap(LPREGSHOT lpShot, HKEY hRegKey, LPTSTR lpszRegKeyName, LPKEYCONTENT lpFatherKC, LPKEYCONTENT *lplpCaller)
{
    LPKEYCONTENT lpKC;
    DWORD cSubKeys;
    DWORD cchMaxSubKeyName;
    LPTSTR lpszFullNameUI;

    // Process registry key itself, then key values with data, then sub keys (see msdn.microsoft.com/en-us/library/windows/desktop/ms724256.aspx)
    // Extra local block to reduce stack usage due to recursive calls
    {
        LPTSTR lpszFullName;
        DWORD cValues;
        DWORD cchMaxValueName;
        DWORD cbMaxValueData;

        if (!bRunning)
            return NULL;

        // Create new key content
        // put in a separate var for later use
        lpKC = MYALLOC0(sizeof(KEYCONTENT));

        // Set father of current key
        lpKC->lpFatherKC = lpFatherKC;

        // Set key name
        lpKC->lpszKeyName = lpszRegKeyName;
        lpKC->cchKeyName = _tcslen(lpKC->lpszKeyName);

        // Check if key is to be excluded
        lpszFullNameUI = GetWholeKeyName(lpKC, FALSE);
        if (NULL != pRegSkipList[0].lpSkipString) {  // only if there is something to exclude
            if ((NULL != lpKC->lpszKeyName) && (IsInSkipList(lpKC->lpszKeyName, pRegSkipList, FALSE))) {
                MYFREE(lpszFullNameUI);
                FreeAllKeyContents(lpKC);
                return NULL;
            }

            lpszFullName = GetWholeKeyName(lpKC, FALSE);
            if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
                MYFREE(lpszFullName);
                MYFREE(lpszFullNameUI);
                FreeAllKeyContents(lpKC);
                return NULL;
            }
            MYFREE(lpszFullName);
        }
        // Examine key for values and sub keys, get counts and also maximum lengths of names plus value data
        nErrNo = RegQueryInfoKey(
                     hRegKey,
                     NULL,
                     NULL,
                     NULL,
                     &cSubKeys,
                     &cchMaxSubKeyName,  // in TCHARs *not* incl. NULL char
                     NULL,
                     &cValues,
                     &cchMaxValueName,   // in TCHARs *not* incl. NULL char
                     &cbMaxValueData,
                     NULL,
                     NULL
                 );
        if (ERROR_SUCCESS != nErrNo) {
            // TODO: process/protocol issue in some way, do not silently ignore it (at least in Debug builds)
            FreeAllKeyContents(lpKC);
            return NULL;
        }

        // Copy pointer to current key into caller's pointer
        if (NULL != lplpCaller) {
            *lplpCaller = lpKC;
        }

        // Increase key count
        lpShot->stCounts.cKeys++;

        // Copy the registry values of the current key
        if (0 < cValues) {
            LPVALUECONTENT lpVC;
            LPVALUECONTENT *lplpVCPrev;
            DWORD i;
            DWORD cchValueName;
            DWORD nValueType;
            DWORD cbValueData;
#ifdef DEBUGLOG
            LPTSTR lpszDebugMsg;
#endif

            // Account for NULL char
            if (0 < cchMaxValueName) {
                cchMaxValueName++;
            }

            // Get buffer for maximum value name length
            nSourceSize = cchMaxValueName * sizeof(TCHAR);
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);

            // Get buffer for maximum value data length
            nSourceSize = cbMaxValueData;
            nDataBufferSize = AdjustBuffer(&lpDataBuffer, nDataBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);

            // Get registry key values
            lplpVCPrev = &lpKC->lpFirstVC;
            for (i = 0; ; i++) {
                // Enumerate value
                cchValueName = (DWORD)nStringBufferSize;
                cbValueData = (DWORD)nDataBufferSize;
                nErrNo = RegEnumValue(hRegKey, i,
                                      lpStringBuffer,
                                      &cchValueName,   // in TCHARs; in *including* and out *excluding* NULL char
                                      NULL,
                                      &nValueType,
                                      lpDataBuffer,
                                      &cbValueData);
                if (ERROR_NO_MORE_ITEMS == nErrNo) {
                    break;
                }
                if (ERROR_SUCCESS != nErrNo) {
                    // TODO: process/protocol issue in some way, do not silently ignore it (at least in Debug builds)
                    continue;
                }
                lpStringBuffer[cchValueName] = (TCHAR)'\0';  // safety NULL char

#ifdef DEBUGLOG
                DebugLog(lpszDebugTryToGetValueLog, TEXT("trying: "), FALSE);
                DebugLog(lpszDebugTryToGetValueLog, lpStringBuffer, TRUE);
#endif

                // Create new value content
                // put in a separate var for later use
                lpVC = MYALLOC0(sizeof(VALUECONTENT));

                // Set father key of current value to current key
                lpVC->lpFatherKC = lpKC;

                // Copy value name
                if (0 < cchValueName) {
                    lpVC->lpszValueName = MYALLOC(((size_t)cchValueName + 1) * sizeof(TCHAR));
                    _tcscpy(lpVC->lpszValueName, lpStringBuffer);
                    lpVC->cchValueName = _tcslen(lpVC->lpszValueName);
                }

                // Check if value is to be excluded
                if (NULL != pRegSkipList[i].lpSkipString) {  // only if there is something to exclude
                    if ((NULL != lpVC->lpszValueName) && (IsInSkipList(lpVC->lpszValueName, pRegSkipList, FALSE))) {
                        FreeAllValueContents(lpVC);
                        continue;  // ignore this entry and continue with next brother value
                    }

                    lpszFullName = GetWholeValueName(lpVC, FALSE);
                    if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
                        MYFREE(lpszFullName);
                        FreeAllValueContents(lpVC);
                        continue;  // ignore this entry and continue with next brother value
                    }
                    MYFREE(lpszFullName);
                }

                // Copy pointer to current value into previous value's next value pointer
                if (NULL != lplpVCPrev) {
                    *lplpVCPrev = lpVC;
                }

                // Increase value count
                lpShot->stCounts.cValues++;

                // Copy value meta data
                lpVC->nTypeCode = nValueType;
                lpVC->cbData = cbValueData;

                // Copy value data
                if (0 < cbValueData) {  // otherwise leave it NULL
                    lpVC->lpValueData = MYALLOC(cbValueData);
                    CopyMemory(lpVC->lpValueData, lpDataBuffer, cbValueData);
                }

#ifdef DEBUGLOG
                lpszDebugMsg = MYALLOC0(REGSHOT_DEBUG_MESSAGE_LENGTH * sizeof(TCHAR));
                _sntprintf(lpszDebugMsg, REGSHOT_DEBUG_MESSAGE_LENGTH, TEXT("LGVN:%08d LGVD:%08d VN:%08d VD:%08d\0"), cchMaxValueName, cbMaxValueData, cchValueName, cbValueData);
                lpszDebugMsg[REGSHOT_DEBUG_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
                DebugLog(lpszDebugValueNameDataLog, lpszDebugMsg, TRUE);
                MYFREE(lpszDebugMsg);

                lpszDebugMsg = GetWholeValueName(lpVC, FALSE);
                DebugLog(lpszDebugValueNameDataLog, lpszDebugMsg, FALSE);
                MYFREE(lpszDebugMsg);

//LPTSTR GetWholeValueData(LPTSTR lpszValueData[], LPVALUECONTENT lpVC, DWORD nActionType, size_t iNameLength, int iOutputType)
                lpszDebugMsg[0] = GetWholeValueData(lpszDebugMsg, lpVC, 0, 0, iOutputType);
                DebugLog(lpszDebugValueNameDataLog[0], lpszDebugMsg, TRUE);
                MYFREE(lpszDebugMsg);
#endif

                // Set "lpBrotherVC" pointer for storing the next brother's pointer
                lplpVCPrev = &lpVC->lpBrotherVC;
            }
        }
    }  // End of extra local block

    // Update counters display
    nCurrentTime = GetTickCount64();
    if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, lpszFullNameUI);
        MYFREE(lpszFullNameUI);
    }

    // Process sub keys
    if (0 < cSubKeys) {
        LPKEYCONTENT lpKCSub;
        LPKEYCONTENT *lplpKCPrev;
        DWORD i;
        LPTSTR lpszRegSubKeyName;
        HKEY hRegSubKey;

        // Account for NULL char
        if (0 < cchMaxSubKeyName) {
            cchMaxSubKeyName++;
        }

        // Get buffer for maximum sub key name length
        nSourceSize = cchMaxSubKeyName * sizeof(TCHAR);
        nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);

        // Get registry sub keys
        lplpKCPrev = &lpKC->lpFirstSubKC;
        for (i = 0; ; i++) {
            // Extra local block to reduce stack usage due to recursive calls
            {
                DWORD cchSubKeyName;
#ifdef DEBUGLOG
                LPTSTR lpszDebugMsg;
#endif

                // Enumerate sub key
                cchSubKeyName = (DWORD)nStringBufferSize;
                nErrNo = RegEnumKeyEx(hRegKey, i,
                                      lpStringBuffer,
                                      &cchSubKeyName,  // in TCHARs; in *including* and out *excluding* NULL char
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL);
                if (ERROR_NO_MORE_ITEMS == nErrNo) {
                    break;
                }
                if (ERROR_SUCCESS != nErrNo) {
                    // TODO: process/protocol issue in some way, do not silently ignore it (at least in Debug builds)
                    continue;
                }
                lpStringBuffer[cchSubKeyName] = (TCHAR)'\0';  // safety NULL char

                // Copy sub key name
                lpszRegSubKeyName = NULL;
                if (0 < cchSubKeyName) {
                    lpszRegSubKeyName = MYALLOC(((size_t)cchSubKeyName + 1) * sizeof(TCHAR));
                    _tcscpy(lpszRegSubKeyName, lpStringBuffer);
                }

#ifdef DEBUGLOG
                lpszDebugMsg = MYALLOC0(REGSHOT_DEBUG_MESSAGE_LENGTH * sizeof(TCHAR));
                _sntprintf(lpszDebugMsg, REGSHOT_DEBUG_MESSAGE_LENGTH, TEXT("LGKN:%08d KN:%08d\0"), cchMaxSubKeyName, cchSubKeyName);
                lpszDebugMsg[REGSHOT_DEBUG_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
                DebugLog(lpszDebugKeyLog, lpszDebugMsg, TRUE);
                MYFREE(lpszDebugMsg);

                lpszDebugMsg = GetWholeKeyName(lpKC, FALSE);
                DebugLog(lpszDebugKeyLog, lpszDebugMsg, FALSE);
                MYFREE(lpszDebugMsg);

                DebugLog(lpszDebugKeyLog, TEXT("\\"), FALSE);
                DebugLog(lpszDebugKeyLog, lpszRegSubKeyName, TRUE);
#endif
            }  // End of extra local block

            nErrNo = RegOpenKeyEx(hRegKey, lpszRegSubKeyName, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hRegSubKey);
            if (ERROR_SUCCESS != nErrNo) {
                // TODO: process/protocol issue in some way, do not silently ignore it (at least in Debug builds)
                //       e.g. when ERROR_ACCESS_DENIED then at least add key itself to the list
                MYFREE(lpszRegSubKeyName);
                continue;
            }

            lpKCSub = GetRegistrySnap(lpShot, hRegSubKey, lpszRegSubKeyName, lpKC, lplpKCPrev);
            RegCloseKey(hRegSubKey);

            // Set "lpBrotherKC" pointer for storing the next brother's pointer
            if (NULL != lpKCSub) {
                lplpKCPrev = &lpKCSub->lpBrotherKC;
            }
        }
    }

    return lpKC;
}


// ----------------------------------------------------------------------
// Shot Engine
// ----------------------------------------------------------------------
VOID Shot(LPREGSHOT lpShot)
{
    DWORD cchString;
    BOOL  bFirst = lpShot->fFirst;

    // Clear shot
    cEnd = 0;
    FreeShot(lpShot);
    lpShot->fFirst = bFirst;

    // Setup GUI for shot...
    UI_InitCounters();

    // New temporary buffers
    lpStringBuffer = NULL;
    lpDataBuffer = NULL;

    // Set computer name
    lpShot->lpszComputerName = MYALLOC0((MAX_COMPUTERNAME_LENGTH + 1) * sizeof(TCHAR));
    cchString = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(lpShot->lpszComputerName, &cchString);   // in TCHARs; in *including* and out *excluding* NULL char

    // Set user name
    lpShot->lpszUserName = MYALLOC0((UNLEN + 1) * sizeof(TCHAR));
    cchString = UNLEN + 1;
    GetUserName(lpShot->lpszUserName, &cchString);   // in TCHARs; in and out including NULL char

    // Set current system time
    GetSystemTime(&lpShot->systemtime);

    // Set user name
    lpShot->lpszFileName = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
    if (lpShot->lpszFileName != NULL)
        _tcscpy(lpShot->lpszFileName, TEXT("\0"));

    // Set title
//    GetDlgItemText(hMainWnd, IDC_EDITTITLE, lpShot->lpszTitle, MAX_PATH);
    lpShot->lpszTitle = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
    if (lpShot->lpszTitle != NULL)
        _tcscpy(lpShot->lpszTitle, lpszTitle);

//    if (1 == SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), BM_GETCHECK, (WPARAM)0, (LPARAM)0)) {
    if (TRUE == fHKLM) {
        // Take HKLM registry shot
        GetRegistrySnap(lpShot, HKEY_LOCAL_MACHINE, lpszHKLMShort, NULL, &lpShot->lpHKLM);
        // Update counters display (reg keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

//    if (1 == SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKU), BM_GETCHECK, (WPARAM)0, (LPARAM)0)) {
    if (TRUE == fHKU) {
        // Take HKU registry shot
        GetRegistrySnap(lpShot, HKEY_USERS, lpszHKUShort, NULL, &lpShot->lpHKU);
        // Update counters display (reg keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

//    if (1 == SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), BM_GETCHECK, (WPARAM)0, (LPARAM)0)) {
    if (TRUE == fHKCU) {
        // Take HKCU registry shot
        GetRegistrySnap(lpShot, HKEY_CURRENT_USER, lpszHKCUShort, NULL, &lpShot->lpHKCU);
        // Update counters display (reg keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Take file shot
//    if (1 == SendMessage(GetDlgItem(hMainWnd, IDC_CHECKDIR), BM_GETCHECK, (WPARAM)0, (LPARAM)0)) {
    if (TRUE == fFileshot) {
        FileShot(lpShot);

        // Update counters display (dirs/files final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextDir].lpszText, asLangTexts[iszTextFile].lpszText, lpShot->stCounts.cDirs, lpShot->stCounts.cFiles, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Get total count of all items
    lpShot->stCounts.cAll = lpShot->stCounts.cKeys
                            + lpShot->stCounts.cValues
                            + lpShot->stCounts.cDirs
                            + lpShot->stCounts.cFiles;
//    _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %us%03ums\0"), asLangTexts[iszTextTime].lpszText, (nCurrentTime - nStartTime) / 1000, (nCurrentTime - nStartTime) % 1000);
//    lpShot->duration = nCurrentTime - nStartTime;

    // Set flags
    lpShot->fFilled = TRUE;
    lpShot->fLoaded = FALSE;

    if (bRunning)
        UI_ShowHideCounters(SW_HIDE);

    if (NULL != lpStringBuffer) {
        MYFREE(lpStringBuffer);
        lpStringBuffer = NULL;
    }
    if (NULL != lpDataBuffer) {
        MYFREE(lpDataBuffer);
        lpDataBuffer = NULL;
    }
}




// ----------------------------------------------------------------------
// Save registry key with values to HIVE file
//
// This routine is called recursively to store the keys of the Registry tree
// Therefore temporary vars are put in a local block to reduce stack usage
// ----------------------------------------------------------------------
VOID SaveRegKeys(HANDLE hFile, LPREGSHOT lpShot, LPKEYCONTENT lpKC, DWORD nFPFatherKey, DWORD nFPCaller)
{
    DWORD nFPKey;

    for (; NULL != lpKC; lpKC = lpKC->lpBrotherKC) {
        // Get current file position
        // put in a separate var for later use
        nFPKey = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

        // Write position of current reg key in caller's field
        if (0 < nFPCaller) {
            WriteFileBuffer(hFile, nFPCaller, &nFPKey, sizeof(nFPKey));
        }

        // Initialize key content

        // Set file positions of the relatives inside the tree
#ifdef _UNICODE
        sKC.ofsKeyName = 0;      // not known yet, may be defined in this call
#endif
#ifndef _UNICODE
        // Key name will always be stored behind the structure, so its position is already known
        sKC.ofsKeyName = nFPKey + sizeof(sKC);
#endif
        sKC.ofsFirstValue = 0;   // not known yet, may be re-written in this iteration
        sKC.ofsFirstSubKey = 0;  // not known yet, may be re-written by another recursive call
        sKC.ofsBrotherKey = 0;   // not known yet, may be re-written in another iteration
        sKC.ofsFatherKey = nFPFatherKey;

        // New since key content version 2
        sKC.nKeyNameLen = 0;

        // Extra local block to reduce stack usage due to recursive calls
        {
            LPTSTR lpszKeyName;

            lpszKeyName = lpKC->lpszKeyName;

            // Determine correct key name and length plus file offset
            if (NULL != lpszKeyName) {
                sKC.nKeyNameLen = (DWORD)lpKC->cchKeyName;

                //if ((fUseLongRegHead) && (0 == nFPFatherKey)) {
                //    // Adopt to long HKLM/HKU
                //    if (lpszHKLMShort == lpszKeyName) {
                //        lpszKeyName = lpszHKLMLong;
                //        sKC.nKeyNameLen = (DWORD)_tcslen(lpszKeyName);;
                //    } else if (lpszHKUShort == lpszKeyName) {
                //        lpszKeyName = lpszHKULong;
                //        sKC.nKeyNameLen = (DWORD)_tcslen(lpszKeyName);;
                //    } else if (lpszHKCUShort == lpszKeyName) {
                //        lpszKeyName = lpszHKULong;
                //        sKC.nKeyNameLen = (DWORD)_tcslen(lpszKeyName);;
                //    }
                //}

                // Determine key name length and file offset
                if (0 < sKC.nKeyNameLen) {  // otherwise leave it all 0
                    sKC.nKeyNameLen++;  // account for NULL char
#ifdef _UNICODE
                    // Key name will always be stored behind the structure, so its position is already known
                    sKC.ofsKeyName = nFPKey + sizeof(sKC);
#endif
                }
            }

            // Increase count
            cCurrent++;

            // Write key content to file
            // Make sure that ALL fields have been initialized/set
            WriteFileBuffer(hFile, -1, &sKC, sizeof(sKC));

            // Write key name to file
            if (0 < sKC.nKeyNameLen) {
                WriteFileBuffer(hFile, -1, lpszKeyName, sKC.nKeyNameLen * sizeof(TCHAR));
#ifndef _UNICODE
            } else {
                // Write empty string for backward compatibility
                WriteFileBuffer(hFile, -1, lpszEmpty, 1 * sizeof(TCHAR));
#endif
            }
        }  // End of extra local block

        // Save the values of the current key
        if (NULL != lpKC->lpFirstVC) {
            LPVALUECONTENT lpVC;
            DWORD nFPValue;

            // Write all values of key
            nFPCaller = nFPKey + offsetof(SAVEKEYCONTENT, ofsFirstValue);  // Write position of first value into key
            for (lpVC = lpKC->lpFirstVC; NULL != lpVC; lpVC = lpVC->lpBrotherVC) {
                // Get current file position
                // put in a separate var for later use
                nFPValue = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

                // Write position of current reg value in caller's field
                if (0 < nFPCaller) {
                    WriteFileBuffer(hFile, nFPCaller, &nFPValue, sizeof(nFPValue));
                }

                // Initialize value content

                // Copy values
                sVC.nTypeCode = lpVC->nTypeCode;
                sVC.cbData = lpVC->cbData;

                // Set file positions of the relatives inside the tree
#ifdef _UNICODE
                sVC.ofsValueName = 0;       // not known yet, may be defined in this iteration
#endif
#ifndef _UNICODE
                // Value name will always be stored behind the structure, so its position is already known
                sVC.ofsValueName = nFPValue + sizeof(sVC);
#endif
                sVC.ofsValueData = 0;       // not known yet, may be re-written in this iteration
                sVC.ofsBrotherValue = 0;    // not known yet, may be re-written in next iteration
                sVC.ofsFatherKey = nFPKey;

                // New since value content version 2
                sVC.nValueNameLen = 0;

                // Determine value name length
                if (NULL != lpVC->lpszValueName) {
                    sVC.nValueNameLen = (DWORD)lpVC->cchValueName;
                    if (0 < sVC.nValueNameLen) {  // otherwise leave it all 0
                        sVC.nValueNameLen++;  // account for NULL char
#ifdef _UNICODE
                        // Value name will always be stored behind the structure, so its position is already known
                        sVC.ofsValueName = nFPValue + sizeof(sVC);
#endif
                    }
                }

                // Increase count
                cCurrent++;

                // Write value content to file
                // Make sure that ALL fields have been initialized/set
                WriteFileBuffer(hFile, -1, &sVC, sizeof(sVC));

                // Write value name to file
                if (0 < sVC.nValueNameLen) {
                    WriteFileBuffer(hFile, -1, lpVC->lpszValueName, sVC.nValueNameLen * sizeof(TCHAR));
#ifndef _UNICODE
                } else {
                    // Write empty string for backward compatibility
                    WriteFileBuffer(hFile, -1, lpszEmpty, 1 * sizeof(TCHAR));
#endif
                }

                // Write value data to file
                if (0 < sVC.cbData) {
                    DWORD nFPValueData;

                    // Write position of value data in value content field ofsValueData
                    nFPValueData = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

                    WriteFileBuffer(hFile, nFPValue + offsetof(SAVEVALUECONTENT, ofsValueData), &nFPValueData, sizeof(nFPValueData));

                    // Write value data
                    WriteFileBuffer(hFile, -1, lpVC->lpValueData, sVC.cbData);
                }

                // Set "ofsBrotherValue" position for storing the following brother's position
                nFPCaller = nFPValue + offsetof(SAVEVALUECONTENT, ofsBrotherValue);
            }
        }

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }

        // ATTENTION!!! sKC will be INVALID from this point on, due to recursive calls
        // If the entry has childs, then do a recursive call for the first child
        // Pass this entry as father and "ofsFirstSubKey" position for storing the first child's position
        if (NULL != lpKC->lpFirstSubKC) {
            SaveRegKeys(hFile, lpShot, lpKC->lpFirstSubKC, nFPKey, nFPKey + offsetof(SAVEKEYCONTENT, ofsFirstSubKey));
        }

        // Set "ofsBrotherKey" position for storing the following brother's position
        nFPCaller = nFPKey + offsetof(SAVEKEYCONTENT, ofsBrotherKey);
    }
}


// ----------------------------------------------------------------------
// Save registry and files to HIVE file
// ----------------------------------------------------------------------
VOID SaveShot(LPREGSHOT lpShot)
{
    OPENFILENAME opfn;
    LPTSTR filepath = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
//    LPTSTR pFilepath = &filepath;
    DWORD nFPCurrent;
    //UINT cchTitle;
    size_t cchString;

    // Check if there's anything to save
    if ((NULL == lpShot->lpHKLM) && (NULL == lpShot->lpHKU) && (NULL == lpShot->lpHKCU) && (NULL == lpShot->lpHF)) {
        return;  // leave silently
    }

    LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
//    GetDlgItemText(hMainWnd, IDC_EDITPATH, lpszOutputPath, MAX_PATH);  // length incl. NULL character
    ExpandEnvironmentStrings(lpszOutputPath, lpszTempDirBuffer, MAX_PATH);
    _tcscpy(lpszOutputPath, lpszTempDirBuffer);
    MYFREE(lpszTempDirBuffer);
    
//    cchTitle = GetDlgItemText(hMainWnd, IDC_EDITTITLE, lpszTitle, EXTDIRLEN);  // length incl. NULL character
    cchString = _tcslen(lpszOutputPath);
    if ((0 < cchString) && ((TCHAR)'\\' != *(lpszOutputPath + cchString - 1))) {
        *(lpszOutputPath + cchString) = (TCHAR)'\\';
        *(lpszOutputPath + cchString + 1) = (TCHAR)'\0';  // bug found by "itschy" <itschy@lycos.de> 1.61d->1.61e
        cchString++;
    }
    _tcscat(lpszOutputPath, lpszTitle);
    cchString = _tcslen(lpszOutputPath);
    if ((0 < cchString) && ((TCHAR)'\\' != *(lpszOutputPath + cchString - 1))) {
        *(lpszOutputPath + cchString) = (TCHAR)'\\';
        *(lpszOutputPath + cchString + 1) = (TCHAR)'\0';  // bug found by "itschy" <itschy@lycos.de> 1.61d->1.61e
        cchString++;
    }
    _tcscpy(lpszLastSaveDir, lpszOutputPath);

//    ZeroMemory(filepath, sizeof(filepath));                         // Clear Save File Name result buffer
    if (bNoGui) {
        _tcscpy(filepath, lpszOutputPath);
        _tcscat(filepath, lpszTitle);
        _tcscat(filepath, (lpShot->fFirst ? TEXT("_1") : TEXT("_2")));
        _tcscat(filepath, TEXT("."));
        _tcscat(filepath, lpszRegshotFileDefExt);
    }
    else {
        _tcscpy(filepath, lpszTitle);
        _tcscat(filepath, (lpShot->fFirst ? TEXT("_1") : TEXT("_2")));
    }

    // TODO: Subdirectory TITLE
    CreateDirectory(lpszLastSaveDir, 0);
    // Prepare Save File Name dialog
    ZeroMemory(&opfn, sizeof(opfn));
    opfn.lStructSize = sizeof(opfn);
    opfn.hwndOwner = hMainWnd;
    opfn.lpstrFilter = lpszFilter;
    opfn.lpstrFile = filepath;
    opfn.nMaxFile = MAX_PATH;  // incl. NULL character
    opfn.lpstrInitialDir = lpszLastSaveDir;
    opfn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    opfn.lpstrDefExt = lpszRegshotFileDefExt;

    // Display Save File Name dialog
    if (!bNoGui) {
        if (!GetSaveFileName(&opfn)) {
            return;  // leave silently
        }
    }

    // Open file for writing
    HANDLE hRegshotFile = CreateFile(opfn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hRegshotFile) {
        ErrMsg(asLangTexts[iszTextErrorCreateFile].lpszText);
        return;
    }
    _tcscpy(lpShot->lpszFileName, opfn.lpstrFile);
    //GetDlgItemText(hMainWnd, IDC_EDITTITLE, lpszTitle, MAX_PATH);  // length incl. NULL character

    // Setup GUI for saving...
    cEnd = lpShot->stCounts.cAll;
    UI_InitProgressBar();

    // Initialize file header
    ZeroMemory(&fileheader, sizeof(fileheader));
    ZeroMemory(&fileextradata, sizeof(fileextradata));

    // Copy SBCS/MBCS signature to header (even in Unicode builds for backwards compatibility)
    strncpy(fileheader.signature, szRegshotFileSignature, MAX_SIGNATURE_LENGTH);

    // Set file positions of hives inside the file
    fileheader.ofsHKLM = 0;   // not known yet, may be empty
    fileheader.ofsHKU = 0;    // not known yet, may be empty
    fileheader.ofsHKCU = 0;    // not known yet, may be empty
    fileheader.ofsHF = 0;  // not known yet, may be empty

    // Copy SBCS/MBCS strings to header (even in Unicode builds for backwards compatibility)
    if (NULL != lpShot->lpszComputerName) {
#ifdef _UNICODE
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, lpShot->lpszComputerName, -1, fileheader.computername, OLD_COMPUTERNAMELEN, NULL, NULL);
#else
        strncpy(fileheader.computername, lpShot->lpszComputerName, OLD_COMPUTERNAMELEN);
#endif
    }
    if (NULL != lpShot->lpszUserName) {
#ifdef _UNICODE
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, lpShot->lpszUserName, -1, fileheader.username, OLD_COMPUTERNAMELEN, NULL, NULL);
#else
        strncpy(fileheader.username, lpShot->lpszUserName, OLD_COMPUTERNAMELEN);
#endif
    }

    // Copy system time to header
    CopyMemory(&fileheader.systemtime, &lpShot->systemtime, sizeof(SYSTEMTIME));

    // new since header version 2
    fileheader.nFHSize = sizeof(fileheader);

    fileheader.nFHVersion = FILEHEADER_VERSION_CURRENT;
    fileheader.nCharSize = sizeof(TCHAR);

    fileheader.ofsComputerName = 0;  // not known yet, may be empty
    fileheader.nComputerNameLen = 0;
    if (NULL != lpShot->lpszComputerName) {
        fileheader.nComputerNameLen = (DWORD)_tcslen(lpShot->lpszComputerName);
        if (0 < fileheader.nComputerNameLen) {
            fileheader.nComputerNameLen++;  // account for NULL char
        }
    }

    fileheader.ofsUserName = 0;      // not known yet, may be empty
    fileheader.nUserNameLen = 0;
    if (NULL != lpShot->lpszUserName) {
        fileheader.nUserNameLen = (DWORD)_tcslen(lpShot->lpszUserName);
        if (0 < fileheader.nUserNameLen) {
            fileheader.nUserNameLen++;  // account for NULL char
        }
    }

    fileheader.nKCVersion = KEYCONTENT_VERSION_CURRENT;
    fileheader.nKCSize = sizeof(SAVEKEYCONTENT);

    fileheader.nVCVersion = VALUECONTENT_VERSION_CURRENT;
    fileheader.nVCSize = sizeof(SAVEVALUECONTENT);

    fileheader.nHFVersion = HEADFILE_VERSION_CURRENT;
    fileheader.nHFSize = sizeof(SAVEHEADFILE);

    fileheader.nFCVersion = FILECONTENT_VERSION_CURRENT;
    fileheader.nFCSize = sizeof(SAVEFILECONTENT);

    fileheader.nEndianness = FILEHEADER_ENDIANNESS_VALUE;

    fileheader.ofsTitle = 0;      // not known yet, may be empty
    fileheader.nTitleLen = 0;
    if (NULL != lpShot->lpszTitle) {
        fileheader.nTitleLen = (DWORD)_tcslen(lpShot->lpszTitle);
        if (0 < fileheader.nTitleLen) {
            fileheader.nTitleLen++;  // account for NULL char
        }
    }

    // Write header to file
    WriteFileBuffer(hRegshotFile, -1, &fileheader, sizeof(fileheader));

    // new since header version 2
    // (v2) full computername
    if (0 < fileheader.nComputerNameLen) {
        // Write position in file header
        nFPCurrent = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

        WriteFileBuffer(hRegshotFile, offsetof(FILEHEADER, ofsComputerName), &nFPCurrent, sizeof(nFPCurrent));
        fileheader.ofsComputerName = nFPCurrent;  // keep track in memory too

        // Write computername
        WriteFileBuffer(hRegshotFile, -1, lpShot->lpszComputerName, fileheader.nComputerNameLen * sizeof(TCHAR));
    }

    // (v2) full username
    if (0 < fileheader.nUserNameLen) {
        // Write position in file header
        nFPCurrent = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

        WriteFileBuffer(hRegshotFile, offsetof(FILEHEADER, ofsUserName), &nFPCurrent, sizeof(nFPCurrent));
        fileheader.ofsUserName = nFPCurrent;  // keep track in memory too

        // Write username
        WriteFileBuffer(hRegshotFile, -1, lpShot->lpszUserName, fileheader.nUserNameLen * sizeof(TCHAR));
    }

    // (v3) Title
    if (0 < fileheader.nTitleLen) {
        // Write position in file header
        nFPCurrent = fileextradata.ofsFBFile + fileextradata.ofsFBuffer;

        WriteFileBuffer(hRegshotFile, offsetof(FILEHEADER, ofsTitle), &nFPCurrent, sizeof(nFPCurrent));
        fileheader.ofsTitle = nFPCurrent;  // keep track in memory too

        // Write title
        WriteFileBuffer(hRegshotFile, -1, lpShot->lpszTitle, fileheader.nTitleLen * sizeof(TCHAR));
    }

    // Save HKLM
    if (NULL != lpShot->lpHKLM) {
        SaveRegKeys(hRegshotFile, lpShot, lpShot->lpHKLM, 0, offsetof(FILEHEADER, ofsHKLM));

        // Update progress bar display (keys/values final)
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            UI_UpdateProgressBar();
        }
    }

    // Save HKU
    if (NULL != lpShot->lpHKU) {
        SaveRegKeys(hRegshotFile, lpShot, lpShot->lpHKU, 0, offsetof(FILEHEADER, ofsHKU));

        // Update progress bar display (keys/values final)
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            UI_UpdateProgressBar();
        }
    }

    // (v3) Save HKCU
    if (NULL != lpShot->lpHKCU) {
        SaveRegKeys(hRegshotFile, lpShot, lpShot->lpHKCU, 0, offsetof(FILEHEADER, ofsHKCU));

        // Update progress bar display (keys/values final)
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            UI_UpdateProgressBar();
        }
    }

    // Save HEADFILEs
    if (NULL != lpShot->lpHF) {
        SaveHeadFiles(hRegshotFile, lpShot, lpShot->lpHF, offsetof(FILEHEADER, ofsHF));

        // Update progress bar display (dirs/files final)
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            UI_UpdateProgressBar();
        }
    }

    // Close file
    EmptyFileBuffer(hRegshotFile);
    CloseHandle(hRegshotFile);

    if (NULL != lpFileBuffer) {
        MYFREE(lpFileBuffer);
        lpFileBuffer = NULL;
    }

    // Update progress bar display (final)
    if (0 != cEnd) {
        nCurrentTime = GetTickCount64();
        UI_UpdateProgressBar();
    }

    UI_ShowHideProgressBar(SW_HIDE);

    // overwrite first letter of file name with NULL character to get path only, then create backup for initialization on next call
    *(opfn.lpstrFile + opfn.nFileOffset) = (TCHAR)'\0';  // TODO: check
    _tcscpy(lpszLastSaveDir, opfn.lpstrFile);

    MYFREE(filepath);
}



// ----------------------------------------------------------------------
// Load registry key with values from HIVE file
// ----------------------------------------------------------------------
VOID LoadRegKeys(LPREGSHOT lpShot, DWORD ofsKey, LPKEYCONTENT lpFatherKC, LPKEYCONTENT *lplpCaller)
{
    LPKEYCONTENT lpKC;
    DWORD ofsBrotherKey;

    if (fNoFiltersWhenLoading) {
        bRegSkipAdded = TRUE;
        bRegWhitelistAdded = TRUE;
    }
    // Read all reg keys and their value and sub key contents
    for (; 0 != ofsKey; ofsKey = ofsBrotherKey) {
        // Copy SAVEKEYCONTENT to aligned memory block
        CopyMemory(&sKC, (lpFileBuffer + ofsKey), fileheader.nKCSize);

        // Save offsets in local variables due to recursive calls
        ofsBrotherKey = sKC.ofsBrotherKey;

        // Create new key content
        // put in a separate var for later use
        lpKC = MYALLOC0(sizeof(KEYCONTENT));

        // Set father of current key
        lpKC->lpFatherKC = lpFatherKC;

        // Copy key name
        if (fileextradata.bOldKCVersion) {  // old SBCS/MBCS version
            sKC.nKeyNameLen = (DWORD)strlen((const char *)(lpFileBuffer + sKC.ofsKeyName));
            if (0 < sKC.nKeyNameLen) {
                sKC.nKeyNameLen++;  // account for NULL char
            }
        }
        if (0 < sKC.nKeyNameLen) {  // otherwise leave it NULL
            // Copy string to an aligned memory block
            nSourceSize = (size_t)sKC.nKeyNameLen * (size_t)fileheader.nCharSize;
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, (lpFileBuffer + sKC.ofsKeyName), nSourceSize);

            lpKC->lpszKeyName = MYALLOC(sKC.nKeyNameLen * sizeof(TCHAR));
            if (fileextradata.bSameCharSize) {
                _tcsncpy(lpKC->lpszKeyName, lpStringBuffer, sKC.nKeyNameLen);
            } else {
#ifdef _UNICODE
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, -1, lpKC->lpszKeyName, sKC.nKeyNameLen);
#else
                WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWSTR)lpStringBuffer, -1, lpKC->lpszKeyName, sKC.nKeyNameLen, NULL, NULL);
#endif
            }
            lpKC->lpszKeyName[sKC.nKeyNameLen - 1] = (TCHAR)'\0';  // safety NULL char

            // Set key name length in chars
            lpKC->cchKeyName = _tcslen(lpKC->lpszKeyName);
        }

        // Adopt to short HKLM/HKU/HKCU
        if (NULL == lpKC->lpFatherKC) {
            if ((0 == _tcscmp(lpKC->lpszKeyName, lpszHKLMShort))
                    || (0 == _tcscmp(lpKC->lpszKeyName, lpszHKLMLong))) {
                MYFREE(lpKC->lpszKeyName);
                lpKC->lpszKeyName = lpszHKLMShort;
                lpKC->cchKeyName = _tcslen(lpKC->lpszKeyName);
            } else if ((0 == _tcscmp(lpKC->lpszKeyName, lpszHKUShort))
                       || (0 == _tcscmp(lpKC->lpszKeyName, lpszHKULong))) {
                MYFREE(lpKC->lpszKeyName);
                lpKC->lpszKeyName = lpszHKUShort;
                lpKC->cchKeyName = _tcslen(lpKC->lpszKeyName);
            } else if ((0 == _tcscmp(lpKC->lpszKeyName, lpszHKCUShort))
                       || (0 == _tcscmp(lpKC->lpszKeyName, lpszHKCULong))) {
                MYFREE(lpKC->lpszKeyName);
                lpKC->lpszKeyName = lpszHKCUShort;
                lpKC->cchKeyName = _tcslen(lpKC->lpszKeyName);
            }
        }

        // Check if key is to be excluded
        if ((NULL != pRegSkipList[0].lpSkipString) && !fNoFiltersWhenLoading) {  // only if there is something to exclude
            LPTSTR lpszFullName;

            if ((NULL != lpKC->lpszKeyName) && (IsInSkipList(lpKC->lpszKeyName, pRegSkipList, FALSE))) {
                FreeAllKeyContents(lpKC);
                continue;  // ignore this entry and continue with next brother key
            }

            lpszFullName = GetWholeKeyName(lpKC, FALSE);
            if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
                MYFREE(lpszFullName);
                FreeAllKeyContents(lpKC);
                continue;  // ignore this entry and continue with next brother key
            }
            MYFREE(lpszFullName);
        }

        // Copy pointer to current key into caller's pointer
        if (NULL != lplpCaller) {
            *lplpCaller = lpKC;
        }

        // Increase key count
        lpShot->stCounts.cKeys++;

        // Copy the value contents of the current key
        if (0 != sKC.ofsFirstValue) {
            LPVALUECONTENT lpVC;
            LPVALUECONTENT *lplpCallerVC;
            DWORD ofsValue;
            LPTSTR lpszFullName;

            // Read all values of key
            lplpCallerVC = &lpKC->lpFirstVC;
            for (ofsValue = sKC.ofsFirstValue; 0 != ofsValue; ofsValue = sVC.ofsBrotherValue) {
                // Copy SAVEVALUECONTENT to aligned memory block
                CopyMemory(&sVC, (lpFileBuffer + ofsValue), fileheader.nVCSize);

                // Create new value content
                lpVC = MYALLOC0(sizeof(VALUECONTENT));

                // Set father key of current value to current key
                lpVC->lpFatherKC = lpKC;

                // Copy value name
                if (fileextradata.bOldVCVersion) {  // old SBCS/MBCS version
                    sVC.nValueNameLen = (DWORD)strlen((const char *)(lpFileBuffer + sVC.ofsValueName));
                    if (0 < sVC.nValueNameLen) {
                        sVC.nValueNameLen++;  // account for NULL char
                    }
                }
                if (0 < sVC.nValueNameLen) {  // otherwise leave it NULL
                    // Copy string to an aligned memory block
                    nSourceSize = (size_t)sVC.nValueNameLen * (size_t)fileheader.nCharSize;
                    nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);
                    ZeroMemory(lpStringBuffer, nStringBufferSize);
                    CopyMemory(lpStringBuffer, (lpFileBuffer + sVC.ofsValueName), nSourceSize);

                    lpVC->lpszValueName = MYALLOC(sVC.nValueNameLen * sizeof(TCHAR));
                    if (fileextradata.bSameCharSize) {
                        _tcsncpy(lpVC->lpszValueName, lpStringBuffer, sVC.nValueNameLen);
                    } else {
#ifdef _UNICODE
                        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, -1, lpVC->lpszValueName, sVC.nValueNameLen);
#else
                        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWSTR)lpStringBuffer, -1, lpVC->lpszValueName, sVC.nValueNameLen, NULL, NULL);
#endif
                    }
                    lpVC->lpszValueName[sVC.nValueNameLen - 1] = (TCHAR)'\0';  // safety NULL char

                    // Set value name length in chars
                    lpVC->cchValueName = _tcslen(lpVC->lpszValueName);
                }

                // Check if value is to be excluded
                if ((NULL != pRegSkipList[0].lpSkipString) && !fNoFiltersWhenLoading) {  // only if there is something to exclude
                    if ((NULL != lpVC->lpszValueName) && (IsInSkipList(lpVC->lpszValueName, pRegSkipList, FALSE))) {
                        FreeAllValueContents(lpVC);
                        continue;  // ignore this entry and continue with next brother value
                    }

                    lpszFullName = GetWholeValueName(lpVC, FALSE);
                    if (IsInSkipList(lpszFullName, pRegSkipList, FALSE)) {
                        MYFREE(lpszFullName);
                        FreeAllValueContents(lpVC);
                        continue;  // ignore this entry and continue with next brother value
                    }
                    MYFREE(lpszFullName);
                }

                // Copy pointer to current value into previous value's next value pointer
                if (NULL != lplpCallerVC) {
                    *lplpCallerVC = lpVC;
                }

                // Increase value count
                lpShot->stCounts.cValues++;

                // Copy value meta data
                lpVC->nTypeCode = sVC.nTypeCode;
                lpVC->cbData = sVC.cbData;

                // Copy value data
                if (0 < sVC.cbData) {  // otherwise leave it NULL
                    lpVC->lpValueData = MYALLOC(sVC.cbData);
                    CopyMemory(lpVC->lpValueData, (lpFileBuffer + sVC.ofsValueData), sVC.cbData);
                }

                // Set "lpBrotherVC" pointer for storing the next brother's pointer
                lplpCallerVC = &lpVC->lpBrotherVC;
            }
        }

        // Update counters display
        nCurrentTime = GetTickCount64();
        if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
            UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, TEXT("Loading Shot..."));
        }

        // ATTENTION!!! sKC will be INVALID from this point on, due to recursive calls
        // If the entry has childs, then do a recursive call for the first child
        // Pass this entry as father and "lpFirstSubKC" pointer for storing the first child's pointer
        if (0 != sKC.ofsFirstSubKey) {
            LoadRegKeys(lpShot, sKC.ofsFirstSubKey, lpKC, &lpKC->lpFirstSubKC);
        }

        // Set "lpBrotherKC" pointer for storing the next brother's pointer
        lplpCaller = &lpKC->lpBrotherKC;
    }
}


// ----------------------------------------------------------------------
// Load registry and files from HIVE file
// ----------------------------------------------------------------------
BOOL LoadShot(LPREGSHOT lpShot)
{
    OPENFILENAME opfn;
    TCHAR filepath[MAX_PATH+1];  // length incl. NULL character

    DWORD cbFileSize;
    DWORD cbFileRemain;
    DWORD cbReadSize;
    DWORD cbFileRead;
    BOOL  bFirst = lpShot->fFirst;

    // Clear Get File Name result buffer
    ZeroMemory(filepath, sizeof(filepath));

    // Prepare Open File Name dialog
    ZeroMemory(&opfn, sizeof(opfn));
    opfn.lStructSize = sizeof(opfn);
    opfn.hwndOwner = hMainWnd;
    opfn.lpstrFilter = lpszFilter;
    opfn.lpstrFile = filepath;
    opfn.nMaxFile = MAX_PATH;  // incl. NULL character
    opfn.lpstrInitialDir = lpszLastOpenDir;
    opfn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    opfn.lpstrDefExt = lpszRegshotFileDefExt;

    // Display Open File Name dialog
    if (!GetOpenFileName(&opfn)) {
        return FALSE;
    }

    // Open file for reading
    opfn.lpstrFile[MAX_PATH] = (TCHAR)'\0';
    HANDLE hRegshotFile = CreateFile(opfn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hRegshotFile) {
        ErrMsg(asLangTexts[iszTextErrorOpenFile].lpszText);
        return FALSE;
    }

    cbFileSize = GetFileSize(hRegshotFile, NULL);
    if (sizeof(fileheader) > cbFileSize) {
        CloseHandle(hRegshotFile);
        ErrMsg(TEXT("wrong filesize"));  //TODO: I18N, create text index and translate
        return FALSE;
    }

    // Initialize file header
    ZeroMemory(&fileheader, sizeof(fileheader));
    ZeroMemory(&fileextradata, sizeof(fileextradata));

    // Read first part of file header from file (signature, nHeaderSize)
    if (!ReadFile(hRegshotFile, &fileheader, offsetof(FILEHEADER, ofsHKLM), &NBW, NULL))
        return FALSE;

    // Check for valid file signatures (SBCS/MBCS and UTF-16 signature)
    if ((0 != strncmp(szRegshotFileSignatureSBCS, fileheader.signature, MAX_SIGNATURE_LENGTH)) && (0 != strncmp(szRegshotFileSignatureUTF16, fileheader.signature, MAX_SIGNATURE_LENGTH))) {
        CloseHandle(hRegshotFile);
        ErrMsg(TEXT("It is not a Regshot hive file!"));  //TODO: I18N, create text index and translate
        return FALSE;
    }

    // Check file signature for correct type (SBCS/MBCS or UTF-16)
    if (0 != strncmp(szRegshotFileSignature, fileheader.signature, MAX_SIGNATURE_LENGTH)) {
        CloseHandle(hRegshotFile);
#ifdef _UNICODE
        ErrMsg(TEXT("It is not a Unicode Regshot hive file! Try the ANSI version with it."));  //TODO: I18N, create text index and translate
#else
        ErrMsg(TEXT("It is not an ANSI Regshot hive file! Try the Unicode version with it."));  //TODO: I18N, create text index and translate
#endif
        return FALSE;
    }

    // Clear shot
    cEnd = 0;
    FreeShot(lpShot);
    lpShot->fFirst = bFirst;
    lpShot->lpszFileName = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
    _tcscpy(lpShot->lpszFileName, opfn.lpstrFile);

    // Allocate memory to hold the complete file
    lpFileBuffer = MYALLOC(cbFileSize);

    // Setup GUI for loading...
    cEnd = cbFileSize;
    UI_InitProgressBar();
    UI_UpdateCounters(lpszEmpty, lpszEmpty, 0, 0, lpszEmpty, TEXT("Loading file..."));

    // Read file blockwise for progress bar
    SetFilePointer(hRegshotFile, 0, NULL, FILE_BEGIN);
    cbFileRemain = cbFileSize;  // 100% to go
    cbReadSize = REGSHOT_READ_BLOCK_SIZE;  // next block length to read
    for (cbFileRead = 0; 0 < cbFileRemain; cbFileRead += cbReadSize) {
        // If the rest is smaller than a block, then use the rest length
        if (REGSHOT_READ_BLOCK_SIZE > cbFileRemain) {
            cbReadSize = cbFileRemain;
        }

        // Read the next block
        if (!ReadFile(hRegshotFile, lpFileBuffer + cbFileRead, cbReadSize, &NBW, NULL)) {
            CloseHandle(hRegshotFile);
            ErrMsg(TEXT("Reading ERROR!"));  //TODO: I18N, create text index and translate
            return FALSE;
        }
        if (NBW != cbReadSize) {
            CloseHandle(hRegshotFile);
            ErrMsg(TEXT("Reading ERROR!"));  //TODO: I18N, create text index and translate
            return FALSE;
        }

        // Determine how much to go, if zero leave the for-loop
        cbFileRemain -= cbReadSize;
        if (0 == cbFileRemain) {
            break;
        }

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                cCurrent = cbFileRead;
                UI_UpdateProgressBar();
            }
        }
    }

    // Close file
    CloseHandle(hRegshotFile);

    // Update progress bar display (load final)
    if (0 != cEnd) {
        nCurrentTime = GetTickCount64();
        UI_UpdateProgressBar();
    }

    cEnd = 0;
    UI_ShowHideProgressBar(SW_HIDE);

    // Setup GUI for parsing loaded file...
    UI_InitCounters();

    // Check size for copying file header
    nSourceSize = fileheader.nFHSize;
    if (0 == nSourceSize) {
        nSourceSize = offsetof(FILEHEADER, nFHVersion);
    } else if (sizeof(fileheader) < nSourceSize) {
        nSourceSize = sizeof(fileheader);
    }

    // Copy file header to structure
    CopyMemory(&fileheader, lpFileBuffer, nSourceSize);

    // Enhance data of old headers to be used with newer code
    if (FILEHEADER_VERSION_EMPTY == fileheader.nFHVersion) {
        if ((0 != fileheader.ofsHKLM) && (fileheader.ofsHKU == fileheader.ofsHKLM)) {
            fileheader.ofsHKLM = 0;
        }
        if ((0 != fileheader.ofsHKU) && (fileheader.ofsHF == fileheader.ofsHKU)) {
            fileheader.ofsHKU = 0;
        }
        if ((0 != fileheader.ofsHKCU) && (fileheader.ofsHF == fileheader.ofsHKCU)) {
            fileheader.ofsHKCU = 0;
        }

        fileheader.nFHVersion = FILEHEADER_VERSION_1;
        fileheader.nCharSize = 1;
        fileheader.nKCVersion = KEYCONTENT_VERSION_1;
        fileheader.nKCSize = offsetof(SAVEKEYCONTENT, nKeyNameLen);
        fileheader.nVCVersion = VALUECONTENT_VERSION_1;
        fileheader.nVCSize = offsetof(SAVEVALUECONTENT, nValueNameLen);
        fileheader.nHFVersion = HEADFILE_VERSION_1;
        fileheader.nHFSize = sizeof(SAVEHEADFILE);  // not changed yet, if it is then adopt to offsetof(SAVEHEADFILE, <first new field>)
        fileheader.nFCVersion = FILECONTENT_VERSION_1;
        fileheader.nFCSize = offsetof(SAVEFILECONTENT, nFileNameLen);
    }
    if (FILEHEADER_VERSION_2 >= fileheader.nFHVersion) {
        if (0 == fileheader.nEndianness) {
            *((unsigned char *)&fileheader.nEndianness)     = 0x78;  // old existing versions were all little endian
            *((unsigned char *)&fileheader.nEndianness + 1) = 0x56;
            *((unsigned char *)&fileheader.nEndianness + 2) = 0x34;
            *((unsigned char *)&fileheader.nEndianness + 3) = 0x12;
        }
    }

    // Check structure boundaries
    if (sizeof(SAVEKEYCONTENT) < fileheader.nKCSize) {
        fileheader.nKCSize = sizeof(SAVEKEYCONTENT);
    }
    if (sizeof(SAVEVALUECONTENT) < fileheader.nVCSize) {
        fileheader.nVCSize = sizeof(SAVEVALUECONTENT);
    }
    if (sizeof(SAVEHEADFILE) < fileheader.nHFSize) {
        fileheader.nHFSize = sizeof(SAVEHEADFILE);
    }
    if (sizeof(SAVEFILECONTENT) < fileheader.nFCSize) {
        fileheader.nFCSize = sizeof(SAVEFILECONTENT);
    }

    // Check for incompatible char size (known: 1 = SBCS/MBCS, 2 = UTF-16)
    if (sizeof(TCHAR) == fileheader.nCharSize) {
        fileextradata.bSameCharSize = TRUE;
    } else {
        fileextradata.bSameCharSize = FALSE;
#ifdef _UNICODE
        if (1 == fileheader.nCharSize) {
            ErrMsg(TEXT("It is not a Unicode Regshot hive file! Try the ANSI version with it."));  //TODO: I18N, create text index and translate
        } else if (2 != fileheader.nCharSize) {
            ErrMsg(TEXT("Unknown character size! Maybe created by a newer Regshot version."));  //TODO: I18N, create text index and translate
        } else {
            ErrMsg(TEXT("Unsupported character size!"));  //TODO: I18N, create text index and translate
        }
#else
        ErrMsg(TEXT("It is not an ANSI Regshot hive file! Try the Unicode version with it."));  //TODO: I18N, create text index and translate
#endif
        if (NULL != lpFileBuffer) {
            MYFREE(lpFileBuffer);
            lpFileBuffer = NULL;
        }
        return FALSE;
    }

    // Check for incompatible endianness (known: Intel = Little Endian)
    if (FILEHEADER_ENDIANNESS_VALUE == fileheader.nEndianness) {
        fileextradata.bSameEndianness = TRUE;
    } else {
        fileextradata.bSameEndianness = FALSE;
#ifdef __LITTLE_ENDIAN__
        ErrMsg(TEXT("It is not a Little Endian Regshot hive file!"));  //TODO: I18N, create text index and translate
#else
        ErrMsg(TEXT("It is not a Big Endian Regshot hive file!"));  //TODO: I18N, create text index and translate
#endif
        if (NULL != lpFileBuffer) {
            MYFREE(lpFileBuffer);
            lpFileBuffer = NULL;
        }
        return FALSE;
    }

    if (KEYCONTENT_VERSION_2 > fileheader.nKCVersion) {  // old SBCS/MBCS version
        fileextradata.bOldKCVersion = TRUE;
    } else {
        fileextradata.bOldKCVersion = FALSE;
    }

    if (VALUECONTENT_VERSION_2 > fileheader.nVCVersion) {  // old SBCS/MBCS version
        fileextradata.bOldVCVersion = TRUE;
    } else {
        fileextradata.bOldVCVersion = FALSE;
    }

    if (FILECONTENT_VERSION_2 > fileheader.nFCVersion) {  // old SBCS/MBCS version
        fileextradata.bOldFCVersion = TRUE;
    } else {
        fileextradata.bOldFCVersion = FALSE;
    }

    // ^^^ here the file header can be checked for additional extended content
    // * remember that files from older versions do not provide these additional data

    // New temporary string buffer
    lpStringBuffer = NULL;

    // Copy computer name from file header to shot data
    if (FILEHEADER_VERSION_1 < fileheader.nFHVersion) {  // newer Unicode/SBCS/MBCS version
        if (0 < fileheader.nComputerNameLen) {  // otherwise leave it NULL
            // Copy string to an aligned memory block
            nSourceSize = (size_t)fileheader.nComputerNameLen * (size_t)fileheader.nCharSize;
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, (lpFileBuffer + fileheader.ofsComputerName), nSourceSize);

            lpShot->lpszComputerName = MYALLOC(fileheader.nComputerNameLen * sizeof(TCHAR));
            if (fileextradata.bSameCharSize) {
                _tcsncpy(lpShot->lpszComputerName, lpStringBuffer, fileheader.nComputerNameLen);
            } else {
#ifdef _UNICODE
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, -1, lpShot->lpszComputerName, fileheader.nComputerNameLen);
#else
                WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWSTR)lpStringBuffer, -1, lpShot->lpszComputerName, fileheader.nComputerNameLen, NULL, NULL);
#endif
            }
            lpShot->lpszComputerName[fileheader.nComputerNameLen - 1] = (TCHAR)'\0';  // safety NULL char
        }
    } else {  // old SBCS/MBCS version
        // Copy string to an aligned memory block
        nSourceSize = strnlen((const char *)&fileheader.computername, OLD_COMPUTERNAMELEN);
        if (0 < nSourceSize) {  // otherwise leave it NULL
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, (nSourceSize + 1), REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, &fileheader.computername, nSourceSize);

            lpShot->lpszComputerName = MYALLOC((nSourceSize + 1) * sizeof(TCHAR));
#ifdef _UNICODE
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, (int)(nSourceSize + 1), lpShot->lpszComputerName, (int)(nSourceSize + 1));
#else
            strncpy(lpShot->lpszComputerName, lpStringBuffer, nSourceSize);
#endif
            lpShot->lpszComputerName[nSourceSize] = (TCHAR)'\0';  // safety NULL char
        }
    }

    // Copy user name from file header to shot data
    if (FILEHEADER_VERSION_1 < fileheader.nFHVersion) {  // newer Unicode/SBCS/MBCS version
        if (0 < fileheader.nUserNameLen) {  // otherwise leave it NULL
            // Copy string to an aligned memory block
            nSourceSize = (size_t)fileheader.nUserNameLen * (size_t)fileheader.nCharSize;
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, (lpFileBuffer + fileheader.ofsUserName), nSourceSize);

            lpShot->lpszUserName = MYALLOC(fileheader.nUserNameLen * sizeof(TCHAR));
            if (fileextradata.bSameCharSize) {
                _tcsncpy(lpShot->lpszUserName, lpStringBuffer, fileheader.nUserNameLen);
            } else {
#ifdef _UNICODE
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, -1, lpShot->lpszUserName, fileheader.nUserNameLen);
#else
                WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWSTR)lpStringBuffer, -1, lpShot->lpszUserName, fileheader.nUserNameLen, NULL, NULL);
#endif
            }
            lpShot->lpszUserName[fileheader.nUserNameLen - 1] = (TCHAR)'\0';  // safety NULL char
        }
    } else {  // old SBCS/MBCS version
        // Copy string to an aligned memory block
        nSourceSize = strnlen((const char *)&fileheader.username, OLD_COMPUTERNAMELEN);
        if (0 < nSourceSize) {  // otherwise leave it NULL
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, (nSourceSize + 1), REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, &fileheader.username, nSourceSize);

            lpShot->lpszUserName = MYALLOC((nSourceSize + 1) * sizeof(TCHAR));
#ifdef _UNICODE
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, (int)(nSourceSize + 1), lpShot->lpszUserName, (int)(nSourceSize + 1));
#else
            strncpy(lpShot->lpszUserName, lpStringBuffer, nSourceSize);
#endif
            lpShot->lpszUserName[nSourceSize] = (TCHAR)'\0';  // safety NULL char
        }
    }

    // Copy title from file header to shot data
    if (FILEHEADER_VERSION_2 < fileheader.nFHVersion) {  // newer Unicode/SBCS/MBCS version
        if (0 < fileheader.nTitleLen) {  // otherwise leave it NULL
            // Copy string to an aligned memory block
            nSourceSize = (size_t)fileheader.nTitleLen * (size_t)fileheader.nCharSize;
            nStringBufferSize = AdjustBuffer(&lpStringBuffer, nStringBufferSize, nSourceSize, REGSHOT_BUFFER_BLOCK_BYTES);
            ZeroMemory(lpStringBuffer, nStringBufferSize);
            CopyMemory(lpStringBuffer, (lpFileBuffer + fileheader.ofsTitle), nSourceSize);

            lpShot->lpszTitle = MYALLOC(fileheader.nTitleLen * sizeof(TCHAR));
            if (fileextradata.bSameCharSize) {
                _tcsncpy(lpShot->lpszTitle, lpStringBuffer, fileheader.nTitleLen);
            }
            else {
#ifdef _UNICODE
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)lpStringBuffer, -1, lpShot->lpszTitle, fileheader.nTitleLen);
#else
                WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWSTR)lpStringBuffer, -1, lpShot->lpszTitle, fileheader.nTitleLen, NULL, NULL);
#endif
            }
            lpShot->lpszTitle[fileheader.nTitleLen - 1] = (TCHAR)'\0';  // safety NULL char
        }
    }

    CopyMemory(&lpShot->systemtime, &fileheader.systemtime, sizeof(SYSTEMTIME));

    // Initialize save structures
    ZeroMemory(&sKC, sizeof(sKC));
    ZeroMemory(&sVC, sizeof(sVC));

    if (0 != fileheader.ofsHKLM) {
        LoadRegKeys(lpShot, fileheader.ofsHKLM, NULL, &lpShot->lpHKLM);

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    if (0 != fileheader.ofsHKU) {
        LoadRegKeys(lpShot, fileheader.ofsHKU, NULL, &lpShot->lpHKU);

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    if (0 != fileheader.ofsHKCU) {
        LoadRegKeys(lpShot, fileheader.ofsHKCU, NULL, &lpShot->lpHKCU);

        // Update counters display (keys/values final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextKey].lpszText, asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cKeys, lpShot->stCounts.cValues, asLangTexts[iszTextKey].lpszText, NULL);
    }

    if (0 != fileheader.ofsHF) {
        LoadHeadFiles(lpShot, fileheader.ofsHF, &lpShot->lpHF);

        // Update counters display (dirs/files final)
        nCurrentTime = GetTickCount64();
        UI_UpdateCounters(asLangTexts[iszTextDir].lpszText, asLangTexts[iszTextFile].lpszText, lpShot->stCounts.cDirs, lpShot->stCounts.cFiles, asLangTexts[iszTextKey].lpszText, NULL);
    }

    // Get total count of all items
    lpShot->stCounts.cAll = lpShot->stCounts.cKeys
                            + lpShot->stCounts.cValues
                            + lpShot->stCounts.cDirs
                            + lpShot->stCounts.cFiles;

    // Set GUI fields after loading...
    if (NULL != lpShot->lpHF) {
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECKDIR), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECKDIR, (LPARAM)0);

        FindDirChain(lpShot->lpHF, lpszExtDir, EXTDIRLEN);  // Get new chains, must do this after loading! Length in TCHARs incl. NULL char
        SetDlgItemText(hMainWnd, IDC_EDITDIR, lpszExtDir);
    } else {
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECKDIR), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECKDIR, (LPARAM)0);
//        SetDlgItemText(hWnd, IDC_EDITDIR, lpszEmpty);
    }
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKLM, (LPARAM)0);
    if (NULL != lpShot->lpHKLM) {
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKLM, (LPARAM)0);
    }
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKU), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKU, (LPARAM)0);
    if (NULL != lpShot->lpHKU) {
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKU), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKU, (LPARAM)0);
    }
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKCU, (LPARAM)0);
    if (NULL != lpShot->lpHKCU) {
        SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDC_CHECK_HKCU, (LPARAM)0);
    }

    if (NULL != lpShot->lpszTitle)
        SetDlgItemText(hMainWnd, IDC_EDITTITLE, lpShot->lpszTitle);

    if (NULL != lpStringBuffer) {
        MYFREE(lpStringBuffer);
        lpStringBuffer = NULL;
    }

    if (NULL != lpFileBuffer) {
        MYFREE(lpFileBuffer);
        lpFileBuffer = NULL;
    }

    // Set flags
    lpShot->fFilled = TRUE;
    lpShot->fLoaded = TRUE;

    UI_ShowHideCounters(SW_HIDE);

    // overwrite first letter of file name with NULL character to get path only, then create backup for initialization on next call
    *(opfn.lpstrFile + opfn.nFileOffset) = 0x00;
    _tcscpy(lpszLastOpenDir, opfn.lpstrFile);

    return TRUE;
}

// ----------------------------------------------------------------------
// Display details of shot in message box
// ----------------------------------------------------------------------
VOID DisplayShotInfo(HWND hDlg, LPREGSHOT lpShot)
{
    LPTSTR lpszInfoBox;
    LPTSTR lpszComputerName;
    LPTSTR lpszUserName;
    LPTSTR lpszLoaded;
    LPTSTR lpszInfoTitle;

    lpszComputerName = lpShot->lpszComputerName;
    if (NULL == lpszComputerName) {
        lpszComputerName = lpszEmpty;
    }
    lpszUserName = lpShot->lpszUserName;
    if (NULL == lpszUserName) {
        lpszUserName = lpszEmpty;
    }
    lpszLoaded = lpszEmpty;
    if (lpShot->fLoaded) {
        lpszLoaded = asLangTexts[iszTextLoadedFromFile].lpszText;
    }

    lpszInfoBox = MYALLOC0(SIZEOF_INFOBOX * sizeof(TCHAR));
    if (lpszInfoBox != NULL) {
        _sntprintf(lpszInfoBox, SIZEOF_INFOBOX, TEXT("%s %04d-%02d-%02d %02d:%02d:%02d\n%s %s\n%s %s\n%s %u\n%s %u\n%s %u\n%s %u\n%s\0"),
            asLangTexts[iszTextDateTime].lpszText,
            lpShot->systemtime.wYear, lpShot->systemtime.wMonth, lpShot->systemtime.wDay,
            lpShot->systemtime.wHour, lpShot->systemtime.wMinute, lpShot->systemtime.wSecond,
            asLangTexts[iszTextComputer].lpszText, lpszComputerName,
            asLangTexts[iszTextUsername].lpszText, lpszUserName,
            asLangTexts[iszTextKey].lpszText, lpShot->stCounts.cKeys,
            asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cValues,
            asLangTexts[iszTextDir].lpszText, lpShot->stCounts.cDirs,
            asLangTexts[iszTextFile].lpszText, lpShot->stCounts.cFiles,
            lpszLoaded
        );
        lpszInfoBox[SIZEOF_INFOBOX - 1] = (TCHAR)'\0';  // safety NULL char
    }

    lpszInfoTitle = asLangTexts[iszTextMenuInfo].lpszText;
    if (&Shot1 == lpShot) {
        lpszInfoTitle = asLangTexts[iszTextButtonShot1].lpszText;
    } else if (&Shot2 == lpShot) {
        lpszInfoTitle = asLangTexts[iszTextButtonShot2].lpszText;
    }

    MessageBox(hDlg, lpszInfoBox, lpszInfoTitle, MB_OK);
    
    MYFREE(lpszInfoBox);
}


// ----------------------------------------------------------------------
// Display details of comparison result in message box
// ----------------------------------------------------------------------
VOID DisplayResultInfo(HWND hDlg)
{
    LPTSTR lpszInfoBox;
    LPTSTR lpszInfoBoxTitle;

    lpszInfoBox = MYALLOC0(SIZEOF_INFOBOX * sizeof(TCHAR));
    if (lpszInfoBox != NULL) {
        _sntprintf(lpszInfoBox, SIZEOF_INFOBOX, TEXT("%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n%s %u\n\0"),
            asLangTexts[iszTextKeyDel].lpszText, CompareResult.stcDeleted.cKeys,
            asLangTexts[iszTextKeyAdd].lpszText, CompareResult.stcAdded.cKeys,
            asLangTexts[iszTextValDel].lpszText, CompareResult.stcDeleted.cValues,
            asLangTexts[iszTextValAdd].lpszText, CompareResult.stcAdded.cValues,
            asLangTexts[iszTextValModi].lpszText, CompareResult.stcModified.cValues,
            asLangTexts[iszTextDirDel].lpszText, CompareResult.stcDeleted.cDirs,
            asLangTexts[iszTextDirAdd].lpszText, CompareResult.stcAdded.cDirs,
            asLangTexts[iszTextDirModi].lpszText, CompareResult.stcModified.cDirs,
            asLangTexts[iszTextFileDel].lpszText, CompareResult.stcDeleted.cFiles,
            asLangTexts[iszTextFileAdd].lpszText, CompareResult.stcAdded.cFiles,
            asLangTexts[iszTextFileModi].lpszText, CompareResult.stcModified.cFiles,
            asLangTexts[iszTextTotal].lpszText, CompareResult.stcChanged.cAll
        );
        lpszInfoBox[SIZEOF_INFOBOX - 1] = (TCHAR)'\0';  // safety NULL char
    }

    lpszInfoBoxTitle = asLangTexts[iszTextButtonCompare].lpszText;

    MessageBox(hDlg, lpszInfoBox, lpszInfoBoxTitle, MB_OK);
    MYFREE(lpszInfoBox);
}


// ----------------------------------------------------------------------
// Swap shot 1 and shot 2
// ----------------------------------------------------------------------
VOID SwapShots(VOID)
{
    REGSHOT ShotTemp;

    memcpy(&ShotTemp, &Shot1, sizeof(Shot1));     // backup Shot1 in ShotTemp
    memcpy(&Shot1, &Shot2, sizeof(Shot2));        // copy Shot2 to Shot1
    memcpy(&Shot2, &ShotTemp, sizeof(ShotTemp));  // copy ShotTemp (Shot1) to Shot2
}


// ----------------------------------------------------------------------
// Check if shot 1 and shot 2 are in chronological order, otherwise prompt to swap
//   YES = swap, then proceed (returns TRUE)
//   NO = no swap, then proceed (returns TRUE)
//   CANCEL = no swap, then cancel functions (returns FALSE)
// ----------------------------------------------------------------------
BOOL CheckShotsChronology(HWND hDlg)
{
    FILETIME ftime1;
    FILETIME ftime2;
    LPTSTR lpszChronoBox;
    LPTSTR lpszChronoTitle;
    int nDialogAnswer;

    // CANCEL if not both shots filled
    if ((!Shot1.fFilled) || (!Shot2.fFilled)) {
        return FALSE;
    }

    // Compare time stamps of shots
    SystemTimeToFileTime(&(Shot1.systemtime), &ftime1);
    SystemTimeToFileTime(&(Shot2.systemtime), &ftime2);
    if (0 >= CompareFileTime(&ftime1, &ftime2)) {
        return TRUE;
    }

    // Define texts of message box  //TODO: I18N, create text indexes and translate
    lpszChronoBox = MYALLOC0(SIZEOF_CHRONOBOX * sizeof(TCHAR));
    if (lpszChronoBox != NULL) {
        _sntprintf(lpszChronoBox, SIZEOF_CHRONOBOX, TEXT("%s\n%s %s %04d-%02d-%02d %02d:%02d:%02d\n%s %s %04d-%02d-%02d %02d:%02d:%02d\n%s\n\0"),
            asLangTexts[iszTextShotsNotChronological].lpszText,
            asLangTexts[iszTextButtonShot1].lpszText, asLangTexts[iszTextDateTime].lpszText,
            Shot1.systemtime.wYear, Shot1.systemtime.wMonth, Shot1.systemtime.wDay,
            Shot1.systemtime.wHour, Shot1.systemtime.wMinute, Shot1.systemtime.wSecond,
            asLangTexts[iszTextButtonShot2].lpszText, asLangTexts[iszTextDateTime].lpszText,
            Shot2.systemtime.wYear, Shot2.systemtime.wMonth, Shot2.systemtime.wDay,
            Shot2.systemtime.wHour, Shot2.systemtime.wMinute, Shot2.systemtime.wSecond,
            asLangTexts[iszTextQuestionSwapShots].lpszText
        );
        lpszChronoBox[SIZEOF_CHRONOBOX - 1] = (TCHAR)'\0';  // safety NULL char
    }

    lpszChronoTitle = MYALLOC0(SIZEOF_CHRONOBOX * sizeof(TCHAR));
    if (lpszChronoTitle != NULL) {
        _sntprintf(lpszChronoTitle, SIZEOF_CHRONOBOX, TEXT("%s %s\0"),
                   asLangTexts[iszTextChronology].lpszText,
                   asLangTexts[iszTextWarning].lpszText
                  );
        lpszChronoTitle[SIZEOF_CHRONOBOX - 1] = (TCHAR)'\0';  // safety NULL char
    }

    nDialogAnswer = MessageBox(hDlg, lpszChronoBox, lpszChronoTitle, MB_ICONWARNING | MB_YESNOCANCEL | MB_DEFBUTTON2);
    if (IDCANCEL == nDialogAnswer) {
        return FALSE;
    }

    if (IDYES == nDialogAnswer) {
        SwapShots();
        cEnd = CompareResult.stcChanged.cAll;
        UI_InitProgressBar();
        FreeCompareResult();
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            UI_UpdateProgressBar();
        }
        ClearRegKeyMatchFlags(Shot1.lpHKLM);
        ClearRegKeyMatchFlags(Shot2.lpHKLM);
        ClearRegKeyMatchFlags(Shot1.lpHKU);
        ClearRegKeyMatchFlags(Shot2.lpHKU);
        ClearHeadFileMatchFlags(Shot1.lpHF);
        ClearHeadFileMatchFlags(Shot2.lpHF);
        UI_ShowHideProgressBar(SW_HIDE);
    }

    return TRUE;
}

// ----------------------------------------------------------------------
//   YES = swap, then proceed (returns TRUE)
//   NO = no swap, then proceed (returns TRUE)
//   CANCEL = no swap, then cancel functions (returns FALSE)
// ----------------------------------------------------------------------
BOOL CheckSavingShots(HWND hDlg, LPREGSHOT lpShot)
{
    LPTSTR lpszSavingBox;
    LPTSTR lpszSavingTitle;
    int nDialogAnswer;
    LPTSTR lpszComputerName;
    LPTSTR lpszUserName;

    if (fStoreOnlyUncompared && CompareResult.fFilled)
        return TRUE;

    if (!lpShot->fFilled || lpShot->fLoaded)
        return TRUE;

    if (_tcscmp(lpShot->lpszFileName, lpszEmpty) != 0)
        return TRUE;

    lpszComputerName = lpShot->lpszComputerName;
    if (NULL == lpszComputerName) {
        lpszComputerName = lpszEmpty;
    }
    lpszUserName = lpShot->lpszUserName;
    if (NULL == lpszUserName) {
        lpszUserName = lpszEmpty;
    }

    // Define texts of message box  //TODO: I18N, create text indexes and translate
    lpszSavingBox = MYALLOC0(SIZEOF_SAVINGBOX * sizeof(TCHAR));
    if (lpszSavingBox != NULL) {
        _sntprintf(lpszSavingBox, SIZEOF_SAVINGBOX, TEXT("%s\n\n%s %04d-%02d-%02d %02d:%02d:%02d\n%s %s\n%s %s\n%s %u\n%s %u\n%s %u\n%s %u\0"),
            _TEXT("Save shot?"),
            asLangTexts[iszTextDateTime].lpszText,
            lpShot->systemtime.wYear, lpShot->systemtime.wMonth, lpShot->systemtime.wDay,
            lpShot->systemtime.wHour, lpShot->systemtime.wMinute, lpShot->systemtime.wSecond,
            asLangTexts[iszTextComputer].lpszText, lpszComputerName,
            asLangTexts[iszTextUsername].lpszText, lpszUserName,
            asLangTexts[iszTextKey].lpszText, lpShot->stCounts.cKeys,
            asLangTexts[iszTextValue].lpszText, lpShot->stCounts.cValues,
            asLangTexts[iszTextDir].lpszText, lpShot->stCounts.cDirs,
            asLangTexts[iszTextFile].lpszText, lpShot->stCounts.cFiles
        );
        lpszSavingBox[SIZEOF_SAVINGBOX - 1] = (TCHAR)'\0';  // safety NULL char
    }
    if (&Shot1 == lpShot) {
        lpszSavingTitle = asLangTexts[iszTextButtonShot1].lpszText;
    }
    else {
        lpszSavingTitle = asLangTexts[iszTextButtonShot2].lpszText;
    }
    //_sntprintf(lpszSavingTitle, SIZEOF_SAVINGBOX, TEXT("%s %s\0"),
    //           asLangTexts[iszTextChronology].lpszText,
    //           asLangTexts[iszTextWarning].lpszText
    //          );
    lpszSavingTitle[SIZEOF_SAVINGBOX - 1] = (TCHAR)'\0';  // safety NULL char
    nDialogAnswer = MessageBox(hDlg, lpszSavingBox, lpszSavingTitle, MB_ICONQUESTION | MB_YESNOCANCEL | MB_DEFBUTTON2);
    if (IDCANCEL == nDialogAnswer) {
        return FALSE;
    }

    if (IDYES == nDialogAnswer) {
        bRunning = TRUE;
        UI_SetHourGlassCursor();
        SaveShot(lpShot);
        bRunning = FALSE;
        UI_RemoveHourGlassCursor();
        UI_EnableMainButtons();
        MessageBeep(0xffffffff);
    }

    return TRUE;
}

