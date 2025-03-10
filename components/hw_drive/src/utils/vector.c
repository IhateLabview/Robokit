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

#include "vector.h"
#include <math.h>

const S_vector ROBOKIT_INVALID_VECTOR = {-128, -128};

/**
 * @brief Baut einen Vektor auf, ohne den Winkel anzupassen.
 *
 * @param angle Integer Winkel in Grad
 * @param length Integer Länge als Geschwindigkeit
 * @return S_Vector
 */
static S_vector _robokit_make_vector_polar(int8_t angle, T_Speed length) {
    if(length > ROBOKIT_MAX_SPEED || length < ROBOKIT_MIN_SPEED)
        return ROBOKIT_INVALID_VECTOR;
    
    S_vector vec = {angle, length};
    return vec;
}

/**
 * @brief
 * @param angle Integer Winkel zwischen -180 und 180 Grad.
 * @param length Integer Länge als Geschwindigkeit
 * @return S_Vector
 */
S_vector robokit_make_vector_polar(int16_t angle, T_Speed length) {
    if(length > ROBOKIT_MAX_SPEED || length < ROBOKIT_MIN_SPEED)
        return ROBOKIT_INVALID_VECTOR;
    
    S_vector vec = {angle / 3, length};
    return vec;
}

/**
 * @brief Baut einen Vektor aufgrund eines Joysticks auf.
 *
 * @param x Integer Links oder Rechts Ausrichtung der Steuerung
 * @param y Integer Vorwärts- oder Rückwärtsbewegung der Steuerung
 * @return S_Vector
 */
S_vector robokit_make_vector(int8_t x, int8_t y) {
    float angle = (float)atan2(x, y) * 180.0f / (float)M_PI;
    
    return _robokit_make_vector_polar(
     angle >= 0 ? (angle + 1.5f) / 3.0f : (angle - 1.5f) / 3.0f,
     sqrt( x*x + y*y )
    );
}

/**
 * @brief Vergleicht zwei Vektoren auf Gleichheit.
 * @param vec1 S_vector
 * @param vec2 S_vector
 * @return 1, wenn beide gleich sind, sonst 0.
 */
uint8_t robokit_vector_equals(S_vector vec1, S_vector vec2) {
    return vec1.angle == vec2.angle && vec1.speed == vec2.speed;
}
