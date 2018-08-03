/**
  ******************************************************************************
  * @file    app_ETH.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   ethernet communication 
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_ETH.h"
#include "lwip/dhcp.h"
#include "sys_Variable.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
/* private variables ----------------------------------------------------------*/
/* Expanded Variables ---------------------------------------------------------*/
#ifdef  USE_DHCP
#define MAX_DHCP_TRIES   4
uint32_t DHCPfineTimer = 0;
__IO uint8_t DHCP_state = DHCP_OFF;
#endif

/* private functions prototype ------------------------------------------------*/
/* function body --------------------------------------------------------------*/

/**
  * @brief  Notify the User about the nework interface config status 
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
 {
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#endif /* USE_DHCP */
 }
 else
  {  
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif  /* USE_DHCP */
  } 
}


/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
#ifndef USE_DHCP
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
#endif
  
  if(netif_is_link_up(netif))
  {   
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#else
    IP_ADDR4(&ipaddr, Optimset_st.ucIPAddr_a[0], Optimset_st.ucIPAddr_a[1], 
		         Optimset_st.ucIPAddr_a[2], Optimset_st.ucIPAddr_a[3]);
    IP_ADDR4(&netmask, Optimset_st.ucIPMask_a[0], Optimset_st.ucIPMask_a[1], 
		         Optimset_st.ucIPMask_a[2] ,Optimset_st.ucIPMask_a[3]);
    IP_ADDR4(&gw, Optimset_st.ucIPGateway_a[0], Optimset_st.ucIPGateway_a[1], 
		         Optimset_st.ucIPGateway_a[2], Optimset_st.ucIPGateway_a[3]); 

//  IP_ADDR4(&ipaddr, 192,168,20,100);
//  IP_ADDR4(&gw, 192,168,20,254);
//  IP_ADDR4(&netmask, 255, 255, 255, 0);
					 
    
    netif_set_addr(netif, &ipaddr , &netmask, &gw);  
#endif /* USE_DHCP */   
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);     
  }
  else
  {
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
    
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);   
  }
}

#ifdef USE_DHCP
/**
  * @brief  DHCP_Process_Handle
  * @param  None
  * @retval None
  */
void DHCP_Process(struct netif *netif)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  struct dhcp *dhcp;   
#ifdef USE_LCD 
  uint8_t iptxt[20];
#endif
  
  switch (DHCP_state)
  {
    case DHCP_START:
    {
      ip_addr_set_zero_ip4(&netif->ip_addr);
      ip_addr_set_zero_ip4(&netif->netmask);
      ip_addr_set_zero_ip4(&netif->gw);
      DHCP_state = DHCP_WAIT_ADDRESS;
      dhcp_start(netif);
    }
    break;
    
  case DHCP_WAIT_ADDRESS:
    {
      if (dhcp_supplied_address(netif)) 
      {
        DHCP_state = DHCP_ADDRESS_ASSIGNED;
      }
      else
      {
        dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    
        /* DHCP timeout */
        if (dhcp->tries > MAX_DHCP_TRIES)
        {
          DHCP_state = DHCP_TIMEOUT;
          
          /* Stop DHCP */
          dhcp_stop(netif);
          
          /* Static address used */
					IP_ADDR4(&ipaddr, Optimset_st.ucIPAddr_a[0], Optimset_st.ucIPAddr_a[1], 
									 Optimset_st.ucIPAddr_a[2], Optimset_st.ucIPAddr_a[3]);
					IP_ADDR4(&netmask, Optimset_st.ucIPMask_a[0], Optimset_st.ucIPMask_a[1], 
									 Optimset_st.ucIPMask_a[2] ,Optimset_st.ucIPMask_a[3]);
					IP_ADDR4(&gw, Optimset_st.ucIPGateway_a[0], Optimset_st.ucIPGateway_a[1], 
									 Optimset_st.ucIPGateway_a[2], Optimset_st.ucIPGateway_a[3]);

//  IP_ADDR4(&ipaddr, 192,168,20,100);
//  IP_ADDR4(&gw, 192,168,20,254);
//  IP_ADDR4(&netmask, 255, 255, 255, 0);
									 
          netif_set_addr(netif, &ipaddr, &netmask, &gw);
        }
      }
    }
    break;
  case DHCP_LINK_DOWN:
    {
      /* Stop DHCP */
      dhcp_stop(netif);
      DHCP_state = DHCP_OFF; 
    }
    break;
  default: break;
  }
}

/**
  * @brief  DHCP periodic check
  * @param  localtime the current LocalTime value
  * @retval None
  */
void DHCP_Periodic_Handle(struct netif *netif)
{  
  /* Fine DHCP periodic process every 500ms */
  if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  HAL_GetTick();
    /* process DHCP state machine */
    DHCP_Process(netif);    
  }
}
#endif



/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************/



