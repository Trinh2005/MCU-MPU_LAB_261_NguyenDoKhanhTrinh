/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* =====================================================================
 * 1. SOFTWARE TIMER
 *    TIMER_CYCLE = chu ky ngat TIM2 (ms). Bai 4: doi ngat chi sua dong nay.
 * ===================================================================== */
#define TIMER_CYCLE  10

#define NUM_TIMERS   4
#define TMR_SECOND   0   /* dem giay o mode 1        */
#define TMR_SCAN     1   /* quet LED 7 doan          */
#define TMR_BLINK    2   /* nhay den 2 Hz o mode 2-4 */
#define TMR_REPEAT   3   /* tu tang khi giu nut 2    */

volatile int timer_counter[NUM_TIMERS];
volatile int timer_flag[NUM_TIMERS];

void setTimer(int id, int duration) {        /* duration tinh bang ms */
    int ticks = duration / TIMER_CYCLE;
    if (ticks < 1) ticks = 1;
    timer_counter[id] = ticks;
    timer_flag[id] = 0;
}

int isTimerExpired(int id) {
    return timer_flag[id];
}

void timerRun(void) {
    for (int i = 0; i < NUM_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}

/* =====================================================================
 * 2. DOC NUT NHAN + CHONG DOI PHIM
 *    Nut noi GND, co pull-up 10k ngoai: nhan = 0
 * ===================================================================== */
#define NUM_BUTTONS      3
#define BTN_MODE         0   /* B1 - PA0: doi mode     */
#define BTN_INC          1   /* B2 - PA1: tang gia tri */
#define BTN_SET          2   /* B3 - PA2: luu gia tri  */

#define PRESSED          GPIO_PIN_RESET
#define RELEASED         GPIO_PIN_SET
#define LONG_PRESS_TIME  (1000 / TIMER_CYCLE)   /* 1 giay */

GPIO_TypeDef *btnPort[NUM_BUTTONS] = { B1_GPIO_Port, B2_GPIO_Port, B3_GPIO_Port };
uint16_t      btnPin[NUM_BUTTONS]  = { B1_Pin,       B2_Pin,       B3_Pin };

GPIO_PinState keyReg0[NUM_BUTTONS]     = { RELEASED, RELEASED, RELEASED };
GPIO_PinState keyReg1[NUM_BUTTONS]     = { RELEASED, RELEASED, RELEASED };
GPIO_PinState keyReg2[NUM_BUTTONS]     = { RELEASED, RELEASED, RELEASED };
GPIO_PinState stableState[NUM_BUTTONS] = { RELEASED, RELEASED, RELEASED };

int          pressCounter[NUM_BUTTONS];
volatile int pressFlag[NUM_BUTTONS];
volatile int longFlag[NUM_BUTTONS];

void button_reading(void) {                  /* goi trong ISR moi 10 ms */
    for (int i = 0; i < NUM_BUTTONS; i++) {
        keyReg2[i] = keyReg1[i];
        keyReg1[i] = keyReg0[i];
        keyReg0[i] = HAL_GPIO_ReadPin(btnPort[i], btnPin[i]);

        /* chi tin khi 3 lan doc lien tiep giong nhau */
        if (keyReg0[i] == keyReg1[i] && keyReg1[i] == keyReg2[i]) {
            if (keyReg2[i] != stableState[i]) {          /* vua doi trang thai */
                stableState[i] = keyReg2[i];
                if (stableState[i] == PRESSED) {
                    pressFlag[i] = 1;
                    pressCounter[i] = LONG_PRESS_TIME;
                } else {
                    longFlag[i] = 0;
                }
            } else if (stableState[i] == PRESSED && pressCounter[i] > 0) {
                pressCounter[i]--;                       /* dang giu nut */
                if (pressCounter[i] == 0) longFlag[i] = 1;
            }
        }
    }
}

int isButtonPressed(int index) {             /* tra 1 dung MOT lan moi lan nhan */
    if (pressFlag[index] == 1) {
        pressFlag[index] = 0;
        return 1;
    }
    return 0;
}

int isButtonLongPressed(int index) {         /* = 1 khi dang giu qua 1 giay */
    return longFlag[index];
}

/* =====================================================================
 * 3. HIEN THI: LED 7 DOAN (anode chung, PNP) + DEN GIAO THONG
 * ===================================================================== */
#define LIGHT_OFF     0
#define LIGHT_RED     1
#define LIGHT_GREEN   2
#define LIGHT_YELLOW  3

/* 1 = doan sang. Thu tu cot: a b c d e f g */
const int digit[10][7] = {
    {1,1,1,1,1,1,0},  /* 0 */
    {0,1,1,0,0,0,0},  /* 1 */
    {1,1,0,1,1,0,1},  /* 2 */
    {1,1,1,1,0,0,1},  /* 3 */
    {0,1,1,0,0,1,1},  /* 4 */
    {1,0,1,1,0,1,1},  /* 5 */
    {1,0,1,1,1,1,1},  /* 6 */
    {1,1,1,0,0,0,0},  /* 7 */
    {1,1,1,1,1,1,1},  /* 8 */
    {1,1,1,1,0,1,1}   /* 9 */
};

const uint16_t segA[7] = { SA0_Pin, SA1_Pin, SA2_Pin, SA3_Pin, SA4_Pin, SA5_Pin, SA6_Pin };
const uint16_t segB[7] = { SB0_Pin, SB1_Pin, SB2_Pin, SB3_Pin, SB4_Pin, SB5_Pin, SB6_Pin };

int bufA[2] = {0, 0};      /* cap A: EN0, EN1 */
int bufB[2] = {0, 0};      /* cap B: EN2, EN3 */
int scanIndex = 0;

void writeDigit(const uint16_t seg[7], int num) {      /* RESET = doan sang */
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, seg[i], digit[num][i] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

void setBufferA(int num) { bufA[0] = num / 10; bufA[1] = num % 10; }
void setBufferB(int num) { bufB[0] = num / 10; bufB[1] = num % 10; }

/* Hai cap quet song song. PNP: EN = 0 la sang */
void scan7SEG(void) {
    HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin, GPIO_PIN_SET);   /* 1. tat het */
    HAL_GPIO_WritePin(GPIOB, EN2_Pin | EN3_Pin, GPIO_PIN_SET);

    writeDigit(segA, bufA[scanIndex]);                           /* 2. xuat so */
    writeDigit(segB, bufB[scanIndex]);

    if (scanIndex == 0) {                                        /* 3. bat LED */
        HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
    }
    scanIndex = 1 - scanIndex;
}

/* Den giao thong: cathode xuong GND -> SET = sang */
void setLightHor(int color) {
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, (color == LIGHT_RED)    ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YEL_GPIO_Port, YEL_Pin, (color == LIGHT_YELLOW) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GRN_GPIO_Port, GRN_Pin, (color == LIGHT_GREEN)  ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void setLightVer(int color) {
    HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, (color == LIGHT_RED)    ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YEL1_GPIO_Port, YEL1_Pin, (color == LIGHT_YELLOW) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GRN1_GPIO_Port, GRN1_Pin, (color == LIGHT_GREEN)  ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void toggleColor(int color) {               /* nhay 1 mau o ca hai huong */
    if (color == LIGHT_RED)    HAL_GPIO_TogglePin(GPIOA, RED_Pin | RED1_Pin);
    if (color == LIGHT_YELLOW) HAL_GPIO_TogglePin(GPIOA, YEL_Pin | YEL1_Pin);
    if (color == LIGHT_GREEN)  HAL_GPIO_TogglePin(GPIOA, GRN_Pin | GRN1_Pin);
}

/* =====================================================================
 * 4. FSM (theo so do: Automatic mode / Manual mode)
 * ===================================================================== */
#define INIT        0
#define AUTO        1   /* Mode 1 */
#define MAN_RED     2   /* Mode 2 */
#define MAN_YELLOW  3   /* Mode 3 */
#define MAN_GREEN   4   /* Mode 4 */

int status = INIT;
int statusHor, statusVer;        /* mau hien tai cua 2 huong */
int countHor, countVer;          /* so giay con lai           */
int durRed = 5, durYellow = 2, durGreen = 3;   /* luon giu DO = XANH + VANG */
int tempValue = 0;               /* gia tri dang chinh o mode 2-4 */

int nextColor(int color) {       /* RED -> GREEN -> YELLOW -> RED */
    if (color == LIGHT_RED)   return LIGHT_GREEN;
    if (color == LIGHT_GREEN) return LIGHT_YELLOW;
    return LIGHT_RED;
}

int durationOf(int color) {
    if (color == LIGHT_RED)   return durRed;
    if (color == LIGHT_GREEN) return durGreen;
    return durYellow;
}

int colorOfMode(int mode) {
    if (mode == MAN_RED)    return LIGHT_RED;
    if (mode == MAN_YELLOW) return LIGHT_YELLOW;
    return LIGHT_GREEN;
}

/* Luu gia tri moi, giu DO = XANH + VANG de 2 huong khong cung xanh */
void saveDuration(int color, int value) {
    if (color == LIGHT_RED) {
        if (value < 2) value = 2;
        durRed = value;
        durGreen = durRed - durYellow;               /* sua XANH cho khop */
        if (durGreen < 1) { durGreen = 1; durYellow = durRed - 1; }
    } else {
        if (color == LIGHT_YELLOW) durYellow = value;
        else                       durGreen  = value;
        durRed = durGreen + durYellow;               /* sua DO cho khop */
        if (durRed > 99) {
            durRed = 99;
            if (color == LIGHT_YELLOW) durGreen  = 99 - durYellow;
            else                       durYellow = 99 - durGreen;
        }
    }
}

void enterAuto(void) {
    status = AUTO;
    statusHor = LIGHT_RED;    countHor = durRed;     /* Init (Hor) -> RED   */
    statusVer = LIGHT_GREEN;  countVer = durGreen;   /* Init (Ver) -> GREEN */
    setLightHor(statusHor);
    setLightVer(statusVer);
    setBufferA(countHor);
    setBufferB(countVer);
    setTimer(TMR_SECOND, 1000);
}

void enterManual(int mode) {
    status = mode;
    setLightHor(LIGHT_OFF);
    setLightVer(LIGHT_OFF);
    tempValue = durationOf(colorOfMode(mode));
    setBufferA(tempValue);       /* cap A: gia tri dang chinh  */
    setBufferB(mode);            /* cap B: so mode 02, 03, 04  */
    setTimer(TMR_BLINK, 250);    /* dao moi 250 ms -> nhay 2 Hz */
}

void increaseValue(void) {
    tempValue++;
    if (tempValue > 99) tempValue = 1;
    setBufferA(tempValue);
}

void fsm_run(void) {
    switch (status) {
    case INIT:
        setTimer(TMR_SCAN, 10);
        enterAuto();
        break;

    case AUTO:
        if (isTimerExpired(TMR_SECOND)) {
            setTimer(TMR_SECOND, 1000);
            countHor--;
            if (countHor <= 0) { statusHor = nextColor(statusHor); countHor = durationOf(statusHor); }
            countVer--;
            if (countVer <= 0) { statusVer = nextColor(statusVer); countVer = durationOf(statusVer); }
            setLightHor(statusHor);
            setLightVer(statusVer);
            setBufferA(countHor);
            setBufferB(countVer);
        }
        isButtonPressed(BTN_INC);            /* bo qua nut 2, 3 o mode 1 */
        isButtonPressed(BTN_SET);
        if (isButtonPressed(BTN_MODE)) enterManual(MAN_RED);
        break;

    case MAN_RED:
    case MAN_YELLOW:
    case MAN_GREEN:
        if (isTimerExpired(TMR_BLINK)) {
            setTimer(TMR_BLINK, 250);
            toggleColor(colorOfMode(status));
        }
        /* nut 2: nhan +1, giu qua 1 s tu +1 moi 200 ms */
        if (isButtonPressed(BTN_INC)) {
            increaseValue();
            setTimer(TMR_REPEAT, 200);
        }
        if (isButtonLongPressed(BTN_INC) && isTimerExpired(TMR_REPEAT)) {
            increaseValue();
            setTimer(TMR_REPEAT, 200);
        }
        /* nut 3: luu gia tri */
        if (isButtonPressed(BTN_SET)) {
            saveDuration(colorOfMode(status), tempValue);
            tempValue = durationOf(colorOfMode(status));
            setBufferA(tempValue);
        }
        /* nut 1: MAN_RED -> MAN_YELLOW -> MAN_GREEN -> Automatic */
        if (isButtonPressed(BTN_MODE)) {
            if (status == MAN_GREEN) enterAuto();
            else                     enterManual(status + 1);
        }
        break;
    }

    /* quet 7 doan moi 10 ms: 2 LED moi cap -> 50 Hz */
    if (isTimerExpired(TMR_SCAN)) {
        setTimer(TMR_SCAN, 10);
        scan7SEG();
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  /* tat het 7 doan luc khoi dong (CubeMX dang de muc dau = RESET) */
  HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, EN2_Pin | EN3_Pin, GPIO_PIN_SET);
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    fsm_run();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED_Pin|YEL_Pin|GRN_Pin|RED1_Pin
                          |YEL1_Pin|GRN1_Pin|EN0_Pin|EN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SA0_Pin|SA1_Pin|SA2_Pin|SB3_Pin
                          |SB4_Pin|SB5_Pin|SB6_Pin|EN2_Pin
                          |EN3_Pin|SA3_Pin|SA4_Pin|SA5_Pin
                          |SA6_Pin|SB0_Pin|SB1_Pin|SB2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B1_Pin B2_Pin B3_Pin */
  GPIO_InitStruct.Pin = B1_Pin|B2_Pin|B3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RED_Pin YEL_Pin GRN_Pin RED1_Pin
                           YEL1_Pin GRN1_Pin EN0_Pin EN1_Pin */
  GPIO_InitStruct.Pin = RED_Pin|YEL_Pin|GRN_Pin|RED1_Pin
                          |YEL1_Pin|GRN1_Pin|EN0_Pin|EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SA0_Pin SA1_Pin SA2_Pin SB3_Pin
                           SB4_Pin SB5_Pin SB6_Pin EN2_Pin
                           EN3_Pin SA3_Pin SA4_Pin SA5_Pin
                           SA6_Pin SB0_Pin SB1_Pin SB2_Pin */
  GPIO_InitStruct.Pin = SA0_Pin|SA1_Pin|SA2_Pin|SB3_Pin
                          |SB4_Pin|SB5_Pin|SB6_Pin|EN2_Pin
                          |EN3_Pin|SA3_Pin|SA4_Pin|SA5_Pin
                          |SA6_Pin|SB0_Pin|SB1_Pin|SB2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* ISR: chi doc nut va dem software timer, moi viec khac lam trong while(1) */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    button_reading();
    timerRun();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
