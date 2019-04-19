# WINCE下OPENGL ES建模与贴纹理的示例程序 - xqhrs232的专栏 - CSDN博客
2009年12月08日 10:58:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1904标签：[wince																[buffer																[filter																[header																[alignment																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=alignment&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
void CMy2009_12_5_MFCDlg_OpenGLes_Texture_TestDlg::InitOpenGLES()
{
  EGLConfig configs[10];
  EGLint matchingConfigs; 
  const EGLint configAttribs[] =
  {
      EGL_RED_SIZE,       8,
      EGL_GREEN_SIZE,     8,
      EGL_BLUE_SIZE,      8,
      EGL_ALPHA_SIZE,     EGL_DONT_CARE,
      EGL_DEPTH_SIZE,     16,
      EGL_STENCIL_SIZE,   EGL_DONT_CARE,
      EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
      EGL_NONE,           EGL_NONE
  };
  hDC = ::GetWindowDC(hWnd);
  glesDisplay = eglGetDisplay(hDC);  
  if(!eglInitialize(glesDisplay, NULL, NULL)) 
    return ;
  if(!eglChooseConfig(glesDisplay, configAttribs, &configs[0], 10,  &matchingConfigs))
   return ;
  if (matchingConfigs < 1)
   return ;   
  glesSurface = eglCreateWindowSurface(glesDisplay, configs[0], hWnd, configAttribs);
  if(!glesSurface) 
   return ;
  glesContext=eglCreateContext(glesDisplay,configs[0],0,configAttribs);
  if(!glesContext) 
   return ;
  eglMakeCurrent(glesDisplay, glesSurface, glesSurface, glesContext); 
}
//=====================================================================
 //xqhtest.cpp
//Designed by xqh
//Date:2009--12--7
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "../gles/gl.h"
#include "../gles/egl.h"
#include <math.h>
#define PI 3.141592f
extern int ReadBmp (const char *filename, unsigned char  *buffer, int *width, int *height);
extern void WriteBmp(const char *pFilename, const unsigned int *pBuffer, int nWidth, int nHeight);
static GLuint texName;
static GLint TexWidth, TexHeight;
GLint *ImageDataMy1;
GLint *SubImageDataMy1;
GLvoid CreateImageMy(GLint ImageId)
{
    GLint width, height;
    switch (ImageId)
    {
    case 1:
/*
        width = 272;
        height = 272;
        ImageDataMy1 = (GLint*) malloc (width * height * (sizeof (GLint)));
       // ReadBmp ("PocketMory//Bmps//Bkgnd_red.bmp",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
 ReadBmp ("PocketMory//Bmps//Bkgnd_red_272_272.bmp",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
     //  ReadBmp ("PocketMory//Bmps//CEbkgnd.BMP",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
*/
 //width = 128;
 //height = 128;
width = 256;
height = 256;
ImageDataMy1 = (GLint*) malloc (width * height * (sizeof (GLint)));
//ReadBmp ("PocketMory//Bmps//kat_128_128.bmp",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
//ReadBmp ("PocketMory//Bmps//Bkgnd_red_128_128.bmp",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
ReadBmp ("PocketMory//Bmps//Bkgnd_red_256_256.bmp",(unsigned char*) ImageDataMy1, &TexWidth, &TexHeight);
        break;
    case 2:
        width = 74;
        height = 94;
        SubImageDataMy1 = (GLint*) malloc (width * height * (sizeof (GLint)));
        ReadBmp ("PocketMory//Bmps//Amoeba_74_94.bmp",(unsigned char*) SubImageDataMy1, &TexWidth, &TexHeight);
    }
}
GLvoid FreeImageMy(GLint ImageId)
{
    switch (ImageId)
    {
    case 1:
        free (ImageDataMy1);
        break;
    case 2:
        free (SubImageDataMy1);
        break;
    }
}
 int xqhtest_InitTexture (void)
{
/*
static const GLfloat v[]  = {  
 -1.0, -1.0, 0.0,
                 -1.0, 1.0, 0.0, 
                       1.0, -1.0, 0.0, 
                       // 1.0, 1.0, 0.0, 
1.0, -1.0, 0.0,
                 1.0, 1.0, 0.0, 
                       -1.0, 1.0, 0.0, 
 };
*/
static const GLfloat v[]  = {  
 -1.765, -1.0, 0.0,
                 -1.765, 1.0, 0.0, 
                       1.765, -1.0, 0.0, 
                        1.765, 1.0, 0.0, 
/*
1.765, -1.0, 0.0,
                 1.765, 1.0, 0.0, 
                       -1.765, 1.0, 0.0, 
*/
 };
 static const GLbyte t[] = {
       0, 0,
       0, 1,
       1, 0,
       1, 1,
    /*
       1, 0, 
    0, 1,
    1, 1,
    */
   };
    GLint CurrentTexWidth, CurrentTexHeight;
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable (GL_DEPTH_TEST);
    CreateImageMy( 1);
    CurrentTexWidth = TexWidth;
    CurrentTexHeight = TexHeight;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures (1, &texName);
    glBindTexture (GL_TEXTURE_2D, texName);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, CurrentTexWidth, CurrentTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageDataMy1);
   glVertexPointer(3, GL_FLOAT, 0, v);
   glTexCoordPointer(2, GL_BYTE, 0, t);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   FreeImageMy(1);
    return GL_TRUE;
}
 void xqhtest_Reshape(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if(w<=h)
      glOrthof(-1.0, 1.0, -1.0*(GLfloat)h/(GLfloat)w,
                 1.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
    else
      glOrthof(-1.0*(GLfloat)w/(GLfloat)h, 1.0*(GLfloat)w/(GLfloat)h, -1.0,
                 1.0, -1.0, 1.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}
//################################################################################################
int  xqhtestDemo( )
{
 xqhtest_InitTexture( );
 xqhtest_Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
  return  1;
}
void  xqhtestRender( )
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor4f(0.0,1.0,0.0,0.0);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
/*
glDrawArrays(GL_TRIANGLES, 0, 3);
glColor4f(0.0,1.0,0.0,0.0);
glDrawArrays(GL_TRIANGLES, 3, 3);
*/
/*
//GL_INVALID_ENUM 
EGLint   eglError;
eglError= eglGetError( );
*/
   glFlush();
   glDisable(GL_TEXTURE_2D);
}
int  xqhtestDeinit( )
{
  return  1;
}
//===========================================================================
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "../gles/gl.h"
#include <math.h>
int ReadBmp (const char *filename, unsigned char  *buffer, int *width, int *height)
{
       FILE *f;
       BITMAPFILEHEADER bmfh;
       BITMAPINFOHEADER bmih;
       int i;
       unsigned char r, g, b;
       f = fopen(filename, "rb");
       if(f)
       {
               // read file header
               fread(&bmfh, 1, sizeof(bmfh), f);
               // write bitmap header
               fread(&bmih, 1, sizeof(bmih), f);
               *width = bmih.biWidth;
               *height = bmih.biHeight;
               for(i = 0;i < (*width)*(*height);++i)
               {
                       b = fgetc(f); /* write blue */
                       g = fgetc(f); /* write green */
                       r = fgetc(f);   /* write red */
                       *(buffer+4*i) = r;                    //r;
                       *(buffer+4*i+1) = g;                    //g;
                       *(buffer+4*i+2) = b;                    //b;
                       *(buffer+4*i+3) = 0x000000ff;        //a
               }
               fclose(f);
               return 1;
       }
       else
       {
               *width = 0;
               *height = 0;
               return 0;
       }
}
void WriteBmp(const char *pFilename, const unsigned int *pBuffer, int nWidth, int nHeight)
{
       FILE *pf;
       BITMAPFILEHEADER sBmfh;
       BITMAPINFOHEADER sBmih;
       int nIndex;
       unsigned char nR, nG, nB;
       unsigned int nColor_RGB8888;
       pf = fopen(pFilename, "wb");
       if (pf==NULL)
       {
               exit(-1);
       }
       // write file header
       sBmfh.bfType = *((unsigned short*)"BM");
       sBmfh.bfSize =  sizeof(BITMAPFILEHEADER) +
                                       sizeof(BITMAPINFOHEADER) +
                                       (nWidth * 3 * nHeight);
       sBmfh.bfReserved1 = 0;
       sBmfh.bfReserved2 = 0;
       sBmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
       fwrite(&sBmfh, 1, sizeof(sBmfh), pf);
       // write bitmap header
       sBmih.biSize = sizeof(sBmih);
       sBmih.biWidth = nWidth;
       sBmih.biHeight = nHeight;
       sBmih.biPlanes = 1;
       sBmih.biBitCount = 24;
       sBmih.biCompression = 0;
       sBmih.biSizeImage = 0;
       sBmih.biXPelsPerMeter = 10076;
       sBmih.biYPelsPerMeter = 10076;
       sBmih.biClrUsed = 0;
       sBmih.biClrImportant = 0;
       fwrite(&sBmih, 1, sizeof(sBmih), pf);
       //for(nIndex = nWidth*nHeight; nIndex--; )
       for (nIndex = 0; nIndex < nWidth * nHeight; nIndex++)
       {
               nColor_RGB8888 = pBuffer[nIndex];
               nR = (unsigned char) (((nColor_RGB8888) & 0xFF000000) >> 24);
               nG = (unsigned char) (((nColor_RGB8888) & 0x00FF0000) >> 16);
               nB = (unsigned char) (((nColor_RGB8888) & 0x0000FF00) >> 8);
               fputc(nB, pf); /* write blue */
               fputc(nG, pf); /* write green */
               fputc(nR, pf);   /* write red */
       }
       fclose(pf);
}
//==========================================================================
 最后肯定不要忘了在适当的地方进行eglSwapBuffers(glesDisplay, glesSurface);
 备注：OPENGL ES V1。1的纹理图片必须是2的N次幂，要不然贴不上去的！！！听说OPENGL ES V2。0没这个要求但我没有试过。
