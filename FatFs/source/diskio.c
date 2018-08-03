/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "usb_host.h"

/* Definitions of physical drive number for each drive */
#define   USB_DEFAULT_BLOCK_SIZE       (512)

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    DRESULT  res = RES_ERROR;
	 
	  if(USBH_MSC_UnitIsReady(&hUsbHostHS, pdrv))
		{
		   res = RES_OK;
		}
		else
		{
			 res = RES_ERROR;
		}	
  
    return res;		
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
   DRESULT  res = RES_ERROR;
	 MSC_LUNTypeDef   info;
	
	 if(USBH_MSC_Read(&hUsbHostHS, pdrv, sector, buff, count) == USBH_OK)
	 {
	    res = RES_OK;
	 }
	 else
	 {
	    USBH_MSC_GetLUNInfo(&hUsbHostHS, pdrv, &info);
		 
		  switch(info.sense.asc)
			{
				 case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
				 case SCSI_ASC_MEDIUM_NOT_PRESENT:
				 case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
					    USBH_ErrLog("USB Disk is not ready");
				      res = RES_NOTRDY;
				      break;
				 default: 
					    res = RES_ERROR;
				      break;
			}
	 }
	 
	 return  res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
   DRESULT  res = RES_ERROR;
	 MSC_LUNTypeDef  info;
	
	 if(USBH_MSC_Write(&hUsbHostHS, pdrv, sector, (BYTE *)buff, count) == USBH_OK)
	 {
	    res = RES_OK;
	 }
	 else
	 {
	    USBH_MSC_GetLUNInfo(&hUsbHostHS, pdrv, &info);
		 
		  switch(info.sense.asc)
			{
				case SCSI_ASC_WRITE_PROTECTED:
					   USBH_ErrLog("USB Disk is Write protected!");
				     res = RES_WRPRT;
				     break;
				case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
				case SCSI_ASC_MEDIUM_NOT_PRESENT:
				case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
					   USBH_ErrLog("USB_Disk_is not ready!");
				     res = RES_NOTRDY;
				     break;
				default:
					   res = RES_ERROR;
				     break;
			}
	 }
	 
	 return  res;
}




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT  res = RES_ERROR;
	  MSC_LUNTypeDef  info;
	
	  switch(cmd)
		{
			/* Make sure that no pending wrtie prosess */
			case CTRL_SYNC:
				   res = RES_OK;
			     break;
			
			/* Get number of sectors on the disck (DWORD)*/
			case GET_SECTOR_COUNT:
				   if(USBH_MSC_GetLUNInfo(&hUsbHostHS, pdrv, &info) == USBH_OK)
					 {
					    *(DWORD*)buff = info.capacity.block_nbr;
						  res = RES_OK;
					 }
					 else
					 {
						   res = RES_ERROR;
					 }
					 break;
					 
			/* Get R/W sector size(WORD) */
			case GET_SECTOR_SIZE:
				   if(USBH_MSC_GetLUNInfo(&hUsbHostHS, pdrv, &info) == USBH_OK)
				   {
				     *(DWORD*)buff = info.capacity.block_size;
				  	 res = RES_OK;
				   }
				   else
				  {
				     res = RES_ERROR;
				  }
				  break;
				
				/* Get erase block size in uint of sector (DWORD) */
			case GET_BLOCK_SIZE:
				  if(USBH_MSC_GetLUNInfo(&hUsbHostHS, pdrv, &info) == USBH_OK)
					{
					   *(DWORD*)buff = info.capacity.block_size / USB_DEFAULT_BLOCK_SIZE;
						 res = RES_OK;
					}
					else
					{
					    res = RES_ERROR;
					}
					break;
					
			default:
				  res = RES_PARERR;
		}
		
		return res;
}


//获取时间
DWORD get_fattime(void)
{
   return 0;
}
