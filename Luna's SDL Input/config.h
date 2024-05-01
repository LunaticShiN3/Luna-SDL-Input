#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL3\SDL.h>
#include <SDL3\SDL_main.h>

FILE* logfile;
FILE* configfile;

typedef enum InputType {
	INPUTTYPE_KEYBOARD,
	INPUTTYPE_SDLGAMEPAD_BUTTON,
	INPUTTYPE_SDLGAMEPAD_AXIS_POSITIVE,
	INPUTTYPE_SDLGAMEPAD_AXIS_NEGATIVE,
	//more soon :3
} InputType;

typedef struct Button {
	InputType type;
	SDL_JoystickGUID guid;
	byte contEnum;
	byte mapping;
} Button;

typedef struct Modifier {
	InputType type;
	SDL_JoystickGUID guid;
	byte contEnum;
	byte mapping;
	float multiplierX;
	float multiplierY;
} Modifier;

typedef struct Controller {
	byte isConnected;

	//N64 Buttons
	Button DpadRight[3];
	Button DpadLeft[3];
	Button DpadDown[3];
	Button DpadUp[3];
	Button Start[3];
	Button Z[3];
	Button B[3];
	Button A[3];
	Button CRight[3];
	Button CLeft[3];
	Button CDown[3];
	Button CUp[3];
	Button R[3];
	Button L[3];

	//N64 Analog stick
	Button AnalogRight[3];
	Button AnalogLeft[3];
	Button AnalogDown[3];
	Button AnalogUp[3];

	//Analog to analog mapping
	float a2aRangeX;
	float a2aRangeY;
	float a2aDeadzone;
	float a2aOuterEdge;
	//Digital to analog mapping
	float d2aRangeCardinalX;
	float d2aRangeCardinalY;
	float d2aRangeDiagonalX;
	float d2aRangeDiagonalY;
	//Analog to digital mapping
	float a2dStickThreshold;
	float a2dTriggerThreshold;

	//Analog stick modifiers
	Modifier modifiers[50];
} Controller;

typedef struct Config {
	int ConfigVersion;
	Controller controller[4];
} Config;

Config config;

void GetEmuFolderPath(char* FolderPath);
void LogWrite(char* message);
void saveConfig(void);
void loadConfig(void);
void restoreDefaults(void);