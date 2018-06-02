
#include "img/imgio.inl"

#include <direct.h>

int test_tif()
{
  char* szFileName;
  img_t im[1] = {0};

  _chdir("E:/pub/bin/pdf");

  szFileName = "SSS00.tif";
  szFileName = "1.tif";
  szFileName = "14-08.TIF";
  szFileName = "009.TIF";
  szFileName = "22.tif";
  szFileName = "2.tif";
  tif_load_filename(szFileName, 0, im);
  return 0;
}
#if 0
int test_tif()
{
  int nTotalFrame;
  char* dtitle;
  char* szFileName;
  TIFF* tiff = NULL;
  img_t im[1] = {0};
  int width, height;
  float resolution;
  uint16 bitspersample;
  uint16 samplesperpixel;
  uint16 bitsperpixel;
  DWORD dwBytePerLine;
  DWORD64 dwLeng;
  BYTE* pData;
  uint32* raster;
  uint32* row;
  LPBYTE bits2;
  int x, y;

  _chdir("E:/pub/bin/pdf");

  szFileName = "SSS00.tif";
  szFileName = "1.tif";
  szFileName = "14-08.TIF";
  szFileName = "009.TIF";
  szFileName = "22.tif";
  szFileName = "2.tif";
  tif_load_filename(szFileName, 0, im);
  //return 0;
  tiff = TIFFOpen(szFileName, "r");//打开Tiff文件，得到指针，以后所有的操作都通过指针进行
  nTotalFrame = TIFFNumberOfDirectories(tiff);    //得到图像的总帧数

  //TIFFSetDirectory(tiff,0);
  //我们打开第一幅图，也就是第0帧，如果是第1帧，第二个参数写1，由此类推。因为Windows下图像基本
  //操作都是以BMP格式进行，我们读出该帧并转成BMP格式。

  TIFFGetField(tiff, TIFFTAG_PAGENAME, &dtitle);
  //得到该帧的名字，存放在dtitle中。

  TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);  //得到宽度
  TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);//得到高度

  resolution = (float)MAX(width, height);

  bitspersample = 1;
  samplesperpixel = 1;

  TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
  //每个像素占多少机器字，24位图samplesperpixel应该等于3。
  TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  //每一个机器字长，这里应为8。

  bitsperpixel = bitspersample * samplesperpixel;
  //算出每个像素占多少bit，24位图，值为24
  dwBytePerLine = (width * bitsperpixel + 31) / 32 * 4;
  //由上面几个参数算出图像每行所占字节(BYTE)数。

  dwLeng = height * dwBytePerLine; //在内存里存放这帧图像数据所需要的长度
  pData = MALLOC(BYTE, dwLeng);    //为存放数据分配内存空间


  raster = (uint32*)malloc(width * height * sizeof(uint32));
  TIFFReadRGBAImage(tiff, width, height, (uint32*)pData, 1);
  //以上几行读出该帧数据,保存到raster中。

  row = &raster[0];
  bits2 = pData;

  for (y = 0; y < height; y++) {
    LPBYTE bits = bits2;

    for (x = 0; x < width; x++) {
      *bits++ = (BYTE)TIFFGetB(row[x]);
      *bits++ = (BYTE)TIFFGetG(row[x]);
      *bits++ = (BYTE)TIFFGetR(row[x]);
    }

    row += width;
    bits2 += dwBytePerLine;
  }

  _TIFFfree(raster);
#if 0
#endif
  return 0;
}
#endif
