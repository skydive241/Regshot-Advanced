/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
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

/* This file orignal coded by tulipfan
   Change function/variable name to more proper ones and fix for x64 by tianwei
   Totally rewritten to have a plethora of settings available in a logical order (skydive241@gmx.de)
*/

#include "global.h"

// 1.8.2 move definition from global.h to this place
// setup based on regshot.ini by tulipfan (tfx)

// Strings containing names of sections and values
// Section [Setup]
LPTSTR lpszIniSetup                             = TEXT("Setup");
LPTSTR lpszIniTitle                             = TEXT("Title");
LPTSTR lpszIniLanguage                          = TEXT("Language");
LPTSTR lpszIniOutDir                            = TEXT("BaseDir");
LPTSTR lpszIniEditor                            = TEXT("Editor");
LPTSTR lpszIniExtDir                            = TEXT("ExtDir");
LPTSTR lpszIniDontDisplayInfoAfterShot          = TEXT("DontDisplayInfoAfterShot");
LPTSTR lpszIniDontDisplayInfoAfterComparison    = TEXT("DontDisplayInfoAfterComparison");
LPTSTR lpszIniSaveSettingsOnExit                = TEXT("SaveSettingsOnExit");
LPTSTR lpszIniNoFiltersWhenLoading              = TEXT("NoFiltersWhenLoading");
LPTSTR lpszIniShowSIDFilterRules                = TEXT("ShowSIDFilterRules");
LPTSTR lpszIniStartDefaults                     = TEXT("StartDefaultActionsByButtons");
LPTSTR lpszIniAutoCompare                       = TEXT("AutoCompare");
LPTSTR lpszIniStoreOnQuit                       = TEXT("StoreOnQuit");
LPTSTR lpszIniStoreOnlyUncompared               = TEXT("StoreOnlyUncompared");
LPTSTR lpszIniExpandLevels                      = TEXT("ExpandLevels");
LPTSTR lpszIniMaxNodes                          = TEXT("MaxNodes");
LPTSTR lpszIniMaxResultLines                    = TEXT("MaxLines");

// Section [Registry-Scan]
LPTSTR lpszIniRegistryScan                      = TEXT("Registry-Scan");
LPTSTR lpszIniHKLM                              = TEXT("HKEY_LOCAL_MACHINE");
LPTSTR lpszIniHKU                               = TEXT("HKEY_USERS");
LPTSTR lpszIniHKCU                              = TEXT("HKEY_CURRENT_USER");

// Section [Skips]
LPTSTR lpszIniScanDir                           = TEXT("ScanDir");
LPTSTR lpszIniSkipRegKey                        = TEXT("SkipRegKey");
LPTSTR lpszIniWhitelistRegKey                   = TEXT("RegKeyWhitelist");
LPTSTR lpszIniSkipDir                           = TEXT("SkipDir");
LPTSTR lpszIniSkipFile                          = TEXT("SkipFile");

// Section [Output]
LPTSTR lpszIniOutput                            = TEXT("Output");
LPTSTR lpszIniFlag                              = TEXT("Flag");
LPTSTR lpszIniOpenEditor                        = TEXT("OpenEditor");
LPTSTR lpszIniOutputTXTFile                     = TEXT("TXTFile");
LPTSTR lpszIniOutputUNLFile                     = TEXT("UNLFile");
LPTSTR lpszIniOutputBATFile                     = TEXT("BATFile");
LPTSTR lpszIniOutputHTMFile                     = TEXT("HTMFile");
LPTSTR lpszIniOutputISSDeinstallFile            = TEXT("ISSDeinstallFile");
LPTSTR lpszIniOutputISSInstallFile              = TEXT("ISSInstallFile");
LPTSTR lpszIniOutputNSIDeinstallFile            = TEXT("NSIDeinstallFile");
LPTSTR lpszIniOutputNSIInstallFile              = TEXT("NSIInstallFile");
LPTSTR lpszIniOutputRegDeinstallFile            = TEXT("RegDeinstallFile");
LPTSTR lpszIniOutputRegInstallFile              = TEXT("RegInstallFile");
LPTSTR lpszIniUseLongRegHead                    = TEXT("UseLongRegHead");
LPTSTR lpszIniOutSeparateObjs                   = TEXT("OutSeparateObjs");
LPTSTR lpszIniOutMaxBinaryBytes                 = TEXT("OutMaxBinaryBytes");
LPTSTR lpszIniOutMaxResultLines                 = TEXT("OutMaxResultLines");
LPTSTR lpszIniLogEnvironmentStrings             = TEXT("LogEnvironmentStrings");
LPTSTR lpszIniSuppressLogs                      = TEXT("SuppressLogs");
LPTSTR lpszIniLogUNLOrder                       = TEXT("LogUNLOrder");
LPTSTR lpszIniDeleteReadOnly                    = TEXT("DeleteReadOnly");
LPTSTR lpszIniDeleteDirNotEmpty                 = TEXT("DeleteDirNotEmpty");
LPTSTR lpszIniCheckResult                       = TEXT("CheckResult");

// Section [UNL]
LPTSTR lpszIniUNL                               = TEXT("UNL");
LPTSTR lpszIniOnlyNewEntries                    = TEXT("OnlyNewEntries");
LPTSTR lpszIniNoVals                            = TEXT("NoVals");

// Section [ISS]
LPTSTR lpszIniISS                               = TEXT("ISS");
LPTSTR lpszIniUseDifferentISSOutputFolder       = TEXT("UseDifferentISSOutputFolder");
LPTSTR lpszIniISSOutputFolder                   = TEXT("ISSOutputFolder"); 
LPTSTR lpszIniUseDifferentISSEditor             = TEXT("UseDifferentISSEditor");
LPTSTR lpszIniISSEditor                         = TEXT("ISSEditor");

// Section [NSI]
LPTSTR lpszIniNSI                               = TEXT("NSI");
LPTSTR lpszIniUseDifferentNSIOutputFolder       = TEXT("UseDifferentNSIOutputFolder");
LPTSTR lpszIniNSIOutputFolder                   = TEXT("NSIOutputFolder");
LPTSTR lpszIniUseDifferentNSIEditor             = TEXT("UseDifferentNSIEditor");
LPTSTR lpszIniNSIEditor                         = TEXT("NSIEditor");

// Section [REG]
LPTSTR lpszIniREG                               = TEXT("REG");
LPTSTR lpszIniREG5                              = TEXT("REG5");

// Section [UNL-Output]
LPTSTR lpszIniUNLOutout                         = TEXT("UNL-Output");

// Global fields storing the settings
// Section [Setup]
BOOL  fStartDefaults;
BOOL  fAutoCompare; 
BOOL  fStoreOnQuit;
BOOL  fStoreOnlyUncompared;
BOOL  fDontDisplayInfoAfterShot;
BOOL  fDontDisplayInfoAfterComparison;
BOOL  fSaveSettingsOnExit;
BOOL  fNoFiltersWhenLoading;
BOOL  fShowSIDFilterRules;
BOOL  fFileshot;
UINT  nExpandLevels;
UINT  nMaxNodes;
UINT  nMaxLines;

// Section [Registry-Scan]
BOOL  fHKLM;
BOOL  fHKU; 
BOOL  fHKCU;

// Section [Skips]
LPTSTR lpgrszRegSkipStrings;                                                // content of whole section [SkipRegKey]
LPTSTR lpgrszRegWhitelistStrings;                                           // content of whole section [RegKeyWhitelist]
LPTSTR lpgrszTempStrings;                                                   // content of whole section [ScanDir/SkipDir/SkipFile]
SKIPLIST * pRegSkipList;
SKIPLIST * pRegWhiteList;
SKIPLIST * pFileSkipList;
SKIPLIST * pDirScanList;

// Section [Output]
BOOL  fOpenEditor;
BOOL  fOutputTXTfile;
BOOL  fOutputUNLfile;
BOOL  fOutputBATfile;
BOOL  fOutputHTMfile;
BOOL  fISSInstallFile;
BOOL  fISSDeinstallFile;
BOOL  fNSIInstallFile;
BOOL  fNSIDeinstallFile;
BOOL  fRegDel;
BOOL  fRegIns;
BOOL  fLogEnvironmentStrings;
BOOL  fSuppressLogs;
BOOL  fLogUNLOrder;
BOOL  fDeleteReadOnly;
BOOL  fDeleteDirNotEmpty;
BOOL  fCheckResult;
BOOL  fUseLongRegHead;
BOOL  fOutSeparateObjs;
int   nOutMaxResultLines;

// Section [UNL]
BOOL  fOnlyNewEntries;
BOOL  fNoVals;  

// Section [ISS]
BOOL  fUseDifferentISSOutputFolder;
BOOL  fUseDifferentISSEditor;

// Section [NSI]
BOOL  fUseDifferentNSIOutputFolder;
BOOL  fUseDifferentNSIEditor;

// Section [REG]
BOOL  fREG5;  

LPTSTR szSID = NULL;
// some macros to distinguish the loaded skip and scan lists
#define REGSKIP  1
#define DIRSKIP  2
#define FILESKIP 3
#define FILESCAN 4
#define REGWHITE 5

//-------------------------------------------------------------
// Load all settings from regshot.ini at program startup
//-------------------------------------------------------------
BOOL LoadSettingsFromIni(HWND hDlg) // tfx get ini info
{
    int iRowsCount = 0;
// Section [SkipRegKey]
    MYFREE(lpgrszRegSkipStrings);
    MYFREE(pRegSkipList);
    pRegSkipList = MYALLOC0(sizeof(SKIPLIST));
    lpgrszRegSkipStrings = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    pRegSkipList = FillSkipList(pRegSkipList, lpszIniSkipRegKey, lpgrszRegSkipStrings, REGSKIP);

// Section [RegKeyWhitelist]
    MYFREE(lpgrszRegWhitelistStrings);
    MYFREE(pRegWhiteList);
    pRegWhiteList = MYALLOC0(sizeof(SKIPLIST));
    lpgrszRegWhitelistStrings = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    pRegWhiteList = FillSkipList(pRegWhiteList, lpszIniWhitelistRegKey, lpgrszRegWhitelistStrings, REGWHITE);

// Section [ScanDir]
    MYFREE(pDirScanList);
    pDirScanList = MYALLOC0(sizeof(SKIPLIST));
    lpgrszTempStrings = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    pDirScanList = FillSkipList(pDirScanList, lpszIniScanDir, lpgrszTempStrings, FILESCAN);
    SetDlgItemText(hDlg, IDC_EDITDIR, lpszExtDir);
    MYFREE(lpgrszTempStrings);

// Section [SkipDir]
    if (pFileSkipList != NULL) {
        for (iRowsCount = 0; pFileSkipList[iRowsCount].lpSkipString != NULL; iRowsCount++) {
            MYFREE(pFileSkipList[iRowsCount].lpSkipString);
            pFileSkipList[iRowsCount].lpSkipString = NULL;
        }
    }
    MYFREE(pFileSkipList);
    pFileSkipList = MYALLOC0(2 * sizeof(SKIPLIST));
    lpgrszTempStrings = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    pFileSkipList = FillSkipList(pFileSkipList, lpszIniSkipDir, lpgrszTempStrings, DIRSKIP);;
// Section [SkipFile]
    pFileSkipList = FillSkipList(pFileSkipList, lpszIniSkipFile, lpgrszTempStrings, FILESKIP);
    MYFREE(lpgrszTempStrings);

    BYTE  nFlag;
// Section [Setup]
    nFlag = (BYTE)GetPrivateProfileInt(lpszIniSetup, lpszIniFlag, 1, lpszRegshotIni); // default from 0 to 1 in 1.8.2 (TEXT)
    fStartDefaults = GetPrivateProfileInt(lpszIniSetup, lpszIniStartDefaults, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_DEFAULTS), BM_SETCHECK, fStartDefaults, (LPARAM)0);
    fAutoCompare = GetPrivateProfileInt(lpszIniSetup, lpszIniAutoCompare, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_AUTOCOMPARE), BM_SETCHECK, fAutoCompare, (LPARAM)0);
    EnableWindow(GetDlgItem(hDlg, IDC_CHECK_AUTOCOMPARE), (fStartDefaults ? TRUE : FALSE));
    fStoreOnQuit = (BOOL)GetPrivateProfileInt(lpszIniSetup, lpszIniStoreOnQuit, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONQUIT), BM_SETCHECK, fStoreOnQuit, (LPARAM)0);
    fStoreOnlyUncompared = (BOOL)GetPrivateProfileInt(lpszIniSetup, lpszIniStoreOnlyUncompared, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONLYUNCOMP), BM_SETCHECK, fStoreOnlyUncompared, (LPARAM)0);

    GetPrivateProfileString(lpszIniSetup, lpszIniTitle, TEXT("Default"), lpszTitle, MAX_PATH, lpszRegshotIni);
    SetDlgItemText(hDlg, IDC_EDITTITLE, lpszTitle);
    GetPrivateProfileString(lpszIniSetup, lpszIniEditor, TEXT("notepad.exe"), lpszEditor, MAX_PATH, lpszRegshotIni);

    if (0 != GetPrivateProfileString(lpszIniSetup, lpszIniOutDir, NULL, lpszOutputPath, MAX_PATH, lpszRegshotIni)) {
        LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
        ExpandEnvironmentStrings(lpszOutputPath, lpszTempDirBuffer, MAX_PATH);
        _tcscpy(lpszOutputPath, lpszTempDirBuffer);
        MYFREE(lpszTempDirBuffer);
        SetDlgItemText(hDlg, IDC_EDITPATH, lpszOutputPath);
    }
    else {
        SetDlgItemText(hDlg, IDC_EDITPATH, lpszTempPath);
    }
    fDontDisplayInfoAfterShot = GetPrivateProfileInt(lpszIniSetup, lpszIniDontDisplayInfoAfterShot, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fDontDisplayInfoAfterComparison = GetPrivateProfileInt(lpszIniSetup, lpszIniDontDisplayInfoAfterShot, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fSaveSettingsOnExit = GetPrivateProfileInt(lpszIniSetup, lpszIniSaveSettingsOnExit, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fNoFiltersWhenLoading = GetPrivateProfileInt(lpszIniSetup, lpszIniNoFiltersWhenLoading, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fShowSIDFilterRules = GetPrivateProfileInt(lpszIniSetup, lpszIniShowSIDFilterRules, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    nExpandLevels = (int)GetPrivateProfileInt(lpszIniSetup, lpszIniExpandLevels, 4, lpszRegshotIni);
    if (0 >= nExpandLevels) {
        nExpandLevels = 4;
    }
    nMaxNodes = (int)GetPrivateProfileInt(lpszIniSetup, lpszIniMaxNodes, 9999, lpszRegshotIni);
    if ((0 >= nMaxNodes) || (nMaxNodes>9999)) {
        nMaxNodes = 9999;
    }
    nMaxLines = (int)GetPrivateProfileInt(lpszIniSetup, lpszIniMaxResultLines, 10000, lpszRegshotIni);
    if ((0 >= nMaxLines) || (nMaxLines >99999)) {
        nMaxLines = 99999;
    }

// Section [Output]
    int nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("Unicode (UTF-8)"));
    nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("Unicode (UTF-16 LE)"));
    nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("ANSI (CP1252)"));
    nCodePage = (int)GetPrivateProfileInt(lpszIniOutput, TEXT("Codepage"), -1, lpszRegshotIni); // default from 0 to 1 in 1.8.2 (TEXT)
    nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_FINDSTRINGEXACT, (WPARAM)0, (LPARAM)(nCodePage == CP_UTF8 ? TEXT("Unicode (UTF-8)") : (nCodePage == -1 ? TEXT("Unicode (UTF-16 LE)") : TEXT("ANSI (CP1252)"))));
    if (CB_ERR != nResult) {
        SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_SETCURSEL, (WPARAM)nResult, (LPARAM)0);
    }
    //SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, (WPARAM)(nFlag & 0x01), (LPARAM)0);           // BAT output
    //SendMessage(GetDlgItem(hDlg, IDC_CHECK_HTML), BM_SETCHECK, (WPARAM)((nFlag & 0x02) >> 1), (LPARAM)0);   // HTML output
    //SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, (WPARAM)((nFlag & 0x04) >> 2), (LPARAM)0);    // UNL output
    SendMessage(GetDlgItem(hDlg, IDC_CHECKDIR), BM_SETCHECK, (WPARAM)((nFlag & 0x08) >> 3), (LPARAM)0);
    EnableWindow(GetDlgItem(hDlg, IDC_PROP_SCANS), ((nFlag & 0x08) >> 3));

    fOpenEditor = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniOpenEditor, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPENEDITOR), BM_SETCHECK, fOpenEditor, (LPARAM)0);
    fLogEnvironmentStrings = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniLogEnvironmentStrings, 0, lpszRegshotIni);
    fSuppressLogs = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniSuppressLogs, 0, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS_LOGS), BM_SETCHECK, fSuppressLogs, (LPARAM)0);
    fLogUNLOrder = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniLogUNLOrder, 0, lpszRegshotIni);
    fUseLongRegHead = GetPrivateProfileInt(lpszIniOutput, lpszIniUseLongRegHead, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fDeleteReadOnly = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniDeleteReadOnly, 0, lpszRegshotIni);
    fDeleteDirNotEmpty = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniDeleteDirNotEmpty, 0, lpszRegshotIni);
    fCheckResult = (BOOL)GetPrivateProfileInt(lpszIniOutput, lpszIniCheckResult, 0, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_RESULT), BM_SETCHECK, fCheckResult, (LPARAM)0);
    fOutSeparateObjs = GetPrivateProfileInt(lpszIniOutput, lpszIniOutSeparateObjs, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    nOutMaxResultLines = GetPrivateProfileInt(lpszIniOutput, lpszIniOutMaxResultLines, 10, lpszRegshotIni);
    if (0 >= nOutMaxResultLines) {
        nOutMaxResultLines = 10;
    }
    LPTSTR strOutMaxResultLines = MYALLOC0(4 * sizeof(TCHAR));
    if (strOutMaxResultLines != NULL) {
        _sntprintf(strOutMaxResultLines, 3, TEXT("%i"), nOutMaxResultLines);
        strOutMaxResultLines[3] = _T('\0');
        SetDlgItemText(hDlg, IDC_EDIT_OUTPUTLINES, strOutMaxResultLines);
        MYFREE(strOutMaxResultLines);
    }

    fRegDel = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputRegDeinstallFile, 1, lpszRegshotIni);
    fRegIns = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputRegInstallFile, 1, lpszRegshotIni);
    fISSInstallFile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputISSInstallFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fISSDeinstallFile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputISSDeinstallFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fNSIInstallFile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputNSIInstallFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fNSIDeinstallFile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputNSIDeinstallFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fOutputTXTfile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputTXTFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    fOutputUNLfile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputUNLFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, fOutputUNLfile, (LPARAM)0);
    fOutputBATfile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputBATFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, fOutputBATfile, (LPARAM)0);
    fOutputHTMfile = GetPrivateProfileInt(lpszIniOutput, lpszIniOutputHTMFile, 0, lpszRegshotIni) != 0 ? TRUE : FALSE;
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_HTML), BM_SETCHECK, fOutputHTMfile, (LPARAM)0);

// Section [Registry-Scan]
    fHKLM = GetPrivateProfileInt(lpszIniRegistryScan, lpszIniHKLM, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKLM), BM_SETCHECK, fHKLM, (LPARAM)0);
    fHKU = GetPrivateProfileInt(lpszIniRegistryScan, lpszIniHKU, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKU), BM_SETCHECK, fHKU, (LPARAM)0);
    fHKCU = GetPrivateProfileInt(lpszIniRegistryScan, lpszIniHKCU, 1, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKCU), BM_SETCHECK, fHKCU, (LPARAM)0);

// Section [UNL]
    fOnlyNewEntries = (BOOL)GetPrivateProfileInt(lpszIniUNL, lpszIniOnlyNewEntries, 0, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_ONLYADDED), BM_SETCHECK, fOnlyNewEntries, (LPARAM)0);
    fNoVals = (BOOL)GetPrivateProfileInt(lpszIniUNL, lpszIniNoVals, 0, lpszRegshotIni);
    SendMessage(GetDlgItem(hDlg, IDC_CHECK_NOVALS), BM_SETCHECK, fNoVals, (LPARAM)0);

// Section [ISS]
    fUseDifferentISSOutputFolder = (BOOL)GetPrivateProfileInt(lpszIniISS, lpszIniUseDifferentISSOutputFolder, 0, lpszRegshotIni);
    GetPrivateProfileString(lpszIniISS, lpszIniISSOutputFolder, TEXT("C:\\Temp"), lpszISSOutputFolder, MAX_PATH, lpszRegshotIni);
    fUseDifferentISSEditor = (BOOL)GetPrivateProfileInt(lpszIniISS, lpszIniUseDifferentISSEditor, 0, lpszRegshotIni);
    GetPrivateProfileString(lpszIniISS, lpszIniISSEditor, TEXT("C:\\Temp\\Inno Setup 6\\Compil32.exe"), lpszISSEditor, MAX_PATH, lpszRegshotIni);

// Section [NSI]
    fUseDifferentNSIOutputFolder = (BOOL)GetPrivateProfileInt(lpszIniNSI, lpszIniUseDifferentNSIOutputFolder, 0, lpszRegshotIni);
    GetPrivateProfileString(lpszIniNSI, lpszIniNSIOutputFolder, TEXT("C:\\Temp"), lpszNSIOutputFolder, MAX_PATH, lpszRegshotIni);
    fUseDifferentNSIEditor = (BOOL)GetPrivateProfileInt(lpszIniNSI, lpszIniUseDifferentNSIEditor, 0, lpszRegshotIni);
    GetPrivateProfileString(lpszIniNSI, lpszIniNSIEditor, TEXT("C:\\Temp\\nsis-3.06.1-setup\\makensisw.exe"), lpszNSIEditor, MAX_PATH, lpszRegshotIni);

// Section [REG]
    fREG5 = (BOOL)GetPrivateProfileInt(lpszIniREG, lpszIniREG5, 1, lpszRegshotIni);

    return TRUE;
}

//-------------------------------------------------------------
// Save settings to regshot.ini when wished,
// but not the skip and scan lists
//-------------------------------------------------------------
BOOL SaveSettingsToIni(HWND hDlg)
{
    HANDLE  hTest;
    // 1.8.2, someone might not want to create a regshot.ini when there isn't one. :O
    hTest = CreateFile(lpszRegshotIni, GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hTest == INVALID_HANDLE_VALUE) {
        return FALSE;
    }
    CloseHandle(hTest);

    // Fensterposition o.ä. vor diesem Ausgang speichern 
//    fSaveIni = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SAVEINI), BM_GETCHECK, (WPARAM)0, (LPARAM)0);    // 1.9.1: Separate objects in output with empty line
    //if (!fSaveIni)
    //    return FALSE;

    BYTE    nFlag;
    LPTSTR  lpszValue = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));  // EXTDIRLEN due to IDC_EDITDIR

// Section [Setup]
    if (GetDlgItemText(hDlg, IDC_EDITTITLE, lpszValue, EXTDIRLEN) != 0) {
        lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
        WritePrivateProfileString(lpszIniSetup, lpszIniTitle, lpszValue, lpszRegshotIni);
    }

    nFlag = (BYTE)(SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0) |
                   SendMessage(GetDlgItem(hDlg, IDC_CHECK_HTML), BM_GETCHECK, (WPARAM)0, (LPARAM)0) << 1 |
                   SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0) << 2 |
                   SendMessage(GetDlgItem(hDlg, IDC_CHECKDIR), BM_GETCHECK, (WPARAM)0, (LPARAM)0) << 3);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), nFlag);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniFlag, lpszValue, lpszRegshotIni);

    if (GetDlgItemText(hDlg, IDC_EDITPATH, lpszValue, EXTDIRLEN) != 0) {
        LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
        if ((lpszValue != NULL) && (lpszTempDirBuffer != NULL)) {
            if (PathUnExpandEnvStrings(lpszValue, lpszTempDirBuffer, MAX_PATH)) {
                _tcscpy(lpszValue, lpszTempDirBuffer);
            }
        }
        if (lpszValue != NULL) {
            lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
            WritePrivateProfileString(lpszIniSetup, lpszIniOutDir, lpszValue, lpszRegshotIni);
        }
        MYFREE(lpszTempDirBuffer);
    }

    WritePrivateProfileString(lpszIniSetup, lpszIniLanguage, lpszLanguage, lpszRegshotIni);

    lpszEditor[MAX_PATH] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniEditor, lpszEditor, lpszRegshotIni);

    fStartDefaults = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_DEFAULTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fStartDefaults);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniStartDefaults, lpszValue, lpszRegshotIni);

    fAutoCompare = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_AUTOCOMPARE), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fAutoCompare);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniAutoCompare, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fStoreOnQuit);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniStoreOnQuit, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fStoreOnlyUncompared);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniStoreOnlyUncompared, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fDontDisplayInfoAfterShot);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniDontDisplayInfoAfterShot, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fDontDisplayInfoAfterComparison);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniDontDisplayInfoAfterComparison, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fSaveSettingsOnExit);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniSaveSettingsOnExit, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fNoFiltersWhenLoading);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniNoFiltersWhenLoading, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fShowSIDFilterRules);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniShowSIDFilterRules, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%i\0"), nExpandLevels);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniExpandLevels, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%i\0"), nMaxNodes);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniMaxNodes, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%i\0"), nMaxLines);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniSetup, lpszIniMaxResultLines, lpszValue, lpszRegshotIni);

// Section [Output]
    int nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
    if (CB_ERR == nResult) {
        return FALSE;
    }
    if (nResult == 0)
        nCodePage = 0;
    else if (nResult == 1)
        nCodePage = -1;
    else
        nCodePage = 65001;
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), nCodePage);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, TEXT("Codepage"), lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fUseLongRegHead);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniUseLongRegHead, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fLogEnvironmentStrings);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';  // safety NULL char
    WritePrivateProfileString(lpszIniOutput, lpszIniLogEnvironmentStrings, lpszValue, lpszRegshotIni);

    fSuppressLogs = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS_LOGS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fSuppressLogs);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';  // safety NULL char
    WritePrivateProfileString(lpszIniOutput, lpszIniSuppressLogs, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fLogUNLOrder);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniLogUNLOrder, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fDeleteReadOnly);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniDeleteReadOnly, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fDeleteDirNotEmpty);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniDeleteDirNotEmpty, lpszValue, lpszRegshotIni);

    fOpenEditor = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPENEDITOR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOpenEditor);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOpenEditor, lpszValue, lpszRegshotIni);

    fCheckResult = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_RESULT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fCheckResult);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniCheckResult, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOutSeparateObjs);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutSeparateObjs, lpszValue, lpszRegshotIni);

    GetDlgItemText(hDlg, IDC_EDIT_OUTPUTLINES, lpszValue, 4);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutMaxResultLines, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fRegDel);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputRegDeinstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fRegIns);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputRegInstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOutputTXTfile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputTXTFile, lpszValue, lpszRegshotIni);

    fOutputUNLfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOutputUNLfile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputUNLFile, lpszValue, lpszRegshotIni);

    fOutputBATfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOutputBATfile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputBATFile, lpszValue, lpszRegshotIni);

    fOutputHTMfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_HTML), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOutputHTMfile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputHTMFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fISSDeinstallFile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputISSDeinstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fISSInstallFile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputISSInstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fNSIDeinstallFile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputNSIDeinstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fNSIInstallFile);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputNSIInstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fRegIns);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputRegInstallFile, lpszValue, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fRegDel);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniOutput, lpszIniOutputRegDeinstallFile, lpszValue, lpszRegshotIni);

// Section [Registry-Scan]
    fHKLM = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKLM), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fHKLM);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniRegistryScan, lpszIniHKLM, lpszValue, lpszRegshotIni);

    fHKU = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKU), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fHKU);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniRegistryScan, lpszIniHKU, lpszValue, lpszRegshotIni);

    fHKCU = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_HKCU), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fHKCU);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniRegistryScan, lpszIniHKCU, lpszValue, lpszRegshotIni);

// Section [UNL]
    fOnlyNewEntries = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ONLYADDED), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fOnlyNewEntries);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniUNL, lpszIniOnlyNewEntries, lpszValue, lpszRegshotIni);

    fNoVals = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NOVALS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fNoVals);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniUNL, lpszIniNoVals, lpszValue, lpszRegshotIni);

// Section [ISS]
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fUseDifferentISSOutputFolder);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniISS, lpszIniUseDifferentISSOutputFolder, lpszValue, lpszRegshotIni);
    lpszISSOutputFolder[MAX_PATH] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniISS, lpszIniISSOutputFolder, lpszISSOutputFolder, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fUseDifferentISSEditor);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniISS, lpszIniUseDifferentISSEditor, lpszValue, lpszRegshotIni);
    lpszISSEditor[MAX_PATH] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniISS, lpszIniISSEditor, lpszISSEditor, lpszRegshotIni);

// Section [NSI]
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fUseDifferentNSIOutputFolder);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniNSI, lpszIniUseDifferentNSIOutputFolder, lpszValue, lpszRegshotIni);
    lpszNSIOutputFolder[MAX_PATH] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniNSI, lpszIniNSIOutputFolder, lpszNSIOutputFolder, lpszRegshotIni);

    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fUseDifferentNSIEditor);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniNSI, lpszIniUseDifferentNSIEditor, lpszValue, lpszRegshotIni);
    lpszNSIEditor[MAX_PATH] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniNSI, lpszIniNSIEditor, lpszNSIEditor, lpszRegshotIni);

// Section [REG]
    _sntprintf(lpszValue, EXTDIRLEN, TEXT("%d\0"), fREG5);
    lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';
    WritePrivateProfileString(lpszIniREG, lpszIniREG5, lpszValue, lpszRegshotIni);

    MYFREE(lpszValue);

    return TRUE;
}

//-------------------------------------------------------------
// SkipLists aus den Settings aufbauen
//-------------------------------------------------------------
SKIPLIST* FillSkipList(SKIPLIST* pList, LPTSTR lpszIniSection, LPTSTR lpszSectionContent, int iList)
{
    int   i = 0;
    DWORD cchSection;
    int iSkipCounter = 0;
    SKIPLIST* pListTemp;
    LPTSTR pSubstr;
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = MAX_PATH + 1;
    int iRowsCount = 0;

    for (iRowsCount = 0; pList[iRowsCount].lpSkipString != NULL; iRowsCount++);

    i = 0;
    pList[i + iRowsCount].lpSkipString = NULL;                                                                           // saftey NULL pointer
    cchSection = GetPrivateProfileSection(lpszIniSection, lpszSectionContent, MAX_INI_SECTION_CHARS, lpszRegshotIni);   // length incl. double NULL character
    pSubstr = lpszSectionContent;
    while ((0 < cchSection) && ('\0' != *pSubstr)) {
        size_t substrLen = _tcslen(pSubstr);
        LPTSTR pos = _tcschr(pSubstr, _T('='));                                                                         // split substring on '=' char
        if (NULL != pos) {                                                                                              // only HK.... relevant
            if ((iList == REGSKIP) && (pSubstr != _tcsstr(pSubstr, TEXT("HK"))))
                continue;
            _tcscpy(pos, TEXT("\0"));                                                                                   // finish string on end of key

            if ((iList == FILESCAN) || (iList == DIRSKIP) || (iList == FILESKIP)) {
                lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR));                                                     // nBufferSize must > commentlength + 10 .txt 0000
                ExpandEnvironmentStrings(pSubstr, lpszBuffer, nBufferSize);
                pList[i + iRowsCount].lpSkipString = lpszBuffer;                                                         // and add key to pList array
            }
            else
                pList[i + iRowsCount].lpSkipString = pSubstr;
            pList[i + iRowsCount].bSkipKeyActive = FALSE;
            pList[i + iRowsCount].bAutomaticallyAdded = FALSE;
            pList[i + iRowsCount].bAdhoc = FALSE;
            pList[i + iRowsCount].bDeleted = FALSE;
            if (iList == FILESKIP)
                pList[i + iRowsCount].bFile = TRUE;
            else
                pList[i + iRowsCount].bFile = FALSE;

            if (0 == _tcscmp(++pos, TEXT("1"))) {
                pList[i + iRowsCount].bSkipKeyActive = TRUE;
                if (iList == FILESCAN) {
                    if (pList[i + iRowsCount].lpSkipString != NULL) {
                        if (i + iRowsCount == 0)
                            _tcscpy(lpszExtDir, pList[i + iRowsCount].lpSkipString);
                        else {
                            _tcscat(lpszExtDir, TEXT(";"));
                            _tcscat(lpszExtDir, pList[i + iRowsCount].lpSkipString);
                        }
                    }
                }
            }
            i++;
            iSkipCounter = i + iRowsCount + 2;
            pListTemp = MYREALLOC0(iSkipCounter * sizeof(SKIPLIST), pList);
            if (pListTemp != NULL)
                pList = pListTemp;
            pList[i + iRowsCount].lpSkipString = NULL;
        }
        pSubstr += (substrLen + 1);                                                                                     // jump over the current substring plus its null
    }

    // Redundante Rootkeys ausschließen
    if (iList == REGSKIP) {
        pList = EvaluateAndSkipSID(pList);
    }

    return pList;
}

//-------------------------------------------------------------
// Aktuell geprüften Eintrag in den SkipLists suchen
//-------------------------------------------------------------
BOOL IsInSkipList(LPTSTR lpszString, SKIPLIST* pSkipList, BOOL bForOutput)  // tfx skip the list
{
    int i;
    LPTSTR pos, lpszStringStart;

    if ((BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOFILTERS), BM_GETCHECK, (WPARAM)0, (LPARAM)0))
        return FALSE;

    lpszStringStart = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    if (lpszStringStart != NULL)
        _tcscpy(lpszStringStart, lpszString);

    for (i = 0; (NULL != pSkipList[i].lpSkipString); i++) {
        if ((lpszString != NULL) && (lpszStringStart != NULL))
            _tcscpy(lpszString, lpszStringStart);
        pos = lpszString;
        while (NULL != pos) {
            if ((0 == _tcsicmp(lpszString, pSkipList[i].lpSkipString) && (pSkipList[i].bSkipKeyActive) && (!pSkipList[i].bDeleted))) {
                MYFREE(lpszStringStart);
                return TRUE;
            }
            if (bForOutput) {
                pos = _tcsrchr(lpszString, _T('\\'));
                if (NULL != pos)
                    _tcscpy(pos, TEXT("\0"));
            }
            else
                break;
        }

    }
    MYFREE(lpszStringStart);

    return FALSE;
}

//-------------------------------------------------------------
// Aktuell geprüften Eintrag in den SkipLists suchen
//-------------------------------------------------------------
BOOL IsInWhiteList(LPTSTR lpszString, BOOL bForOutput)  // tfx skip the list
{
    int i;
    LPTSTR pos, lpszStringStart, lpszStringBackup;
    BOOL bWhiteListeActive = FALSE;

    for (i = 0; (NULL != pRegWhiteList[i].lpSkipString); i++) {
        if ((pRegWhiteList[i].bSkipKeyActive) && (!pRegWhiteList[i].bDeleted)) {
            bWhiteListeActive = TRUE;
            break;
        }
    }
    if (!bWhiteListeActive)
        return TRUE;

    lpszStringBackup = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    if (lpszStringBackup != NULL)
        _tcscpy(lpszStringBackup, lpszString);

    lpszStringStart = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    if (lpszStringStart != NULL)
        _tcscpy(lpszStringStart, lpszString);

    for (i = 0; (NULL != pRegWhiteList[i].lpSkipString); i++) {
        if ((lpszString != NULL) && (lpszStringStart != NULL))
            _tcscpy(lpszString, lpszStringStart);
        pos = lpszString;
        while (NULL != pos) {
            if ((0 == _tcsicmp(lpszString, pRegWhiteList[i].lpSkipString) && (pRegWhiteList[i].bSkipKeyActive) && (!pRegWhiteList[i].bDeleted))) {
                if ((lpszString != NULL) && (lpszStringBackup != NULL))
                    _tcscpy(lpszString, lpszStringBackup);
                MYFREE(lpszStringBackup);
                MYFREE(lpszStringStart);
                return TRUE;
            }
            if (bForOutput) {
                pos = _tcsrchr(lpszString, _T('\\'));
                if (NULL != pos)
                    _tcscpy(pos, TEXT("\0")); 
            }
            else
                break;
        }

    }
    if ((lpszString != NULL) && (lpszStringBackup != NULL))
        _tcscpy(lpszString, lpszStringBackup);
    MYFREE(lpszStringBackup);
    MYFREE(lpszStringStart);
    
    return FALSE;
}

// ----------------------------------------------------------------------
// Find lpszSearch's position in lpgrszSection
//
// Functionality is similar to GetPrivateProfileString(), but return value is a
// pointer inside a memory block with all strings (normally a section buffer),
// which avoids handling multiple pointers individually.
// The section buffer must not contain unnecessary whitespaces, comments,
// empty lines, etc. Windows' GetPrivateProfileSection() already cares about this.
// ----------------------------------------------------------------------
LPTSTR FindKeyInIniSection(LPTSTR lpgrszSection, LPTSTR lpszSearch, size_t cchSectionLen, size_t cchSearchLen)
{
    size_t i;
    size_t nszSectionLen;

    if ((NULL == lpgrszSection) || (NULL == lpszSearch) || (1 > cchSearchLen)) {
        return NULL;
    }

    for (i = 0; i < cchSectionLen; i++) {
        if (0 == lpgrszSection[i]) {  // reached the end of the section buffer
            break;
        }

        nszSectionLen = _tcslen(&lpgrszSection[i]);
        if (nszSectionLen <= cchSearchLen) {
            i += nszSectionLen;  // skip this string as it is too short (or would result in an empty return string)
            continue;
        }

        if (0 == _tcsnccmp(&lpgrszSection[i], lpszSearch, cchSearchLen)) {
            return &lpgrszSection[(i + cchSearchLen)];
        }

        i += nszSectionLen;  // skip this string as it does not match
    }

    return NULL;
}

//-------------------------------------------------------------
// SID des angemeldeten Benutzers ermitteln und zugehörige
// redundante Registry Rootkeys ausschließen
//-------------------------------------------------------------
SKIPLIST * EvaluateAndSkipSID(SKIPLIST * pList)
{
    BYTE bSID[SECURITY_MAX_SID_SIZE];
    BOOL bRVal;
    DWORD dwSidSize = SECURITY_MAX_SID_SIZE;

    LPTSTR szUserName;
    DWORD dwUserSize = 256;
    szUserName = MYALLOC0(dwUserSize * sizeof(TCHAR));

    LPTSTR szDomainName;
    DWORD dwDomainSize = 256;
    szDomainName = MYALLOC0(dwDomainSize * sizeof(TCHAR));

    SID_NAME_USE snu = SidTypeInvalid;

    //find the current user's name
    if (!(GetUserName(szUserName, &dwUserSize)))
        return pList;

    bRVal = LookupAccountName(
        NULL, //use this system
        szUserName, //the user to look up
        bSID, //the returned SID
        &dwSidSize, //the size of the SID returned
        szDomainName, //the returned domain name
        &dwDomainSize, //the size of the domain name
        &snu //the type of sid
    );

    bRVal = ConvertSidToStringSid(bSID, &szSID);
    MYFREE(szUserName);
    MYFREE(szDomainName);

    SKIPLIST* pListTemp = NULL;
    int iSkipCounter = 0;
    int iRowsCount = 0;
    for (iRowsCount = 0; pList[iRowsCount].lpSkipString != NULL; iRowsCount++);
    if (szSID != NULL) {
        pList[iRowsCount].lpSkipString = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
        if (pList[iRowsCount].lpSkipString != NULL) {
            _tcscpy(pList[iRowsCount].lpSkipString, TEXT("HKU\\"));
            _tcscat(pList[iRowsCount].lpSkipString, szSID);
        }
        pList[iRowsCount].bSkipKeyActive = TRUE;
        pList[iRowsCount].bAutomaticallyAdded = TRUE;
        pList[iRowsCount].bAdhoc = FALSE;
        pList[iRowsCount].bDeleted = FALSE;
        pList[iRowsCount].bFile = FALSE;
        iRowsCount++;
        iSkipCounter = iRowsCount + 1;
        pListTemp = MYREALLOC0(iSkipCounter * sizeof(SKIPLIST), pList);
        if (pListTemp != NULL) {
            pList = pListTemp;
            pList[iRowsCount].lpSkipString = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
            if (pList[iRowsCount].lpSkipString != NULL) {
                _tcscpy(pList[iRowsCount].lpSkipString, TEXT("HKU\\"));
                _tcscat(pList[iRowsCount].lpSkipString, szSID);
                _tcscat(pList[iRowsCount].lpSkipString, TEXT("_Classes"));
            }
            pList[iRowsCount].bSkipKeyActive = TRUE;
            pList[iRowsCount].bAutomaticallyAdded = TRUE;
            pList[iRowsCount].bAdhoc = FALSE;
            pList[iRowsCount].bDeleted = FALSE;
            pList[iRowsCount].bFile = FALSE;
            iRowsCount++;
        }
        iSkipCounter = iRowsCount + 1;
        pListTemp = MYREALLOC0(iSkipCounter * sizeof(SKIPLIST), pList);
        if (pListTemp != NULL) {
            pList = pListTemp;
            pList[iRowsCount].lpSkipString = NULL;
        }
    }
    return pList;
}

BOOL ResetOutputOptions(VOID)
{
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_BAT), BM_SETCHECK, (WPARAM)(FALSE), (LPARAM)0);                  // CMD output
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_UNL), BM_SETCHECK, (WPARAM)(FALSE), (LPARAM)0);                  // UNL output
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HTML), BM_SETCHECK, (WPARAM)(FALSE), (LPARAM)0);                 // HTML output
    fNSIInstallFile = TRUE;
    fNSIDeinstallFile = TRUE;
    fISSInstallFile = FALSE;
    fISSDeinstallFile = FALSE;
    fRegDel = FALSE;
    fRegIns = FALSE;
    fREG5 = TRUE;
    fOutputTXTfile = FALSE;
    
    fNoVals = TRUE;
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOVALS), BM_SETCHECK, (WPARAM)(fNoVals), (LPARAM)0);             // Keine Werte protokollieren
    fOnlyNewEntries = TRUE;
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_ONLYADDED), BM_SETCHECK, (WPARAM)(fOnlyNewEntries), (LPARAM)0);  // nur neue Objekte protokollieren
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOFILTERS), BM_SETCHECK, (WPARAM)FALSE, (LPARAM)0);
    fOpenEditor = TRUE;
    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_OPENEDITOR), BM_SETCHECK, (WPARAM)fOpenEditor, (LPARAM)0);

    return TRUE;
}

//-------------------------------------------------------------
// Speicher der settings freigeben
//-------------------------------------------------------------
BOOL CleanUpGlobalMemory(VOID) 
{
    MYFREE(szSID);
    MYFREE(lpszRegshotIni);
    MYFREE(lpszLanguage);
    MYFREE(lpszExtDir);
    MYFREE(lpszTitle);
    MYFREE(lpszLanguageIni);
    MYFREE(lpszMessage);
    MYFREE(lpszWindowsDirName);
    MYFREE(lpszTempPath);
    MYFREE(lpszOutputPath);
    MYFREE(lpszEditor);
    MYFREE(lpszISSOutputFolder);
    MYFREE(lpszISSEditor);
    MYFREE(lpszNSIOutputFolder);
    MYFREE(lpszNSIEditor);

    /* Clean up global memory and from LoadSettingsFromIni() plus SetTextsToSelectedLanguage()*/
    if (NULL != lpgrszRegSkipStrings) {
        MYFREE(lpgrszRegSkipStrings);
    }
    if (NULL != lpgrszRegWhitelistStrings) {
        MYFREE(lpgrszRegWhitelistStrings);
    }
    if (NULL != lpgrszLangSection) {
        MYFREE(lpgrszLangSection);
    }
    
    int iRowsCount = 0;
    if (pFileSkipList != NULL) {
        for (iRowsCount = 0; pFileSkipList[iRowsCount].lpSkipString != NULL; iRowsCount++) {
            MYFREE(pFileSkipList[iRowsCount].lpSkipString);
            pFileSkipList[iRowsCount].lpSkipString = NULL;
        }
    }
    MYFREE(pFileSkipList);
    MYFREE(pRegSkipList);
    MYFREE(pRegWhiteList);

    if (pDirScanList != NULL) {
        for (iRowsCount = 0; pDirScanList[iRowsCount].lpSkipString != NULL; iRowsCount++) {
            MYFREE(pDirScanList[iRowsCount].lpSkipString);
            pDirScanList[iRowsCount].lpSkipString = NULL;
        }
    }
    MYFREE(pDirScanList);

    return TRUE;
}

