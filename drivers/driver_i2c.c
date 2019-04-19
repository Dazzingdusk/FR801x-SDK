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
#include <stdint.h>           // standard integer functions
#include "iic.h"
#include "system.h"
#include "reg_access.h"
#include "driver_i2c.h"

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

/** @function group I2C APIs (I2C��ص�API)
 * @{
 */

/*********************************************************************
 * @fn      i2c_init
 *
 * @brief   Initialize I2C instance.
 *			��ʼ��һ��I2Cʵ��.
 *
 * @param   app_i2c_param - pointer to the i2c configuration parameters.
 *			
 * @return  None.
 */
void i2c_init(i2c_config_t *app_i2c_param)
{
    if(app_i2c_param->chan_num == I2C_CHANNEL_0)
    {
        system_set_port_mux(GPIO_PORT_A, GPIO_BIT_0, PORTA0_FUNC_I2C0_CLK);
        system_set_port_mux(GPIO_PORT_A, GPIO_BIT_1, PORTA1_FUNC_I2C0_DAT);
    }
    else
    {
        system_set_port_mux(GPIO_PORT_A, GPIO_BIT_2, PORTA2_FUNC_I2C1_CLK);
        system_set_port_mux(GPIO_PORT_A, GPIO_BIT_3, PORTA3_FUNC_I2C1_DAT);
    }
}

/*********************************************************************
 * @fn      i2c_get_min_speed
 *
 * @brief   Get the minimum speed of I2C interface.
 *			��ȡI2C�������С�ٶ�.
 *
 * @param   None.
 *			
 * @return  I2C speed.
 */
uint16_t i2c_get_min_speed(void)
{
	uint16_t min_speed = system_get_pclk()/((0x1ff*2+10)*1000)+1;
	
	return min_speed;
}

/*********************************************************************
 * @fn      i2c_get_max_speed
 *
 * @brief   Get the maximum speed of I2C interface.
 *			��ȡI2C���������ٶ�.
 *
 * @param   None.
 *			
 * @return  I2C speed.
 */
uint16_t i2c_get_max_speed(void)
{
	uint16_t max_speed = (system_get_pclk()/((0x7*2+10)*1000))/10*10;
	
	return max_speed;
}


/*********************************************************************
 * @fn      i2c_config
 *
 * @brief   Configure the speed of I2C.
 *			����I2C���ٶ�.
 *
 * @param   app_i2c_param - pointer to the i2c configuration parameters.
 * @return  None.
 */
void i2c_config(i2c_config_t *app_i2c_param)
{
	iic_init(app_i2c_param->chan_num, app_i2c_param->speed);
}

/*********************************************************************
 * @fn      i2c_read_byte
 *
 * @brief   Read one byte through I2C.
 *			��I2C��һ���ֽ�.
 *
 * @param   chan_num - i2c channel: I2C_CHANNEL_0 or I2C_CHANNEL_1.
 *          slave_addr - slave address
 *          addr - address
 *          pbuf - pointer to one byte buffer
 * @return  None.
 */
void i2c_read_byte(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf)
{
	iic_read_byte_small(chan_num, slave_addr, addr, pbuf);
}

/*********************************************************************
 * @fn      i2c_read_bytes
 *
 * @brief   Read multiple bytes through I2C.
 *			��I2C��������ֽ�.
 *
 * @param   chan_num - i2c channel: I2C_CHANNEL_0 or I2C_CHANNEL_1.
 *          slave_addr - slave address
 *          addr - address
 *          pbuf - pointer to multiple bytes buffer
 *          len - buffer length
 * @return  None.
 */
void i2c_read_bytes(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf, uint16_t len)
{
	iic_read_bytes_small(chan_num, slave_addr, addr, pbuf, len);
}

/*********************************************************************
 * @fn      i2c_write_byte
 *
 * @brief   Write one byte through I2C.
 *			ͨ��I2Cдһ���ֽ�.
 *
 * @param   chan_num - i2c channel: I2C_CHANNEL_0 or I2C_CHANNEL_1.
 *          slave_addr - slave address
 *          addr - address
 *          pdata - one byte data
 * @return  None.
 */
void i2c_write_byte(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t pdata)
{
	iic_write_byte_small(chan_num, slave_addr, addr, pdata);
}

/*********************************************************************
 * @fn      i2c_write_bytes
 *
 * @brief   Write multiple bytes through I2C.
 *			ͨ��I2Cд������ֽ�.
 *
 * @param   chan_num - i2c channel: I2C_CHANNEL_0 or I2C_CHANNEL_1.
 *          slave_addr - slave address
 *          addr - address
 *          pbuf - pointer to multiple bytes buffer
 *          len - buffer length
 * @return  None.
 */
void i2c_write_bytes(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf, uint16_t len)
{
	iic_write_bytes_small(chan_num, slave_addr, addr, pbuf, len);
}
 
/** @function group I2C APIs (I2C��ص�API)
 * @{
 */
 
 
