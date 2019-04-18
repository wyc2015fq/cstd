# CxImage读取与创建多页TIFF文件 - 深之JohnChen的专栏 - CSDN博客

2010年03月31日 15:52:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4247


如何读取一个多页TIFF 
CxImage cxMulti;
cxMulti.Load("c://multi.tif" , CXIMAGE_FORMAT_TIF);//加载一个多页tiff文件
int nPageCount = cxMulti.GetNumFrames();//得到tiff文件的页数
CxImage *pImageData = new CxImage[nPageCount+1];
for(int i=0;i<nPageCount;i++)
{
  cxMulti.SetFrame(i);//设置当前帧为i编号图像
  cxMulti.Load("c://multi.tif" , CXIMAGE_FORMAT_TIF);
  pImageData[i].Copy(cxMulti);//将此页图像复制到pImageData[i]中
}
if(pImageData)
{
   delete []pImageData;
   pImageData = NULL;
}

如何创建一副多页TIFF 
CxImage * pimage[ 3 ];
pimage[ 0 ] =& image1;
pimage[ 1 ] =& image2;
pimage[ 2 ] =& image3;
FILE * hFile;
hFile = fopen( " multipage.tif " , " w+b " );
CxImageTIF multiimage;
multiimage.Encode(hFile,pimage, 3 );
fclose(hFile);
或者
FILE * hFile;
hFile = fopen( " c://multi.tif " , " w+b " );
CxImageTIF image;
image.Load( " c://1.tif " ,CXIMAGE_FORMAT_TIF);
image.Encode(hFile, true );
image.Load( " c://2.bmp " ,CXIMAGE_FORMAT_BMP);
image.Encode(hFile, true );
image.Load( " c://3.png " ,CXIMAGE_FORMAT_PNG);
image.Encode(hFile);
fclose(hFile); 

