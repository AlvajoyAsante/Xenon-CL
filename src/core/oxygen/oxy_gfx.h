#ifndef OXY_GFX_H
#define OXY_GFX_H

#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
// #include "oxy_gfx.h"

#define oxy_FillRoundRectangle_Upper(x, y, w, h) \
    oxy_FillRoundRectangle(x, y, w, h, 1)

#define oxy_FillRoundRectangle_Bottom(x, y, w, h) \
    oxy_FillRoundRectangle(x, y, w, h, 2)

#define oxy_FillRoundRectangle_Left(x, y, w, h) \
    oxy_FillRoundRectangle(x, y, w, h, 3)

#define oxy_FillRoundRectangle_Right(x, y, w, h) \
    oxy_FillRoundRectangle(x, y, w, h, 4)

#define oxy_RoundRectangle_Upper(x, y, w, h) \
    oxy_RoundRectangle(x, y, w, h, 1)

#define oxy_RoundRectangle_Bottom(x, y, w, h) \
    oxy_RoundRectangle(x, y, w, h, 2)

#define oxy_RoundRectangle_Left(x, y, w, h) \
    oxy_RoundRectangle(x, y, w, h, 3)

#define oxy_RoundRectangle_Right(x, y, w, h) \
    oxy_RoundRectangle(x, y, w, h, 4)

#define oxy_Rgb1555toGFX(x) \
    oxy_Rgb1555toPalette(x, gfx_palette, 256)

// K.I.S.S. PRINCIPLE

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /**
     * Filled Rectangle, Rounded on all sides.
     * @param x X Position
     * @param y Y Position
     * @param w Width of Rectangle
     * @param h Height of Rectangle
     * @param type ranges from one to 4
     */
    void oxy_FillRoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

    /**
     * Filled Rectangle, Rounded on all sides.
     * @param x X Position
     * @param y Y Position
     * @param w Width of Rectangle
     * @param h Height of Rectangle
     * @param type ranges 0 one to 4
     */
    void oxy_RoundRectangle(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t type);

    // Sprite Routines
    /**
     * Apply a new palette onto a sprite.
     * @param in_sprite Sprite you want to apply palette onto.
     * @param map List of palette map.
     */
    gfx_sprite_t *oxy_RepalettizeSprite(gfx_sprite_t *in, const uint16_t *palette, const uint8_t palette_size);

    /**
     * Replaces all colors in sprite with new colors.
     * @param in Sprite containing color.
     * @param old_color Old color,color you want to change.
     * @param new_color Color you want to replace old color.
     */
    gfx_sprite_t *oxy_ReplaceSpriteColor(gfx_sprite_t *in, const uint8_t old_color, const uint8_t new_color);

    /**
     * Apply a mask over a sprite.
     * @param in Sprite you want to apply mask over.
     * @param mask Mask sprite that will be applied over "in_sprite".
     * if "mask_sprite" and "in_sprite" don't have the same width and height.
     */
    gfx_sprite_t *oxy_ApplyMaskToSprite(gfx_sprite_t *in, gfx_sprite_t *mask, const uint8_t mask_color, const uint8_t transparent_color);

    // Color Routines.
    /**
     * Condense sprite into using a condensed version of xlibc palette.
     * @param in Pointer to an initialized sprite structure.
     */
    void oxy_CondenseSprite(gfx_sprite_t *in);

    /**
     * Inserts a condensed version of xlibc into the palette.
     * @param offset Palette index to insert at.
     */
    void oxy_InsertCondensePalette(uint8_t offset);

    /**
     * Take in_sprite and returns list of colors .
     * @param in Pointer to an initialized sprite structure.
     * @returns Returns a pointer to list of colors
     */
    uint16_t *oxy_GetSpritePalette(gfx_sprite_t *in);

    /**
     * Convert Rgb1555 colors to closet palette color.
     * @param color Color to convert to Palette.
     * @param palette (uint16_t) Pointer to palette to set.
     * @param palette_size Size of the palette.
     * @returns (uint8_t) Palette Position.
     */
    uint8_t oxy_Rgb1555toPalette(uint16_t color, const uint16_t *palette, const uint8_t palette_size);

    /**
     * Convert Rgb1555 colors to RBG.
     * @param color color to convert.
     * @returns a pointer to RBG colors.
     */
    uint8_t *oxy_Rgb1555ToRGB(uint16_t color);

    /**
     * Returns the difference between two colors (used in oxy_Rgb1555toPalette).
     * @param color1 color compared to color2.
     * @param color2 color compared to color1.
     * @return int The difference between to color (read more here https://en.wikipedia.org/wiki/Color_difference#sRGB).
     */
    int oxy_ColorDifference(uint16_t color1, uint16_t color2);

    /**
     * Darken an rectangular area.
     * @param x X Position
     * @param y Y Position
     * @param w Width of Rectangle
     * @param h Height of Rectangle
     * @param amount the amount users wants to Darken (max is 255)
     */
    void oxy_DarkenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount);

    /**
     * Lighten an rectangular area.
     * @param x X Position
     * @param y Y Position
     * @param w Width of Rectangle
     * @param h Height of Rectangle
     * @param amount The amount users wants to lighten (max is 255)
     */
    void oxy_LightenArea(uint16_t x, uint8_t y, int w, uint8_t h, uint8_t amount);

    /**
     * Lighten an rectangular area.
     * @param amount The amount users wants to darken (max is 255).
     * @param palette (uint16_t) Pointer to palette to set.
     * @param palette_size Size of the palette.
     */
    uint16_t *oxy_DarkenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

    /**
     * Lighten an rectangular area.
     * @param amount  The amount users wants to lighten (max is 255).
     * @param palette (uint16_t) Pointer to palette to set.
     * @param palette_size Size of the palette.
     */
    uint16_t *oxy_LightenPalette(const uint8_t amount, uint16_t *palette, const uint8_t palette_size);

    /**
     * Returns Transparent Palette color.
     * @return Returns Transparent Palette color.
     */
    uint8_t oxy_ReturnTransparentColor(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif