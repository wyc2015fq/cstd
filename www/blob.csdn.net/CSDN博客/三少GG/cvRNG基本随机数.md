# cvRNG基本随机数 - 三少GG - CSDN博客
2011年04月09日 21:49:00[三少GG](https://me.csdn.net/scut1135)阅读数：3631
[http://hi.baidu.com/bingshanzhu/blog/item/fefccce7d817af3fb938208d.html](http://hi.baidu.com/bingshanzhu/blog/item/fefccce7d817af3fb938208d.html)
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"highgui200.lib")
#pragma comment(lib,"cvaux200.lib")
//随机数简单示例
int _tmain(int argc, _TCHAR* argv[])
{
    CvRNG rng;
    rng= cvRNG(cvGetTickCount());
    for (int i= 0;i<10;i++)
    {
        printf("%d/n",cvRandInt(&rng)%256);//如果%256出来的将会是0~255的正整数
        printf("%.2f/n",cvRandReal(&rng));
    }
    printf("Tick Frequency= %f/n",cvGetTickFrequency());
    system("pause");
    return 0;
}
**cvGetTickCount()****返回64位长整数的时间数据,在OpenCV是为CvRNG设置的专用种子**。
cvGetTickFrequency()
返回系统时钟频率
**cvRNG()**
**跟一般的C语言srand()使用方法一样，要先给它一个种子，但srand()用到的是unsigned int的32位种子范围，而cvRNG()用的是64位长整数种子。初始化CvRNG资料结构，假如seed给0，它将会自动转成-1。cvRNG(64位种子)**
**cvRandInt()**
**返回均匀分布32位的随机数，**均匀分布为统计学上的专有名词，表示长时间下所有数字出现的概率都是一样的，而cvRandint()在opencv里使用的公式
temp = (uint64)(unsigned)temp*1554115554 + (temp >> 32);
这个公式的名称叫Multiply-with-carry (MWC) generator，有兴趣的话可以在网络上找“随机数产生器”，Multiply-with-carry是将64位的种子去产生32位的随机数。
unsigned cvRandInt(CvRNG资料结构)
**cvRandReal()**
**返回均匀分布，0~1之间的随机小数**，cvRandReal()的公式则是用
cvRandInt(rng)*2.3283064365386962890625e-10
的方法,其实就是cvRandInt(rng)*2^(-32),也就是将cvRandInt()随机出来的结果(32位除以2的三十二次方,因此,出现的结果将会是0~1之间的小数,也就是随机0~1之间。
double cvRandReal(CvRNG资料结构)
 附录： [http://blog.csdn.net/superdont/archive/2010/09/27/5909730.aspx](http://blog.csdn.net/superdont/archive/2010/09/27/5909730.aspx)
图像添加噪声【OpenCV学习笔记1】
// Xoo.cpp : Defines the entry point for the console application.   
  
#include "cv.h"   
#include "highgui.h"   
int main( int argc, char** argv )   
{   
    // 结构中载入图像：图像也是BMP图像（cvLoadImage）或者其它格式   
    // XML/YAML (cvLoad)   
    CvImage img(argc > 1 ? argv[1] : "lena.jpg", 0, CV_LOAD_IMAGE_COLOR),   
        img_yuv, y, noise;   
    //  RNG   
    //初始化随机数生成器状态   
    //CvRNG cvRNG( int64 seed=-1 );   
    //seed   
    //64-bit 的值用来初始化一个随机序列   
    //函数 cvRNG 初始化随机数生成器并返回其状态。   
    //指向这个状态的指针可以传递给函数 cvRandInt,    
    //cvRandReal 和 cvRandArr .    
    //在通常的实现中使用一个 multiply-with-carry generator 。   
    CvRNG rng = cvRNG(-1);   
    if( !img.data() ) // 检查图像是否被载入   
        return -1;   
    img_yuv = img.clone(); // 克隆图像   
    // 色彩空间转换   
    //void cvCvtColor( const CvArr* src, CvArr* dst, int code );   
    //src==source   
    //dst=destination   
    //int code==转换的具体格式   
    cvCvtColor( img, img_yuv, CV_BGR2YCrCb );    
    // 创建图像   
    //CvImage::create   
    //void CvImage::create(CvSize size, int depth, int channels);   
    //创建一个影象。   
    //size   
    //图像大小   
    //depth   
    //像素深度   
    //channels   
    //通道数   
    y.create( img.size(), IPL_DEPTH_8U, 1 );    
    noise.create( img.size(), IPL_DEPTH_32F, 1 );   
    //cvSplit 函数，分解图像到单个色彩通道上，然后单独处理。   
//void cvSplit( const CvArr* src, CvArr* dst0, CvArr* dst1,   
//               CvArr* dst2, CvArr* dst3 );   
//#define cvCvtPixToPlane cvSplit   
//src   
//原数组.   
//dst0...dst3   
//目标通道   
//函数 cvSplit 分割多通道数组成分离的单通道数组d。   
//可获得两种操作模式 . 如果原数组有N通道且前N输出数组非NULL,    
//所有的通道都会被从原数组中提取，如果前N个通道只有一个通道非NULL函数只提取该指定通道，   
//否则会产生一个错误，余下的通道（超过前N个通道的以上的）必须被设置成NULL，   
//对于设置了COI的IplImage 结使用cvCopy 也可以从图像中提取单通道。   
    cvSplit( img_yuv, y, 0, 0, 0 );    
    // 正态分布的随机数组   
    //  用随机数填充数组并更新 RNG 状态   
    //void cvRandArr( CvRNG* rng, CvArr* arr, int dist_type, CvScalar param1, CvScalar param2 );   
    //rng   
    //被 cvRNG 初始化的 RNG 状态.   
    //arr   
    //输出数组   
    //dist_type   
    //分布类型:   
    //CV_RAND_UNI - 均匀分布   
    //CV_RAND_NORMAL - 正态分布 或者 高斯分布   
    //param1   
    //分布的第一个参数。如果是均匀分布它是随机数范围的闭下边界。如果是正态分布它是随机数的平均值。   
    //param2   
    //分布的第二个参数。如果是均匀分布它是随机数范围的开上边界。如果是正态分布它是随机数的标准差。   
    //函数 cvRandArr 用均匀分布的或者正态分布的随机数填充输出数组。   
    //在下面的例子中该函数被用来添加一些正态分布的浮点数到二维数组的随机位置。   
    cvRandArr( &rng, noise, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(20) );    
    //各种方法的图像平滑   
    //void cvSmooth( const CvArr* src, CvArr* dst,   
    //               int smoothtype=CV_GAUSSIAN,   
    //               int param1=3, int param2=0, double param3=0, double param4=0 );   
    //src   
    //输入图像.   
    //dst   
    //输出图像.   
    //smoothtype   
    //平滑方法:   
    //CV_BLUR_NO_SCALE (简单不带尺度变换的模糊) - 对每个象素的 param1×param2 领域求和。   
    //如果邻域大小是变化的，可以事先利用函数 cvIntegral 计算积分图像。   
    //CV_BLUR (simple blur) - 对每个象素param1×param2邻域 求和并做尺度变换 1/(param1•param2).   
    //CV_GAUSSIAN (gaussian blur) - 对图像进行核大小为 param1×param2 的高斯卷积   
    //CV_MEDIAN (median blur) - 对图像进行核大小为param1×param1 的中值滤波 (i.e. 邻域是方的).   
    //CV_BILATERAL (双向滤波) - 应用双向 3x3 滤波，彩色 sigma=param1，空间 sigma=param2. 关于双向滤波，   
    //可参考 http://www.dai.ed.ac.uk/CVonline/LOCAL_COPIES/MANDUCHI1/Bilateral_Filtering.html   
    //param1   
    //平滑操作的第一个参数.   
    //param2   
    //平滑操作的第二个参数. 对于简单/非尺度变换的高斯模糊的情况，如果param2的值 为零，则表示其被设定为param1。   
    //param3   
    //对应高斯参数的 Gaussian sigma (标准差). 如果为零，则标准差由下面的核尺寸计算：   
    //sigma = (n/2 - 1)*0.3 + 0.8, 其中 n=param1 对应水平核,   
    //                                 n=param2 对应垂直核.   
    //对小的卷积核 (3×3 to 7×7) 使用如上公式所示的标准 sigma 速度会快。如果 param3 不为零，   
    //而 param1 和 param2 为零，则核大小有 sigma 计算 (以保证足够精确的操作).   
    //函数 cvSmooth 可使用上面任何一种方法平滑图像。每一种方法都有自己的特点以及局限。   
    //没有缩放的图像平滑仅支持单通道图像，并且支持8位到16位的转换(与cvSobel和cvaplace相似)和32位浮点数到32位浮点数的变换格式。   
    //简单模糊和高斯模糊支持 1- 或 3-通道, 8-比特 和 32-比特 浮点图像。这两种方法可以（in-place）方式处理图像。   
    //中值和双向滤波工作于 1- 或 3-通道， 8-位图像，但是不能以 in-place 方式处理图像.   
    //   
    //中值滤波   
    //中值滤波法是一种非线性平滑技术，它将每一象素点的灰度值设置为该点某邻域窗口内的所有象素点灰度值的中值。实现方法：   
    //通过从图像中的某个采样窗口取出奇数个数据进行排序   
    //用排序后的中值取代要处理的数据即可   
    //中值滤波法对消除椒盐噪音非常有效，在光学测量条纹图象的相位分析处理方法中有特殊作用，但在条纹中心分析方法中作用不大。   
    //中值滤波在图像处理中,常用于用来保护边缘信息,是经典的平滑噪声的方法   
    //中值滤波原理   
    //中值滤波是基于排序统计理论的一种能有效抑制噪声的非线性信号处理技术，中值滤波的基本原理是把数字图像   
    //或数字序列中一点的值用该点的一个拎域中各点值的中值代替，让周围的像素值接近的值，从而消除孤立的噪声点。   
    //方法是去某种结构的二维滑动模板，将板内像素按照像素值的大小进行排序，生成单调上升（或下降）的为二维数据序列。   
    //二维中值滤波输出为g（x,y）=med{f(x-k,y-l),(k,l∈W)} ，其中,f(x,y)，g(x,y)分别为原始图像和处理后图像。   
    //W为二维模板，通常为2*2，3*3区域，也可以是不同的的形状，如线状，圆形，十字形，圆环形等。   
    //高斯滤波   
    //高斯滤波实质上是一种信号的滤波器，其用途是信号的平滑处理，我们知道数字图像用于后期应用，其噪声是最大的问题，   
    //由于误差会累计传递等原因，很多图像处理教材会在很早的时候介绍Gauss滤波器，用于得到信噪比SNR较高的图像（反应真实信号）。   
    //于此相关的有Gauss-Lapplace变换，其实就是为了得到较好的图像边缘，先对图像做Gauss平滑滤波，   
    //剔除噪声，然后求二阶导矢，用二阶导的过零点确定边缘，在计算时也是频域乘积=>空域卷积。   
    //滤波器就是建立的一个数学模型，通过这个模型来将图像数据进行能量转化，能量低的就排除掉，噪声就是属于低能量部分   
    //其实编程运算的话就是一个模板运算，拿图像的八连通区域来说，中间点的像素值就等于八连通区的像素值的均值，这样达到平滑的效果   
    //若使用理想滤波器，会在图像中产生振铃现象。采用高斯滤波器的话，系统函数是平滑的，避免了振铃现象。   
    cvSmooth( noise, noise, CV_GAUSSIAN, 5, 5, 1, 1 ); // GAUSSIAN滤波做平衡   
    //将帧叠加到累积器（accumulator）中   
    //void cvAcc( const CvArr* image, CvArr* sum, const CvArr* mask=NULL );   
    //image   
    //输入图像, 1- 或 3-通道, 8-比特或32-比特浮点数. (多通道的每一个通道都单独处理).   
    //sum   
    //同一个输入图像通道的累积，32-比特或64-比特浮点数数组.   
    //mask   
    //可选的运算 mask.   
    // noise = noise + y   
    cvAcc( y, noise );     
    //ConvertScale   
    //使用线性变换转换数组   
    //void cvConvertScale( const CvArr* src, CvArr* dst, double scale=1, double shift=0 );   
    //#define cvCvtScale cvConvertScale   
    //#define cvScale  cvConvertScale   
    //#define cvConvert( src, dst )  cvConvertScale( (src), (dst), 1, 0 )   
    //src   
    //输入数组.   
    //dst   
    //输出数组   
    //scale   
    //比例因子.   
    //shift   
    //该加数被加到输入数组元素按比例缩放后得到的元素上   
    //函数 cvConvertScale 有多个不同的目的因此就有多个同义函数（如上面的#define所示）。    
    //该函数首先对输入数组的元素进行比例缩放，然后将shift加到比例缩放后得到的各元素上，   
    //即： dst(I)=src(I)*scale + (shift,shift,...)，最后可选的类型转换将结果拷贝到输出数组。   
    //多通道的数组对各个通道是独立处理的。   
    //类型转换主要用舍入和溢出截断来完成。   
    //也就是如果缩放+转换后的结果值不能用输出数组元素类型值精确表达，   
    //就设置成在输出数组数据轴上最接近该数的值。   
    //如果 scale=1, shift=0 就不会进行比例缩放. 这是一个特殊的优化，   
    //相当于该函数的同义函数名：cvConvert 。如果原来数组和输出数组的类型相同，   
    //这是另一种特殊情形，可以被用于比例缩放和平移矩阵或图像，   
    //此时相当于该函数的同义函数名：cvScale。   
    cvConvert( noise, y ); // y = noise * 1 + 0   
    //Merge   
    //从几个单通道数组组合成多通道数组或插入一个单通道数组   
    //void cvMerge( const CvArr* src0, const CvArr* src1,   
    //              const CvArr* src2, const CvArr* src3, CvArr* dst );   
    //#define cvCvtPlaneToPix cvMerge   
    //src0... src3   
    //输入的通道.   
    //dst   
    //输出数组.   
    //函数cvMerge 是前一个函数的反向操作。   
    //如果输出数组有N个通道并且前N个输入通道非NULL，   
    //就拷贝所有通道到输出数组，如果在前N个通道中只有一个单通道非NULL ，   
    //只拷贝这个通道到输出数组，否则 就会产生错误。   
    //除前N通道以外的余下的通道必须置NULL。   
    //对于设置了COI的 IplImage结构使用 cvCopy也可以实现向图像中插入一个通道 。   
    cvMerge( y, 0, 0, 0, img_yuv );  // 图层合并   
    cvCvtColor( img_yuv, img, CV_YCrCb2BGR ); // 图像色彩空间转换   
    //cvNamedWindow   
    //   
    //创建窗口   
    //int cvNamedWindow( const char* name, int flags=CV_WINDOW_AUTOSIZE );   
    //name    
    //窗口的名字，它被用来区分不同的窗口，并被显示为窗口标题。   
    //flags    
    //窗口属性标志，为1时表示会根据图像自动调整窗口大小。   
    //目前唯一支持的标志是CV_WINDOW_AUTOSIZE。   
    //当这个标志被设置后，用户不能手动改变窗口大小，   
    //窗口大小会自动调整以适合被显示图像（参考cvShowImage）。   
    //函数cvNamedWindow创建一个可以放置图像和trackbar的窗口。   
    //被创建的窗口可以通过它们的名字被引用。   
    //如果已经存在这个名字的窗口，这个函数将不做任何事情。   
    cvNamedWindow( "image with grain", CV_WINDOW_AUTOSIZE );   
    //cvShowImage   
    //   
    //在指定窗口中显示图像   
    //void cvShowImage( const char* name, const CvArr* image );   
    //name   
    //窗口的名字。   
    //image   
    //被显示的图像。   
    //函数cvShowImage 在指定窗口中显示图像。   
    //如果窗口创建的时候被设定标志CV_WINDOW_AUTOSIZE，   
    //那么图像将以原始尺寸显示；否则，图像将被伸缩以适合窗口大小。   
    img.show( "image with grain" ); // cvShowImage的另外一种形式   
    //cvWaitKey   
    //   
    //等待按键事件   
    //int cvWaitKey( int delay=0 );   
    //delay    
    //延迟的毫秒数。   
    //函数cvWaitKey无限制的等待按键事件（delay<=0时）；或者延迟"delay"毫秒。   
    //返回值为被按键的值，如果超过指定时间则返回-1。   
    //注释：这个函数是HighGUI中唯一能够获取和操作事件的函数，   
    //所以在一般的事件处理中，它需要周期地被调用，   
    //除非HighGUI被用在某些能够处理事件的环境中。   
    /*译者注：比如在MFC环境下，这个函数不起作用。*/  
    cvWaitKey();   
    return 0;   
    // 所有图像自动释放，这是使用C++类比较方便的地方   
}  
本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/superdont/archive/2010/09/27/5909730.aspx
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
知道了上面的以后，我的需求是:  随机数可能重复. 因此希望使用一个随机数做种子用它来确定一组"无规律"的自然数序列, 并且在这个序列中不会出现重复的自然数. 即**随机数的产生不会出现重复.**
**方法1. 若重复，再次生成。**
逐个产生这些随机数，每产生一个，都跟前面的随机数比较，如果重复，就重新产生：
**这种算法越到后面，遇到已使用过的元素的可能性越高，重复次数就越多，但是当m较小时还是很好的：）**
**方法2. **
**算法四（1）**
**for (i=0;i<n;i++)**
**  a[i]=i+1;**
**for (i=n-1;i>0;i--)**
**{w=rand()%i;**
** t=a[i];**
** a[i]=a[w];**
** a[w]=t;**
**}**
**这个算法很不错，有人会怀疑其随机性，但个人认为是没问题的，首先第二行按顺序用0到n填满整个数组；第三行，是随机产生从0到n-2个数组下标，把这个下标的元素值跟n-1下标的元素值交换，一直进行到下标为1的元素。因此它只需要遍历一次就能产生全部的随机数。**
上述算法参考：c语言中的随机函数分析与生成m个不重复随机数算法比较[http://s99f.blog.163.com/blog/static/35118365200811196445340/](http://s99f.blog.163.com/blog/static/35118365200811196445340/)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
同样精彩的一篇文章：
[http://hi.baidu.com/darwinmarx/blog/item/2e41e9e9a2b24131b90e2d70.html](http://hi.baidu.com/darwinmarx/blog/item/2e41e9e9a2b24131b90e2d70.html)
CvRNG,RNG代表的意思就是Random Number Generation,隨機數產生,隨機的部份實際上用起來很簡單,但原理的部份就稍微難一些了,基本的函式為cvRNG(),初始化資料結構,cvRandInt(),32bits隨機及浮點數隨機的cvRandReal(),以下程式碼實作.
簡單的隨機程式製作1
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
**int** main()
{
     CvRNG rng;
     rng = cvRNG(cvGetTickCount());
**for**(**int** i=0;i<100;i++)
     {
         printf("%d/n",cvRandInt(`&`rng));
     }
     printf("The Tick Frequency is %f/n",cvGetTickFrequency());
     system("pause");
}
這邊用cvGetTickCount()當種子與C語言time()不同的是,它雖然同樣用時間產生種子,但是它用的是int64長整數型別,而time()則是要看機器而定32位元電腦用32bits,64位元電腦則用64bits.這個程式產生了100個有正有負隨機均勻分佈的整數,cvRandInt()產生的是32bit隨機的數,用int去接的話會介於-2147483648~2147483647之間,而用unsign int去接的話會是0~4294967295,而printf()裡"%d"出來的型別是int,如果用printf("%d/n",cvRandInt(`&`rng)%256)的話出來的將會是0~255的正整數.
改成cvRandInt(`&`rng)%256之後
簡單的隨機程式製作2
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
**int** main()
{
     CvRNG rng;
     rng = cvRNG(cvGetTickCount());
**for**(**int** i=0;i<100;i++)
     {
         printf("%.2f/n",cvRandReal(`&`rng));
     }
     system("pause");
}
執行結果:
這裡cvRandReal()是拿cvRandInt()的結果乘以二的負三十二次方,因此兩個數會被轉成double去做除法運算而變成浮點數隨機的均勻分佈,如果這邊將printf("%.2f/n",cvRandReal(`&`rng))改成printf("%d/n",cvRandReal(`&`rng))亦會出現一堆整數數字,但會因為變數格式的關係隨機的意義將會失焦,因此,這邊程式不該用"%d"來列印,這個部份就要參考計算機組織與設計的計算機算數的部份.
**cvGetTickCount()**
回傳長整數64bits的時間數據,在OpenCV是為CvRNG而設的的專用種子.
**cvGetTickFrequency()**
回傳系統時脈頻率.
**cvRNG()**
跟一般C語言的srand()使用方法一樣,要先給它一個種子,但srand()用到的是unsigned int的32bits的種子範圍,而cvRNG()用的是LONGLONG型態,64bits大小的長整數種子,LONGLONG這個變數型別是在windows.h底下宣告的,也可以用long long表示.這邊初始化CvRNG資料結構,假如seed給0的話,它將會自動轉成-1.
cvRNG(64bits種子的數字)
**cvRandInt()**
傳回均勻分佈(Uniform Distribution),32bits的隨機數,均勻分佈為統計學上的專有名詞.表示長時間下所有數字出現的機率都是一樣的,而cvRandInt()在OpenCV裡使用的公式是
temp = (uint64)(unsigned)temp*1554115554 + (temp >> 32);
這公式(演算法)的名稱叫做Multiply-with-carry (MWC) generator,有興趣的話可以在網路上找"隨機數產生器"(Random Number Generator)可以找到一些資料,Multiply-with-carry是將64bits的種子去產生32bits的隨機數
unsigned cvRandInt(CvRNG資料結構)
**cvRandReal()**
傳回均勻分佈,0~1之間的隨機小數.cvRandReal()的公式則是用
cvRandInt(rng)*2.3283064365386962890625e-10
的方法,其實就是cvRandInt(rng)*2^(-32),也就是將cvRandInt()隨機出來的結果(32bits)除以2的三十二次方,因此,出現的結果將會是0~1之間的小數,也就是機率0~1之間.
double cvRandReal(CvRNG資料結構)
