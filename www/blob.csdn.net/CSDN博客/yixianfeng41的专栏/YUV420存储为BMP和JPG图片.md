# YUV420存储为BMP和JPG图片 - yixianfeng41的专栏 - CSDN博客
2016年08月11日 14:52:49[第2梦](https://me.csdn.net/yixianfeng41)阅读数：15016
##        网上大多数关于YUV420的资料都是关于YUV420P的，很少有YUV420SP的，因为YUV420SP的UV是交错存放的，处理起来相对麻烦点，但是YUV420SP也是一种常见格式，因此，在这里，我将关于YUV420SP格式数据的处理总结下，方便有需要的同志。
## 一、YUV420格式数据介绍
       YUV,分为三个分量，“Y”表示明亮度，也就是灰度值；“U"和”V"表示的则是色度，作用是描述影像色彩饱和度，用于指定像素的颜色。YUV主流的采样方式有三种：YUV4：4：4，YUV4：2：2，YUV4：2：0，这里主要介绍下YUV420。
        在YUV420中，一个像素点对应一个Y，一个2X2的小方块对应一个U和V。对于所有YUV420图像，它们的Y值排列是完全相同的，因为只有Y的图像就是灰度图像。YUV420又分为YUV420SP与YUV420P这两种，这两种格式的Y分布是相同的，区别在于UV：YUV420p它是先把U存放完后，再存放V，也就是说UV它们是连续的;而YUV420sp它是UV、UV这样交替存放的。（他们的内存分布图如下，左是YUV420sp,右是YUV420p）
![](https://img-blog.csdn.net/20160811125656000)![](https://img-blog.csdn.net/20160811125716688)
## 二、从YUV内存中取数据组方法
       注意YUV是每四个Y对应一个UV,并且YUV420P和YUV420SP的UV的存放格式不同，取法也不同，总的来说，YUV420P的取法简单，YUV420SP的取法相对复杂点。
### 1、YUV420SP
```cpp
for(int j=0;j<DataHeight;j++)   
{  
    for(int i=0;i<DataWidth;i++)  
    {  
        y=ybase[i + j * DataWidth];// 每四个y对应一个uv  
        u=ubase[j/2 * DataWidth+(i/2)*2];  
        v=ubase[j/2 * DataWidth+(i/2)*2+1];  //一定要注意是u+1      
    }  
}
```
### 2、YUV420P
```cpp
for(int j=0;j<DataHeight;j++)   
{  
    for(int i=0;i<DataWidth;i++)  
    {  
        //yyyyyy ... uuuu ...vvv
        y=ybase[i + j * DataWidth];
        u=ubase[j/2 * DataWidth/2+(i/2)];  
        v=vbase[j/2 * DataWidth/2+(i/2)];  
    }  
}
```
上面代码里，ybase就是YUV中Y的起始地址，ubase就是u的起始地址，vbase就是v的起始地址。而YUV420SP格式中，V就是U的地址加一；YUV420P中U和V都是连续的。按照上面方法，我们就可以得到每一组YUV数据，然后自己可以将每一组数据保存下来，再进行处理。
## 三、YUV420转换为RGB数据
### 1、转换公式
R=Y+1.4075*(V-128)
G=Y-0.3455*(U-128) – 0.7169*(V-128)
B=Y+1.779*(U-128)
### **2.转换方法**
#### YUV420SP
```cpp
for(int j=0;j<DataHeight;j++)   
{  
    for(int i=0;i<DataWidth;i++)  
    {  
        unsigned char r,g,b;  
        y=ybase[i + j * DataWidth];
        u=ubase[j/2 * DataWidth+(i/2)*2];  
        v=ubase[j/2 * DataWidth+(i/2)*2+1];      
        b=(unsigned char)(y+1.779*(u- 128));  
        g=(unsigned char)(y-0.7169*(v - 128)-0.3455*(u - 128));    
        r=(unsigned char)(y+ 1.4075*(v - 128));*/  
    }  
}
```
YUV420P
```cpp
for(int j=0;j<DataHeight;j++)   
{  
    for(int i=0;i<DataWidth;i++)  
    {  
        unsigned char r,g,b;  
      
        y=ybase[i + j * DataWidth];
        u=ubase[j/2 * DataWidth/2+(i/2)];  
        v=vbase[j/2 * DataWidth/2+(i/2)];  
        b=(unsigned char)(y+1.779*(u- 128));  
        g=(unsigned char)(y-0.7169*(v - 128)-0.3455*(u - 128));    
        r=(unsigned char)(y+ 1.4075*(v - 128));*/  
    }  
}
```
## 四、RGB数据存储为图片
注意rgb数据存储为bmp和jpg时的不同，将RBG数据存储为bmp时，数据是逆序存放，并且不是rgb,而是bgr；当将rgb数据存储为jpg时，则不用，不用逆序，数据也还是rgb。
### 1、存储为BMP图片
请看另一篇博客，[RGB TO BMP](http://blog.csdn.net/yixianfeng41/article/details/46712723)
### 2、存储为JPG图片
存储为JPG图片要用到一个开运库，libjpeg,或者libjpeg-turbo，我用的是libjpeg,网上关于这两个开源库的资料很多，可以从这个下载编译好的包，[LIBJPEG包](http://download.csdn.net/download/yixianfeng41/9580434)
封装的存储方法如下：
```cpp
int rgb2jpeg(const char * filename, unsigned char* rgbData,int image_width,int image_height,int quality)  
{  
    struct jpeg_compress_struct jpeg;  //identify a compress object
    struct jpeg_error_mgr jerr;  //error information
    jpeg.err = jpeg_std_error(&jerr);  
    jpeg_create_compress(&jpeg);  //init compress object
    FILE* pFile;
    fopen_s(&pFile,filename,"wb" );  
    if( !pFile )  return 0;  
    jpeg_stdio_dest(&jpeg, pFile);  
    //compress param set,i just did a simple param set
    jpeg.client_data=(void*)&pFile;
    jpeg.image_width = image_width;  
    jpeg.image_height = image_height;  
    jpeg.input_components  = 3;  
    jpeg.in_color_space = JCS_RGB;   
    jpeg_set_defaults(&jpeg);   
    //// 指定亮度及色度质量  
    jpeg.q_scale_factor[0] = jpeg_quality_scaling(100);  
    jpeg.q_scale_factor[1] = jpeg_quality_scaling(100);  
    //// 图像采样率，默认为2 * 2  
    jpeg.comp_info[0].v_samp_factor = 2;  
    jpeg.comp_info[0].h_samp_factor = 2;  
    //// set jpeg compress quality  
    jpeg_set_quality(&jpeg, quality, TRUE);  //100 is the highest
    //start compress
    jpeg_start_compress(&jpeg, TRUE);  
    JSAMPROW row_pointer[1];  
    //from up to down ,set every pixel
    for( unsigned int i=0;i<jpeg.image_height;i++ )  
    {  
        row_pointer[0] = rgbData+i*jpeg.image_width*3;  
        jpeg_write_scanlines( &jpeg,row_pointer,1 );  
    }  
    //stop compress
    jpeg_finish_compress(&jpeg);  
    fclose( pFile );  
    pFile = NULL;  
    jpeg_destroy_compress(&jpeg);  
    return 0;  
}
```
## 五、YUV数据存储为JPG
网上有不少关于YUV420数据存储为JPG的代码和博客，但是我用他们的代码，老是不成功，不是运行不起来，就是效果不好，不过还是表示万分感谢。
### 1、YUV420SP
```cpp
int yuv420p_to_jpeg(const char * filename, const char* pdata,int image_width,int image_height, int quality)
{   
    struct jpeg_compress_struct cinfo;  
    struct jpeg_error_mgr jerr;  
    cinfo.err = jpeg_std_error(&jerr);  
    jpeg_create_compress(&cinfo);  
    FILE * outfile;    // target file  
    if ((outfile = fopen(filename, "wb")) == NULL) {  
        fprintf(stderr, "can't open %s\n", filename);  
        exit(1);  
    }  
    jpeg_stdio_dest(&cinfo, outfile);  
    cinfo.image_width = image_width;  // image width and height, in pixels  
    cinfo.image_height = image_height;  
    cinfo.input_components = 3;    // # of color components per pixel  
    cinfo.in_color_space = JCS_YCbCr;  //colorspace of input image  
    jpeg_set_defaults(&cinfo);  
    jpeg_set_quality(&cinfo, quality, TRUE );  
    //////////////////////////////  
    //  cinfo.raw_data_in = TRUE;  
    cinfo.jpeg_color_space = JCS_YCbCr;  
    cinfo.comp_info[0].h_samp_factor = 2;  
    cinfo.comp_info[0].v_samp_factor = 2;  
    /////////////////////////  
    jpeg_start_compress(&cinfo, TRUE);  
    JSAMPROW row_pointer[1];
    unsigned char *yuvbuf;
    if((yuvbuf=(unsigned char *)malloc(image_width*3))!=NULL)
        memset(yuvbuf,0,image_width*3);
    unsigned char *ybase,*ubase;
    ybase=pdata;
    ubase=pdata+image_width*image_height;  
    int j=0;
    while (cinfo.next_scanline < cinfo.image_height) 
    {
        int idx=0;
        for(int i=0;i<image_width;i++)
        { 
            yuvbuf[idx++]=ybase[i + j * image_width];
            yuvbuf[idx++]=ubase[j/2 * image_width+(i/2)*2];
            yuvbuf[idx++]=ubase[j/2 * image_width+(i/2)*2+1];    
        }
        row_pointer[0] = yuvbuf;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
        j++;
    }
    jpeg_finish_compress(&cinfo);  
    jpeg_destroy_compress(&cinfo);  
    fclose(outfile);  
    return 0;  
}
```
### 2、YUV420P
其实YUV420P和YUV420SP主要区别就是取数据方式不同，前面对于YUV420P如何取数据已经讲得很清楚了，YUV420P存储为JPG只需要在上面YUV420SP存储为JPG的基础上改改取数据方法就好了。
### 效果：
![](https://img-blog.csdn.net/20160811141957094)
这是一张1280X720的图片，大小385kb,因为是USB摄像头，所以图片质量感觉不是很高，总的来说效果不错！
## 六、YUV420SP 与 YUV420P相互转换
       知道了YUV420SP以及YUV420P的内存格式后，互相转换就不是难事了。
### 1、YUV420SP TO YUV420P
```cpp
int yuv420sp_to_yuv420p(unsigned char * yuv420sp,unsigned char* yuv420p,int width,int height)
{
    if(yuv420sp==NULL)
        return;
    int i=0,j=0;
    //Y
    for(i=0;i<width*height;i++)
    {
        yuv420p[i]=yuv420sp[i];
    }
    //U
    for(int j=0,i=0;j<width*height/2;j+=2,i++)
    {
        yuv420p[i + width*height] = yuv420sp[j+width*height];
    }
    
    //V
    for(i=0,j=1,j<width*height/2;j+=2,i++)
    {
        yuv420p[i+width*height*5/4] = yuv420sp[j+width*height];
    }
}
```
### 2、YUV420P TO YUV420SP
```cpp
int yuv420p_to_yuv420sp(unsigned char * yuv420p,unsigned char* yuv420sp,int width,int height)
{
    if(yuv420p==NULL)
        return;
    int i=0,j=0;
    //Y
    for(i=0;i<width*height;i++)
    {
        yuv420sp[i]=yuv420p[i];
    }
    int m=0,n=0;
    for(int j=0;j<width*height/2;j++)
    {
        if(j%2==0)
           yuv420sp[j+width*height]=yuv420p[m++];
        else
           yuv420sp[j+width*height]=yuv420p[n++];
    }
}
```
## 七、参考博客
1、[图文详解YUV420数据格式](http://blog.csdn.net/jefry_xdz/article/details/7931018)
2[、使用libjpeg库将yuv420数据转换为jpg图片](http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=29798130&id=5465042)
3、[yuv420存储为JPG](http://blog.csdn.net/andrew57/article/details/45603063)
