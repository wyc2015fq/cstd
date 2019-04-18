# VC数字图像处理编程讲座之三 - 深之JohnChen的专栏 - CSDN博客

2005年11月25日 13:06:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4603标签：[图像处理																[编程																[扩展																[byte																[float																[signal](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


VC数字图像处理编程讲座之三

                                                                                cndg

图像的基本概念

1.采样是指将在空间上连续的图像转换成离散的采样点（即像素）集的操作。由于图像是二维分布的信息，所以采样是在x轴和y轴两个方向上进行。一般情况下，x轴方向与y轴方向的采样间隔相同。

★采样分辨率是指映射到图像平面上的单个像素的景物元素的尺寸。



单位：像素/英寸，像素/厘米



（如：扫描仪的指标 300dpi）



2.量化是将各个像素所含的明暗信息离散化后，用数字来表示。一般的量化值为整数。充分考虑到人眼的识别能力之后，目前非特殊用途的图像均为8bit量化，即用[0 255]描述“从黑到白”。



3．数字图像的描述



所谓的数字图像的描述是指如何用一个数值方式来表示一个图像。因为矩阵是二维的，所以可以用矩阵来描述数字图像。同时，前面我们已经提到，量化值是整数，因此描述数字图像的矩阵一定是整数阵。



4．图像的分类：



4.1黑白图像



黑白图像是指图像的每个像素只能是黑或者白，没有中间的过渡，故又称为２值图像。2值图像的像素值为0、1。




4.2灰度图像

灰度图像是指每个像素的信息由一个量化的灰度级来描述的图像，没有彩色信息。

4.3彩色图像

彩色图像是指每个像素的信息由RGB三原色构成的图像，其中RBG是由不同的灰度级来描述的。彩色图像不能用一个矩阵来描述了，一般是用三个矩阵同时来描述。

5灰度直方图



在数字图像处理中，灰度直方图是最简单且最有用的工具，可以说，对图像的分析与观察，直到形成一个有效的处理方法，都离不开直方图。灰度直方图统计(histogram)有时我们需要知道一幅图中的灰度分布情况，这时就可以采用灰度直方图来表示，图中的横坐标表示灰度值，纵坐标表示该灰度值出现的次数（频率）。下图的灰度直方图，可见，低灰度的像素占了绝大部分。

6. 灰度直方图均衡化(histogram equalization)

在介绍灰度直方图均衡化之前，先讲讲直方图修正。所谓直方图修正，就是通过一个灰度映射函数Gnew=F(Gold)，将原灰度直方图改造成你所希望的直方图。所以，直方图修正的关键就是灰度映射函数。我们刚才介绍的阈值化，削波，灰度窗口变换等等，都是灰度映射函数。

直方图均衡化是一种最常用的直方图修正。它是把给定图象的直方图分布改造成均匀直方图分布。由信息学的理论来解释，具有最大熵（信息量）的图象为均衡化图象。直观地讲，直方图均衡化导致图象的对比度增加。

void CDibView::OnZft()//灰度直方图
{
int iWidth=300,iHeight=150; //表示图象的高度，宽度



BYTE*lpData = newBYTE[iWidth*iHeight];



Floatnum[256],p[256];



Memset(num,0,sizeof(float)*256);



Memset(p,0,sizeof(float)*256);



//统计灰度（0-255）出现次数



for(i=0;i< iHeight-1;I++)
for(j=0;j< iWidth ;J++)
{
num[lpData[iWidth *i+j]}++;
}



//统计灰度出现的概率



for(i=0;i<256;i++)
{
p[i]=num[i]/(iWidth*iHeight);
}



// 绘制直方图，在0-255灰度上对应的有多少值



…………….



……………..
}







void CDibView::OnZftJh()//灰度直方图均衡化在VC中实现
{
CClientDC pDC(this);



HDC hDC=pDC.GetSafeHdc();//获取当前设备上下文的句柄；
SetStretchBltMode(hDC,COLORONCOLOR);
CDibDoc *pDoc=GetDocument();
HDIB hdib;
hdib=pDoc->GetHDIB();
BITMAPINFOHEADER *lpDIBHdr;//位图信息头结构指针；
BYTE *lpDIBBits;//指向位图像素灰度值的指针；
lpDIBHdr=( BITMAPINFOHEADER *)GlobalLock(hdib);//得到图像的位图头信息
lpDIBBits=(BYTE*)lpDIBHdr+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
file://获取图像像素值
float p[256],p1[256],num[256];
int i,j,k;
for(i=0;i<256;i++)//清空三个数组;
{ num[i]=0.0f;
p[i]=0.0f;
p1[i]=0.0f;
}
file://num[]存放图象各个灰度级出现的次数;
int Height=lpDIBHdr->biHeight;
int Width=lpDIBHdr->biWidth;
for(i=0;i<HEIGHT;I++)
for(j=30;j<WIDTH-20;J++)
{
num[*(lpDIBBits+WIDTHBYTES(Width*8)*i+j)]++;
}
file://p[]存放图像各个灰度级的出现概率；
for(i=0;i<256;i++)
{
p[i]=num[i]/(Width*Height);
}
file://p1[]存放各个灰度级之前的概率和，用于直方图变换；
for(i=0;i<256;i++)
{ 
for(k=0;k<=i;k++)
p1[i]+=p[k];
}
file://直方图变换；
for(i=0;i<HEIGHT;I++)
for(j=30;j<WIDTH-20;J++)
{ *(lpDIBBits+WIDTHBYTES(Width*8)*i+j)=(BYTE)(p1[*(lpDIBBits+WIDTHBYTES(Width*8)*i+j)]*255+0.5);
}
StretchDIBits (hDC,0,0,lpDIBHdr->biWidth,lpDIBHdr->biHeight,0,0,
lpDIBHdr->biWidth,lpDIBHdr->biHeight,
lpDIBBits,(LPBITMAPINFO)lpDIBHdr,
DIB_RGB_COLORS,
SRCCOPY);//显示图像；
}




7.彩色图转灰度图(color to grayscale) 



我们在第二讲时提到了YUV的颜色表示方法，知道在这种表示方法中，Y分量的物理含义就是亮度，它含了灰度图的所有信息，只用Y分量就完全能够表示出一幅灰度图来。YUV和RGB之间有着如下的对应关系

我们利用上式，根据R，G，B的值求出Y值后，将R，G，B值都赋值成Y，就能表示出灰度图来，这就是彩色图转灰度图的原理。

先看看真彩图。我们知道真彩图不带调色板，每个像素用3个字节，表示R，G，B三个分量。所以处理很简单，根据R，G，B的值求出Y值后，将R，G，B值都赋值成Y，写入新图即可。再来看看带调色板的彩色图，我们知道位图中的数据只是对应调色板中的一个索引值，我们只需要将调色板中的彩色变成灰度，形成新调色板，而位图数据不用动，就可以了。

8.真彩图转256色图(true color to 256 indexed color) 



我们知道，真彩图中包含最多达2的24次方种颜色，怎样从中选出256种颜色，又要使颜色的失真比较小，这是一个比较复杂的问题。一种简单的做法是将R：G：B以3：3：2表示，即取R，G的高3位，B的高两位，组成一个字节，这样就可以表示256种颜色了，但不难想象，这种方法的失真肯定很严重。我们下面介绍的算法能够比较好的实现真彩到256色的转换。

它的思想是：准备一个长度为4096的数组，代表4096种颜色。对图中的每一个像素，取R，G，B的最高四位，拼成一个12位的整数，对应的数组元素加1。全部统计完后，就得到了这4096种颜色的使用频率。这其中，可能有一些颜色一次也没用到，即对应的数组元素为零（假设不为零的数组元素共有PalCounts个）。将这些为零的数组元素清除出去，使得前PalCounts个元素都不为零。将这PalCounts个数按从大到小的顺序排列（这里我们使用起泡排序），这样，前256种颜色就是用的最多的颜色，它们将作为调色板上的256种颜色。对于剩下的PalCounts-256种颜色并不是简单的丢弃，而是用前256种颜色中的一种来代替，代替的原则是找有最小平方误差的那个。再次对图中的每一个像素，取R，G，B的最高四位，拼成一个12位的整数，如果对应值在前256种颜色中，则直接将该索引值填入位图数据中，如果是在后PalCounts-256种颜色中，则用代替色的索引值填入位图数据中。

9.对比度扩展(contrast stretching)



假设有一幅图，由于成象时光照不足，使得整幅图偏暗，（例如，灰度范围从0到63）；或者成象时光照过强，使得整幅图偏亮，（例如，灰度范围从200到255），我们称这些情况为低对比度，即灰度都挤在一起，没有拉开。灰度扩展的意思就是把你所感兴趣的灰度范围拉开，使得该范围内的像素，亮的越亮，暗的越暗，从而达到了增强对比度的目的。我们可以用如下的图来说明对比度扩展的原理。

图中的横坐标gold表示原图的灰度值，纵坐标gnew表示gold经过对比度扩展后得到了新的灰度值。a,b,c为三段直线的斜率，因为是对比度扩展，所以斜率b>1。g1old和g2old表示原图中要进行对比度扩展的范围，g1new和g2new表示对应的新值。用公式表示为

a*gold当0<=gold<g1old时

gnew=b*(gold-g1old)+g1new当g1old<=gold<g2old时

c*(gold-g2old)+g2new当g2old<gold<=255时

显然要得到对比度扩展后的灰度，我们需要知道a,b,c,g1old,g2old五个参数。由于有新图的灰度级别也是255这个约束，所以满足a*g1old+b*(g2old-g1old)+c(255-g2old)=255这个方程。这样，我们只需给出四个参数，而另一个可以可以代入方程求得。我们假设a=c，这样，我们只要给出b，g1old和g2old，就可以求出

a=(255-b*(g2old-g1old))/(255-(g2old-g1old)) 

要注意的是，给出的三个参数必须满足1. b*(g2old-g1old)<=255；2. (g2old-g1old)<=255

这两点是显然的。




注：关于彩色图像的直方图，就是把彩色图象中的RGB（红绿蓝）分别画出直方图。

RGB (red green blue): 红绿蓝三基色
CMYK (Cyan-Magenta-Yellow-black inK): 青色-品红-黄色-黑色
HSI (Hue-Saturation-Intensity): 色调-饱和度-强度
DDB (device-dependent bitmap): 设备相关位图
DIB (device-independent bitmap): 设备无关位图
CVBS (Composite Video Broadcast Signal): 复合电视广播信号

YUV（亦称Y Cr Cb）是被欧洲电视系统所采用的一种颜色编码方法（属于PAL制）。YUV主要用于优化彩色视频信号的传输，使其向后兼容老式黑白电视。与RGB视频信号传输相比，它最大的优点在于只需占用极少的带宽（RGB要求三个独立的视频信号同时传输）。其中"Y"表示明亮度（Luminance或Luma），也就是灰阶值；而"U"和"V"表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。"亮度"是通过R GB输入信号来创建的，方法是将RGB信号的特定部分叠加到一起。"色度"则定义了颜色的两个方面-色调与饱和度，分别用Cr和CB来表示。其中，C r反映了GB输入信号红色部分与RGB信号亮度值之间的差异。而CB反映的是RGB输入信号蓝色部分与RGB信号亮度值之同的差异。




// RGB与HIS转换代码



voidRGB2HSI(intr,intg,intb,int &h,int &s,int &i)



{



m_fr=r;



m_fg=g;



m_fb=b;



m_fii=(m_fr+m_fg+m_fb)/3;//计算Intensity







if((m_fr+m_fg+m_fb)==0)



m_fss=0;



else



m_fss=1-(3*(min(min(m_fr,m_fg),m_fb))/(m_fr+m_fg+m_fb+0.000001));//计算Saturation







if((((m_fr-m_fg)*(m_fr-m_fg))+((m_fr-m_fb)*(m_fg-m_fb)))==0)



m_fhh=0;



else{



m_ftemp=((m_fr-m_fg)+(m_fr-m_fb))/(0.000001+2*sqrt(((m_fr-m_fg)*(m_fr-m_fg))+((m_fr-m_fb)*(m_fg-m_fb))));



if(m_fg>=m_fb)



m_fhh=acos(m_ftemp);//计算Hue



//hh=acos(temp)*180/pi;//计算Hue



else



m_fhh=2*PI-acos(m_ftemp);//计算Hue



//hh=(2*pi-acos(temp))*180/pi;//计算Hue 



}



h=int((m_fhh/(3.1415926*2))*240);



s=int(m_fss*240);



i=int(240*m_fii/255);



}



























我们利用上式，根据R，G，B的值求出Y值后，将R，G，B值都赋值成Y，就能表示出灰度图来，这就是彩色图转灰度图的原理。

先看看真彩图。我们知道真彩图不带调色板，每个像素用3个字节，表示R，G，B三个分量。所以处理很简单，根据R，G，B的值求出Y值后，将R，G，B值都赋值成Y，写入新图即可。再来看看带调色板的彩色图，我们知道位图中的数据只是对应调色板中的一个索引值，我们只需要将调色板中的彩色变成灰度，形成新调色板，而位图数据不用动，就可以了。

8.真彩图转256色图(true color to 256 indexed color) 



我们知道，真彩图中包含最多达2的24次方种颜色，怎样从中选出256种颜色，又要使颜色的失真比较小，这是一个比较复杂的问题。一种简单的做法是将R：G：B以3：3：2表示，即取R，G的高3位，B的高两位，组成一个字节，这样就可以表示256种颜色了，但不难想象，这种方法的失真肯定很严重。我们下面介绍的算法能够比较好的实现真彩到256色的转换。

它的思想是：准备一个长度为4096的数组，代表4096种颜色。对图中的每一个像素，取R，G，B的最高四位，拼成一个12位的整数，对应的数组元素加1。全部统计完后，就得到了这4096种颜色的使用频率。这其中，可能有一些颜色一次也没用到，即对应的数组元素为零（假设不为零的数组元素共有PalCounts个）。将这些为零的数组元素清除出去，使得前PalCounts个元素都不为零。将这PalCounts个数按从大到小的顺序排列（这里我们使用起泡排序），这样，前256种颜色就是用的最多的颜色，它们将作为调色板上的256种颜色。对于剩下的PalCounts-256种颜色并不是简单的丢弃，而是用前256种颜色中的一种来代替，代替的原则是找有最小平方误差的那个。再次对图中的每一个像素，取R，G，B的最高四位，拼成一个12位的整数，如果对应值在前256种颜色中，则直接将该索引值填入位图数据中，如果是在后PalCounts-256种颜色中，则用代替色的索引值填入位图数据中。

9.对比度扩展(contrast stretching)



假设有一幅图，由于成象时光照不足，使得整幅图偏暗，（例如，灰度范围从0到63）；或者成象时光照过强，使得整幅图偏亮，（例如，灰度范围从200到255），我们称这些情况为低对比度，即灰度都挤在一起，没有拉开。灰度扩展的意思就是把你所感兴趣的灰度范围拉开，使得该范围内的像素，亮的越亮，暗的越暗，从而达到了增强对比度的目的。我们可以用如下的图来说明对比度扩展的原理。

图中的横坐标gold表示原图的灰度值，纵坐标gnew表示gold经过对比度扩展后得到了新的灰度值。a,b,c为三段直线的斜率，因为是对比度扩展，所以斜率b>1。g1old和g2old表示原图中要进行对比度扩展的范围，g1new和g2new表示对应的新值。用公式表示为

a*gold当0<=gold<g1old时

gnew=b*(gold-g1old)+g1new当g1old<=gold<g2old时

c*(gold-g2old)+g2new当g2old<gold<=255时

显然要得到对比度扩展后的灰度，我们需要知道a,b,c,g1old,g2old五个参数。由于有新图的灰度级别也是255这个约束，所以满足a*g1old+b*(g2old-g1old)+c(255-g2old)=255这个方程。这样，我们只需给出四个参数，而另一个可以可以代入方程求得。我们假设a=c，这样，我们只要给出b，g1old和g2old，就可以求出

a=(255-b*(g2old-g1old))/(255-(g2old-g1old)) 

要注意的是，给出的三个参数必须满足1. b*(g2old-g1old)<=255；2. (g2old-g1old)<=255

这两点是显然的。




注：关于彩色图像的直方图，就是把彩色图象中的RGB（红绿蓝）分别画出直方图。

RGB (red green blue): 红绿蓝三基色
CMYK (Cyan-Magenta-Yellow-black inK): 青色-品红-黄色-黑色
HSI (Hue-Saturation-Intensity): 色调-饱和度-强度
DDB (device-dependent bitmap): 设备相关位图
DIB (device-independent bitmap): 设备无关位图
CVBS (Composite Video Broadcast Signal): 复合电视广播信号

YUV（亦称Y Cr Cb）是被欧洲电视系统所采用的一种颜色编码方法（属于PAL制）。YUV主要用于优化彩色视频信号的传输，使其向后兼容老式黑白电视。与RGB视频信号传输相比，它最大的优点在于只需占用极少的带宽（RGB要求三个独立的视频信号同时传输）。其中"Y"表示明亮度（Luminance或Luma），也就是灰阶值；而"U"和"V"表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。"亮度"是通过R GB输入信号来创建的，方法是将RGB信号的特定部分叠加到一起。"色度"则定义了颜色的两个方面-色调与饱和度，分别用Cr和CB来表示。其中，C r反映了GB输入信号红色部分与RGB信号亮度值之间的差异。而CB反映的是RGB输入信号蓝色部分与RGB信号亮度值之同的差异。




// RGB与HIS转换代码



voidRGB2HSI(intr,intg,intb,int &h,int &s,int &i)



{



m_fr=r;



m_fg=g;



m_fb=b;



m_fii=(m_fr+m_fg+m_fb)/3;//计算Intensity







if((m_fr+m_fg+m_fb)==0)



m_fss=0;



else



m_fss=1-(3*(min(min(m_fr,m_fg),m_fb))/(m_fr+m_fg+m_fb+0.000001));//计算Saturation







if((((m_fr-m_fg)*(m_fr-m_fg))+((m_fr-m_fb)*(m_fg-m_fb)))==0)



m_fhh=0;



else{



m_ftemp=((m_fr-m_fg)+(m_fr-m_fb))/(0.000001+2*sqrt(((m_fr-m_fg)*(m_fr-m_fg))+((m_fr-m_fb)*(m_fg-m_fb))));



if(m_fg>=m_fb)



m_fhh=acos(m_ftemp);//计算Hue



//hh=acos(temp)*180/pi;//计算Hue



else



m_fhh=2*PI-acos(m_ftemp);//计算Hue



//hh=(2*pi-acos(temp))*180/pi;//计算Hue 



}



h=int((m_fhh/(3.1415926*2))*240);



s=int(m_fss*240);



i=int(240*m_fii/255);



}







































