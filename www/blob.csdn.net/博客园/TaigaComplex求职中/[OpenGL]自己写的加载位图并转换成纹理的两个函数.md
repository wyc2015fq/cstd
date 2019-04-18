# [OpenGL]自己写的加载位图并转换成纹理的两个函数 - TaigaComplex求职中 - 博客园







# [[OpenGL]自己写的加载位图并转换成纹理的两个函数](https://www.cnblogs.com/TaigaCon/archive/2012/06/24/2559640.html)





这学期初接触OpenGL，本来就是要跟图形学同时学的东西，不过本人太懒，并没有把OpenGL跟进上。虽然对于其中的算法算是有所了解，不过细节的东西估计要歇菜了，外加考试那天出了点事故，默哀一下我的图形学（不要挂，不要挂），题外话不多扯了，下面正题。

刚刚开始在配置OpenGL环境的时候，并没有把gluax.h和gluax.dll加入到环境中（本人用的是vs2010），后面到了要加载位图，用到函数auxDIBImageLoad（），这个函数是gluax.dll提供的，因此就想到自己动手写一个加载位图的函数。

首先我们来了解一下位图的格式，位图.BMP格式大体分为两种类型：

一个是位图里面的像素数据直接由RGB三原色组成，每个颜色分量为8bit，即一个像素24bit，这也就是所谓的24位真彩色图片

另一个是位图里面的像素数据由调色板索引组成，每个像素索引为8bit，即所谓的256色图片
|RGB|RGB|RGB|
|----|----|----|
|RGB|RGB|RGB|
|RGB|RGB|RGB|


|RGB（还有一个字节的保留区，共四个字节）|RGB|RGB|
|----|----|----|
|RGB|RGB|RGB|
|RGB|RGB|RGB|


|8bit索引|8bit索引|8bit索引|
|----|----|----|
|8bit索引|8bit索引|8bit索引|
|8bit索引|8bit索引|8bit索引|



可见，由于24位的BMP的像素数据为RGB三原色，是可以由像素数据加载到帧缓存中显示的，而8位的BMP则不行，它比需先由这8bit的索引值去调色板中才能得到RGB的真实值





接下来我们来了解一下位图的头，无论是8位的还是24位的BMP，都有固定格式的位图文件头和位图信息头：

位图文件头包含了图像类型、图像大小、图像数据存放地址和两个保留未使用的字段，共14个字节。

第0，1个字节为BM（BMP文件的固定开头）

第2，3，4，5个字节为该文件大小

第6，7，8，9个字节为保留字节，都是0

第10，11，12，13个字节为位图像素数据的开始地址



位图信息头包含了位图信息头的大小、图像的宽高、图像的色深、压缩说明图像数据的大小和其他一些参数，共40个字节。

第14 ，15，16，17个字节为0x28，即40，是位图信息头的大小

第18，19，20，21个字节是位图的宽度，以像素为单位

第22，23，24，25个字节是位图的高度

第26，27个字节为1（目标设备级别？）

第28，29个字节是位图的bit，即位图是多少位的

第30，31，32，33个字节用来表示位图是否经过压缩

第34，35，36，37个字节是位图像素数据的大小

第38，39，40，41水平像素

第42，43，44，45垂直像素

第46，47，48，49位图使用的调色板中的颜色数

第50，51，52，53位图使用的重要颜色数

（注：位图信息头的某些数据的数值并不会对位图本身造成影响，因此数据的填与不填并没硬性规定，这个估计要看生成位图的软件）



然后我们知道要使位图转换成纹理，需要的是包含位图RGB三分量的内存块，和位图的宽、高，然后就可以编写函数了

（要注意的是，无论是调色板还是位图的像素数据，位图存放RGB的顺序都是BGR）

以下为加载BMP

```
1 unsigned char* LoadBMP(FILE * img)
 2 {
 3     unsigned long bBMPFormat = 0;
 4     unsigned long bData = 0;
 5     DWORD size = 0;
 6     unsigned char *data;
 7 
 8     fseek(img,10,SEEK_SET);
 9     fread(&bData,4,1,img);
10     fseek(img,28,SEEK_SET);
11     fread(&bBMPFormat,2,1,img);
12     fseek(img,0,SEEK_END);
13     size = ftell(img) - bData;
14 
15     if(bBMPFormat==24)
16     {
17         data = (unsigned char*)malloc(size);
18         fseek(img,bData,SEEK_SET);
19         fread(data,size,1,img);
20     }
21     else if(bBMPFormat==8)
22     {
23         
24         data = (unsigned char*)malloc(size*3);
25         unsigned char* pdata = data;
26 
27         unsigned char *ColorBoard = (unsigned char*)malloc(1024);
28         fseek(img,54,SEEK_SET);
29         fread(ColorBoard,1024,1,img);
30 
31         unsigned char *ColorIndex = (unsigned char*)malloc(size);
32         fseek(img,1078,SEEK_SET);
33         fread(ColorIndex,size,1,img);
34 
35         for(int i=0;i<size;i++)
36         {
37             
38             *(pdata) = *(ColorBoard+(*ColorIndex)*4);
39             *(pdata+1) = *(ColorBoard+1+(*ColorIndex)*4);
40             *(pdata+2) = *(ColorBoard+2+(*ColorIndex)*4);
41             ColorIndex++;
42             pdata=pdata+3;
43         }
44     }
45 
46     return data;
47 }
```



以下为加载纹理

```
1 unsigned int LoadTex(char* Image)
 2 {
 3     unsigned int Texture=0;
 4 
 5                 FILE* img = NULL;
 6                img = fopen(Image,"rb");
 7 
 8     unsigned long bWidth = 0;    
 9     unsigned long bHeight = 0;  
10     unsigned long bdata = 0;   
11 
12     fseek(img,18L,SEEK_SET);
13     fread(&bWidth,4,1,img);
14     fread(&bHeight,4,1,img);
15 
16     unsigned char* data=LoadBMP(img);
17 
18     fclose(img);
19 
20     glGenTextures(1, &Texture);
21     glBindTexture(GL_TEXTURE_2D, Texture);
22     gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
23     
24 
25     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
26     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
27     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
28     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
29     //glTexImage2D(GL_TEXTURE_2D,0,3,bWidth,bHeight,0,GL_RGB,GL_UNSIGNED_BYTE,data);
30 
31     if (data)
32         free(data);
33     
34     return Texture;
35 }
```












