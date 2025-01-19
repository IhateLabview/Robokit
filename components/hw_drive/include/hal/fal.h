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

#include <stdint.h>

// Es wird nur eine Option angegeben pro Kommando!
enum {
	E_FAL_OPTION_DISABLE = 0,
	E_FAL_OPTION_CALIBRATE,
	E_FAL_OPTION_ENABLE
};



/**
 * @brief Initializes the necessary peripherals and configurations for FAL (Follow A Line) operation.
 *
 * This function sets up the GPIO pins for RGB output and configures the ADC channels
 * for the FAL sensors. Additionally, it registers the command handler for FAL commands.
 */
void fal_init(void);

/**
 * @brief Checks if the calibration process is completed.
 *
 * This function returns the current calibration status of the system,
 * indicating whether the calibration has been successfully finished or not.
 *
 * @return uint8_t Non-zero if calibrated, zero if not calibrated.
 */
uint8_t fal_is_calibrated(void);

/**
 * @brief Updates the status of the fal (feedback and light) system and handles related commands.
 *
 * The function controls the color reading sequence, checks the drive command status,
 * and initiates commands accordingly if the system is running.
 * It alternates between reading red, green, and blue using internal sensor functions
 * and renders the result once the cycle is complete.
 */
void fal_update(void);

/**
 * @brief Returns the character representation of a color index.
 *
 * This function maps a color index to a character representation based on a predefined array
 * of colors. The mapping is defined for indices from 0 to 7.
 *
 * @param[in] color uint8_t The index of the color.
 *
 * @return unsigned char The character representing the color if the index is valid, otherwise '-'.
 */
unsigned char fal_name_of_color(uint8_t color);

#endif //FAL_H