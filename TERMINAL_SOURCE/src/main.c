/*
 *--------------------------------------
 * Program Name: XENON CL
 * Author: ALVAJOY
 * License: MIT
 * Description: COMMAND INTERFACE FOR XENON (v1.0)
 *--------------------------------------
 */

#include <tice.h>

// Keep these headers
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Graphic lib's
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <compression.h>
#include <debug.h>
#include <fontlibc.h>
// #include "oxy/oxy_gui.h"

/* Structure for fonts */


struct terminal_info_t{
	uint8_t user_pos; 
	
	// command attributes
	char *PROGRAM_NAME;
	uint8_t FG, BG;
	char *BACKGROUND_IMAGE;
	char *CUR_DIR[10];
	
}terminal_info;

bool init_terminal(void); 
uint8_t init_users(void);
void new_user(void);
bool ret_user_data(void);
void save_user_data(void);
bool exuc_input(void);
void cmd_loop(void);

void main(void)
{
	gfx_Begin();
	gfx_SetDraw(1);

	if(init_terminal()) goto end;

terminal_input:
	cmd_loop();
	
end:
	gfx_End();
	return;
} 

bool init_terminal(void) // returns true if there is an error
{
	fontlib_font_t *font;
	char* words[14] = {"cat","dog","door","moon", "magic", "ice", "zipper", "trademan", "noiseAtDoor", "monkey", "cringe", "xenon", "admin"};
	gfx_FillScreen(0);
	
	/* Sets up the temp accounts for debuging */
	for (int i = 0; i < 5; i++){
		user_info[i].active = 1;
		user_info[i].username = words[randInt(0,14)];
	}
	save_user_data();

	gfx_SetTextFGColor(0xFF);
	gfx_SetTextBGColor(0x00);
	gfx_SetTextTransparentColor(254);

	/* Setting up the fonts via font-lib */

	ti_CloseAll();
	
	font = fontlib_GetFontByIndex("Gohufont", 1);
	
	if (font) {
		fontlib_SetFont(font, 0);
		font_info.char_width = fontlib_GetStringWidth(" ");
		font_info.char_height = fontlib_GetCurrentFontHeight();
		fontlib_SetColors(0xFF, 0x00); // Sets colors.
		fontlib_SetWindowFullScreen();
	} else {
		gfx_PrintStringXY("Font not found: ", 1, 1);
		gfx_PrintString("Gohufont");
        while(!kb_AnyKey()) kb_Scan();
		return true;
	}

	fontlib_DrawString("Xenon CL v1.0");
	fontlib_Newline();
	gfx_Blit(1);
	delay(100);

	ti_CloseAll();
	return false;
}

uint8_t init_users(void) 
{
	int user_amount = 0;
	int y, i;

	if(ret_user_data()){
		// fontlib_SetCursorPosition(1, 10);
		fontlib_DrawString("Choose a user:");
		fontlib_Newline();
		// fontlib_SetCursorPosition(10, y);
		
		for (i = 0; i < 10; i++){
			if (user_info[i].active){
				user_amount++;

				if (10 * i < 0){
					y = fontlib_GetCursorY();
				} else y += (font_info.char_height * i);

				fontlib_SetCursorPosition(10, fontlib_GetCursorY());
				fontlib_DrawString(user_info[i].username);
				fontlib_Newline();
			}
		}
		

		i = 0;
		y = 20;

		while (kb_Data[6] != kb_Enter){
			kb_Scan();

			gfx_SetColor(0);
			gfx_FillRectangle(0, 20, 8, font_info.char_height * user_amount);
			
			fontlib_SetCursorPosition(1, y);
			fontlib_DrawString("=");
			
			if  (kb_AnyKey()){
				if (kb_Data[7] == kb_Up){
					if (y > 20){
						y -= font_info.char_height;
						if (i > 0) i--;
					}
				}
				
				if (kb_Data[7] == kb_Down){
					if (y <  font_info.char_height * user_amount){
						y += font_info.char_height;
						if (i < user_amount) i++;
					}
				}
				
				delay(100);
			}

			if (kb_Data[6] == kb_Clear) return 2;

			gfx_Blit(1);
		}
		terminal_info.userpos = i;
		return 1;

	}else return 0; 
	// return;
}

int CheckforPassword(int pos) // checks if there is any password and make user type it in
{
main:
	if (user_info.active_pass){
		fontlib_Newline();
		fontlib_DrawString("Please Enter Password:");
		
		if (/* input */ != ){
			// check if the correct pass was inserted
			if (){
				fontlib_Newline();
				fontlib_DrawString("Incorrect Password! Please Try Again.");
				delay(100);
				goto main;
			}else{
				fontlib_Newline();
				fontlib_DrawString("Welcome! ");
				fontlib_DrawString(user_info.username);
				delay(100);
				return;
			}
	} return; // forced return by [clear]
	}else return;
}

void new_user(void)
{
	fontlib_Newline();
	// fontlib_SetCursorPosition(1, 1);
	fontlib_DrawString("Welcome to Xenon CL - A Xenon Terminal Interface.");
	gfx_Blit(1);
	
	fontlib_Newline();
	fontlib_DrawString("Please set a Username:");
	// input here
	gfx_Blit(1);
	
	fontlib_Newline();
	fontlib_DrawString("Do you want to set a password (y/n):");
	// input here
	gfx_Blit(1);
	
	fontlib_Newline();
	fontlib_DrawString("Please set your password:");
	// input here
	gfx_Blit(1);
	
	// Save all the input data in oxygen appvar
	return;
}

// Apply to oxygen.

bool ret_user_data(void)
{
	ti_var_t file;
	ti_CloseAll();
	
	file = ti_OpenVar("XE_USERS", "r", TI_APPVAR_TYPE);
	if (file) {
		ti_Read(&user_info, sizeof(user_info), 1, file);
		return 1;
	}
	return 0;
}

void save_user_data(void)
{
	ti_var_t file;
	ti_CloseAll();
	
	file = ti_OpenVar("XE_USERS", "w", TI_APPVAR_TYPE);
	
	ti_Write(&user_info, sizeof(user_info), 1, file);
	
	ti_SetArchiveStatus(true, file);
	
	ti_Close(file);
	return;
}

/* commands sections */
void cmd_loop(void)
{
	bool force_exit = 0;
	int command_pos;
	/* includes command input */
	do {
main_input:
		// input here
		command_pos = exuc_input();
		
		// checks for end or empty input data
		switch(command_pos){
			case 0: // empty or code not found
				if (!/*INPUT DATA*/) {
					goto main_input;
				}else{
					// COMMAND NOT RECONIZED
					fontlib_Newline();
					fontlib_DrawString();
					fontlib_DrawString(" , is not recognized!");
					fontlib_Newline();
					goto main_input;
				}
				
			break;
			
			case 1: // exit command
				goto end;
			break;
		}
		
		command_bank(command_pos);
	} while (force_exit != 1)

end:
	return;
}

/* command bank */
int exuc_input(void)
{
	// grab data from input.
	
	// check if it equals to something.
	return 0; // return command type 
}

void command_bank(int command_num) // Basically the command "runner"
{ 
	switch(command_num){
		case 2: // LS
		break;
		
		case 3: // CD DIRECTORY
		break;
		
		case 4: // CLS
		break;
		
		case 5: // RUN PROGRAM_NAME
		break;
		
		case 6: // DEL PROGRAM_NAME
		break;
		
		case 7: // ARC PROGRAM_NAME
		break;
		
		case 8: // UNARC PROGRAM_NAME
		break;
		
		case 9: // ICON PROGRAM_NAME
		break;
		
		case 10: // LOCK PROGRAM_NAME
		break;
		
		case 11: // DATE
		break;
		
		case 12: // RAND
		
		break;
		
		case 13: // HELP
		break;
		
		case 14: // ABOUT
			fontlib_Newline();
			fontlib_DrawString("Xenon CL v1.0, A slim and simple command line");
			fontlib_Newline();
		break;
		
		case 15: // ADDCMD APPVAR
		break;
		
		case 16: // SETCOLOR FG BG
		break;
		
		case 17: // PRGMINFO PROGRAM_NAME
		break;
		
		case 18: // EDITPRGM PROGRAM_NAME
		break;
		
		case 19: // CLRPRGMINFO PROGRAM_NAME
		break;
	}
	return;
}

/* commands functions */