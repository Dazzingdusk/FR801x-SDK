/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
 
/*
 * INCLUDES (����ͷ�ļ�)
 */
#include "app.h"
#include "user_timer.h"
#include "osal_timer.h"

/*
 * MACROS (�궨��)
 */

/*
 * CONSTANTS (��������)
 */

/*
 * TYPEDEFS (���Ͷ���)
 */

/*
 * GLOBAL VARIABLES (ȫ�ֱ���)
 */

/*
 * LOCAL VARIABLES (���ر���)
 */

/*
 * LOCAL FUNCTIONS (���غ���)
 */

/*
 * EXTERN FUNCTIONS (�ⲿ����)
 */

/*
 * PUBLIC FUNCTIONS (ȫ�ֺ���)
 */

/** @function group OS timer APIs (OS��ʱ����ص�API)
 * @{
 */

/*********************************************************************
 * @fn      osal_timer_init
 *
 * @brief   Initialize an OS timer and malloc a buffer for it.
 *			����һ������������ʼ��һ��OS��ʱ��.
 *
 * @param   ptimer - pointer to the timer buffer.
 *			pfunction - callback of the timer
 *          parg - parameter of timer callback
 * @return  None.
 */
void osal_timer_init(osal_timer_struct *ptimer, osal_timer_func_t pfunction, void *parg)
{
	os_timer_t *p_os_timer;
	
    ptimer = (void *)ke_malloc(sizeof(os_timer_t) + 1, KE_MEM_NON_RETENTION);
	
	p_os_timer = (osal_timer_struct *)ptimer;
	
	os_timer_setfn(p_os_timer, pfunction, parg);
}

/*********************************************************************
 * @fn      osal_timer_start
 *
 * @brief   Start a timer.
 *			����Timer.
 *
 * @param   ptimer - pointer to the timer buffer.
 *			ms - expire of the timer with milliseconds
 *          repeat_flag - repeat or one shot timer
 * @return  None.
 */
void osal_timer_start(osal_timer_struct *ptimer,uint32_t ms,bool repeat_flag)
{
	os_timer_t *p_os_timer;
	
	p_os_timer = (osal_timer_struct *)ptimer;
	
    os_timer_arm(p_os_timer, ms, repeat_flag);
}
	  
 /*********************************************************************
 * @fn      osal_timer_stop
 *
 * @brief   Stop a timer.
 *			ֹͣtimer.
 *
 * @param   ptimer - pointer to the timer buffer.
 *
 * @return  None.
 */
void osal_timer_stop(osal_timer_struct *ptimer)
{	
	os_timer_t *p_os_timer;
	
	p_os_timer = (osal_timer_struct *)ptimer;
	
	os_timer_disarm(p_os_timer);
	
    ke_free(ptimer);
}
 
/** @function group OS timer APIs (OS��ʱ����ص�API)
 * @{
 */
 
 
