/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
#ifndef BLE_SIMPLE_PERIPHERAL_H
#define BLE_SIMPLE_PERIPHERAL_H
 
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

/** @function group ble peripheral device APIs (ble������ص�API)
 * @{
 */



/*********************************************************************
 * @fn      sp_cb_dev_cfg_done
 *
 * @brief   Read confirm message to the peer. User application invokes it while received BLE_GATT_READ_REQ_IND.
 *			���ض���ȷ����Ϣ���Է���Ӧ�ò����յ�BLE_GATT_READ_REQ_IND��������������
 *
 * @param   
 *          msg - os event message from gatt task
 *           gatt�����͵��¼���Ϣ.
 *
 * @return  None.
 */
void sp_cfg_dev_init_done_cb(void *arg);

void sp_cfg_svc_init_done_cb(void *arg);


/*********************************************************************
 * @fn      gatt_write_cfm_cb
 *
 * @brief   Write confirm message to the peer. User application invokes it while received BLE_GATT_WRITE_REQ_IND.
 *			����д��ȷ����Ϣ���Է���Ӧ�ò����յ�BLE_GATT_WRITE_REQ_IND��������������
 *
 * @param   
 *          msg - os event message from gatt task
 *           gatt�����͵��¼���Ϣ.
 *
 * @return  None.
 */
void sp_gap_connected_cb(void *arg);

void sp_gap_disconnected_cb(void *arg);


void sp_param_update_ind_cb(void *arg);


void sp_param_updated_cb(void *arg);

void sp_param_update_rejected_cb(void *arg);



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

/*********************************************************************
 * @fn      gatt_notification
 *
 * @brief   Sending notification.
 *			����notification.
 *
 * @param   ntf_att - in which service and which attribute the notification will be sent
 *					  This parameter contains servcie ID and attribute ID 
 *					  to indicate exact which attribute will be used to send notification.
 *					  ָ����һ���������һ������ֵ��������notification, �ó�ʼ���Ѿ�ȷ���õķ���ID������ֵID.
 *			p_data - notification data to be sent.
 *					 ���͵�����.
 *			data_len - length of notification data.
 *					   �������ݵĳ���.
 *
 * @return  None.
 */

/*********************************************************************
 * @fn      gatt_indication
 *
 * @brief   Sending indication.
 *			����indication.
 *
 * @param   ind_att - in which service and which attribute the indication will be sent
 *					  This parameter contains servcie ID and attribute ID 
 *					  to indicate exact which attribute will be used to send indication.
 *					  ָ����һ���������һ������ֵ��������indication, �ó�ʼ���Ѿ�ȷ���õķ���ID������ֵID.
 *			p_data - indication data to be sent.
 *					 ���͵�����.
 *			data_len - length of indication data.
 *					   �������ݵĳ���.
 *
 * @return  None.
 */
  
#endif
