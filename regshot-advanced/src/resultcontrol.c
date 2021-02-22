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

BOOL CheckFilters(LPVALUECONTENT lpContent, LPTSTR lpszKeyName, LPTSTR * lpszValueName, DWORD nActionType, BOOL* pbKey);

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
    psp[PROP_TVDIRS].pszIcon = NULL;  // MAKEINTRESOURCE(IDI_ICON_GREEN);
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

    PropertySheet(&psh);

    return;
}

//--------------------------------------------------
// Dialog Procedure of comparison result check 
// treeview
//--------------------------------------------------
BOOL CALLBACK DlgSkipTVProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    HWND hwndTV; 
    TVITEM pitem;
    LPNMHDR lpnmhdr;
    LPNMTREEVIEW pnmtv;
    int iPropertyPage;

    switch (message) {
        case WM_INITDIALOG: {
            bSuppressNotifications = TRUE;

            InitCommonControls();

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
            hwndTV = GetDlgItem(hDlg, IDC_TREE_SKIPS);
            switch (lpnmhdr->code)
            {
                case PSN_SETACTIVE:
                    SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_SETCHECK, bNoOutput, (LPARAM)0);
                    break;

                case PSN_RESET:
                case PSN_QUERYCANCEL:
                case PSN_KILLACTIVE:
                {
                    bNoOutput = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
                    break;
                }

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
            
                case PSN_APPLY:
                {
                    iPropertyPage = PropSheet_HwndToIndex(GetParent(hDlg), hDlg);
                    SearchTVDown(hwndTV, TreeView_GetRoot(hwndTV), iPropertyPage);
                    bNoOutput = (BOOL)SendMessage(GetDlgItem(hDlg, IDC_CHECK_SUPPRESS), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
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

    LPTSTR * lpszNodeTextTV = MYALLOC0(99* sizeof(LPTSTR));
    int iStringCounter = 0;
    int iSkipCounter = 0;

    if (iPropertyPage == PROP_TVREGS)
        pSkipList = pRegSkipList;
    else
        pSkipList = pFileSkipList;

    for (iSkipCounter = 0; (NULL != pSkipList[iSkipCounter].lpSkipString); iSkipCounter++);
    pSkipList[iSkipCounter].lpSkipString = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
    pSkipList[iSkipCounter].bSkipKeyActive = TRUE;
    pSkipList[iSkipCounter].bAutomaticallyAdded = FALSE;
    pSkipList[iSkipCounter].bAdhoc = TRUE;
    pSkipList[iSkipCounter].bDeleted = FALSE;
    pSkipList[iSkipCounter].bFile = FALSE;
    pSkipList = MYREALLOC0((iSkipCounter+2) * sizeof(SKIPLIST), pSkipList);

    // SkipString bilden
    if (lpszNodeTextTV != NULL)
        lpszNodeTextTV[iStringCounter] = MYALLOC0(MAX_PATH * sizeof(TCHAR));
    if (lpszNodeTextTV[iStringCounter] != NULL) {
        memset(&pitem, 0, sizeof(pitem));
        pitem.hItem = hItem;
        pitem.mask = TVIF_TEXT;
        pitem.pszText = lpszNodeTextTV[iStringCounter];
        pitem.cchTextMax = MAX_PATH;
        TreeView_GetItem(hwndTV, &pitem);
    }

    while (TRUE) {
        hItem = TreeView_GetParent(hwndTV, hItem);
        if (hItem == NULL)
            break;
        iStringCounter++;

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
    
    _tcscpy(pSkipList[iSkipCounter].lpSkipString, TEXT(""));
    for (int i = iStringCounter; i >= 0; i--) {
        _tcscat(pSkipList[iSkipCounter].lpSkipString, lpszNodeTextTV[i]);
        MYFREE(lpszNodeTextTV[i]);
        if (i>0)
            _tcscat(pSkipList[iSkipCounter].lpSkipString, TEXT("\\"));
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

    if (iPropertyPage == 0) {
        for (int i = 0; i < _countof(szRootKeys); i++) {
            memset(&tvinsert, 0, sizeof(tvinsert));
            tvinsert.hParent = NULL;
            tvinsert.hInsertAfter = TVI_ROOT;
            tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
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
                lpszDrive = MYALLOC0(_tcslen(pDirScanList[i].lpSkipString) * sizeof(TCHAR));
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
HTREEITEM FindOrCreateTreeItem(HWND hwndTV, HTREEITEM hParent, HTREEITEM hItem, int nLevel, LPTSTR lpszNodeText, int iPropertyPage, BOOL bKey)
{
    TVITEM pitem;
    TV_INSERTSTRUCT tvinsert;
    LPTSTR lpszNodeTextTV = MYALLOC0((MAX_PATH + 1) * sizeof(TCHAR));
    
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
                tvinsert.item.iImage = (bKey ? 0 : 3);
                tvinsert.item.iSelectedImage = (bKey ? 0 : 3);
            }
            else {
                tvinsert.item.iImage = (bKey ? 0 : 2);
                tvinsert.item.iSelectedImage = (bKey ? 0 : 2);
            }
            hItem = TreeView_InsertItem(hwndTV, &tvinsert);
            TreeView_SetCheckState(hwndTV, hParent, TRUE);
            TreeView_SetCheckState(hwndTV, hItem, TRUE);
            if (nLevel <= 4)
                TreeView_Expand(hwndTV, hParent, TVE_EXPAND);

            break;
        }
    }

    return hItem;
}

//--------------------------------------------------
// Fill treeview with values of comarison result 
//--------------------------------------------------
BOOL InitTreeViewItems(HWND hwndTV, int iPropertyPage)
{
    TV_INSERTSTRUCT tvinsert;
    LPCOMPRESULTNEW lpCR;
    LPTSTR lpszFullName = NULL;
    LPTSTR lpszValueName = NULL;
    LPTSTR lpszFullNameTV = MYALLOC0(EXTDIRLEN * sizeof(TCHAR));
    LPTSTR pos = NULL;
    LPTSTR NamePartTV = NULL;
    BOOL bKey = TRUE;
    
    LPCOMPRESULTNEW lpCRRelevantPart;
    if (iPropertyPage == 0) {
        lpCRRelevantPart = CompareResult.stCRHeads.lpCRRegistryLast;
    }
    else {
        lpCRRelevantPart = CompareResult.stCRHeads.lpCRFilesystemLast;
    }

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
            
            //lpszFullName = NULL;
            //if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType)) {
            //    bKey = TRUE;
            //    lpszFullName = GetWholeKeyName(lpCR->lpContentOld, FALSE);
            //}
            //else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
            //    bKey = FALSE;
            //    lpszFullName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
            //    lpszValueName = ((LPVALUECONTENT)(lpCR->lpContentOld))->lpszValueName;
            //}
            //if (lpszFullName != NULL)
            //    _tcscpy(lpszFullNameTV, lpszFullName);

            //if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType) ||
            //    (VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
            //    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //}
            //if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) ||
            //    (DIRMODI == lpCR->nActionType) || (FILEDEL == lpCR->nActionType) ||
            //    (FILEADD == lpCR->nActionType) || (FILEMODI == lpCR->nActionType)) {
            //    if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) || (DIRMODI == lpCR->nActionType))
            //        bKey = TRUE;
            //    else
            //        bKey = FALSE;
            //    lpszFullName = GetWholeFileName(lpCR->lpContentOld, 0, FALSE);
            //    if (lpszFullName != NULL)
            //        _tcscpy(lpszFullNameTV, lpszFullName);
            //    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //}
            //MYFREE(lpszFullName);
        }
        if (NULL != lpCR->lpContentNew) {
//            CheckFilters(LPVALUECONTENT lpContent, LPTSTR lpszKeyName, LPTSTR lpszValueName, DWORD nActionType, BOOL * pbKey)
            if (!CheckFilters(lpCR->lpContentNew, lpszFullNameTV, &lpszValueName, lpCR->nActionType, &bKey))
                continue;
            
            //lpszFullName = NULL;
            //if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType)) {
            //    bKey = TRUE;
            //    lpszFullName = GetWholeKeyName(lpCR->lpContentNew, FALSE);
            //}
            //else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
            //    bKey = FALSE;
            //    lpszFullName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentNew))->lpFatherKC, FALSE);
            //    lpszValueName = ((LPVALUECONTENT)(lpCR->lpContentNew))->lpszValueName;
            //}
            //if (lpszFullName != NULL)
            //    _tcscpy(lpszFullNameTV, lpszFullName);
        
            //if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType) ||
            //    (VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
            //    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //}
            //if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) || (DIRMODI == lpCR->nActionType) ||
            //    (FILEDEL == lpCR->nActionType) || (FILEADD == lpCR->nActionType) || (FILEMODI == lpCR->nActionType)) {
            //    if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) || (DIRMODI == lpCR->nActionType)) 
            //        bKey = TRUE;
            //    else
            //        bKey = FALSE;
            //    lpszFullName = GetWholeFileName(lpCR->lpContentNew, 0, FALSE);
            //    if (lpszFullName != NULL)
            //        _tcscpy(lpszFullNameTV, lpszFullName);
            //    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
            //        MYFREE(lpszFullName);
            //        continue;
            //    }
            //}
            //MYFREE(lpszFullName);
        }
        
        NamePartTV = lpszFullNameTV;
        pos = _tcschr(lpszFullNameTV, _T('\\'));
        
        int nLevel = 0;
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
                    hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, TRUE);
                }
            }
            else {
                hParent = hItem;
                hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
                hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, TRUE);
            }
            NamePartTV = ++pos;
            nLevel++;
            pos = _tcschr(pos, _T('\\'));
        }
            
        hParent = hItem;
        hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
        hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, NamePartTV, iPropertyPage, (lpszValueName != NULL ? TRUE : bKey));

        if (lpszValueName != NULL) {
            hParent = hItem;
            hItem = TreeView_GetNextItem(hwndTV, hItem, TVGN_CHILD);
            hItem = FindOrCreateTreeItem(hwndTV, hParent, hItem, nLevel, lpszValueName, iPropertyPage, bKey);
        }
    }

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

    if (1 == 1) {
        if ((hImageList = ImageList_Create(18, 18, ILC_COLOR32, 4, 4)) == NULL)
            return FALSE;

        HMODULE hmod = LoadLibraryA("shell32");
        if (hmod != NULL) {
            hi = LoadIcon(hmod, MAKEINTRESOURCE(4));            // closed folder
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);

            hi = LoadIcon(hmod, MAKEINTRESOURCE(235));          // open folder
            ImageList_AddIcon(hImageList, hi);
            DeleteObject(hi);

            hi = LoadIcon(hmod, MAKEINTRESOURCE(152));          // file
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
        if ((hImageList = ImageList_Create(18, 18, FALSE, 4, 4)) == NULL)
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
    }
    
    TreeView_SetImageList(hwndTV, hImageList, TVSIL_NORMAL);        
    
    return TRUE;
}

//--------------------------------------------------
// Check actual content against filter 
//--------------------------------------------------
BOOL CheckFilters(LPVALUECONTENT lpContent, LPTSTR lpszKeyName, LPTSTR * lpszValueName, DWORD nActionType, BOOL * pbKey)
{
    LPTSTR lpszFullName = NULL;
    if ((KEYDEL == nActionType) || (KEYADD == nActionType)) {
        *pbKey = TRUE;
        lpszFullName = GetWholeKeyName(lpContent, FALSE);
    }
    else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
        *pbKey = FALSE;
        lpszFullName = GetWholeKeyName(((LPVALUECONTENT)(lpContent))->lpFatherKC, FALSE);
        if ((lpContent->lpszValueName != NULL) && (lpszValueName != NULL))
            *lpszValueName = lpContent->lpszValueName;
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