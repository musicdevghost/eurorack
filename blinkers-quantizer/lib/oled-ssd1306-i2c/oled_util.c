#include "driver/i2c.h"
#include "esp_log.h"
#include "fonts.h"
#include "ssd1306.h"
#include "oled_util.h"
#include "string.h"
#include <math.h>

#define OLED_SCL_PIN 22
#define OLED_SDA_PIN 21
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

int OLED_ID = 0;

void init_oled(int id) {
	OLED_ID = id;
	
     if (ssd1306_init(OLED_ID, OLED_SCL_PIN, OLED_SDA_PIN)) {
		ESP_LOGI("OLED", "oled inited");
	} else {
		ESP_LOGE("OLED", "oled init failed");
	}

	return;
}

void ssd1306_draw_line(int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        ssd1306_draw_pixel(OLED_ID, x0, y0, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void oled_draw_circle_pulse() {
    int radius = 8;
    int x = OLED_WIDTH / 2;
    int y = OLED_HEIGHT / 2;

    ssd1306_clear(OLED_ID);
    ssd1306_draw_circle(OLED_ID, x, y, radius, 1);
    ssd1306_refresh(OLED_ID, 1);

    vTaskDelay(1 / portTICK_PERIOD_MS);
    
    ssd1306_clear(OLED_ID);
    ssd1306_refresh(OLED_ID, 1);
}

void oled_display_boot() {
    char *line1 = "VOXMACHINA";
    char *line2 = "BLINKERS";
    int text_len_1 = strlen(line1);
    int text_len_2 = strlen(line2);

    ssd1306_clear(OLED_ID);
    ssd1306_select_font(OLED_ID, 1);

    uint8_t font_height = ssd1306_get_font_height(OLED_ID);
    
    int x1 = (OLED_WIDTH - (text_len_1 * 6)) / 2;
    int y1 = (OLED_HEIGHT - font_height) / 2;
    int x2 = (OLED_WIDTH - (text_len_2 * 6)) / 2;
    int y2 = y1 + font_height;

    ssd1306_draw_string(OLED_ID, x1, y1, line1, 1, 0);
    ssd1306_draw_string(OLED_ID, x2 + 3, y2 + 4, line2, 1, 0);

    ssd1306_refresh(OLED_ID, 1);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ssd1306_clear(OLED_ID);
    ssd1306_refresh(OLED_ID, 1);
}

void oled_display_text_centered(char *text) {
    int text_len = strlen(text);

    int x = (OLED_WIDTH - (text_len * 6)) / 2;
    int y = (OLED_HEIGHT - 8) / 2;
    
	ssd1306_select_font(OLED_ID, 1);
    ssd1306_draw_string(OLED_ID, x, y, text, 1, 0);
	ssd1306_refresh(OLED_ID, true);
}

void oled_set_loading() {
	int x = OLED_WIDTH / 2;
    int y = OLED_HEIGHT / 2;
    int radius = 16;
    int num_spokes = 4;
    int spoke_length = radius / 2;


	// Animate the spinning wheel by rotating it
	for (int angle = 0; angle < 360; angle += 10) {
		ssd1306_clear(OLED_ID);

		// Draw the center of the wheel
		ssd1306_draw_circle(OLED_ID, x, y, radius / 8, 1);

		// Draw the spokes of the wheel
		for (int i = 0; i < num_spokes; i++) {
			double spoke_angle = (2 * M_PI * i) / num_spokes + angle;
			int spoke_x1 = x + radius * cos(spoke_angle);
			int spoke_y1 = y + radius * sin(spoke_angle);
			int spoke_x2 = x + (radius - spoke_length) * cos(spoke_angle);
			int spoke_y2 = y + (radius - spoke_length) * sin(spoke_angle);
			ssd1306_draw_line(spoke_x1, spoke_y1, spoke_x2, spoke_y2, 1);
		}

		ssd1306_refresh(OLED_ID, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
