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
#include <stdbool.h>
#include "gap_api.h"
#include "gatt_api.h"
#include "osi.h"
#include "driver_gpio.h"
#include "driver_ext_int.h"
#include "simple_gatt_profile.h"
/*
 * MACROS (�궨��)
 */

/*
 * CONSTANTS (��������)
 */

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
// GAP-�㲥��������,�31���ֽ�.��һ������ݿ��Խ�ʡ�㲥ʱ��ϵͳ����.
static uint8_t adv_data[] =
{
  // service UUID, to notify central devices what services are included
  // in this peripheral. ����central������ʲô����, ��������ֻ��һ����Ҫ��.
  0x03,   // length of this data
  GAP_ADVTYPE_16BIT_MORE,      // some of the UUID's, but not all
  0xFF,
  0xFE,
};

// GAP - Scan response data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
// GAP-Scan response����,�31���ֽ�.��һ������ݿ��Խ�ʡ�㲥ʱ��ϵͳ����.
static uint8_t scan_rsp_data[] =
{
  // complete name �豸����
  0x12,   // length of this data
  GAP_ADVTYPE_LOCAL_NAME_COMPLETE,
  'S',
  'i',
  'm',
  'p',
  'l',
  'e',
  ' ',
  'P',
  'e',
  'r',
  'i',
  'p',
  'h',
  'e',
  'r',
  'a',
  'l',

  // Tx power level ���书��
  0x02,   // length of this data
  GAP_ADVTYPE_POWER_LEVEL,
  0,	   // 0dBm
};


/*
 * TYPEDEFS (���Ͷ���)
 */

/*
 * GLOBAL VARIABLES (ȫ�ֱ���)
 */
static gatt_service_t simple_profile_svc;

static uint8_t exti_type = EXT_INTERRUPT_TYPE_LOW;

/*
 * LOCAL VARIABLES (���ر���)
 */
 
/*
 * LOCAL FUNCTIONS (���غ���)
 */
void gpio_button_config(void);

/*
 * EXTERN FUNCTIONS (�ⲿ����)
 */

/*
 * PUBLIC FUNCTIONS (ȫ�ֺ���)
 */

/** @function group ble peripheral device APIs (ble������ص�API)
 * @{
 */

/*********************************************************************
 * @fn      sp_gatt_read_cb
 *
 * @brief   Simple Profile user application handles read request in this callback.
 *			Ӧ�ò�������ص��������洦���������
 *
 * @param   p_read  - the pointer to read buffer. NOTE: It's just a pointer from lower layer, please create the buffer in application layer.
 *					  ָ�����������ָ�롣 ��ע����ֻ��һ��ָ�룬����Ӧ�ó����з��仺����. Ϊ�������, ���Ϊָ���ָ��.
 *          len     - the pointer to the length of read buffer. Application to assign it.
 *                    ���������ĳ��ȣ��û�Ӧ�ó���ȥ������ֵ.
 *          att_idx - index of the attribute value in it's attribute table.
 *					  Attribute��ƫ����.
 *
 * @return  ������ĳ���.
 */
static uint16_t sp_gatt_read_cb(uint8_t **p_read, uint16_t *len, uint16_t att_idx)
{
    switch (att_idx)
    {
        case SP_IDX_CHAR1_VALUE:
            *len = 1;
            simple_profile_att_table[SP_IDX_CHAR1_VALUE].p_data[0] = 1;
        break;

        case SP_IDX_CHAR2_VALUE:
            *len = 2;
            simple_profile_att_table[SP_IDX_CHAR2_VALUE].p_data[0] = 1;
            simple_profile_att_table[SP_IDX_CHAR2_VALUE].p_data[1] = 2;
        break;

        case SP_IDX_CHAR3_VALUE:
            *len = 3;
            simple_profile_att_table[SP_IDX_CHAR3_VALUE].p_data[0] = 1;
            simple_profile_att_table[SP_IDX_CHAR3_VALUE].p_data[1] = 2;
            simple_profile_att_table[SP_IDX_CHAR3_VALUE].p_data[2] = 3;
        break;
        
        case SP_IDX_CHAR4_CFG:
            *len = 2;
            simple_profile_att_table[SP_IDX_CHAR4_CFG].p_data[0] = 0;
            simple_profile_att_table[SP_IDX_CHAR4_CFG].p_data[1] = 0;
        break;
        
        case SP_IDX_CHAR5_VALUE:
            *len = 5;
            simple_profile_att_table[SP_IDX_CHAR5_VALUE].p_data[0] = 1;
            simple_profile_att_table[SP_IDX_CHAR5_VALUE].p_data[1] = 2;
            simple_profile_att_table[SP_IDX_CHAR5_VALUE].p_data[2] = 3;
            simple_profile_att_table[SP_IDX_CHAR5_VALUE].p_data[3] = 4;
            simple_profile_att_table[SP_IDX_CHAR5_VALUE].p_data[4] = 5;
        break;
        
        default:
        break;
    }
    
	printf(">>>>>>>>>>>>>>>> read request: len: %d  value: 0x%x 0x%x \r\n", *len, (*p_read)[0], (*p_read)[*len-1]);
    
	return 1;
}

/*********************************************************************
 * @fn      sp_gatt_write_cb
 *
 * @brief   Simple Profile user application handles write request in this callback.
 *			Ӧ�ò�������ص��������洦��д������
 *
 * @param   write_buf   - the buffer for write
 *			              д����������.
 *					  
 *          len         - the length of write buffer.
 *                        д�������ĳ���.
 *          att_idx     - index of the attribute value in it's attribute table.
 *					      Attribute��ƫ����.
 *
 * @return  д����ĳ���.
 */
static uint16_t sp_gatt_write_cb(uint8_t *write_buf, uint16_t len, uint16_t att_idx)
{
	for (int i = 0; i < len; i++)
		printf(">>>>>>>>>>>>>>>> write request: len: %d, 0x%x \r\n", len, write_buf[i]);
	
	uint16_t uuid = BUILD_UINT16( simple_profile_att_table[att_idx].uuid.p_uuid[0], simple_profile_att_table[att_idx].uuid.p_uuid[1] );
	if (uuid == GATT_CLIENT_CHAR_CFG_UUID)
		printf(">>>>>>>>>>>>>>>> Notification status changed\r\n");
	return 1;
}

/*********************************************************************
 * @fn      sp_gatt_add_service
 *
 * @brief   Simple Profile add GATT service function.
 *			���GATT service��ATT�����ݿ����档
 *
 * @param   None. 
 *        
 *
 * @return  None.
 */
static void sp_gatt_add_service(void)
{
	simple_profile_svc.p_att_tb = simple_profile_att_table;
	simple_profile_svc.att_nb = SP_IDX_NB;
	simple_profile_svc.read_handler = sp_gatt_read_cb;
	simple_profile_svc.write_handler = sp_gatt_write_cb;
	
	gatt_add_service(&simple_profile_svc);
}


/*********************************************************************
 * @fn      sp_cfg_dev_init_done_cb
 *
 * @brief   Simple Profile callback function indicates device configuration is done. User can start adding BLE Service. 
 *			�豸��ʼ����ɵĻص�����,���Դ����￪ʼ���BLE�ķ���
 *
 * @param   arg - 
 *       
 *
 * @return  None.
 */
void sp_cfg_dev_init_done_cb(void *arg)
{
	printf(">>>>>> Device init done, adding service \r\n");
	sp_gatt_add_service();
}

/*********************************************************************
 * @fn      sp_cfg_svc_init_done_cb
 *
 * @brief   Simple Profile callback function indicates BLE services are added. User can start advertising from here if wanted. 
 *			BLE�ķ�������ֵ�����ϵĻص�����,���Ը�⣬���Դ����￪ʼ�㲥��
 *
 * @param   arg - 
 *       
 *
 * @return  None.
 */
void sp_cfg_svc_init_done_cb(void *arg)
{
    gpio_button_config();
    
	printf(">>>>>> Service init done, start advertising \r\n");
	printf(">>>>>>>>>>>>>>>>> Setting adv data\r\n");
	gap_set_advertising_data(adv_data, sizeof(adv_data));
	gap_set_scan_response_data(scan_rsp_data, sizeof(scan_rsp_data));
	printf(">>>>>>>>>>>>>>>>> Start advertising\r\n");
	gap_start_advertising(NULL);
}

/*********************************************************************
 * @fn      sp_gap_connected_cb
 *
 * @brief   Simple Profile callback of remote connected. Remote device connected.
 *			���ӽ����ɹ��Ļص�������
 *
 * @param  arg - 
 *        
 *
 * @return  None.
 */
void sp_gap_connected_cb(void *arg)
{
	printf(">>>>>>Connected.\r\n");
}


/*********************************************************************
 * @fn      sp_gap_disconnected_cb
 *
 * @brief   Simple Profile callback of remote disconnected. Remote device disconnected.
 *			���ӶϿ��Ļص�������
 *
 * @param   arg - 
 *        
 *
 * @return  None.
 */
void sp_gap_disconnected_cb(void *arg)
{
	printf(">>>>>>Disconnected.\r\n");
}

/*********************************************************************
 * @fn      sp_param_update_ind_cb
 *
 * @brief   Simple Profile callback of connection paramteers update indication.
 *			���Ӳ�����������ص�������
 *
 * @param   arg - 
 *        
 *
 * @return  None.
 */
void sp_param_update_ind_cb(void *arg)
{
}

/*********************************************************************
 * @fn      sp_param_updated_cb
 *
 * @brief   Simple Profile callback of connection peers update successfuly updated.
 *			���Ӳ������³ɹ���
 *
 * @param   arg - 
 *        
 *
 * @return  None.
 */
void sp_param_updated_cb(void *arg)
{
	printf(">>>>>>Connection parameters updated.\r\n");
}

/*********************************************************************
 * @fn      sp_param_update_rejected_cb
 *
 * @brief   Simple Profile callback of connection paramteers update rejected.
 *			���Ӳ����������󱻾ܾ���
 *
 * @param   arg - 
 *        
 *
 * @return  None.
 */
void sp_param_update_rejected_cb(void *arg)
{
		printf(">>>>>>Connection parameters rejected.\r\n");
}



/*********************************************************************
 * @fn      gatt_msg_handler
 *
 * @brief   the default gatt message handler.
 *			�����gatt controller��������message.
 *
 * @param   msg - msg from gatt controller.
 *					  ��gatt controller����message.
 *
 * @return  None.
 */
static void param_timer_func(void *arg)
{
}


/*********************************************************************
 * @fn      external_interrupt_handler
 *
 * @brief   GPIO interrupt handle function. This function actually called after debounce, 
 *          debounce is already done. Debounce is configrued in ext_int_set_control().
 *			GPIO���жϴ�����.������������õ�ʱ��ʵ������
 *          ��ȥ�������ˣ�ȥ����ext_int_set_control()�����á�
 *
 * @param   None. 
 *
 * @return  None.
 */
void external_interrupt_handler(void)
{
    uint32_t status;
    status = ext_interrupt_get_status();
    ext_interrupt_clear_status(status);

    gatt_ntf_t notify_att;
    notify_att.svc_id = simple_profile_svc.svc_id;
    notify_att.att_idx = SP_IDX_CHAR4_VALUE;
    
    uint8_t notify_data = 0xFF;
    uint8_t notify_data_len = 1;
    
    if(exti_type == EXT_INTERRUPT_TYPE_LOW)
    {
        exti_type = EXT_INTERRUPT_TYPE_HIGH;
        if ( 0x100000 == status ) // ����1
        {
            notify_data = 1;
            ext_interrupt_set_type(20, exti_type);
        }
        else if ( 0x200000 == status )  // ����2
        {
            notify_data = 2;
            ext_interrupt_set_type(21, exti_type);
        }

    }
    else if(exti_type == EXT_INTERRUPT_TYPE_HIGH)
    {
        exti_type = EXT_INTERRUPT_TYPE_LOW;
        if ( 0x100000 == status ) // ����1
        {
            ext_interrupt_set_type(20, exti_type);
        }
        else if ( 0x200000 == status )  // ����2
        {
            ext_interrupt_set_type(21, exti_type);
        }

        notify_data = 0;
    }

    // Send button is pressed notification.
    gatt_notification(notify_att, &notify_data, notify_data_len);
    
}


/*********************************************************************
 * @fn      gatt_add_service
 *
 * @brief   Addding a services & characteristics into gatt database.
 *			���һ������͸�������ֵ��gatt�����ݿ�����ȥ.
 *
 * @param   service - service data to be added.
 *					  ��Ҫ��ӵķ���������Ϣ.
 *
 * @return  None.
 */
void gpio_button_config(void)
{
    //multi gpio wkup setting
    //set_sleep_flag_after_key_release(false);
    //appm_set_exit_int_gpio(0x003800f3);   //0x003800f3

    gpio_set_port_mux(GPIO_PORT_C, GPIO_BIT_4, PORT_FUNC_EXT_INT);
    gpio_set_port_pull(20, true);
    ext_interrupt_set_type(20, exti_type);

    ext_interrupt_set_control(20, 1000, 10);
    ext_interrupt_pin_select(20);

    gpio_set_port_mux(GPIO_PORT_C, GPIO_BIT_5, PORT_FUNC_EXT_INT);
    gpio_set_port_pull(21, true);
    ext_interrupt_set_type(21, exti_type);

    ext_interrupt_set_control(21, 1000, 10);
    ext_interrupt_pin_select(21);

    
    //NVIC_EnableIRQ(EXT_INTR_IRQn);
    ext_interrupt_enable();
}


