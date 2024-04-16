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
		switch (button[i].type) {
		case INPUTTYPE_KEYBOARD:
			ButtonState = keyboardState[button[i].mapping];
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			ButtonState = SDL_GameControllerGetButton(button[i].uuid, button[i].mapping);
			break;
		default:
			//idk
			break;
		}
	}
}