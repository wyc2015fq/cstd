# Opencv源代码分析HoughLines - qq_19833853的博客 - CSDN博客
2017年04月01日 18:33:25[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：631
一个有毅力的知识搬运工的第二天搬运：
霍夫变换是一种特征提取技术。经典的霍夫变换能够识别出图像中的直线，后来又发展到能够识别出任意形状，但更常见的识别形状是圆和椭圆。函数HoughLines的作用就是能够检测出边缘图像中的直线。
在直角坐标系下，直线被定义为：
*y *= *mx *+ *b*         （1）
其中，*m*为斜率，*b*为与*y*轴的截距，只要确定了*m*和*b*，一条直线就可以被唯一地确定下来。如果用*ρ*0表示原点到该直线的代数距离，*θ*0表示该直线的正交线与x轴的夹角，则：
![](https://img-blog.csdn.net/20151213110129053)（2）
![](https://img-blog.csdn.net/20151213110147094)  （3）
则该直线又可表示为：
![](https://img-blog.csdn.net/20151213110204624)   （4）
写成更一般的形式：
![](https://img-blog.csdn.net/20151213110215305)（5）
很容易想到，（*ρ*，*θ*）是极坐标的表示形式。但如果把（*ρ*，*θ*）也用直角坐标的形式表示，即把*ρ*和*θ*做正交处理，则（*ρ*，*θ*）就被称为霍夫空间。
在直角坐标系中的一点，对应于霍夫空间的一条正弦曲线。直线是由无数个点组成的，在霍夫空间就是无数条正弦曲线，但这些正弦曲线会相交于一点（*ρ*0，*θ*0），把该点带入公式2和公式3就得了直线的斜率和截距，这样一条直线就被确定了下来。因此在用霍夫变换识别直线时，霍夫空间中的极大值就有可能对应一条直线。
下面给出函数HoughLines的霍夫变换直线检测的步骤：
1、对边缘图像进行霍夫空间变换；
2、在4邻域内找到霍夫空间变换的极大值；
3、对这些极大值安装由大到小顺序进行排序，极大值越大，越有可能是直线；
4、输出直线。
函数HoughLines的原型为：
void HoughLines(InputArray image,OutputArray lines, double rho, double theta, int threshold, double srn=0,double stn=0 )
image为输入图像，要求是单通道的二值图像
lines为输出直线向量，两个元素的向量（*ρ*，*θ*）代表一条直线，*ρ*是从原点（图像的左上角）的距离，*θ*是直线的角度（单位是弧度），0表示垂直线，π/2表示水平线
rho为距离分辨率
theta为角度分辨率
threshold为阈值，在步骤2中，只有大于该值的点才有可能被当作极大值，即至少有多少条正弦曲线交于一点才被认为是直线
srn和stn在多尺度霍夫变换的时候才会使用，在这里我们只研究经典霍夫变换
HoughLines函数是在sources/modules/imgproc/src/hough.cpp文件中定义的：
- void cv::HoughLines( InputArray _image, OutputArray _lines,  
- double rho, double theta, int threshold,  
- double srn, double stn )  
- {  
- //申请一段内存，用于存储霍夫变换后所检测到的直线
-     Ptr<CvMemStorage> storage = cvCreateMemStorage(STORAGE_SIZE);  
- //提取输入图像矩阵
-     Mat image = _image.getMat();  
-     CvMat c_image = image;  
- //调用由C语音写出的霍夫变换的函数
-     CvSeq* seq = cvHoughLines2( &c_image, storage, srn == 0 && stn == 0 ?  
-                     CV_HOUGH_STANDARD : CV_HOUGH_MULTI_SCALE,  
-                     rho, theta, threshold, srn, stn );  
- //把由cvHoughLines2函数得到的霍夫变换直线序列转换为数组
-     seqToMat(seq, _lines);  
- }  
cvHoughLines2函数是霍夫变换直线检测的关键，函数的输出为所检测到的直线序列，它的第3个形参“srn == 0 && stn == 0 ? CV_HOUGH_STANDARD :CV_HOUGH_MULTI_SCALE”表示的是该霍夫变换是经典霍夫变换还是多尺度霍夫变换，它是由变量srn和stn决定的，只要这两个变量有一个不为0，就进行多尺度霍夫变换，否则为经典霍夫变换。另外cvHoughLines2函数不仅可以用于经典霍夫变换和多尺度霍夫变换，还可以用于概率霍夫变换
- CV_IMPL CvSeq*  
- cvHoughLines2( CvArr* src_image, void* lineStorage, int method,  
- double rho, double theta, int threshold,  
- double param1, double param2 )  
- {  
-     CvSeq* result = 0;  
- 
-     CvMat stub, *img = (CvMat*)src_image;  
-     CvMat* mat = 0;  
-     CvSeq* lines = 0;  
-     CvSeq lines_header;  
-     CvSeqBlock lines_block;  
- int lineType, elemSize;  
- int linesMax = INT_MAX;    //输出最多直线的数量，设为无穷多
- int iparam1, iparam2;  
- 
-     img = cvGetMat( img, &stub );  
- //确保输入图像是8位单通道
- if( !CV_IS_MASK_ARR(img))  
-         CV_Error( CV_StsBadArg, "The source image must be 8-bit, single-channel" );  
- //内存空间申请成功，以备输出之用
- if( !lineStorage )  
-         CV_Error( CV_StsNullPtr, "NULL destination" );  
- //确保rho，theta，threshold这三个参数大于0
- if( rho <= 0 || theta <= 0 || threshold <= 0 )  
-         CV_Error( CV_StsOutOfRange, "rho, theta and threshold must be positive" );  
- 
- if( method != CV_HOUGH_PROBABILISTIC )    //经典霍夫变换和多尺度霍夫变换
-     {  
- //输出直线的类型为32位浮点双通道，即ρ和θ两个浮点型变量
-         lineType = CV_32FC2;  
-         elemSize = sizeof(float)*2;  
-     }  
- else//概率霍夫变换
-     {  
- //输出直线的类型为32位有符号整型4通道，即两个像素点的4个坐标
-         lineType = CV_32SC4;  
-         elemSize = sizeof(int)*4;  
-     }  
- //判断lineStorage的类型，经分析lineStorage只可能是STORAGE类型
- if( CV_IS_STORAGE( lineStorage ))  
-     {  
- //在lineStorage内存中创建一个序列，用于存储霍夫变换的直线，lines为该序列的指针
-         lines = cvCreateSeq( lineType, sizeof(CvSeq), elemSize, (CvMemStorage*)lineStorage );  
-     }  
- elseif( CV_IS_MAT( lineStorage ))  
-     {  
-         mat = (CvMat*)lineStorage;  
- 
- if( !CV_IS_MAT_CONT( mat->type ) || (mat->rows != 1 && mat->cols != 1) )  
-             CV_Error( CV_StsBadArg,  
- "The destination matrix should be continuous and have a single row or a single column" );  
- 
- if( CV_MAT_TYPE( mat->type ) != lineType )  
-             CV_Error( CV_StsBadArg,  
- "The destination matrix data type is inappropriate, see the manual" );  
- 
-         lines = cvMakeSeqHeaderForArray( lineType, sizeof(CvSeq), elemSize, mat->data.ptr,  
-                                          mat->rows + mat->cols - 1, &lines_header, &lines_block );  
-         linesMax = lines->total;  
-         cvClearSeq( lines );  
-     }  
- else
-         CV_Error( CV_StsBadArg, "Destination is not CvMemStorage* nor CvMat*" );  
- 
-     iparam1 = cvRound(param1);  
-     iparam2 = cvRound(param2);  
- 
- switch( method )  
-     {  
- case CV_HOUGH_STANDARD:    //经典霍夫变换
-           icvHoughLinesStandard( img, (float)rho,  
-                 (float)theta, threshold, lines, linesMax );  
- break;  
- case CV_HOUGH_MULTI_SCALE:    //多尺度霍夫变换
-           icvHoughLinesSDiv( img, (float)rho, (float)theta,  
-                 threshold, iparam1, iparam2, lines, linesMax );  
- break;  
- case CV_HOUGH_PROBABILISTIC:    //概率霍夫变换
-           icvHoughLinesProbabilistic( img, (float)rho, (float)theta,  
-                 threshold, iparam1, iparam2, lines, linesMax );  
- break;  
- default:  
-         CV_Error( CV_StsBadArg, "Unrecognized method id" );  
-     }  
- //在前面判断lineStorage类型时，已经确定它是STORAGE类型，因此没有进入else if内，也就是没有对mat赋值，所以在这里进入的是else
- if( mat )  
-     {  
- if( mat->cols > mat->rows )  
-             mat->cols = lines->total;  
- else
-             mat->rows = lines->total;  
-     }  
- else
-         result = lines;  
- //返回lines序列的指针，该序列存储有霍夫变换的直线
- return result;  
- }  
对于经典霍夫变换，调用的是icvHoughLinesStandard函数：
- staticvoid
- icvHoughLinesStandard( const CvMat* img, float rho, float theta,  
- int threshold, CvSeq *lines, int linesMax )  
- {  
-     cv::AutoBuffer<int> _accum, _sort_buf;  
-     cv::AutoBuffer<float> _tabSin, _tabCos;  
- 
- const uchar* image;  
- int step, width, height;  
- int numangle, numrho;  
- int total = 0;  
- int i, j;  
- float irho = 1 / rho;  
- double scale;  
- //再次确保输入图像的正确性
-     CV_Assert( CV_IS_MAT(img) && CV_MAT_TYPE(img->type) == CV_8UC1 );  
- 
-     image = img->data.ptr;    //得到图像的指针
-     step = img->step;    //得到图像的步长
-     width = img->cols;    //得到图像的宽
-     height = img->rows;    //得到图像的高
- //由角度和距离的分辨率得到角度和距离的数量，即霍夫变换后角度和距离的个数
-     numangle = cvRound(CV_PI / theta);  
-     numrho = cvRound(((width + height) * 2 + 1) / rho);  
- 
- //为累加器数组分配内存空间
- //该累加器数组其实就是霍夫空间，它是用一维数组表示二维空间
-     _accum.allocate((numangle+2) * (numrho+2));  
- //为排序数组分配内存空间
-     _sort_buf.allocate(numangle * numrho);  
- //为正弦和余弦列表分配内存空间
-     _tabSin.allocate(numangle);  
-     _tabCos.allocate(numangle);  
- //分别定义上述内存空间的地址指针
- int *accum = _accum, *sort_buf = _sort_buf;  
- float *tabSin = _tabSin, *tabCos = _tabCos;  
- //累加器数组清零
-     memset( accum, 0, sizeof(accum[0]) * (numangle+2) * (numrho+2) );  
- 
- float ang = 0;  
- //为避免重复运算，事先计算好sinθi/ρ和cosθi/ρ
- for(int n = 0; n < numangle; ang += theta, n++ )  
-     {  
-         tabSin[n] = (float)(sin((double)ang) * irho);  
-         tabCos[n] = (float)(cos((double)ang) * irho);  
-     }  
- 
- // stage 1. fill accumulator
- //执行步骤1，逐点进行霍夫空间变换，并把结果放入累加器数组内
- for( i = 0; i < height; i++ )  
- for( j = 0; j < width; j++ )  
-         {  
- //只对图像的非零值处理，即只对图像的边缘像素进行霍夫变换
- if( image[i * step + j] != 0 )  
- for(int n = 0; n < numangle; n++ )  
-                 {  
- int r = cvRound( j * tabCos[n] + i * tabSin[n] );  
-                     r += (numrho - 1) / 2;  
- //r表示的是距离，n表示的是角点，在累加器内找到它们所对应的位置（即霍夫空间内的位置），其值加1
-                     accum[(n+1) * (numrho+2) + r+1]++;  
-                 }  
-         }  
- 
- // stage 2. find local maximums
- //执行步骤2，找到局部极大值，即非极大值抑制
- for(int r = 0; r < numrho; r++ )  
- for(int n = 0; n < numangle; n++ )  
-         {  
- //得到当前值在累加器数组的位置
- int base = (n+1) * (numrho+2) + r+1;  
- if( accum[base] > threshold &&    //必须大于所设置的阈值
- //在4邻域内进行非极大值抑制
-                 accum[base] > accum[base - 1] && accum[base] >= accum[base + 1] &&  
-                 accum[base] > accum[base - numrho - 2] && accum[base] >= accum[base + numrho + 2] )  
- //把极大值位置存入排序数组内——sort_buf
-                 sort_buf[total++] = base;  
-         }  
- 
- // stage 3. sort the detected lines by accumulator value
- //执行步骤3，对存储在sort_buf数组内的累加器的数据按由大到小的顺序进行排序
-     icvHoughSortDescent32s( sort_buf, total, accum );  
- 
- // stage 4. store the first min(total,linesMax) lines to the output buffer
- //执行步骤4，只输出前linesMax条直线，这里linesMax就等于total，即输出所有直线
-     linesMax = MIN(linesMax, total);  
- //事先定义一个尺度
-     scale = 1./(numrho+2);  
- for( i = 0; i < linesMax; i++ )  
-     {  
- // CvLinePolar结构在该文件的前面被定义
-         CvLinePolar line;  
- //idx为极大值在累加器数组的位置
- int idx = sort_buf[i];  
- //分离出该极大值在霍夫空间中的位置
- int n = cvFloor(idx*scale) - 1;  
- int r = idx - (n+1)*(numrho+2) - 1;  
- //最终得到极大值所对应的角度和距离
-         line.rho = (r - (numrho - 1)*0.5f) * rho;  
-         line.angle = n * theta;  
- //存储到序列内
-         cvSeqPush( lines, &line );  
-     }  
- }  
由于函数HoughLines只输出直线所对应的角度和距离，所以在进行直线检测的时候还要把其转换为直角坐标系下的数据，另外输入图像还必须是边缘图像，下面就是具体的实例：
- #include "opencv2/core/core.hpp"
- #include "opencv2/highgui/highgui.hpp"
- #include "opencv2/imgproc/imgproc.hpp"
- #include <iostream>
- usingnamespace cv;  
- usingnamespace std;  
- 
- int main( int argc, char** argv )  
- {  
-     Mat src, edge;  
-     src=imread("building.jpg");  
- if( !src.data )    
- return -1;    
- //Canny边缘检测
-     Canny(src,edge,50,200,3);  
- //定义输出数组，用于存储直线的角度和距离这两个变量
-     vector<Vec2f> lines;  
- //距离分辨率为1，角度分辨率为π/180，阈值为215
- //阈值的选取直接影响到输出直线的数量
-     HoughLines(edge,lines,1,CV_PI/180,215,0,0);  
- //画直线
- for( size_t i = 0; i < lines.size(); i++ )  
-     {  
- //提取出距离和角度
- float rho = lines[i][0], theta = lines[i][1];  
- //定义两个点，两点确定一条直线
- //计算得到的两点的坐标为（ρcosθ-1000sinθ，ρsinθ+1000cosθ），（ρcosθ+1000sinθ，ρsinθ-1000cosθ）
-         Point pt1, pt2;  
- double a = cos(theta), b = sin(theta);  
- double x0 = a*rho, y0 = b*rho;  
-         pt1.x = cvRound(x0 + 1000*(-b));  
-         pt1.y = cvRound(y0 + 1000*(a));  
-         pt2.x = cvRound(x0 - 1000*(-b));  
-         pt2.y = cvRound(y0 - 1000*(a));  
- //在原图上画宽带为2的红线
-         line( src, pt1, pt2, Scalar(0,0,255),2);  
-     }  
- 
-     namedWindow( "lines", CV_WINDOW_AUTOSIZE );  
-     imshow( "lines", src );  
-     waitKey(0);  
- 
- return 0;  
- }  
