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
#include "user_profile.h"
#include "gatt_api.h"
#include "gatt_sig_uuid.h"
#include "user_msg_q.h"
#include "rwprf_config.h"

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
static uint8_t total_service_num = 0;
//static uint16_t svc_id_list[MAX_USER_PRF_NUM];
static gatt_service_t gatt_svc_list[MAX_USER_PRF_NUM];
 
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

static int16_t get_active_service_id(uint16_t att_handle, uint16_t *active_svc_id, uint16_t *offset)
{
	uint16_t svc_start_hdl = 0;
	int i;
	gatt_service_t *p_gatt_svc = &gatt_svc_list[0];
	
	for(i = 0; i < total_service_num; i ++)
	{
		svc_start_hdl = user_get_prf_start_hdl(p_gatt_svc[i].svc_id);
		if((att_handle > svc_start_hdl) && (att_handle - svc_start_hdl <= p_gatt_svc[i].att_nb))
			break;
	}
	printf(">>>>>>>>>>>>>>>> service ID: %d, index: %d \r\n", p_gatt_svc[i].svc_id, i);

	if(i == total_service_num) //  Service id should be more than handle since 'i' will minus 1 to get the current active service id.
		return -1;

	// �õ�servcie ID
	*active_svc_id = p_gatt_svc[i].svc_id;
	// ���attribute handle������Service��һ��attribute handle����ٸ�
	*offset = att_handle - svc_start_hdl;    

	// ����gatt_svc_list[]�б��index
	return (i);
} 

/*********************************************************************
 * @fn      gatt_read_cfm_cb
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
static bool gatt_read_cfm_cb(os_event_t *msg)
{
	uint16_t active_svc_id, offset;
	uint8_t *p_read;
	uint16_t len;
	int16_t index;
	
    struct gattc_read_req_ind *param = (struct gattc_read_req_ind *)msg->param;
	struct gattc_read_cfm* cfm;

    // ����attribute handle��ȡ�ø�attribute������servcie��id, ��attribute������service���attribute inddex offset���Լ�����service��gatt_svc_list�����index��
    if((index = get_active_service_id(param->handle, &active_svc_id, &offset)) < 0)
		return false;
	
	printf(">>>>>>>>>>>>>>>> service id: %d, start hdl: 0x%04x, attribute handle to read: 0x%04x \r\n", active_svc_id, user_get_prf_start_hdl(active_svc_id), user_get_prf_start_hdl(active_svc_id)+offset);


	if ( gatt_svc_list[index].p_att_tb[offset].prop & GATT_PROP_READ )
	{
        // �����ϲ�ص����������������ע��p_read�Ŀռ������ϲ�Ӧ�þ���������û��������ԭ������ΪlenҲ�����ϲ����
        gatt_svc_list[index].read_handler(&p_read, &len, offset);
        
        cfm = (struct gattc_read_cfm*)ke_malloc(sizeof(struct gattc_read_cfm)+len, KE_MEM_NON_RETENTION);
        cfm->handle = param->handle;
        
	    cfm->length = len;
	    cfm->status = ATT_ERR_NO_ERROR;

		printf(">>>>>>>>>>>>>>>> value: 0x%x 0x%x,  len: %d \r\n", gatt_svc_list[index].p_att_tb[offset].p_data[0], gatt_svc_list[index].p_att_tb[offset].p_data[len-1], len);
	    memcpy(cfm->value, gatt_svc_list[index].p_att_tb[offset].p_data, len);

	}
	else
	{
		len = 0;
	    cfm->length = 0;
	    cfm->status = ATT_ERR_READ_NOT_PERMITTED;
	}

    os_event_t evt;
    evt.event_id = GATTC_READ_CFM;
    evt.src_task_id = user_get_prf_task_num(active_svc_id);
    evt.param = cfm;
    evt.param_len = sizeof(struct gattc_read_cfm)+len;
    os_msg_post(msg->src_task_id,&evt);

    ke_free(cfm);

    printf("READ_REQ,hdl:%d,%d\r\n",param->handle,user_get_prf_start_hdl(active_svc_id));	
	
	return true;
}

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
 * @return  bool.
 */
static bool gatt_write_cfm_cb(os_event_t *msg)
{
    struct gattc_write_req_ind *param = (struct gattc_write_req_ind *)msg->param;
	int16_t index;
	uint16_t active_svc_id, offset;
    struct gattc_write_cfm cfm;
	
    if((index = get_active_service_id(param->handle, &active_svc_id, &offset)) < 0)
		return false;
	

	if ( gatt_svc_list[index].p_att_tb[offset].prop & GATT_PROP_WRITE )
	{
        gatt_svc_list[index].write_handler(param->value, param->length, offset);
        
        cfm.handle = param->handle;
        
	    cfm.status = ATT_ERR_NO_ERROR;
	}
	else
	{
		cfm.status = ATT_ERR_WRITE_NOT_PERMITTED;
	}

    os_event_t evt;
    evt.event_id = GATTC_WRITE_CFM;
    evt.src_task_id = user_get_prf_task_num(active_svc_id);
    evt.param = &cfm;
    evt.param_len = sizeof(struct gattc_write_cfm);
    os_msg_post(msg->src_task_id,&evt);

    printf("WRITE_REQ2,hdl:%d,%d\r\n",param->handle,user_get_prf_start_hdl(active_svc_id));
    show_reg2(param->value,param->length,1);
	
	return true;
}


/*********************************************************************
 * @fn      gatt_info_cfm_cb
 *
 * @brief   Information request confirm to the peer. User application invokes it while received GATTC_ATT_INFO_REQ_IND. 
 *			������Ϣ��ȷ����Ϣ���Է���Ӧ�ò����յ�GATTC_ATT_INFO_REQ_IND�����������������������Ҫ����ȷ��write���Ե�����ֵ�ĳ��ȣ����ڶԷ�д����.
 *
 * @param   
 *          msg - os event message from gatt task
 *           gatt�����͵��¼���Ϣ.
 *
 * @return  bool.
 */
static bool gatt_info_cfm_cb(os_event_t *msg)
{
	struct gattc_att_info_req_ind *param = (struct gattc_att_info_req_ind *)msg->param;
	int16_t index;
	uint16_t active_svc_id, offset;
	uint8_t *p_read;
	uint16_t len;
	
	struct gattc_att_info_cfm cfm;
	cfm.handle = param->handle;
	
    if((index = get_active_service_id(param->handle, &active_svc_id, &offset)) < 0)
		return false;
	
	// ���ǶԷ���ȷ����������ܱ�д������ֵ��value�ĳ��ȣ�������write��property.
	if ( gatt_svc_list[index].p_att_tb[offset].prop & GATT_PROP_WRITE )
	{
		gatt_svc_list[index].read_handler(&p_read, &len, offset);
		
		// Information request need only length
		cfm.length = len;
		cfm.status = ATT_ERR_NO_ERROR;
	}
	else
	{
		cfm.length = 0;
		cfm.status = ATT_ERR_WRITE_NOT_PERMITTED;
	}

	os_event_t evt;
	evt.event_id = GATTC_ATT_INFO_CFM;
	evt.src_task_id = user_get_prf_task_num(active_svc_id);
	evt.param = &cfm;
	evt.param_len = sizeof(struct gattc_att_info_cfm);
	os_msg_post(msg->src_task_id, &evt);
	
	printf(">>>>>>>>>>>>>>>> ATT_INFO_REQ,hdl:%d,%d\r\n",param->handle,user_get_prf_start_hdl(active_svc_id));

	return true;
}

/*********************************************************************
 * @fn      gatt_read_cb
 *
 * @brief   Gatt read callback function. it invokes application layer read callback function when there are any read activities happen. 
 *			Gatt read ��Ϊ�Ļص�����. �˻ص����������Ӧ�ò㶨���read�ص�.
 *
 * @param   
 *          msg - os event message from gatt task
 *           gatt�����͵��¼���Ϣ.
 *
 * @return  bool.
 */
static bool gatt_read_cb(os_event_t *msg)
{
	bool ret;
	switch (msg->event_id)
	{
		case GATTC_READ_REQ_IND:
			ret = gatt_read_cfm_cb(msg);
			break;
			
//		case GATTC_ATT_INFO_REQ_IND:
//			ret = gatt_info_cfm_cb(msg);
			
		default:
			break;
	}
	return ret;
}


/*********************************************************************
 * @fn      gatt_write_cb
 *
 * @brief   Gatt write callback function. it invokes application layer write callback function when there are any read activities happen. 
 *			Gatt write ��Ϊ�Ļص�����. �˻ص����������Ӧ�ò㶨���write�ص�.
 *
 * @param   
 *          msg - os event message from gatt task
 *           gatt�����͵��¼���Ϣ.
 *
 * @return  bool.
 */
static bool gatt_write_cb(os_event_t *msg)
{
	bool ret;
	switch (msg->event_id)
	{
		case GATTC_WRITE_REQ_IND:
			ret  = gatt_write_cfm_cb(msg);
			break;

		default:
			break;
	}
	return ret;
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
static int gatt_msg_handler(os_event_t *msg)
{
	enum ke_msg_status_tag ret_status = KE_MSG_CONSUMED;
	printf(">>>>>>>>>>>>>>>> gatt request received : 0x%04x : \r\n ", msg->event_id);
	
    switch(msg->event_id)
	{
		case GATTC_READ_REQ_IND:	// 0x0c13  // 0x13 = 19
			//if(gatt_read_cb(msg) == true)
            if ( gatt_read_cfm_cb(msg) == true )    
			{
				ret_status = KE_MSG_CONSUMED;
			}
			break;
		
		case GATTC_WRITE_REQ_IND:	// 0x0c15	
			//if(gatt_write_cb(msg) == true)
			if ( gatt_write_cfm_cb(msg) == true )
			{
				ret_status = KE_MSG_CONSUMED;
			}
			break;
		
		case GATTC_ATT_INFO_REQ_IND:	//0xc17
		    if ( gatt_info_cfm_cb(msg) == true )
            {
                ret_status = KE_MSG_CONSUMED;
            }      
			break;
            
        case GATTC_CMP_EVT:     //0xc00, only for NTF or INTF
        {
            struct gattc_cmp_evt *param = (struct gattc_cmp_evt *)msg->param;
            switch(param->operation)
            {
                case GATTC_NOTIFY:
                    printf("NTF operation done,\r\n");
                    break;
                case GATTC_INDICATE:
                    printf("IND operation done,\r\n");
                    break;
                default:
                    break;
            }
            printf("%x,%d\r\n",param->status,param->seq_num);
        }
        break;
				
        default:
			break;
			
	}	
	
	return ret_status;
}


/*********************************************************************
 * @fn      gatt_att_tb_convert
 *
 * @brief   Convert attributre table to be fitable for stack to add to database.
 *			���û�attribute tableת����Э��ջ���õĽṹ�����ڷ����ʼ��.
 *
 * @param   p_att_db_out - output the attribute table for profile creation.
 *					  	   ���������Ҫ��ӵķ����attribute table.
 *
 *			p_uuid128 - output 128bit UUID arry.
 * 						���128bit��UUID����.
 *
 *			p_att_tb_in - input user defined attribute table.
 * 						  ����ͻ������attribute table.
 *
 *			att_nb - input user defined attribute table, the number of attributes in the table.
 * 					 ����ͻ������attribute table, �����table��attribute�ĸ������������ڶ���enum����õ�.
 * @return  None.
 */
static void gatt_att_tb_convert( struct attm_desc **p_att_db_out, uint8_t **p_uuid128, gatt_attribute_t *p_att_tb_in, uint8_t att_nb )
{
	uint8_t uuid128_cnt = 0;
	
	for ( int i = 0; i < att_nb; i++ )
	{
		/*** Handle attm_desc UUID ***/
		// If UUID is 2 bytes and valid, just copy it.
		if ( p_att_tb_in[i].uuid.size == UUID_SIZE_2 )
		{
			uint16_t uuid = BUILD_UINT16( p_att_tb_in[i].uuid.p_uuid[0], p_att_tb_in[i].uuid.p_uuid[1] );

			(*p_att_db_out)[i].uuid = uuid;
#if 1			
			if ( uuid == GATT_PRIMARY_SERVICE_UUID )	// Should be i = 0
			{
				// Service UUID is 16 bytes
				if ( p_att_tb_in[i].max_size == UUID_SIZE_16 )
					// Record how many 16 bytes uuids, at the end copy them.
					uuid128_cnt++;
			}
#endif
			else if ( uuid == GATT_CHARACTER_UUID )
			{
				// The next attribute must be the "value", set the PERM(RI, ENABLE), let application decide data.
				(*p_att_db_out)[i + 1].ext_perm |= PERM(RI, ENABLE);
			}
		}
		else if ( p_att_tb_in[i].uuid.size == UUID_SIZE_16 )
		{
			(*p_att_db_out)[i].uuid = 0xFFFF;
			// Record how many 16 bytes uuids, at the end copy them.
			uuid128_cnt++;

			// ext_perm,need to add the flag
			(*p_att_db_out)[i].ext_perm |= PERM_VAL(UUID_LEN, PERM_UUID_128);
		}

		/*** Handle attm_desc perm ***/
		if ( GATT_PROP_READ & p_att_tb_in[i].prop )
		{
			(*p_att_db_out)[i].perm |= PERM(RD, ENABLE);
		}
		if ( GATT_PROP_WRITE & p_att_tb_in[i].prop )
		{
			(*p_att_db_out)[i].perm |= (PERM(WRITE_COMMAND, ENABLE) | PERM(WRITE_REQ, ENABLE));
		}
		if ( GATT_PROP_NOTIFICATION & p_att_tb_in[i].prop )
		{
			(*p_att_db_out)[i].perm |= PERM(NTF, ENABLE);
		}
		if ( GATT_PROP_INDICATION & p_att_tb_in[i].prop )
		{
			(*p_att_db_out)[i].perm |= PERM(IND, ENABLE);
		}
        if ( GATT_PROP_AUTHEN_READ & p_att_tb_in[i].prop )
        {
            (*p_att_db_out)[i].perm |= (PERM(RD, ENABLE) | PERM(RP, AUTH));
        }
        if ( GATT_PROP_AUTHEN_WRITE & p_att_tb_in[i].prop )
        {
            (*p_att_db_out)[i].perm |= (PERM(WRITE_COMMAND, ENABLE) | PERM(WRITE_REQ, ENABLE) | PERM(WP, AUTH));
        }

		/*** Handle attm_desc max_size ***/
		if ( (*p_att_db_out)[i].uuid == GATT_PRIMARY_SERVICE_UUID )
		{
			(*p_att_db_out)[i].max_size = 0;
		}
		else if ( (*p_att_db_out)[i].uuid == GATT_CHARACTER_UUID )
		{
			// note: max_size: for characteristic declaration contains handle offset.
			(*p_att_db_out)[i].max_size = i + 1;
		}
		else
		{
			(*p_att_db_out)[i].max_size = p_att_tb_in[i].max_size;
		}
		
	}
    

	/*** Handle 16 bytes UUIDs if they exist ***/
	if ( uuid128_cnt > 0)
	{
		printf(">>>>>>>>>>>>>>>> number of uuid128: %d \r\n", uuid128_cnt);
		*p_uuid128 = (uint8_t*)ke_malloc(uuid128_cnt * UUID_SIZE_16 ,KE_MEM_NON_RETENTION);
		uint8_t *p_tmp = *p_uuid128;
		
		// Copy service 16 bytes UUID first if it exists
		if ( p_att_tb_in[0].max_size == UUID_SIZE_16 )
		{
			memcpy( p_tmp, p_att_tb_in[0].p_data, UUID_SIZE_16 );
			p_tmp += UUID_SIZE_16;
		}

		// Now find all the 16 bytes UUIDs and copy them
		for ( int i = 1; i < att_nb; i++ )
		{
			if ( p_att_tb_in[i].uuid.size == UUID_SIZE_16 )
			{
				memcpy( p_tmp, p_att_tb_in[i].uuid.p_uuid, UUID_SIZE_16 );
				p_tmp += UUID_SIZE_16;
			}
		}

#if 0	// Just for debug
		for (int i = 0; i < uuid128_cnt; i++)
		{	
			printf(">>>>>>>>>>>>>>>> uuid128:");
			for (int idx = 0 + i * 16; idx < (i+1) * 16; idx++)
				printf(" 0x%x ", (*p_uuid128)[idx]);
			printf("\r\n");
		}
#endif
	}
}

/*********************************************************************
 * @fn      gatt_servcie_added
 *
 * @brief   Service added callback, here initialize attribute values.
 *			������Ӻã���ʼ��һЩ��Ҫ��attribute value.
 *
 * @param   p_param - struct gapm_profile_added_ind.
 *					  ��Ҫ�����ҵ�service id.
 *
 * @return  None.
 */
static void gatt_servcie_added( void *p_param)
{
    // �ص���������ʵservice��att table���ݿⶼ�Ѿ���ʼ�������
    // ������Ҫ��ʼ��һЩgatt service table�����value������att ���ݿ�����ĳ�ʼֵ

    // ���ҵ���Ӧ��service ID���ڱ���attribute table���ҵ�value��CCC ��user description�����г�ʼ��
    for ( int svc_idx = 0; svc_idx < total_service_num; svc_idx++ )
    {
        //printf(">>>>>>>>>>>>>>>> Init values svd ID: %d \r\n", gatt_svc_list[svc_idx].svc_id);
        if ( gatt_svc_list[svc_idx].svc_id == ((struct gapm_profile_added_ind*)p_param)->prf_task_id )
        { 
            for ( int att_idx = 0; att_idx < gatt_svc_list[svc_idx].att_nb; att_idx++ )
            {
                //printf(">>>>>>>>>>>>>>>> Init values att idx: %d \r\n", att_idx);
                if ( gatt_svc_list[svc_idx].p_att_tb[att_idx].uuid.size == UUID_SIZE_2 )
                {
                    uint16_t uuid = BUILD_UINT16( gatt_svc_list[svc_idx].p_att_tb[att_idx].uuid.p_uuid[0], gatt_svc_list[svc_idx].p_att_tb[att_idx].uuid.p_uuid[1] );
                    //printf(">>>>>>>>>>>>>>>> Init values UUID: %04X \r\n", uuid);

                    /*** Handle value ***/
                    if ( uuid == GATT_CHARACTER_UUID )
                    {
                        // ��һ��att_idx��Ӧ����value
                        if ( gatt_svc_list[svc_idx].p_att_tb[att_idx+1].p_data != NULL )
                        {
                            // �������ֵ�ı�Ҫ��ʼ��
                            // ......
                        }
                    }
                    /*** Handle CCC ***/
                    else if ( uuid == GATT_CLIENT_CHAR_CFG_UUID )
                    {
                        if ( gatt_svc_list[svc_idx].p_att_tb[att_idx].p_data != NULL )
                        {
                            // �������ֵ�ı�Ҫ��ʼ��
                            // ......
                        }
                    }
                    /*** Handle characteristic user description ***/
                    else if ( uuid == GATT_CHAR_USER_DESC_UUID )
                    {
                        if ( gatt_svc_list[svc_idx].p_att_tb[att_idx].p_data != NULL )
                        {
                            //printf(">>>>>>>>>>>>>>>> Init values adding desc \r\n");
                            // ���ҵ���Ӧ��attribute handle
                            uint16_t att_handle = user_get_prf_start_hdl(gatt_svc_list[svc_idx].svc_id) + att_idx;
                            // ��ʼ����Ӧֵ
                            attmdb_att_set_value(att_handle, gatt_svc_list[svc_idx].p_att_tb[att_idx].max_size, 0, gatt_svc_list[svc_idx].p_att_tb[att_idx].p_data);
                        }
                        
                    }
                }
            }
        }
    }

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
void gatt_add_service(gatt_service_t *p_service)
{
    user_svc_req_t req;
	user_svc_cfg_t cfg;
	gatt_service_t *p_svc;
	uint8_t *p_uuid128;
	struct attm_desc *p_att_db;

	// ����attribute table�Ŀռ䣬������
	p_att_db = ( struct attm_desc* )ke_malloc((p_service->att_nb) * sizeof(struct attm_desc), KE_MEM_NON_RETENTION);
	memset(p_att_db, 0, (p_service->att_nb) * sizeof(struct attm_desc));
	
	gatt_att_tb_convert( &p_att_db, &p_uuid128, p_service->p_att_tb, p_service->att_nb);
	
	// req
    req.app_task 	= TASK_APP;
    req.prf_id 		= user_get_free_prf_id();
    req.operation   = GAPM_PROFILE_TASK_ADD;
	req.sec_lvl     = PERM(SVC_AUTH, ENABLE);
#if 0
	if (service.svc_uuid.len == 16) // 16 bytes = 128 bits
		req.sec_lvl |= PERM_VAL(SVC_UUID_LEN, PERM_UUID_128);	
#else
	if ( p_service->p_att_tb[0].max_size == UUID_SIZE_16 )
	{
		req.sec_lvl |= PERM_VAL(SVC_UUID_LEN, PERM_UUID_128);
	}
#endif
    req.start_hdl   = 0;

	// cfg
	//if (service.svc_uuid.len == 16)
	if ( p_service->p_att_tb[0].max_size == UUID_SIZE_16 )
	{
		cfg.svc_uuid = ATT_UUID_16(0x0000);
		//cfg.db_ext_uuid128 = service.svc_uuid.p_uuid128;
		cfg.db_ext_uuid128 = p_uuid128;
	}
	//else if (service.svc_uuid.len == 2)
	else if ( p_service->p_att_tb[0].max_size == UUID_SIZE_2 )
	{
#if 0
		if (service.svc_uuid.uuid16 != 0x0000) 
			cfg.svc_uuid = ATT_UUID_16(service.svc_uuid.uuid16);
#else
		uint16_t uuid = BUILD_UINT16( p_service->p_att_tb[0].p_data[0], p_service->p_att_tb[0].p_data[1] );
		if ( uuid != 0x0000 )
			cfg.svc_uuid = ATT_UUID_16(uuid);
#endif
		cfg.db_ext_uuid128 = NULL;
	}
	//cfg.svc_att_db = (struct attm_desc *)(service.p_att_tb);
	//memcpy(p_att_db, (struct attm_desc *)(service.p_att_tb), sizeof(struct attm_desc) * (service.att_nb));
    cfg.svc_att_db = p_att_db;
    cfg.svc_att_nb = p_service->att_nb;
    cfg.svc_db_func = gatt_msg_handler;//service.p_gatt_cb;
    
#if 0	// for dubug    
    for (int i = 0; i < service.att_nb; i++)
		printf(">>>>>>>>>>>>>>>> att index: %d, uuid: 0x%04x, perm: 0x%04x, ext_perm: 0x%04x, max_size: %d\r\n", i, p_att_db[i].uuid, p_att_db[i].perm, p_att_db[i].ext_perm, p_att_db[i].max_size);
		//printf(">>>>>>>>>>>>>>>> att index: %d, uuid: 0x%04x, perm: 0x%04x, ext_perm: 0x%04x, max_size: %d\r\n", i, service.p_att_tb[i].uuid, service.p_att_tb[i].perm, service.p_att_tb[i].ext_perm, service.p_att_tb[i].max_size);
#endif
	if(user_add_svc_to_list(&req, &cfg, NULL) == ERR_OK)
	{
        p_service->svc_id = req.prf_id;
		printf(">>>>>>>>>>>>>>>> svc added to list: ID: %d \n", req.prf_id);
	}
	
	p_svc = &(gatt_svc_list[total_service_num++]);
	//printf("ATT_INFO_REQ2,hdl:%d,%d\r\n",param->handle,user_get_prf_start_hdl(service.svc_id);
	memcpy((void *)p_svc, (void *)(p_service), sizeof(gatt_service_t));

    // ������ע��ص�������APP_EVT_ID_SVC_ADDED �� APP_EVT_ID_DB_ADDED �����¼���Ӧ�Ļص�������
    // app_task.c ���� ��GAPM_PROFILE_ADDED_IND�¼�����ǰ��һ�����
    // app_svc_added_ind_func(param); �� app_db_init_complete_ind_func();
    // �����Ӧ����app_svc_added_ind_func(param);������ ��struct gapm_profile_added_ind��
    // ����ע������ص���Ŀ���ǿ������ڿ������ʼ����gatt service table�����value������att ���ݿ�����ĳ�ʼֵ
    // Ȼ���ٵ���Ӧ�ò㶨���APP_EVT_ID_DB_ADDED �ص���������ʼ�㲥֮�ࡣ
    appm_set_cb_func(APP_EVT_ID_SVC_ADDED, (app_callback_func_t)gatt_servcie_added);
    
    appm_add_svc();

}

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
void gatt_notification(gatt_ntf_t ntf_att, uint8_t *p_data, uint8_t data_len)
{
    struct user_prf_pkt pkt_param;
    pkt_param.prf_id = ntf_att.svc_id;
    pkt_param.att_idx = ntf_att.att_idx;
    pkt_param.op_type = GATTC_NOTIFY;  //  GATTC_NOTIFY / GATTC_INDICATE

    pkt_param.packet = p_data;
    pkt_param.packet_size = data_len;
    user_profile_send_ntf(&pkt_param);
	 
}

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
 void gatt_indication(gatt_ind_t ind_att, uint8_t *p_data, uint8_t data_len)
 {
    struct user_prf_pkt pkt_param;
    pkt_param.prf_id = ind_att.svc_id;
    pkt_param.att_idx = ind_att.att_idx;
    pkt_param.op_type = GATTC_INDICATE;  //  GATTC_NOTIFY / GATTC_INDICATE

    pkt_param.packet = p_data;
    pkt_param.packet_size = data_len;
    user_profile_send_ntf(&pkt_param);
	 
 }
	  

