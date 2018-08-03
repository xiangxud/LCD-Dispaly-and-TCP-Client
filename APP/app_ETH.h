/**
  ******************************************************************************
  * @file    app_ETH.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_ETH.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_ETH_H
#define _APP_ETH_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lwip/netif.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* DHCP process states */
#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5
	
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void User_notification(struct netif *netif);

#ifdef USE_DHCP
void DHCP_Process(struct netif *netif);
void DHCP_Periodic_Handle(struct netif *netif);
#endif

#endif /* end of _APP_ETH_H */





