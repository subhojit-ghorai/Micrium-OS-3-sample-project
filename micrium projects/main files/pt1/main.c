#include "stm32f4xx.h"                //this onefile
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "os.h"
#include "os_cfg.h"
#include "cpu_cfg.h"
//#include "em_device.h"
//#include "bsp.h"
//#include "os.h"

OS_TCB         AppTaskStartTCB1;
OS_TCB         AppTaskStartTCB2;
//static  OS_TCB         AppTaskStartTCB3;

#define firs (OS_FLAGS)0x0001
#define seco (OS_FLAGS)0x0002
//#define thir (OS_FLAGS)0x0004

OS_FLAGS which_flags2 , which_flags1 , set_flags1, set_flags2 , flags_cur;

static  void  AppTaskStart1 (void *p_arg);
static  void  AppTaskStart2 (void *p_arg);
void MyISR (void);

OS_FLAG_GRP mygrp;
void led();
void delay( uint16_t time);

 OS_ERR err;
int main()
{
   // OS_ERR *err;
   // BSP_Init();
   //CPU_TS ts;
   OSInit(&err); 

  OSTaskCreate(  &AppTaskStartTCB1,              /* Create the start task                                */
                 "App Task Start 1",
                 AppTaskStart1,                   // start this task as soon as the initialization gets complete for the task
                 0,                                    
                 1,//APP_CFG_TASK_START_PRIO
                 0,//(CPU_STK      *)&AppTaskStartStk[0u],
                 0,//(CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 0,//(CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 0,
                 (OS_TICK       )0,
                 0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);
  
  
  OSTaskCreate(   &AppTaskStartTCB2,              /* Create the start task                                */
                 "App Task Start 2",
                 AppTaskStart2,
                 0u,
                 2,//APP_CFG_TASK_START_PRIO,
                 0,//(CPU_STK      *)&AppTaskStartStk[0u],
                 0,//(CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 0,//(CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);    //0u;
  
  
 
  
   OSFlagCreate( &mygrp,
                 "my group",
                 (OS_FLAGS)0,
                 &err);
   
  
   
  led();
  
 // AppTaskStart1();     //AppTaskStart1(*p_arg);
  //AppTaskStart2();     //AppTaskStart2(*p_arg);
  //MyISR();
  
  
 while (1)
 {
       AppTaskStart1(0);     //AppTaskStart1(*p_arg);
       MyISR();
       AppTaskStart2(0);     //AppTaskStart2(*p_arg);
      // MyISR();
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
 
 //BSP_Init();

}

void delay( uint16_t time)
{
  while (1)
 {
  
  OSTimeDlyHMSM((CPU_INT16U) 0,
  (CPU_INT16U) 0,
  (CPU_INT16U) 0,
  (CPU_INT32U) time,
  (OS_OPT )OS_OPT_TIME_HMSM_STRICT,
  (OS_ERR *)&err);
 }

}


static  void  AppTaskStart1 (void *p_arg)
{
    //OS_ERR *err;
   // BSP_Init();
    CPU_TS ts;
    //OS_FLAGS which_flags1; // , set_flags;
    //OS_FLAGS set_flags1;
    p_arg = p_arg;
   OSInit(&err); 
    while (DEF_ON)
    {
     GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
     delay(1000);
 
     which_flags1 = OSFlagPend(&mygrp,                   /* (1) Pointer to event flag group */
                              firs ,                    /* Which bits to wait on */
                              10,                       /* Maximum time to wait */
                              OS_OPT_PEND_BLOCKING +
                              OS_OPT_PEND_FLAG_SET_ANY, /* Option(s) */
                              &ts,                      /* Timestamp of when posted to */
                             &err);                    /* Pointer to Error returned */
    
     set_flags1 = OSFlagPost (&mygrp,
                             seco,                                        // 0000+0002= 0002
                             OS_OPT_POST_FLAG_SET,
                            &err);
    
  }
}


void MyISR (void)
{
 //OS_ERR *err;
// OS_FLAGS flags_cur;
 
 flags_cur = OSFlagPost(&mygrp, 
                        firs,                      //thir, //firs,
                        OS_OPT_POST_FLAG_SET,     //OS_OPT_POST_FLAG_CLR,                    OS_OPT_POST_FLAG_SET, 
                        &err);
 /* Check ’err” */
 
 
}



static  void  AppTaskStart2 (void *p_arg)
{
    //OS_ERR *err;
   CPU_TS ts;
   // OS_FLAGS which_flags2; //, set_flags;
    //OS_FLAGS set_flags2;
   p_arg = p_arg;
   OSInit(&err); 
    while (DEF_ON)
    {
     GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
     delay(1000);
 
     which_flags2 = OSFlagPend(&mygrp,                   /* (1) Pointer to event flag group */
                               set_flags1,// seco ,                    /* Which bits to wait on */
                               10,                       /* Maximum time to wait */
                               OS_OPT_PEND_BLOCKING +
                               OS_OPT_PEND_FLAG_SET_ANY, /* Option(s) */
                                &ts,                      /* Timestamp of when posted to */
                               &err);                    /* Pointer to Error returned */
    
     set_flags2 = OSFlagPost (&mygrp,
                             flags_cur, //firs,                                        // 0001+0002= 0003
                             OS_OPT_POST_FLAG_SET,
                             &err);
    
  }
}

































