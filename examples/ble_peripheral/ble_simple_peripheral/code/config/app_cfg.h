/**
 * Copyright (c) 2019, Tsingtao Freqchip
 * 
 * All rights reserved.
 * 
 * 
 */
#ifndef APP_CFG_H
#define APP_CFG_H

/*
 * INCLUDES (����ͷ�ļ�)
 */
#include <stdio.h>
#include <stdint.h>

/*
 * MACROS (�궨��)
 */
/** @define group ble GAP defines, change here to get your settings (ble��GAP����, �����������޸ĳ�����Ҫ��ģʽ)
 * @{
 */
//!< Advertising mode, �㲥ģʽ
#define GAP_ADV_MODE			GAP_ADV_TYPE_NON_CONN//GAP_ADV_TYPE_UNDIRECT
// Advertising address type, ��ַ����
#define GAP_ADV_ADDR_TYPE		GAP_ADDR_TYPE_PUBLIC  
// Default advertising interval, units of 625us, 160=100ms, �㲥���, 625us Ϊ��λ��160=100ms
#define GAP_ADV_INTV			160
// Advertising channel map, 37, 38, 39, �㲥�ŵ�ѡ��37,38,39
#define GAP_ADV_CHNL_MAP		GAP_ADV_CHAN_ALL
// Advertising discoverable mode, �㲥�ɱ�����ģʽ����
#define GAP_ADV_DIS_MODE		GAP_GEN_DISCOVERABLE
// Advertising filter scan parameters, ����ʲô�����豸ɨ�赽
#define GAP_ADV_FILT_POLICY		ADV_ALLOW_SCAN_ANY_CON_ANY

//!< Connection parameters, ���Ӳ���
#define GAP_CONN_MIN_INTV
#define GAP_CONN_MAX_INTV
#define GAP_CONN_SLAVE_LATENCY
#define GAP_CONN_SUPERVISION_TIMEOUT

/*
 * CONSTANTS (��������)
 */
#define CFG_OTA_ENABLE

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


#endif // end of #ifndef APP_CFG_H
