#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "os.h"
#include "os_cfg.h"
#include "cpu_cfg.h"


#define firs (OS_FLAGS)0x0001
#define seco (OS_FLAGS)0x0002
#define thir (OS_FLAGS)0x0003




static  OS_TCB         AppTaskStartTCB1;
static  OS_TCB         AppTaskStartTCB2;
static  OS_TCB         AppTaskStartTCB3;


OS_FLAG_GRP MYGRP;

//OS_FLAGS which_flags , set_flags;



int main()
{
  OS_ERR err;

  OSTaskCreate(  (OS_TCB       *)&AppTaskStartTCB1,              /* Create the start task                                */
                 (CPU_CHAR     *)"App Task Start 1",
                 (OS_TASK_PTR   )AppTaskStart1,                   // start this task as soon as the initialization gets complete for the task
                 (void         *)0u,                                    
                 (OS_PRIO       )1,//APP_CFG_TASK_START_PRIO
                 (CPU_STK      *)&AppTaskStartStk[0u],
                 //(CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 //(CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
  
  
  OSTaskCreate(  (OS_TCB       *)&AppTaskStartTCB2,              /* Create the start task                                */
                 (CPU_CHAR     *)"App Task Start 2",
                 (OS_TASK_PTR   )AppTaskStart2,
                 (void         *)0u,
                 (OS_PRIO       )2,//APP_CFG_TASK_START_PRIO,
                 //(CPU_STK      *)&AppTaskStartStk[0u],
                 //(CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
  
  
  OSTaskCreate(  (OS_TCB       *)&AppTaskStartTCB3,              /* Create the start task                                */
                 (CPU_CHAR     *)"App Task Start 3",
                 (OS_TASK_PTR   )AppTaskStart3,                
                 (void         *)0u,
                 (OS_PRIO       )3,//APP_CFG_TASK_START_PRIO,
                 //(CPU_STK      *)&AppTaskStartStk[0u],
                 //(CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
  
  OSFlagCreate (&MYGRP,
                "my group",
                 (OS_FLAGS)0,
                 &err);
  
  
  while (DEF_ON)
  {
    
  };
}

void led() // setting up the led pins and linkin them to the alternate func.

{
  
 GPIO_InitTypeDef led;
 
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
 
 led.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 ;
 led.GPIO_OType = GPIO_OType_PP;
 led.GPIO_PuPd = GPIO_PuPd_NOPULL;
 led.GPIO_Mode = GPIO_Mode_AF;
 led.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOD, &led);
 
 BSP_Init();

}

static  void  AppTaskStart1 (void *p_arg)
{
  
    OS_ERR err;
    CPU_TS ts;
    OS_FLAGS which_flags , set_flags;
    
   
 
 while (DEF_ON) {
   GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
 
 which_flags = OSFlagPend(&MYGRP,                   /* (1) Pointer to event flag group */
                          firs ,                    /* Which bits to wait on */
                          10,                       /* Maximum time to wait */
                          //OS_OPT_PEND_BLOCKING +
                          //OS_OPT_PEND_FLAG_SET_ANY, /* Option(s) */
                          &ts,                      /* Timestamp of when posted to */
                          &err);                    /* Pointer to Error returned */

    
 set_flags = OSFlagPost (&MYGRP,
                         seco,                                        // 0000+0002= 0002
                         OS_OPT_POST_FLAG_SET,
                         &err);
    
  }
}


static  void  AppTaskStart2 (void *p_arg)
{
  
  
    OS_ERR err;
    CPU_TS ts;
    OS_FLAGS which_flags , set_flags;
    
   
 
 while (DEF_ON) {
 
    GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
 which_flags = OSFlagPend(&MYGRP,                      /* (1) Pointer to event flag group */
                          seco ,                       /* Which bits to wait on */
                          10,                           /* Maximum time to wait */
                          //OS_OPT_PEND_BLOCKING +
                          //OS_OPT_PEND_FLAG_SET_ANY,    /* Option(s) */
                          &ts,                         /* Timestamp of when posted to */
                          &err);                       /* Pointer to Error returned */
 
 set_flags = OSFlagPost (&MYGRP,
                         firs,                          // 0002+0001 = 0003
                         OS_OPT_POST_FLAG_SET,
                         &err);
  }
}

static  void  AppTaskStart3 (void *p_arg)
{
  
   
    OS_ERR err;
    CPU_TS ts;
    OS_FLAGS which_flags , set_flags;
    
    
 
 while (DEF_ON) {
 
   GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
   
 which_flags = OSFlagPend(&MYGRP,                   /* (1) Pointer to event flag group */
                          thir ,                    /* Which bits to wait on */
                          10,                        /* Maximum time to wait */
                          //OS_OPT_PEND_BLOCKING +
                          //OS_OPT_PEND_FLAG_SET_ANY, /* Option(s) */
                          &ts,                      /* Timestamp of when posted to */
                          &err);                    /* Pointer to Error returned */
 
 set_flags = OSFlagPost (&MYGRP,
                         firs,
                         OS_OPT_POST_FLAG_SET,
                         &err);
 }
    
    
  }
  
 

 
          

