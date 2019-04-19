# libjpeg库编码图片为jpg(ffmpeg解码视频存储图片：RGB格式位图压缩为jpg格式图片) - 建建的博客 - CSDN博客
2017年07月19日 17:18:32[纪建](https://me.csdn.net/u013898698)阅读数：458
压缩流程（结合网上大家的资料以及libjpeg库的example.c示例：都大同小异）：
1、如何分配和初始化一个jpeg压缩对象：
     在libjpeg中jpeg压缩对象被定义为struct jpeg_compresss_struct这样一个结构体（在这个结构体中有一些子结构体，而这些子结构体是通过malloc来分配的）；                  另外我们还需要定义一个错误处理器struct jpeg_error_mgr结构体（错误处理器的作用是将jpeg库中的错误流输出到stderr，如果出现致命的错误就退出程序）。
例1：
         struct jpeg_compress_struct  cinfo;
         strcut jpeg_error_mgrjerr;
         cinfo.err  = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
2、指定压缩数据保存的位置：
例2：
         FILE *pOutfile =NULL;
         if(pOutfile =fopen(strFileName,”wb”))
         {
              return;
          }
          jpeg_stdio_dest(&cinfo,pOutfile);
不能在jpeg_start_compress与jpeg_finish_compress之间更改数据保存的位置。
3.       设置压缩对象参数，这里主要需要设置的参数有：
     cinfo.image_width;(图像的宽度，以像素为单位)；
     cinfo.image_height;(图像的高度，以像素为单位)；
     cinfo.input_components;(图像的索引，1代表灰阶图，2代表彩色图)；
     cinfo.in_color_space;(输入图像的色阶，JCS_GRAYSCALE代表灰阶，JCS_RGB代表彩色图像)；
     质量参数（0~100）；
/===========================
**第三个参数如果是灰度图，一定要设置成1，这个是貌似是通道数。对应的第四个参数一定要设置成JCS_GRAYSCALE。**
**如果是RGB图像，第三个参数设置成3，然后第四个参数设置成JCS_RGB。**
**再后面还有一句话**
**row_stride = jpegWidth ;**
**如果是灰度图，这条语句就没有问题，如果是RGB图，这个语句要改成**
**row_stride = jpegWidth*3 ;**
**彩色图像压缩，我一直没有实验成功，说明还有某些地方做的不正确。**
/===========================
     例3：
           jpeg_set_default(&cinfo)//设置压缩参数；
           jpeg_set_quality(&cinfo,100,TRUE)//设置质量参数；
4、开始压缩循环，逐行进行压缩：使用jpeg_start_compress开始一个压缩循环。
     例4：
           jpeg_start_compress(&cinfo,TRUE);
           JSAMPROW pRow[1];
           int iWidth = cinfo.image_width *cinfo. input_components;//一行数据所占的字节数
           while(cinfo.next_scanline <cinfo.image_height)
           {
              pRow[0] =pData[cinfo.next_scanline*iWidth];//需要压缩成jpeg图片的位  图数据缓冲区
              jpeg_write_scanlines(&cinfo,pRow,1);//参数：1表示写入一行
           }
5、结束压缩循环：
例5：
       jpeg_finish_compress(&cinfo);
6、释放jpeg压缩对象
           jpeg_destorycompress(&cinfo);
           fclose(pOutfile);
**[objc]**[view
 plain](http://blog.csdn.net/peng_apple/article/details/54632813#)[copy](http://blog.csdn.net/peng_apple/article/details/54632813#)
- 实例：（下面函数接口是RGB格式位图压缩为jpg格式图片：与ffmpeg解码视频结合）  
- //实现视频帧的jpeg压缩：首先ffmpeg解码视频为YUV420格式，先转码为RGB格式，这个过程在ffmpeg解码部分实现，这里不再详解。
- void MyWriteJPEG2(AVFrame* pFrame, int width, int height, int iFrame)  
- {  
- char fname[128] = { 0 };  
- 
- /* 分配和一个jpeg压缩对象 */
- struct jpeg_compress_struct cinfo;  
- struct jpeg_error_mgr jerr;  
-     JSAMPROW row_pointer[1];  
- int row_stride;  
- uint8_t *buffer;  
- FILEFILE *fp;  
- 
-     <span style="color:#ff0000;"><strong>buffer = pFrame->data[0];</strong>  
- </span>    sprintf(fname, "%s%d.jpg", "frame", iFrame);  
- 
- /* 指定压缩数据保存的位置 */
-     fp = fopen(fname, "wb");  
-     cinfo.err = jpeg_std_error(&jerr);  
-     jpeg_create_compress(&cinfo);/* 初始化 */
-     jpeg_stdio_dest(&cinfo, fp);  
- 
-     cinfo.image_width = width;  
-     cinfo.image_height = height;  
-     cinfo.input_components = 3;  
-     cinfo.in_color_space = JCS_RGB;  
- 
- /* 设置压缩参数 */
-     jpeg_set_defaults(&cinfo);  
- 
- /* 设置质量参数 */
-     jpeg_set_quality(&cinfo, 100, TRUE);  
- 
- /* 开始压缩循环，逐行进行压缩：使用jpeg_start_compress开始一个压缩循环 */
-     jpeg_start_compress(&cinfo, TRUE);  
- 
-     row_stride = width * 3;/* 一行数据所占字节数:如果图片为RGB，这个值要*3.灰度图像不用 */
- while (cinfo.next_scanline < height)  
-     {  
-         row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];//需要压缩成jpeg图片的位  图数据缓冲区
-         (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);//参数：1表示写入一行
-     }  
- 
-     jpeg_finish_compress(&cinfo);//结束压缩循环
-     fclose(fp);  
-     jpeg_destroy_compress(&cinfo);//释放jpeg压缩对象
- 
-     printf("compress one picture success.\n");  
- return;  
- }  
