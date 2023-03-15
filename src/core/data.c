#include "core.h"
#include "core/oxygen/oxygen.h"

#include <tice.h>
#include <graphx.h>
#include <string.h>
#include <keypadc.h>
#include <debug.h>

struct core_data_t core_data;
struct core_settings_t core_settings;

int core_prgm_ret(void *data, int retval);
char* core_GetTypeText(uint8_t type);
bool core_EditProgram(const uint8_t i, const char* editor);


void core_Setup(void)
{
	gfx_SetDrawBuffer();
	gfx_SetTextTransparentColor(254);
	
	if (!core_Load()) {
		core_InitData();
		core_RenderBoot();
	}
	
	gfx_SetTextFGColor(core_settings.FG);
	gfx_SetTextBGColor(core_settings.BG);
	
	dbg_sprintf(dbgout, "CL: Setup Finished!\n");
}

void core_InitData(void)
{
	/* Text Print Position */
	core_data.x_pos = core_data.y_pos = 0;
		
	/* Colors */
	core_settings.FG = 255;
	core_settings.BG = 0;
	
	/* Username Information */
	core_data.folder_pos = core_data.curr_user_id = core_data.logged_in = 0;
}


char* core_GetPos(void)
{	
	char *pos;
	pos = (char*)malloc(sizeof(oxy_user[core_data.curr_user_id].name) + sizeof(oxy_folder[core_data.folder_pos].name));
	pos[0] = '\0';
	
	strcat(pos, oxy_user[core_data.curr_user_id].name);
	if (core_data.folder_pos >= 0) {
		strcat(pos, ":");
		strcat(pos, oxy_folder[core_data.folder_pos].name);
		strcat(pos, ">");
	}else{
		strcat(pos, "~");
	}
	
	return pos;
}


static bool _IsFolderInFolder(const int fld_a, const int fld_b)
{
	int fld_b_pos = oxy_folder[fld_b].position;
	int i = fld_b;
	
	if (fld_a == fld_b_pos){
		return 1;
	}
	
	
	while (i != fld_a) {
		if (i < 0) return 0;
		i = oxy_folder[i].position;
	}
	
	return 1;
}

static bool core_ApproveCommand(char * question)
{
	char* str;
	
	str = oxy_StringInput(question, 1, core_data.y_pos += 10, 1);
	
	if (!strncasecmp(str, "Y", 1)) return 1;
	
	return 0;
}

void core_CheckCommand(char *str)
{
	core_ManageScroll(str);
	
	if (oxy_stringinput.forced_exit) return;
	
	switch (str[0]) {
		case 'V':
		case 'v':
			if (!strcasecmp(str, "VER") || !strcasecmp(str, "VERSION")) {
				if (core_data.y_pos + 30 >= 220) core_Scroll(4); 
				
				gfx_SetTextFGColor(231);
				gfx_PrintStringXY(CORE_COPYRIGHT, 1, core_data.y_pos += 10);
				
				gfx_SetTextFGColor(30);
				gfx_SetTextXY(1, core_data.y_pos += 10);
				gfx_PrintString(CORE_NAME);
				gfx_PrintString(": ");
				gfx_PrintString(CORE_VERSION);
				
				gfx_SetTextXY(1, core_data.y_pos += 10);
				gfx_PrintString("OXYGEN: ");
				gfx_PrintString(OXY_VERSION);
				return;
			}
		
		case 'H':
		case 'h':
			if (!strcasecmp(str, "HELP")) {
				int main_pos = core_data.y_pos;
				uint8_t page = 0;
				
				if (core_data.y_pos + 150 >= 220){
					core_Scroll(15); 
					main_pos = core_data.y_pos;
				} 
				
				help_loop:
				gfx_SetColor(core_settings.BG);
				gfx_FillRectangle(1, main_pos + 10, LCD_WIDTH, 150);
				
				core_data.y_pos = main_pos;
				gfx_SetTextFGColor(30);
				
				switch (page){
					case 0:
						gfx_PrintStringXY("CP 'FL' 'DIR', Copy file to dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("CD 'DIR', Move to dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("COLOR 'INDEX', Set the text color", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("DATE, Show date and time", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("DF, Show ram and archive usage", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("EXEC 'FL', Runs a file in folder", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("EXIT, Exits the terminal", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("EDIT 'FL', Edit a file in dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("LS, List the files in the current dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("MV 'FL' 'DIR', Move file or folder to a dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("MKDIR 'FL', Make a folder at curr dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("MAN 'CMD', Show the manual for a command", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("PUSHDIR, Move into a dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("POPDIR, Return to the previous dir", 1, core_data.y_pos += 10);
						break;
						
					case 1:
						gfx_PrintStringXY("PWD, Tell your current dir", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("RM 'FL', Delete a file", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("RMDIR 'DIR', Delete a dir/folder", 1, core_data.y_pos += 10);
						gfx_PrintStringXY("WHATIS 'FL', Shows the attrib of a file", 1, core_data.y_pos += 10);
						break;
				}
				
				gfx_SetTextFGColor(7);
				gfx_SetTextXY(1, main_pos + 140);
				gfx_PrintString("Page: ");
				gfx_PrintUInt(page + 1, 2);
				gfx_PrintString("  <,> - CHANGE PAGE   [CLEAR] - STOP");
				core_data.y_pos += 10;
				
				gfx_Blit(1);
				
				while (!(kb_Data[6] & kb_Clear)){
					kb_Scan();
					
					if (kb_Data[7] & kb_Right && page < 1) {
						page++;
						goto help_loop;
					}
					
					if (kb_Data[7] & kb_Left && page > 0) {
						page--;
						goto help_loop;
					}
				}
				
				core_data.y_pos = main_pos + 140;
				while (kb_AnyKey()) kb_Scan();
				
				return;
			}
		
		case 'E':
		case 'e':
			if (!strcasecmp(str, "EXIT")) {
				oxy_stringinput.forced_exit = 1;
				return;
			}
			
			if (!strncasecmp(str, "EXEC", 4)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " "); 
				
				gfx_SetTextFGColor(224);
				if (token == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `EXEC`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); 
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token)) {
						if (oxy_file[i].location == core_data.folder_pos) {
							if (oxy_GetFileType(oxy_file[i].type) != TI_APPVAR_TYPE) {
								core_RunProgram(i);
							}else{
								gfx_PrintStringXY("ERROR: FILE IS NOT RUNABLE!", 1, core_data.y_pos += 10);
								return;
							}
						}else{
							gfx_PrintStringXY("ERROR: FILE NOT IN DIR!", 1, core_data.y_pos += 10);
							return;
						}
					}
				}
				
				gfx_PrintStringXY("ERROR: FILE NOT FOUND!", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncasecmp(str, "EDIT", 4)) {
				char *token[2];
				
				strtok(str, " ");
				token[0] = strtok(NULL, " ");
				token[1] = strtok(NULL, " ");
				
				gfx_SetTextFGColor(224);
				if (token[0] == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `EDIT`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token[0]) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token[0]);
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				if (token[1] != NULL){
					if (strlen(token[1]) > 8) {
						gfx_SetTextXY(1, core_data.y_pos += 10);
						gfx_PrintString("ERROR:'");
						gfx_PrintString(token[1]);
						gfx_PrintString("'");
						gfx_PrintString(" MAX LENGTH IS 8!");
						return;
					}
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token[0])) {
						if (oxy_file[i].location == core_data.folder_pos) {
							if (token[1] != NULL) {
									core_EditProgram(i, token[1]);
									gfx_PrintStringXY("ERROR: EDITOR NOT FOUND!", 1, core_data.y_pos += 10);
									return;
							}
							
							if (oxy_GetFileType(oxy_file[i].type) == TI_PRGM_TYPE) {
								#if XENON_CL_DEVELOPER
									core_EditProgram(i, NULL);
									gfx_PrintStringXY("ERROR: CANNOT EDIT FILE!", 1, core_data.y_pos += 10);
								#else
									gfx_PrintStringXY("ERROR: YOU CANT EDIT IN THIS VERSION!", 1, core_data.y_pos += 10);
								#endif
								return;
							}else{	
								gfx_PrintStringXY("ERROR: FILE IS NOT BASIC!", 1, core_data.y_pos += 10);
								return;
							}
						}
					}
				}
				
				gfx_PrintStringXY("ERROR: CANNOT FIND FILE!", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'P':
		case 'p':
			if (!strncasecmp(str, "PUSHDIR", 7) || !strncasecmp(str, "PUSHD", 5)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " "); // find next input
				
				gfx_SetTextFGColor(224);
				if (token == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `PUSHDIR`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfolders; i++) {
					if (!strcmp(oxy_folder[i].name, token)) {
						core_data.folder_pos = i;
						return;	
					}
				}
				
				gfx_PrintStringXY("ERROR: FOLDER NOT FOUND!", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncasecmp(str, "POPDIR", 6) || !strncasecmp(str, "POPD", 4)) {
				gfx_SetTextFGColor(224);
				if (core_data.folder_pos > -1) {
					core_data.folder_pos = oxy_folder[core_data.folder_pos].position;
				}else{
					gfx_PrintStringXY("ERROR: IN ROOT DIR!", 1, core_data.y_pos += 10);
				}
				return;
			}
			
			if (!strcasecmp(str, "PWD")) {
				gfx_SetTextFGColor(7);
				gfx_SetTextXY(1, core_data.y_pos += 10);
				
				if (core_data.folder_pos < 0) {
					gfx_PrintString("~/");
					return;
				}
				
				if (oxy_folder[core_data.folder_pos].position < 0){
					gfx_PrintString(oxy_folder[core_data.folder_pos].name);
					gfx_PrintString("/");
					return;
				}
				
				for (int i = 0; i < core_data.folder_pos + 1; i++){
					if (_IsFolderInFolder(i, core_data.folder_pos)){
						gfx_PrintString(oxy_folder[i].name);
						gfx_PrintString("/");
					}
				}
				return;
			}
		
		case 'C':
		case 'c':
			if (!strcasecmp(str, "CLS")) {
				gfx_ZeroScreen();
				core_data.y_pos = -10;
				return;
			}
		
			if (!strncasecmp(str, "CP", 2)) {
				char *token[2];
				bool found = false;
				int pos;
				
				strtok(str, " ");
				token[0] = strtok(NULL, " ");
				token[1] = strtok(NULL, " "); 
				
				gfx_SetTextFGColor(224);
				if (token[0] == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `CP`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token[0]) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token[0]); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				if (token[1] == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `FL`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token[1]) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token[1]); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token[0])) {
						if (oxy_file[i].location == core_data.folder_pos) {
							found = 1;
							pos = i;
						}
					}
				}
				
				if (!found){
					for (int i = 0; i < oxy_file_system.numfolders; i++) {
						if (!strcmp(oxy_folder[i].name, token[0])) {
							if (oxy_folder[i].position != core_data.folder_pos) {
								gfx_SetTextFGColor(224);
								gfx_PrintStringXY("ERROR: FILE NOT FOUND!", 1, core_data.y_pos += 10);
								return;
							}else pos = i;
						}
					}
				}
				
				for (int i = 0; i < oxy_file_system.numfolders; i++) {
					if (!strcmp(oxy_folder[i].name, token[1])) {
						if (found){ // Tries to copy a file.
							if (oxy_file[pos].location != i){
								gfx_SetTextFGColor(7);
								oxy_AddFile();
								oxy_file[oxy_file_system.numfolders] = (struct oxy_files_t)oxy_file[pos];
								oxy_file[oxy_file_system.numfolders].location = i;
								gfx_PrintStringXY("SUCCESS: FILE COPIED!", 1, core_data.y_pos += 10);
							}else{
								gfx_PrintStringXY("ERROR: CAN'T COPY INTO THE SAME DIR!", 1, core_data.y_pos += 10);
							}
						}else{ // Tries to copy a folder.
							if (oxy_folder[pos].position != i) {
								gfx_SetTextFGColor(7);
								oxy_AddFolder(oxy_folder[pos].name, i);
								gfx_PrintStringXY("FOLDER COPIED!", 1, core_data.y_pos += 10);
							}else{
								gfx_PrintStringXY("ERROR: CAN'T COPY INTO THE SAME DIR!", 1, core_data.y_pos += 10);
							}
						}
						return;
					}
				}
				
				return;
			}
			
			if (!strncasecmp(str, "CD", 2)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " "); // find next input
				
				gfx_SetTextFGColor(224);
				if (token == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `CD`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				if (!strcmp(token, "..")) {
					if (core_data.folder_pos > -1) {
						core_data.folder_pos = oxy_folder[core_data.folder_pos].position;
						return;
					}else{
						gfx_PrintStringXY("ERROR: IN ROOT DIR!", 1, core_data.y_pos += 10);
					}
				}
				
				for (int i = 0; i < oxy_file_system.numfolders; i++) {
					if (!strcmp(oxy_folder[i].name, token) && oxy_folder[i].position == core_data.folder_pos){
						core_data.folder_pos = i;
						return;
					}
				}
				
				gfx_SetTextFGColor(224);
				gfx_PrintStringXY("ERROR: FOLDER NOT FOUND!", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncasecmp(str, "COLOR", 5)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " "); 
				
				gfx_SetTextFGColor(224);
				
				if (token != NULL) {
					if (atoi(token) != 0) {
						core_settings.FG = atoi(token);
					}else{
						gfx_PrintStringXY("ERROR: # WAS NOT DETECTED!", 1, core_data.y_pos += 10);
					}
				}else{
					gfx_PrintStringXY("ERROR: EXPECTED `INDEX` AFTER `COLOR`!", 1, core_data.y_pos += 10);
				}
				
				return;
			}
		
		case 'D':
		case 'd':
			if (!strcasecmp(str, "DATE")) {
				gfx_SetTextFGColor(30);
				core_data.y_pos += 10;
				oxy_PrintDate(1, core_data.y_pos);
				oxy_PrintTime(100, core_data.y_pos);
				return;
			}
			
			if (!strcasecmp(str, "DF")) {
				if (core_data.y_pos + 20 > 220) core_Scroll(3);
				
				gfx_SetTextFGColor(30);
				gfx_SetTextXY(1, core_data.y_pos += 10);
				gfx_PrintString("RAM-Free: ");
				gfx_PrintInt(os_MemChk(NULL), 1);
	
				gfx_SetTextXY(1, core_data.y_pos += 10);
				gfx_PrintString("ARC-Free: ");
				os_ArcChk();
				gfx_PrintInt(os_TempFreeArc, 1);
				return;
			}
			
		case 'M':
		case 'm':
			if (!strncasecmp(str, "MV", 2)) {
				char *token[2];
				bool found = false;
				int pos;
				
				strtok(str, " ");
				token[0] = strtok(NULL, " ");
				token[1] = strtok(NULL, " "); 
				
				gfx_SetTextFGColor(224);
				if (token[0] == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `MV`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token[0]) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token[0]); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				if (token[1] == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `FL`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token[1]) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token[1]); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token[0])) {
						if (oxy_file[i].location == core_data.folder_pos) {
							found = 1;
							pos = i;
						}
					}
				}
				
				if (!found){
					for (int i = 0; i < oxy_file_system.numfolders; i++) {
						if (!strcmp(oxy_folder[i].name, token[0])) {
							if (oxy_folder[i].position != core_data.folder_pos) {
								gfx_SetTextFGColor(224);
								gfx_PrintStringXY("ERROR: FILE NOT FOUND!", 1, core_data.y_pos += 10);
								return;
							}else pos = i;
						}
					}
				}
				
				for (int i = 0; i < oxy_file_system.numfolders; i++) {
					if (!strcmp(oxy_folder[i].name, token[1])) {
							if (found){
								oxy_file[pos].location = i;
							}else{
								oxy_folder[pos].position = i;
							}
							
							gfx_SetTextFGColor(7);
							gfx_PrintStringXY("FILE MOVED!", 1, core_data.y_pos += 10);
							return;
					}
				}
				
				return;
			}
			
			if (!strncasecmp(str, "MAN", 3)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " ");
				
				if (token == NULL) {
					gfx_SetTextFGColor(224);
					gfx_PrintStringXY("ERROR: EXPECTED `CMD` AFTER `MAN `!", 1, core_data.y_pos += 10);
				}else{
					core_ShowManual(token);
				}
				
				return;
			}
			
			if (!strncasecmp(str, "MKDIR", 5)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " ");
				
				if (token == NULL){
					gfx_SetTextFGColor(224);
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `MKDIR`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextFGColor(224);
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" MAX LENGTH IS 8!");
					return;
				}
				
				if (oxy_AddFolder(token, core_data.folder_pos)) {
					gfx_SetTextFGColor(7);
					gfx_PrintStringXY("SUCCESS: DIR CREATED!", 1, core_data.y_pos += 10);
					return;
				}else{
					gfx_SetTextFGColor(224);
					gfx_PrintStringXY("ERROR: DIR CONNOT BE CREATED!", 1, core_data.y_pos += 10);
				}
				return;
			}
			
		case 'W':
		case 'w':
			if (!strncasecmp(str, "WHATIS", 6)) {
				char *token ;
				
				strtok(str, " ");
				token = strtok(NULL, " ");
				
				if (token == NULL){
					gfx_SetTextFGColor(224);
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `WHATIS`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextFGColor(224);
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" IS NOT A FILE NAME!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token) && oxy_file[i].location == core_data.folder_pos){
						// Icon
						if (oxy_file[i].icon != NULL) {
							gfx_PrintStringXY("ICON: ", 90 + gfx_GetStringWidth(oxy_file[i].name), core_data.y_pos + 10);
							gfx_Sprite(oxy_file[i].icon, 130+gfx_GetStringWidth(oxy_file[i].name), core_data.y_pos + 10);
						}
						
						// Name
						gfx_SetTextFGColor(255);
						gfx_SetTextXY(1, core_data.y_pos += 10);
						gfx_PrintString("NAME: ");
						gfx_SetTextFGColor(30);
						gfx_PrintString(oxy_file[i].name);
						
						// Type
						gfx_SetTextFGColor(255);
						gfx_SetTextXY(1, core_data.y_pos += 10);
						gfx_PrintString("TYPE: ");
						gfx_PrintString(core_GetTypeText(oxy_file[i].type));
						
						// Size
						gfx_SetTextFGColor(255);
						gfx_SetTextXY(1, core_data.y_pos += 10);
						gfx_PrintString("SIZE: ");
						gfx_SetTextFGColor(30);
						gfx_PrintInt(oxy_file[i].size, 5);
						
						// Description 
						gfx_SetTextFGColor(255);
						gfx_SetTextXY(1, core_data.y_pos += 10);
						
						if (oxy_file[i].description != NULL) {
							gfx_PrintString("DESC: ");
							gfx_SetTextFGColor(30);
							gfx_PrintString(oxy_file[i].description);
						}else{
							gfx_PrintString("DESC: ");
							gfx_SetTextFGColor(30);
							gfx_PrintString("NO DESCRIPTION!");
						}
							
						return;
					}
				}
				
				gfx_SetTextFGColor(224);
				gfx_PrintStringXY("ERROR: FILE NOT FOUND!", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'L':
		case 'l':
			if (!strcasecmp(str, "LS")) {
				int fileamt, folderamt, k;
				folderamt = fileamt = k = 0;
				
				for (int i = 0; i < oxy_file_system.numfolders + oxy_file_system.numfiles; i++) {
					if (i < oxy_file_system.numfolders){
						// List folders
						if (oxy_folder[i].position == core_data.folder_pos) {
							
							if (k % 4 == 0) {
								if (core_data.y_pos + 10 >= 220) {
									core_Scroll(2);
								}
								core_data.y_pos += 10;
								k = 0;
							}
							
							gfx_SetTextFGColor(25);
							gfx_PrintStringXY(oxy_folder[i].name, 1 + (k * 72), core_data.y_pos);
							folderamt++;
							k++;
						}
					}else{
						// List Programs
						if (oxy_file[i - oxy_file_system.numfolders].location == core_data.folder_pos) {
							
							if (k % 4 == 0) {
								if (core_data.y_pos + 10 >= 220) {
									core_Scroll(2);
								}
								core_data.y_pos += 10;
								k = 0;
							}
							
							gfx_SetTextFGColor(31);
							gfx_PrintStringXY(oxy_file[i - oxy_file_system.numfolders].name, 1 + (k * 72), core_data.y_pos);
							fileamt++;
							k++;
						}
					}
				}
				
				gfx_SetTextFGColor(224);
				if (!folderamt && !fileamt) {
					gfx_PrintStringXY("NO FILES...", 1 + (k * 72), core_data.y_pos += 10);
				}else{
					if (folderamt && !fileamt){
						gfx_PrintStringXY("NO FILES...", 1 + (k * 72), core_data.y_pos);
					}
				}
				
				return;
			}
			
		case 'R':
		case 'r':
			if (!strncasecmp(str, "RMDIR", 5)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " ");
				
				gfx_SetTextFGColor(224);
				if (token == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `DIR` AFTER `RMDIR`!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" IS NOT A FILE NAME!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfolders; i++) {
					if (!strcmp(oxy_folder[i].name, token)) {
						if (oxy_folder[i].position == core_data.folder_pos) {
							if (core_data.y_pos + 30 > 220) core_Scroll(4);
							gfx_PrintStringXY("DELETING DIR WILL DELETE ALL FILES INSIDE!", 1, core_data.y_pos += 10);
							if (core_ApproveCommand("ARE YOU WANT TO DELETE?(Y/N) ")){
								oxy_Delete(true, i); 
								gfx_SetTextFGColor(7);
								gfx_PrintStringXY("SUCCESS: FOLDER DELETED!", 1, core_data.y_pos += 10);
							}
							return;
						}else{
							gfx_PrintStringXY("ERROR: FOLDER NOT IN DIR!", 1, core_data.y_pos += 10);
							return;
						}
					}
				}
			}else if (!strncasecmp(str, "RM", 2)) {
				char *token;
				
				strtok(str, " ");
				token = strtok(NULL, " ");
				
				gfx_SetTextFGColor(224);
				if (token == NULL){
					gfx_PrintStringXY("ERROR: EXPECTED `FL` AFTER `RM `!", 1, core_data.y_pos += 10);
					return;
				}else if (strlen(token) > 8) {
					gfx_SetTextXY(1, core_data.y_pos += 10);
					gfx_PrintString("ERROR:'");
					gfx_PrintString(token); // update this later
					gfx_PrintString("'");
					gfx_PrintString(" IS NOT A FOLDER NAME!");
					return;
				}
				
				for (int i = 0; i < oxy_file_system.numfiles; i++) {
					if (!strcmp(oxy_file[i].name, token)) {
						if (oxy_file[i].location == core_data.folder_pos) {
							if (core_data.y_pos + 20 > 220) core_Scroll(3);
							if (core_ApproveCommand("ARE YOU WANT TO DELETE?(Y/N) ")) {
								if (oxy_Delete(false, i)) {
									gfx_SetTextFGColor(7);
									gfx_PrintStringXY("SUCCESS: FILE DELETED!", 1, core_data.y_pos += 10);
								}else{
									gfx_PrintStringXY("ERROR: FILE WAS NOT DELETED!", 1, core_data.y_pos += 10);
								}
								
							}
							return;
						}
					}
				}
			}
			
		default:
			gfx_SetTextFGColor(224);
			
			for (int i = 0; i < oxy_file_system.numfiles; i++) {
				if (!strcmp(oxy_file[i].name, str)) {
					if (oxy_file[i].location == core_data.folder_pos) {
						if (oxy_GetFileType(oxy_file[i].type) != TI_APPVAR_TYPE) {
							core_RunProgram(i);
						}else{
							gfx_PrintStringXY("ERROR: FILE IS NOT RUNABLE!", 1, core_data.y_pos += 10);
							return;
						}
					}else{
						gfx_PrintStringXY("ERROR: FILE NOT IN DIR!", 1, core_data.y_pos += 10);
						return;
					}
				}
			}
		
			
			gfx_PrintStringXY("ERROR: COMMAND NOT RECOGNIZED!", 1, core_data.y_pos += 10);
			break;
			
		case '\0':
		case ' ':
			break;
	}
}

char* core_GetTypeText(const uint8_t type)
{
	switch (type){
		case OXY_BASIC_TYPE: // Basic Program
			gfx_SetTextFGColor(226);
			return "BASIC";
		
		case OXY_PBASIC_TYPE: // Protected Basic Programs
			gfx_SetTextFGColor(225);
			return "PROTECTED BASIC";
		
		case OXY_ASM_TYPE: // ASM
			gfx_SetTextFGColor(255);
			return "ASM";
		
		case OXY_C_TYPE: // C
			gfx_SetTextFGColor(7);
			return "C";
		
		case OXY_ICE_TYPE: // ICE
			gfx_SetTextFGColor(22);
			return "ICE";
		
		case OXY_ICES_TYPE: // ICE Source
			gfx_SetColor(25);
			return "ICE SOURCE CODE";
			
		case OXY_APPVAR_TYPE: // App-var
			gfx_SetTextFGColor(230);
			return "APPVAR";
			
		default:
			gfx_SetTextFGColor(224);
			return "NOT DETECTED!";
	}
}

bool core_EditProgram(const uint8_t i, const char* editor)
{
	/*
	* Saves any data after running the shell.
	*/
	core_SaveData();
	
	/*
	* Saves all Oxygen's systems.
	*/
	oxy_End();
	
	while (kb_AnyKey()) kb_Scan();
	
	if (editor != NULL){
		return oxy_EditPrgm(oxy_file[i].name, editor, core_prgm_ret);
	}else{
		return oxy_EditPrgm(oxy_file[i].name, NULL, NULL);
	}
	
}

int core_RunProgram(const uint8_t i)
{
	/*
	* Saves any data after running the shell.
	*/
	core_SaveData();
	
	/*
	* Saves all Oxygen's systems.
	*/
	oxy_End();
	
	while (kb_AnyKey()) kb_Scan();
	
	/*
	* End of CE C Graphical Setup.
	*/
	gfx_End();
	
	return os_RunPrgm(oxy_file[i].name, NULL, 0, core_prgm_ret);
}

int core_prgm_ret(void *data, int retval)
{
	dbg_sprintf(dbgout,"core executed program.\n");
    dbg_sprintf(dbgout,"retval: %d", retval);
    (void)data;
	
	gfx_Begin();
	oxy_Begin(); 
	
	core_Setup(); 
	core_RenderHome();
	core_SaveData();
	
	oxy_End();
	gfx_End();
	
	return 0;
}