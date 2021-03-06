/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
    Copyright 1999-2003,2007 TiANWEi
    Copyright 2007 Belogorokhov Youri
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

// TODO: handle environment variables
// TODO: "gespiegelte" Reg-Pfade prüfen und ausschließen

#include "global.h"
#include "version.h"

/*
    Define window title for main with version, revision, etc. (see version.rc.h for title structure)
*/
LPTSTR lpszProgramName      = REGSHOT_TITLE;  // tfx  add program titile
LPTSTR lpszAboutRegshot     = TEXT("Regshot Advanced is a free and open source registry and filesystem compare utility.\n\n")
                              TEXT("Version:") REGSHOT_VERSION_NUM_DOTS2 REGSHOT_VERSION_NAME4 TEXT("\n")
                              TEXT("Revision: ") REGSHOT_REVISION_NUM2 REGSHOT_REVISION_NUM_SUFFIX2 TEXT("\n")
                              TEXT("Architecture: ") REGSHOT_VERSION_PLATFORM TEXT("\n")
                              TEXT("Codepage: ") REGSHOT_CODEPAGE TEXT("\n")
                              TEXT("Compiler: ") REGSHOT_VERSION_COMPILER TEXT("\n")
#ifdef REGSHOT_BUILDTYPE    
                              TEXT("Build: ") REGSHOT_BUILDTYPE TEXT("\n")
#endif
                              TEXT("\n")
                              REGSHOT_URL TEXT("\n")
                              TEXT("\n")
                              REGSHOT_VERSION_COPYRIGHT TEXT("\n")
                              TEXT("\n");

LPTSTR lpszIniFileName      = TEXT("regshot.ini"); // tfx
LPTSTR lpszLanguageFileName = TEXT("language.ini");

LPTSTR lpszExtDir;
LPTSTR lpszTitle;
LPTSTR lpszEditor;
LPTSTR lpszOutputPath;
LPTSTR lpszLastSaveDir;
LPTSTR lpszLastOpenDir;
LPTSTR lpszWindowsDirName;
LPTSTR lpszTempPath;
LPTSTR lpszStartDir;
LPTSTR lpszLanguageIni;  // For language.ini
LPTSTR lpszCurrentTranslator;
LPTSTR lpszRegshotIni;
LPTSTR lpszISSOutputFolder;
LPTSTR lpszISSEditor;
LPTSTR lpszNSIOutputFolder;
LPTSTR lpszNSIEditor;

MSG        msg;                 // Windows MSG struct
HWND       hMainWnd;            // The handle of REGSHOT
HINSTANCE  hInst;
HMENU      hMainMenu;           // Handle of dialog menu
HMENU      hButtonsMenu;        // Handle of popup menu
LPREGSHOT  lpMenuShot;          // Pointer to current Shot for popup menus and alike
RECT       rect;                // Window RECT
BOOL       bRunning;
BROWSEINFO BrowseInfo1;         // BrowseINFO struct
HICON ico_green, ico_red;
BOOL bRegSkipAdded;
BOOL bRegWhitelistAdded;
BOOL bFileSkipAdded;
BOOL bDirScanAdded;
BOOL bNoGui;

BOOL bCompareFS = TRUE;
BOOL bCompareReg = TRUE;

#ifdef USEHEAPALLOC_DANGER
HANDLE hHeap;  // 1.8.2
#endif

int nCodePage;

//LRESULT CALLBACK OwnerDrawButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
//    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
//{
//    LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
//    switch (uMsg)
//    {
//    case WM_PAINT:
//        SetTextColor(pDIS->hDC, RGB(100, 0, 100));
//            return TRUE;
//        // Other cases...
//    }
//    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
//}
//
//--------------------------------------------------
// Main Dialog Proc
//--------------------------------------------------
BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    size_t  nLen;
    BOOL fStartDefaults;
    BOOL fAutoCompare;

    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
        //case WM_DRAWITEM:
        //{
        //    
        //    LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
        //    if (pDIS->hwndItem == GetDlgItem(hMainWnd, IDC_1STSHOT))
        //    {
        //        SetTextColor(pDIS->hDC, RGB(100, 0, 100));
        //        WCHAR staticText[99];
        //        int len = SendMessage(GetDlgItem(hMainWnd, IDC_1STSHOT), WM_GETTEXT,
        //            ARRAYSIZE(staticText), (LPARAM)staticText);
        //        TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
        //    }
        //    return TRUE;
        //}
        case WM_INITDIALOG:  // Window creation (="program start")
            hMainWnd = hDlg;
            hMainMenu = GetMenu(hDlg);
            bRegSkipAdded = FALSE;                                         // flag to show whether there are reg exclusions ADDED after shots (to be used for comarison)
            bRegWhitelistAdded = FALSE;                                    // flag to show whether there are reg exclusions ADDED after shots (to be used for comarison)
            bFileSkipAdded = FALSE;                                        // flag to show whether there are reg exclusions ADDED after shots (to be used for comarison)
            
            //SetWindowSubclass(GetDlgItem(hMainWnd, IDC_1STSHOT), OwnerDrawButtonProc, 0, 0);
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
            SendDlgItemMessage(hDlg, IDC_EDITCOMMENT, EM_SETLIMITTEXT, (WPARAM)(COMMENTLENGTH - 1), (LPARAM)0);
            SendDlgItemMessage(hDlg, IDC_EDITPATH, EM_SETLIMITTEXT, (WPARAM)(MAX_PATH - 1), (LPARAM)0);
            SendDlgItemMessage(hDlg, IDC_EDITDIR, EM_SETLIMITTEXT, (WPARAM)(EXTDIRLEN - 1), (LPARAM)0);
            SendDlgItemMessage(hDlg, IDC_EDIT_OUTPUTLINES, EM_SETLIMITTEXT, (WPARAM)(3), (LPARAM)0);
            SendDlgItemMessage(hDlg, IDC_CHECK_SAVEINI, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);

            //enlarge some buffer in 201201
            lpszLanguage        = NULL;
            lpszExtDir          = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));      // EXTDIRLEN is actually MAX_PATH * 4
            lpszTitle           = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));      // TITLELEN is actually MAX_PATH * 4
            lpszLanguageIni     = MYALLOC0((MAX_PATH + 1 + _tcslen(lpszLanguageFileName)) * sizeof(TCHAR));     // for language.ini
            lpszRegshotIni      = MYALLOC0((MAX_PATH + 1 + _tcslen(lpszIniFileName)) * sizeof(TCHAR));          // for regshot.ini
            lpszMessage         = MYALLOC0(REGSHOT_MESSAGE_LENGTH * sizeof(TCHAR));                             // For status bar text message store
            lpszWindowsDirName  = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszTempPath        = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszStartDir        = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszOutputPath      = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));  // store last save/open hive file dir, +1 for possible change in CompareShots()
            lpszEditor          = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszISSOutputFolder = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszISSEditor       = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszNSIOutputFolder = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpszNSIEditor       = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
            lpgrszLangSection   = NULL;

            SendDlgItemMessage(hDlg, IDC_STATIC_BMP, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)ico_green);

            ZeroMemory(&Shot1, sizeof(Shot1));
            ZeroMemory(&Shot2, sizeof(Shot2));
            ZeroMemory(&CompareResult, sizeof(CompareResult));

            if (GetWindowsDirectory(lpszWindowsDirName, MAX_PATH))
                lpszWindowsDirName[MAX_PATH] = (TCHAR)'\0';

            GetTempPath(MAX_PATH, lpszTempPath);  // length incl. NULL character
            lpszTempPath[MAX_PATH] = (TCHAR)'\0';

            //_asm int 3;
            GetCurrentDirectory(MAX_PATH, lpszStartDir);  // length incl. NULL character // fixed in 1.8.2 former version used getcommandline()
            lpszStartDir[MAX_PATH] = (TCHAR)'\0';

            _tcscpy(lpszLanguageIni, lpszStartDir);
            nLen = _tcslen(lpszLanguageIni);
            if (0 < nLen) {
                nLen--;
                  if (lpszLanguageIni[nLen] != (TCHAR)'\\') {
                    _tcscat(lpszLanguageIni, TEXT("\\"));
                }
            }
            _tcscat(lpszLanguageIni, lpszLanguageFileName);

            _tcscpy(lpszRegshotIni, lpszStartDir);
            nLen = _tcslen(lpszRegshotIni);
            if (0 < nLen) {
                nLen--;
                if (lpszRegshotIni[nLen] != (TCHAR)'\\') {
                    _tcscat(lpszRegshotIni, TEXT("\\"));
                }
            }
            _tcscat(lpszRegshotIni, lpszIniFileName);

            CalculateMaxLanguageNameLength();
            SetTextsToDefaultLanguage();
            LoadLanguageFromIni();
            SetTextsToSelectedLanguage(hDlg, FALSE);
            
            EnableMenuItem(hMainMenu, ID_FILE_OPEN, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_FILE_CLEARALL, MF_BYCOMMAND | MF_GRAYED);
//            EnableMenuItem(hMainMenu, ID_FILE_OPTIONS, MF_BYCOMMAND | MF_GRAYED);

            EnableMenuItem(hMainMenu, ID_1_SAVE, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_1_CLEAR, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_1_INFO, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_1_SWAP, MF_BYCOMMAND | MF_GRAYED);

            EnableMenuItem(hMainMenu, ID_2_SHOT, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_SHOTANDSAVE, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_SHOTCOMPAREOUTPUT, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_LOAD, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_SAVE, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_CLEAR, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_INFO, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_2_SWAP, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_COMPARE_COMPARE, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_COMPARE_CLEAR, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMainMenu, ID_COMPARE_INFO, MF_BYCOMMAND | MF_GRAYED);

            EnableMenuItem(hMainMenu, 2, MF_BYPOSITION | MF_GRAYED);
            EnableMenuItem(hMainMenu, 3, MF_BYPOSITION | MF_GRAYED);

            SendMessage(hDlg, WM_COMMAND, (WPARAM)IDC_CHECKDIR, (LPARAM)0);

            LoadSettingsFromIni(hDlg); // tfx
            EnableWindow(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS_LOGS), (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_RESULT), BM_GETCHECK, (WPARAM)0, (LPARAM)0));

// TODO: Subdirectory TITLE
            nLen = _tcslen(lpszOutputPath);
            if (0 < nLen) {
                nLen--;
                if (lpszOutputPath[nLen] != (TCHAR)'\\') {
                    _tcscat(lpszOutputPath, TEXT("\\"));
                }
            }
            _tcscat(lpszOutputPath, lpszTitle);
            nLen = _tcslen(lpszOutputPath);
            if (0 < nLen) {
                nLen--;
                if (lpszOutputPath[nLen] != (TCHAR)'\\') {
                    _tcscat(lpszOutputPath, TEXT("\\"));
                }
            }
            lpszLastSaveDir = lpszOutputPath;
            lpszLastOpenDir = lpszOutputPath;
            bRunning = FALSE;

            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_1STSHOT:  // Button: "1st Shot"
                    lpMenuShot = &Shot1;  // Popup window messages are for 1st Shot
                    lpMenuShot->fFirst = TRUE;
                    fStartDefaults = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_DEFAULTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    if (fStartDefaults) {
                        if (lpMenuShot->fFilled)
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_1_INFO, (LPARAM)0);
                        else
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_1_SHOT, (LPARAM)0);
                        return(TRUE);
                    }
                    if (fStartDefaults) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_COMPARE_OUTPUT, (LPARAM)0);
                        return(TRUE);
                    }
                    UI_CreateShotPopupMenu();
                    GetWindowRect(GetDlgItem(hDlg, IDC_1STSHOT), &rect);
                    TrackPopupMenu(hButtonsMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left + 10, rect.top + 10, 0, hDlg, NULL);
                    DestroyMenu(hButtonsMenu);
                    break;

                case IDC_2NDSHOT:  // Button: "2nd Shot"
                    lpMenuShot = &Shot2;  // Popup window messages are for 2nd Shot
                    lpMenuShot->fFirst = FALSE;
                    fStartDefaults = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_DEFAULTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fAutoCompare = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_AUTOCOMPARE), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    if (fStartDefaults) {
                        if (lpMenuShot->fFilled)
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_2_INFO, (LPARAM)0);
                        else
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)(fAutoCompare ? ID_2_SHOTCOMPAREOUTPUT : ID_2_SHOT), (LPARAM)0);
                        return(TRUE);
                    }
                    UI_CreateShotPopupMenu();
                    GetWindowRect(GetDlgItem(hDlg, IDC_2NDSHOT), &rect);
                    TrackPopupMenu(hButtonsMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left + 10, rect.top + 10, 0, hDlg, NULL);
                    DestroyMenu(hButtonsMenu);
                    break;

                case ID_1_SHOT:         
                case ID_1_SHOTANDSAVE:  
                case ID_1_SAVE:         
                case ID_1_LOAD:         
                case ID_1_CLEAR:        
                case ID_1_INFO:         
                case ID_1_SWAP:         
                case IDC_PROP_CLEAR1:
                    lpMenuShot = &Shot1;
                    lpMenuShot->fFirst = TRUE;
                    if (LOWORD(wParam)== ID_1_SHOT)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SHOTONLY, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_SHOTANDSAVE)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SHOTSAVE, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_SAVE)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SAVE, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_LOAD)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_LOAD, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_CLEAR)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                    else if (LOWORD(wParam) == IDC_PROP_CLEAR1)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_INFO)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_INFO, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_1_SWAP)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SWAP, (LPARAM)0);
                    break;

                case ID_2_SHOT:         
                case ID_2_SHOTCOMPAREOUTPUT: 
                case ID_2_SHOTANDSAVE:  
                case ID_2_SAVE:         
                case ID_2_LOAD:         
                case ID_2_CLEAR:        
                case ID_2_INFO:         
                case ID_2_SWAP:         
                case IDC_PROP_CLEAR2:
                    lpMenuShot = &Shot2;  // Popup window messages are for 2nd Shot
                    lpMenuShot->fFirst = FALSE;
                    if (LOWORD(wParam)== ID_2_SHOT)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SHOTONLY, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_SHOTANDSAVE)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SHOTSAVE, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_SHOTCOMPAREOUTPUT)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SHOTCOMPAREOUTPUT, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_SAVE)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SAVE, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_LOAD)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_LOAD, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_CLEAR)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                    else if (LOWORD(wParam) == IDC_PROP_CLEAR2)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_INFO)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_INFO, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_2_SWAP)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_SWAP, (LPARAM)0);
                    break;

                case IDM_SHOTONLY:  
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    UI_DisableSelectionButtons();
                    UI_GetShotDialogSettings();
                    Shot(lpMenuShot);
                    SetDlgItemText(hDlg, IDC_QUIT, lpszQuit);
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();

                    MessageBeep(0xffffffff);
                    if (!fDontDisplayInfoAfterShot) {
                        DisplayShotInfo(hDlg, lpMenuShot);
                    }
                    if (CheckShotsChronology(hDlg)) {
                        UI_EnableMainButtons();
                    }
                    break;

                case IDM_SHOTCOMPAREOUTPUT:  
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    UI_DisableSelectionButtons();
                    UI_GetShotDialogSettings();
                    Shot(lpMenuShot);
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    //if (!fDontDisplayInfoAfterShot) {
                    //    DisplayShotInfo(hDlg, lpMenuShot);
                    //}
                    if (CheckShotsChronology(hDlg)) {
                        UI_EnableMainButtons();
                    }
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    if (CheckShotsChronology(hDlg)) {
                        CompareShots();
                        OutputComparisonResult();
                    }
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (!fDontDisplayInfoAfterComparison) {
                        DisplayResultInfo(hDlg);
                    }
                    break;

                case IDM_SHOTSAVE:  // Shot Popup Menu: "Shot and Save..."
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    UI_DisableSelectionButtons();
                    UI_GetShotDialogSettings();
                    Shot(lpMenuShot);
                    MessageBeep(0xffffffff);
                    if (bRunning)
                        SaveShot(lpMenuShot);
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (!fDontDisplayInfoAfterShot) {
                        DisplayShotInfo(hDlg, lpMenuShot);
                    }
                    if (CheckShotsChronology(hDlg)) {
                        UI_EnableMainButtons();
                    }
                    break;

                case IDM_LOAD:  // Shot Popup Menu: "Load..."
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    UI_DisableSelectionButtons();
                    LoadShot(lpMenuShot);
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (lpMenuShot->fFilled && !fDontDisplayInfoAfterShot) {
                        DisplayShotInfo(hDlg, lpMenuShot);
                    }
                    if (CheckShotsChronology(hDlg)) {
                        UI_EnableMainButtons();
                    }
                    break;

                case IDM_SAVE:  // Shot Popup Menu: "Save..."
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    UI_GetShotDialogSettings();
                    SaveShot(lpMenuShot);
                    bRunning = FALSE;
                    UI_RemoveHourGlassCursor();
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    break;

                case IDM_SWAP:  // Shot Popup Menu: "Swap"
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
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
                    bRunning = FALSE;
                    UI_RemoveHourGlassCursor();
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (CheckShotsChronology(hDlg)) {
                        UI_EnableMainButtons();
                    }
                    break;

                case IDM_OUTPUT_FS:
                case ID_COMPARE_OUTPUT_FS:
                    bCompareReg = FALSE;
                    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_OUTPUT, (LPARAM)0);
                    bCompareReg = TRUE;
                    break;

                case IDM_OUTPUT_REG:
                case ID_COMPARE_OUTPUT_REG:
                    bCompareFS = FALSE;
                    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_OUTPUT, (LPARAM)0);
                    bCompareFS = TRUE;
                    break;

                case IDM_COMPAREOUTPUT_FS:
                    bCompareReg = FALSE;
                    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPAREOUTPUT, (LPARAM)0);
                    bCompareReg = TRUE;
                    break;

                case IDM_COMPAREOUTPUT_REG:
                    bCompareFS = FALSE;
                    SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPAREOUTPUT, (LPARAM)0);
                    bCompareFS = TRUE;
                    break;

                case ID_COMPARE_COMPARE:  
                case ID_COMPARE_COMPAREANDOUTPUT:
                case ID_COMPARE_COMPAREANDOUTPUT_FS:
                case ID_COMPARE_COMPAREANDOUTPUT_REG:
                case ID_COMPARE_OUTPUT:
                case ID_COMPARE_CLEAR: 
                case ID_COMPARE_INFO:  
                    lpMenuShot = NULL;
                    if (LOWORD(wParam) == ID_COMPARE_COMPARE)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPARE, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_COMPAREANDOUTPUT)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPAREOUTPUT, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_COMPAREANDOUTPUT_FS)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPAREOUTPUT_FS, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_COMPAREANDOUTPUT_REG)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_COMPAREOUTPUT_REG, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_OUTPUT)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_OUTPUT, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_CLEAR)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                    else if (LOWORD(wParam) == ID_COMPARE_INFO)
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_INFO, (LPARAM)0);
                    break;

                case IDC_COMPARE:  // Button: "Compare"
                    lpMenuShot = NULL;
                    UI_CreateComparePopupMenu();
                    fStartDefaults = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_DEFAULTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    if (fStartDefaults) {
                        if (CompareResult.fFilled)
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_COMPARE_OUTPUT, (LPARAM)0);
                        else
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)ID_COMPARE_COMPAREANDOUTPUT, (LPARAM)0);
                        return(TRUE);
                    }
                    GetWindowRect(GetDlgItem(hDlg, IDC_COMPARE), &rect);
                    TrackPopupMenu(hButtonsMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left + 10, rect.top + 10, 0, hDlg, NULL);
                    DestroyMenu(hButtonsMenu);
                    break;

                case IDM_COMPARE:  // Compare Popup Menu: "Compare"
                    bRunning = TRUE;
                    lpMenuShot = NULL;
                    UI_SetHourGlassCursor();
                    if (CheckShotsChronology(hDlg)) {
                        CompareShots();
                    }
                    UI_RemoveHourGlassCursor();
                    if (!bRunning) {
                        SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        return(TRUE);
                    }
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (!fDontDisplayInfoAfterComparison) {
                        DisplayResultInfo(hDlg);
                    }
                    break;

                case IDM_COMPAREOUTPUT:  // Compare Popup Menu: "Compare and Output"
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    if (CheckShotsChronology(hDlg)) {
                        CompareShots();
                        if (bRunning) {
                            OutputComparisonResult();
                        }
                        else {
                            lpMenuShot = NULL;
                            SendMessage(hMainWnd, WM_COMMAND, (WPARAM)IDM_CLEAR, (LPARAM)0);
                        }
                    }
                    bRunning = FALSE;
                    UI_RemoveHourGlassCursor();
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    if (!fDontDisplayInfoAfterComparison) {
                        DisplayResultInfo(hDlg);
                    }
                    break;

                case IDM_OUTPUT:  // Compare Popup Menu: "Output"
                    bRunning = TRUE;
                    UI_SetHourGlassCursor();
                    OutputComparisonResult();
                    UI_RemoveHourGlassCursor();
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    break;

                case ID_FILE_CLEARALL:  // Button: "Clear All"
                case IDC_CLEARALL:  
                    bRunning = TRUE;
                    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOFILTERS), BM_SETCHECK, (WPARAM)FALSE, (LPARAM)0);
                    UI_SetHourGlassCursor();
                    cEnd = Shot1.stCounts.cAll + Shot2.stCounts.cAll + CompareResult.stcChanged.cAll;
                    UI_InitProgressBar();
                    Shot1.fFirst = TRUE;
                    FreeShot(&Shot1);
                    FreeShot(&Shot2);
                    bRegSkipAdded = FALSE;
                    bRegWhitelistAdded = FALSE;
                    bFileSkipAdded = FALSE;
                    FreeCompareResult();
                    if (0 != cEnd) {
                        nCurrentTime = GetTickCount64();
                        UI_UpdateProgressBar();
                    }
                    UI_ShowHideProgressBar(SW_HIDE);
                    UI_RemoveHourGlassCursor();
                    bRunning = FALSE;
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    break;

                case IDM_CLEAR:  // Shot/Compare Popup Menu: "Clear"
                    UI_SetHourGlassCursor();
                    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_NOFILTERS), BM_SETCHECK, (WPARAM)FALSE, (LPARAM)0);
                    cEnd = 0;
                    if (NULL != lpMenuShot) {
                        cEnd = lpMenuShot->stCounts.cAll;
                    }
                    cEnd += CompareResult.stcChanged.cAll;
                    UI_InitProgressBar();
                    if (NULL != lpMenuShot) {
                        FreeShot(lpMenuShot);
                        bRegSkipAdded = FALSE;
                        bRegWhitelistAdded = FALSE;
                        bFileSkipAdded = FALSE;
                    }
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
                    UI_RemoveHourGlassCursor();
                    UI_EnableMainButtons();
                    MessageBeep(0xffffffff);
                    break;

                case IDM_INFO:  // Shot/Compare Popup Menu: "Info"
                    if (NULL != lpMenuShot) {
                        DisplayShotInfo(hDlg, lpMenuShot);
                    } else {
                        DisplayResultInfo(hDlg);
                    }
                    break;

                case IDC_CHECK_RESULT:
                    EnableWindow(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS_LOGS), (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_RESULT), BM_GETCHECK, (WPARAM)0, (LPARAM)0));
                    break;

                case IDC_CHECKDIR:
                    EnableWindow(GetDlgItem(hDlg, IDC_PROP_SCANS), (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECKDIR), BM_GETCHECK, (WPARAM)0, (LPARAM)0));
                    break;

                case IDC_CHECK_HKLM:
                case IDC_CHECK_HKU:
                case IDC_CHECK_HKCU:
                case IDC_CHECK_REGDEL:
                case IDC_CHECK_REGINS:
                case IDC_CHECK_ONLYADDED:
                    break;

                case IDC_CHECK_SUPPRESS_LOGS:
                    fSuppressLogs = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS_LOGS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;

                case IDC_CHECK_DEFAULTS:
                    EnableWindow(GetDlgItem(hDlg, IDC_CHECK_AUTOCOMPARE), (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_DEFAULTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0));
                    break;

                case IDC_SAVE_INI:
                    SaveSettingsToIni(hDlg);
                    break;

                case IDC_LOAD_INI:
                    LoadSettingsFromIni(hDlg);
                    break;

                case ID_FILE_QUIT:  
                case IDC_QUIT:  
                case IDCANCEL: {    // Button: Window Close
                    if (bRunning) {
                        bRunning = FALSE;
                        SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT4, WM_SETTEXT, (WPARAM)0, (LPARAM)TEXT(""));
                        return(TRUE);
                    }
                    if (fStoreOnQuit) {
                        if (!CheckSavingShots(hDlg, &Shot1))
                            return(TRUE);
                        if (!CheckSavingShots(hDlg, &Shot2))
                            return(TRUE);;
                    }
                    if (fSaveSettingsOnExit)
                        SaveSettingsToIni(hDlg);
                    CleanUpGlobalMemory();
                    PostQuitMessage(0);
//_CrtDumpMemoryLeaks();
                    break;
                }

                case IDC_BROWSE2: {  // Button: Output path
                    LPITEMIDLIST lpidlist;

                    BrowseInfo1.hwndOwner = hDlg;
                    BrowseInfo1.pszDisplayName = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                    //BrowseInfo1.lpszTitle = TEXT("Select:");
                    //-----------------
                    // Added by Youri in 1.8.2 ,Thanks!
                    // if you add this code, the browse dialog will be expand path and have button "Create Folder"
                    BrowseInfo1.ulFlags |= 0x0040; // BIF_NEWDIALOGSTYLE;    // button "Create Folder" and resizable
                    BrowseInfo1.lpfn = SelectBrowseFolder;                   // function for expand path
                    BrowseInfo1.lParam = (LPARAM)BrowseInfo1.pszDisplayName;

                    // Initialize selection path
                    LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
                    GetDlgItemText(hDlg, IDC_EDITPATH, BrowseInfo1.pszDisplayName, MAX_PATH); 
                    ExpandEnvironmentStrings(BrowseInfo1.pszDisplayName, lpszTempDirBuffer, MAX_PATH);
                    _tcscpy(BrowseInfo1.pszDisplayName, lpszTempDirBuffer);
                    MYFREE(lpszTempDirBuffer);

                    lpidlist = SHBrowseForFolder(&BrowseInfo1);
                    if (NULL != lpidlist) {
                        SHGetPathFromIDList(lpidlist, BrowseInfo1.pszDisplayName);
                        //lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
                        //if (PathUnExpandEnvStrings(BrowseInfo1.pszDisplayName, lpszTempDirBuffer, MAX_PATH)) {
                        //    _tcscpy(BrowseInfo1.pszDisplayName, lpszTempDirBuffer);
                        //}
                        //MYFREE(lpszTempDirBuffer);
                        SetDlgItemText(hDlg, IDC_EDITPATH, BrowseInfo1.pszDisplayName);
                        MYFREE(lpidlist);
                    }

                    MYFREE(BrowseInfo1.pszDisplayName);
                }
                return(TRUE);

                case ID_HELP_ABOUT:   // Button: About
                case IDC_ABOUT: {  
                    LPTSTR   lpszAboutBox;
                    //_asm int 3;
                    lpszAboutBox = MYALLOC0(SIZEOF_ABOUTBOX * sizeof(TCHAR));
                    // it is silly that when wsprintf encounters a NULL string, it will write the whole string to NULL!
                    _sntprintf(lpszAboutBox, SIZEOF_ABOUTBOX, TEXT("%s%s%s%s%s%s\0"), lpszAboutRegshot, TEXT("["), lpszLanguage, TEXT("]"), TEXT(" by: "), lpszCurrentTranslator);
                    lpszAboutBox[SIZEOF_ABOUTBOX - 1] = (TCHAR)'\0';  // safety NULL char
                    MessageBox(hDlg, lpszAboutBox, asLangTexts[iszTextAbout].lpszText, MB_OK);
                    MYFREE(lpszAboutBox);
                    break;
                }
                
                case IDC_PROP_SCANS:
                    DoPropertySheet(hMainWnd, PROP_SCANS);
                    break;

                case IDC_PROP_REGS:
                    DoPropertySheet(hMainWnd, PROP_REGS);
                    break;
                
                case IDC_PROP_DIRS:
                    DoPropertySheet(hMainWnd, PROP_DIRS);
                    break;
                
                case IDC_PROP_LOGS:
                    DoPropertySheet(hMainWnd, PROP_COMMON_2);
                    break;

                case IDC_RESET:
                    ResetOutputOptions();
                    break;

                case ID_FILE_OPTIONS:
                case ID_SETTINGS:
                    DoPropertySheet(hMainWnd, PROP_COMMON);
                    break; 

                case IDC_COMBO_MAINCP:  // Combo Box: Language
                    if (CBN_SELCHANGE == HIWORD(wParam)) {  // Only react when user selected something
                        //SetTextsToDefaultLanguage();
                        //SetTextsToSelectedLanguage(hDlg, TRUE);
                        int nResult = (int)SendDlgItemMessage(hDlg, IDC_COMBO_MAINCP, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                        if (CB_ERR == nResult) {
                            return(TRUE);
                        }
                        if (nResult == 0)
                            nCodePage = 0;
                        else if (nResult == 1)
                            nCodePage = -1;
                        else
                            nCodePage = 65001;
                        UI_EnableMainButtons();
                        return(TRUE);
                    }
                    break;
                
                case IDC_CHECK_UNL:
                    fOutputUNLfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                
                case IDC_CHECK_BAT:
                    fOutputBATfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                
                case IDC_CHECK_HTML:
                    fOutputTXTfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
            }
    }
    return(FALSE);
}

/*BOOL SetPrivilege(HANDLE hToken, LPCSTR pString, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES    tp;
    LUID    luid;
    TOKEN_PRIVILEGES    tpPrevious;
    DWORD   cbSize = sizeof(TOKEN_PRIVILEGES);

    if (!LookupPrivilegeValue(NULL,pString,&luid))
        return FALSE;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = 0;
    if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),&tpPrevious,&cbSize))
        return FALSE;
    tpPrevious.PrivilegeCount = 1;
    tpPrevious.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tpPrevious.Privileges[0].Attributes| = (SE_PRIVILEGE_ENABLED);
    else
        tpPrevious.Privileges[0].Attributes^ = ((tpPrevious.Privileges[0].Attributes)&(SE_PRIVILEGE_ENABLED));
    if (!AdjustTokenPrivileges(hToken,FALSE,&tpPrevious,cbSize,NULL,NULL))
        return FALSE;
    return TRUE;
}*/

/*int
#if defined(_M_CEE_PURE)
__clrcall
#else
WINAPI
#endif
wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
    );
*/

//#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//#include <iostream>

//extern "C"
//{
//    int wWinMainCRTStartup(void);
//    int MyEntryPoint(void);
//}
//
//__declspec(noinline) int MyEntryPoint(void)
//{
//    // __debugbreak();
//    BOOL Result = AllocConsole();
//    freopen("CON", "w", stdout);
//    freopen("CON", "w", stderr);
//    freopen("CON", "r", stdin);
//    return wWinMainCRTStartup();
//}
#ifdef C_CONSOLE
int main(int argc, char* argv[], char* envp[])
{
    UNREFERENCED_PARAMETER(envp);

#ifdef USEHEAPALLOC_DANGER
    hHeap = GetProcessHeap(); // 1.8.2
#endif
    char* pArg;
    BOOL bSnp2 = FALSE;
    bNoGui = TRUE;

    /*
        BOOL Result = AllocConsole();
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
        freopen("CON", "r", stdin);
    */

    /*
        int hCrt;
        FILE* hf;

// https://www.betaarchive.com/wiki/index.php/Microsoft_KB_Archive/105305
        AllocConsole();
        hCrt = _open_osfhandle(
            (long)GetStdHandle(STD_OUTPUT_HANDLE),
            _O_TEXT
        );
        hf = _fdopen(hCrt, "w");
        *stdout = *hf;
        int i = setvbuf(stdout, NULL, _IONBF, 0);
        printf(stdout, L"This is a test to stdout\n");
        printf(stderr, L"This is a test to stderr\n");
    */
//    fwprintf(stdout, L"This is a test to stdout\n");
    _tprintf(_T("############################################################\n"));
    _tprintf(_T("%s (Console version)\n"), lpszProgramName);
    _tprintf(_T("############################################################\n"));
    _tprintf(_T("%s\n"), lpszAboutRegshot);
    _tprintf(_T("############################################################\n"));
    if (argc > 1) {
        lpszRegshotIni = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));   // for regshot.ini
        for (int i = 1; i < argc; i++) {
            pArg = argv[i];
            if (strcmp(pArg, "-snp2") == 0)
                bSnp2 = TRUE;
            if (strcmp(pArg, "-f") == 0) {
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)argv[i + 1], -1,lpszRegshotIni,  MAX_PATH);
            }
        }

        bNoGui = TRUE;
        bRunning = TRUE;
        lpMenuShot = (bSnp2 ? &Shot2 : &Shot1);
        lpMenuShot->fFirst = (bSnp2 ? FALSE : TRUE);
        lpszTitle = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));      // TITLELEN is actually MAX_PATH * 4
        lpszExtDir = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));      // EXTDIRLEN is actually MAX_PATH * 4
        lpszOutputPath = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));  // store last save/open hive file dir, +1 for possible change in CompareShots()
        lpszLastSaveDir = lpszOutputPath;
        LoadSettingsFromIni(NULL);
        BYTE nFlag = (BYTE)GetPrivateProfileInt(lpszIniSetup, TEXT("Flag"), 1, lpszRegshotIni); // default from 0 to 1 in 1.8.2 (TEXT)
        fFileshot = (nFlag & 0x08) >> 3;
        printf("Starting shot.\n");
        Shot(lpMenuShot);
        printf("Saving shot.\n");
        SaveShot(lpMenuShot);
        bRunning = FALSE;
        printf("Shot stored.\n");

        return 0;
    }
}

#else
#include <crtdbg.h>
int WINAPI _tWinMain(_In_ HINSTANCE hInstance, 
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_  LPTSTR lpszCmdLine, 
                     _In_ int nCmdShow)
{

    /*
    BOOL    bWinNTDetected;
    HANDLE  hToken = 0;
    OSVERSIONINFO winver;
    winver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&winver);
    bWinNTDetected = (winver.dwPlatformId == VER_PLATFORM_WIN32_NT) ? TRUE : FALSE;
    //hWndMonitor be created first for the multilanguage interface.

    //FARPROC       lpfnDlgProc;
    //lpfnDlgProc = MakeProcInstance((FARPROC)DialogProc,hInstance);    // old style of create dialogproc
    */
    UNREFERENCED_PARAMETER(lpszCmdLine);
    UNREFERENCED_PARAMETER(hPrevInstance);
    hInst = hInstance;

#ifdef USEHEAPALLOC_DANGER
    hHeap = GetProcessHeap(); // 1.8.2
//    hHeap = HeapCreate(0,0,0);
#endif

    bNoGui = FALSE;
    ico_green = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_GREEN));
    ico_red = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_RED));
    hMainWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);

    SetClassLongPtr(hMainWnd, GCLP_HICON, (LONG_PTR)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON)));
//    _CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_CHECK_ALWAYS_DF);

    SetWindowText(hMainWnd, lpszProgramName);  // tfx set program title to lpszProgramName to avoid edit resource file
    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);
    //SetPriorityClass(hInstance,31);
    /*if (bWinNTDetected)
      {
          if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken) == TRUE)
          {
              if (SetPrivilege(hToken,"SeSystemProfilePrivilege",TRUE) == TRUE)
              {
                  MessageBox(hWnd,"We are in system level,enjoy!","Info:",MB_OK);
              }
              CloseHandle(hToken);
          }
      }*/
    while (GetMessage(&msg, NULL, (WPARAM)NULL, (LPARAM)NULL)) {
        if (!IsDialogMessage(hMainWnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return(int)(msg.wParam);
}
#endif // C_CONSOLE

#ifdef REPLACEMENT_STRNLEN
size_t strnlen(const char *lpszText, size_t cchMax)
{
    size_t cchActual;

    for (cchActual = 0; (cchActual < cchMax) && ('\0' != lpszText[cchActual]); cchActual++) {
    }

    return cchActual;
}
#endif
