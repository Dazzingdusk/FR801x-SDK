/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
#ifndef DRIVER_PWM_H
#define DRIVER_PWM_H

/*
 * INCLUDES (����ͷ�ļ�)
 */

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
one count means:
@48M 21ns
@24M 41ns
@12M 83ns

high_count:1~0xffffffff
low_count:1~0xffffffff
high_count + low_count <=0xffffffff

NOTE: Refer to driver_iomux.h, do not configure the same GPIO pin in different GPIO port as PWM.
ע�⣺�ο�driver_iomux.h, ��Ҫ�Ѳ�ͬ��GPIO�˿�����ͬ��GPIO PIN�ź����ó�PWM��ÿ��PIN�ź�ֻ�ܶ�Ӧһ��PWM��
*/
typedef struct {
	uint8_t gpio_port; //GPIO port 
	uint8_t gpio_pin;  //GPIO pin number 
	uint32_t high_count; //counts for PWM HIGH
	uint32_t low_count; //count for PWM LOW
} pwm_config_t;

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
void pwm_timer_init(pwm_config_t *app_pwm_param);

/*********************************************************************
 * @fn      pwm_timer_config
 *
 * @brief   Configure the PWM timer, like duty cycle and period.
 *			����PWM��ʱ��.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 * @return  None.
 */
void pwm_timer_config(pwm_config_t *app_pwm_param);

/*********************************************************************
 * @fn      pwm_timer_start
 *
 * @brief   Start PWM.
 *			����PWM.
 *
 * @param   app_pwm_param - pointer to the pwm configuration parameters.
 * @return  None.
 */
void pwm_timer_start(pwm_config_t *app_pwm_param);

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
void pwm_timer_stop(pwm_config_t *app_pwm_param);

#endif // DRIVER_PWM_H
