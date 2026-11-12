#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

#define SCH_MAX_TASKS  10   /* so task toi da                  */
#define NO_TASK_ID     0    /* ID tra ve khi them task that bai */
#define TICK           10   /* chu ky ngat TIM2 (ms)           */

void     SCH_Init(void);
void     SCH_Update(void);                  /* goi trong ngat timer moi TICK */
void     SCH_Dispatch_Tasks(void);          /* goi trong while(1)            */
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);  /* DELAY, PERIOD: ms */
uint8_t  SCH_Delete_Task(uint32_t taskID);  /* 1 = da xoa, 0 = khong tim thay */

#endif /* INC_SCHEDULER_H_ */
