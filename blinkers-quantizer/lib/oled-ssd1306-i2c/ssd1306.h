/**
  ******************************************************************************
  * @file    ssd1306.h
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.6
  * @date    Dec 29, 2014
  * @brief   SSD1306 OLED driver interface for ESP8266. For panel
  *          configuration, edit the panel option block in ssd1306_i2c.c
  *
  ******************************************************************************
  * @copyright
  *
  * Many of the work here are based on Adafruit SSD1306 and GFX Arduino library.
  * Please support Adafruit by purchasing products from Adafruit!
  *
  * Copyright (c) 2015, Baoshi Zhu. All rights reserved.
  * Use of this source code is governed by a BSD-style license that can be
  * found in the LICENSE.txt file.
  *
  * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY.  IN NO EVENT WILL THE AUTHOR(S) BE HELD LIABLE FOR ANY DAMAGES
  * ARISING FROM THE USE OF THIS SOFTWARE.
  *
  */

#ifndef SSD1306_H
#define SSD1306_H
#include "stdbool.h"



//! @brief Drawing color
typedef enum
{
    SSD1306_COLOR_TRANSPARENT = -1, //!< Transparent (not drawing)
    SSD1306_COLOR_BLACK = 0,        //!< Black (pixel off)
    SSD1306_COLOR_WHITE = 1,        //!< White (or blue, yellow, pixel on)
    SSD1306_COLOR_INVERT = 2,       //!< Invert pixel (XOR)
} ssd1306_color_t;


/**
 * @brief   Initialize OLED panel
 * @param   id  Panel ID
 * @param   scl_pin  SCL Pin
 * @param   sda_pin  SDA Pin
 * @return  true if successful
 * @remark  Possible reasons for failure include non-configured panel type, out of memory or I2C not responding
 */
bool ssd1306_init(uint8_t id,uint8_t scl_pin, uint8_t sda_pin);


/**
 * @brief   De-initialize OLED panel, turn off power and free memory
 * @param   id  Panel ID
 * @return  true if successful
 * @remark  Possible reasons for failure include non-configured panel type, out of memory or I2C not responding
 */
void ssd1306_term(uint8_t id);


/**
 * @brief   Return OLED panel width
 * @param   id  Panel ID
 * @return  Panel width, or return 0 if failed (panel not initialized)
 */
uint8_t ssd1306_get_width(uint8_t id);


/**
 * @brief   Return OLED panel height
 * @param   id  Panel ID
 * @return  Panel height, or return 0 if failed (panel not initialized)
 */
uint8_t ssd1306_get_height(uint8_t id);

/**
 * @brief   Clear display buffer (fill with black)
 * @param   id Panel ID
 */
void ssd1306_clear(uint8_t id);

/**
 * @brief   Refresh display (send display buffer to the panel)
 * @param   id      Panel ID
 * @param   force   The program automatically tracks "dirty" region to minimize refresh area. Set #force to true
 *                  ignores the dirty region and refresh the whole screen.
 */
void ssd1306_refresh(uint8_t id, bool force);

/**
 * @brief   Draw one pixel
 * @param   id      Panel ID
 * @param   x       X coordinate
 * @param   y       Y coordinate
 * @param   color   Color of the pixel
 */
void ssd1306_draw_pixel(uint8_t id, int8_t x, int8_t y, ssd1306_color_t color);


/**
 * @brief   Draw horizontal line
 * @param   id      Panel ID
 * @param   x       X coordinate or starting (left) point
 * @param   y       Y coordinate or starting (left) point
 * @param   w       Line width
 * @param   color   Color of the line
 */
void ssd1306_draw_hline(uint8_t id, int8_t x, int8_t y, uint8_t w, ssd1306_color_t color);


/**
 * @brief   Draw vertical line
 * @param   id      Panel ID
 * @param   x       X coordinate or starting (top) point
 * @param   y       Y coordinate or starting (top) point
 * @param   h       Line height
 * @param   color   Color of the line
 */
void ssd1306_draw_vline(uint8_t id, int8_t x, int8_t y, uint8_t h, ssd1306_color_t color);

/**
 * @brief   Draw a rectangle
 * @param   id      Panel ID
 * @param   x       X coordinate or starting (top left) point
 * @param   y       Y coordinate or starting (top left) point
 * @param   w       Rectangle width
 * @param   h       Rectangle height
 * @param   color   Color of the rectangle border
 */
void ssd1306_draw_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, ssd1306_color_t color);


/**
 * @brief   Draw a filled rectangle
 * @param   id      Panel ID
 * @param   x       X coordinate or starting (top left) point
 * @param   y       Y coordinate or starting (top left) point
 * @param   w       Rectangle width
 * @param   h       Rectangle height
 * @param   color   Color of the rectangle
 */
void ssd1306_fill_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, ssd1306_color_t color);


/**
 * @brief   Draw a circle
 * @param   id      Panel ID
 * @param   x0      X coordinate or center
 * @param   y0      Y coordinate or center
 * @param   r       Radius
 * @param   color   Color of the circle
 */
void ssd1306_draw_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color);

/**
 * @brief   Draw a filled circle
 * @param   id      Panel ID
 * @param   x0      X coordinate or center
 * @param   y0      Y coordinate or center
 * @param   r       Radius
 * @param   color   Color of the circle
 */
void ssd1306_fill_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color);

/**
 * @brief   Select font for drawing
 * @param   id      Panel ID
 * @param   idx     Font index, see fonts.c
 */
void ssd1306_select_font(uint8_t id, uint8_t idx);

/**
 * @brief   Draw one character using currently selected font
 * @param   id          Panel ID
 * @param   x           X position of character (top-left corner)
 * @param   y           Y position of character (top-left corner)
 * @param   c           The character to draw
 * @param   foreground  Character color
 * @param   background  Background color
 * @return  Width of the character
 */
uint8_t ssd1306_draw_char(uint8_t id, uint8_t x, uint8_t y,unsigned char c, ssd1306_color_t foreground, ssd1306_color_t background);

/**
 * @brief   Draw string using currently selected font
 * @param   id          Panel ID
 * @param   x           X position of string (top-left corner)
 * @param   y           Y position of string (top-left corner)
 * @param   str         The string to draw
 * @param   foreground  Character color
 * @param   background  Background color
 * @return  Width of the string (out-of-display pixels also included)
 */
uint8_t ssd1306_draw_string(uint8_t id, uint8_t x, uint8_t y, char *str, ssd1306_color_t foreground, ssd1306_color_t background);

/**
 * @brief   Measure width of string with current selected font
 * @param   id          Panel ID
 * @param   str         String to measure
 * @return  Width of the string
 */
uint8_t ssd1306_measure_string(uint8_t id, char *str);

/**
 * @brief   Get the height of current selected font
 * @param   id          Panel ID
 * @return  Height of the font (in pixels) or 0 if none font selected
 */
uint8_t ssd1306_get_font_height(uint8_t id);

/**
 * @brief   Get the "C" value (space between adjacent characters)
 * @param   id          Panel ID
 * @return  "C" value
 */
uint8_t ssd1306_get_font_c(uint8_t id);

/**
 * @brief   Set normal or inverted display
 * @param   id          Panel ID
 * @param   invert      Invert display?
 */
void ssd1306_invert_display(uint8_t id, bool invert);

/**
 * @brief   Direct update display buffer
 * @param   id          Panel ID
 * @param   data        Data to fill display buffer, no length check is performed!
 * @param   length      Length of data
 */
void ssd1306_update_buffer(uint8_t id, uint8_t* data, uint16_t length);



#endif  /* SSD1306_H */

