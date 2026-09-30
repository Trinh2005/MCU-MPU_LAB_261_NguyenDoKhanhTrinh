/*
 ******************************************************************************
 * @file    lab_config.h
 * @brief   LAB 2 - Timer Interrupt and LED Scanning
 *
 *          Toàn bộ 10 bài tập của LAB 2 nằm chung trong một project.
 *          Chọn bài muốn build bằng cách đổi giá trị của macro EXERCISE
 *          bên dưới, sau đó Build (Ctrl+B) và nạp file .elf vào Proteus.
 ******************************************************************************
 */

#ifndef INC_LAB_CONFIG_H_
#define INC_LAB_CONFIG_H_

/* ==========================================================================
 *                      >>>  CHỌN BÀI TẬP TẠI ĐÂY  <<<
 *
 *   EXERCISE = 1  : 2 LED 7 đoạn hiển thị "1" và "2", quét 500ms   (bài 3.1)
 *   EXERCISE = 2  : 4 LED 7 đoạn "1 2 3 0" + 2 LED DOT nháy 1s     (bài 3.2)
 *   EXERCISE = 3  : hàm update7SEG(index) với led_buffer[4]        (bài 3.3)
 *   EXERCISE = 4  : tần số quét 4 LED 7 đoạn = 1Hz (250ms / LED)   (bài 3.4)
 *   EXERCISE = 5  : đồng hồ số, cập nhật bằng HAL_Delay(1000)      (bài 3.5)
 *   EXERCISE = 6  : software timer + demo nháy LED PA5             (bài 3.6)
 *   EXERCISE = 7  : đồng hồ số dùng software timer, DOT ở main     (bài 3.7)
 *   EXERCISE = 8  : chuyển update7SEG ra main, ngắt chỉ timer_run  (bài 3.8)
 *   EXERCISE = 9  : thêm LED matrix 8x8 hiển thị ký tự 'A'         (bài 3.9)
 *   EXERCISE = 10 : animation - dịch chữ sang trái trên LED matrix (bài 3.10)
 * ========================================================================== */
#define EXERCISE                1

#if (EXERCISE < 1) || (EXERCISE > 10)
#error "EXERCISE chi duoc nhan gia tri tu 1 den 10"
#endif

/* --------------------------------------------------------------------------
 * Chu kỳ ngắt của Timer 2 (ms)
 *   f_TIM2 = 8MHz / (PSC + 1) / (ARR + 1) = 8MHz / 8000 / 10 = 100Hz
 *   -> T    = 1 / 100Hz = 10ms
 * -------------------------------------------------------------------------- */
#define TIMER_CYCLE             10

/* Đổi mili-giây sang số lần ngắt của Timer 2 */
#define MS_TO_TICK(ms)          ((ms) / TIMER_CYCLE)

/* --------------------------------------------------------------------------
 * Các mốc thời gian dùng chung cho các bài tập (đơn vị: ms)
 * -------------------------------------------------------------------------- */
#define SCAN_PERIOD_500MS       500     /* bài 1..3: mỗi LED 7 đoạn sáng 500ms */
#define SCAN_PERIOD_1HZ         250     /* bài 4+  : 4 x 250ms = 1s  -> 1Hz     */
#define DOT_PERIOD              1000    /* chu kỳ nhấp nháy của 2 LED DOT       */
#define CLOCK_PERIOD            1000    /* 1 giây của đồng hồ số                */
#define BLINK_PERIOD            1000    /* bài 6: chu kỳ nháy LED đỏ PA5        */

/* Riêng cho LED matrix (bài 9, 10) */
#define MATRIX_SCAN_PERIOD      10      /* 8 cột x 10ms = 80ms -> 12.5Hz        */
#define MATRIX_SHIFT_PERIOD     200     /* tốc độ dịch chuyển của animation     */
#define MATRIX_SCROLL_TEXT      "HCMUT "

/* Giờ khởi tạo của đồng hồ số (bài 5 trở đi) */
#define CLOCK_INIT_HOUR         15
#define CLOCK_INIT_MINUTE       8
#define CLOCK_INIT_SECOND       50

#endif /* INC_LAB_CONFIG_H_ */
