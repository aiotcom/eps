#ifndef _bind_H
#define _bind_H
#include "sampleApp.h"
#include "AF.h"
#include "stdio.h"
void StartBind(uint8 NumOfId,const cId_t *pId );
void ResponeBind(uint16 *pId,afAddrType_t *dstAddr);
#endif