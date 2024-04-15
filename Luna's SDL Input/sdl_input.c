#include "sdl_input.h"

char* filePath[MAX_PATH];
int err;

void SDLInit() {
	//Init SDL
	SDL_SetMainReady();
	SDLInit(SDL_INIT_GAMECONTROLLER);
	//Find gamecontrollerdb.txt
	GetModuleFileNameA(NULL, filePath, sizeof(filePath));
	PathRemoveFileSpecA(filePath);
	PathAppendA(filePath, "Plugin\\Input\\gamecontrollerdb.txt");
	SDL_GameControllerAddMappingsFromFile(filePath);
}

void SDLDeinit() {
	SDL_Quit();
}

void SDLGetKeys() {
	keyboardState = SDL_GetKeyboardState;
}

byte GetButtonState(Button* button) {
	int ButtonState = 0;
	int i;
	for (i = 0; i < 3; i++) {
		if (button[i].uuid[0] == 0 && button[i].uuid[1] == 0) {
			ButtonState = keyboardState[button[i].mapping];
		}
		else {
			ButtonState = SDL_GameControllerGetButton(button[i].uuid, button[i].mapping);
		}
	}
}