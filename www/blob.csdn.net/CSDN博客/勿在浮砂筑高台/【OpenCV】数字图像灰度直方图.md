# 【OpenCV】数字图像灰度直方图 - 勿在浮砂筑高台 - CSDN博客





2014年12月05日 16:56:57[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：882









灰度直方图是数字图像中最简单且有用的工具，这一篇主要总结OpenCV中直方图CvHistogram的结构和应用。

### 灰度直方图的定义


灰度直方图是灰度级的函数，描述图像中该灰度级的像素个数（或该灰度级像素出现的频率）：其横坐标是灰度级，纵坐标表示图像中该灰度级出现的个数（频率）。



一维直方图的结构表示为

![](https://img-my.csdn.net/uploads/201205/25/1337911570_7633.png)



高维直方图可以理解为图像在每个维度上灰度级分布的直方图。常见的是二维直方图。如红-蓝直方图的两个分量分别表示红光图像的灰度值和蓝光图像灰度值的函数。其图像坐标（Dr,Db）处对应在红光图像中具有灰度级Dr同时在蓝光图像中具有灰度级Db的像素个数。这是基于多光谱——每个像素有多个变量——的数字图像，二维中对应每个像素统计个变量。




### OpenCV中的直方图CvHistogram


注意我们在上面理解直方图的意义时更多把他想象成一幅**“图”**，继而理解图中横坐标，纵坐标的意义。而在OpenCV中，应该更多把直方图看做**“数据结构”**来理解。


OpenCV中用CvHistogram表示多维直方图（[http://www.opencv.org.cn/index.php/Cv%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86#CvHistogram](http://www.opencv.org.cn/index.php/Cv%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86#CvHistogram)）：




**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- typedefstruct CvHistogram  
- {  
- int     type;  
-     CvArr*  bins;  //存放每个灰度级数目的数组指针
- float   thresh[CV_MAX_DIM][2];  //均匀直方图
- float** thresh2; //非均匀直方图
-     CvMatND mat;  //直方图数组的内部数据结构
- }  
- CvHistogram;  


这个结构看起来简单（比IplImage*元素少多了。。。）其实并不太好理解。


第一个成员type用来指定第二个成员bins的类型。OpenCv中常见到CvArr*的接口，可以用以指定诸如CvMat、CvMatND、IplImage的类型，其实CvArr*的是一个指向void的指针。在函数内部有时需要得到确切的指向类型，这就需要type来指定。


thresh用来指定统计直方图分布的上下界。比如[0 255]表示用来统计图像中像素分别在灰度级[0 255]区间的分布情况，CV_MAX_DIM对应直方图的维数，假如设定二维红-蓝直方图的thresh为[0 255;100 200]，就是分别统计红色图像灰度级在[0 255]以及蓝色图像在灰度级[100 200]的分布情况。


thresh用以指定均匀直方图的分布，我们按每个像素理解自然是“均匀分布”，其实也可以统计像素在几个区间的分布。如果统计像素在2个区间的分布，则对应[0 255]的上下界，均匀分布统计的区间即[0 127] [127 255]分布的概率，这也是为什么thresh第二个维数默认为2——会自动均分上下界；而thresh2指定非均匀的分布，这就需要指定每个区间的上下界，如果要统计直方图在区间（0，10,100,255）的分布，那需要指定thresh2的一个维度为[0 10 100 255]，所以用float**形式表示。


mat简单说就是存储了直方图的信息，即我们统计的直方图分布概率。




### 创建直方图 cvCreateHist()


OpenCV中用cvCreateHist()创建一个直方图：




**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- CvHistogram* cvCreateHist(   
- int dims, //直方图维数 
- int* sizes,//直翻图维数尺寸
- int type, //直方图的表示格式
- float** ranges=NULL, //图中方块范围的数组
- int uniform=1 //归一化标识
-     );  


size数组的长度为dims，每个数表示分配给对应维数的bin的个数。如dims=3，则size中用[s1,s2,s3]分别指定每维bin的个数。

type有两种：CV_HIST_ARRAY 意味着直方图数据表示为多维密集数组 CvMatND; CV_HIST_TREE 意味着直方图数据表示为多维稀疏数组 CvSparseMat。



ranges就是那个复杂的不好理解的thresh的范围，他的内容取决于uniform的值。uniform为0是均匀的，非0时不均匀。



计算图像直方图的函数为CalcHist()：





**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- void cvCalcHist(   
-     IplImage** image, //输入图像（也可用CvMat**）
-     CvHistogram* hist, //直方图指针
- int accumulate=0, //累计标识。如果设置，则直方图在开始时不被清零。
- const CvArr* mask=NULL //操作 mask, 确定输入图像的哪个象素被计数
-     );  


要注意的是这个函数用来计算一张（或多张）**单通道**图像的直方图，如果要计算多通道，则用这个函数分别计算图像每个单通道。




### 实践：一维直方图


下面实践一下用OpenCV生成图像的一维直方图




**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- int main( )  
- {  
-     IplImage * src= cvLoadImage("baboon.jpg");  
-     IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);  
-     cvCvtColor(src,gray_plane,CV_BGR2GRAY);  
- 
- int hist_size = 256;    //直方图尺寸
- int hist_height = 256;  
- float range[] = {0,255};  //灰度级的范围
- float* ranges[]={range};  
- //创建一维直方图，统计图像在[0 255]像素的均匀分布
-     CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);  
- //计算灰度图像的一维直方图
-     cvCalcHist(&gray_plane,gray_hist,0,0);  
- //归一化直方图
-     cvNormalizeHist(gray_hist,1.0);  
- 
- int scale = 2;  
- //创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）
-     IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);  
-     cvZero(hist_image);  
- //统计直方图中的最大直方块
- float max_value = 0;  
-     cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);  
- 
- //分别将每个直方块的值绘制到图中
- for(int i=0;i<hist_size;i++)  
-     {  
- float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率
- int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度
-         cvRectangle(hist_image,  
-             cvPoint(i*scale,hist_height-1),  
-             cvPoint((i+1)*scale - 1, hist_height - intensity),  
-             CV_RGB(255,255,255));    
-     }  
-     cvNamedWindow( "GraySource", 1 );  
-     cvShowImage("GraySource",gray_plane);  
-     cvNamedWindow( "H-S Histogram", 1 );  
-     cvShowImage( "H-S Histogram", hist_image );  
- 
-     cvWaitKey(0);  
- }  


试验结果：


![](https://img-my.csdn.net/uploads/201205/25/1337914766_6961.png)



对应的，我们可以用一样的思路统计每个通道的直方图，并绘制图像每个通道像素的分布：

![](https://img-my.csdn.net/uploads/201205/25/1337914853_3708.png)






### 实践：二维直方图



我们也可以结合OpenCV的例子生成二维直方图：




**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- IplImage* r_plane  = cvCreateImage( cvGetSize(src), 8, 1 );  
-     IplImage* g_plane  = cvCreateImage( cvGetSize(src), 8, 1 );  
-     IplImage* b_plane  = cvCreateImage( cvGetSize(src), 8, 1 );  
-     IplImage* planes[] = { r_plane, g_plane };  
- //将HSV图像分离到不同的通道中
-     cvCvtPixToPlane( src, b_plane, g_plane, r_plane, 0 );  
- 
- // 生成二维直方图数据结构
- int r_bins =256, b_bins = 256;   
-     CvHistogram* hist;  
-     {  
- int    hist_size[] = { r_bins, b_bins };  
- float  r_ranges[]  = { 0, 255 };          // hue is [0,180]
- float  b_ranges[]  = { 0, 255 };   
- float* ranges[]    = { r_ranges,b_ranges };  
-         hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1);   
-     }  
- //计算一张或多张单通道图像image(s) 的直方图
-     cvCalcHist( planes, hist, 0, 0 );  


刚才的图我们是对应每个横坐标绘制纵坐标的直方块，二维的图需要绘制每个点：




**[cpp]**[view
 plain](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7600666#)



- for( int h = 0; h < r_bins; h++ ) {  
- for( int s = 0; s < b_bins; s++ ) {  
- float bin_val = cvQueryHistValue_2D( hist, h, s ); //查询直方块的值
- int intensity = cvRound( bin_val * 255 / max_value );  
-         cvRectangle( hist_img,   
-           cvPoint( h*scale, s*scale ),  
-           cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),  
-           CV_RGB(intensity,intensity,intensity),   
-           CV_FILLED);  
-     }  
- }  






最终生成二维直方图：

![](https://img-my.csdn.net/uploads/201205/25/1337915223_1211.png)











转载自：[http://blog.csdn.net/xiaowei_cqu](http://blog.csdn.net/xiaowei_cqu)






