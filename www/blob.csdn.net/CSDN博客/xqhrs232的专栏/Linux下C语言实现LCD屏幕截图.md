# Linux下C语言实现LCD屏幕截图 - xqhrs232的专栏 - CSDN博客
2016年05月18日 11:38:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：866
原文地址::[http://www.360doc.com/content/13/0114/19/1317564_260166733.shtml](http://www.360doc.com/content/13/0114/19/1317564_260166733.shtml)
相关文章
1、Linux下jpeglib库的安装详解----[http://blog.sina.com.cn/s/blog_aafd61a201018qwi.html](http://blog.sina.com.cn/s/blog_aafd61a201018qwi.html)
一、概述
        最近看到网上有人问怎么用C语言实现屏幕截图，刚好自己也在研究Linux驱动，于是花了半天时间把Linux的FrameBuffer驱动看懂了个七八，接着就动手写了个LCD屏幕截图的应用程序。
     一些说明：
     1. 板子LCD的分辨率为480x272，像素格式为RGB565。
     2. 程序首先获取LCD的一些参数 ，然后将获取到的一帧数据转换为RGB24格式，最后用jpeglib库压缩为jpg格式的图片。
　  3. 在嵌入式Linux的板子上测试通过。

二、实现
       程序中有注释，不多说，直接上程序：
  1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<sys/ioctl.h>
  4 #include<unistd.h>
  5 #include<fcntl.h>
  6 #include <malloc.h>
  7 
  8 #include<linux/fb.h>
  9 
 10 #include <jpeglib.h>
 11 #include <jerror.h>
 12 
 13 
 14 /*RGB565转RGB24函数
 15 *@rgb565: 指向存放rgb565数据的起始地址
 16 *@rgb24：指向存放rgb24数据的起始地址
 17 *@width：屏幕（分辨率）的宽度
 18 *@height：屏幕（分辨率）的高度
 19 */
 20 int RGB565_to_RGB24(unsigned char *rgb565,unsigned char *rgb24,int width,int height)
 21 {
 22     int i;
 23     int whole = width*height;
 24     unsigned char r,g,b;
 25     unsigned short int *pix565;
 26     
 27     pix565 = (unsigned short int *)rgb565;
 28 
 29     for(i = 0;i < whole;i++)
 30     {    
 31         r = ((*pix565)>>11)&0x1f;
 32         *rgb24 = (r<<3) | (r>>2);
 33         rgb24++;
 34         g = ((*pix565)>>5)&0x3f;
 35         *rgb24 = (g<<2) | (g>>4);
 36         rgb24++;
 37         b = (*pix565)&0x1f;
 38         *rgb24 = (b<<3) | (b>>2);
 39         rgb24++;
 40         pix565++;    
 41             
 42     }
 43     return 1;
 44 }
 45 
 46 /*jpeg压缩函数
 47 *@rgb：指向存放rgb24数据的起始地址
 48 *@width：屏幕（分辨率）的宽度
 49 *@height：屏幕（分辨率）的高度
 50 */
 51 int jpeg_compress(unsigned char *rgb,int width,int height)
 52 {
 53         struct jpeg_compress_struct cinfo;
 54         struct jpeg_error_mgr jerr;
 55         FILE * outfile;
 56         JSAMPROW row_pointer[1];
 57         int row_stride;
 58         cinfo.err = jpeg_std_error(&jerr);
 59         jpeg_create_compress(&cinfo);
 60         //输出文件名为：out.jpg
 61         if ((outfile = fopen("out.jpg", "wb")) == NULL)
 62         {
 63             printf("can not open out.jpg\n");
 64             return -1;
 65         }
 66         jpeg_stdio_dest(&cinfo, outfile);
 67 
 68         cinfo.image_width = width;
 69         cinfo.image_height = height;
 70         cinfo.input_components = 3;
 71         //输入数据格式为RGB
 72         cinfo.in_color_space = JCS_RGB;
 73 
 74         jpeg_set_defaults(&cinfo);
 75         //压缩质量为80
 76         jpeg_set_quality(&cinfo, 80, TRUE );
 77         jpeg_start_compress(&cinfo, TRUE);
 78         row_stride = width * 3;
 79 
 80         while (cinfo.next_scanline < cinfo.image_height)
 81         {
 82             row_pointer[0] = &rgb[cinfo.next_scanline * row_stride];
 83             (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
 84         }
 85 
 86         jpeg_finish_compress(&cinfo);
 87         fclose(outfile);
 88 
 89         jpeg_destroy_compress(&cinfo);
 90 
 91         return 1;
 92 }
 93 
 94 
 95 int main()
 96 {
 97 
 98     int fd;
 99     struct fb_var_screeninfo fb_var_info;
100     struct fb_fix_screeninfo fb_fix_info;
101     unsigned char *trgb;
102     unsigned char *rgb;
103     int buffer_size;
104 
105     //打开framebuffer设备
106     fd = open("/dev/fb0",O_RDONLY);
107     if(fd < 0)
108     {
109         printf("can not open dev\n");
110         exit(1);
111     }
112 
113     //获取LCD的可变参数
114     ioctl(fd,FBIOGET_VSCREENINFO,&fb_var_info);
115     //一个像素多少位    
116     printf("bits_per_pixel: %d\n",fb_var_info.bits_per_pixel);
117     //x分辨率
118     printf("xres: %d\n",fb_var_info.xres);
119     //y分辨率
120     printf("yres: %d\n",fb_var_info.yres);
121     //r分量长度（bit）
122     printf("red_length: %d\n",fb_var_info.red.length);
123     //g分量长度（bit）
124     printf("green_length: %d\n",fb_var_info.green.length);
125     //b分量长度（bit）
126     printf("blue_length: %d\n",fb_var_info.blue.length);
127     //t(透明度)分量长度（bit）
128     printf("transp_length: %d\n",fb_var_info.transp.length);
129     //r分量偏移
130     printf("red_offset: %d\n",fb_var_info.red.offset);
131     //g分量偏移
132     printf("green_offset: %d\n",fb_var_info.green.offset);
133     //b分量偏移
134     printf("blue_offset: %d\n",fb_var_info.blue.offset);
135     //t分量偏移
136     printf("transp_offset: %d\n",fb_var_info.transp.offset);
137 
138     //获取LCD的固定参数
139     ioctl(fd,FBIOGET_FSCREENINFO,&fb_fix_info);
140     //一帧大小
141     printf("smem_len: %d\n",fb_fix_info.smem_len);
142     //一行大小
143     printf("line_length: %d\n",fb_fix_info.line_length);
144 
145     //一帧大小
146     buffer_size = (fb_var_info.xres * fb_var_info.yres * fb_var_info.bits_per_pixel/8);
147 
148     trgb = (unsigned char *)malloc(buffer_size);
149     if(trgb==NULL)
150         exit(0);
151     rgb = (unsigned char *)malloc(fb_var_info.xres * fb_var_info.yres * 3);
152     if(rgb==NULL)
153     {
154         goto here;
155     }
156     //获取一帧数据
157     if(read(fd,trgb,buffer_size) < 0)
158     {
159         printf("reaf failed!\n");
160         goto read_fail;
161     }
162     //格式转换
163     RGB565_to_RGB24(trgb,rgb,fb_var_info.xres,fb_var_info.yres);
164     //jpeg压缩
165     if(jpeg_compress(rgb,fb_var_info.xres,fb_var_info.yres)<0)
166         printf("compress failed!\n");        
167     
168 read_fail:
169     free(rgb);
170 here:
171     free(trgb);
172     
173 
174     close(fd);
175 
176     return 0;
177 }
