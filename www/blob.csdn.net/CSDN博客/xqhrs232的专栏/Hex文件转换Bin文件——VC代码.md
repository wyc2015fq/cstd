# Hex文件转换Bin文件——VC代码 - xqhrs232的专栏 - CSDN博客
2013年03月08日 21:31:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2074
原文地址::[http://flying-in-rain.blog.163.com/blog/static/1318685942009102564347569/](http://flying-in-rain.blog.163.com/blog/static/1318685942009102564347569/)
(2009-08-27)
//FuncCode.h
＃i nclude "stdafx.h"
int HexToBin(char* pHex,BYTE* pBin);
//Hex转换Bin文件
//pHex: *.Hex文件的内容
//pBin: 存放数据的缓冲
//返回值:数据块字节数
BOOL CharToByte(char* pChar,BYTE* pByte);
//两个字符转换成1个字节，例如"af"->0xaf
//pChar:2字符
//pByte:1字节
//返回值:TRUE表示转换成功
//FuncCode.cpp
＃i nclude "FuncCode.h"
＃i nclude "StdAfx.h"
int HexToBin(char* pHex,BYTE* pBin)//pBin指向的缓存必须不小于64k
{ 
//pHex指向的缓存必须以0结尾
   CString strHex;
   CArray<BYTE,BYTE>* pByteArray=NULL;
   CArray<CArray<BYTE,BYTE>*,CArray<BYTE,BYTE>*> arByteRec;
   strHex=pHex;
   int nEnd;
   BYTE byteTmp;
   int n=0,m=0;
   nEnd=strHex.Find(":00000001");
   if(nEnd<0)//先判断文件结尾处
   {
     AfxMessageBox("*.Hex文件转换失败(未找到\":00000001\"记录)!");
     return -1;
   }
   while(n<nEnd)//先把所有字符转换成字节
   {
     if(pHex[n]==':')//发现':'就新增一条记录
     {
       pByteArray=new CArray<BYTE,BYTE>;
       arByteRec.Add(pByteArray);//增加一条记录
       n ;
     }
     else if(pHex[n]==0x0d)//检查是不是换行
     {
       n ;
     }
     else if(pHex[n]==0x0a)
     {
       n ;
     }
     else//字符转换成字节
     {
       if(CharToByte(&pHex[n],&byteTmp))
       {
         if(pByteArray==NULL)
         return -1;
         pByteArray->Add(byteTmp);//增加一个字节
         n ;
         n ;
       }
       else
       {
         AfxMessageBox("*.Hex文件转换失败(错误的字符)!");
         return -1;
       }
     }
   }
   int nRec=arByteRec.GetSize();
   int nAddr=0;//buf中的偏移地址
   BYTE* pBuf;
   int nLength=0;//数据块长度
   int nByte=0;
   memset(pBin,0xff,0xffff);//先把每个单元置成FFH
   for(n=0;n<=nRec;n )//分析每条记录
   {
     pByteArray=arByteRec.GetAt(n);
     if(pByteArray->GetSize()<5)
     {
       AfxMessageBox("*.Hex文件转换失败(错误的记录)!");
       return -1;
     }
     if(pByteArray->GetAt(3)==0)//判断是不是数据记录
     {
       nByte=(int)pByteArray->GetAt(0);
       if(nByte>(pByteArray->GetSize()-4))//记录中数据长度不符合
       {
         AfxMessageBox("*.Hex文件转换失败(错误的记录)!");
         return -1;
       }
       ((BYTE*)&nAddr)[0]=pByteArray->GetAt(2);//装入地址
       ((BYTE*)&nAddr)[1]=pByteArray->GetAt(1);
       pBuf=&pBin[nAddr];
       for(m=0;m<=nByte;m )//把数据都拷贝到pBin
       {
         pBuf[m]=pByteArray->GetAt(4 m);
         nLength ;
       }
     }
     else if(pByteArray->GetAt(3)==2)
     {
       AfxMessageBox("*.Hex文件转换失败(不支持大于64k)!");
       return -1;
     }
     else if(pByteArray->GetAt(3)==4)
     {
       AfxMessageBox("*.Hex文件转换失败(不支持大于64k)!");
       return -1;
     }
     else
     {
       AfxMessageBox("*.Hex文件转换失败(错误的记录)!");
       return -1;
     }
   }
   return nLength; 
}
BOOL CharToByte(char* pChar,BYTE* pByte)
{
   char h,l;
   h=pChar[0];//高4位
   l=pChar[1];//低4位
   if(l>='0'&&l<='9')
     l=l-'0';
   else if(l>='a' && l<='f')
     l=l-'a' 0xa;
   else if(l>='A' && l<='F')
     l=l-'A' 0xa;
   else
     return FALSE;
   if(h>='0'&&h<='9')
     h=h-'0';
   else if(h>='a' && h<='f')
     h=h-'a' 0xa;
   else if(h>='A' &&h <='F')
     h=h-'A' 0xa;
   else
     return FALSE;
   *pByte=(BYTE)h*16 l;
   TRACE("BYTE:x\n",*pByte);
   return TRUE
}
