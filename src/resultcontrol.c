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

//--------------------------------------------------
// Initialize TreeView for checking the comparison 
// result
//--------------------------------------------------
VOID DoTVPropertySheet(HWND hwndOwner, int iSelectedPage)
{
    PROPSHEETPAGE psp[PROP_TVDIRS + 1];
    PROPSHEETHEADER psh;

    psp[PROP_TVREGS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_TVREGS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_TVREGS].hInstance = hInst;
    psp[PROP_TVREGS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_TV_REGS);
    psp[PROP_TVREGS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
    psp[PROP_TVREGS].pfnDlgProc = (DLGPROC)DlgSkipTVProc;
    psp[PROP_TVREGS].pszTitle = TEXT("Registry comparison result"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_TVREGS].lParam = PROP_TVREGS;
    psp[PROP_TVREGS].pfnCallback = NULL;

    psp[PROP_TVDIRS].dwSize = sizeof(PROPSHEETPAGE);
    psp[PROP_TVDIRS].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[PROP_TVDIRS].hInstance = hInst;
    psp[PROP_TVDIRS].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_TV_REGS);
    psp[PROP_TVDIRS].pszIcon = NULL;
    psp[PROP_TVDIRS].pfnDlgProc = (DLGPROC)DlgSkipTVProc;
    psp[PROP_TVDIRS].pszTitle = TEXT("Directory comparison result"); // MAKEINTRESOURCE(IDS_BORDER);
    psp[PROP_TVDIRS].lParam = PROP_TVDIRS;
    psp[PROP_TVDIRS].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
    psh.hwndParent = hwndOwner;
    psh.hInstance = hInst;
    psh.pszIcon = MAKEINTRESOURCE(IDI_MAINICON);
    psh.pszCaption = TEXT("Comparison result (uncheck irrelevant parts)");
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = iSelectedPage;
    psh.ppsp = (LPCPROPSHEETPAGE)&psp;
    psh.pfnCallback = NULL;

    bNoOutput = fSuppressLogs;
    PropertySheet(&psh);

    return;
}

//--------------------------------------------------
// Dialog Procedure of comparison result check 
// treeview
//--------------------------------------------------
BOOL CALLBACK DlgSkipTVProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwndTV; 
    TVITEM pitem;
    LPNMHDR lpnmhdr;
    LPNMTREEVIEW pnmtv;
    int iPropertyPage;

    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_CHECK_SUPPRESS:
                    bNoOutput = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_OPENEDITOR:
                    fOpenEditor = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPENEDITOR), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_OPENEDITOR), BM_SETCHECK, fOpenEditor, (LPARAM)0);
                    break;
                case IDC_CHECK_UNL:
                    fOutputUNLfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    //SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_UNL), BM_SETCHECK, 
                    //    (WPARAM)(SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);
                    break;
                case IDC_CHECK_BAT:
                    fOutputBATfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    //SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_BAT), BM_SETCHECK,
                    //    (WPARAM)(SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);
                    //break;
                case IDC_CHECK_TXT:
                    fOutputTXTfile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_ISSINS:
                    fISSInstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_ISSDEL:
                    fISSDeinstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_NSIINS:
                    fNSIInstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_NSIDEL:
                    fNSIDeinstallFile = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_REGINS:
                    fRegIns = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGINS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                case IDC_CHECK_REGDEL:
                    fRegDel = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGDEL), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                default:
                    break;
            }
            break;

        case WM_INITDIALOG: {
            bSuppressNotifications = TRUE;

            InitCommonControls();

            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSINS), BM_SETCHECK, fISSInstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSDEL), BM_SETCHECK, fISSDeinstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIINS), BM_SETCHECK, fNSIInstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIDEL), BM_SETCHECK, fNSIDeinstallFile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPENEDITOR), BM_SETCHECK, fOpenEditor, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGINS), BM_SETCHECK, fRegIns, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGDEL), BM_SETCHECK, fRegDel, (LPARAM)0);
//            SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, (WPARAM)(SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);    // CMD output
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, fOutputBATfile, (LPARAM)0);
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_SETCHECK, fOutputTXTfile, (LPARAM)0);
//            SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, (WPARAM)(SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);    // UNL output
            SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, fOutputUNLfile, (LPARAM)0);

            PROPSHEETPAGE* s = (PROPSHEETPAGE*)lParam;
            iPropertyPage = (int)s->lParam;
            hwndTV = GetDlgItem(hDlg, IDC_TREE_SKIPS);

            InitTreeViewImageLists(hwndTV);
            InitTreeViewItems(hwndTV, iPropertyPage);

            SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_SETCHECK, bNoOutput, (LPARAM)0);
            bSuppressNotifications = FALSE;
            return TRUE;
        }

        case WM_NOTIFY: {
            lpnmhdr = (NMHDR FAR*)lParam;
            LPPSHNOTIFY lppsn = (LPPSHNOTIFY)lParam;
            hwndTV = GetDlgItem(hDlg, IDC_TREE_SKIPS);
            switch (lpnmhdr->code)
            {
                case PSN_APPLY:
                {
                    iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
                    SearchTVDown(hwndTV, TreeView_GetRoot(hwndTV), iPropertyPage);
                    if (lppsn->lParam == 1) {                    // OK-Button pressed
                        TreeView_DeleteAllItems(hwndTV);
                    }
                    break;
                }

                case PSN_SETACTIVE:
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_SETCHECK, bNoOutput, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSINS), BM_SETCHECK, fISSInstallFile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_ISSDEL), BM_SETCHECK, fISSDeinstallFile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIINS), BM_SETCHECK, fNSIInstallFile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_NSIDEL), BM_SETCHECK, fNSIDeinstallFile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGINS), BM_SETCHECK, fRegIns, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_REGDEL), BM_SETCHECK, fRegDel, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_TXT), BM_SETCHECK, fOutputTXTfile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, fOutputBATfile, (LPARAM)0);
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, fOutputUNLfile, (LPARAM)0);
                    //SendMessage(GetDlgItem(hDlg, IDC_CHECK_UNL), BM_SETCHECK, (WPARAM)(SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_UNL), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);    // UNL output
                    //SendMessage(GetDlgItem(hDlg, IDC_CHECK_BAT), BM_SETCHECK, (WPARAM)(SendMessage(GetDlgItem(hMainWnd, IDC_CHECK_BAT), BM_GETCHECK, (WPARAM)0, (LPARAM)0)), (LPARAM)0);    // UNL output
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_OPENEDITOR), BM_SETCHECK, fOpenEditor, (LPARAM)0);
                    break;

                case PSN_RESET:
                    TreeView_DeleteAllItems(hwndTV);
                    break;

                case PSN_QUERYCANCEL:
                case PSN_KILLACTIVE:
                    hwndTV = GetDlgItem(hDlg, IDC_TREE_SKIPS);
                    break;

                //case TVN_DELETEITEM:
                //    pnmtv = (LPNMTREEVIEW)lParam;
                //    if (pnmtv->itemOld.lParam != NULL)
                //        MYFREE(pnmtv->itemOld.lParam);
                //    break;

                case TVN_ITEMEXPANDED:
                    pnmtv = (LPNMTREEVIEW)lParam;
                    memset(&pitem, 0, sizeof(pitem));
                    pitem.hItem = pnmtv->itemNew.hItem;
                    pitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                    pitem.iImage = (pnmtv->action == TVE_EXPAND ? 1 : 0);
                    pitem.iSelectedImage = pitem.iImage;
                    TreeView_SetItem(hwndTV, &pitem);

                    break;

                case TVN_ITEMCHANGED:
                {
                    if (bSuppressNotifications)
                        break;
                    NMTVITEMCHANGE * pnmt = (NMTVITEMCHANGE *)lParam;
                    HTREEITEM hItem;
                    if (((BOOL)(pnmt->uStateOld >> 12) - 1) != ((BOOL)(pnmt->uStateNew >> 12) - 1)) {
                        hItem = TreeView_GetNextItem(hwndTV, pnmt->hItem, TVGN_CHILD);
                        while (TRUE) {
                            TreeView_SetCheckState(hwndTV, hItem, (BOOL)(pnmt->uStateNew >> 12) - 1);
                            hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_NEXT);

                            if (hItem == NULL)
                                break;
                        }
                        bSuppressNotifications = TRUE;
                        if ((BOOL)(pnmt->uStateNew >> 12) - 1) {
                            hItem = TreeView_GetParent(hwndTV, pnmt->hItem);
                            while (TRUE) {
                                TreeView_SetCheckState(hwndTV, hItem, (BOOL)(pnmt->uStateNew >> 12) - 1);
                                hItem = TreeView_GetParent(hwndTV, hItem);

                                if (hItem == NULL)
                                    break;
                            }
                        }
                        bSuppressNotifications = FALSE;
                        PropSheet_Changed(GetParent(hDlg), hDlg);
                    }
                    break;
                }
            
                default:
                    break;
            }
            break;
        }
    }
    return (FALSE);
}

//--------------------------------------------------
// Search treeview down for unchecked items
//--------------------------------------------------
BOOL SearchTVDown(HWND hwndTV, HTREEITEM hItem, int iPropertyPage)
{
    TVITEM pitem;
    HTREEITEM hItemStart = hItem, hItemChild = NULL;

    if (hItem == NULL)
        return TRUE;

    memset(&pitem, 0, sizeof(pitem));
    pitem.hItem = hItem;
    pitem.mask = TVIF_PARAM;
    TreeView_GetItem(hwndTV, &pitem);
    if ((BOOL)(pitem.lParam))
        return TRUE;

    pitem.lParam = (LPARAM)TRUE;
    TreeView_SetItem(hwndTV, &pitem);

    if (TreeView_GetCheckState(hwndTV, hItem)) {
        hItemChild = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
        if (hItemChild != NULL)
            SearchTVDown(hwndTV, hItemChild, iPropertyPage);
    }
    else {
        BuildSkipString(hwndTV, hItem, iPropertyPage);
    }
    
    hItem = hItemStart;
    while (TRUE) {
        hItem = TreeView_GetNextSibling(hwndTV, hItem);
        if (hItem == NULL)
            break;
        SearchTVDown(hwndTV, hItem, iPropertyPage);
    }

    return TRUE;
}

//--------------------------------------------------
// Build skipstring from unchecked item up to root
//--------------------------------------------------
BOOL BuildSkipString(HWND hwndTV, HTREEITEM hItem, int iPropertyPage)
{
    TVITEM pitem;
//    LPARAM lParam = (LPARAM)NULL;
    SKIPLIST* pSkipList;
    SKIPLIST* pListTemp;

    LPTSTR * lpszNodeTextTV = MYALLOC0(99 * sizeof(LPTSTR));
    int iStringCounter = 0;
    DWORD iSkipCounter = 0;
    DWORD nSkipListSize = 0;

    if (iPropertyPage == PROP_TVREGS) {
        pSkipList = pRegSkipList;
        bRegSkipAdded = TRUE;
    }
    else {
        pSkipList = pFileSkipList;
        bFileSkipAdded = TRUE;
    }

    for (iSkipCounter = 0; (NULL != pSkipList[iSkipCounter].lpSkipString); iSkipCounter++);
    pSkipList[iSkipCounter].lpSkipString = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
    pSkipList[iSkipCounter].bSkipKeyActive = TRUE;
    pSkipList[iSkipCounter].bAutomaticallyAdded = FALSE;
    pSkipList[iSkipCounter].bAdhoc = TRUE;
    pSkipList[iSkipCounter].bDeleted = FALSE;
    pSkipList[iSkipCounter].bFile = FALSE;
    nSkipListSize = iSkipCounter + 2;
    pListTemp = MYREALLOC0(nSkipListSize * sizeof(SKIPLIST), pSkipList);
    if (pListTemp != NULL)
        pSkipList = pListTemp;

    // SkipString bilden
    if (lpszNodeTextTV != NULL) {
        lpszNodeTextTV[iStringCounter] = MYALLOC0(MAX_PATH * sizeof(TCHAR));
        if (lpszNodeTextTV[iStringCounter] != NULL) {
            memset(&pitem, 0, sizeof(pitem));
            pitem.hItem = hItem;
            pitem.mask = TVIF_TEXT;
            pitem.pszText = lpszNodeTextTV[iStringCounter];
            pitem.cchTextMax = MAX_PATH;
            TreeView_GetItem(hwndTV, &pitem);
        }
    }

    while (TRUE) {
        hItem = TreeView_GetParent(hwndTV, hItem);
        if (hItem == NULL)
            break;
        iStringCounter++;

        if (lpszNodeTextTV != NULL) {
            lpszNodeTextTV[iStringCounter] = MYALLOC0(MAX_PATH * sizeof(TCHAR));
            if (lpszNodeTextTV[iStringCounter] != NULL) {
                memset(&pitem, 0, sizeof(pitem));
                pitem.hItem = hItem;
                pitem.mask = TVIF_TEXT;
                pitem.pszText = lpszNodeTextTV[iStringCounter];
                pitem.cchTextMax = MAX_PATH;
                TreeView_GetItem(hwndTV, &pitem);
            }
        }
    }
    
    _tcscpy(pSkipList[iSkipCounter].lpSkipString, TEXT(""));
    for (int i = iStringCounter; i >= 0; i--) {
        if (pSkipList != NULL) {
            if ((pSkipList[iSkipCounter].lpSkipString != NULL) && (lpszNodeTextTV != NULL) && (lpszNodeTextTV[i] != NULL)) {
                _tcscat(pSkipList[iSkipCounter].lpSkipString, lpszNodeTextTV[i]);
            }
            if (lpszNodeTextTV != NULL)
                MYFREE(lpszNodeTextTV[i]);
            if (i > 0)
                _tcscat(pSkipList[iSkipCounter].lpSkipString, TEXT("\\"));
        }
    }

    MYFREE(lpszNodeTextTV);
    if (iPropertyPage == PROP_TVREGS)
        pRegSkipList = pSkipList;
    else
        pFileSkipList = pSkipList;

    return TRUE;
}

//--------------------------------------------------
// Delete filters from earlier comparison check
//--------------------------------------------------
BOOL ResetAdhocFilters()
{
    for (int i = 0; (NULL != pRegSkipList[i].lpSkipString); i++)
    {
        if (pRegSkipList[i].bAdhoc)
            pRegSkipList[i].bDeleted = TRUE;
    }
    for (int i = 0; (NULL != pFileSkipList[i].lpSkipString); i++)
    {
        if (pFileSkipList[i].bAdhoc)
            pFileSkipList[i].bDeleted = TRUE;
    }

    return TRUE;
}

//--------------------------------------------------
// Prepare treeviews with corresponding root items:
// o root keys for registry view
// o scanned directories for filesystem view
//--------------------------------------------------
BOOL InsertRootItems(HWND hwndTV, int iPropertyPage)
{
    TV_INSERTSTRUCT tvinsert;
    HTREEITEM hRootItem;
    TCHAR szRootKeys[3][6] = { TEXT("HKLM\0"), TEXT("HKU\0"), TEXT("HKCU\0") };
//    TCHAR szRootKeysLong[3][20] = { TEXT("HKEY_LOCAL_MACHINE\0"), TEXT("HKEY_USERS\0"), TEXT("HKEY_CURRENT_USER\0") };

    if (iPropertyPage == 0) {
        for (int i = 0; i < _countof(szRootKeys); i++) {
            memset(&tvinsert, 0, sizeof(tvinsert));
            tvinsert.hParent = NULL;
            tvinsert.hInsertAfter = TVI_ROOT;
            tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
//            tvinsert.item.pszText = (fUseLongRegHead ? szRootKeysLong[i]: szRootKeys[i]);
            tvinsert.item.pszText = szRootKeys[i];
            tvinsert.item.iImage = 0;
            tvinsert.item.iSelectedImage = 0;
            hRootItem = TreeView_InsertItem(hwndTV, &tvinsert);
            TreeView_SetCheckState(hwndTV, hRootItem, TRUE);
        }
    }
    else {
        LPTSTR lpszDrive;
        LPTSTR pos = NULL;
        for (int i = 0; ((pDirScanList[i].lpSkipString != NULL) && (pDirScanList[i].bSkipKeyActive) && (!pDirScanList[i].bDeleted)); i++) {
            lpszDrive = pDirScanList[i].lpSkipString;
            pos = _tcsrchr(pDirScanList[i].lpSkipString, _T('\\'));
            if (pDirScanList[i].lpSkipString + _tcslen(pDirScanList[i].lpSkipString) - 1 == pos) {
                lpszDrive = MYALLOC0(_tcslen((pDirScanList[i].lpSkipString) + 1) * sizeof(TCHAR));
                if (lpszDrive != NULL) {
                    _tcscpy(lpszDrive, pDirScanList[i].lpSkipString);
                    lpszDrive[_tcslen(pDirScanList[i].lpSkipString) - 1] = _T('\0');
                }
            }
            memset(&tvinsert, 0, sizeof(tvinsert));
            tvinsert.hInsertAfter = TVI_ROOT; // work as root level
            tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvinsert.item.pszText = lpszDrive;
            tvinsert.item.iImage = 0;
            tvinsert.item.iSelectedImage = 0;
            hRootItem = TreeView_InsertItem(hwndTV, &tvinsert);
            TreeView_SetCheckState(hwndTV, hRootItem, TRUE);
        }
    }

    return TRUE;
}

//--------------------------------------------------
// Search treeview down and create new item 
// if needed
//--------------------------------------------------
HTREEITEM FindOrCreateTreeItem(HWND hwndTV, HTREEITEM hParent, HTREEITEM hItem, UINT nLevel, LPTSTR lpszNodeText, int iPropertyPage, BOOL bKey, LPCOMPRESULTNEW lpCR)
{
    TVITEM pitem;
    TV_INSERTSTRUCT tvinsert;
    LPTSTR lpszNodeTextTV = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
    int iSelectedImage = 0;

    while (TRUE) {
        memset(&pitem, 0, sizeof(pitem));
        pitem.hItem = hItem;
        pitem.mask = TVIF_TEXT;
        pitem.pszText = lpszNodeTextTV;
        pitem.cchTextMax = MAX_PATH;
        TreeView_GetItem(hwndTV, &pitem);
        if ((pitem.pszText == NULL) || _tcscmp(lpszNodeText, pitem.pszText) == 0) {
            break;
        }
        hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_NEXT);
        if (hItem == NULL) {
            memset(&tvinsert, 0, sizeof(tvinsert));
            tvinsert.hParent = hParent;
            tvinsert.hInsertAfter = TVI_SORT;
            tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvinsert.item.pszText = lpszNodeText;
            if (iPropertyPage == PROP_TVREGS) {
                iSelectedImage = (bKey ? 0 : lpCR->nActionType - 1);
                iSelectedImage = (iSelectedImage == 3 ? 5 : iSelectedImage);
                tvinsert.item.iImage = iSelectedImage;
                tvinsert.item.iSelectedImage = iSelectedImage;
            }
            else {
                iSelectedImage = (bKey ? 0 : lpCR->nActionType - 7);
                tvinsert.item.iImage = iSelectedImage;
                tvinsert.item.iSelectedImage = iSelectedImage;
            }
            hItem = TreeView_InsertItem(hwndTV, &tvinsert);
            TreeView_SetCheckState(hwndTV, hParent, TRUE);
            TreeView_SetCheckState(hwndTV, hItem, TRUE);
            if (nLevel <= nExpandLevels)
                TreeView_Expand(hwndTV, hParent, TVE_EXPAND);

            break;
        }
    }

    if (lpszNodeTextTV != NULL)
        MYFREE(lpszNodeTextTV);

    return hItem;
}

//--------------------------------------------------
// Fill treeview with values of comarison result 
//--------------------------------------------------
BOOL InitTreeViewItems(HWND hwndTV, int iPropertyPage)
{
    TV_INSERTSTRUCT tvinsert;
    LPCOMPRESULTNEW lpCR;
    LPTSTR lpszValueName = NULL;
    LPTSTR lpszFullNameTV = MYALLOC0((MAX_PATH+1) * sizeof(TCHAR));
    LPTSTR pos = NULL;
    LPTSTR NamePartTV = NULL;
    BOOL bKey = TRUE;
    
    LPCOMPRESULTNEW lpCRRelevantPart = (iPropertyPage == 0 ? CompareResult.stCRHeads.lpCRRegistryLast : CompareResult.stCRHeads.lpCRFilesystemLast);
    //if (iPropertyPage == 0) {
    //    lpCRRelevantPart = CompareResult.stCRHeads.lpCRRegistryLast;
    //}
    //else {
    //    lpCRRelevantPart = CompareResult.stCRHeads.lpCRFilesystemLast;
    //}

    InsertRootItems(hwndTV, iPropertyPage);
    
    for (lpCR = lpCRRelevantPart; NULL != lpCR; lpCR = lpCR->lpPrevCRNew) {
        lpszValueName = NULL;
        if (fOnlyNewEntries && 
            (DIRADD != lpCR->nActionType) && (FILEADD != lpCR->nActionType) &&
            (VALADD != lpCR->nActionType) && (KEYADD != lpCR->nActionType))
            continue;

        if ((NULL != lpCR->lpContentOld) && (NULL == lpCR->lpContentNew)) {
            if (!CheckFilters(lpCR->lpContentOld, lpszFullNameTV, &lpszValueName, lpCR->nActionType, &bKey))
                continue;
        }
        if (NULL != lpCR->lpContentNew) {
            if (!CheckFilters(lpCR->lpContentNew, lpszFullNameTV, &lpszValueName, lpCR->nActionType, &bKey))
                continue;
        }
        
        NamePartTV = lpszFullNameTV;
        pos = _tcschr(lpszFullNameTV, _T('\\'));
        
        UINT nLevel = 0;
        HANDLE hParent;
        HANDLE hItem = TreeView_GetNextItem(hwndTV, NULL, TVGN_ROOT);
        if (pos == NULL)
            hItem = NULL;
        while (pos != NULL) {
            _tcscpy(pos, TEXT("\0"));

// Vordere Namensteile
            if (nLevel == 0) {
                hParent = NULL;
                if (hItem == NULL) {
                    memset(&tvinsert, 0, sizeof(tvinsert));
                    tvinsert.hParent = hParent;
                    tvinsert.hInsertAfter = TVI_ROOT;
                    tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                    tvinsert.item.pszText = NamePartTV;
                    tvinsert.item.iImage = 0;
                    tvinsert.item.iSelectedImage = 1;
                    hItem = TreeView_InsertItem(hwndTV, &tvinsert);
                    TreeView_SetCheckState(hwndTV, hItem, TRUE);
                    TreeView_SetCheckState(hwndTV, hParent, TRUE);
                    TreeView_Expand(hwndTV, hItem, TVE_EXPAND);
                }
                else {
                    hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, TRUE, lpCR);
                }
            }
            else {
                hParent = hItem;
                hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
                hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, TRUE, lpCR);
            }
            NamePartTV = ++pos;
            nLevel++;
            pos = _tcschr(pos, _T('\\'));
        }
            
        hParent = hItem;
        hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
        hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, (lpszValueName != NULL ? TRUE : bKey), lpCR);

        if (lpszValueName != NULL) {
            hParent = hItem;
            hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
            hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, lpszValueName, iPropertyPage, bKey, lpCR);
        }
    }

    if (lpszFullNameTV != NULL)
        MYFREE(lpszFullNameTV);
    
    return TRUE;
}

//--------------------------------------------------
// InitTreeViewImageLists - creates an image list, adds three bitmaps 
// to it, and associates the image list with a tree-view control. 
// Returns TRUE if successful, or FALSE otherwise. 
// hwndTV - handle to the tree-view control. 
//--------------------------------------------------
BOOL InitTreeViewImageLists(HWND hwndTV)
{
    HIMAGELIST hImageList;
    HBITMAP hBitMap;      
    HICON hi;

    if (TRUE) {
        if ((hImageList = ImageList_Create(18, 18, ILC_COLOR32, 6, 6)) == NULL)
            return FALSE;

        HMODULE hmod = LoadLibraryA("shell32");
        if (hmod != NULL) {
            hi = LoadIcon(hmod, MAKEINTRESOURCE(4));            // closed folder
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);

            hi = LoadIcon(hmod, MAKEINTRESOURCE(235));          // open folder
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);

            //#define VALDEL          3
            hi = LoadIcon(hmod, MAKEINTRESOURCE(261));          // deleted file
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);
            
            //#define VALADD          4
            hi = LoadIcon(hmod, MAKEINTRESOURCE(152));          // (new) file
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);
            
            //#define VALMODI         5
            hi = LoadIcon(hmod, MAKEINTRESOURCE(243));          // changed file
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);


            FreeLibrary(hmod);
        }
        
        hmod = LoadLibraryA("regedit.exe");
        if (hmod != NULL) {
            hi = LoadIcon(hmod, MAKEINTRESOURCE(205));          // value
            ImageList_AddIcon(hImageList, hi);

            FreeLibrary(hmod);
        }
        
    }
    else {
        if ((hImageList = ImageList_Create(18, 18, FALSE, 6, 6)) == NULL)
            return FALSE;
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_CLOSED));
        int i = ImageList_Add(hImageList, hBitMap, NULL);				
        DeleteObject(hBitMap);
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_OPEN));	
        i = ImageList_Add(hImageList, hBitMap, NULL);					
        DeleteObject(hBitMap);											
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE));			
        i = ImageList_Add(hImageList, hBitMap, NULL);					
        DeleteObject(hBitMap);											
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE));			
        i = ImageList_Add(hImageList, hBitMap, NULL);					
        DeleteObject(hBitMap);											
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE));			
        i = ImageList_Add(hImageList, hBitMap, NULL);					
        DeleteObject(hBitMap);											
    
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE));			
        i = ImageList_Add(hImageList, hBitMap, NULL);					
        DeleteObject(hBitMap);											
    }
    
    TreeView_SetImageList(hwndTV, hImageList, TVSIL_NORMAL);        
    
    return TRUE;
}

//--------------------------------------------------
// Check actual content against filter 
//--------------------------------------------------
BOOL CheckFilters(LPVOID lpContent, LPTSTR lpszKeyName, LPTSTR * lpszValueName, DWORD nActionType, BOOL * pbKey)
{
    LPTSTR lpszFullName = NULL;
    if ((KEYDEL == nActionType) || (KEYADD == nActionType)) {
        *pbKey = TRUE;
        lpszFullName = GetWholeKeyName(lpContent, FALSE);
    }
    else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
        *pbKey = FALSE;
        lpszFullName = GetWholeKeyName(((LPVALUECONTENT)(lpContent))->lpFatherKC, FALSE);
        if ((((LPVALUECONTENT)(lpContent))->lpszValueName != NULL) && (lpszValueName != NULL))
            *lpszValueName = ((LPVALUECONTENT)(lpContent))->lpszValueName;
    }
    if ((lpszKeyName != NULL) && (lpszFullName != NULL))
        _tcscpy(lpszKeyName, lpszFullName);

    if ((KEYDEL == nActionType) || (KEYADD == nActionType) ||
        (VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
        if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
            MYFREE(lpszFullName);
            return FALSE;
        }
        if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
            MYFREE(lpszFullName);
            return FALSE;
        }
    }
    if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType) ||
        (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
        if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType))
            *pbKey = TRUE;
        else
            *pbKey = FALSE;
        lpszFullName = GetWholeFileName(lpContent, 0, FALSE);
        if ((lpszKeyName != NULL) && (lpszFullName != NULL))
            _tcscpy(lpszKeyName, lpszFullName);
        if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
            MYFREE(lpszFullName);
            return FALSE;
        }
    }
    MYFREE(lpszFullName);

    return TRUE;
}