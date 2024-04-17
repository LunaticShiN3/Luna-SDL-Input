#include "pch.h"

#define PLUGIN_NAME "Luna's SDL Input"
#define PLUGIN_VERSION "1.0.0"
#define PLUGIN_NAMEVER PLUGIN_NAME " v" PLUGIN_VERSION
#define PLUGIN_REPO "https://github.com/LunaticShiN3/Luna-SDL-Input"

#include "zilmar_controller_1.0.h"
#include "sdl_input.h"
#include "config.h"

char* filePath[MAX_PATH];
int err;

HMODULE hModuleVariable;
HWND hMainWindowVariable;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    hModuleVariable = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //Create %AppData%\Luna-Project64 folder for people who use cringe emu
        GetEmuFolderPath(filePath);
        CreateDirectoryA(filePath, NULL);
        //Create log file
        PathAppendA(filePath, "Logs");
        CreateDirectoryA(filePath, NULL);
        PathAppendA(filePath, "LunaSDLlog.txt");
        err = fopen_s(logfile, filePath, "w");
        //Create config file
        GetEmuFolderPath(filePath);
        PathAppendA(filePath, "Config");
        CreateDirectoryA(filePath, NULL);
        PathAppendA(filePath, "LunaSDLconfig.bin");
        err = fopen_s(configfile, filePath, "wb");
        fclose(configfile);
        //loadConfig();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        fclose(logfile);
        break;
    }
    return TRUE;
}

EXPORT void CALL CloseDLL(void) {
    SDLDeinit();
}

EXPORT void CALL DllAbout(HWND hParent) {
    MessageBoxA(
        hParent,
        "An input plugin that FINALLY works for everything.\n"
        "Made by ShiN3\n"
        "Version " PLUGIN_VERSION "\n"
        "Compiled on " __DATE__ "\n\n"
        PLUGIN_REPO,
        "About " PLUGIN_NAMEVER,
        MB_OK | MB_ICONINFORMATION
    );
}

EXPORT void CALL DllConfig(HWND hParent) {
    OpenDialog(hModuleVariable, hParent);
}

EXPORT void CALL GetDllInfo(PLUGIN_INFO* PluginInfo) {
    PluginInfo->Version = 0x0100;
    PluginInfo->Type = PLUGIN_TYPE_CONTROLLER;
    strncpy_s(
        PluginInfo->Name,
        sizeof(PluginInfo->Name),
        PLUGIN_NAMEVER,
        sizeof(PluginInfo->Name)
    );
}

EXPORT void CALL GetKeys(int Control, BUTTONS* Keys) {
    if (config.controller[Control].isConnected) {
        byte AnalogX;
        byte AnalogY;

        Keys->R_DPAD = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->L_DPAD = GetButtonState(Control, config.controller[Control].DpadLeft);
        Keys->D_DPAD = GetButtonState(Control, config.controller[Control].DpadDown);
        Keys->U_DPAD = GetButtonState(Control, config.controller[Control].DpadUp);
        Keys->START_BUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->Z_TRIG = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->B_BUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->A_BUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->R_CBUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->L_CBUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->D_CBUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->U_CBUTTON = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->R_TRIG = GetButtonState(Control, config.controller[Control].DpadRight);
        Keys->L_TRIG = GetButtonState(Control, config.controller[Control].DpadRight);

        GetAnalogState(Control, &AnalogX, &AnalogY);
        //X and Y axis namings are swapped in input spec
        Keys->Y_AXIS = AnalogX;
        Keys->X_AXIS = AnalogY;
    }
}

EXPORT void CALL InitiateControllers(HWND hMainWindow, CONTROL Controls[4])
{
    for (int i = 0; i < 4; ++i)
    {
        Controls[i].Present = FALSE; //Enables controller 1 only. Might change later.
        Controls[i].RawData = FALSE;
    }
    Controls[0].Present = TRUE;

    hMainWindowVariable = hMainWindow;

    DInputInit(hModuleVariable, hMainWindowVariable);
}

EXPORT void CALL RomClosed(void) {
    //required for PJ64 2.x and newer
}

EXPORT void CALL RomOpen(void) {
    //IDirectInputDevice8_Unacquire(lpdiKeyboard);
    //IDirectInputDevice8_SetCooperativeLevel(lpdiKeyboard, hMainWindowVariable, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    //int i;
    //for (i = 0; i < 100; i++) {
        //HRESULT result = IDirectInputDevice8_Acquire(lpdiKeyboard);
        //if (result != DIERR_OTHERAPPHASPRIO) {
            //break;
        //}
        //Sleep(50);
    //}
}

/*EXPORT void CALL WM_KeyDown(WPARAM wParam, LPARAM lParam) {
    //stuff here later
}

EXPORT void CALL WM_KeyUp(WPARAM wParam, LPARAM lParam) {
    //stuff here later
}*/