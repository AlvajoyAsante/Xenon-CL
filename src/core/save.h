#ifndef SAVE_H
#define SAVE_H

#include <tice.h>
#include <fileioc.h>

/*
 * Save all core data and settings.
 */
void core_Save(void);

/*
 * Loads all core data and settings and re-scans files if needed.
 * @returns bool return true if there was a file found.
 */
bool core_Load(void);

#endif /* __SAVE_H__ */
