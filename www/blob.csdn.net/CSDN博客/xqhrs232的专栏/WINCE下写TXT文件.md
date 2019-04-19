# WINCE下写TXT文件 - xqhrs232的专栏 - CSDN博客
2009年10月17日 13:38:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1390
void  CreateLogoTxtFile(void)  
{
//HANDLE   ghSPILogoTxt  ;
ghSPILogoTxt=CreateFile(TEXT("//NandFlash//SPILogo.txt"),  
    GENERIC_READ|GENERIC_WRITE,
  0,        // comm devices must be opened with exclusive access
  NULL,       // no security attributes
//  OPEN_EXISTING,     // comm devices must use OPEN_EXISTING
       CREATE_ALWAYS,
  0,        // Async I/O
  NULL); 
//CloseHandle(ghSPILogoTxt);
}
//=====================================================
char   NumberToChar(unsigned char data)  //数字转字符
{
    char retval;
   if((data>=0)&&(data<=9))
   {
       retval=data-0+0x30;
   }
   else if((data>=0x0a)&&(data<=0x0f))
   {
        retval=data-0x09+0x40;
   }
   return   retval;
}
//=====================================================
unsigned char   CharToNumber( char c)  //字符转数字
{
    unsigned char retval;
   if((c>='0')&&(c<='9'))
   {
       retval=c-0x30;
   }
   else if((c>='a')&&(c<='f'))
   {
        retval=c-0x57;
   }
   else if((c>='A')&&(c<='F'))
   {
        retval=c-0x37;
   }
   return   retval;
}
//=====================================================
CreateLogoTxtFile(  );//创建一个LOGOTXT文件，用于写SPI数据
for(j=0;j<dataNumBackUp;j++)
{
 SetFilePointer (ghSPILogoTxt, 0, NULL, FILE_END) ;
 SPIRxdSfrData=datRecBuf[j];
 Bitdata=(SPIRxdSfrData>>4)&0x0f; //高4位，按16进制形式写入文件
 c=NumberToChar(Bitdata);
 WriteFile(ghSPILogoTxt,&c,1,&returntxt,NULL); //写数据
 //---------------------------------------------------------
 Bitdata=(SPIRxdSfrData)&0x0f;//低4位，按16进制形式写入文件
 c=NumberToChar(Bitdata);
 WriteFile(ghSPILogoTxt,&c,1,&returntxt,NULL); //写数据
 tempData=0x20;
  WriteFile(ghSPILogoTxt,&tempData,1,&returntxt,NULL); //写入空格
//===========================================================
 if(j%64==0x00)  //写入回车换行
 {
  tempData=0x0d;
  WriteFile(ghSPILogoTxt,&tempData,1,&returntxt,NULL); 
  tempData=0x0a;
  WriteFile(ghSPILogoTxt,&tempData,1,&returntxt,NULL); 
 }
}
 CloseHandle(ghSPILogoTxt);  //数据写完，关闭对应的文件句柄
