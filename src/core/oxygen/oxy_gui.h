#ifndef OXY_GUI_H
#define OXY_GUI_H

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Used in string input.
 *
 */
struct oxy_stringinput_t {
    int charsamount; 
    char *text; 
    uint8_t type;
	bool forced_exit;
	uint8_t max_char;

    // History
    bool enable_history;
    char *history[256];
    uint8_t history_pos;
    uint8_t history_size;

    // Autofill
    bool enable_autofill;
    char *autofill[500];
    int autofill_size;
};
extern struct oxy_stringinput_t oxy_stringinput;

// String input.
/**
 * Gets string input from user.
 * @param title Input text (text displayed before input).
 * @param x X coordinate
 * @param y Y coordinate
 * @param maxchar Max input characters
 * @returns char Return users input
 */
char* oxy_StringInput(const char title[], uint16_t x, uint8_t y, uint8_t maxchar);

void oxy_ResetStringInputHistory(void);
void oxy_StringInput_AddToHistory(const char *input);

void oxy_ResetStringInputAutofill(void);
void oxy_StringInput_AddToAutofill(const char *input);


// Others.
/**
 * Draws a color picker at x, y.
 * @param cur_select Preslected color (values range from 0-255)
 * @param x X coordinate
 * @param y Y coordinate
 */
uint8_t oxy_ColorPicker(uint8_t cur_select, uint16_t x, uint8_t y);

/**
 * Prints Current Time at x, y.
 * @param x X coordinate
 * @param y Y coordinate
 */
void oxy_PrintTime(uint16_t x, uint8_t y);

/**
 * Prints Current Date at x, y.
 * @param x X coordinate
 * @param y Y coordinate
 */
void oxy_PrintDate(uint16_t x, uint8_t y);

/**
 * Prints Battery status Icon at x, y.
 * @param x X coordinate
 * @param y Y coordinate
 */
void oxy_PrintBatteryStatus(uint16_t x, uint8_t y);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_GUI_H__ */