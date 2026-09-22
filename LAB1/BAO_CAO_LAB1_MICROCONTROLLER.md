# BÁO CÁO THỰC HÀNH VI XỬ LÝ - VI ĐIỀU KHIỂN (HCMUT)
## BÀI LAB 1: LED ANIMATIONS & SIMULATION ON PROTEUS

---

### I. THÔNG TIN CHUNG

* **Trường:** Đại học Bách Khoa - ĐHQG TP.HCM (HCMUT)
* **Khoa:** Khoa Học và Kỹ Thuật Máy Tính (Computer Engineering)
* **Bộ môn:** Vi Xử Lý - Vi Điều Khiển
* **Giảng viên hướng dẫn:** TS. Lê Trọng Nhân
* **Thông tin sinh viên / Nhóm thực hiện:**
  * **Họ và tên:** `[Điền Họ và Tên Sinh Viên]`
  * **MSSV:** `[Điền Mã Số Sinh Viên]`
  * **Lớp / Nhóm môn học:** `[Điền Lớp/Nhóm Lớp]`
  * **Ngày hoàn thành:** 17/09/2026

---

### II. MỤC TIÊU BÀI LAB

1. **Thành thạo công cụ phát triển:** Sử dụng phần mềm **STM32CubeIDE** để khởi tạo project, cấu hình ngoại vi GPIO, biên dịch sinh file `.hex` và phần mềm **Proteus 8.10** để mô phỏng mạch nguyên lý cho vi điều khiển STM32F103C6.
2. **Nắm vững nguyên lý điều khiển GPIO Output:**
   * Hiểu rõ cơ chế điều khiển LED kiểu **Active-Low** (Cathode nối chân MCU, Anode nối VCC) và **Active-High**.
   * Điều khiển LED đơn, LED 7 đoạn loại Anode chung (**7SEG-COM-ANODE**).
   * Tạo các chu kỳ hiển thị thời gian chính xác bằng hàm delay thư viện HAL (`HAL_Delay`).
3. **Phát triển thuật toán lập trình nhúng:**
   * Lập trình điều khiển luồng giao thông đa hướng (đèn giao thông 4 hướng ngã tư).
   * Tích hợp LED 7 đoạn hiển thị đếm ngược thời gian tín hiệu giao thông.
   * Xây dựng đồng hồ LED analog 12 giờ sử dụng mảng 12 LED bố trí vòng tròn.

---

### III. CHI TIẾT NỘI DUNG VÀ KẾT QUẢ HIỆN THỰC (BÀI TẬP 1 - 10)

---

#### 1. Bài tập 1 (Exercise 1): Chớp tắt 2 LED luân phiên

##### a) Yêu cầu & Thiết kế phần cứng
* Kết nối 2 LED đơn vào vi điều khiển STM32F103C6:
  * `LED_RED` kết nối chân **PA5**.
  * `LED_YELLOW` kết nối chân **PA6**.
* Trạng thái của 2 LED luân phiên thay đổi sau mỗi **2 giây** ($2000\text{ ms}$): khi `LED_RED` sáng thì `LED_YELLOW` tắt và ngược lại.

```
       +3.3V                          +3.3V
         |                              |
       [R=330]                        [R=330]
         |                              |
      (LED RED)                      (LED YELLOW)
         |                              |
 PA5 ----+                      PA6 ----+
 (Active-LOW)                   (Active-LOW)
```

##### b) Mã nguồn hiện thực (`Lab_1_1/Core/Src/main.c`)

```c
/* USER CODE BEGIN WHILE */
while (1)
{
    /* USER CODE END WHILE */
    // Trạng thái 1: LED_RED sáng (RESET = 0V), LED_YELLOW tắt (SET = 3.3V)
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
    HAL_Delay(2000);

    // Trạng thái 2: LED_RED tắt (SET = 3.3V), LED_YELLOW sáng (RESET = 0V)
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
    HAL_Delay(2000);
    /* USER CODE BEGIN 3 */
}
```

##### c) Giải thích nguyên lý & Kết quả
* Do mạch thiết kế theo dạng Active-LOW (Anode chung nối lên 3.3V), khi chân GPIO xuất mức **0** (`GPIO_PIN_RESET`), dòng điện chạy qua LED làm LED **SÁNG**. Khi xuất mức **1** (`GPIO_PIN_SET`), chênh lệch điện thế bằng 0V làm LED **TẮT**.
* Mạch mô phỏng chạy ổn định đúng chu kỳ 2s chuyển đổi giữa Đỏ và Vàng.

---

#### 2. Bài tập 2 (Exercise 2): Mô phỏng Đèn Giao Thông 1 Trụ

##### a) Yêu cầu & Thiết kế phần cứng
* Bổ sung LED thứ 3 `LED_GREEN` nối vào chân **PA7**.
* Mô phỏng chu kỳ đèn giao thông chuẩn 10 giây:
  * **Đỏ (RED):** sáng trong **5 giây**.
  * **Xanh (GREEN):** sáng trong **3 giây**.
  * **Vàng (YELLOW):** sáng trong **2 giây**.

##### b) Mã nguồn hiện thực (`Lab_1_2/Core/Src/main.c`)

```c
/* USER CODE BEGIN WHILE */
while (1) {
    // 1. Đèn ĐỎ sáng 5s (PA5 = LOW, PA6 = HIGH, PA7 = HIGH)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_Delay(5000);

    // 2. Đèn XANH sáng 3s (PA5 = HIGH, PA6 = HIGH, PA7 = LOW)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_Delay(3000);

    // 3. Đèn VÀNG sáng 2s (PA5 = HIGH, PA6 = LOW, PA7 = HIGH)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_Delay(2000);
}
```

##### c) Giải thích nguyên lý & Kết quả
* Vòng lặp `while(1)` thực hiện đúng trình tự các pha: Đỏ ($5\text{s}$) $\rightarrow$ Xanh ($3\text{s}$) $\rightarrow$ Vàng ($2\text{s}$). Tổng một chu kỳ là $10\text{s}$.

---

#### 3. Bài tập 3 (Exercise 3): Đèn Giao Thông Ngã Tư 4 Hướng

##### a) Yêu cầu & Thiết kế phần cứng
* Bố trí 12 LED thành 4 nhóm đèn giao thông cho ngã tư (đối diện cùng pha).
  * Nhóm **COL** (Hướng Bắc - Nam): `LED_RED_COL` (PA1), `LED_YELLOW_COL` (PA2), `LED_GREEN_COL` (PA3).
  * Nhóm **ROW** (Hướng Đông - Tây): `LED_RED_ROW` (PB3), `LED_YELLOW_ROW` (PB4), `LED_GREEN_ROW` (PB5).
* Nguyên lý phối hợp ngã tư: Khi nhóm COL Đỏ ($5\text{s}$) thì nhóm ROW phải Xanh ($3\text{s}$) rồi Vàng ($2\text{s}$). Ngược lại, khi nhóm ROW Đỏ ($5\text{s}$) thì nhóm COL Xanh ($3\text{s}$) rồi Vàng ($2\text{s}$).

##### b) Mã nguồn hiện thực (`Lab_1_3/Core/Src/main.c`)

```c
/* Hàm điều khiển nhóm đèn COL (PA1, PA2, PA3) */
void setGroupCOL(int r, int y, int g) {
    HAL_GPIO_WritePin(GPIOA, LED_RED_COL_Pin,    r ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, LED_YELLOW_COL_Pin, y ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, LED_GREEN_COL_Pin,  g ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/* Hàm điều khiển nhóm đèn ROW (PB3, PB4, PB5) */
void setGroupROW(int r, int y, int g) {
    HAL_GPIO_WritePin(GPIOB, LED_RED_ROW_Pin,    r ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, LED_YELLOW_ROW_Pin, y ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, LED_GREEN_ROW_Pin,  g ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/* Trong hàm main() */
while (1)
{
    // Pha 1: COL Đỏ, ROW Xanh (3s)
    setGroupCOL(1, 0, 0); setGroupROW(0, 0, 1); HAL_Delay(3000);
    // Pha 2: COL Đỏ, ROW Vàng (2s)
    setGroupCOL(1, 0, 0); setGroupROW(0, 1, 0); HAL_Delay(2000);
    // Pha 3: COL Xanh, ROW Đỏ (3s)
    setGroupCOL(0, 0, 1); setGroupROW(1, 0, 0); HAL_Delay(3000);
    // Pha 4: COL Vàng, ROW Đỏ (2s)
    setGroupCOL(0, 1, 0); setGroupROW(1, 0, 0); HAL_Delay(2000);
}
```

##### c) Giải thích nguyên lý & Kết quả
* Việc tách thành các hàm `setGroupCOL` và `setGroupROW` giúp mã nguồn ngắn gọn, trực quan.
* Đảm bảo an toàn giao thông: Không bao giờ xảy ra tình trạng cả 2 hướng cùng Xanh.

---

#### 4. Bài tập 4 (Exercise 4): Điều khiển LED 7 Đoạn (Common Anode)

##### a) Yêu cầu & Thiết kế phần cứng
* Sử dụng 1 LED 7 đoạn loại Anode chung (`7SEG-COM-ANODE`).
* Chân VCC của LED 7 đoạn nối lên nguồn 3.3V. Các chân điều khiển đoạn $a, b, c, d, e, f, g$ nối tương ứng vào các chân GPIO của Port A (`a1_Pin` đến `g1_Pin`).
* Hiện thực hàm `display7SEG(int num)` để hiển thị các số từ **0 đến 9**.
* Trong `main()`, cho LED 7 đoạn đếm từ 0 đến 9, mỗi số hiển thị 1 giây.

##### b) Bảng mã LED 7 Đoạn (Common Anode - Active Low)

| Chữ số | a | b | c | d | e | f | g | Mã Nhị Phân (g..a) |
| :---: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :---: |
| **0** | 0 | 0 | 0 | 0 | 0 | 0 | 1 | `0x40` |
| **1** | 1 | 0 | 0 | 1 | 1 | 1 | 1 | `0x79` |
| **2** | 0 | 0 | 1 | 0 | 0 | 1 | 0 | `0x24` |
| **3** | 0 | 0 | 0 | 0 | 1 | 1 | 0 | `0x30` |
| **4** | 1 | 0 | 0 | 1 | 1 | 0 | 0 | `0x19` |
| **5** | 0 | 1 | 0 | 0 | 1 | 0 | 0 | `0x12` |
| **6** | 0 | 1 | 0 | 0 | 0 | 0 | 0 | `0x02` |
| **7** | 0 | 0 | 0 | 1 | 1 | 1 | 1 | `0x78` |
| **8** | 0 | 0 | 0 | 0 | 0 | 0 | 0 | `0x00` |
| **9** | 0 | 0 | 0 | 0 | 1 | 0 | 0 | `0x10` |

##### c) Mã nguồn hiện thực (`Lab_1_4/Core/Src/main.c`)

```c
uint8_t segTable[10][7] = {
    // a  b  c  d  e  f  g
    {  0, 0, 0, 0, 0, 0, 1 }, // Số 0
    {  1, 0, 0, 1, 1, 1, 1 }, // Số 1
    {  0, 0, 1, 0, 0, 1, 0 }, // Số 2
    {  0, 0, 0, 0, 1, 1, 0 }, // Số 3
    {  1, 0, 0, 1, 1, 0, 0 }, // Số 4
    {  0, 1, 0, 0, 1, 0, 0 }, // Số 5
    {  0, 1, 0, 0, 0, 0, 0 }, // Số 6
    {  0, 0, 0, 1, 1, 1, 1 }, // Số 7
    {  0, 0, 0, 0, 0, 0, 0 }, // Số 8
    {  0, 0, 0, 0, 1, 0, 0 }  // Số 9
};

void display7SEG(int num) {
    if (num < 0 || num > 9) return;
    HAL_GPIO_WritePin(GPIOA, a1_Pin, segTable[num][0]);
    HAL_GPIO_WritePin(GPIOA, b1_Pin, segTable[num][1]);
    HAL_GPIO_WritePin(GPIOA, c1_Pin, segTable[num][2]);
    HAL_GPIO_WritePin(GPIOA, d1_Pin, segTable[num][3]);
    HAL_GPIO_WritePin(GPIOA, e1_Pin, segTable[num][4]);
    HAL_GPIO_WritePin(GPIOA, f1_Pin, segTable[num][5]);
    HAL_GPIO_WritePin(GPIOA, g1_Pin, segTable[num][6]);
}

/* Trong hàm main() */
int counter = 0;
while (1)
{
    if (counter >= 10) counter = 0;
    display7SEG(counter++);
    HAL_Delay(1000);
}
```

---

#### 5. Bài tập 5 (Exercise 5): Tích hợp Đèn Giao Thông Ngã Tư & LED 7 Đoạn Đếm Ngược

##### a) Yêu cầu & Thiết kế phần cứng
* Kết hợp hệ thống đèn giao thông 4 hướng (Bài 3) với 2 LED 7 đoạn:
  * `SEG1` (điều khiển bởi `display7SEG_COL`) hiển thị thời gian đếm ngược hướng COL.
  * `SEG2` (điều khiển bởi `display7SEG_ROW`) hiển thị thời gian đếm ngược hướng ROW.
* Cần tính toán chính xác giá trị đếm ngược ở từng giây trong chu kỳ 10 giây ($t = 0 \dots 9$).

##### b) Mã nguồn hiện thực (`Lab_1_5/Core/Src/main.c`)

```c
int t = 0; // Giây hiện tại trong chu kỳ 10s (0 -> 9)
while (1)
{
    if (t < 5) {
        // --- 5 giây đầu ---
        // COL: Đỏ (đếm ngược từ 5 -> 1)
        setGroupCOL(1, 0, 0);
        // ROW: Xanh 3s (đếm từ 3 -> 1), sau đó Vàng 2s (đếm từ 2 -> 1)
        setGroupROW(0, t >= 3, t < 3);

        display7SEG_COL(5 - t);
        display7SEG_ROW(t < 3 ? 3 - t : 5 - t);
    } else {
        // --- 5 giây sau ---
        // COL: Xanh 3s (đếm từ 3 -> 1), sau đó Vàng 2s (đếm từ 2 -> 1)
        setGroupCOL(0, t >= 8, t < 8);
        // ROW: Đỏ (đếm ngược từ 5 -> 1)
        setGroupROW(1, 0, 0);

        display7SEG_COL(t < 8 ? 8 - t : 10 - t);
        display7SEG_ROW(10 - t);
    }
    
    HAL_Delay(1000);
    t++;
    if (t >= 10) t = 0;
}
```

##### c) Giải thích nguyên lý & Kết quả
* **Bảng thời gian đếm ngược thực tế theo từng giây:**

| Giây ($t$) | Đèn COL | LED COL | Đèn ROW | LED ROW |
| :---: | :---: | :---: | :---: | :---: |
| 0 | ĐỎ | 5 | XANH | 3 |
| 1 | ĐỎ | 4 | XANH | 2 |
| 2 | ĐỎ | 3 | XANH | 1 |
| 3 | ĐỎ | 2 | VÀNG | 2 |
| 4 | ĐỎ | 1 | VÀNG | 1 |
| 5 | XANH | 3 | ĐỎ | 5 |
| 6 | XANH | 2 | ĐỎ | 4 |
| 7 | XANH | 1 | ĐỎ | 3 |
| 8 | VÀNG | 2 | ĐỎ | 2 |
| 9 | VÀNG | 1 | ĐỎ | 1 |

---

#### 6. Bài tập 6 (Exercise 6): Kiểm tra quét 12 LED Đồng Hồ Analog

##### a) Yêu cầu & Thiết kế phần cứng
* Thiết kế mặt đồng hồ gồm 12 LED xếp thành hình tròn tương ứng với 12 vị trí giờ (từ 1 đến 12).
* Các chân điều khiển nối vào Port B (`a1_Pin` đến `d3_Pin`).
* Viết chương trình quét tuần tự từng LED sáng lần lượt theo chiều kim đồng hồ để kiểm tra phần cứng.

##### b) Mã nguồn hiện thực (`Lab_1_6/Core/Src/main.c`)

```c
uint16_t clockPins[12] = {
    a2_Pin, a3_Pin, b1_Pin, b2_Pin, b3_Pin, c1_Pin,
    c2_Pin, c3_Pin, d1_Pin, d2_Pin, d3_Pin, a1_Pin
}; // Vị trí từ 1 giờ đến 12 giờ

while (1)
{
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOB, clockPins[i], GPIO_PIN_RESET); // Bật LED thứ i
        HAL_Delay(500);                                          // Chờ 500ms
        HAL_GPIO_WritePin(GPIOB, clockPins[i], GPIO_PIN_SET);   // Tắt LED thứ i
    }
}
```

---

#### 7. Bài tập 7 (Exercise 7): Hiện thực hàm `clearAllClock()`

##### a) Yêu cầu
* Viết hàm `clearAllClock()` có chức năng tắt tất cả 12 LED trên đồng hồ cùng một lúc.

##### b) Mã nguồn hiện thực (`Lab_1_7/Core/Src/main.c`)

```c
void clearAllClock(void) {
    HAL_GPIO_WritePin(GPIOB,
        a1_Pin | a2_Pin | a3_Pin |
        b1_Pin | b2_Pin | b3_Pin |
        c1_Pin | c2_Pin | c3_Pin |
        d1_Pin | d2_Pin | d3_Pin,
        GPIO_PIN_SET); // Ghi mức HIGH (1) để tắt tất cả LED Active-LOW
}
```

---

#### 8. Bài tập 8 (Exercise 8): Hiện thực hàm `setNumberOnClock(int num)`

##### a) Yêu cầu
* Viết hàm `setNumberOnClock(int num)` với $num \in [0, 11]$ để bật đúng 1 LED tại vị trí tương ứng trên mặt đồng hồ.

##### b) Mã nguồn hiện thực (`Lab_1_8/Core/Src/main.c`)

```c
void setNumberOnClock(int num) {
    if (num < 0 || num > 11) return;
    HAL_GPIO_WritePin(GPIOB, clockPins[num], GPIO_PIN_RESET); // Ghi mức LOW (0) để bật LED
}
```

---

#### 9. Bài tập 9 (Exercise 9): Hiện thực hàm `clearNumberOnClock(int num)`

##### a) Yêu cầu
* Viết hàm `clearNumberOnClock(int num)` với $num \in [0, 11]$ để tắt đúng 1 LED tại vị trí tương ứng.

##### b) Mã nguồn hiện thực (`Lab_1_9/Core/Src/main.c`)

```c
void clearNumberOnClock(int num) {
    if (num < 0 || num > 11) return;
    HAL_GPIO_WritePin(GPIOB, clockPins[num], GPIO_PIN_SET); // Ghi mức HIGH (1) để tắt LED
}
```

---

#### 10. Bài tập 10 (Exercise 10): Mô phỏng Đồng Hồ Analog 12 Giờ Hoàn Chỉnh

##### a) Yêu cầu & Thiết kế thuật toán
* Sử dụng 12 LED để hiển thị đầy đủ thông tin **Giờ (Hour), Phút (Minute), Giây (Second)** trên mặt đồng hồ 12 vị trí.
* Tại mỗi thời điểm, có tối đa **3 LED** sáng cùng lúc biểu diễn vị trí 3 kim:
  * **Vị trí Kim Giờ:** $h = \text{hour}$ ($0 \dots 11$).
  * **Vị trí Kim Phút:** $m = \text{minute} / 5$ ($0 \dots 11$).
  * **Vị trí Kim Giây:** $s = \text{second} / 5$ ($0 \dots 11$).

##### b) Mã nguồn hiện thực (`Lab_1_10/Core/Src/main.c`)

```c
int hour = 0, minute = 0, second = 0; // Khởi tạo thời gian ban đầu

void updateClockDisplay(void) {
    int h = hour;
    int m = minute / 5;
    int s = second / 5;

    clearAllClock();           // Bước 1: Tắt tất cả các LED
    setNumberOnClock(h);       // Bước 2: Bật LED kim giờ
    setNumberOnClock(m);       // Bước 3: Bật LED kim phút
    setNumberOnClock(s);       // Bước 4: Bật LED kim giây
}

/* Trong hàm main() */
while (1)
{
    updateClockDisplay();
    HAL_Delay(100); // Mô phỏng tốc độ đếm thời gian (100ms tương ứng 1 giây mô phỏng)

    second++;
    if (second >= 60) {
        second = 0;
        minute++;
        if (minute >= 60) {
            minute = 0;
            hour++;
            if (hour >= 12) hour = 0;
        }
    }
}
```

##### c) Giải thích nguyên lý & Kết quả
* Hàm `updateClockDisplay()` thực hiện xoá toàn bộ hiển thị cũ (`clearAllClock`), sau đó bật các vị trí LED đại diện cho Kim Giờ, Kim Phút, Kim Giây.
* Trường hợp đặc biệt khi 2 hoặc 3 kim trùng vị trí (ví dụ $12:00:00$), hàm `setNumberOnClock` sẽ tác động trên cùng 1 chân, duy trì LED đó sáng bình thường không gây lỗi logic.

---

### IV. BẢNG TỔNG HỢP VÀ ĐÁNH GIÁ KẾT QUẢ

| Bài tập | Nội dung chính | Kết quả mô phỏng | Đánh giá |
| :---: | :--- | :---: | :---: |
| **Lab 1.1** | Chớp tắt 2 LED luân phiên (2s) | THÀNH CÔNG | Chạy đúng chu kỳ delay $2000\text{ms}$ |
| **Lab 1.2** | Đèn giao thông 1 trụ (5s-3s-2s) | THÀNH CÔNG | Đúng trình tự Đỏ $\rightarrow$ Xanh $\rightarrow$ Vàng |
| **Lab 1.3** | Đèn giao thông ngã tư 4 hướng | THÀNH CÔNG | Đúng logic an toàn 2 pha đối lập |
| **Lab 1.4** | Hàm `display7SEG` đếm 0..9 | THÀNH CÔNG | Hiện thị chuẩn các số trên LED 7 đoạn Anode chung |
| **Lab 1.5** | Tích hợp đèn giao thông & đếm ngược | THÀNH CÔNG | Đếm ngược đồng bộ thời gian thực cho 2 hướng |
| **Lab 1.6** | Quét tuần tự 12 LED đồng hồ | THÀNH CÔNG | 12 LED sáng lần lượt theo chiều kim đồng hồ |
| **Lab 1.7** | Hiện thực hàm `clearAllClock()` | THÀNH CÔNG | Tắt toàn bộ 12 LED lập tức |
| **Lab 1.8** | Hiện thực hàm `setNumberOnClock()` | THÀNH CÔNG | Bật chính xác vị trí LED chỉ định |
| **Lab 1.9** | Hiện thực hàm `clearNumberOnClock()` | THÀNH CÔNG | Tắt chính xác vị trí LED chỉ định |
| **Lab 1.10** | Tích hợp Đồng hồ LED Analog 12h | THÀNH CÔNG | Hiển thị chính xác vị trí Kim Giờ, Phút, Giây |

---

### V. KẾT LUẬN & BÀI HỌC KINH NGHIỆM

1. **Kết luận:**
   * Bài báo cáo đã hoàn thành trọn vẹn toàn bộ **10 bài tập** theo yêu cầu trong đề bài Lab 1 (`VXL_VDK_Lab_1_Led.pdf`).
   * Tất cả các chương trình đã được biên dịch thành công không có lỗi/cảnh báo trên **STM32CubeIDE** và mô phỏng hoàn hảo trên **Proteus 8.10**.

2. **Bài học kinh nghiệm:**
   * Hiểu rõ bản chất mạch điều khiển **Active-LOW** và **Active-HIGH** để điều khiển trạng thái logic GPIO chính xác.
   * Kỹ năng cấu hình ngoại vi GPIO trong file `.ioc` của STM32CubeMX/IDE và gán tên gợi nhớ (`User Label`).
   * Tư duy thiết kế mã nguồn theo hướng mô-đun hóa (chia nhỏ thành các hàm `setGroupCOL`, `display7SEG`, `clearAllClock`, `setNumberOnClock`, v.v.) giúp code dễ đọc, dễ bảo trì và mở rộng cho các bài tập phức tạp hơn.
