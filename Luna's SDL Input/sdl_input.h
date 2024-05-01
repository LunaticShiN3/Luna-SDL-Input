#include "config.h"

const byte* keyboardState;

void SDLInit();
void SDLDeinit();
void SDLGetKeys();
SDL_Gamepad* GetCurrentController(SDL_JoystickGUID guid, byte contEnum);
byte SDLAnalogToDigital(int Cont, byte Mapping, byte Sign);
byte GetButtonState(int Cont, Button* button);
byte GetModifierState(int Cont, Modifier* modifier, int id);
float GetAnalogDirection(int Cont, Button* Direction, int i);
void GetAnalogState(int Cont, byte* AnalogX, byte* AnalogY);