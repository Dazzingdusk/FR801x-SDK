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
#include "rwip_config.h"
#include "ke_mem.h"

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

/** @function group OS memory APIs (OS�ڴ���ص�API)
 * @{
 */

/*********************************************************************
 * @fn      osal_malloc
 *
 * @brief   malloc a block of memory ����һ��������.
 * @param  size - the size of  size is size; 
 *			
 *
 * @return    the pointer of memory buffer allocated, if no memory is available return NULL
 */
void *osal_malloc(uint32_t size)
{
	return ke_malloc(size, KE_MEM_NON_RETENTION);
}

/*********************************************************************
 * @fn      osal_free
 *
 * @brief  free a block of memory
 * 
 *
 * @param   the pointer of memory buffer
 * @return  NULL
 */
void osal_free(void *ptr)
{
	ke_free(ptr);
}

/** @function group OS memory APIs (OS�ڴ���ص�API)
 * @{
 */
 
 
