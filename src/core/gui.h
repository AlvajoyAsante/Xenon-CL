#ifndef GUI_H
#define GUI_H

#include <tice.h>


/*
* Renders the boot screen on set up.
*/
void core_RenderBoot(void);

/*
* Renders the terminal loop aka the home screen
*/
void core_RenderHome(void);

/*
* Resets and clears the terminal
*/
void core_Reset(void);


/*
* Shows a maunal for a certain command.
* @param str command name
*/
void core_ShowManual(char* str);


/*
* Scrolls the terminal screen by a certain amount and updates the cursors position.
* @param amount amount you want to scroll multiply by 10
*/
void core_Scroll(uint8_t amount);

/*
* Check if scroll is needed before scrolling based users command.
* @param str command name
*/
void core_ManageScroll(char* str);

#endif /* __GUI_H__ */
