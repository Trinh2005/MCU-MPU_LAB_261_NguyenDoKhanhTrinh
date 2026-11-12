/* ====== Chep tung doan vao dung vung USER CODE cua main.c ====== */

/* USER CODE BEGIN Includes */
#include "scheduler.h"
/* USER CODE END Includes */

/* USER CODE BEGIN 0 */
uint32_t idLed1;

void task1(void) { HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); }   /* moi 0.5 s */
void task2(void) { HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); }   /* moi 1 s   */
void task3(void) { HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin); }   /* moi 1.5 s */
void task4(void) { HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin); }   /* moi 2 s   */
void task5(void) { HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin); }   /* moi 2.5 s */

/* Task chay 1 lan: xoa task1 -> LED1 ngung nhay (demo SCH_Delete_Task) */
void taskStopLed1(void) { SCH_Delete_Task(idLed1); }
/* USER CODE END 0 */

  /* USER CODE BEGIN 2 */
  SCH_Init();
  idLed1 = SCH_Add_Task(task1, 500, 500);
  SCH_Add_Task(task2, 1000, 1000);
  SCH_Add_Task(task3, 1500, 1500);
  SCH_Add_Task(task4, 2000, 2000);
  SCH_Add_Task(task5, 2500, 2500);
  SCH_Add_Task(taskStopLed1, 20000, 0);   /* chay 1 lan sau 20 s */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

    /* USER CODE BEGIN 3 */
    SCH_Dispatch_Tasks();
  }
  /* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    SCH_Update();
  }
}
/* USER CODE END 4 */
