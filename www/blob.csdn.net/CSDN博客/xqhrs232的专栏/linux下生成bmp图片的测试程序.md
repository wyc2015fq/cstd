# linux下生成bmp图片的测试程序 - xqhrs232的专栏 - CSDN博客
2016年05月18日 14:56:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1663
原文地址::[http://wenku.baidu.com/link?url=8PrgVQ_h2yyVAewEz0Ugap_zzVxusWe1d0-ddMtqiJe7NqXuR5g8n6J45iZhjXgfBM16RwhX4bXeLiyaxhml_mx0xoiMwB2GoXYdrcdRROS](http://wenku.baidu.com/link?url=8PrgVQ_h2yyVAewEz0Ugap_zzVxusWe1d0-ddMtqiJe7NqXuR5g8n6J45iZhjXgfBM16RwhX4bXeLiyaxhml_mx0xoiMwB2GoXYdrcdRROS)
相关文章
1、[ARGB32位转化为RGB24位的函数（修改版）](http://blog.csdn.net/tinyhell127/article/details/7859226)----[http://blog.csdn.net/tinyhell127/article/details/7859226](http://blog.csdn.net/tinyhell127/article/details/7859226)
2、[VC创建BMP文件](http://blog.csdn.net/jonahzheng/article/details/8029870)----[http://blog.csdn.net/jonahzheng/article/details/8029870](http://blog.csdn.net/jonahzheng/article/details/8029870)
/*linux  bmp generator*/
#include <string.h>
#include <stdio.h>
typedef int BOOL;
typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef struct {
        WORD    bfType;//2
        DWORD   bfSize;//4
        WORD    bfReserved1;//2
        WORD    bfReserved2;//2
        DWORD   bfOffBits;//4
}__attribute__((packed))FileHead;
typedef struct{
        DWORD      biSize;//4
        LONG       biWidth;//4
        LONG       biHeight;//4
        WORD       biPlanes;//2
        WORD       biBitCount;//2
        DWORD      biCompress;//4
        DWORD      biSizeImage;//4
        LONG       biXPelsPerMeter;//4
        LONG       biYPelsPerMeter;//4
        DWORD      biClrUsed;//4
        DWORD      biClrImportant;//4
}__attribute__((packed))Infohead;
typedef struct
{
    BYTE b;
    BYTE g;
    BYTE r;
}RGB_data;//RGB TYPE
int bmp_generator(char *filename,int height,int width,unsigned char *data)
{
    FileHead bmp_head;
    Infohead bmp_info;
    int size = width*height*3;
    bmp_head.bfType=0x4d42;
    bmp_head.bfSize=size+sizeof(FileHead)+sizeof(Infohead);//24+head+info no quad    
    bmp_head.bfReserved1=bmp_head.bfReserved2=0;
    bmp_head.bfOffBits=bmp_head.bfSize-size;
    //finish the initial of head
    bmp_info.biSize=40;
    bmp_info.biWidth=width;
    bmp_info.biHeight=height;
    bmp_info.biPlanes=1;
    bmp_info.biBitCount = 24;
    bmp_info.biCompress=0;
    bmp_info.biSizeImage=size;
    bmp_info.biXPelsPerMeter=0;
    bmp_info.biYPelsPerMeter=0;
    bmp_info.biClrUsed=0;
    bmp_info.biClrImportant=0;
    //finish the initial of infohead;
    //copy the data
    FILE *fp;
    if(!(fp=fopen(filename,"wb"))) return 0;
    fwrite(&bmp_head,1,sizeof(FileHead),fp);
    fwrite(&bmp_info,1,sizeof(Infohead),fp);
    fwrite(data,1,size,fp);
    fclose(fp); 
    return 0;
}
int main(void)
{
    int i,j;
    RGB_data buffer[1280][800];//hight width
    char *path = "/home/s/800-1280_0.bmp";//generator path
    printf("bmp generator!\n");
    memset(buffer, 0, sizeof(buffer));
    //testdata
    for (i=0;i<1280;i++) //hight
for (j=0;j<800;j++)//width
        {
   if(i%2)
                buffer[i][j].g=buffer[i][j].b=buffer[i][j].r=0;
            else
buffer[i][j].g=buffer[i][j].b=buffer[i][j].r=0xff;
        }
    bmp_generator(path,1280,800,(unsigned char*)buffer);    
    return 0;
}
