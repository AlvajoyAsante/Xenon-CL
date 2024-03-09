#ifndef SETTINGS_H
#define SETTINGS_H

#define CORE_NAME "XENON CL"
#define CORE_VERSION "a0.0.28 `Pre-Release`"
#define CORE_COPYRIGHT "(C) 2018-2023 ALVAJOY ASANTE"
#define CORE_APPVAR "XENONCL"

#include <tice.h>

/**
 * Core data used to store information and simple temp data.
 */
struct core_data_t
{
	// Cursor Position
	int x_pos;
	int y_pos;

	// Used for returning info
	bool executed_file;
	char executed_name;

	// Oxygen filesystem data
	int folder_pos;
};
extern struct core_data_t core_data;


/**
 * Core settings used to store main data settings.
 */
struct core_settings_t
{
	/* Colors */
	uint8_t FG;
	uint8_t BG;
};
extern struct core_settings_t core_settings;


void core_CheckCommand(char *str);

int core_RunProgram(const uint8_t i);

/**
 * Always runs at the beginning of the program, It loads all data from Xenon app-var if not it creates a new user.
 */
void core_Setup(void);

/**
 * Gets the folder position based on Oxygen file system.
 * @retuns char* position in string form
 */
char *core_GetPos(void);


#endif /* SETTINGS_H */
