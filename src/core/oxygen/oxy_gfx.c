#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>
#include <math.h>
#include <debug.h>

#include "oxy_gfx.h"
#include "gfx/oxy_sprites.h"

// Shapes Routines
/**
 * This function renders a filled round rectangle depending on the type on "type" (read the .h file for more).
 */
void oxy_FillRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type)
{
	switch (type)
	{
	case 0: // oxy_RoundFillRectangle
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 1: // oxy_RoundFillRectangle_Upper
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 1);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 2: // oxy_RoundFillRectangle_Bottom
		for (int i = 0; i < w; i++)
		{
			if (i == 0 || i == w - 1)
			{
				gfx_VertLine(x + i, y, h - 1);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 3: // oxy_RoundFillRectangle_Left
		for (int i = 0; i < w; i++)
		{
			if (i == 0)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;

	case 4: // oxy_RoundFillRectangle_Right
		for (int i = 0; i < w; i++)
		{
			if (i == w - 1)
			{
				gfx_VertLine(x + i, y + 1, h - 2);
			}
			else
				gfx_VertLine(x + i, y, h);
		}
		break;
	}
}

/**
 * This function renders a (not filled) round rectangle depending on the type on "type" (read the .h file for more).
 */
void oxy_RoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type)
{
	switch (type)
	{
	case 0: // oxy_RoundRectangle
		gfx_HorizLine(x + 1, y, w - 2);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 2);
		gfx_VertLine(x, y + 1, h - 2);
		gfx_VertLine((x + w) - 1, y + 1, h - 2);
		break;

	case 1: // oxy_RoundRectangle_Upper
		gfx_HorizLine(x + 1, y, w - 2);
		gfx_HorizLine(x, (y + h) - 1, w);
		gfx_VertLine(x, y + 1, h - 1);
		gfx_VertLine((x + w) - 1, y + 1, h - 1);
		break;

	case 2: // oxy_RoundRectangle_Bottom
		gfx_HorizLine(x, y, w);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 2);
		gfx_VertLine(x, y, h - 1);
		gfx_VertLine((x + w) - 1, y, h - 1);
		break;

	case 3: // oxy_RoundRectangle_Left
		gfx_HorizLine(x + 1, y, w - 1);
		gfx_HorizLine(x + 1, (y + h) - 1, w - 1);
		gfx_VertLine(x, y + 1, h - 2);
		gfx_VertLine((x + w) - 1, y, h);
		break;

	case 4: // oxy_RoundRectangle_Right
		gfx_HorizLine(x, y, w);
		gfx_HorizLine(x, (y + h) - 1, w - 1);
		gfx_VertLine(x, y, h - 1);
		gfx_VertLine((x + w) - 1, y + 1, h - 2);
		break;
	}
}

// Sprite Routines.
/**
 * This function is use to replace the color in a sprite.
 */
gfx_sprite_t *oxy_ReplaceSpriteColor(gfx_sprite_t *in, const uint8_t old_color, const uint8_t new_color)
{
	int size = in->width * in->height;
	gfx_sprite_t *out;

	out = gfx_MallocSprite(in->width, in->height);
	memcpy(out, in, size + 2);

	for (int i = 0; i < size; i++)
	{
		if (out->data[i] == old_color)
			out->data[i] = new_color;
	}

	return out;
}

/**
 * This function cuts out sprite based on based on given mask.
 */
gfx_sprite_t *oxy_ApplyMaskToSprite(gfx_sprite_t *in, gfx_sprite_t *mask, const uint8_t mask_color, const uint8_t transparent_color)
{
	gfx_sprite_t *out, *resize;
	const int mask_size = mask->width * mask->height;
	const int in_size = in->width * in->height;
	const bool resized = (mask_size != in_size);

	out = gfx_MallocSprite(in->width, in->height);

	if (resized)
	{
		resize = gfx_MallocSprite(in->width, in->height);
		gfx_ScaleSprite(resize, mask);
	}

	for (int i = 0; i < in_size; i++)
	{
		if (!resized)
		{
			if (mask->data[i] == mask_color)
				out->data[i] = transparent_color;
		}
		else
		{
			if (resize->data[i] == mask_color)
				out->data[i] = transparent_color;
		}
	}

	return out;
}

/**
 * This function replaces the whole sprites palette.
 */
gfx_sprite_t *oxy_RepalettizeSprite(gfx_sprite_t *in, const uint16_t *palette, const uint8_t palette_size)
{
	gfx_sprite_t *out;
	const int size = in->width * in->height;
	out = gfx_MallocSprite(in->width, in->height);

	for (int i = 0; i < size; i++)
	{
		out->data[i] = oxy_Rgb1555toPalette((uint16_t)in->data[i], palette, palette_size);
	}

	return out;
}

// Color Routines.
/*uint16_t* oxy_BlurSprite(gfx_sprite_t *in)
{

}

void oxy_BlurArea(uint16_t x, uint8_t y, int w, uint8_t h)
{
	int red;
	int green;
	int blue;
	int counter = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			red = green = blue = 0;
			counter = 0;

			if (i + 1 < h && j - 1 >= 0) {
				counter++;
			}

			if (j + 1 < w) {
				counter++;
			}

			if (i + 1 < h && j + 1 < w) {
				counter++;
			}

			if (i + 1 < h) {
				counter++;
			}

			if (j - 1 >= 0) {
				counter++;
			}

			if (i - 1 >= 0) {
				counter++;
			}
		}
	}

}*/

/**
 * Condense Sprite into using a condensed version of Xlibc Palette
 * Created by Epsilon5!
 */
void oxy_CondenseSprite(gfx_sprite_t *in)
{
	for (int i = 0; i < in->width * in->height; i++)
		in->data[i] = xlibc_condensed_data[in->data[i]];
}

/**
 * Inserts a condensed version of Xlibc into the Palette
 */
void oxy_InsertCondensePalette(uint8_t offset)
{
	gfx_SetPalette(xlibc_condensed_palette, sizeof_xlibc_condensed_palette, offset);
}

/**
 * This function gets a list of colors in a sprite (not order from least to greatest).
 */
uint16_t *oxy_GetSpritePalette(gfx_sprite_t *in)
{
	const int size = in->width * in->height;
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * size);
	uint8_t output_size = 0;

	for (int i = 0; i < size; i++)
	{
		for (int r = 0; r < output_size + 1; r++)
		{
			if (output[r] == in->data[i])
			{
				output[output_size] = in->data[i];
				output_size++;
			}
		}
	}

	return output;
}

/**
 * This function returns a palette color from a Rgb1555 color.
 */
uint8_t oxy_Rgb1555ToPalette(uint16_t color, const uint16_t *palette, const uint8_t palette_size)
{
	int Difference = 10000;
	uint8_t output;

	dbg_sprintf(dbgout, "oxy_Rgb1555toPalette(%d, palette, %d){", color, palette_size);

	for (int i = 0; i < palette_size; i++)
	{
		if (oxy_ColorDifference(gfx_RGBTo1555(oxy_Rgb1555ToRGB(color)[0], oxy_Rgb1555ToRGB(color)[1], oxy_Rgb1555ToRGB(color)[2]), palette[i]) < Difference)
		{
			Difference = oxy_ColorDifference(gfx_RGBTo1555(oxy_Rgb1555ToRGB(color)[0], oxy_Rgb1555ToRGB(color)[1], oxy_Rgb1555ToRGB(color)[2]), palette[i]);
			output = i;

			dbg_sprintf(dbgout, "\nNew Best Color: %d / %u / %u,%u,%u\n", output, palette[output], oxy_Rgb1555ToRGB(palette[output])[0], oxy_Rgb1555ToRGB(palette[output])[1], oxy_Rgb1555ToRGB(palette[output])[2]);
			dbg_sprintf(dbgout, "\nDifference: %u", Difference);
		}
	}

	dbg_sprintf(dbgout, "Final Best Color: %d / %u / %u,%u,%u\n", output, palette[output], oxy_Rgb1555ToRGB(palette[output])[0], oxy_Rgb1555ToRGB(palette[output])[1], oxy_Rgb1555ToRGB(palette[output])[2]);
	dbg_sprintf(dbgout, "}Returned:%d\n", output);

	return output;
}

/**
 * This function gives the closets color relating to the input Rgb1555
 */
uint8_t *oxy_Rgb1555ToRGB(uint16_t color)
{
	dbg_sprintf(dbgout, "oxy_Rgb1555ToRGB(%d){\n", color);

	uint8_t *output = (uint8_t *)malloc(sizeof(uint8_t) * 3);

	output[0] = (uint8_t)(((color & 0xFC00) >> 10) << 3);
	output[1] = (uint8_t)(((color & 0x03E0) >> 5) << 3);
	output[2] = (uint8_t)((color & 0x001F) << 3);

	dbg_sprintf(dbgout, "R: %d\n B: %d\n G: %d\n}\n", output[0], output[1], output[2]);

	return output;
}

/**
 * This function gives the difference between two rgb1555 colors.
 */
int oxy_ColorDifference(uint16_t color1, uint16_t color2)
{
	return (abs(((int)(oxy_Rgb1555ToRGB(color1)[0])) - ((int)(oxy_Rgb1555ToRGB(color2)[0]))) + abs(((int)(oxy_Rgb1555ToRGB(color1)[1])) - ((int)(oxy_Rgb1555ToRGB(color2)[1]))) + abs(((int)(oxy_Rgb1555ToRGB(color1)[2])) - ((int)(oxy_Rgb1555ToRGB(color2)[2]))));
}

/**
 * This function "Darkness" an area which is basically a rectangle.
 */
void oxy_DarkenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount)
{
	for (int i = 0; i < w; i++)
	{
		for (int r = 0; r < h; r++)
		{
			gfx_SetColor(gfx_Darken((uint16_t)gfx_GetPixel(x + i, y + r), amount));
			gfx_SetPixel(x + i, y + r);
		}
	}
}

/**
 * This function "Lighten" an area which is basically a rectangle.
 */
void oxy_LightenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount)
{
	for (int i = 0; i < w; i++)
	{
		for (int r = 0; r < h; r++)
		{
			gfx_SetColor(gfx_Lighten((uint16_t)gfx_GetPixel(x + i, y + r), amount));
			gfx_SetPixel(x + i, y + r);
		}
	}
}

/**
 * This function returns a pointer to a list of darken palette colors.
 */
uint16_t *oxy_DarkenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size)
{
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * palette_size);

	for (uint8_t i = 0; i < palette_size; i++)
	{
		output[i] = gfx_Darken(palette[i], amount);
	}

	return output;
}

/**
 * This function returns a pointer to a list of lighten palette colors.
 */
uint16_t *oxy_LightenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size)
{
	uint16_t *output = (uint16_t *)malloc(sizeof(uint16_t) * palette_size);

	for (uint8_t i = 0; i < palette_size; i++)
	{
		output[i] = gfx_Lighten(palette[i], amount);
	}

	return output;
}

/**
 * This function returns a the set transparent of "gfx" libary.
 * It's not needed but it's fun to have :).
 */
uint8_t oxy_ReturnTransparentColor(void)
{
	return gfx_SetTransparentColor(gfx_SetTransparentColor(gfx_SetTransparentColor(0)));
}