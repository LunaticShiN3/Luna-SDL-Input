#include "sdl_input.h"

char* filePath[MAX_PATH];
int err;
SDL_Gamepad* controller[10]; //Support up to 10 SDL controllers
SDL_JoystickID* joystick;
char* MappedGUIDString[33];
char* GamepadGUIDString[33];
int GamepadEnum;
SDL_Gamepad* CurrentGamepad;
int DigitalToAnalogCount;

void SDLInit() {
	//Init SDL
	SDL_SetMainReady();
	SDLInit(SDL_INIT_GAMEPAD);
	//Find gamecontrollerdb.txt
	GetModuleFileNameA(NULL, filePath, sizeof(filePath));
	PathRemoveFileSpecA(filePath);
	PathAppendA(filePath, "Plugin\\Input\\gamecontrollerdb.txt");
	SDL_AddGamepadMappingsFromFile(filePath);
	//Get all (up to 10) connected controllers
	int count = 10;
	joystick = SDL_GetGamepads(&count);
	SDL_OpenGamepad(joystick);
	


	for (int i = 0; i < SDL_GetJoysticks; i++) {
		//too many controllers
		if (i >= 10) {
			break;
		}
		if (SDL_IsGamepad(i) && (controller[i] = SDL_OpenGamepad(i))) {
			joystick[i] = SDL_GetGamepadJoystick(controller[i]);
		}
	}
}

void SDLDeinit() {
	SDL_Quit();
}

void SDLGetKeys() {
	keyboardState = SDL_GetKeyboardState;
}

SDL_Gamepad* GetCurrentController(SDL_JoystickGUID guid, byte contEnum) {
	SDL_GUIDToString(guid, MappedGUIDString, sizeof(MappedGUIDString));
	GamepadEnum = 0;
	int i;
	for (i = 0; i < 10; i++) {
		SDL_GUIDToString(SDL_GetGamepadInstanceGUID(i), GamepadGUIDString, sizeof(GamepadGUIDString));
		if (GamepadGUIDString == MappedGUIDString) {
			if (GamepadEnum == contEnum) {
				return controller[i];
			}
			else {
				GamepadEnum++;
			}
		}
	}
	
}

byte SDLAnalogToDigital(int Cont, byte Mapping, byte Sign) {
	if ((Mapping == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) || (Mapping = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)) {
		if (Sign * (SDL_GetGamepadAxis(CurrentGamepad, Mapping)) >= config.controller[Cont].a2dTriggerThreshold) {
			return 1;
		}
	}
	else {
		if (Sign * (SDL_GetGamepadAxis(CurrentGamepad, Mapping)) >= config.controller[Cont].a2dStickThreshold) {
			return 1;
		}
	}
	return 0;
}

byte GetButtonState(int Cont, Button* button) {
	int ButtonState = 0;
	int i;
	for (i = 0; i < 3; i++) {
		switch (button[i].type) {
		case INPUTTYPE_KEYBOARD:
			ButtonState = keyboardState[button[i].mapping];
			break;
		case INPUTTYPE_SDLGAMEPAD_BUTTON:
			//Get controller to be used
			CurrentGamepad = GetCurrentController(button[i].guid, button[i].contEnum);
			//Digital to digital mapping
			if (button[i].mapping < 0x80) {
				ButtonState = SDL_GetGamepadButton(CurrentGamepad, button[i].mapping);
			}
			break;
		case INPUTTYPE_SDLGAMEPAD_AXIS_POSITIVE:
			//Get controller to be used
			CurrentGamepad = GetCurrentController(button[i].guid, button[i].contEnum);
			//Analog to digital mapping (positive)
			ButtonState = SDLAnalogToDigital(Cont, button[i].mapping, +1);
			break;
		case INPUTTYPE_SDLGAMEPAD_AXIS_NEGATIVE:
			//Get controller to be used
			CurrentGamepad = GetCurrentController(button[i].guid, button[i].contEnum);
			//Analog to digital mapping (negative)
			ButtonState = SDLAnalogToDigital(Cont, button[i].mapping, -1);
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
	case INPUTTYPE_SDLGAMEPAD_BUTTON:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(modifier[id].guid, modifier[id].contEnum);
		//Digital to digital mapping
		ModifierState = SDL_GetGamepadButton(CurrentGamepad, modifier[id].mapping);
		break;
	case INPUTTYPE_SDLGAMEPAD_AXIS_POSITIVE:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(modifier[id].guid, modifier[id].contEnum);
		//Analog to digital mapping (positive)
		ModifierState = SDLAnalogToDigital(Cont, modifier[id].mapping, +1);
		break;
	case INPUTTYPE_SDLGAMEPAD_AXIS_NEGATIVE:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(modifier[id].guid, modifier[id].contEnum);
		//Analog to digital mapping (negative)
		ModifierState = SDLAnalogToDigital(Cont, modifier[id].mapping, -1);
	default:
		//idk
		break;
	}
	return ModifierState;
}

float GetAnalogDirection(int Cont, Button* Direction, int i) {
	DigitalToAnalogCount = 0;
			switch (Direction[i].type) {
	case INPUTTYPE_KEYBOARD:
		if (keyboardState[config.controller[Cont].AnalogRight[i].mapping != 0]) {
			DigitalToAnalogCount += 1;
			return 1.f;
		}
		break;
	case INPUTTYPE_SDLGAMEPAD_BUTTON:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(Direction[i].guid, Direction[i].contEnum);
		//Digital to analog mapping
		if (SDL_GetGamepadButton(CurrentGamepad, Direction[i].mapping) != 0) {
			DigitalToAnalogCount += 1;
			return 1.f;
		}
		break;
	case INPUTTYPE_SDLGAMEPAD_AXIS_POSITIVE:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(Direction[i].guid, Direction[i].contEnum);
		//Analog to analog mapping (positive)
		if (SDL_GetGamepadAxis(CurrentGamepad, Direction[i].mapping) > 0) {
			return (abs(SDL_GetGamepadAxis(CurrentGamepad, Direction[i].mapping))) / 32768.f;
		}
		break;
	case INPUTTYPE_SDLGAMEPAD_AXIS_NEGATIVE:
		//Get controller to be used
		CurrentGamepad = GetCurrentController(Direction[i].guid, Direction[i].contEnum);
		//Analog to analog mapping (negative)
		if (SDL_GetGamepadAxis(CurrentGamepad, Direction[i].mapping) < 0) {
			return (abs(SDL_GetGamepadAxis(CurrentGamepad, Direction[i].mapping))) / 32768.f;
		}
	default:
		//idk
		break;
	}
}

void GetAnalogState(int Cont, byte* AnalogX, byte* AnalogY) {
	float AnalogRight = 0;
	float AnalogLeft = 0;
	float AnalogDown = 0;
	float AnalogUp = 0;
	float AnalogCurrentX = 0;
	float AnalogCurrentY = 0;

	int i;
	for (i = 0; i < 3; i++) {
		DigitalToAnalogCount = 0;

		AnalogRight = GetAnalogDirection(Cont, config.controller[Cont].AnalogRight, i);
		AnalogLeft = GetAnalogDirection(Cont, config.controller[Cont].AnalogLeft, i);
		AnalogDown = GetAnalogDirection(Cont, config.controller[Cont].AnalogDown, i);
		AnalogUp = GetAnalogDirection(Cont, config.controller[Cont].AnalogUp, i);

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

		//Highest magnitude input for each axis takes priority
		if (abs((int)AnalogCurrentX) > abs(AnalogX)) {
			AnalogX = (int)AnalogCurrentX;
		}
		if (abs((int)AnalogCurrentX) > abs(AnalogX)) {
			AnalogY = (int)AnalogCurrentY;
		}
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