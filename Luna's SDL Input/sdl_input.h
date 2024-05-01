#include "config.h"

const byte* keyboardState;
char* filePath[MAX_PATH];
int err;
SDL_Gamepad* Gamepad[10]; //Support up to 10 SDL controllers
SDL_JoystickID* Joystick;
char* MappedGUIDString[33];
char* GamepadGUIDString[33];
int GamepadEnum;
SDL_Gamepad* CurrentGamepad;
int DigitalToAnalogCount;
int GamepadCount;

void SDLInit();
void SDLDeinit();
void SDLGetKeys();
SDL_Gamepad* GetCurrentController(SDL_JoystickGUID guid, byte contEnum);
byte SDLAnalogToDigital(int Cont, byte Mapping, byte Sign);
byte GetButtonState(int Cont, Button* button);
byte GetModifierState(int Cont, Modifier* modifier, int id);
float GetAnalogDirection(int Cont, Button* Direction, int i);
void GetAnalogState(int Cont, byte* AnalogX, byte* AnalogY);