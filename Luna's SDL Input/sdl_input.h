#include <SDL.h>
#include "config.h"

const byte* keyboardState;

void SDLInit();
void SDLDeinit();
void SDLGetKeys();
byte GetButtonState(Button* button);