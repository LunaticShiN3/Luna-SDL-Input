#include "pch.h"
#include "resource.h"
#include "config.h"
#include <shellapi.h>

HWND hDlgItem;
HWND parentVariable;
LVITEMA LvItem;
LVCOLUMNA LvColumn;

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);