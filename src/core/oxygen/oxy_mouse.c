#include "oxy_mouse.h"
#include "gfx/oxy_sprites.h"

#include <tice.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>

struct oxy_detect_t *oxy_detect;
struct oxy_mouse_t oxy_mouse;
int hover_index;


void oxy_InitMouse(void)
{
	oxy_mouse.x = (LCD_WIDTH - cursorA_width)/ 2;
	oxy_mouse.y = (LCD_HEIGHT - cursorA_height)/ 2;
	
	oxy_mouse.speed = 4;
	
	oxy_mouse.scroll_X = oxy_mouse.scroll_Y = oxy_mouse.hover_amount = 0;
	oxy_mouse.clicked_index = -2;
}

void oxy_ResetMouse(void)
{
	oxy_mouse.scroll_X = oxy_mouse.scroll_Y = 0;
	oxy_mouse.clicked_index = -2;
}


uint8_t oxy_AddHover(uint16_t x, uint8_t y, uint16_t w, uint8_t h)
{
	struct oxy_detect_t *curr_detect;

	oxy_mouse.hover_amount++;
	oxy_detect = realloc(oxy_detect, oxy_mouse.hover_amount * sizeof(struct oxy_detect_t)); // add to hover_amount here
	curr_detect = &oxy_detect[oxy_mouse.hover_amount - 1];
	
	// sets data.
	curr_detect->x = x;
	curr_detect->y = y;
	curr_detect->w = w;
	curr_detect->h = h;
	
	
	curr_detect->right_click = curr_detect->right_arg = curr_detect->left_click = curr_detect->left_arg = NULL;
	curr_detect->description = NULL;
	curr_detect->data = 0;
	
	return oxy_mouse.hover_amount - 1; // Returns the index "ptr" to hoverspot
}

bool oxy_RemoveHover(uint8_t index)
{
	if (oxy_mouse.hover_amount > 0){
        oxy_detect[index] = oxy_detect[index - 1];
        oxy_detect = realloc(oxy_detect, --oxy_mouse.hover_amount * sizeof(struct oxy_detect_t));
		return 1;
    } else return 0;
}

void oxy_RemoveAllHover(void)
{
	oxy_detect = realloc(oxy_detect, sizeof(struct oxy_detect_t));
	oxy_mouse.hover_amount = 0;
}

void oxy_RemoveAllBelow(uint8_t index)
{
	uint8_t size = oxy_mouse.hover_amount;
	
	if (index + 1 > size) return;
	
	for(int i = index + 1; i < size; i++){
		oxy_RemoveHover(i);
	}
}

void oxy_RemoveAllAbove(uint8_t index)
{	
	if (index - 1 < 0) return;
	
	for(int i = index - 1; i > 0; i--){
		oxy_RemoveHover(i);
	}
}

bool oxy_SetHoverDescription(char *text, uint8_t index)
{
	if (oxy_mouse.hover_amount > 0){
		oxy_detect[index].description = text;
		return 1;
	}
	
	return 0;
}

int oxy_ReturnClickedIndex(void)
{
	return oxy_mouse.clicked_index;
}

void oxy_SetRightClick(void *function, void *arg, uint8_t index)
{
	uint8_t size = oxy_mouse.hover_amount;
	
	if (index < size) {
		oxy_detect[index].right_click = function;
		oxy_detect[index].right_arg = arg;
	}
}

void oxy_SetLeftClick(void *function, void *arg, uint8_t index)
{
	uint8_t size = oxy_mouse.hover_amount;
	
	if (index < size) {
		oxy_detect[index].left_click = function;
		oxy_detect[index].left_arg = arg;
	}
}

void oxy_SetMouseSpeed(uint8_t speed) 
{
	if (speed < 10 && speed > 1)
		oxy_mouse.speed = speed;
}


static bool oxy_DetectRect(int ix, int iy, int iw, int ih)
{
	// This function checks if the oxy_mouse x and y pos is in the given x,y,x+w,y+h. (I Hope that makes sense.)
	return (bool)(oxy_mouse.x > ix && oxy_mouse.x < ix + iw && oxy_mouse.y > iy && oxy_mouse.y < iy + ih);
}

static int oxy_DetectHover(void)
{
	if (!oxy_mouse.hover_amount) return -1;
	
	for (int i = 0; i < oxy_mouse.hover_amount; i++){
		// Detect spot found
		if (oxy_DetectRect(oxy_detect[i].x, oxy_detect[i].y, oxy_detect[i].w, oxy_detect[i].h)) return i; 		
	}
	
	return -1; // Throws an error or nothing found
}

static void oxy_UpdateMouse(void)
{
	struct oxy_detect_t *curr_detect;
	
	kb_key_t key;
	key = kb_Data[7];
	
	kb_Scan();
	
	if (kb_Data[6] & kb_Clear) {
		oxy_mouse.clicked_index = -3; // Returns a -3 for Clear clicked
		return;
	}
	
	if ((kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) && (hover_index != -1)) { // Right Click
		oxy_mouse.clicked_index = hover_index;
		curr_detect = &oxy_detect[hover_index];
		
		if (curr_detect->right_click != NULL) {
			if (curr_detect->right_arg != NULL){
				curr_detect->right_click(curr_detect->left_arg);
			}else curr_detect->right_click(NULL);
		}
		return;
	}
	
	if ((kb_Data[1] & kb_Mode) && (hover_index != -1)) { // Left Click
		oxy_mouse.clicked_index = hover_index;
		curr_detect = &oxy_detect[hover_index];
		
		if (curr_detect->left_click != NULL) {
			if (curr_detect->left_arg != NULL){
				curr_detect->left_click(curr_detect->left_arg);
			}else curr_detect->left_click(NULL);
		}
		return;
	}
	
	/*// Detects if [Mode] + [UP] was pressed and increases oxy_mouse speed.
	if (kb_IsDown(kb_KeyMode) && kb_IsDown(kb_KeyUp)) {
		if (oxy_mouse.speed != 10) oxy_mouse.speed++; // Increases oxy_mouse speed.
		return;
	}
	
	// Detects if [Mode] + [UP] was pressed and decrease oxy_mouse speed.
	if (kb_IsDown(kb_KeyMode) && kb_IsDown(kb_KeyDown)) {
		if (oxy_mouse.speed != 1) oxy_mouse.speed--; // Decrease oxy_mouse speed.
		return;
	}*/
	
	// The next four if statement are for scrolling.
	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyUp)) {
		oxy_mouse.scroll_Y -= 10;
		return;
	}
	
	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyLeft)) {
		if (oxy_mouse.scroll_Y != 0) {
			oxy_mouse.scroll_X -= 10;
		}
		return;
	}
		
	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyDown)) {
		oxy_mouse.scroll_Y += 10;
	}
	
	if (kb_IsDown(kb_KeyAlpha) && kb_IsDown(kb_KeyRight)){
		oxy_mouse.scroll_X += 10;
		return;
	}
	
	// This switch is used to create the movement
	switch (key){
		case kb_Down:
			if (oxy_mouse.y + oxy_mouse.speed <= LCD_HEIGHT) oxy_mouse.y += oxy_mouse.speed;
			break;
	
		case kb_Up:
			if (oxy_mouse.y - oxy_mouse.speed > -2) oxy_mouse.y -= oxy_mouse.speed;
			break;
			
		case kb_Left:
			if (oxy_mouse.x - oxy_mouse.speed > -2) oxy_mouse.x -= oxy_mouse.speed;
			break;

		case kb_Right:
			if (oxy_mouse.x + oxy_mouse.speed <= LCD_WIDTH) oxy_mouse.x += oxy_mouse.speed;
			break;
	}
	
	// These functions allow the oxy_mouse to move.
	if (kb_IsDown(kb_KeyUp) && kb_IsDown(kb_KeyRight)){
		if (oxy_mouse.y - oxy_mouse.speed > -2) oxy_mouse.y -= oxy_mouse.speed;
		if (oxy_mouse.x + oxy_mouse.speed <= LCD_WIDTH) oxy_mouse.x += oxy_mouse.speed;
	}
		
	if (kb_IsDown(kb_KeyUp) && kb_IsDown(kb_KeyLeft)){
		if (oxy_mouse.y - oxy_mouse.speed > -2) oxy_mouse.y -= oxy_mouse.speed;
		if (oxy_mouse.x - oxy_mouse.speed > -2) oxy_mouse.x -= oxy_mouse.speed;
	}
		
	if (kb_IsDown(kb_KeyDown) && kb_IsDown(kb_KeyRight)){
		if (oxy_mouse.y + oxy_mouse.speed <= LCD_HEIGHT) oxy_mouse.y += oxy_mouse.speed;
		if (oxy_mouse.x + oxy_mouse.speed <= LCD_WIDTH) oxy_mouse.x += oxy_mouse.speed;
	}
		
	if (kb_IsDown(kb_KeyDown) && kb_IsDown(kb_KeyLeft)){
		if (oxy_mouse.y + oxy_mouse.speed <= LCD_HEIGHT) oxy_mouse.y += oxy_mouse.speed;
		if (oxy_mouse.x - oxy_mouse.speed > -2) oxy_mouse.x -= oxy_mouse.speed;
	}

	// Smooth Scrolling
	oxy_mouse.scroll_X = oxy_mouse.scroll_X * .9; 
	oxy_mouse.scroll_Y = oxy_mouse.scroll_Y * .9;
}

void oxy_RenderMouse(void)
{
	uint16_t des_x;
	uint8_t des_y;
	gfx_sprite_t *mouse_buff;
	gfx_sprite_t *text_buff = NULL;
	
	mouse_buff = gfx_MallocSprite(cursorA_width, cursorA_height);
	gfx_GetSprite(mouse_buff, oxy_mouse.x, oxy_mouse.y);
	
	kb_Scan();
	
	hover_index = oxy_DetectHover();  // Checks hover data
	
	gfx_SetTransparentColor(0xf0);
	
	if (hover_index == -1) {
		gfx_TransparentSprite(cursorA, oxy_mouse.x, oxy_mouse.y);
	}else{
		gfx_TransparentSprite(cursorB, oxy_mouse.x, oxy_mouse.y);
		
		if (oxy_detect[hover_index].description != NULL) {
			text_buff = gfx_MallocSprite(gfx_GetStringWidth(oxy_detect[hover_index].description), 9);
			
			if (oxy_mouse.x <= LCD_WIDTH/2) des_x = oxy_mouse.x + cursorB_width + 1;
			if (oxy_mouse.x > LCD_WIDTH/2) des_x = oxy_mouse.x - gfx_GetStringWidth(oxy_detect[hover_index].description) - 8;
			if (oxy_mouse.y <= LCD_HEIGHT/2) des_y =  oxy_mouse.y + cursorB_height + 1;
			if (oxy_mouse.y > LCD_HEIGHT/2)	des_y =  oxy_mouse.y + 1;
			
			gfx_GetSprite(text_buff, des_x, des_y);
			gfx_PrintStringXY(oxy_detect[hover_index].description, des_x, des_y);
		}
	}
	
	gfx_Blit(1);
	
	/*Update buffer here*/ 
	gfx_Sprite(mouse_buff, oxy_mouse.x, oxy_mouse.y);
	free(mouse_buff);
	
	if (text_buff != NULL) {
		gfx_Sprite(text_buff, des_x, des_y);
		free(text_buff);
	}
		
	
	oxy_UpdateMouse();
}



