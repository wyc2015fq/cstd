# STM32 对内部FLASH读写接口函数 - xqhrs232的专栏 - CSDN博客
2013年04月27日 16:41:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：915
原文地址::[http://blog.sina.com.cn/s/blog_6035432c0100vcve.html](http://blog.sina.com.cn/s/blog_6035432c0100vcve.html)
因为要用内部FLASH代替外部EEPROM，把参数放在STM32的0x08000000+320K处，其中20K是bootloader，300K是应用程序。
原理：先要把整页FLASH的内容搬到RAM中，然后在RAM中改动，然后擦除整页FLASH，再把改动后的内容写入原Flash页。下面程序调试通过。
/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          :
*                  -RomAddr
*                  -NumByteToRead
*                  -pRomData
* Output         : None
* Return         : None
*******************************************************************************
void I2C_EE_BufferRead(u16 RomAddr,u16 NumByteToRead,u8 *pRomData)
{ 
    u32 param_flashbase;
    u8* ptr;
    param_flashbase = 0x8000000+(300+20)*1024;
    ptr = (u8*)(param_flashbase + RomAddr);
    while( NumByteToRead-- >0)
    {
        *pRomData = *ptr;            //直接赋值即可
         printf("0x%x ",*pRomData); 
         pRomData++;
         ptr++;
    }
    return;
} 
/*******************************************************************************
* Function Name  :I2C_EE_BufferWrite
* Description    : Write a block of data to the EEPROM.
* Input          :
*                  -RomAddr
*                  -NumByteToRead
*                  -pRomData
* Output         : None
* Return         : None
*******************************************************************************
void I2C_EE_BufferWrite(u8 DeviceType,u8 SlaveAddr,u16 RomAddr,u16 NumByteToWrite,u8 *pRomData)
{
  uint32_t param_flashbase;
  uint32_t  tempaddress;
  uint32_t  startaddress;
  uint32_t FlashAddress;
  uint32_t datasource;
  u8 buf1[PAGE_SIZE];
  u8 buf2[PAGE_SIZE];
  u32 pagenumber = 0x0;
  u32 EraseCounter = 0x0;
  u32 i = 0;
  FLASH_Status FLASHStatus = FLASH_COMPLETE;
 param_flashbase = 0x8000000+(300+20)*1024;
  startaddress=tempaddress = param_flashbase+RomAddr;
 /*********************起始指针不在Flash页的开始端*********************
 if( (tempaddress%PAGE_SIZE) != 0)
 {   printf("startptr  not in Page head \r\n");
  if(  ((startaddress%PAGE_SIZE)+NumByteToWrite) >  PAGE_SIZE   ) /*超出一页范围
  {
      I2C_EE_BufferRead(0,0,(tempaddress-(tempaddress % PAGE_SIZE)),PAGE_SIZE,buf1);  /*把起始地址所在页的内容读到内存buf1中
      memcpy(buf1+(tempaddress % PAGE_SIZE),pRomData,PAGE_SIZE-(tempaddress % PAGE_SIZE)); /*把需要写入的数据覆盖到buf1中
      while(  FLASHStatus == FLASH_ErasePage(tempaddress)   )       /*buf1写入到Flash
     {
      i=PAGE_SIZE/4;
         datasource = (uint32_t)buf1;
         FlashAddress = tempaddress-(tempaddress % PAGE_SIZE);
         while(i-- >0)
         {
             FLASH_ProgramWord(FlashAddress,*(uint32_t*)datasource);
             if (*(uint32_t*)FlashAddress != *(uint32_t*)datasource)           
         {
          printf("I2C_EE_BufferWrite error!\r\n");
          return ;
          }
          datasource += 4;
          FlashAddress += 4;    
    }
    break;
   }
   NumByteToWrite -= PAGE_SIZE-(startaddress % PAGE_SIZE); 需要写入字节数减去，上面覆盖上去的数据的字节数
   tempaddress +=  PAGE_SIZE-(tempaddress % PAGE_SIZE);        /*把ptr指针指向下一个页起始位置
   if((NumByteToWrite % PAGE_SIZE) != 0) /*末指针不在Flash页的开始端
   {
//读取1 PAGE 数据到内存，修改，然后写进去
       I2C_EE_BufferRead(0,0,tempaddress,PAGE_SIZE,buf2);
       memcpy(buf2,pRomData+PAGE_SIZE-startaddress%PAGE_SIZE+NumByteToWrite-NumByteToWrite%PAGE_SIZE,(NumByteToWrite%PAGE_SIZE));
    while(  FLASHStatus == FLASH_ErasePage( tempaddress+NumByteToWrite) )   /*把buf2写入到Flash中*
    {
        i=PAGE_SIZE/4;
        datasource = (uint32_t)buf2;
        FlashAddress = (tempaddress+NumByteToWrite-(NumByteToWrite % PAGE_SIZE));  /*末地址指针的页首
        while(i-- >0)
       {
           FLASH_ProgramWord(FlashAddress,*(uint32_t*)datasource);
           if (*(uint32_t*)FlashAddress != *(uint32_t*)datasource)            
           {
                printf("I2C_EE_BufferWrite error!\r\n");
                return ;
            }
            datasource += 4;
            FlashAddress += 4;          
      }
     break;
    }      
   }
   NumByteToWrite -= NumByteToWrite % PAGE_SIZE;
//擦除Flash
    pagenumber =  NumByteToWrite/PAGE_SIZE;
   for (EraseCounter = 0; (EraseCounter < pagenumber)
 && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
   {
         FLASHStatus = FLASH_ErasePage( tempaddress + PAGE_SIZE*EraseCounter );
   }
//写Flash
   datasource = *(uint32_t *)(pRomData+ PAGE_SIZE-(startaddress % PAGE_SIZE)  );
   FlashAddress = tempaddress;
   while( pagenumber-- > 0 )
   {
    i=PAGE_SIZE/4;
    while(i -- >0)
    {
            FLASH_ProgramWord(FlashAddress,*(uint32_t*)datasource);
            if (*(uint32_t*)FlashAddress != *(uint32_t*)datasource)
           {
               printf("I2C_EE_BufferWrite error!\r\n");
               return ;
           }
           datasource += 4;
           FlashAddress += 4;
    }
   }
  }
  else /*写的内容没有超出一页范围
  {
   printf("FlashWrire --in one page \r\n");
   I2C_EE_BufferRead(0,0,(startaddress-(startaddress % PAGE_SIZE)),PAGE_SIZE,buf1);     /*把起始地址所在页的内容读到内存buf1中
   memcpy( (buf1+(tempaddress % PAGE_SIZE)),pRomData, NumByteToWrite );  /*把需要写入的数据覆盖到buf1中
   while(  FLASHStatus == FLASH_ErasePage(tempaddress)   )
   {
      i=PAGE_SIZE/4;
      datasource = (uint32_t)buf1;
      FlashAddress = tempaddress-(tempaddress % PAGE_SIZE);
      while(i-- >0)
      {
        FLASH_ProgramWord(FlashAddress,*(uint32_t*)datasource);
        if (*(uint32_t *)FlashAddress != *(uint32_t *)datasource) /*读取Flash中的数据，看是否写入正确
        {
               printf("I2C_EE_BufferWrite error!\r\n");
               return ;
