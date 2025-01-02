#include "oxygen.h"

#include <tice.h>
#include <fileioc.h>
#include <debug.h>

void oxy_SaveAll(void)
{
	ti_var_t slot;
	
	slot = ti_Open(OXY_APPVAR_NAME, "w");
	
	/* Notify System */
	ti_Write(&oxy_notify_system, sizeof(struct oxy_notify_system_t), 1, slot);
	if (oxy_notify_system.stack_amount) {
		ti_Write(oxy_notify, oxy_notify_system.stack_amount * sizeof(struct oxy_notify_t), 1, slot);
		free(oxy_notify);
	}

	/* User System */
	ti_Write(&oxy_user_system, sizeof(struct oxy_user_system_t), 1, slot);
	if (oxy_user_system.user_amount) {
		ti_Write(oxy_user, oxy_user_system.user_amount * sizeof(struct oxy_user_t), 1, slot);
		free(oxy_user);
	}

	/* Mouse System */
	ti_Write(&oxy_mouse, sizeof(struct oxy_mouse_t), 1, slot);
	if (oxy_mouse.hover_amount){
		ti_Write(oxy_detect, oxy_mouse.hover_amount * sizeof(struct oxy_detect_t), 1, slot);
		free(oxy_detect);
	}

	/* File System */
	ti_Write(&oxy_file_system, sizeof(struct oxy_file_system_t), 1, slot);
	if (oxy_file_system.numfolders) {
		ti_Write(oxy_folder, oxy_file_system.numfolders * sizeof(struct oxy_folders_t), 1, slot);
		free(oxy_folder);
	}
	if (oxy_file_system.numfiles) {
		ti_Write(oxy_file, oxy_file_system.numfiles * sizeof(struct oxy_files_t), 1, slot);
		free(oxy_file);
	}
	
	ti_SetArchiveStatus(1, slot);

	ti_Close(slot);
}

static void oxy_InitAll(void)
{	
	/* Notify System */
	oxy_DeleteAllNotify(); // Clears all the notifications.
	
	/* User System */
	oxy_NewUser("USER", NULL, 0); // Creates a user
	
	/* Mouse System */
	oxy_InitMouse(); // Init the mouse and sets it up
	
	/* File System */
	oxy_InitFilesStystem(); // There would be no need to setup the System if it already exist.
	oxy_DetectAllFiles(); // Detects all the files (must be done every time after program loads)
}

bool oxy_LoadAll(void) 
{
	ti_var_t slot;
	
	dbg_sprintf(dbgout, "Oxygen: Loading Systems...\n");
	
	if ((slot = ti_Open(OXY_APPVAR_NAME, "r"))) {
		
		/* Notify System */
		if (ti_Read(&oxy_notify_system, sizeof(struct oxy_notify_system_t), 1, slot)) { // All ways check for the system before reading
			oxy_notify = malloc(oxy_notify_system.stack_amount * sizeof(struct oxy_notify_t));
			ti_Read(oxy_notify, oxy_notify_system.stack_amount * sizeof(struct oxy_notify_t), 1, slot);
			dbg_sprintf(dbgout, "Oxygen: Loaded Notify System...\n");
		}
		
		/* User System */
		if (ti_Read(&oxy_user_system, sizeof(struct oxy_user_system_t), 1, slot)) {
			oxy_user = malloc(oxy_user_system.user_amount * sizeof(struct oxy_user_t));
			ti_Read(oxy_user, oxy_user_system.user_amount * sizeof(struct oxy_user_t), 1, slot);
			dbg_sprintf(dbgout, "Oxygen: Loaded User System...\n");
		}
		
		/* Mouse System */
		if (ti_Read(&oxy_mouse, sizeof(struct oxy_mouse_t), 1, slot)) {
			oxy_detect = malloc(oxy_mouse.hover_amount * sizeof(struct oxy_detect_t));
			ti_Read(oxy_detect, oxy_mouse.hover_amount * sizeof(struct oxy_detect_t), 1, slot);
			dbg_sprintf(dbgout, "Oxygen: Loaded Mouse System...\n");
		}
		
		/* File System */
		if (ti_Read(&oxy_file_system, sizeof(struct oxy_file_system_t), 1, slot)) {
			oxy_folder = malloc(oxy_file_system.numfolders * sizeof(struct oxy_folders_t));
			oxy_file = malloc(oxy_file_system.numfiles * sizeof(struct oxy_files_t));
			ti_Read(oxy_folder, oxy_file_system.numfolders * sizeof(struct oxy_folders_t), 1, slot);
			ti_Read(oxy_file, oxy_file_system.numfiles * sizeof(struct oxy_files_t), 1, slot);
			oxy_CheckFileSystem();
			dbg_sprintf(dbgout, "Oxygen: Loaded File System...\n");
		}
		
		ti_Close(slot);
		dbg_sprintf(dbgout, "Oxygen: Load Done!\n");
		return true;
		
	}
	
	dbg_sprintf(dbgout, "Oxygen: Save Appvar not Detected!\n");
	oxy_InitAll(); // Init All systems if appvar was not found.
	dbg_sprintf(dbgout, "Oxygen: Systems Setup Done!\n");
	
	return false;
}