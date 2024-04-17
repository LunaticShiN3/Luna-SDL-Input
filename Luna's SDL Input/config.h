#include <ShlObj.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <stdint.h>

FILE* logfile;
FILE* configfile;

enum InputType {
	INPUTTYPE_KEYBOARD,
	INPUTTYPE_SDLGAMECONTROLLER,
	//more soon :3
};

enum AnalogType {
	ANALOG_DIGITAL,
	ANALOG_ANALOG,
};

typedef struct Button {
	enum InputType type;
	uint64_t uuid[2];
	byte mapping;
} Button;

typedef struct Modifier {
	enum InputType type;
	uint64_t uuid[2];
	byte mapping;
	float multiplierX;
	float multiplierY;
} Modifier;

typedef struct Controller {
	byte isConnected;
	enum AnalogType analogType[3];

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
	byte a2aRangeX;
	byte a2aRangeY;
	byte a2aDeadzone;
	byte a2aOuterEdge;
	byte a2aStickRangeClamping;
	//Digital to analog mapping
	byte d2aRangeCardinalX;
	byte d2aRangeCardinalY;
	byte d2aRangeDiagonalX;
	byte d2aRangeDiagonalY;
	//Analog to digital mapping
	byte a2dStickThreshold;
	byte a2dTriggerThreshold;

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