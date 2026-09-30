/*
 ******************************************************************************
 * @file    software_timer.h
 * @brief   Software timer (bài 3.6 - Program 1.6)
 *
 *          Mỗi software timer là một bộ đếm xuống, được giảm một đơn vị sau
 *          mỗi lần ngắt Timer 2 (TIMER_CYCLE = 10ms). Khi bộ đếm về 0 thì cờ
 *          timerX_flag được bật lên 1. Nhờ vậy hàm main không cần HAL_Delay()
 *          (non-blocking delay), tránh làm nghẽn chương trình.
 *
 *          LAB 2 dùng tối đa 5 timer:
 *            timer0 - đồng hồ số (1s) / demo nháy LED ở bài 6
 *            timer1 - quét 4 LED 7 đoạn
 *            timer2 - nhấp nháy 2 LED DOT
 *            timer3 - quét LED matrix 8x8
 *            timer4 - dịch chuyển animation trên LED matrix
 ******************************************************************************
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "lab_config.h"

/* Các biến này được ghi trong ngắt và đọc trong main -> phải là volatile */
extern volatile int timer0_flag;
extern volatile int timer1_flag;
extern volatile int timer2_flag;
extern volatile int timer3_flag;
extern volatile int timer4_flag;

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void setTimer4(int duration);

/* Được gọi trong HAL_TIM_PeriodElapsedCallback, mỗi TIMER_CYCLE ms */
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
