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

/*
https://docs.microsoft.com/en-us/windows/win32/dlgbox/using-dialog-boxes
https://docs.microsoft.com/en-us/windows/win32/controls/list-view-control-reference
https://github.com/mity/old-new-win32api#dialogs
http://www.oehive.org/book/export/html/385.html
https://www.codeproject.com/Articles/2890/Using-ListView-control-under-Win32-API
https://www.codeproject.com/Articles/9148/C-functions-to-insert-columns-items-and-subitems-i

https://docs.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_insertitem
https://docs.microsoft.com/en-us/windows/win32/controls/bumper-list-view-control-reference-macros
*/

#include "global.h"

BOOL CALLBACK DlgSkipProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgOptionsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DoContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID DoPropertySheet(HWND hwndOwner, int iSelectedPage);
VOID SetCheckStates(HWND hDlg, int iCase);
//BOOL CALLBACK DlgPropSheetProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

VOID DoPropertySheet(HWND hwndOwner, int iSelectedPage)
{
    PROPSHEETPAGE psp[PROP_FILES + 1];
    PROPSHEETHEADER psh;

    psp[PROP_COMMON].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_COMMON].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_COMMON].hInstance = hInst;
    psp[PROP_COMMON].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_COMMON);
    psp[PROP_COMMON].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_MAINICON);
    psp[PROP_COMMON].pfnDlgProc = (DLGPROC)DlgOptionsProc;
    psp[PROP_COMMON].pszTitle = TEXT("Common"); // MAKEINTRESOURCE(IDS_FONT)
    psp[PROP_COMMON].lParam = PROP_COMMON;
    psp[PROP_COMMON].pfnCallback = NULL;
   
    psp[PROP_COMMON_2].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_COMMON_2].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_COMMON_2].hInstance = hInst;
    psp[PROP_COMMON_2].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_COMMON_2);
    psp[PROP_COMMON_2].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_MAINICON);
    psp[PROP_COMMON_2].pfnDlgProc = (DLGPROC)DlgOptionsProc;
    psp[PROP_COMMON_2].pszTitle = TEXT("Additional formats"); // MAKEINTRESOURCE(IDS_FONT)
    psp[PROP_COMMON_2].lParam = PROP_COMMON_2;
    psp[PROP_COMMON_2].pfnCallback = NULL;
   
    psp[PROP_SCANS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_SCANS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_SCANS].hInstance = hInst;
    psp[PROP_SCANS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_SCANDIR);
    psp[PROP_SCANS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_SCANS].pfnDlgProc = (DLGPROC)DlgSkipProc;
    psp[PROP_SCANS].pszTitle = TEXT("Filesystem scan settings"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_SCANS].lParam = PROP_SCANS;
    psp[PROP_SCANS].pfnCallback = NULL;

    psp[PROP_WLREGS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_WLREGS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_WLREGS].hInstance = hInst;
    psp[PROP_WLREGS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_SCANREG);
    psp[PROP_WLREGS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_WLREGS].pfnDlgProc = (DLGPROC)DlgSkipProc;
    psp[PROP_WLREGS].pszTitle = TEXT("Registry whitelist settings"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_WLREGS].lParam = PROP_WLREGS;
    psp[PROP_WLREGS].pfnCallback = NULL;

    psp[PROP_REGS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_REGS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_REGS].hInstance = hInst;
    psp[PROP_REGS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_SKIPREG);
    psp[PROP_REGS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_REGS].pfnDlgProc = (DLGPROC)DlgSkipProc;
    psp[PROP_REGS].pszTitle = TEXT("Registry skip settings"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_REGS].lParam = PROP_REGS;
    psp[PROP_REGS].pfnCallback = NULL;

    psp[PROP_DIRS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_DIRS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_DIRS].hInstance = hInst;
    psp[PROP_DIRS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_SKIPDIR);
    psp[PROP_DIRS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_DIRS].pfnDlgProc = (DLGPROC)DlgSkipProc;
    psp[PROP_DIRS].pszTitle = TEXT("Directory skip settings"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_DIRS].lParam = PROP_DIRS;
    psp[PROP_DIRS].pfnCallback = NULL;

    psp[PROP_FILES].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_FILES].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_FILES].hInstance = hInst;
    psp[PROP_FILES].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_OPT_SKIPFILE);
    psp[PROP_FILES].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_FILES].pfnDlgProc = (DLGPROC)DlgSkipProc;
    psp[PROP_FILES].pszTitle = TEXT("File skip settings"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_FILES].lParam = PROP_FILES;
    psp[PROP_FILES].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
    psh.hwndParent = hwndOwner;
    psh.hInstance = hInst;
    psh.pszIcon = MAKEINTRESOURCE(IDI_MAINICON);
    psh.pszCaption = TEXT("Options");
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = iSelectedPage;
    psh.ppsp = (LPCPROPSHEETPAGE)&psp;
    psh.pfnCallback = NULL;

    PropertySheet(&psh);

    return;
}

VOID SetCheckStates(HWND hDlg, int iCase)
{
    HWND hWndListViewSkip = GetDlgItem(hDlg, IDC_LIST_SKIP);
    BOOL bState = FALSE;
    for (int i = 0; i < ListView_GetItemCount(hWndListViewSkip); i++) {
        if (iCase == 0)
            bState = TRUE;
        else if (iCase == 1)
            bState = FALSE;
        else
            bState = !ListView_GetCheckState(hWndListViewSkip, i);
        ListView_SetCheckState(hWndListViewSkip, i, bState);
    }
    PropSheet_Changed(GetParent(hDlg), hDlg);

    return;
}

//--------------------------------------------------
// Main Dialog Proc
//--------------------------------------------------
BOOL CALLBACK DlgSkipProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpnmhdr;
    TCHAR Temp[9];
    LPTSTR  lpszValue, lpszBuffer;
    int position, count, iListItems;
    int iPropertyPage;
    OPENFILENAME opfn;
    TCHAR filepath[MAX_PATH];  // length incl. NULL character

    LVCOLUMN lvSkipCol;                                                         // Make Coloumn struct for ListView
    LVITEM lvSkipItem;                                                          // ListView Item struct
    HWND hWndListViewSkip;
    SKIPLIST* pSkipList;
    SKIPLIST* pListTemp;
    int iSkipCounter = 0;

    switch (message) {
    case WM_INITDIALOG: {
// https://github.com/louiscrocker/Win32-List-View
        bSuppressNotifications = TRUE;
        InitCommonControls();
        PROPSHEETPAGE* s = (PROPSHEETPAGE*)lParam;
        iPropertyPage = (int)s->lParam;

        hWndListViewSkip = GetDlgItem(hDlg, IDC_LIST_SKIP);
        ListView_SetBkColor(hWndListViewSkip, CLR_NONE);
        ListView_SetExtendedListViewStyle(hWndListViewSkip, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
        
        memset(&lvSkipCol, 0, sizeof(lvSkipCol));
        lvSkipCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        RECT  rc;
        GetClientRect(hWndListViewSkip, &rc);
        lvSkipCol.cx = rc.right - rc.left;

        pSkipList = pRegSkipList;
        if (iPropertyPage == PROP_REGS) {
            pSkipList = pRegSkipList;
            lvSkipCol.pszText = TEXT("Excluded registry items"); 
            SendDlgItemMessage(hDlg, IDC_STATIC_SKIP_COMMENT2, WM_SETTEXT, (WPARAM)0, (LPARAM)szSID);
        }
        else if (iPropertyPage == PROP_DIRS) {
            pSkipList = pFileSkipList;
            lvSkipCol.pszText = TEXT("Excluded directories");
        }
        else if (iPropertyPage == PROP_FILES) {
            pSkipList = pFileSkipList;
            lvSkipCol.pszText = TEXT("Excluded files");
        }
        else if (iPropertyPage == PROP_SCANS) {
            pSkipList = pDirScanList;
            lvSkipCol.pszText = TEXT("Scanned directories");
        }
        else if (iPropertyPage == PROP_WLREGS) {
            pSkipList = pRegWhiteList;
            lvSkipCol.pszText = TEXT("Registry whitelist items");
        }
        ListView_InsertColumn(hWndListViewSkip, 0, &lvSkipCol);

        lvSkipCol.cx = 0;
        lvSkipCol.pszText = TEXT("Position");
        ListView_InsertColumn(hWndListViewSkip, 1, &lvSkipCol);

        memset(&lvSkipItem, 0, sizeof(lvSkipItem));                             
        lvSkipItem.mask = LVIF_TEXT;                                            
        lvSkipItem.cchTextMax = EXTDIRLEN;                                      
        lvSkipItem.iSubItem = 0;                                                
        
        count = 0;
        for (int i = 0; (NULL != pSkipList[i].lpSkipString); i++) {
            if (((TRUE != pSkipList[i].bAutomaticallyAdded) || fShowSIDFilterRules) && (TRUE != pSkipList[i].bDeleted)) {
                if ((pSkipList[i].bFile && (iPropertyPage == PROP_FILES)) || 
                    (!pSkipList[i].bFile && (iPropertyPage != PROP_FILES))) {
                    lvSkipItem.iItem = 99999;                    
                    lvSkipItem.iSubItem = 0;
                    lvSkipItem.pszText = pSkipList[i].lpSkipString;
                    count = ListView_InsertItem(hWndListViewSkip, &lvSkipItem);
                    ListView_SetCheckState(hWndListViewSkip, count, pSkipList[i].bSkipKeyActive);

                    lvSkipItem.iItem = count;
                    _sntprintf(Temp, 9, TEXT("%i"), i);
                    lvSkipItem.iSubItem = 1;
                    lvSkipItem.pszText = Temp;
                    ListView_SetItem(hWndListViewSkip, &lvSkipItem);
                }
            }
        }
        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ADDSKIP), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SAVESKIP2INI), FALSE);
        bSuppressNotifications = FALSE;
        
        return TRUE;
    }

    case WM_NOTIFY: {
        lpnmhdr = (NMHDR FAR*)lParam;
        switch (lpnmhdr->code)
        {
        case PSN_KILLACTIVE:
        {
            //MessageBox(hDlg, TEXT("PSN_KILLACTIVE"), asLangTexts[iszTextAbout].lpszText, MB_OK);
            break;
        }
        case PSN_APPLY:
        {
            iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
            pSkipList = pRegSkipList;
            if (iPropertyPage == PROP_REGS)
                pSkipList = pRegSkipList;
            else if (iPropertyPage == PROP_DIRS)
                pSkipList = pFileSkipList;
            else if (iPropertyPage == PROP_FILES)
                pSkipList = pFileSkipList;
            else if (iPropertyPage == PROP_SCANS)
                pSkipList = pDirScanList;
            else if (iPropertyPage == PROP_WLREGS)
                pSkipList = pRegWhiteList;
            hWndListViewSkip = GetDlgItem(hDlg, IDC_LIST_SKIP);

            iListItems = ListView_GetItemCount(hWndListViewSkip);
            for (count = 0; (NULL != pSkipList[count].lpSkipString); count++) {
                if ((pSkipList[count].bFile && (iPropertyPage == PROP_FILES)) || 
                    (!pSkipList[count].bFile && (iPropertyPage != PROP_FILES)))
                    pSkipList[count].bDeleted = TRUE;
            }
            _tcscpy(lpszExtDir, lpszEmpty);
            for (int i = 0; (iListItems > i); i++) {
                ListView_GetItemText(hWndListViewSkip, i, 1, Temp, 9);
                Temp[8] = (TCHAR)'\0';
                position = _tstoi(Temp);
                if (position != -1) {
                    pSkipList[position].bSkipKeyActive = ListView_GetCheckState(hWndListViewSkip, i);
                    pSkipList[position].bDeleted = FALSE;
                    if ((iPropertyPage == PROP_SCANS) && (ListView_GetCheckState(hWndListViewSkip, i))) {
                        if (pSkipList[position].lpSkipString != NULL) {
                            if (pSkipList[position].lpSkipString != NULL) {
                                if (_tcslen(lpszExtDir) == 0)
                                    _tcscpy(lpszExtDir, pSkipList[position].lpSkipString);
                                else {
                                    _tcscat(lpszExtDir, TEXT(";"));
                                    _tcscat(lpszExtDir, pSkipList[position].lpSkipString);
                                }
                            }
                        }
                    }
                }
                else {
                    pSkipList[count].lpSkipString = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
                    ListView_GetItemText(hWndListViewSkip, i, 0, pSkipList[count].lpSkipString, EXTDIRLEN);
                    pSkipList[count].bSkipKeyActive = ListView_GetCheckState(hWndListViewSkip, i);
                    pSkipList[count].bAutomaticallyAdded = FALSE;
                    pSkipList[count].bAdhoc = FALSE;
                    pSkipList[count].bDeleted = FALSE;
                    pSkipList[count].bFile = FALSE;
                    if (iPropertyPage == PROP_FILES)
                        pSkipList[count].bFile = TRUE;
                    
                    if ((iPropertyPage == PROP_SCANS) && (ListView_GetCheckState(hWndListViewSkip, i))) {
                        if (pSkipList[count].lpSkipString != NULL) {
                            if (_tcslen(lpszExtDir) == 0)
                                _tcscpy(lpszExtDir, pSkipList[count].lpSkipString);
                            else {
                                _tcscat(lpszExtDir, TEXT(";"));
                                _tcscat(lpszExtDir, pSkipList[count].lpSkipString);
                            }
                        }
                    }
                    memset(&lvSkipItem, 0, sizeof(lvSkipItem));                            
                    lvSkipItem.mask = LVIF_TEXT;                                           
                    lvSkipItem.cchTextMax = EXTDIRLEN;                                     
                    lvSkipItem.iItem = i;
                    _sntprintf(Temp, 9, TEXT("%i"), count);
                    lvSkipItem.iSubItem = 1;
                    lvSkipItem.pszText = Temp;
                    ListView_SetItem(hWndListViewSkip, &lvSkipItem);
                    
                    count++;
                    iSkipCounter = count + 1;
                    pListTemp = MYREALLOC0(iSkipCounter * sizeof(SKIPLIST), pSkipList);
                    if (pListTemp != NULL)
                        pSkipList = pListTemp;
                    if (iPropertyPage == PROP_REGS)
                        pRegSkipList = pSkipList;
                    else if (iPropertyPage == PROP_WLREGS)
                        pRegWhiteList = pSkipList;
                    else if (iPropertyPage == PROP_SCANS)
                        pDirScanList = pSkipList;
                    else
                        pFileSkipList = pSkipList;
                    pSkipList[iSkipCounter - 1].lpSkipString = NULL;  // saftey NULL pointer

                }
                if (iPropertyPage == PROP_REGS)
                    bRegSkipAdded = TRUE;
                else if (iPropertyPage == PROP_SCANS)
                    bDirScanAdded = TRUE;
                else if (iPropertyPage == PROP_WLREGS)
                    bRegWhitelistAdded = TRUE;
                else
                    bFileSkipAdded = TRUE;
            }
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SAVESKIP2INI), TRUE);
            //MessageBox(hDlg, TEXT("PSN_KILLACTIVE"), asLangTexts[iszTextAbout].lpszText, MB_OK);
            if (iPropertyPage == PROP_SCANS) {
                SetDlgItemText(hMainWnd, IDC_EDITDIR, lpszExtDir);
                if (_tcslen(lpszExtDir) == 0)
                    SendMessage(GetDlgItem(hMainWnd, IDC_CHECKDIR), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
            }
            break;
        }
        case LVN_ITEMCHANGED:
        {
            if (bSuppressNotifications)
                break;
            LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
            if (((BOOL)(((pnmv->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1)) != ((BOOL)(((pnmv->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1))) {
                PropSheet_Changed(GetParent(hDlg), hDlg);
            }
            break;
        }
        default:
            break;
        }
        break;
    }

    case WM_CONTEXTMENU: {
        //return DlgPropSheetProc(hDlg, message, wParam, lParam);
        if (DoContextMenu(hDlg, wParam, lParam))
            return FALSE;
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BROWSE_FILESKIP: {
            iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
            if (iPropertyPage == PROP_FILES) {
                ZeroMemory(filepath, sizeof(filepath));
                ZeroMemory(&opfn, sizeof(opfn));
                opfn.lStructSize = sizeof(opfn);
                opfn.hwndOwner = hMainWnd;
                opfn.lpstrFilter = TEXT("All files\0*.*\0\0");
                opfn.lpstrFile = filepath;
                opfn.nMaxFile = MAX_PATH;  // incl. NULL character
                opfn.lpstrInitialDir = TEXT("%SYSTEMDRIVE%");
                opfn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOTESTFILECREATE;

                if (!GetOpenFileName(&opfn)) {
                    return FALSE;
                }
                SetDlgItemText(hDlg, IDC_EDIT_ADDSKIPSTRING, opfn.lpstrFile);
            }
            if ((iPropertyPage == PROP_DIRS) || (iPropertyPage == PROP_SCANS)) {
                LPITEMIDLIST lpidlist;

                BrowseInfo1.hwndOwner = hDlg;
                BrowseInfo1.pszDisplayName = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                //BrowseInfo1.lpszTitle = TEXT("Select:");
                BrowseInfo1.ulFlags = 0;     // 3 lines added in 1.8.2
                BrowseInfo1.lpfn = NULL;
                BrowseInfo1.lParam = 0;
                lpidlist = SHBrowseForFolder(&BrowseInfo1);
                if (NULL != lpidlist) {
                    SHGetPathFromIDList(lpidlist, BrowseInfo1.pszDisplayName);
                    MYFREE(lpidlist);
                }
                SetDlgItemText(hDlg, IDC_EDIT_ADDSKIPSTRING, BrowseInfo1.pszDisplayName);
                MYFREE(BrowseInfo1.pszDisplayName);
            }
            return TRUE;
        }

        case IDC_EDIT_ADDSKIPSTRING:
            lpszValue = MYALLOC0(MAX_PATH * sizeof(TCHAR));                    // EXTDIRLEN due to IDC_EDITDIR
            GetDlgItemText(hDlg, IDC_EDIT_ADDSKIPSTRING, lpszValue, MAX_PATH);
            if (_tcslen(lpszValue) > 0)
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ADDSKIP), TRUE);
            else
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ADDSKIP), FALSE);
            MYFREE(lpszValue);
            return TRUE;

        case IDC_BUTTON_ADDSKIP: {
            hWndListViewSkip = GetDlgItem(hDlg, IDC_LIST_SKIP); 
            iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
            int i = ListView_GetItemCount(hWndListViewSkip);
            
            memset(&lvSkipItem, 0, sizeof(lvSkipItem));                          // Reset Item Struct
            lvSkipItem.mask = LVIF_TEXT;                                         // Text Style
            lvSkipItem.cchTextMax = EXTDIRLEN;                                   // Max size of text
            lvSkipItem.iSubItem = 0;                                             // Put in first coluom
            lvSkipItem.iItem = i;                                                // choose item  
            lpszValue = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));                     // EXTDIRLEN due to IDC_EDITDIR
            BOOL bNewEntry = FALSE;
            if (lpszValue != NULL)
                bNewEntry = (GetDlgItemText(hDlg, IDC_EDIT_ADDSKIPSTRING, lpszValue, EXTDIRLEN) != 0 ? TRUE : FALSE);
            lpszValue[EXTDIRLEN - 1] = (TCHAR)'\0';  // safety NULL char
            if ((iPropertyPage == PROP_REGS) || (iPropertyPage == PROP_WLREGS)) {
                bNewEntry = FALSE;
                if (lpszValue == _tcsstr(lpszValue, TEXT("HK"))) {
                    bNewEntry = TRUE;
                    LPTSTR pos = _tcschr(lpszValue, _T('\\'));
                    if (pos != NULL)
                        _tcscpy(pos++, TEXT("\0"));
                    if (_tcscmp(lpszValue, TEXT("HKEY_LOCAL_MACHINE")) == 0) {
                        _tcscpy(lpszValue, TEXT("HKLM"));
                        if (pos != NULL) {
                            _tcscat(lpszValue, TEXT("\\"));
                            _tcscat(lpszValue, pos);
                        }
                    }
                    else if (_tcscmp(lpszValue, TEXT("HKEY_USERS")) == 0) {
                        _tcscpy(lpszValue, TEXT("HKU"));
                        if (pos != NULL) {
                            _tcscat(lpszValue, TEXT("\\"));
                            _tcscat(lpszValue, pos);
                        }
                    }
                    else if (_tcscmp(lpszValue, TEXT("HKEY_CURRENT_USER")) == 0) {
                        _tcscpy(lpszValue, TEXT("HKCU"));
                        if (pos != NULL) {
                            _tcscat(lpszValue, TEXT("\\"));
                            _tcscat(lpszValue, pos);
                        }
                    }
                    else if ((_tcscmp(lpszValue, TEXT("HKLM")) != 0) &&
                             (_tcscmp(lpszValue, TEXT("HKU")) != 0) &&
                             (_tcscmp(lpszValue, TEXT("HKCU")) != 0)) 
                        bNewEntry = FALSE;
                    else if (pos != NULL)
                        _tcsncpy(--pos, _T("\\"), 1);
                }
            }
            else {
                lpszBuffer = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                ExpandEnvironmentStrings(lpszValue, lpszBuffer, MAX_PATH);
                _tcscpy(lpszValue, lpszBuffer);
                MYFREE(lpszBuffer);
                
                if (iPropertyPage == PROP_SCANS) {
                }
                else if (iPropertyPage == PROP_FILES) {
                }
                else if (iPropertyPage == PROP_DIRS) {
                }
            }
            if (bNewEntry) {
                lpszBuffer = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                if (lpszBuffer != NULL) {
                    for (int j = 0; j < i; j++) {
                        ListView_GetItemText(hWndListViewSkip, j, 0, lpszBuffer, MAX_PATH);
                        ListView_SetItemState(hWndListViewSkip, j, !LVIS_DROPHILITED, LVIS_DROPHILITED);
                        if (_tcscmp(lpszBuffer, lpszValue) == 0) {
                            ListView_SetCheckState(hWndListViewSkip, j, TRUE);
                            ListView_EnsureVisible(hWndListViewSkip, j, FALSE);
                            ListView_SetItemState(hWndListViewSkip, j, LVIS_DROPHILITED, LVIS_DROPHILITED);
                            bNewEntry = FALSE;
                            break;
                        }
                    }
                    MYFREE(lpszBuffer);
                }
                if (bNewEntry) {
                    lvSkipItem.pszText = lpszValue;
                    ListView_InsertItem(hWndListViewSkip, &lvSkipItem);
                    ListView_SetCheckState(hWndListViewSkip, i, TRUE);

                    lvSkipItem.iSubItem = 1;
                    lvSkipItem.pszText = TEXT("-1");
                    ListView_SetItem(hWndListViewSkip, &lvSkipItem);
                    ListView_EnsureVisible(hWndListViewSkip, i, FALSE);
                    ListView_SetItemState(hWndListViewSkip, i, LVIS_DROPHILITED, LVIS_DROPHILITED);
                }
            }
            else {
                MYFREE(lpszValue);
            }
            SetDlgItemText(hDlg, IDC_EDIT_ADDSKIPSTRING, lpszEmpty);
            return TRUE;
        }

        case IDC_BUTTON_SAVESKIP2INI: {
            iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
            LPTSTR lpszIniSection;
            if (iPropertyPage == PROP_REGS) {
                pSkipList = pRegSkipList;
                lpszIniSection = lpszIniSkipRegKey;
            }
            else if (iPropertyPage == PROP_SCANS) {
                pSkipList = pDirScanList;
                lpszIniSection = lpszIniScanDir;
            }
            else if (iPropertyPage == PROP_DIRS) {
                pSkipList = pFileSkipList;
                lpszIniSection = lpszIniSkipDir;
            }
            else if (iPropertyPage == PROP_WLREGS) {
                pSkipList = pRegWhiteList;
                lpszIniSection = lpszIniWhitelistRegKey;
            }
            else {
                pSkipList = pFileSkipList;
                lpszIniSection = lpszIniSkipFile;
            }
            WritePrivateProfileSection(lpszIniSection, lpszEmpty, lpszRegshotIni);
            for (int i = 0; (NULL != pSkipList[i].lpSkipString); i++) {
                if ((TRUE != pSkipList[i].bAutomaticallyAdded) && (TRUE != pSkipList[i].bDeleted)) {
                    if ((pSkipList[i].bFile && (iPropertyPage == PROP_FILES)) || 
                        (!pSkipList[i].bFile && (iPropertyPage != PROP_FILES))) {
                        lpszBuffer = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                        _tcscpy(lpszBuffer, pSkipList[i].lpSkipString);
                        if ((iPropertyPage != PROP_REGS) && (iPropertyPage != PROP_COMMON)) {
                            if (!PathUnExpandEnvStrings(pSkipList[i].lpSkipString, lpszBuffer, MAX_PATH)) {
                                _tcscpy(lpszBuffer, pSkipList[i].lpSkipString);
                            }
                        }
                        WritePrivateProfileString(lpszIniSection,
                            lpszBuffer,
                            (((pSkipList[i].bSkipKeyActive) && (iPropertyPage != PROP_WLREGS)) ? TEXT("1") : TEXT("0")),
                            lpszRegshotIni);

                        MYFREE(lpszBuffer);
                    }
                }
            }
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SAVESKIP2INI), FALSE);

            return TRUE;
        }
        }

        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
            case ID_CONTEXTMENU_DELETE: {
                hWndListViewSkip = GetDlgItem(hDlg, IDC_LIST_SKIP);
                for (int i = 0; i < ListView_GetItemCount(hWndListViewSkip); i++) {
                    if (ListView_GetItemState(hWndListViewSkip, i, LVIS_SELECTED))
                        ListView_DeleteItem(hWndListViewSkip, i);
                }
                PropSheet_Changed(GetParent(hDlg), hDlg);
                break;
            }

            case ID_CONTEXTMENU_CHECKALL: {
                SetCheckStates(hDlg, 0);
                break;
            }

            case ID_CONTEXTMENU_UNCHECKALL: {
                SetCheckStates(hDlg, 1);
                break;
            }

            case ID_CONTEXTMENU_INVERTCHECKS: {
                SetCheckStates(hDlg, 2);
                break;
            }
        }
    }
    return (FALSE);
}

//BOOL CALLBACK DlgPropSheetProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message) {
//
//        case WM_CONTEXTMENU: {
//            if (DoContextMenu(hDlg, wParam, lParam))
//                return FALSE;
//            break;
//        }
//    }
//    return(FALSE);
//}

BOOL DoContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    HWND  hwndList = (HWND)wParam;
    HMENU hMenuLoad, hMenu;
    if (hwndList != GetDlgItem(hWnd, IDC_LIST_SKIP))
        return FALSE;
    
    hMenuLoad = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_CONTEXT_SKIPLIST));
    hMenu = GetSubMenu(hMenuLoad, 0);

    if (ListView_GetSelectedCount(hwndList) == 0)
        EnableMenuItem(hMenu, ID_CONTEXTMENU_DELETE, MF_BYCOMMAND | MF_GRAYED);

    TrackPopupMenu(hMenu,
        TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        LOWORD(lParam),
        HIWORD(lParam),
        0,
        hWnd,
        NULL);

    DestroyMenu(hMenuLoad);

    return TRUE;
}

//--------------------------------------------------
// Common Dialog Proc
//--------------------------------------------------
BOOL CALLBACK DlgOptionsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR     lpnmhdr;
    int iIndex;
    OPENFILENAME opfn;
    TCHAR filepath[MAX_PATH+1];

    switch (message) {
    case WM_INITDIALOG: {
        PROPSHEETPAGE* s = (PROPSHEETPAGE*)lParam;
        iIndex = (int)s->lParam;

        if (iIndex == PROP_COMMON) {
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONQUIT), BM_SETCHECK, fStoreOnQuit, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONLYUNCOMP), BM_SETCHECK, fStoreOnlyUncompared, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_LONGREGKEYS), BM_SETCHECK, fUseLongRegHead, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SEPARATEOBJECTS), BM_SETCHECK, fOutSeparateObjs, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_DONTDISPLAYINFO), BM_SETCHECK, fDontDisplayInfoAfterShot, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_DONTDISPLAYRESULT), BM_SETCHECK, fDontDisplayInfoAfterComparison, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SAVEINI), BM_SETCHECK, fSaveSettingsOnExit, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_NOFILTERS), BM_SETCHECK, fNoFiltersWhenLoading, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SIDFILTERS), BM_SETCHECK, fShowSIDFilterRules, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_ENVIRONMENT), BM_SETCHECK, fLogEnvironmentStrings, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_UNLORDER), BM_SETCHECK, fLogUNLOrder, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_READONLY), BM_SETCHECK, fDeleteReadOnly, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_NONEMPTY), BM_SETCHECK, fDeleteDirNotEmpty, (LPARAM)0);
            SetDlgItemText(hDlg, IDC_COMMON_EDIT_EDITOR, lpszEditor);
            LPTSTR strExpandLevels = MYALLOC0(4 * sizeof(TCHAR));
            if (strExpandLevels != NULL) {
                _sntprintf(strExpandLevels, 3, TEXT("%i"), nExpandLevels);
                strExpandLevels[3] = _T('\0');
                SetDlgItemText(hDlg, IDC_EDIT_EXPANDLEVELS, strExpandLevels);
                MYFREE(strExpandLevels);
            }
            SendMessage(GetDlgItem(hDlg, IDC_SPIN_EXPANDLEVELS), UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELPARAM(9,0));
            LPTSTR strMaxNodes = MYALLOC0(5 * sizeof(TCHAR));
            if (strMaxNodes != NULL) {
                _sntprintf(strMaxNodes, 4, TEXT("%i"), nMaxNodes);
                strMaxNodes[4] = _T('\0');
                SetDlgItemText(hDlg, IDC_EDIT_MAXNODES, strMaxNodes);
                MYFREE(strMaxNodes);
            }
            SendMessage(GetDlgItem(hDlg, IDC_SPIN_MAXNODES), UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELPARAM(9999,0));
            
            LPTSTR strMaxLines = MYALLOC0(6 * sizeof(TCHAR));
            if (strMaxLines != NULL) {
                _sntprintf(strMaxLines, 5, TEXT("%i"), nMaxLines);
                strMaxLines[5] = _T('\0');
                SetDlgItemText(hDlg, IDC_EDIT_MAXLINES, strMaxLines);
                MYFREE(strMaxLines);
            }
            SendMessage(GetDlgItem(hDlg, IDC_SPIN_MAXLINES), UDM_SETRANGE32, (WPARAM)5000, (LPARAM)99999);
            FillComboboxWithLanguagesFromIni(hDlg);
        }
        else if (iIndex == PROP_COMMON_2) {
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSINS), BM_SETCHECK, fISSInstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSDEL), BM_SETCHECK, fISSDeinstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSOUTPUTFOLDER), BM_SETCHECK, fUseDifferentISSOutputFolder, (LPARAM)0);
            SetDlgItemText(hDlg, IDC_COMMON_EDIT_ISSOUTPUT, lpszISSOutputFolder);
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_EDIT_ISSOUTPUT), (fUseDifferentISSOutputFolder ? TRUE : FALSE));
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_BROWSEISSOUTPUT), (fUseDifferentISSOutputFolder ? TRUE : FALSE));
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSEDITOR), BM_SETCHECK, fUseDifferentISSEditor, (LPARAM)0);
            SetDlgItemText(hDlg, IDC_COMMON_EDIT_ISSEDITOR, lpszISSEditor);
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_EDIT_ISSEDITOR), (fUseDifferentISSEditor ? TRUE : FALSE));
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_BROWSEISSEDITOR), (fUseDifferentISSEditor ? TRUE : FALSE));
            
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIINS), BM_SETCHECK, fNSIInstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIDEL), BM_SETCHECK, fNSIDeinstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIOUTPUTFOLDER), BM_SETCHECK, fUseDifferentNSIOutputFolder, (LPARAM)0);
            SetDlgItemText(hDlg, IDC_COMMON_EDIT_NSIOUTPUT, lpszNSIOutputFolder);
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_EDIT_NSIOUTPUT), (fUseDifferentNSIOutputFolder ? TRUE : FALSE));
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_BROWSENSIOUTPUT), (fUseDifferentNSIOutputFolder ? TRUE : FALSE));
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIEDITOR), BM_SETCHECK, fUseDifferentNSIEditor, (LPARAM)0);
            SetDlgItemText(hDlg, IDC_COMMON_EDIT_NSIEDITOR, lpszNSIEditor);
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_EDIT_NSIEDITOR), (fUseDifferentNSIEditor ? TRUE : FALSE));
            EnableWindow(GetDlgItem(hDlg, IDC_COMMON_BROWSENSIEDITOR), (fUseDifferentNSIEditor ? TRUE : FALSE));
            
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGINS), BM_SETCHECK, fRegIns, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGDEL), BM_SETCHECK, fRegDel, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_REGEDIT4), BM_SETCHECK, !fREG5, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_REGEDIT5), BM_SETCHECK, fREG5, (LPARAM)0);
            
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_SETCHECK, fOutputTXTfile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_AU3), BM_SETCHECK, fOutputAU3file, (LPARAM)0);
        }
        return TRUE;
    }

    case WM_NOTIFY: {
        lpnmhdr = (NMHDR FAR*)lParam;
        switch (lpnmhdr->code) 
        {
            case PSN_KILLACTIVE:
            {
//              MessageBox(hDlg, TEXT("PSN_KILLACTIVE"), asLangTexts[iszTextAbout].lpszText, MB_OK);
                break;
            }
            case PSN_APPLY:
            {
                iIndex = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
                
                if (iIndex == PROP_COMMON) {
                    fStoreOnQuit = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONQUIT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fStoreOnlyUncompared = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_STOREONLYUNCOMP), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fUseLongRegHead = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_LONGREGKEYS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fOutSeparateObjs = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SEPARATEOBJECTS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fDontDisplayInfoAfterShot = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_DONTDISPLAYINFO), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fDontDisplayInfoAfterComparison = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_DONTDISPLAYRESULT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fSaveSettingsOnExit = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SAVEINI), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fNoFiltersWhenLoading = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_NOFILTERS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fShowSIDFilterRules = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_SIDFILTERS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fLogEnvironmentStrings = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_ENVIRONMENT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fLogUNLOrder = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_UNLORDER), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fDeleteReadOnly = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_READONLY), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fDeleteDirNotEmpty = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPT_NONEMPTY), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    GetDlgItemText(hDlg, IDC_COMMON_EDIT_EDITOR, lpszEditor, MAX_PATH);  // length incl. NULL character
                    LPTSTR  lpszValue = MYALLOC0(8 * sizeof(TCHAR));
                    if (lpszValue != NULL) {
                        GetDlgItemText(hDlg, IDC_EDIT_EXPANDLEVELS, lpszValue, 4);
                        lpszValue[4] = (TCHAR)'\0';
                        nExpandLevels = _tstoi(lpszValue);
                    }
                    if (lpszValue != NULL) {
                        GetDlgItemText(hDlg, IDC_EDIT_MAXNODES, lpszValue, 5);
                        lpszValue[5] = (TCHAR)'\0';
                        nMaxNodes = _tstoi(lpszValue);
                        if ((0 >= nMaxNodes) || (nMaxNodes > 9999)) {
                            nMaxNodes = 9999;
                        }
                    }
                    if (lpszValue != NULL) {
                        GetDlgItemText(hDlg, IDC_EDIT_MAXLINES, lpszValue, 7);
                        lpszValue[7] = (TCHAR)'\0';
                        nMaxLines = _tstoi(lpszValue);
                        if ((0 >= nMaxLines) || (nMaxLines > 99999)) {
                            nMaxNodes = 99999;
                        }
                    }
                    MYFREE(lpszValue);


                    WritePrivateProfileString(lpszIniSetup, lpszIniLanguage, lpszLanguage, lpszRegshotIni);
                    
                    //LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
                    //GetDlgItemText(hMainWnd, IDC_EDITPATH, lpszOutputPath, MAX_PATH);  // length incl. NULL character
                    //if ((lpszOutputPath != NULL) && (lpszTempDirBuffer != NULL)) {
                    //    if (PathUnExpandEnvStrings(lpszOutputPath, lpszTempDirBuffer, MAX_PATH)) {
                    //        _tcscpy(lpszOutputPath, lpszTempDirBuffer);
                    //    }
                    //}
                    //MYFREE(lpszTempDirBuffer);
                    //if (lpszOutputPath != NULL)
                    //    SetDlgItemText(hMainWnd, IDC_EDITPATH, lpszOutputPath);

                }
                else if (iIndex == PROP_COMMON_2) {
                    fISSInstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fISSDeinstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fUseDifferentISSOutputFolder = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSOUTPUTFOLDER), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    GetDlgItemText(hDlg, IDC_COMMON_EDIT_ISSOUTPUT, lpszISSOutputFolder, MAX_PATH);  // length incl. NULL character
                    fUseDifferentISSEditor = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSEDITOR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    GetDlgItemText(hDlg, IDC_COMMON_EDIT_ISSEDITOR, lpszISSEditor, MAX_PATH);  // length incl. NULL character

                    fNSIInstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fNSIDeinstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fUseDifferentNSIOutputFolder = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIOUTPUTFOLDER), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    GetDlgItemText(hDlg, IDC_COMMON_EDIT_NSIOUTPUT, lpszNSIOutputFolder, MAX_PATH);  // length incl. NULL character
                    fUseDifferentNSIEditor = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIEDITOR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    GetDlgItemText(hDlg, IDC_COMMON_EDIT_NSIEDITOR, lpszNSIEditor, MAX_PATH);  // length incl. NULL character

                    fRegDel = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fRegIns = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fREG5 = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_REGEDIT5), BM_GETCHECK, (WPARAM)0, (LPARAM)0);

                    fOutputTXTfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    fOutputAU3file = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_AU3), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                }
                break;
            }
        // Process other property sheet notifications here.
            default:
                break;
        }
        break;
    }

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hDlg), hDlg);   // TODO: richtige Prüfung
        switch (LOWORD(wParam)) {
            case IDC_COMBOLANGUAGE:
                if (CBN_SELCHANGE == HIWORD(wParam)) {  // Only react when user selected something
                    SetTextsToDefaultLanguage();
                    SetTextsToSelectedLanguage(hDlg, TRUE);
                    UI_EnableMainButtons();
                    return(TRUE);
                }
                break;

            case IDC_COMMON_BROWSEEDITOR:
            case IDC_COMMON_BROWSEISSEDITOR:
            case IDC_COMMON_BROWSENSIEDITOR:
                ZeroMemory(filepath, sizeof(filepath));
                ZeroMemory(&opfn, sizeof(opfn));
                opfn.lStructSize = sizeof(opfn);
                opfn.hwndOwner = hMainWnd;
                opfn.lpstrFilter = TEXT("Executable files\0*.exe\0\0");
                opfn.lpstrFile = filepath;
                opfn.nMaxFile = MAX_PATH;  // incl. NULL character
                opfn.lpstrInitialDir = TEXT("%SYSTEMDRIVE%");
                opfn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOTESTFILECREATE;

                if (!GetOpenFileName(&opfn)) {
                    return FALSE;
                }
                opfn.lpstrFile[MAX_PATH] = (TCHAR)'\0';
                if (LOWORD(wParam) == IDC_COMMON_BROWSEEDITOR)
                    SetDlgItemText(hDlg, IDC_COMMON_EDIT_EDITOR, opfn.lpstrFile);
                else if (LOWORD(wParam) == IDC_COMMON_BROWSEISSEDITOR)
                    SetDlgItemText(hDlg, IDC_COMMON_EDIT_ISSEDITOR, opfn.lpstrFile);
                else if (LOWORD(wParam) == IDC_COMMON_BROWSENSIEDITOR)
                    SetDlgItemText(hDlg, IDC_COMMON_EDIT_NSIEDITOR, opfn.lpstrFile);

                break;
            
            case IDC_COMMON_BROWSENSIOUTPUT:
            case IDC_COMMON_BROWSEISSOUTPUT: {
                LPITEMIDLIST lpidlist;
                int nIDDEdit = 0;
                if (LOWORD(wParam) == IDC_COMMON_BROWSENSIOUTPUT) {
                    nIDDEdit = IDC_COMMON_EDIT_NSIOUTPUT;
                }
                else if (LOWORD(wParam) == IDC_COMMON_BROWSEISSOUTPUT) {
                    nIDDEdit = IDC_COMMON_EDIT_ISSOUTPUT;
                }
                
                BrowseInfo1.hwndOwner = hDlg;
                BrowseInfo1.pszDisplayName = MYALLOC0(MAX_PATH * sizeof(TCHAR));
                BrowseInfo1.ulFlags |= 0x0040;
                BrowseInfo1.lpfn = SelectBrowseFolder;
                BrowseInfo1.lParam = (LPARAM)BrowseInfo1.pszDisplayName;

                LPTSTR lpszTempDirBuffer = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
                if ((BrowseInfo1.pszDisplayName != NULL) && (lpszTempDirBuffer != NULL)) {
                    GetDlgItemText(hDlg, nIDDEdit, BrowseInfo1.pszDisplayName, MAX_PATH);
                    ExpandEnvironmentStrings(BrowseInfo1.pszDisplayName, lpszTempDirBuffer, MAX_PATH);
                    _tcscpy(BrowseInfo1.pszDisplayName, lpszTempDirBuffer);
                    MYFREE(lpszTempDirBuffer);
                }

                lpidlist = SHBrowseForFolder(&BrowseInfo1);
                if ((BrowseInfo1.pszDisplayName != NULL) && (NULL != lpidlist)) {
                    SHGetPathFromIDList(lpidlist, BrowseInfo1.pszDisplayName);
                    SetDlgItemText(hDlg, nIDDEdit, BrowseInfo1.pszDisplayName);
                    MYFREE(lpidlist);
                    MYFREE(BrowseInfo1.pszDisplayName);
                }

                break;
            }
            
            case IDC_CHECK_NSIOUTPUTFOLDER:
            case IDC_CHECK_NSIEDITOR:
            case IDC_CHECK_ISSOUTPUTFOLDER:
            case IDC_CHECK_ISSEDITOR: {
                int nIDDEdit = 0;
                int nIDDBrowse = 0;
                BOOL bEnabled = (BOOL)SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                
                if (LOWORD(wParam) == IDC_CHECK_NSIOUTPUTFOLDER) {
                    nIDDEdit = IDC_COMMON_EDIT_NSIOUTPUT;
                    nIDDBrowse = IDC_COMMON_BROWSENSIOUTPUT;
                }
                else if (LOWORD(wParam) == IDC_CHECK_NSIEDITOR) {
                    nIDDEdit = IDC_COMMON_EDIT_NSIEDITOR;
                    nIDDBrowse = IDC_COMMON_BROWSENSIEDITOR;
                }
                else if (LOWORD(wParam) == IDC_CHECK_ISSOUTPUTFOLDER) {
                    nIDDEdit = IDC_COMMON_EDIT_ISSOUTPUT;
                    nIDDBrowse = IDC_COMMON_BROWSEISSOUTPUT;
                }
                else if (LOWORD(wParam) == IDC_CHECK_ISSEDITOR) {
                    nIDDEdit = IDC_COMMON_EDIT_ISSEDITOR;
                    nIDDBrowse = IDC_COMMON_BROWSEISSEDITOR;
                }
                EnableWindow(GetDlgItem(hDlg, nIDDEdit), bEnabled);
                EnableWindow(GetDlgItem(hDlg, nIDDBrowse), bEnabled);

                break;
            }
        }
    }
    return(FALSE);
}


