/**
  ******************************************************************************
  * @file    ssd1306_i2c.c
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.6
  * @date    Dec 29, 2014
  * @brief   SSD1306 I2C OLED driver for ESP8266
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
  * ARISING FROM THE USE OF THIS SOFTWARE,
  *
  ******************************************************************************
  */


//#include "esp_common.h"
//#include "dmsg.h"
#include "i2c.h"
#include "fonts.h"
#include "stddef.h"
#include "ssd1306.h"
#include "stdlib.h"
#include "string.h"


/**
 * @name User configurable panel option block
 * @{
 */

//! @brief Panel 0 type, define to SSD1306_NONE if not used.
#define PANEL0_TYPE SSD1306_128x64
//! @brief I2C address for panel 0
#define PANEL0_ADDR (0x3c<<1)
//! @brief If panel 0 has external RESET pin, define this as 1
#define PANEL0_USE_RST 0
//! @brief GPIO MUX for panel 0 RESET pin
#define PANEL0_RST_MUX  PERIPHS_IO_MUX_MTDI_U
//! @brief GPIO FUNC for panel 0 RESET pin
#define PANEL0_RST_FUNC FUNC_GPIO12
//! @brief GPIO bit location for panel 0 RESET pin
#define PANEL0_RST_BIT  BIT12

//! @brief Panel 1 type, define to SSD1306_NONE if not used.
#define PANEL1_PANEL_TYPE SSD1306_128x64
//! @brief I2C address for panel 1
#define PANEL1_ADDR (0x3d << 1)
//! @brief If panel 1 has external RESET pin, define this as 1
#define PANEL1_USE_RST 0
//! @brief GPIO MUX for panel 1 RESET pin
#define PANEL1_RST_MUX  PERIPHS_IO_MUX_MTMS_U
//! @brief GPIO FUNC for panel 1 RESET pin
#define PANEL1_RST_FUNC FUNC_GPIO14
//! @brief GPIO bit location for panel 1 RESET pin
#define PANEL1_RST_BIT  BIT14

/** @} */


#define SSD1306_NONE       0  //!< not used
#define SSD1306_128x64     1  //!< 128x32 panel
#define SSD1306_128x32     2  //!< 128x64 panel


void _command(uint8_t adress, uint8_t c)
{
    bool ret;
    i2c_start();
    ret = i2c_write(adress);
    if (!ret) // NACK
        i2c_stop();
    i2c_write(0x00);    // Co = 0, D/C = 0
    i2c_write(c);
    i2c_stop();
}


void _data(uint8_t adress, uint8_t d)
{
    bool ret;
    i2c_start();
    ret = i2c_write(adress);
    if (!ret) // NACK
        i2c_stop();
    i2c_write(0x40);    // Co = 0, D/C = 1
    i2c_write(d);
    i2c_stop();
}


typedef struct _oled_i2c_ctx
{
    uint8_t type;       // Panel type
    uint8_t address;        // I2C address
    uint8_t *buffer;        // display buffer
    uint8_t width;          // panel width (128)
    uint8_t height;         // panel height (32 or 64)
    uint8_t id;             // my id
    uint8_t refresh_top;    // "Dirty" window
    uint8_t refresh_left;
    uint8_t refresh_right;
    uint8_t refresh_bottom;
    const font_info_t* font;    // current font
} oled_i2c_ctx;

oled_i2c_ctx *_ctxs[2] = { NULL };


bool ssd1306_init(uint8_t id,uint8_t scl_pin, uint8_t sda_pin)
{
	i2c_init(scl_pin,sda_pin);
    oled_i2c_ctx *ctx = NULL;

    if ((id != 0) && (id != 1))
        goto oled_init_fail;

    // free old context (if any)
    ssd1306_term(id);

    ctx = malloc(sizeof(oled_i2c_ctx));
//    memset(&ctx,0,sizeof(oled_i2c_ctx));
    if (ctx == NULL)
    {
//        dmsg_err_puts("Alloc OLED context failed.");
        goto oled_init_fail;
    }
    if (id == 0)
    {
#if (PANEL0_TYPE != 0)
  #if (PANEL0_TYPE == SSD1306_128x64)
        ctx->type = SSD1306_128x64;
        ctx->buffer = malloc(1024); // 128 * 64 / 8
        ctx->width = 128;
        ctx->height = 64;
  #elif (PANEL0_TYPE == SSD1306_128x32)
        ctx->type = SSD1306_128x32;
        ctx->buffer = malloc(512);  // 128 * 32 / 8
//        memset(&(ctx->buffer),0,512);
        ctx->width = 128;
        ctx->height = 32;
  #else
    #error "Panel 0 undefined"
  #endif
        if (ctx->buffer == NULL)
        {
//            dmsg_err_puts("Alloc OLED buffer failed.");
            goto oled_init_fail;
        }
        ctx->address = PANEL0_ADDR;
  #if PANEL0_USE_RST
        // Panel 0 reset
        PIN_FUNC_SELECT(PANEL0_RST_MUX, PANEL0_RST_FUNC);
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | PANEL0_RST_BIT);
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, PANEL0_RST_BIT);
        os_delay_us(10000);
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, PANEL0_RST_BIT);
  #endif
#else
        dmsg_err_puts("Panel 0 not defined.");
        goto oled_init_fail;
#endif
    }
    else if (id == 1)
    {
#if (PANEL1_PANEL_TYPE != 0)
  #if (PANEL1_PANEL_TYPE ==SSD1306_128x64)
        ctx->type = SSD1306_128x64;
        ctx->buffer = malloc(1024); // 128 * 64 / 8
//        memset(&(ctx->buffer),0,1024);
        ctx->width = 128;
        ctx->height = 64;
  #elif (PANEL1_PANEL_TYPE == SSD1306_128x32)
        ctx->type = SSD1306_128x32;
        ctx->buffer = zalloc(512);  // 128 * 32 / 8
        ctx->width = 128;
        ctx->height = 32;
  #else
     #error "Unknown Panel 1 type"
  #endif
        if (ctx->buffer == NULL)
        {
//            dmsg_err_puts("Alloc OLED buffer failed.");
            goto oled_init_fail;
        }
        ctx->address = PANEL1_ADDR;
  #if PANEL1_USE_RST
        // Panel 1 reset
        PIN_FUNC_SELECT(PANEL1_RST_MUX, PANEL1_RST_FUNC);
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | PANEL1_RST_BIT);
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, PANEL1_RST_BIT);
        os_delay_us(10000);
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, PANEL1_RST_BIT);
  #endif
#else
        dmsg_err_puts("Panel 1 not defined.");
        goto oled_init_fail;
#endif
    }

    // Panel initialization
    // Try send I2C address check if the panel is connected
    i2c_start();
    if (!i2c_write(ctx->address))
    {
        i2c_stop();
//        dmsg_err_puts("OLED I2C bus not responding.");
        goto oled_init_fail;
    }
    i2c_stop();

    // Now we assume all sending will be successful
    if (ctx->type == SSD1306_128x64)
    {
        _command(ctx->address, 0xae); // SSD1306_DISPLAYOFF
        _command(ctx->address, 0xd5); // SSD1306_SETDISPLAYCLOCKDIV
        _command(ctx->address, 0x80); // Suggested value 0x80
        _command(ctx->address, 0xa8); // SSD1306_SETMULTIPLEX
        _command(ctx->address, 0x3f); // 1/64
        _command(ctx->address, 0xd3); // SSD1306_SETDISPLAYOFFSET
        _command(ctx->address, 0x00); // 0 no offset
        _command(ctx->address, 0x40); // SSD1306_SETSTARTLINE line #0
        _command(ctx->address, 0x20); // SSD1306_MEMORYMODE
        _command(ctx->address, 0x00); // 0x0 act like ks0108
        _command(ctx->address, 0xa1); // SSD1306_SEGREMAP | 1
        _command(ctx->address, 0xc8); // SSD1306_COMSCANDEC
        _command(ctx->address, 0xda); // SSD1306_SETCOMPINS
        _command(ctx->address, 0x12);
        _command(ctx->address, 0x81); // SSD1306_SETCONTRAST
        _command(ctx->address, 0xcf);
        _command(ctx->address, 0xd9); // SSD1306_SETPRECHARGE
        _command(ctx->address, 0xf1);
        _command(ctx->address, 0xdb); // SSD1306_SETVCOMDETECT
        _command(ctx->address, 0x30);
        _command(ctx->address, 0x8d); // SSD1306_CHARGEPUMP
        _command(ctx->address, 0x14); // Charge pump on
        _command(ctx->address, 0x2e); // SSD1306_DEACTIVATE_SCROLL
        _command(ctx->address, 0xa4); // SSD1306_DISPLAYALLON_RESUME
        _command(ctx->address, 0xa6); // SSD1306_NORMALDISPLAY
    }
    else if (ctx->type == SSD1306_128x32)
    {
        _command(ctx->address, 0xae); // SSD1306_DISPLAYOFF
        _command(ctx->address, 0xd5); // SSD1306_SETDISPLAYCLOCKDIV
        _command(ctx->address, 0x80); // Suggested value 0x80
        _command(ctx->address, 0xa8); // SSD1306_SETMULTIPLEX
        _command(ctx->address, 0x1f); // 1/32
        _command(ctx->address, 0xd3); // SSD1306_SETDISPLAYOFFSET
        _command(ctx->address, 0x00); // 0 no offset
        _command(ctx->address, 0x40); // SSD1306_SETSTARTLINE line #0
        _command(ctx->address, 0x8d); // SSD1306_CHARGEPUMP
        _command(ctx->address, 0x14); // Charge pump on
        _command(ctx->address, 0x20); // SSD1306_MEMORYMODE
        _command(ctx->address, 0x00); // 0x0 act like ks0108
        _command(ctx->address, 0xa1); // SSD1306_SEGREMAP | 1
        _command(ctx->address, 0xc8); // SSD1306_COMSCANDEC
        _command(ctx->address, 0xda); // SSD1306_SETCOMPINS
        _command(ctx->address, 0x02);
        _command(ctx->address, 0x81); // SSD1306_SETCONTRAST
        _command(ctx->address, 0x2f);
        _command(ctx->address, 0xd9); // SSD1306_SETPRECHARGE
        _command(ctx->address, 0xf1);
        _command(ctx->address, 0xdb); // SSD1306_SETVCOMDETECT
        _command(ctx->address, 0x40);
        _command(ctx->address, 0x2e); // SSD1306_DEACTIVATE_SCROLL
        _command(ctx->address, 0xa4); // SSD1306_DISPLAYALLON_RESUME
        _command(ctx->address, 0xa6); // SSD1306_NORMALDISPLAY
    }
    // Save context
    ctx->id = id;
    _ctxs[id] = ctx;

    ssd1306_clear(id);
    ssd1306_refresh(id, true);

    _command(ctx->address, 0xaf); // SSD1306_DISPLAYON

    return true;

oled_init_fail:
    if (ctx && ctx->buffer) free(ctx->buffer);
    if (ctx) free(ctx);
    return false;
}


void ssd1306_term(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    if (ctx == NULL)
       return;

    _command(ctx->address, 0xae); // SSD_DISPLAYOFF
    _command(ctx->address, 0x8d); // SSD1306_CHARGEPUMP
    _command(ctx->address, 0x10); // Charge pump off

    if (ctx->buffer)
        free(ctx->buffer);
    free(ctx);

    _ctxs[id] = NULL;
}


uint8_t ssd1306_get_width(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    if (ctx == NULL)
       return 0;

    return ctx->width;
}


uint8_t ssd1306_get_height(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    if (ctx == NULL)
       return 0;

    return ctx->height;
}


void ssd1306_clear(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    if (ctx == NULL)
        return;

    if (ctx->type == SSD1306_128x64)
    {
        memset(ctx->buffer, 0, 1024);
    }
    else if (ctx->type == SSD1306_128x32)
    {
        memset(ctx->buffer, 0, 512);
    }
    ctx->refresh_right = ctx->width - 1;
    ctx->refresh_bottom = ctx->height - 1;
    ctx->refresh_top = 0;
    ctx->refresh_left = 0;
}


void ssd1306_refresh(uint8_t id, bool force)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint8_t i,j;
    uint16_t k;
    uint8_t page_start, page_end;

    if (ctx == NULL)
        return;

    if (force)
    {
        if (ctx->type == SSD1306_128x64)
        {
            _command(ctx->address, 0x21); // SSD1306_COLUMNADDR
            _command(ctx->address, 0);    // column start
            _command(ctx->address, 127);  // column end
            _command(ctx->address, 0x22); // SSD1306_PAGEADDR
            _command(ctx->address, 0);    // page start
            _command(ctx->address, 7);    // page end (8 pages for 64 rows OLED)
            for (k = 0; k < 1024; k++)
            {
                i2c_start();
                i2c_write(ctx->address);
                i2c_write(0x40);
                for (j = 0; j < 16; ++j)
                {
                    i2c_write(ctx->buffer[k]);
                    ++k;
                }
                --k;
                i2c_stop();
            }
        }
        else if (ctx->type == SSD1306_128x32)
        {
            _command(ctx->address, 0x21); // SSD1306_COLUMNADDR
            _command(ctx->address, 0);    // column start
            _command(ctx->address, 127);  // column end
            _command(ctx->address, 0x22); // SSD1306_PAGEADDR
            _command(ctx->address, 0);    // page start
            _command(ctx->address, 3);    // page end (4 pages for 32 rows OLED)
            for (k = 0; k < 512; k++)
            {
                i2c_start();
                i2c_write(ctx->address);
                i2c_write(0x40);
                for (j = 0; j < 16; ++j)
                {
                    i2c_write(ctx->buffer[k]);
                    ++k;
                }
                --k;
                i2c_stop();
            }
        }
    }
    else
    {
        if ((ctx->refresh_top <= ctx->refresh_bottom) && (ctx->refresh_left <= ctx->refresh_right))
        {
            page_start = ctx->refresh_top / 8;
            page_end = ctx->refresh_bottom / 8;
            _command(ctx->address, 0x21); // SSD1306_COLUMNADDR
            _command(ctx->address, ctx->refresh_left);    // column start
            _command(ctx->address, ctx->refresh_right);   // column end
            _command(ctx->address, 0x22); // SSD1306_PAGEADDR
            _command(ctx->address, page_start);    // page start
            _command(ctx->address, page_end); // page end
            k = 0;
            for (i = page_start; i <= page_end; ++i)
            {
                for (j = ctx->refresh_left; j <= ctx->refresh_right; ++j)
                {
                    if (k == 0)
                    {
                        i2c_start();
                        i2c_write(ctx->address);
                        i2c_write(0x40);
                    }
                    i2c_write(ctx->buffer[i * ctx->width + j]);
                    ++k;
                    if (k == 16)
                    {
                        i2c_stop();
                        k = 0;
                    }

                }
            }
            if (k != 0) // for last batch if stop was not sent
                i2c_stop();
        }
    }
    // reset dirty area
    ctx->refresh_top = 255;
    ctx->refresh_left = 255;
    ctx->refresh_right = 0;
    ctx->refresh_bottom = 0;
}


void ssd1306_draw_pixel(uint8_t id, int8_t x, int8_t y, ssd1306_color_t color)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint16_t index;

    if (ctx == NULL)
        return;

    if ((x >= ctx->width) || (x < 0) || (y >= ctx->height) || (y < 0))
        return;

    index = x + (y / 8) * ctx->width;
    switch (color)
    {
    case SSD1306_COLOR_WHITE:
        ctx->buffer[index] |= (1 << (y & 7));
        break;
    case SSD1306_COLOR_BLACK:
        ctx->buffer[index] &= ~(1 << (y & 7));
        break;
    case SSD1306_COLOR_INVERT:
        ctx->buffer[index] ^= (1 << (y & 7));
        break;
    default:break;
    }
    if (ctx->refresh_left > x) ctx->refresh_left = x;
    if (ctx->refresh_right < x) ctx->refresh_right = x;
    if (ctx->refresh_top > y) ctx->refresh_top = y;
    if (ctx->refresh_bottom < y) ctx->refresh_bottom = y;
}


void ssd1306_draw_hline(uint8_t id, int8_t x, int8_t y, uint8_t w, ssd1306_color_t color)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint16_t index;
    uint8_t mask, t;

    if (ctx == NULL)
        return;
    // boundary check
    if ((x >= ctx->width) || (x < 0) || (y >= ctx->height) || (y < 0))
        return;
    if (w == 0)
        return;
    if (x + w > ctx->width)
        w = ctx->width - x;

    t = w;
    index = x + (y / 8) * ctx->width;
    mask = 1 << (y & 7);
    switch (color)
    {
    case SSD1306_COLOR_WHITE:
        while (t--)
        {
            ctx->buffer[index] |= mask;
            ++index;
        }
        break;
    case SSD1306_COLOR_BLACK:
        mask = ~mask;
        while (t--)
        {
            ctx->buffer[index] &= mask;
            ++index;
        }
        break;
    case SSD1306_COLOR_INVERT:
        while (t--)
        {
            ctx->buffer[index] ^= mask;
            ++index;
        }
        break;
    default:break;
    }
    if (ctx->refresh_left > x) ctx->refresh_left = x;
    if (ctx->refresh_right < x + w - 1) ctx->refresh_right = x + w - 1;
    if (ctx->refresh_top > y) ctx->refresh_top = y;
    if (ctx->refresh_bottom < y) ctx->refresh_bottom = y;
}


void ssd1306_draw_vline(uint8_t id, int8_t x, int8_t y, uint8_t h, ssd1306_color_t color)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint16_t index;
    uint8_t mask, mod, t;

    if (ctx == NULL)
        return;
    // boundary check
    if ((x >= ctx->width) || (x < 0) || (y >= ctx->height) || (y < 0))
        return;
    if (h == 0)
        return;
    if (y + h > ctx->height)
        h = ctx->height - y;

    t = h;
    index = x + (y / 8) * ctx->width;
    mod = y & 7;
    if (mod) // partial line that does not fit into byte at top
    {
        // Magic from Adafruit
        mod = 8 - mod;
        static const uint8_t premask[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
        mask = premask[mod];
        if (t < mod)
            mask &= (0xFF >> (mod - t));
        switch (color)
        {
        case SSD1306_COLOR_WHITE:
            ctx->buffer[index] |= mask;
            break;
        case SSD1306_COLOR_BLACK:
            ctx->buffer[index] &= ~mask;
            break;
        case SSD1306_COLOR_INVERT:
            ctx->buffer[index] ^= mask;
            break;
        default:break;
        }
        if (t < mod)
            goto draw_vline_finish;
        t -= mod;
        index += ctx->width;
    }
    if (t >= 8) // byte aligned line at middle
    {
        switch (color)
        {
        case SSD1306_COLOR_WHITE:
            do
           {
               ctx->buffer[index] = 0xff;
               index += ctx->width;
               t -= 8;
           } while (t >= 8);
            break;
        case SSD1306_COLOR_BLACK:
            do
            {
               ctx->buffer[index] = 0x00;
               index += ctx->width;
               t -= 8;
            } while (t >= 8);
            break;
        case SSD1306_COLOR_INVERT:
            do
            {
                ctx->buffer[index] = ~ctx->buffer[index];
                index += ctx->width;
                t -= 8;
            } while (t >= 8);
            break;
        default:break;
        }
    }
    if (t) // // partial line at bottom
    {
        mod = t & 7;
        static const uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
        mask = postmask[mod];
        switch (color)
        {
        case SSD1306_COLOR_WHITE:
            ctx->buffer[index] |= mask;
            break;
        case SSD1306_COLOR_BLACK:
            ctx->buffer[index] &= ~mask;
            break;
        case SSD1306_COLOR_INVERT:
            ctx->buffer[index] ^= mask;
            break;
        default:break;
        }
    }
draw_vline_finish:
    if (ctx->refresh_left > x) ctx->refresh_left = x;
    if (ctx->refresh_right < x) ctx->refresh_right = x;
    if (ctx->refresh_top > y) ctx->refresh_top = y;
    if (ctx->refresh_bottom < y + h - 1) ctx->refresh_bottom = y + h - 1;
    return;
}


void ssd1306_draw_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, ssd1306_color_t color)
{
    ssd1306_draw_hline(id, x, y, w, color);
    ssd1306_draw_hline(id, x, y + h - 1, w, color);
    ssd1306_draw_vline(id, x, y, h, color);
    ssd1306_draw_vline(id, x + w - 1, y, h, color);
}


void ssd1306_fill_rectangle(uint8_t id, int8_t x, int8_t y, uint8_t w, uint8_t h, ssd1306_color_t color)
{
    // Can be optimized?
    uint8_t i;
    for (i = x; i < x + w; ++i)
        ssd1306_draw_vline(id, i, y, h, color);
}


void ssd1306_draw_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color)
{
    // Refer to http://en.wikipedia.org/wiki/Midpoint_circle_algorithm for the algorithm

    oled_i2c_ctx *ctx = _ctxs[id];
    int8_t x = r;
    int8_t y = 1;
    int16_t radius_err = 1 - x;

    if (ctx == NULL)
        return;

    if (r == 0)
        return;

    ssd1306_draw_pixel(id, x0 - r, y0,     color);
    ssd1306_draw_pixel(id, x0 + r, y0,     color);
    ssd1306_draw_pixel(id, x0,     y0 - r, color);
    ssd1306_draw_pixel(id, x0,     y0 + r, color);

    while (x >= y)
    {
        ssd1306_draw_pixel(id, x0 + x, y0 + y, color);
        ssd1306_draw_pixel(id, x0 - x, y0 + y, color);
        ssd1306_draw_pixel(id, x0 + x, y0 - y, color);
        ssd1306_draw_pixel(id, x0 - x, y0 - y, color);
        if (x != y)
        {
            /* Otherwise the 4 drawings below are the same as above, causing
             * problem when color is INVERT
             */
            ssd1306_draw_pixel(id, x0 + y, y0 + x, color);
            ssd1306_draw_pixel(id, x0 - y, y0 + x, color);
            ssd1306_draw_pixel(id, x0 + y, y0 - x, color);
            ssd1306_draw_pixel(id, x0 - y, y0 - x, color);
        }
        ++y;
        if (radius_err < 0)
        {
            radius_err += 2 * y + 1;
        }
        else
        {
            --x;
            radius_err += 2 * (y - x + 1);
        }

    }
}


void ssd1306_fill_circle(uint8_t id, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    int8_t x = 1;
    int8_t y = r;
    int16_t radius_err = 1 - y;
    int8_t x1;

    if (ctx == NULL)
        return;

    if (r == 0)
        return;

    ssd1306_draw_vline(id, x0, y0 - r, 2 * r + 1, color); // Center vertical line
    while (y >= x)
    {
        ssd1306_draw_vline(id, x0 - x, y0 - y, 2 * y + 1, color);
        ssd1306_draw_vline(id, x0 + x, y0 - y, 2 * y + 1, color);
        if (color != SSD1306_COLOR_INVERT)
        {
            ssd1306_draw_vline(id, x0 - y, y0 - x, 2 * x + 1, color);
            ssd1306_draw_vline(id, x0 + y, y0 - x, 2 * x + 1, color);
        }
        ++x;
        if (radius_err < 0)
        {
            radius_err += 2 * x + 1;
        }
        else
        {
            --y;
            radius_err += 2 * (x - y + 1);
        }
    }

    if (color == SSD1306_COLOR_INVERT)
    {
        x1 = x; // Save where we stopped

        y = 1;
        x = r;
        radius_err = 1 - x;
        ssd1306_draw_hline(id, x0 + x1, y0, r - x1 + 1, color);
        ssd1306_draw_hline(id, x0 - r, y0, r - x1 + 1, color);
        while (x >= y)
        {
            ssd1306_draw_hline(id, x0 + x1, y0 - y, x - x1 + 1, color);
            ssd1306_draw_hline(id, x0 + x1, y0 + y, x - x1 + 1, color);
            ssd1306_draw_hline(id, x0 - x,  y0 - y, x - x1 + 1, color);
            ssd1306_draw_hline(id, x0 - x,  y0 + y, x - x1 + 1, color);
            ++y;
            if (radius_err < 0)
            {
                radius_err += 2 * y + 1;
            }
            else
            {
                --x;
                radius_err += 2 * (y - x + 1);
            }
        }
    }
}


void ssd1306_select_font(uint8_t id, uint8_t idx)
{
    oled_i2c_ctx *ctx = _ctxs[id];

    if (ctx == NULL)
            return;
    if (idx < NUM_FONTS)
        ctx->font = fonts[idx];
}


// return character width
uint8_t ssd1306_draw_char(uint8_t id, uint8_t x, uint8_t y, unsigned char c, ssd1306_color_t foreground, ssd1306_color_t background)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint8_t i, j;
    const uint8_t *bitmap;
    uint8_t line=0;

    if (ctx == NULL)
        return 0;

    if (ctx->font == NULL)
        return 0;

    // we always have space in the font set
    if ((c < ctx->font->char_start) || (c > ctx->font->char_end))
        c = ' ';
    c = c - ctx->font->char_start;   // c now become index to tables
    bitmap = ctx->font->bitmap + ctx->font->char_descriptors[c].offset;
    for (j = 0; j < ctx->font->height; ++j)
    {
        for (i = 0; i < ctx->font->char_descriptors[c].width; ++i)
        {
            if (i % 8 == 0)
            {
                line = bitmap[(ctx->font->char_descriptors[c].width + 7) / 8 * j + i / 8]; // line data
            }
            if (line & 0x80)
            {
                ssd1306_draw_pixel(id, x + i, y + j, foreground);
            }
            else
            {
                switch (background)
                {
                case SSD1306_COLOR_TRANSPARENT:
                    // Not drawing for transparent background
                    break;
                case SSD1306_COLOR_WHITE:
                case SSD1306_COLOR_BLACK:
                    ssd1306_draw_pixel(id, x + i, y + j, background);
                    break;
                case SSD1306_COLOR_INVERT:
                    // I don't know why I need invert background
                    break;
                }
            }
            line = line << 1;
        }
    }
    return (ctx->font->char_descriptors[c].width);
}


uint8_t ssd1306_draw_string(uint8_t id, uint8_t x, uint8_t y, char *str, ssd1306_color_t foreground, ssd1306_color_t background)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint8_t t = x;

    if (ctx == NULL)
        return 0;

    if (ctx->font == NULL)
        return 0;

    if (str == NULL)
        return 0;

    while (*str)
    {
       x += ssd1306_draw_char(id, x, y, *str, foreground, background);
       ++str;
       if (*str)
           x += ctx->font->c;
    }

    return (x - t);
}


// return width of string
uint8_t ssd1306_measure_string(uint8_t id, char *str)
{
    oled_i2c_ctx *ctx = _ctxs[id];
    uint8_t w = 0;
    unsigned char c;

    if (ctx == NULL)
        return 0;

    if (ctx->font == NULL)
        return 0;

    while (*str)
    {
        c = *str;
        // we always have space in the font set
        if ((c < ctx->font->char_start) || (c > ctx->font->char_end))
            c = ' ';
        c = c - ctx->font->char_start;   // c now become index to tables
        w += ctx->font->char_descriptors[c].width;
        ++str;
       if (*str)
           w += ctx->font->c;
    }
    return w;
}


uint8_t ssd1306_get_font_height(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];

    if (ctx == NULL)
        return 0;

    if (ctx->font == NULL)
        return 0;

    return (ctx->font->height);
}


uint8_t ssd1306_get_font_c(uint8_t id)
{
    oled_i2c_ctx *ctx = _ctxs[id];

    if (ctx == NULL)
        return 0;

    if (ctx->font == NULL)
        return 0;

    return (ctx->font->c);
}


void ssd1306_invert_display(uint8_t id, bool invert)
{
    oled_i2c_ctx *ctx = _ctxs[id];

    if (ctx == NULL)
        return;

    if (invert)
        _command(ctx->address, 0xa7); // SSD1306_INVERTDISPLAY
    else
        _command(ctx->address, 0xa6); // SSD1306_NORMALDISPLAY

}


void ssd1306_update_buffer(uint8_t id, uint8_t* data, uint16_t length)
{
    oled_i2c_ctx *ctx = _ctxs[id];

    if (ctx == NULL)
        return;

    if (ctx->type == SSD1306_128x64)
    {
        memcpy(ctx->buffer, data, (length < 1024) ? length : 1024);

    }
    else if (ctx->type == SSD1306_128x32)
    {
        memcpy(ctx->buffer, data, (length < 512) ? length : 512);
    }
    ctx->refresh_right = ctx->width - 1;
    ctx->refresh_bottom = ctx->height - 1;
    ctx->refresh_top = 0;
    ctx->refresh_left = 0;
}

