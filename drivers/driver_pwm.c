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
#include "gpio.h"
#include "pwm.h"
#include <stdint.h>           // standard integer functions
#include "system.h"
#include "reg_access.h"
#include "driver_pwm.h"

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

/** @function group PWM timer APIs (PWM��ʱ����ص�API)
 * @{
 */

/*********************************************************************
 * @fn      pwm_timer_init
 *
 * @brief   Initialize PWM timer.
 *			��ʼ��һ��PWM��ʱ��.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 *			
 * @return  None.
 */
void pwm_timer_init(pwm_config_t *app_pwm_param)
{
	system_set_port_mux(app_pwm_param->gpio_port, app_pwm_param->gpio_pin, PORT_FUNC_PWM);
}

/*********************************************************************
 * @fn      pwm_timer_config
 *
 * @brief   Configure the PWM timer, like duty cycle and period.
 *			����PWM��ʱ��.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 * @return  None.
 */
void pwm_timer_config(pwm_config_t *app_pwm_param)
{
	pwm_set_param(app_pwm_param->gpio_pin, app_pwm_param->high_count, app_pwm_param->low_count);
}

/*********************************************************************
 * @fn      pwm_timer_start
 *
 * @brief   Start PWM.
 *			����PWM.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 * @return  None.
 */
void pwm_timer_start(pwm_config_t *app_pwm_param)
{
	pwm_start(app_pwm_param->gpio_pin);
}

 /*********************************************************************
 * @fn      pwm_timer_stop
 *
 * @brief   Stop a PWM timer.
 *			ֹͣPWM.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 *
 * @return  None.
 */
void pwm_timer_stop(pwm_config_t *app_pwm_param)
{
	pwm_stop(app_pwm_param->gpio_pin);
}
 
/** @function group PWM timer APIs (PWM��ʱ����ص�API)
 * @{
 */
 
 
