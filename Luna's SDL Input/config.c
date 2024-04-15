#include "config.h"
#include <time.h>
#include <stdarg.h>

#define LOG_SIZE 64000

//set FolderPath to %AppData%\Luna-Project64
void GetEmuFolderPath(char* FolderPath) {
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, FolderPath);
	PathAppendA(FolderPath, "Luna-Project64");
}

//write to log function (taken from Octomino)
void LogWrite(char* message) {
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);
    char buf[1024];

    char timestr[9];
    strftime(timestr, sizeof(timestr), "%X", timeinfo);

    fprintf(logfile, "[%s] ", timestr);

    va_list args;
    va_start(args, message);
    vfprintf(logfile, message, args);
    vsnprintf(buf, sizeof(buf), message, args);
    if (buf[0]) { strcat(buf, "\n"); }

    if ((strlen(buf) + strlen(message) + 1) >= LOG_SIZE) {
        memcpy(buf, buf + LOG_SIZE / 2, LOG_SIZE / 2); //don't forget to deal with the buffer overrun or I will
    }

    strcat(buf, message);
    va_end(args);

    fprintf(logfile, "\n");
}

void saveConfig(void) {

}

void loadConfig(void) {

}

void restoreDefaults(void) {

}