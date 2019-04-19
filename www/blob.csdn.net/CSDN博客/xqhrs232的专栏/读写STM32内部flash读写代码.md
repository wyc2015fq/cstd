# 读写STM32内部flash读写代码 - xqhrs232的专栏 - CSDN博客
2013年04月27日 16:41:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1104
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/ws84643557/article/details/7391642](http://blog.csdn.net/ws84643557/article/details/7391642)
由于老师的要求，所以写了一份读写STM32内部FLASH的代码，这样的话就可以把STM32里面没有用来保存代码段的部分用来存储数据了。
由于《stm32flash编程手册》是很久很久以前看的，现在也没心情去仔细看那份手册了。大概浏览了一下，只看到了STM32里面的flash是以16位为单位读写的。
为什么记住这个，因为之前想写一个字节老是出错，所以翻翻手册，果然看到不能写1个字节数据。而且还发现，flash写地址为奇数时也会出错。所以这份代码里面
写flash的地址只能是偶数。浏览过程中还发现，手册里面说写flash和擦除flash的时候会影响cpu的运行速度，没仔细看（真心不想看）。其他具体手册里面的资料的话没有了，
看有没有高手仔细看完，然后写个总结什么的。。。
而且flash的读写保护也没看清楚，好像这些保护都是针对页大小来保护的话。这样的话就可以只保护代码段，这些放数据段不用保护了，这样可以省下flash芯片，不错吧。
废话不多说，直接上代码。。
flash读写代码也不用初始化，拿来直接就用。哈哈。。。。。。。。。。
-------------------DataFlash.c----------------------------------
#include "DataFlash.h"
uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite) {
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;
//    FLASH_UnlockBank1();
    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t*)buf);
      i = i+2;
      iAddress = iAddress + 2;
      buf = buf + 2;
    }
    return iNumByteToWrite;
}
/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *  
  */
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite) {
                /* Unlock the Flash Bank1 Program Erase controller */
        uint32_t secpos;
        uint32_t iNumByteToWrite = iNbrToWrite;
uint16_t secoff;
uint16_t secremain;
uint16_t i = 0;    
        uint8_t tmp[FLASH_PAGE_SIZE];
        FLASH_UnlockBank1();
secpos=iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//扇区地址 
secoff=iAddress & (FLASH_PAGE_SIZE -1);     //在扇区内的偏移
secremain=FLASH_PAGE_SIZE-secoff;           //扇区剩余空间大小 
        volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
        if(iNumByteToWrite<=secremain) secremain = iNumByteToWrite;//不大于4096个字节
        while( 1 ) {
            Flash_Read(secpos, tmp, FLASH_PAGE_SIZE);   //读出整个扇区
            for(i=0;i<secremain;i++) {       //校验数据
       if(tmp[secoff+i]!=0XFF)break;       //需要擦除 
   }
            if(i<secremain) {  //需要擦除
                FLASHStatus = FLASH_ErasePage(secpos); //擦除这个扇区
                if(FLASHStatus != FLASH_COMPLETE)
                  return -1;
                for(i=0;i<secremain;i++) {  //复制
                        tmp[i+secoff]=buf[i];
                }
                Flash_Write_Without_check(secpos ,tmp ,FLASH_PAGE_SIZE);//写入整个扇区  
            } else {
                Flash_Write_Without_check(iAddress,buf,secremain);//写已经擦除了的,直接写入扇区剩余区间.
            }
            if(iNumByteToWrite==secremain) //写入结束了
                break;
            else {
                secpos += FLASH_PAGE_SIZE;
                secoff = 0;//偏移位置为0 
                buf += secremain;  //指针偏移
                iAddress += secremain;//写地址偏移
                iNumByteToWrite -= secremain; //字节数递减
                if(iNumByteToWrite>FLASH_PAGE_SIZE) secremain=FLASH_PAGE_SIZE;//下一个扇区还是写不完
                else secremain = iNumByteToWrite; //下一个扇区可以写完了
            }
         }
        FLASH_LockBank1();
        return iNbrToWrite; 
}
/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to read from flash
  * @retval if success return the number to write, without error
  *  
  */
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) {
        int i = 0;
        while(i < iNbrToRead ) {
           *(buf + i) = *(__IO uint8_t*) iAddress++;
           i++;
        }
        return i;
}
-------------------DataFlash.h----------------------------------
#ifndef   __DATAFLASH_H__
#define   __DATAFLASH_H__
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);
#endif
