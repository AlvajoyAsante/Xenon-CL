#ifndef DATA_H
#define DATA_H

#define CORE_NAME "XENON CL"
#define CORE_VERSION "a0.0.27 `Pre-Release`"
#define CORE_COPYRIGHT "(C) 2018-2023 ALVAJOY ASANTE"
#define CORE_APPVAR "XENONCL"

/**
 * Core data used to store information and simple temp data.
 */
struct core_data_t { 
    uint8_t clipboard[2];
	
	// Cursor Positionn
	int x_pos;
	int y_pos;
	
	// Used for returning info
	bool executed_file;
	char executed_name;
	
	// Oxygen filesystem data
	uint8_t curr_user_id;
	bool logged_in;
	int folder_pos;
};
extern struct core_data_t core_data;

/**
 * Core settings used to store main data settings.
 */
struct core_settings_t {
    /* Colors */
    uint8_t FG;
    uint8_t BG;
};
extern struct core_settings_t core_settings;

/**
 * Used to Initial data on Xenon first boot.
 */
void core_InitData(void);

/**
* Always runs at the beginning of the program, It loads all data from Xenon app-var if not it creates a new user.
*/
void core_Setup(void);

/*
* Gets the folder position based on Oxygen file system.
* @retuns char* position in string form
*/
char* core_GetPos(void);

/*
* Check command name at the begning of the input string before running.
*/
void core_CheckCommand(char *str);

/**
* Runs a set program in oxygen's file system.
*/
int core_RunProgram(uint8_t i);

#endif /* __DATA_H__ */
