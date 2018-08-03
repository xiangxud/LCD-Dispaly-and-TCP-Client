#ifndef _FUNCTION_H
#define _FUNCTION_H


#include "stm32f4xx_hal.h"



void   ASCII2HEX(uint8_t *mdata,const uint8_t *sdata,uint16_t len);  /*ASII��HEX�໥ת��*/
void   HEX2ASCII(uint8_t *mdata,const uint8_t *sdata,uint16_t len);

uint16_t    CRC16(const uint8_t *puchMsg,uint16_t usDataLen);       /*���շ�������У��*/
uint8_t     Sum_Check(const uint8_t *data,uint16_t Len);
uint16_t    CHKsum(const uint8_t* data,uint16_t len);
uint16_t    LCHKsum(uint16_t data);

#endif




