
# 读与复制.BMP文件 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月30日 21:11:07[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1684


// 读与复制.BMP文件.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#include "Windows.h"
\#include<stdio.h>
using namespace std;
bool readBMP(char *bmpName);
bool saveBMP(char *bmpName,unsigned char *imgBuf,int width,int height,int biBitCount,RGBQUAD *pColorTable);
unsigned char *pBmpBuf;
int bmpWidth;
int bmpHeight;
RGBQUAD *pColorTable;
int biBitCount;
int _tmain(int argc, _TCHAR* argv[])
{
char *readPath = "58A.BMP";
if(readBMP(readPath))
{
cout<<"read BMP is ok!"<<endl;
cout<<"width="<<bmpWidth<<" height="<<bmpHeight<<" biBitCount="<<biBitCount<<endl;
}
else
cout<<"read error!"<<endl;
char *writePath = "copy58A.BMP";
if(saveBMP(writePath,pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable))
cout<<"copy ok!"<<endl;
else
cout<<"copy error!"<<endl;
delete[] pBmpBuf;
if(biBitCount==8)
delete[] pColorTable;
system("pause");
return 0;
}
bool readBMP(char *bmpName)
{
FILE *fp = fopen(bmpName,"rb");
if(fp==NULL)
return 0;
fseek(fp,sizeof(BITMAPFILEHEADER),0);
BITMAPINFOHEADER head;
fread(&head,sizeof(BITMAPINFOHEADER),1,fp);
bmpWidth = head.biWidth;
bmpHeight = head.biHeight;
biBitCount = head.biBitCount;
int lineByte = (bmpWidth*biBitCount/8+3)/4*4;
if(biBitCount==8)
{
pColorTable = new RGBQUAD[256];
fread(pColorTable,sizeof(RGBQUAD),256,fp);
}
pBmpBuf = new unsigned char[lineByte*bmpHeight];
if(pBmpBuf==NULL)
return 0;
fclose(fp);
return 1;
}
bool saveBMP(char *bmpName,unsigned char *imgBuf,int width,int height,int biBitCount,RGBQUAD *pColorTable)
{
if(imgBuf==NULL)
return 0;
int colorTableSize = 0;
if(biBitCount==8)
colorTableSize = 1024;
int lineByte = (width*biBitCount/8+3)/4*4;
FILE *fp = fopen(bmpName,"wb");
if(fp==0)
return 0;
BITMAPFILEHEADER fileHead;
fileHead.bfType = 0x4D42;
fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize + lineByte;
fileHead.bfReserved1 = fileHead.bfReserved2 = 0;
fileHead.bfOffBits = 54 + colorTableSize;
fwrite(&fileHead,sizeof(BITMAPFILEHEADER),1,fp);
BITMAPINFOHEADER head;
head.biBitCount = biBitCount;
head.biClrImportant = 0;
head.biClrUsed = 0;
head.biCompression = 0;
head.biHeight = height;
head.biPlanes = 1;
head.biSize = 40;
head.biSizeImage = lineByte*height;
head.biWidth = width;
head.biXPelsPerMeter = 0;
head.biYPelsPerMeter = 0;
fwrite(&head,sizeof(BITMAPINFOHEADER),1,fp);
if(biBitCount==8)
fwrite(pColorTable,sizeof(RGBQUAD),256,fp);
fwrite(imgBuf,height*lineByte,1,fp);
fclose(fp);
return 1;
}

