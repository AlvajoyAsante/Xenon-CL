#ifndef OXY_SAVE_H
#define OXY_SAVE_H

#define OXY_APPVAR_NAME "OXYGEN"
#define OXY_VERSION "a0.0.58"

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Loads all systems to oxygen main app-var.
 * @return bool true if the "OXY_APPVAR_NAME" was found and false otherwise.
 */ 
bool oxy_LoadAll(void);
#define oxy_Begin() \
oxy_LoadAll()

/**
 * @brief Save all systems to oxygen main app-var.  
 */ 
void oxy_SaveAll(void);
#define oxy_End() \
oxy_SaveAll()

/**
 * @brief Init all systems mainly used in "oxy_LoadAll"
 */
// void oxy_InitAll(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_SAVE_H__ */