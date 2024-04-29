#include "sdl_input.h"

char* filePath[MAX_PATH];
int err;
SDL_GameController* controller[10]; //Support up to 10 SDL controllers
SDL_Joystick* joystick[10];

void SDLInit() {
	//Init SDL
	SDL_SetMainReady();
	SDLInit(SDL_INIT_GAMECONTROLLER);
	//Find gamecontrollerdb.txt
	GetModuleFileNameA(NULL, filePath, sizeof(filePath));
	PathRemoveFileSpecA(filePath);
	PathAppendA(filePath, "Plugin\\Input\\gamecontrollerdb.txt");
	SDL_GameControllerAddMappingsFromFile(filePath);
	//Get all (up to 10) connected controllers
	for (int i = 0; i < SDL_NumJoysticks; i++) {
		//too many controllers
		if (i >= 10) {
			break;
		}
		if (SDL_IsGameController(i) && (controller[i] = SDL_GameControllerOpen(i))) {
			joystick[i] = SDL_GameControllerGetJoystick(controller[i]);
		}

	}
}

void SDLDeinit() {
	SDL_Quit();
}

void SDLGetKeys() {
	keyboardState = SDL_GetKeyboardState;
}

byte GetButtonState(int Cont, Button* button) {
	int ButtonState = 0;
	int i;
	for (i = 0; i < 3; i++) {
		switch (button[i].type) {
		case INPUTTYPE_KEYBOARD:
			ButtonState = keyboardState[button[i].mapping];
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			//Digital to digital mapping
			if (button[i].mapping < 0x80) {
				ButtonState = SDL_GameControllerGetButton(/*Controller*/, button[i].mapping);
			}
			//Analog to digital mapping
			else {
				int AxisMapping = button[i].mapping - 0x80;
				if ((AxisMapping == SDL_CONTROLLER_AXIS_TRIGGERLEFT) || (AxisMapping = SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) {
					if (SDL_GameControllerGetAxis(/*Controller*/, AxisMapping) >= config.controller[Cont].a2dTriggerThreshold) {
						ButtonState = 1;
					}
				}
				else {
					if (SDL_GameControllerGetAxis(/*Controller*/, AxisMapping) >= config.controller[Cont].a2dStickThreshold) {
						ButtonState = 1;
					}
				}
			}
			break;
		default:
			//idk
			break;
		}
		if (ButtonState != 0) {
			break;
		}
	}
	return ButtonState;
}

byte GetModifierState(int Cont, Modifier* modifier, int id) {
	int ModifierState = 0;
	switch (modifier[id].type) {
	case INPUTTYPE_KEYBOARD:
		ModifierState = keyboardState[modifier[id].mapping];
		break;
	case INPUTTYPE_SDLGAMECONTROLLER:
		//Digital to digital mapping
		if (modifier[id].mapping < 0x80) {
			ModifierState = SDL_GameControllerGetButton(/*Controller*/, modifier[id].mapping);
		}
		//Analog to digital mapping
		else {
			int AxisMapping = modifier[id].mapping - 0x80;
			if ((AxisMapping == SDL_CONTROLLER_AXIS_TRIGGERLEFT) || (AxisMapping = SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) {
				if (SDL_GameControllerGetAxis(/*Controller*/, AxisMapping) >= config.controller[Cont].a2dTriggerThreshold) {
					ModifierState = 1;
				}
			}
			else {
				if (SDL_GameControllerGetAxis(/*Controller*/, AxisMapping) >= config.controller[Cont].a2dStickThreshold) {
					ModifierState = 1;
				}
			}
		}
		break;
	default:
		//idk
		break;
	}
	return ModifierState;
}

void GetAnalogState(int Cont, byte* AnalogX, byte* AnalogY) {
	float AnalogRight = 0;
	float AnalogLeft = 0;
	float AnalogDown = 0;
	float AnalogUp = 0;
	float AnalogCurrentX = 0;
	float AnalogCurrentY = 0;
	int DigitalToAnalogCount;

	int i;
	for (i = 0; i < 3; i++) {
		DigitalToAnalogCount = 0;

		//Analog Right
		switch (config.controller[Cont].AnalogRight[i].type) {
		case INPUTTYPE_KEYBOARD:
			if (keyboardState[config.controller[Cont].AnalogRight[i].mapping != 0]) {
				AnalogRight = 1.f;
				DigitalToAnalogCount += 1;
			}
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			//Digital to analog mapping
			if (config.controller[Cont].AnalogRight[i].mapping < 0x80) {
				if (SDL_GameControllerGetButton(/*Controller*/, config.controller[Cont].AnalogRight[i].mapping) != 0) {
					AnalogRight = 1.f;
					DigitalToAnalogCount += 1;
				}
			}
			//Analog to analog mapping
			else {
				if (SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogRight[i].mapping - 0x80) > 0) {
					AnalogRight = abs(SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogRight[i].mapping - 0x80));
					AnalogRight /= 32768.f;
				}
			}
			break;
		default:
			//idk
			break;
		}

		//Analog Left
		switch (config.controller[Cont].AnalogLeft[i].type) {
		case INPUTTYPE_KEYBOARD:
			if (keyboardState[config.controller[Cont].AnalogLeft[i].mapping != 0]) {
				AnalogLeft = 1.f;
				DigitalToAnalogCount += 1;
			}
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			//Digital to analog mapping
			if (config.controller[Cont].AnalogLeft[i].mapping < 0x80) {
				if (SDL_GameControllerGetButton(/*Controller*/, config.controller[Cont].AnalogLeft[i].mapping) != 0) {
					AnalogLeft = 1.f;
					DigitalToAnalogCount += 1;
				}
			}
			//Analog to analog mapping
			else {
				if (SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogLeft[i].mapping - 0x80) < 0) {
					AnalogLeft = abs(SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogLeft[i].mapping - 0x80));
					AnalogLeft /= 32768.f;
				}
			}
			break;
		default:
			//idk
			break;
		}

		//Analog down
		switch (config.controller[Cont].AnalogDown[i].type) {
		case INPUTTYPE_KEYBOARD:
			if (keyboardState[config.controller[Cont].AnalogDown[i].mapping != 0]) {
				AnalogDown = 1.f;
				DigitalToAnalogCount += 1;
			}
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			//Digital to analog mapping
			if (config.controller[Cont].AnalogDown[i].mapping < 0x80) {
				if (SDL_GameControllerGetButton(/*Controller*/, config.controller[Cont].AnalogDown[i].mapping) != 0) {
					AnalogDown = 1.f;
					DigitalToAnalogCount += 1;
				}
			}
			//Analog to analog mapping
			else {
				if (SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogDown[i].mapping - 0x80) > 0) {
					AnalogDown = abs(SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogDown[i].mapping - 0x80));
					AnalogDown /= 32768.f;
				}
			}
			break;
		default:
			//idk
			break;
		}

		//Analog Up
		switch (config.controller[Cont].AnalogUp[i].type) {
		case INPUTTYPE_KEYBOARD:
			if (keyboardState[config.controller[Cont].AnalogLeft[i].mapping != 0]) {
				AnalogUp = 1.f;
				DigitalToAnalogCount += 1;
			}
			break;
		case INPUTTYPE_SDLGAMECONTROLLER:
			//Digital to analog mapping
			if (config.controller[Cont].AnalogUp[i].mapping < 0x80) {
				if (SDL_GameControllerGetButton(/*Controller*/, config.controller[Cont].AnalogUp[i].mapping) != 0) {
					AnalogUp = 1.f;
					DigitalToAnalogCount += 1;
				}
			}
			//Analog to analog mapping
			else {
				if (SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogUp[i].mapping - 0x80) < 0) {
					AnalogUp = abs(SDL_GameControllerGetAxis(/*Controller*/, config.controller[Cont].AnalogUp[i].mapping - 0x80));
					AnalogUp /= 32768.f;
				}
			}
			break;
		default:
			//idk
			break;
		}

		//Digital to analog input handling
		if (DigitalToAnalogCount > 2) {
			if (AnalogLeft != 0) {
				AnalogCurrentX = -AnalogLeft;
			}
			else if (AnalogRight != 0) {
				AnalogCurrentX = AnalogRight;
			}
			else {
				AnalogCurrentX = 0;
			}
			if (AnalogDown != 0) {
				AnalogCurrentY = -AnalogDown;
			}
			else if (AnalogUp != 0) {
				AnalogCurrentY = AnalogUp;
			}
			else {
				AnalogCurrentX = 0;
			}

			if ((AnalogCurrentX != 0) && (AnalogCurrentY != 0)) {
				AnalogCurrentX *= config.controller[Cont].d2aRangeDiagonalX;
				AnalogCurrentY *= config.controller[Cont].d2aRangeDiagonalY;
			}
			else {
				AnalogCurrentX * config.controller[Cont].d2aRangeCardinalX;
				AnalogCurrentY * config.controller[Cont].d2aRangeCardinalY;
			}
		}
		//Analog to analog input handling
		else {
			AnalogCurrentX = AnalogRight - AnalogLeft;
			AnalogCurrentY = AnalogUp - AnalogDown;
			AnalogCurrentX *= config.controller[Cont].a2aRangeX;
			AnalogCurrentY *= config.controller[Cont].a2aRangeY;
			if (AnalogCurrentX <= config.controller[Cont].a2aDeadzone) {
				AnalogCurrentX = 0.f;
			}
			if (AnalogCurrentY <= config.controller[Cont].a2aDeadzone) {
				AnalogCurrentY = 0.f;
			}
			if (AnalogCurrentX >= config.controller[Cont].a2aOuterEdge) {
				AnalogCurrentX = 1.f;
			}
			if (AnalogCurrentY >= config.controller[Cont].a2aOuterEdge) {
				AnalogCurrentY = 1.f;
			}
		}
		//Things independent of a2a vs d2a
		byte ModifierState;
		for (i = 0; i < 50; i++) {
			if ((config.controller[Cont].modifiers[i].type != 0) && (config.controller[Cont].modifiers[i].mapping != 0)) {
				ModifierState = GetModifierState(Cont, config.controller[Cont].modifiers, i);
				if (ModifierState != 0) {
					AnalogCurrentX *= config.controller[Cont].modifiers[i].multiplierX;
					AnalogCurrentY *= config.controller[Cont].modifiers[i].multiplierY;
				}
			}
		}
		AnalogCurrentX *= 128;
		AnalogCurrentY *= 128;
		AnalogX = (int)AnalogCurrentX;
		AnalogY = (int)AnalogCurrentY;
	}
}


//Lunadinput stuff for reference on keyboard analog handling

/*if (deviceState[config.keybindLeft] >> 7) {
	integerX = -1;
}
else if (deviceState[config.keybindRight] >> 7) {
	integerX = 1;
}
else {
	integerX = 0;
}
if (deviceState[config.keybindDown] >> 7) {
	integerY = -1;
}
else if (deviceState[config.keybindUp] >> 7) {
	integerY = 1;
}
else {
	integerY = 0;
}

if (integerX != 0 && integerY != 0) {
	Keys->Y_AXIS = integerX * config.rangeDiagonalX;
	Keys->X_AXIS = integerY * config.rangeDiagonalY;
}
else {
	Keys->Y_AXIS = integerX * config.rangeCardinalX;
	Keys->X_AXIS = integerY * config.rangeCardinalY;
}

for (int i = 0; i < sizeof(config.modifiers) / sizeof(Modifier); i++) {
	if ((config.modifiers[i].keybind != 0) && (deviceState[config.modifiers[i].keybind] >> 7)) {
		Keys->Y_AXIS = (float)Keys->Y_AXIS * config.modifiers[i].multiplierX; //X and Y axis are swapped because of course they are????
		Keys->X_AXIS = (float)Keys->X_AXIS * config.modifiers[i].multiplierY;
	}
}*/