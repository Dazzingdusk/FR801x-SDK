/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
 
#ifndef GATT_API_H
#define GATT_API_H


 /*
 * INCLUDES (����ͷ�ļ�)
 */

/*
 * MACROS (�궨��)
 */

/*
 * CONSTANTS (��������)
 */
#define UUID_SIZE_2		2	//!< 2 bytes UUID, usually SIG assigned UUID. 2�ֽڵ�UUID, ͨ����SIGָ��.
#define UUID_SIZE_16	16	//!< 16 bytes UUID, usually assigned by users. 16�ֽڵ�UUID, ͨ�������û��Զ���.

/** @defgroup GATT_PROP_BITMAPS_DEFINES GATT Attribute Access Permissions Bit Fields
 * @{
 */
	
#define GATT_PROP_READ                 	0x0001 //!< Attribute is Readable
#define GATT_PROP_WRITE                	0x0002 //!< Attribute is Writable
#define GATT_PROP_AUTHEN_READ          	0x0004 //!< Read requires Authentication
#define GATT_PROP_AUTHEN_WRITE         	0x0008 //!< Write requires Authentication
#define GATT_PROP_AUTHOR_READ          	0x0010 //!< Read requires Authorization
#define GATT_PROP_AUTHOR_WRITE         	0x0020 //!< Write requires Authorization
#define GATT_PROP_ENCRYPT_READ         	0x0040 //!< Read requires Encryption
#define GATT_PROP_ENCRYPT_WRITE        	0x0080 //!< Write requires Encryption
#define GATT_PROP_NOTIFICATION         	0x0100 //!< Attribute is able to send notification
#define GATT_PROP_INDICATION         	0x0200 //!< Attribute is able to send indication
	  
/** @} End GATT_PERMIT_BITMAPS_DEFINES */

/*
 * TYPEDEFS (���Ͷ���)
 */

/*********************************************************************
 * @fn      gatt_read_handler
 *
 * @brief   User application handles read request in this callback.
 *			Ӧ�ò�������ص��������洦���������
 *
 * @param   p_read  - the pointer to read buffer. NOTE: It's just a pointer from lower layer, please create the buffer in application layer.
 *					  ָ�����������ָ�롣 ��ע����ֻ��һ��ָ�룬����Ӧ�ó����з��仺����. Ϊ�������, ���Ϊָ���ָ��.
 *          len     - the pointer to the length of read buffer. Application to assign it.
 *                    ���������ĳ��ȣ��û�Ӧ�ó���ȥ������ֵ.
 *          att_idx - offset of the attribute value.
 *					  Attribute��ƫ����.
 *
 * @return  ������ĳ���.
 */
typedef uint16_t (*gatt_read_handler)(uint8_t **p_read, uint16_t *len, uint16_t att_idx);

/*********************************************************************
 * @fn      gatt_write_handler
 *
 * @brief   User application handles write request in this callback.
 *			Ӧ�ò�������ص��������洦��д������
 *
 * @param   write_buf   - the buffer for write
 *			              д����������.
 *					  
 *          len         - the length of write buffer.
 *                        д�������ĳ���.
 *          att_idx     - offset of the attribute value.
 *					      Attribute��ƫ����.
 *
 * @return  д����ĳ���.
 */
typedef uint16_t (*gatt_write_handler)(uint8_t *write_buf, uint16_t len, uint16_t att_idx);


/**
* GATT UUID format.
*/
typedef struct
{
	uint8_t size;		//!< Length of UUID (2 or 16 bytes UUIDs: UUID_SIZE_2 or UUID_SIZE_16). UUID����(2 ���� 16�ֽڳ���UUID). 
	uint8_t *p_uuid;	//!< Pointer to uuid, could be 2 or 16 bytes array. ָ��UUID��ָ��, ������2�ֽڵ�, Ҳ������16�ֽڵ�����.
}gatt_uuid_t;

/**
* BLE attribute define format.
*/
typedef struct
{
	gatt_uuid_t 	uuid;
	uint16_t		prop;
	uint16_t		max_size;
	uint8_t			*p_data;
}gatt_attribute_t;

 /**
 * BLE service define format.
 */
typedef struct
{
  gatt_attribute_t	 	*p_att_tb;    	//!< Service's attributes table to add to system attribute database. �����������ֵ�ı������, ������ӵ�ϵͳ������ֵ���ݿ���.
  uint8_t		 		att_nb;    		//!< Service's attributes number. �����������ֵ�ı�����������ֵ����, ����������е�����ֵ, value, descriptor, CCC�ȵȵȵ�, ���ǵ����������ݴ����Ǽ���valueֵ.
  uint16_t				svc_id;			//!< Service ID among all services in current system. ÿ�����񶼻���ϵͳ�ﱻ����һ���ض������ʶ��ID, ������svc_id�м�¼,��ֻ��ϵͳ�ڲ���, ע�����SIGҪ�����ȫ����ΨһUUID��ͬ.
  gatt_read_handler 	read_handler;	//!< Read request callback function. ������ص�����ָ��.
  gatt_write_handler 	write_handler; 	//!< Write request callback function. д����ص�����ָ��.
} gatt_service_t;

 /**
 * BLE notification format.
 */
typedef struct 
{
  uint16_t			svc_id;		//!< Service ID among all services in current system. ÿ�����񶼻���ϵͳ�ﱻ����һ���ض������ʶ��ID, ������svc_id�м�¼,��ֻ��ϵͳ�ڲ���, ע�����SIGҪ�����ȫ����ΨһUUID��ͬ.
  uint8_t		 	att_idx;    	//!< Attribute id number in its service attribute table. ��Ҫ����notification������ֵ����service������ֵ����е�λ�ã�����Դ�Ԥ��������ֵ������������ɸ��Ƶõ�.
} gatt_ntf_t;

 /**
 * BLE indication format.
 */
typedef struct 
{
  uint16_t			svc_id;		//!< Service ID among all services in current system. ÿ�����񶼻���ϵͳ�ﱻ����һ���ض������ʶ��ID, ������svc_id�м�¼,��ֻ��ϵͳ�ڲ���, ע�����SIGҪ�����ȫ����ΨһUUID��ͬ.
  uint8_t		 	att_idx;    	//!< Attribute id number in its service attribute table. ��Ҫ����indication������ֵ����service������ֵ����е�λ�ã�����Դ�Ԥ��������ֵ������������ɸ��Ƶõ�.
} gatt_ind_t;

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
void gatt_add_service(gatt_service_t *p_service);

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
void gatt_notification(gatt_ntf_t ntf_att, uint8_t *p_data, uint8_t data_len);
	  
 /*********************************************************************
  * @fn 	 gatt_indication
  *
  * @brief	 Sending indication.
  * 		 ����indication.
  *
  * @param	 ind_att - in which service and which attribute the indication will be sent
  * 				   This parameter contains servcie ID and attribute ID 
  * 				   to indicate exact which attribute will be used to send indication.
  * 				   ָ����һ���������һ������ֵ��������indication, �ó�ʼ���Ѿ�ȷ���õķ���ID������ֵID.
  * 		 p_data - indication data to be sent.
  * 				  ���͵�����.
  * 		 data_len - length of indication data.
  * 					�������ݵĳ���.
  *
  * @return  None.
  */
void gatt_indication(gatt_ind_t ind_att, uint8_t *p_data, uint8_t data_len);


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
void gatt_indication(gatt_ind_t ind_att, uint8_t *p_data, uint8_t data_len);

#endif // end of #ifndef GATT_API_H
