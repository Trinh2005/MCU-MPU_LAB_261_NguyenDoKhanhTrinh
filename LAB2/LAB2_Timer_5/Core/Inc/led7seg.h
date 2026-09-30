/*
 ******************************************************************************
 * @file    led7seg.h
 * @brief   Driver quét 4 LED 7 đoạn loại common anode.
 *
 *          Phần cứng (xem schematic Proteus):
 *            - SEG0..SEG6 (a..g) -> PB0..PB6, nối chung cho cả 4 LED
 *            - EN0..EN3          -> PA6..PA9, điều khiển 4 transistor PNP
 *            - DOT               -> PA4, điều khiển 2 LED ở giữa đồng hồ
 ******************************************************************************
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#include "main.h"

#define MAX_LED         4       /* số LED 7 đoạn trên mạch            */
#define LED_BLANK       10      /* mã đặc biệt: tắt toàn bộ 7 đoạn    */

/* Dữ liệu hiển thị của 4 LED 7 đoạn và chỉ số LED đang được quét */
extern int led_buffer[MAX_LED];
extern int index_led;

void display7SEG(int num);
void enable7SEG(int index);
void update7SEG(int index);
void updateClockBuffer(int hour, int minute);
void setDot(int on);
void toggleDot(void);

#endif /* INC_LED7SEG_H_ */
