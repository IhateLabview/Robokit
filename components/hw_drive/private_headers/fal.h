/*
 * MIT License
 *
 * Copyright (c) 2024 Th. Abplanalp, F. Romer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FAL_H
#define FAL_H

#define GPIO_RED   10  // GPIO_10 als Ausgang für Rot
#define GPIO_GREEN 11  // GPIO_11 als Ausgang für Grün
#define GPIO_BLUE  12  // GPIO_12 als Ausgang für Blau

#define ROBOKIT_FAL_BLACK			0b000
#define ROBOKIT_FAL_RED				0b100
#define ROBOKIT_FAL_GREEN			0b010
#define ROBOKIT_FAL_BLUE			0b001
#define ROBOKIT_FAL_YELLOW			0b110
#define ROBOKIT_FAL_MAGENTA			0b101
#define ROBOKIT_FAL_CYAN			0b011
#define ROBOKIT_FAL_WHITE			0b111

#include <stdint.h>

// Es wird nur eine Option angegeben pro Kommando!
enum {
	E_FAL_OPTION_DISABLE = 0,
	E_FAL_OPTION_CALIBRATE,
	E_FAL_OPTION_ENABLE
};

typedef struct {
	uint16_t fb_1_left:3;
	uint16_t fb_2_middle_left:3;
	uint16_t fb_3_middle:3;
	uint16_t fb_4_middle_right:3;
	uint16_t fb_5_right:3;
} S_Fal_Result;

void fal_init(void);
uint8_t fal_is_calibrated(void);
void fal_update(void);

unsigned char fal_name_of_color(uint8_t color);


void fal_set_line_result_interpreter(void (*interpreter) (S_Fal_Result *));

void fal_default_line_result_interpreter(S_Fal_Result *result);

#endif //FAL_H
