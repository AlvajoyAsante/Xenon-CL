#include "oxy_save.h"
#include "oxy_notify.h"
#include "oxy_gfx.h"

#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>
#include <time.h>

struct oxy_notify_t *oxy_notify;
struct oxy_notify_system_t oxy_notify_system;


void oxy_NewNotify(gfx_sprite_t *icon, char title[9], char text[30])
{
	struct oxy_notify_t *curr_index;
	
	oxy_notify = realloc(oxy_notify, oxy_notify_system.stack_amount++ * sizeof(struct oxy_notify_t));
	curr_index = &oxy_notify[oxy_notify_system.stack_amount - 1];
	
    strncpy(curr_index->title, title, 9);
    strncpy(curr_index->text, text, 30);
	
	if (icon != NULL) {
		curr_index->icon = icon; // set index icon to pointer
	}else curr_index->icon = NULL; 
	
}

void oxy_DeleteNotify(uint8_t index)
{
	if (oxy_notify_system.stack_amount > 0){
        //overwrite the element to be deleted with the last element and resize the array
        oxy_notify[index] = oxy_notify[oxy_notify_system.stack_amount - 1];
        oxy_notify = realloc(oxy_notify, oxy_notify_system.stack_amount-- * sizeof(struct oxy_notify_t));
    }
}

void oxy_DeleteAllNotify(void)
{
	oxy_notify = realloc(oxy_notify, sizeof(struct oxy_notify_t));
	oxy_notify_system.stack_amount = 0;
}

int oxy_CheckNotify(void)
{
	if (!(oxy_notify_system.stack_amount)) return -1;
	
	for (int i = 0; i < oxy_notify_system.stack_amount; i++) {
		return i;
	}
	
	return -1;
}

void oxy_AlertNotify(int index)
{
	if (index == -1) return;
	
	struct oxy_notify_t *curr_notify;
	gfx_sprite_t *back_buff;
	uint8_t xprint;
	uint16_t yprint = 15;
	
	curr_notify = &oxy_notify[index];
	back_buff = gfx_MallocSprite(204, 40);
	gfx_GetSprite(back_buff, 61, 15);

	gfx_SetColor(0);
	
	if (curr_notify->icon != NULL){
		xprint = (320 - 205)/2;
		oxy_FillRoundRectangle(xprint, yprint, 205, 40, 0);
		
		gfx_SetColor(255);
		oxy_FillRoundRectangle(xprint, yprint + 1, 38, 38, 0);
		
		/* place prog name here */
		gfx_SetColor(10);
		oxy_FillRoundRectangle(xprint + 40, yprint + 1, 163, 14, 0);
		
		oxy_WhiteText(10);
	
		if (curr_notify->title[0] != '\0'){
			gfx_PrintStringXY(curr_notify->title, 102, 19);
		}
		
		/* place text here */ 
		gfx_SetColor(255);
		oxy_FillRoundRectangle(101, 31, 163, 23, 0);
		
		oxy_BlackText(255);
		
		if (curr_notify->text[0] != '\0'){
			gfx_PrintStringXY(curr_notify->text, 102, yprint + 18);
		}
	}else{
		xprint = (320 - 165)/2;
		oxy_FillRoundRectangle(xprint, yprint, 165, 40, 0);
		
		/* place prog name here */ 
		gfx_SetColor(10);
		oxy_FillRoundRectangle(xprint + 1, yprint + 1, 163, 14, 0);
		
		oxy_WhiteText(10);
	
		if (curr_notify->title[0] != '\0'){
			gfx_PrintStringXY(curr_notify->title, xprint + 2, yprint + 3);
		}
		
		/* place text here */ 
		gfx_SetColor(255);
		oxy_FillRoundRectangle(xprint + 1, yprint + 16, 163, 23, 0);
		
		oxy_BlackText(255);
		
		if (curr_notify->text[0] != '\0'){
			gfx_PrintStringXY(curr_notify->text, xprint + 2, yprint + 18);
		}
	}
	
	gfx_Blit(1);

	while (!os_GetCSC());
	
	oxy_DeleteNotify(index);
	
	gfx_Sprite(back_buff, 61, 15);
	free(back_buff);
}