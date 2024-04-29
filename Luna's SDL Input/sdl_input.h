#include "config.h"

const byte* keyboardState;

void SDLInit();
void SDLDeinit();
void SDLGetKeys();
byte GetButtonState(int Cont, Button* button);
byte GetModifierState(int Cont, Modifier* modifier, int id);
void GetAnalogState(int Cont, byte* AnalogX, byte* AnalogY);

typedef struct SDLConnectedControllers {
	SDL_JoystickGUID guid
};