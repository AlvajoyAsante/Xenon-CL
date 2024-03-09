#include "save.h"
#include "settings.h"


void core_Save(void)
{
	ti_var_t slot;
	
	slot = ti_Open(CORE_APPVAR, "w");
	
	ti_Write(&core_data, sizeof(struct core_data_t), 1, slot); 	
	ti_Write(&core_settings, sizeof(struct core_settings_t), 1, slot);
	
	ti_SetArchiveStatus(1, slot);
	
	ti_Close(slot);
}

bool core_Load(void)
{
	ti_var_t slot;
	
	slot = ti_Open(CORE_APPVAR, "r");
	
	if (slot) {
		ti_Read(&core_data, sizeof(struct core_data_t), 1, slot);
		ti_Read(&core_settings, sizeof(struct core_settings_t), 1, slot);
		
		ti_Close(slot);
		return 1;
	} 
	
	return 0; 
}
