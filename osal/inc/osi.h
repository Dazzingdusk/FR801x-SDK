/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
#ifndef OSI_H
#define OSI_H

/*
 * INCLUDES (����ͷ�ļ�)
 */
#include <stdio.h>
#include <stdint.h>

/*
 * MACROS (�궨��)
 */

/*
 * CONSTANTS (��������)
 */

/*
 * TYPEDEFS (���Ͷ���)
 */
  /**
 * OS event definition.
 */
 typedef struct
{
    uint16_t event_id;
    void *param;
    uint16_t param_len;
    uint16_t src_task_id;
} osi_event_t;

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

/** @function OS Interface APIs (OS�ӿ���ص�API)
 * @{
 */

/*********************************************************************
 * @fn      osal_malloc
 *
 * @brief   malloc a buffer.buffer
 * @return  Allocates a memory block whose
 *			����һ��������.
 *
 * @param   size - the size of  size is size; if no memory is available return NULL
 */
void *osal_malloc(uint32_t size);

/*********************************************************************
 * @fn      osal_free
 *
 * @brief  free a block of memory
 * 
 *
 * @param   the pointer of memory buffer
 * @return  NULL
 */
void osal_free(void *ptr);

#endif // end of #ifndef OSI_H
