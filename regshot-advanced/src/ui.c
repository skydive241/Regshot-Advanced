/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
    Copyright 1999-2003,2007 TiANWEi
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

LPTSTR lpszMessage;

HCURSOR hHourGlass = NULL;   // Handle of cursor
HCURSOR hSaveCursor = NULL;  // Handle of cursor

// Some vars used to update the "status bar"
ULONGLONG nStartTime;
ULONGLONG nCurrentTime;
ULONGLONG nLastTime;

// Some vars used to update the progress bar
DWORD cCurrent;
DWORD cEnd;

// ----------------------------------------------------------------------
// Displays/Hides counters in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_ShowHideCounters(int nCmdShow)  // 1.8.2
{
    ShowWindow(GetDlgItem(hMainWnd, IDC_TEXTCOUNT1), nCmdShow);
    ShowWindow(GetDlgItem(hMainWnd, IDC_TEXTCOUNT2), nCmdShow);
    ShowWindow(GetDlgItem(hMainWnd, IDC_TEXTCOUNT3), nCmdShow);
    ShowWindow(GetDlgItem(hMainWnd, IDC_TEXTCOUNT4), SW_SHOW);
    UpdateWindow(hMainWnd);
}

// ----------------------------------------------------------------------
// Displays/Hides progress bar in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_GetShotDialogSettings(VOID)
{
    GetDlgItemText(hMainWnd, IDC_EDITTITLE, lpszTitle, MAX_PATH);
    fHKLM = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    fHKU = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKU), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    fHKCU = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    fFileshot = (BOOL)SendMessage(GetDlgItem(hMainWnd, IDC_CHECKDIR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
    GetDlgItemText(hMainWnd, IDC_EDITDIR, lpszExtDir, EXTDIRLEN);
    GetDlgItemText(hMainWnd, IDC_EDITPATH, lpszOutputPath, MAX_PATH);  // length incl. NULL character
}

// ----------------------------------------------------------------------
// Displays/Hides progress bar in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_ShowHideProgressBar(int nCmdShow)
{
    ShowWindow(GetDlgItem(hMainWnd, IDC_PROGBAR), nCmdShow);
    UpdateWindow(hMainWnd);
}

// ----------------------------------------------------------------------
// Resets and displays counters in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_InitCounters(VOID)
{
    nCurrentTime = 0;
    nStartTime = GetTickCount64();
    nLastTime = nStartTime;

    if (bNoGui)
        return;

    UI_ShowHideProgressBar(SW_HIDE);
    _tcscpy(lpszMessage, TEXT(" "));  // clear the counters
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT1, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT2, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT3, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);
    UI_ShowHideCounters(SW_SHOW);
}

// ----------------------------------------------------------------------
// Resets and displays progress bar in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_InitProgressBar(VOID)
{
    cCurrent = 0;
    nCurrentTime = 0;
    nStartTime = GetTickCount64();
    nLastTime = nStartTime;

    if (bNoGui)
        return;

    UI_ShowHideCounters(SW_HIDE);  // 1.8.2
    SendDlgItemMessage(hMainWnd, IDC_PROGBAR, PBM_SETRANGE32, (WPARAM)0, (LPARAM)MAXPBPOSITION);
    SendDlgItemMessage(hMainWnd, IDC_PROGBAR, PBM_SETPOS, (WPARAM)0, (LPARAM)0);
    UI_ShowHideProgressBar(SW_SHOW);
}

// ----------------------------------------------------------------------
// Update counters in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_UpdateCounters(LPTSTR lpszTitle1, LPTSTR lpszTitle2, DWORD nCount1, DWORD nCount2, LPTSTR lpszTitle3, LPTSTR lpszKeyname)
{
    UNREFERENCED_PARAMETER(lpszTitle3);

    if (bNoGui)
        return;

    // Remember current time for next update interval
    nLastTime = nCurrentTime;

    // Update "status bar"
    _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %I64us%03I64ums\0"), asLangTexts[iszTextTime].lpszText, (nCurrentTime - nStartTime) / 1000, (nCurrentTime - nStartTime) % 1000);
    lpszMessage[REGSHOT_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT3, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);

    _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %u\0"), lpszTitle1, nCount1);
    lpszMessage[REGSHOT_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT1, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);

    _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %u\0"), lpszTitle2, nCount2);
    lpszMessage[REGSHOT_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT2, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);

    if (lpszKeyname != NULL) {
//        _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %s\0"), lpszTitle3, lpszKeyname);
        _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s\0"), lpszKeyname);
        lpszMessage[REGSHOT_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
    }
    else {
//        _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s %s\0"), lpszTitle3, TEXT("-"));
        _sntprintf(lpszMessage, REGSHOT_MESSAGE_LENGTH, TEXT("%s\0"), TEXT(""));
        lpszMessage[REGSHOT_MESSAGE_LENGTH - 1] = (TCHAR)'\0';  // safety NULL char
    }
    SendDlgItemMessage(hMainWnd, IDC_TEXTCOUNT4, WM_SETTEXT, (WPARAM)0, (LPARAM)lpszMessage);

    // Refresh window display
    UpdateWindow(hMainWnd);
    PeekMessage(&msg, hMainWnd, WM_ACTIVATE, WM_ACTIVATE, PM_REMOVE);

    // tell system we are still here
    UI_Refresh();
}

// ----------------------------------------------------------------------
// Update progress bar in the "status bar" of the window
// ----------------------------------------------------------------------
VOID UI_UpdateProgressBar(VOID)
{
    DWORD nPBPos;

    if (bNoGui)
        return;

    // Remember current time for next update interval
    nLastTime = nCurrentTime;

    // Update "status bar"
    if (0 != cEnd) {
        nPBPos = (DWORD)(cCurrent * (__int64)MAXPBPOSITION / cEnd);
        SendDlgItemMessage(hMainWnd, IDC_PROGBAR, PBM_SETPOS, (WPARAM)nPBPos, (LPARAM)0);

        // Refresh window display
        UpdateWindow(hMainWnd);
        PeekMessage(&msg, hMainWnd, WM_ACTIVATE, WM_ACTIVATE, PM_REMOVE);
    }
}

// ----------------------------------------------------------------------
// Set mouse cursor to hour glass
// ----------------------------------------------------------------------
VOID UI_SetHourGlassCursor(VOID)
{
    SetDlgItemText(hMainWnd, IDC_QUIT, lpszStop);
    SendDlgItemMessage(hMainWnd, IDC_STATIC_BMP, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)ico_red);

    if (NULL == hHourGlass) {
        hHourGlass = LoadCursor(NULL, IDC_WAIT);
    }
    hSaveCursor = SetCursor(hHourGlass);
}

// ----------------------------------------------------------------------
// Set back to previous mouse cursor
// ----------------------------------------------------------------------
VOID UI_RemoveHourGlassCursor(VOID)
{
    SetDlgItemText(hMainWnd, IDC_QUIT, lpszQuit);
    SendDlgItemMessage(hMainWnd, IDC_STATIC_BMP, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)ico_green);
    SetCursor(hSaveCursor);
}

// ----------------------------------------------------------------------
// Enable and disable buttons depending on status
// ----------------------------------------------------------------------
VOID UI_DisableSelectionButtons(VOID)
{
    EnableWindow(GetDlgItem(hMainWnd, IDC_EDITTITLE), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKU), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECKDIR), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_EDITDIR), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_BROWSE1), FALSE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_LOAD_INI), FALSE);

    UpdateWindow(hMainWnd);
}

// ----------------------------------------------------------------------
// Enable and disable buttons depending on status
// ----------------------------------------------------------------------
VOID UI_EnableMainButtons(VOID)
{
    DWORD nIDDefault;
    BOOL fEnable;

    EnableMenuItem(hMainMenu, 2, MF_BYPOSITION | MF_GRAYED);
    EnableMenuItem(hMainMenu, 3, MF_BYPOSITION | MF_GRAYED);
    
    // Button: 1st Shot
    nIDDefault = IDC_1STSHOT;
    EnableWindow(GetDlgItem(hMainWnd, IDC_1STSHOT), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_EDITTITLE), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKU), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_CHECKDIR), TRUE);
//    EnableWindow(GetDlgItem(hMainWnd, IDC_EDITDIR), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_BROWSE1), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_1STSHOT), TRUE);
    EnableWindow(GetDlgItem(hMainWnd, IDC_LOAD_INI), TRUE);

    if (Shot1.fFilled) {
        EnableMenuItem(hMainMenu, ID_1_SHOT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_SHOTANDSAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_LOAD, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_SAVE, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_CLEAR, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_INFO, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_SWAP, MF_BYCOMMAND | MF_ENABLED);
        EnableWindow(GetDlgItem(hMainWnd, IDC_PROP_CLEAR1), TRUE);
    }
    else {
        EnableMenuItem(hMainMenu, ID_1_SHOT, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_SHOTANDSAVE, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_LOAD, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_1_SAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_CLEAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_INFO, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_1_SWAP, MF_BYCOMMAND | MF_GRAYED);
        EnableWindow(GetDlgItem(hMainWnd, IDC_PROP_CLEAR1), FALSE);
    }
    if (Shot2.fFilled) {
        EnableMenuItem(hMainMenu, ID_2_SHOT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_SHOTANDSAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_LOAD, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_SAVE, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_CLEAR, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_INFO, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_SWAP, MF_BYCOMMAND | MF_ENABLED);
        EnableWindow(GetDlgItem(hMainWnd, IDC_PROP_CLEAR2), TRUE);
    }
    else {
        EnableMenuItem(hMainMenu, ID_2_SHOT, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_SHOTANDSAVE, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_LOAD, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_2_SAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_CLEAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_INFO, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_2_SWAP, MF_BYCOMMAND | MF_GRAYED);
        EnableWindow(GetDlgItem(hMainWnd, IDC_PROP_CLEAR2), FALSE);
    }

    if (CompareResult.fFilled) {
        EnableMenuItem(hMainMenu, ID_COMPARE_COMPARE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT_FS, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT_REG, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_CLEAR, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_INFO, MF_BYCOMMAND | MF_ENABLED);
    }
    else {
        EnableMenuItem(hMainMenu, ID_COMPARE_COMPARE, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT_FS, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_OUTPUT_REG, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_CLEAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hMainMenu, ID_COMPARE_INFO, MF_BYCOMMAND | MF_GRAYED);
    }
    
    // Button: 2nd Shot
    fEnable = FALSE;
    if ((Shot2.fFilled) || (Shot1.fFilled)) {
        fEnable = TRUE;
        if (Shot1.fFilled) {
            nIDDefault = IDC_2NDSHOT;
            EnableWindow(GetDlgItem(hMainWnd, IDC_EDITTITLE), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKLM), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKU), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_CHECK_HKCU), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_CHECKDIR), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_EDITDIR), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_BROWSE1), FALSE);
            EnableWindow(GetDlgItem(hMainWnd, IDC_LOAD_INI), FALSE);
        }
    }
    EnableWindow(GetDlgItem(hMainWnd, IDC_2NDSHOT), fEnable);
    EnableMenuItem(hMainMenu, 2, MF_BYPOSITION | (fEnable?MF_ENABLED:MF_GRAYED));

    // Button: Compare
    fEnable = FALSE;
    if ((CompareResult.fFilled) || (Shot1.fFilled && Shot2.fFilled)) {
        fEnable = TRUE;
        nIDDefault = IDC_COMPARE;
    }
    EnableWindow(GetDlgItem(hMainWnd, IDC_COMPARE), fEnable);
    EnableMenuItem(hMainMenu, 3, MF_BYPOSITION | (fEnable ? MF_ENABLED : MF_GRAYED));

    // Button: Clear all
    fEnable = FALSE;
    if ((Shot1.fFilled) || (Shot2.fFilled) || (CompareResult.fFilled)) {
        fEnable = TRUE;
    }
    EnableWindow(GetDlgItem(hMainWnd, IDC_CLEARALL), fEnable);
    EnableMenuItem(hMainMenu, ID_FILE_CLEARALL, MF_BYCOMMAND | (fEnable ? MF_ENABLED : MF_GRAYED));

    // Set default button
    SendMessage(hMainWnd, DM_SETDEFID, (WPARAM)nIDDefault, (LPARAM)0);
    SetFocus(GetDlgItem(hMainWnd, nIDDefault));

    DrawMenuBar(hMainWnd);
    UpdateWindow(hMainWnd);
}

//--------------------------------------------------
// Show popup menu for shot buttons
//--------------------------------------------------
VOID UI_CreateShotPopupMenu(VOID)
{
    DWORD nIDDefault;

    hButtonsMenu = CreatePopupMenu();

    if (!lpMenuShot->fFirst)
        AppendMenu(hButtonsMenu, MF_STRING, IDM_SHOTCOMPAREOUTPUT, asLangTexts[iszTextMenuShotCompareOutput].lpszText);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_SHOTONLY, asLangTexts[iszTextMenuShot].lpszText);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_SHOTSAVE, asLangTexts[iszTextMenuShotSave].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_LOAD, asLangTexts[iszTextMenuLoad].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_SAVE, asLangTexts[iszTextMenuSave].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_CLEAR, asLangTexts[iszTextMenuClear].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_INFO, asLangTexts[iszTextMenuInfo].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_SWAP, asLangTexts[iszTextMenuSwap].lpszText);

    if (lpMenuShot->fFilled) {
        nIDDefault = IDM_INFO;
        EnableMenuItem(hButtonsMenu, IDM_SHOTCOMPAREOUTPUT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_SHOTONLY, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_SHOTSAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_LOAD, MF_BYCOMMAND | MF_GRAYED);
    } else {
        nIDDefault = IDM_SHOTONLY;
        if (!lpMenuShot->fFirst)
            nIDDefault = IDM_SHOTCOMPAREOUTPUT;
        EnableMenuItem(hButtonsMenu, IDM_CLEAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_SAVE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_INFO, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_SWAP, MF_BYCOMMAND | MF_GRAYED);
    }

    SetMenuDefaultItem(hButtonsMenu, nIDDefault, FALSE);
}

//--------------------------------------------------
// Show popup menu for compare button
//--------------------------------------------------
VOID UI_CreateComparePopupMenu(VOID)
{
    DWORD nIDDefault;

    hButtonsMenu = CreatePopupMenu();

    AppendMenu(hButtonsMenu, MF_STRING, IDM_COMPARE, asLangTexts[iszTextButtonCompare].lpszText);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_COMPAREOUTPUT, asLangTexts[iszTextMenuCompareOutput].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_OUTPUT, asLangTexts[iszTextMenuOutput].lpszText);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_OUTPUT_FS, asLangTexts[iszTextMenuOutputFS].lpszText);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_OUTPUT_REG, asLangTexts[iszTextMenuOutputReg].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_CLEAR, asLangTexts[iszTextMenuClear].lpszText);
    AppendMenu(hButtonsMenu, MF_SEPARATOR, IDM_BREAK, NULL);
    AppendMenu(hButtonsMenu, MF_STRING, IDM_INFO, asLangTexts[iszTextMenuInfo].lpszText);

    if (CompareResult.fFilled) {
        nIDDefault = IDM_INFO;
        EnableMenuItem(hButtonsMenu, IDM_COMPARE, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_COMPAREOUTPUT, MF_BYCOMMAND | MF_GRAYED);
    } else {
        nIDDefault = IDM_COMPARE;
        EnableMenuItem(hButtonsMenu, IDM_OUTPUT, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_OUTPUT_FS, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_OUTPUT_REG, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_CLEAR, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hButtonsMenu, IDM_INFO, MF_BYCOMMAND | MF_GRAYED);
    }

    SetMenuDefaultItem(hButtonsMenu, nIDDefault, FALSE);
}

// ----------------------------------------------------------------------
// Hopefully tell the OS we are still here and working... RH
// and this will stop "Not Responding" in the heading...
// ----------------------------------------------------------------------
int UI_Refresh(VOID)
{
    MSG       msgRefresh;
    BOOL  rc;

    //    rc = EnableWindow(hWnd, TRUE);   // make sure window is enabled.
    rc = PeekMessage(&msgRefresh, NULL, 0, 0, PM_NOREMOVE);
    if (rc > 0)
    {
        switch (msgRefresh.message) {
        case WM_QUIT:
        case WM_KEYDOWN:
//        case WM_LBUTTONDOWN:
// See if the ESC key or "q" was pressed, ask to quit
            if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) || (GetAsyncKeyState(0x51) & 0x8000))
            {
                LPTSTR lpMsgBox = (LPTSTR)MYALLOC(SIZEOF_ABOUTBOX);
                if (lpMsgBox) {
                    _tcscpy(lpMsgBox, TEXT("Do you really want to quit?\n"));
                    rc = MessageBox(NULL, lpMsgBox, TEXT("Query"), MB_YESNO + MB_ICONSTOP + MB_DEFBUTTON2);
                    MYFREE(lpMsgBox);
                    if (rc == IDYES) {
                        exit(0);
                    }
                }
            }        // end of if ((GetKeyState...
            return(1);
        //case WM_COMMAND:
        //    if (LOWORD(msg.wParam) == IDC_ABOUT) {
        //        return(TRUE);
        //    }
        //    return(TRUE);
        }
    }
    if (PeekMessage(&msgRefresh, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msgRefresh);
        DispatchMessage(&msgRefresh);
    }

    return(0);
}

