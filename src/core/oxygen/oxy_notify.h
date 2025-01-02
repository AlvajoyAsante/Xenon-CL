#ifndef OXY_NOTIFY_H
#define OXY_NOTIFY_H

#include <tice.h>
#include <graphx.h>

#define oxy_BlackText(a) \
gfx_SetTextFGColor(0); \
gfx_SetTextBGColor(a);

#define oxy_WhiteText(a) \
gfx_SetTextFGColor(255); \
gfx_SetTextBGColor(a);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Notifications structure file.  
 */ 
struct oxy_notify_t{
	gfx_sprite_t *icon;
	char title[9];
	char text[30];
};
extern struct oxy_notify_t *oxy_notify;

/**
 * @brief Notification stack system (manages amount of notifications).  
 */ 
struct oxy_notify_system_t{
	uint8_t stack_amount;
};
extern struct oxy_notify_system_t oxy_notify_system;

/**
 * Creates a new notification.
 * @param title Title of the notification (Name of Program).
 * @param text Text or dialog of the notifications.
 * @param time Use oxy_GetNotifyTime();
 * @param date Use oxy_GetNotifyDate();
 */
void oxy_NewNotify(gfx_sprite_t *icon, char title[9], char text[30]);

/**
 * Deletes Notification out of a stack,
 * @param index POS or index of Notification.
 */
void oxy_DeleteNotify(uint8_t index);

/**
 * @brief This function deletes all the notification in notification stack.
 */ 
void oxy_DeleteAllNotify(void);

/**
 * @brief This function is used to check if a notify wants to be displayed.
 */ 
int oxy_CheckNotify(void);

/**
 * @brief This is the default function for displaying a notification,
 * there is other ways to implement display notifications.
 */ 
void oxy_AlertNotify(int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_NOTIFY_H__ */