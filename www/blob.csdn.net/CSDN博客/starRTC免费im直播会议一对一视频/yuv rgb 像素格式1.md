# yuv rgb 像素格式1 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月18日 12:20:42[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：439
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
===========大小=============
一般，直接采集到的视频数据是RGB24的格式
RGB24一帧的大小size＝width×heigth×3 Byte，
RGB32的size＝width×heigth×4
I420（即YUV标准格式4：2：0）的数据量是 size＝width×heigth×1.5 Byte。
X264在进行编码的时候需要标准的YUV（4：2：0）
==============================
YV12虽然也是（4：2：0）但是YV12和I420的却是不同的，在存储空间上面有些区别：UV的顺序不同
YV12 ： 亮度（行×列） ＋
 V（行×列/4) + U（行×列/4）
I420 ： 亮度（行×列） ＋
U（行×列/4) + V（行×列/4）
I420: YYYYYYYY UU VV =>YUV420P
YV12: YYYYYYYY VV UU =>YUV420P
NV12: YYYYYYYY UVUV =>YUV420SP
NV21: YYYYYYYY VUVU =>YUV420SP
YV12和I420的区别：

yuv420p是I420格式，它的图如下所示：
其中前w*h Byte存储Y，接着的w*h*1/4 Byte存储U，最后w*h*1/4 Byte存储V
注意上图中的yuv颜色对应关系，4个Y对应一组uv
======================================
ffmpeg中像素格式名称后面有“P”的，代表是planar格式（如AV_PIX_FMT_YUV420P），否则就是packed格式。
AV_PIX_FMT_YUV420P,    ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) 
  = 0
AV_PIX_FMT_NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
AV_PIX_FMT_NV21,      ///< as above, but U and V bytes are swapped
注：
420p是先把U存放完后，再存放V，也就是说UV它们是连续的。而420sp它是UV、UV这样交替存放的。
**YUV420sp格式如下图                                                          **
                        YUV420p数据格式如下图
I420: YYYYYYYY UU VV =>YUV420P
YV12: YYYYYYYY VV UU =>YUV420P
NV12: YYYYYYYY UVUV =>YUV420SP
NV21: YYYYYYYY VUVU =>YUV420SP

AVPixFmtDescriptor是专门用于描述像素格式的结构体。通过av_pix_fmt_desc_get()可以获得它，然后再通过AVPixFmtDescriptor结构体可以获得不同像素格式的一些信息，如每个像素占用的比特数：
av_get_bits_per_pixel(av_pix_fmt_desc_get(src_pixfmt));   //上面bpp是不是bits per pixel?
所占空间 src_w*src_h*bpp/8


==========读写==========
读yuv
fread(pFrame->data[0],1,y_size,fp_in)<= 0||       // Y  
            fread(pFrame->data[1],1,y_size/4,fp_in)<= 0|| // U  
            fread(pFrame->data[2],1,y_size/4,fp_in)<= 0){ // V  
保存YUV420P
 fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),1,output);
fwrite(pFrameYUV->data[1],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
 fwrite(pFrameYUV->data[2],(pCodecCtx->width)*(pCodecCtx->height)/4,1,output);
保存UYVY
fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height),2,output);
保存RGB
 fwrite(pFrameYUV->data[0],(pCodecCtx->width)*(pCodecCtx->height)*3,1,output);  
==========================YUV，RGB像素数据处理========================
1，**分离YUV420P像素数据中的Y、U、V分量**
将YUV420P数据中的Y、U、V三个分量分离开来并保存成三个文件
- //Y
-         fwrite(pic,1,w*h,fp1);  
- //U
-         fwrite(pic+w*h,1,w*h/4,fp2);  
- //V
-         fwrite(pic+w*h*5/4,1,w*h/4,fp3); 
会把一张分辨率为256x256的文件分离成为三个文件：
纯Y数据，分辨率为256x256。纯U数据，分辨率为128x128。纯V数据，分辨率为128x128。
**2.分离YUV444P像素数据中的Y、U、V分量**
一帧YUV444P像素数据一共占用w*h*3
 Byte的数据。其中前w*h Byte存储Y，接着的w*h Byte存储U，最后w*h Byte存储V。
- fread(pic,1,w*h*3,fp);  //全部读出
- //Y
-         fwrite(pic,1,w*h,fp1);  
- //U
-         fwrite(pic+w*h,1,w*h,fp2);  
- //V
-         fwrite(pic+w*h*2,1,w*h,fp3);  
会把一张分辨率为256x256的文件分离成为三个文件：
纯Y数据，分辨率为256x256。
纯U数据，分辨率为256x256。
纯V数据，分辨率为256x256。

PSNR取值通常情况下都在20-50的范围内，取值越高，代表两张图像越接近，反映出受损图像质量越好。
3，**分离RGB24像素数据中的R、G、B分量**
与YUV420P三个分量分开存储不同，RGB24格式的每个像素的三个分量是连续存储的。一共占用w*h*3 Byte的存储空间。RGB24格式规定首先存储第一个像素的R、G、B，然后存储第二个像素的R、G、B…以此类推。这种存储方式称为Packed方式。
fread(pic,1,w*h*3,fp); 
- **for**(**int** j=0;j<w*h*3;j=j+3){  
- //R
-             fwrite(pic+j,1,1,fp1);  
- //G
-             fwrite(pic+j+1,1,1,fp2);  
- //B
-             fwrite(pic+j+2,1,1,fp3);  
-         } 

4，将RGB24格式像素数据转换为YUV420P格式像素数据
RGB到YUV的转换公式：
**Y= 0.299*R+0.587*G+0.114*B**
**U=-0.147*R-0.289*G+0.463*B**
**V= 0.615*R-0.515*G-0.100*B**

参考：
[http://blog.csdn.net/leixiaohua1020/article/details/50534150](http://blog.csdn.net/leixiaohua1020/article/details/50534150)
 yuv操作
[http://blog.csdn.net/leixiaohua1020/article/details/42134965](http://blog.csdn.net/leixiaohua1020/article/details/42134965)
 有各种yuv的读取与写入方法
[http://blog.csdn.net/leixiaohua1020/article/details/12234821](http://blog.csdn.net/leixiaohua1020/article/details/12234821)
[http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html](http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html)
