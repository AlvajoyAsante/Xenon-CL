#include "oxy_gfx.h"
#include "oxy_gtk.h"

#include <tice.h>
#include <debug.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <ctype.h>
#include <fileioc.h>

uint8_t _getWordWidth(char *word);
bool _iscntrl(int c);

// Windows
struct gtk_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height)
{
	struct gtk_window_t *window = NULL;
	struct gtk_widget_t widget;
	
	strncpy(window->title, title, 9);
	strncpy(window->wm_class, "PARENT", 9);
	
	widget.type = GTK_WINDOW_TYPE;
	widget.position.x = x;
	widget.position.y = y;
	widget.size.width = width;
	widget.size.height = height;
	widget.state.visible = true;
	widget.child = NULL;
	
	window->widget = widget;
	
	return window;
}

void oxy_UpdateWindow(struct gtk_window_t *window)
{	
    if (window->widget.state.visible) {
        // if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
			
		// }
    }
}

void oxy_RenderWindow(struct gtk_window_t *window)
{	
	if (window->widget.state.visible) {
		int x = window->widget.position.x;
		int y = window->widget.position.y;
		uint16_t width = window->widget.size.width;
		uint8_t height = window->widget.size.height;
		
		/* Title Area */
		gfx_SetColor(window->colors.color_a);
		oxy_FillRoundRectangle(x, y, width, height + 12, 0);
		
		gfx_SetTextBGColor(window->colors.color_a);
		gfx_SetTextFGColor(window->colors.text_fg);
		gfx_SetTextTransparentColor(0);
		gfx_PrintStringXY(window->title, 160 - gfx_GetStringWidth(window->title) / 2, y + (12 / 2) - 3);
		
		/* Widget Area */
		gfx_SetColor(window->colors.color_b);
		oxy_FillRoundRectangle(x + 1, y + 12 + 1, width - 2, height - 2, 0);
	}
}

void oxy_CenteredWindow(char *title, int width, int height)
{
	oxy_RenderWindow(oxy_CreateWindow(title, 160 - width / 2, 120 - (height + 13) / 2, width, height));
}

// Window Colors
void oxy_SetWindowColor(struct gtk_window_t *window, uint8_t boarder_color, uint8_t center_color)
{
	window->widget.color.color_a = boarder_color;
	window->widget.color.color_b = center_color;
}

void oxy_SetWindowTextColor(struct gtk_window_t *window, uint8_t text_fg, uint8_t text_bg)
{
	window->widget.color.text_fg = text_fg;
	window->widget.color.text_bg = text_bg;
}

// 	Message & Dialogue
void oxy_Message(char *title, char *message) 
{	
	const int width = 100;
	const int height = 80;
	
    uint8_t numlines = 7;
		
    const uint8_t titlewidth = 12;
    uint8_t padding = 3;
	
	uint8_t xprint = 160 - (100 + 2 * padding) / 2;
    uint8_t yprint;
    
	if (title != NULL) {
		yprint = 120 - (height + 13) / 2;
		oxy_CenteredWindow(title, width, height);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + 1, width, numlines, 0);
	}else{
		yprint = 120 - (numlines * 3 + titlewidth + 2 * padding) / 2;
		oxy_FillRoundRectangle(xprint, yprint, width, height, 0);
		oxy_PrintWordWrap(message, xprint + 2 * padding, yprint + titlewidth + padding + 1, width, numlines, 0);
	}
	
	gfx_Blit(1);
	while (!os_GetCSC());	
}

/**
 * Implemented from "Captain-Calc/textioc"
 * This function works on the assumption that the longest word is <256 characters long
 */
uint8_t _getWordWidth(char *word) 
{
	char *c = word;
	uint8_t width = 0;

	while (!isspace(*c) && !_iscntrl(*c) && width < 240) 
		width += gfx_GetCharWidth(*c++);
	
	return width;
}

bool _iscntrl(int c) 
{
	if (c == '\0' || c == '\n' || c == '\t') {
		return 1;
	}else return 0;
}

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line)
{
	char *curr_char = text;
	uint24_t curr_line_width = 0;
	int word_width = 0;
	uint8_t curr_line_num = init_line;
	uint8_t i;
	
	// Debugging
	gfx_SetColor(224);
	gfx_FillRectangle(x + width, 0, 2, LCD_HEIGHT);
	
	if (gfx_GetStringWidth(text) <= (unsigned int)width) {
		gfx_SetTextXY(x, y);
		gfx_PrintString(text);
		return;
	};
	
	// If the width is way bigger than the LCD_Width
	if (width > LCD_WIDTH) width = LCD_WIDTH;
	
	dbg_sprintf(dbgout, "Current Line: 1 -------------\n");
	
	for ( ; ; ) {

		if (curr_line_num > max_lines || y > LCD_HEIGHT)
			return;

		gfx_SetTextXY(x, y + (curr_line_num - init_line) * 4);
		curr_line_width = 0;

		while (*curr_char != '\0') {

			// Get the width of the next word
			word_width = _getWordWidth(curr_char);
			dbg_sprintf(dbgout, "word_width = %d | Word: ", word_width);

			// If there is room on the current line for the word, print the word
			// else, start a new line
			if (curr_line_width + word_width < (unsigned int)width) {
				curr_line_width += word_width;
				while (!isspace(*curr_char) && !iscntrl(*curr_char)) {
					dbg_sprintf(dbgout, "%c", *curr_char);
					gfx_PrintChar(*curr_char++);
				};
				dbg_sprintf(dbgout, "\n");
			} else {
				dbg_sprintf(dbgout, "Cannot put word on current line. Breaking...\n");
				goto startLine;
			};

			if (*curr_char == '\0') return;

			switch (*curr_char++) {
				case '\t':
					dbg_sprintf(dbgout, "Handling horizontal tab...\n");
					if (curr_line_width + (4 * gfx_GetCharWidth(' ')) < (unsigned int)width) {
						for (i = 1; i < 4; i++) {
							gfx_PrintChar(' ');
							curr_line_width += gfx_GetCharWidth(' ');
						};
					} else {
						dbg_sprintf(dbgout, "Cannot put tab on current line. Breaking...\n");
						goto startLine;
					};
					break;
				
				case '\n':
					dbg_sprintf(dbgout, "Handling newline...\n");
					goto startLine;
				
				case '\v':
					dbg_sprintf(dbgout, "Handling vertical tab...\n");
					goto startLine;
				
				case ' ':
					dbg_sprintf(dbgout, "Handling space...\n");
					gfx_PrintChar(*(curr_char - 1));
					curr_line_width += gfx_GetCharWidth(' ');
					break;
			};

		};

		startLine:
		
		curr_line_num++;
		dbg_sprintf(dbgout, "Line: %d -------------\n", curr_line_num);
	};
}
