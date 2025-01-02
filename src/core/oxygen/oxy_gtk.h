#ifndef OXY_GTK_H
#define OXY_GTK_H

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_NUM_TYPE		0
#define GTK_TEXT_TYPE		1
#define GTK_SPRITE_TYPE		2
#define GTK_BUTTON_TYPE		3
#define GTK_WINDOW_TYPE		4
#define GTK_DIVIDER_TYPE	5

/* Color of gtk objects */
struct gtk_color_t {
	uint8_t text_bg;
	uint8_t text_fg;
	
	uint8_t color_a;
	uint8_t color_b;
};

/* State of an object "derived from gtk" */
struct gtk_state_t {
	bool visible;
	bool clicked;
	bool clickable;
};

/* Array that contains Position and offsets */
struct gtk_position_t {
	int x;
    int y;
};

/* array that contains width and height data */
struct gtk_sizeinfo_t {
    int width;
    int height;
};

/* The widget is the base of the tree for displayable objects. */
struct gtk_widget_t {
	uint8_t type;
	
	struct gtk_color_t color;
	struct gtk_position_t position;
	struct gtk_sizeinfo_t size;
	struct gtk_state_t state;
	
	void (*update)(struct gtk_widget_t *);
	void (*render)(struct gtk_widget_t *);
    
	struct gtk_widget_t **child;
};

/* Window Array */
struct gtk_window_t {
	char *title;
	char *wm_class;
	
	struct gtk_widget_t widget;	
	struct gtk_color_t colors;
};

/* Window management system */
struct gtk_wm_t {
	uint8_t amount;
};
extern struct gtk_wm_t gtk_wm;


/**
 * Gets string input from user.
 * @param title Input text (text displayed before input).
 * @param x X coordinate
 * @param y Y coordinate
 * @param width Max input characters
 * @param height Max input characters
 * @returns gtk_window_t* Return users input
 */
struct gtk_window_t* oxy_CreateWindow(char title[9], int x, uint8_t y, int width, int height);

void oxy_UpdateWindow(struct gtk_window_t *window);

void oxy_RenderWindow(struct gtk_window_t *window);

void oxy_CenteredWindow(char *title, int width, int height);

void oxy_SetWindowColor(struct gtk_window_t *window, uint8_t boarder_color, uint8_t center_color);

void oxy_SetWindowTextColor(struct gtk_window_t *window, uint8_t text_fg, uint8_t text_bg);

void oxy_Message(char *title, char *message);

void oxy_PrintWordWrap(char *text, uint24_t x, uint8_t y, int width, uint8_t max_lines, uint8_t init_line);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GTK_H__ */