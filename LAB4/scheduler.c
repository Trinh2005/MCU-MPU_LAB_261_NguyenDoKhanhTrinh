#include "scheduler.h"

typedef struct {
    void (*pTask)(void);   /* con tro toi ham cua task                        */
    uint32_t Delay;        /* so tick con phai cho, TINH TU task dung truoc no */
    uint32_t Period;       /* chu ky lap lai (tick), 0 = chi chay 1 lan       */
    uint32_t TaskID;
} sTask;

/* Hang doi luon duoc sap xep: [0] la task se chay som nhat */
static sTask    SCH_tasks_G[SCH_MAX_TASKS];
static uint32_t numTasks = 0;
static uint32_t nextID   = 1;

/* Chen task vao dung vi tri theo thoi gian (delay tinh bang tick) */
static uint32_t insertTask(void (*pFunction)(), uint32_t delay, uint32_t period, uint32_t id) {
    uint32_t pos = 0;
    if (numTasks >= SCH_MAX_TASKS) return NO_TASK_ID;

    /* di qua cac task chay truoc minh, tru dan thoi gian cua chung */
    while (pos < numTasks && delay >= SCH_tasks_G[pos].Delay) {
        delay -= SCH_tasks_G[pos].Delay;
        pos++;
    }
    /* don cac task phia sau lui 1 o */
    for (uint32_t i = numTasks; i > pos; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }
    SCH_tasks_G[pos].pTask  = pFunction;
    SCH_tasks_G[pos].Delay  = delay;
    SCH_tasks_G[pos].Period = period;
    SCH_tasks_G[pos].TaskID = id;
    numTasks++;

    /* task ngay sau gio chi con cho them phan chenh lech */
    if (pos + 1 < numTasks) SCH_tasks_G[pos + 1].Delay -= delay;
    return id;
}

/* Go task o vi tri pos ra khoi hang doi */
static void removeAt(uint32_t pos) {
    /* tra lai thoi gian cho task ngay sau */
    if (pos + 1 < numTasks) SCH_tasks_G[pos + 1].Delay += SCH_tasks_G[pos].Delay;
    for (uint32_t i = pos; i + 1 < numTasks; i++) {
        SCH_tasks_G[i] = SCH_tasks_G[i + 1];
    }
    numTasks--;
}

void SCH_Init(void) {
    numTasks = 0;
    nextID   = 1;
}

/* Goi trong ngat timer. Chi dung toi task dau hang doi -> O(1) */
void SCH_Update(void) {
    if (numTasks > 0 && SCH_tasks_G[0].Delay > 0) {
        SCH_tasks_G[0].Delay--;
    }
}

/* Goi trong while(1): chay cac task da het thoi gian cho */
void SCH_Dispatch_Tasks(void) {
    while (numTasks > 0 && SCH_tasks_G[0].Delay == 0) {
        sTask task;

        __disable_irq();                 /* khong cho ngat chen vao khi dang sua hang doi */
        task = SCH_tasks_G[0];
        removeAt(0);
        if (task.Period > 0) {           /* task dinh ky: xep lai vao hang doi */
            insertTask(task.pTask, task.Period, task.Period, task.TaskID);
        }
        __enable_irq();

        (*task.pTask)();                 /* chay task */
    }
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    uint32_t id;
    uint32_t period = PERIOD / TICK;
    if (PERIOD > 0 && period == 0) period = 1;

    __disable_irq();
    id = insertTask(pFunction, DELAY / TICK, period, nextID);
    if (id != NO_TASK_ID) nextID++;
    __enable_irq();
    return id;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    uint8_t found = 0;

    __disable_irq();
    for (uint32_t i = 0; i < numTasks; i++) {
        if (SCH_tasks_G[i].TaskID == taskID) {
            removeAt(i);
            found = 1;
            break;
        }
    }
    __enable_irq();
    return found;
}
