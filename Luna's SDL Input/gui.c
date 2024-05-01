#include "gui.h"


HWND hDlgItem;
HWND parentVariable;
int selectedIndex;
int selectedKey;
float selectedX;
float selectedY;

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    FILE* fptr;
    errno_t err;
    char filePath[MAX_PATH];
    int i;

    switch (message)
    {
    case WM_INITDIALOG:
        hDlgItem = GetDlgItem(hwndDlg, IDC_MODIFIERS);
        memset(&LvColumn, 0, sizeof(LvColumn));
        LvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        LvColumn.cx = 0x28;
        LvColumn.pszText = "Key";
        LvColumn.cx = 0x42;
        SendMessageA(hDlgItem, LVM_INSERTCOLUMNA, 0, (LPARAM)&LvColumn);
        LvColumn.pszText = "X";
        SendMessageA(hDlgItem, LVM_INSERTCOLUMNA, 1, (LPARAM)&LvColumn);
        LvColumn.pszText = "Y";
        SendMessageA(hDlgItem, LVM_INSERTCOLUMNA, 2, (LPARAM)&LvColumn);
        LvItem.mask = LVIF_TEXT;
        LvItem.cchTextMax = 256;
        LvItem.pszText = "Init";
        LvItem.iSubItem = 0;
        for (i = 0; i < 50; i++) {
            LvItem.iItem = i;
            SendMessageA(hDlgItem, LVM_INSERTITEMA, 0, (LPARAM)&LvItem);
        }
        SendMessageA(hDlgItem, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

        resetButtonLabels(hwndDlg);
        break;

    case WM_CLOSE:
        loadConfig();
        EndDialog(hwndDlg, 0);
        break;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case NM_CLICK:
            hDlgItem = GetDlgItem(hwndDlg, IDC_MODIFIERS);
            selectedIndex = SendMessage(hDlgItem, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
            selectedKey = config.controller[0].modifiers[selectedIndex].mapping;
            selectedX = config.controller[0].modifiers[selectedIndex].multiplierX;
            selectedY = config.controller[0].modifiers[selectedIndex].multiplierY;
            setButtonLabel(hwndDlg, IDC_MODIFIERKEY, config.controller[0].modifiers[selectedIndex].mapping);
            setEditBoxContent(hwndDlg, IDC_MODIFIERX, &config.controller[0].modifiers[selectedIndex].multiplierX);
            setEditBoxContent(hwndDlg, IDC_MODIFIERY, &config.controller[0].modifiers[selectedIndex].multiplierY);
            break;
        }
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == EN_UPDATE) {
            switch (LOWORD(wParam))
            {
            case IDC_A2AXRANGE:
                getEditBoxContent(hwndDlg, IDC_A2AXRANGE, &config.controller[0].a2aRangeX);
                break;
            case IDC_A2AYRANGE:
                getEditBoxContent(hwndDlg, IDC_A2AXRANGE, &config.controller[0].a2aRangeY);
                break;
            case IDC_A2ADEADZONE:
                getEditBoxContent(hwndDlg, IDC_A2ADEADZONE, &config.controller[0].a2aDeadzone);
                break;
            case IDC_A2AOUTEREDGE:
                getEditBoxContent(hwndDlg, IDC_A2AOUTEREDGE, &config.controller[0].a2aOuterEdge);
                break;

            case IDC_D2ACARDINALX:
                getEditBoxContent(hwndDlg, IDC_D2ACARDINALX, &config.controller[0].d2aRangeCardinalX);
                break;
            case IDC_D2ACARDINALY:
                getEditBoxContent(hwndDlg, IDC_D2ACARDINALY, &config.controller[0].d2aRangeCardinalY);
                break;
            case IDC_D2ADIAGONALX:
                getEditBoxContent(hwndDlg, IDC_D2ADIAGONALX, &config.controller[0].d2aRangeDiagonalX);
                break;
            case IDC_D2ADIAGONALY:
                getEditBoxContent(hwndDlg, IDC_D2ADIAGONALY, &config.controller[0].d2aRangeDiagonalY);
                break;

            case IDC_A2DSTICKTHRESHOLD:
                getEditBoxContent(hwndDlg, IDC_A2DSTICKTHRESHOLD, &config.controller[0].a2dStickThreshold);
                break;
            case IDC_A2DTRIGGERTHRESHOLD:
                getEditBoxContent(hwndDlg, IDC_A2DTRIGGERTHRESHOLD, &config.controller[0].a2dTriggerThreshold);

            case IDC_MODIFIERX:
                getEditBoxContent(hwndDlg, IDC_MODIFIERX, &selectedX);
                break;
            case IDC_MODIFIERY:
                getEditBoxContent(hwndDlg, IDC_MODIFIERY, &selectedY);
                break;
            }
        }
        switch (wParam)
        {
        case IDCANCEL:
            loadConfig();
            EndDialog(hwndDlg, 0);
            break;
        case IDOK:
            saveConfig();
            EndDialog(hwndDlg, 0);
            break;
        case IDC_RESTOREDEFAULTS:
            restoreDefaults();
            resetButtonLabels(hwndDlg);
            break;

        case IDC_HELP:
            ShellExecuteA(0, 0, "http://sites.google.com/view/shurislibrary/luna-sdl", 0, 0, SW_HIDE);
            break;


        case IDC_ABUTTON1:
            getConfigKey(hwndDlg, IDC_ABUTTON1, &config.controller[0].A[0].mapping);
            break;
        case IDC_ABUTTON2:
            getConfigKey(hwndDlg, IDC_ABUTTON2, &config.controller[0].A[1].mapping);
            break;
        case IDC_ABUTTON3:
            getConfigKey(hwndDlg, IDC_ABUTTON3, &config.controller[0].A[2].mapping);
            break;
        case IDC_BBUTTON1:
            getConfigKey(hwndDlg, IDC_BBUTTON1, &config.controller[0].B[0].mapping);
            break;
        case IDC_BBUTTON2:
            getConfigKey(hwndDlg, IDC_BBUTTON2, &config.controller[0].B[1].mapping);
            break;
        case IDC_BBUTTON3:
            getConfigKey(hwndDlg, IDC_BBUTTON3, &config.controller[0].B[2].mapping);
            break;
        case IDC_START1:
            getConfigKey(hwndDlg, IDC_START1, &config.controller[0].Start[0].mapping);
            break;
        case IDC_START2:
            getConfigKey(hwndDlg, IDC_START2, &config.controller[0].Start[1].mapping);
            break;
        case IDC_START3:
            getConfigKey(hwndDlg, IDC_START3, &config.controller[0].Start[2].mapping);
            break;
        case IDC_LTRIG1:
            getConfigKey(hwndDlg, IDC_LTRIG1, &config.controller[0].L[0].mapping);
            break;
        case IDC_LTRIG2:
            getConfigKey(hwndDlg, IDC_LTRIG2, &config.controller[0].L[1].mapping);
            break;
        case IDC_LTRIG3:
            getConfigKey(hwndDlg, IDC_LTRIG3, &config.controller[0].L[2].mapping);
            break;
        case IDC_ZTRIG1:
            getConfigKey(hwndDlg, IDC_ZTRIG1, &config.controller[0].Z[0].mapping);
            break;
        case IDC_ZTRIG2:
            getConfigKey(hwndDlg, IDC_ZTRIG2, &config.controller[0].Z[1].mapping);
            break;
        case IDC_ZTRIG3:
            getConfigKey(hwndDlg, IDC_ZTRIG3, &config.controller[0].Z[2].mapping);
            break;
        case IDC_RTRIG1:
            getConfigKey(hwndDlg, IDC_RTRIG1, &config.controller[0].R[0].mapping);
            break;
        case IDC_RTRIG2:
            getConfigKey(hwndDlg, IDC_RTRIG2, &config.controller[0].R[1].mapping);
            break;
        case IDC_RTRIG3:
            getConfigKey(hwndDlg, IDC_RTRIG3, &config.controller[0].R[2].mapping);
            break;

        case IDC_ANALOGLEFT1:
            getConfigKey(hwndDlg, IDC_ANALOGLEFT1, &config.controller[0].AnalogLeft[0].mapping);
            break;
        case IDC_ANALOGLEFT2:
            getConfigKey(hwndDlg, IDC_ANALOGLEFT2, &config.controller[0].AnalogLeft[1].mapping);
            break;
        case IDC_ANALOGLEFT3:
            getConfigKey(hwndDlg, IDC_ANALOGLEFT3, &config.controller[0].AnalogLeft[2].mapping);
            break;
        case IDC_ANALOGRIGHT1:
            getConfigKey(hwndDlg, IDC_ANALOGRIGHT1, &config.controller[0].AnalogRight[0].mapping);
            break;
        case IDC_ANALOGRIGHT2:
            getConfigKey(hwndDlg, IDC_ANALOGRIGHT2, &config.controller[0].AnalogRight[1].mapping);
            break;
        case IDC_ANALOGRIGHT3:
            getConfigKey(hwndDlg, IDC_ANALOGRIGHT3, &config.controller[0].AnalogRight[2].mapping);
            break;
        case IDC_ANALOGUP1:
            getConfigKey(hwndDlg, IDC_ANALOGUP1, &config.controller[0].AnalogUp[0].mapping);
            break;
        case IDC_ANALOGUP2:
            getConfigKey(hwndDlg, IDC_ANALOGUP2, &config.controller[0].AnalogUp[1].mapping);
            break;
        case IDC_ANALOGUP3:
            getConfigKey(hwndDlg, IDC_ANALOGUP3, &config.controller[0].AnalogUp[2].mapping);
            break;
        case IDC_ANALOGDOWN1:
            getConfigKey(hwndDlg, IDC_ANALOGDOWN1, &config.controller[0].AnalogDown[0].mapping);
            break;
        case IDC_ANALOGDOWN2:
            getConfigKey(hwndDlg, IDC_ANALOGDOWN2, &config.controller[0].AnalogDown[1].mapping);
            break;
        case IDC_ANALOGDOWN3:
            getConfigKey(hwndDlg, IDC_ANALOGDOWN3, &config.controller[0].AnalogDown[2].mapping);
            break;

        case IDC_CLEFT1:
            getConfigKey(hwndDlg, IDC_CLEFT1, &config.controller[0].CLeft[0].mapping);
            break;
        case IDC_CLEFT2:
            getConfigKey(hwndDlg, IDC_CLEFT2, &config.controller[0].CLeft[1].mapping);
            break;
        case IDC_CLEFT3:
            getConfigKey(hwndDlg, IDC_CLEFT3, &config.controller[0].CLeft[2].mapping);
            break;
        case IDC_CRIGHT1:
            getConfigKey(hwndDlg, IDC_CRIGHT1, &config.controller[0].CRight[0].mapping);
            break;
        case IDC_CRIGHT2:
            getConfigKey(hwndDlg, IDC_CRIGHT2, &config.controller[0].CRight[1].mapping);
            break;
        case IDC_CRIGHT3:
            getConfigKey(hwndDlg, IDC_CRIGHT3, &config.controller[0].CRight[2].mapping);
            break;
        case IDC_CUP1:
            getConfigKey(hwndDlg, IDC_CUP1, &config.controller[0].CUp[0].mapping);
            break;
        case IDC_CUP2:
            getConfigKey(hwndDlg, IDC_CUP2, &config.controller[0].CUp[1].mapping);
            break;
        case IDC_CUP3:
            getConfigKey(hwndDlg, IDC_CUP3, &config.controller[0].CUp[2].mapping);
            break;
        case IDC_CDOWN1:
            getConfigKey(hwndDlg, IDC_CDOWN1, &config.controller[0].CDown[0].mapping);
            break;
        case IDC_CDOWN2:
            getConfigKey(hwndDlg, IDC_CDOWN2, &config.controller[0].CDown[1].mapping);
            break;
        case IDC_CDOWN3:
            getConfigKey(hwndDlg, IDC_CDOWN3, &config.controller[0].CDown[2].mapping);
            break;

        case IDC_DPADLEFT1:
            getConfigKey(hwndDlg, IDC_DPADLEFT1, &config.controller[0].DpadLeft[0].mapping);
            break;
        case IDC_DPADLEFT2:
            getConfigKey(hwndDlg, IDC_DPADLEFT2, &config.controller[0].DpadLeft[1].mapping);
            break;
        case IDC_DPADLEFT3:
            getConfigKey(hwndDlg, IDC_DPADLEFT3, &config.controller[0].DpadLeft[2].mapping);
            break;
        case IDC_DPADRIGHT1:
            getConfigKey(hwndDlg, IDC_DPADRIGHT1, &config.controller[0].DpadRight[0].mapping);
            break;
        case IDC_DPADRIGHT2:
            getConfigKey(hwndDlg, IDC_DPADRIGHT2, &config.controller[0].DpadRight[1].mapping);
            break;
        case IDC_DPADRIGHT3:
            getConfigKey(hwndDlg, IDC_DPADRIGHT3, &config.controller[0].DpadRight[2].mapping);
            break;
        case IDC_DPADUP1:
            getConfigKey(hwndDlg, IDC_DPADUP1, &config.controller[0].DpadUp[0].mapping);
            break;
        case IDC_DPADUP2:
            getConfigKey(hwndDlg, IDC_DPADUP2, &config.controller[0].DpadUp[1].mapping);
            break;
        case IDC_DPADUP3:
            getConfigKey(hwndDlg, IDC_DPADUP3, &config.controller[0].DpadUp[2].mapping);
            break;
        case IDC_DPADDOWN1:
            getConfigKey(hwndDlg, IDC_DPADDOWN1, &config.controller[0].DpadDown[0].mapping);
            break;
        case IDC_DPADDOWN2:
            getConfigKey(hwndDlg, IDC_DPADDOWN2, &config.controller[0].DpadDown[1].mapping);
            break;
        case IDC_DPADDOWN3:
            getConfigKey(hwndDlg, IDC_DPADDOWN3, &config.controller[0].DpadDown[2].mapping);
            break;

        case IDC_MODIFIERKEY:
            getConfigKey(hwndDlg, IDC_MODIFIERKEY, &selectedKey);
            break;
        case IDC_MODIFIERADD:
            hDlgItem = GetDlgItem(hwndDlg, IDC_MODIFIERS);
            selectedIndex = SendMessage(hDlgItem, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
            if (selectedIndex >= 0) {
                config.controller[0].modifiers[selectedIndex].mapping = selectedKey;
                config.controller[0].modifiers[selectedIndex].multiplierX = selectedX;
                config.controller[0].modifiers[selectedIndex].multiplierY = selectedY;
                setListRow(hwndDlg, selectedIndex, selectedKey, selectedX, selectedY);
            }
            break;
        case IDC_MODIFIERCLEAR:
            hDlgItem = GetDlgItem(hwndDlg, IDC_MODIFIERS);
            selectedIndex = SendMessage(hDlgItem, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
            if (selectedIndex >= 0) {
                config.controller[0].modifiers[selectedIndex].mapping = 0;
                config.controller[0].modifiers[selectedIndex].multiplierX = 0;
                config.controller[0].modifiers[selectedIndex].multiplierY = 0;
                setListRow(hwndDlg, selectedIndex, 0, 0, 0);
            }
            break;
        }

    default:
        return FALSE;
    }
    return TRUE;
}

void OpenDialog(HINSTANCE hinst, HWND parent)
{
    parentVariable = parent;
    DialogBox(hinst, MAKEINTRESOURCE(IDD_CONFIG), parent, DlgProc);
}

void getEditBoxContent(HWND hwndDlg, int nIDDlgItem, float* returnVariable) {
    char lpch[32];
    hDlgItem = GetDlgItem(hwndDlg, nIDDlgItem);
    GetWindowTextA(hDlgItem, &lpch, sizeof(lpch));
    *returnVariable = atof(lpch);
    if (*returnVariable < 0) {
        *returnVariable = (0.0 - *returnVariable);
    }
    if (*returnVariable > 1.0) {
        *returnVariable = 1.0;
        setEditBoxContent(hwndDlg, nIDDlgItem, returnVariable);
    }
}

void getConfigKey(HWND hwndDlg, int nIDDlgItem, byte* returnVariable) {
    //Check for key/button pressed to add to config
}

void setButtonLabel(HWND hwndDlg, int nIDDlgItem, byte returnVariable) {
    //Get key or button name from SDL API
}

void setEditBoxContent(HWND hwndDlg, int nIDDlgItem, float* returnVariable) {
    char lpch[32];
    errno_t err;
    hDlgItem = GetDlgItem(hwndDlg, nIDDlgItem);
    Edit_LimitText(hDlgItem, 8);
    err = _gcvt_s(lpch, sizeof(lpch), *returnVariable, 7);
    SetWindowTextA(hDlgItem, lpch);
}

void setListRow(HWND hwndDlg, int Index, int Key, float multX, float multY) {
    char lpch[32];
    hDlgItem = GetDlgItem(hwndDlg, IDC_MODIFIERS);

    LvItem.iItem = Index;
    LvItem.iSubItem = 0;
    if (Key != 0) {
        //Get key or button name from SDL API
        SendMessageW(hDlgItem, LVM_SETITEMW, 0, (LPARAM)&LvItem);
    }
    else {
        LvItem.pszText = "Not set";
        SendMessageA(hDlgItem, LVM_SETITEMA, 0, (LPARAM)&LvItem);
    }

    LvItem.iSubItem = 1;
    _gcvt_s(lpch, sizeof(lpch), multX, 5);
    LvItem.pszText = lpch;
    SendMessageA(hDlgItem, LVM_SETITEMA, 0, (LPARAM)&LvItem);

    LvItem.iSubItem = 2;
    _gcvt_s(lpch, sizeof(lpch), multY, 5);
    LvItem.pszText = lpch;
    SendMessageA(hDlgItem, LVM_SETITEMA, 0, (LPARAM)&LvItem);
}

void resetButtonLabels(HWND hwndDlg) {
    setEditBoxContent(hwndDlg, IDC_A2AXRANGE, &config.controller[0].a2aRangeX);
    setEditBoxContent(hwndDlg, IDC_A2AYRANGE, &config.controller[0].a2aRangeY);
    setEditBoxContent(hwndDlg, IDC_A2ADEADZONE, &config.controller[0].a2aDeadzone);
    setEditBoxContent(hwndDlg, IDC_A2AOUTEREDGE, &config.controller[0].a2aOuterEdge);
    setEditBoxContent(hwndDlg, IDC_D2ACARDINALX, &config.controller[0].d2aRangeCardinalX);
    setEditBoxContent(hwndDlg, IDC_D2ACARDINALY, &config.controller[0].d2aRangeCardinalY);
    setEditBoxContent(hwndDlg, IDC_D2ADIAGONALX, &config.controller[0].d2aRangeDiagonalX);
    setEditBoxContent(hwndDlg, IDC_D2ADIAGONALY, &config.controller[0].d2aRangeDiagonalY);
    setEditBoxContent(hwndDlg, IDC_A2DSTICKTHRESHOLD, &config.controller[0].a2dStickThreshold);

    setButtonLabel(hwndDlg, IDC_ABUTTON1, config.controller[0].A[0].mapping);
    setButtonLabel(hwndDlg, IDC_ABUTTON2, config.controller[0].A[1].mapping);
    setButtonLabel(hwndDlg, IDC_ABUTTON3, config.controller[0].A[2].mapping);
    setButtonLabel(hwndDlg, IDC_BBUTTON1, config.controller[0].B[0].mapping);
    setButtonLabel(hwndDlg, IDC_BBUTTON2, config.controller[0].B[1].mapping);
    setButtonLabel(hwndDlg, IDC_BBUTTON3, config.controller[0].B[2].mapping);
    setButtonLabel(hwndDlg, IDC_START1, config.controller[0].Start[0].mapping);
    setButtonLabel(hwndDlg, IDC_START2, config.controller[0].Start[1].mapping);
    setButtonLabel(hwndDlg, IDC_START3, config.controller[0].Start[2].mapping);
    setButtonLabel(hwndDlg, IDC_LTRIG1, config.controller[0].L[0].mapping);
    setButtonLabel(hwndDlg, IDC_LTRIG2, config.controller[0].L[1].mapping);
    setButtonLabel(hwndDlg, IDC_LTRIG3, config.controller[0].L[2].mapping);
    setButtonLabel(hwndDlg, IDC_ZTRIG1, config.controller[0].Z[0].mapping);
    setButtonLabel(hwndDlg, IDC_ZTRIG2, config.controller[0].Z[1].mapping);
    setButtonLabel(hwndDlg, IDC_ZTRIG3, config.controller[0].Z[2].mapping);
    setButtonLabel(hwndDlg, IDC_RTRIG1, config.controller[0].R[0].mapping);
    setButtonLabel(hwndDlg, IDC_RTRIG2, config.controller[0].R[1].mapping);
    setButtonLabel(hwndDlg, IDC_RTRIG3, config.controller[0].R[2].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGLEFT1, config.controller[0].AnalogLeft[0].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGLEFT2, config.controller[0].AnalogLeft[1].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGLEFT3, config.controller[0].AnalogLeft[2].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGRIGHT1, config.controller[0].AnalogRight[0].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGRIGHT2, config.controller[0].AnalogRight[1].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGRIGHT3, config.controller[0].AnalogRight[2].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGUP1, config.controller[0].AnalogUp[0].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGUP2, config.controller[0].AnalogUp[1].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGUP3, config.controller[0].AnalogUp[2].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGDOWN1, config.controller[0].AnalogDown[0].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGDOWN2, config.controller[0].AnalogDown[1].mapping);
    setButtonLabel(hwndDlg, IDC_ANALOGDOWN3, config.controller[0].AnalogDown[2].mapping);
    setButtonLabel(hwndDlg, IDC_CLEFT1, config.controller[0].CLeft[0].mapping);
    setButtonLabel(hwndDlg, IDC_CLEFT2, config.controller[0].CLeft[1].mapping);
    setButtonLabel(hwndDlg, IDC_CLEFT3, config.controller[0].CLeft[2].mapping);
    setButtonLabel(hwndDlg, IDC_CRIGHT1, config.controller[0].CRight[0].mapping);
    setButtonLabel(hwndDlg, IDC_CRIGHT2, config.controller[0].CRight[1].mapping);
    setButtonLabel(hwndDlg, IDC_CRIGHT3, config.controller[0].CRight[2].mapping);
    setButtonLabel(hwndDlg, IDC_CUP1, config.controller[0].CUp[0].mapping);
    setButtonLabel(hwndDlg, IDC_CUP2, config.controller[0].CUp[1].mapping);
    setButtonLabel(hwndDlg, IDC_CUP3, config.controller[0].CUp[2].mapping);
    setButtonLabel(hwndDlg, IDC_CDOWN1, config.controller[0].CDown[0].mapping);
    setButtonLabel(hwndDlg, IDC_CDOWN2, config.controller[0].CDown[1].mapping);
    setButtonLabel(hwndDlg, IDC_CDOWN3, config.controller[0].CDown[2].mapping);
    setButtonLabel(hwndDlg, IDC_DPADLEFT1, config.controller[0].DpadLeft[0].mapping);
    setButtonLabel(hwndDlg, IDC_DPADLEFT2, config.controller[0].DpadLeft[1].mapping);
    setButtonLabel(hwndDlg, IDC_DPADLEFT3, config.controller[0].DpadLeft[2].mapping);
    setButtonLabel(hwndDlg, IDC_DPADRIGHT1, config.controller[0].DpadRight[0].mapping);
    setButtonLabel(hwndDlg, IDC_DPADRIGHT2, config.controller[0].DpadRight[1].mapping);
    setButtonLabel(hwndDlg, IDC_DPADRIGHT3, config.controller[0].DpadRight[2].mapping);
    setButtonLabel(hwndDlg, IDC_DPADUP1, config.controller[0].DpadUp[0].mapping);
    setButtonLabel(hwndDlg, IDC_DPADUP2, config.controller[0].DpadUp[1].mapping);
    setButtonLabel(hwndDlg, IDC_DPADUP3, config.controller[0].DpadUp[2].mapping);
    setButtonLabel(hwndDlg, IDC_DPADDOWN1, config.controller[0].DpadDown[0].mapping);
    setButtonLabel(hwndDlg, IDC_DPADDOWN2, config.controller[0].DpadDown[1].mapping);
    setButtonLabel(hwndDlg, IDC_DPADDOWN3, config.controller[0].DpadDown[2].mapping);

    int i;
    for (i = 0; i < 50; i++) {
        setListRow(hwndDlg, i, config.controller[0].modifiers[i].mapping, config.controller[0].modifiers[i].multiplierX, config.controller[0].modifiers[i].multiplierY);
    }
}