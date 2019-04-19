/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
#ifndef DRIVER_I2C_H
#define DRIVER_I2C_H

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

/* Please note only channel 0 and 1 are supported */
enum i2c_channel_t
{
    I2C_CHANNEL_0,
    I2C_CHANNEL_1,
};

typedef struct {
	uint8_t gpio_port; //GPIO port 
	uint8_t gpio_pin;  //GPIO pin number 
	enum i2c_channel_t chan_num; //I2C channel number
	uint32_t speed; //I2C speed which has a range.
} i2c_config_t;

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
void i2c_init(i2c_config_t *app_i2c_param);

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
uint16_t i2c_get_min_speed(void);

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
uint16_t i2c_get_max_speed(void);


/*********************************************************************
 * @fn      i2c_config
 *
 * @brief   Configure the speed of I2C.
 *			����I2C���ٶ�.
 *
 * @param   app_i2c_param - pointer to the i2c configuration parameters.
 * @return  None.
 */
void i2c_config(i2c_config_t *app_i2c_param);

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
void i2c_read_byte(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf);

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
void i2c_read_bytes(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf, uint16_t len);

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
void i2c_write_byte(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t pdata);

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
void i2c_write_bytes(enum i2c_channel_t chan_num, uint8_t slave_addr, uint16_t addr, uint8_t *pbuf, uint16_t len);

#endif // DRIVER_I2C_H
