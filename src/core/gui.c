#include "gui.h"
#include "data.h"
#include "oxygen/oxygen.h"

#include <tice.h>
#include <graphx.h>
#include <string.h>
#include <debug.h>


char* core_GetPos(void);

void core_RenderBoot(void)
{
	gfx_FillScreen(0);
	
	gfx_SetTextTransparentColor(254);
	gfx_SetTextBGColor(0);
	gfx_SetTextFGColor(255);
	
	gfx_SetColor(7);
	for(int i = 0; i < 49; i++){
		gfx_FillRectangle(103, (LCD_HEIGHT - ((i*2) + 1))/2, 33, (i*2) + 1);
		gfx_FillRectangle(144, (LCD_HEIGHT - ((i*2) + 1))/2, 33, (i*2) + 1);
		gfx_FillRectangle(185, (LCD_HEIGHT - ((i*2) + 1))/2, 33, (i*2) + 1);
		delay(10);
		gfx_Blit(1);
	}
	
	
	for (int i = 0; i < 9; i++){
		gfx_FillRectangle(111, (LCD_HEIGHT - ((97 +(i*2)) + 1))/2, 17, 97 + (i*2) + 1);
		gfx_FillRectangle(152, (LCD_HEIGHT - ((97 +(i*2)) + 1))/2, 17, 97 + (i*2) + 1);
		gfx_FillRectangle(193, (LCD_HEIGHT - ((97 +(i*2)) + 1))/2, 17, 97 + (i*2) + 1);
		delay(30);
		gfx_Blit(1);
	}
	
	delay(50);

	gfx_PrintStringXY(CORE_NAME, (LCD_WIDTH - gfx_GetStringWidth(CORE_NAME))/2, 193);
	gfx_PrintStringXY(CORE_VERSION, (LCD_WIDTH - gfx_GetStringWidth(CORE_VERSION))/2, 205);
	gfx_Blit(1);
	
	delay(500);
	
	for (int i = 0; i < 22; i++){
		gfx_ShiftUp(10);
		gfx_Blit(1);
	}
	
}

void core_RenderHome(void)
{	
	core_Reset();
	
	while (!oxy_stringinput.forced_exit) {
		gfx_SetTextFGColor(core_settings.FG);
		gfx_SetTextBGColor(core_settings.BG);
		
		gfx_SetColor(core_settings.BG);
		core_CheckCommand(oxy_StringInput(core_GetPos(), 1, core_data.y_pos, 20));
		
		core_data.y_pos += 10;
	}
}

void core_Scroll(uint8_t amount)
{
	for (int i = 0; i < amount; i++){
		if (core_data.y_pos != 0) {
			core_data.y_pos -= 10;
			gfx_ShiftUp(10);
		}else return;
		gfx_Blit(1);
	}
}

void core_ManageScroll(char* str)
{
	if (oxy_stringinput.forced_exit) return;
	
	if (!strcasecmp(str, "CLS") || !strcasecmp(str, "EXIT") || !strcasecmp(str, "EXEC")) return;
	
	if (core_data.y_pos + 10 >= 220) core_Scroll(11);
	
	if (!strcasecmp(str, "HELP")) {
		if (core_data.y_pos + 150 >= 220) core_Scroll(15);
		return;
	}
	
	if (!strncasecmp(str, "WHATIS", 6)) {
		if (core_data.y_pos + 40 >= 220) core_Scroll(6);
		return;
	}
}

void core_ShowManual(char* str)
{
	gfx_SetTextFGColor(7);
	
	switch (str[0]) {
		case 'E':
			if (!strncmp(str, "EXIT", 4)) {
				gfx_PrintStringXY("EXIT, Exits the terminal", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "EXEC", 4)) {
				gfx_PrintStringXY("EXEC 'FL', Runs a file in folder", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "EDIT", 4)){
				gfx_PrintStringXY("EDIT 'FL', Edit a file in dir", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'P':
			if (!strncmp(str, "PUSHDIR", 7) || !strncmp(str, "PUSHD", 5)) {
				gfx_PrintStringXY("PUSHDIR, Move into a dir", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "POPDIR", 6) || !strncmp(str, "POPD", 4)) {
				gfx_PrintStringXY("POPDIR, Return to the previous dir", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "PWD", 3)) {
				gfx_PrintStringXY("PWD, Tell your current dir", 1, core_data.y_pos += 10);
				return;
			}
		
		case 'C':
			if (!strncmp(str, "CLS", 3)) {
				gfx_PrintStringXY("CLS, Clears and resets the terminal.", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "CP", 2)) {
				gfx_PrintStringXY("CP 'FL/DIR' 'DIR', Copies file/dir to dir", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "CD", 2)) {
				gfx_PrintStringXY("CD 'DIR', Move into dir (Similar to `PUSHDIR)", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "COLOR", 5)) {
				gfx_PrintStringXY("COLOR 'INDEX', Set the text color", 1, core_data.y_pos += 10);
				return;
			}
		
		case 'M':
			if (!strncmp(str, "MV", 2)) {
				gfx_PrintStringXY("MV 'FL/FD' 'DIR', Move file or folder to a dir", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "MAN", 3)) {
				gfx_PrintStringXY("MAN 'CMD', Shows help for a command", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "MKDIR", 5)) {
				gfx_PrintStringXY("MKDIR 'FL', Make a folder at curr dir", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'W':
			if (!strncmp(str, "WHATIS", 6)) {
				gfx_PrintStringXY("WHATIS 'FL', Shows the attrib of a file", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'L':
			if (!strncmp(str, "LS", 2)) {
				gfx_PrintStringXY("LS, List the files in the current dir", 1, core_data.y_pos += 10);
				return;
			}
			
			if (!strncmp(str, "LIST", 4)) {
				gfx_PrintStringXY("LIST, List all command", 1, core_data.y_pos += 10);
				return;
			}
			
		case 'R':
			if (!strncmp(str, "RM", 2)) {
				gfx_PrintStringXY("RM 'FL', Delete a file", 1, core_data.y_pos += 10);
				return;
			}
		
		default:
			if (core_data.y_pos + 30 >= 220) core_Scroll(4);
			gfx_SetTextFGColor(224);
			gfx_PrintStringXY("ERROR: COMMAND MANUAL NOT FOUND!", 1, core_data.y_pos += 10);
			gfx_PrintStringXY("TRY TO EXECUTE `HELP`!", 1, core_data.y_pos += 10);
			break;
	}
	
}

void core_Reset(void)
{
	gfx_ZeroScreen();
	gfx_SetTextXY(0,0);
	gfx_PrintString(CORE_NAME);
	gfx_PrintString(" - ");
	gfx_PrintString(CORE_VERSION);
	
	gfx_PrintStringXY("TYPE 'HELP' FOR COMMANDS.", 1, 10);
	
	core_data.y_pos = 20;
}