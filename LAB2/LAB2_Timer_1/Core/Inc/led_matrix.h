/*
 ******************************************************************************
 * @file    led_matrix.h
 * @brief   Driver quét LED matrix 8x8 (bài 3.9 và 3.10)
 *
 *          Phần cứng (xem schematic Proteus):
 *            - ROW0..ROW7 -> PB8..PB15, nối trực tiếp tới anode các hàng
 *              => ghi mức 1 thì LED của hàng đó sáng.
 *            - ENM0..ENM7 -> PA0..PA3, PA10..PA12, PA15, đưa vào ULN2803.
 *              ULN2803 là mảng transistor NPN đảo (cực thu hở), ngõ vào mức 1
 *              sẽ kéo cột COLx xuống mass => cột đó được chọn.
 *
 *          matrix_buffer[i] chứa dữ liệu 8 hàng của cột thứ i
 *          (bit0 = ROW0 ở trên cùng ... bit7 = ROW7 ở dưới cùng).
 ******************************************************************************
 */

#ifndef INC_LED_MATRIX_H_
#define INC_LED_MATRIX_H_

#include "main.h"

#define MAX_LED_MATRIX      8       /* LED matrix 8 cột            */
#define SCROLL_MAX_COLUMNS  64      /* số cột tối đa của chuỗi chạy */

extern uint8_t matrix_buffer[MAX_LED_MATRIX];
extern int index_led_matrix;

void clearLEDMatrix(void);
void updateLEDMatrix(int index);
void loadCharToMatrix(char c);

/* Dùng cho animation của bài 10 */
void initScrollText(const char *text);
void shiftLEDMatrixLeft(void);

#endif /* INC_LED_MATRIX_H_ */
