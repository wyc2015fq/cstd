# OpenCV中IplImage/CvMat/Mat转化关系 - wishchinYang的专栏 - CSDN博客
2015年08月30日 15:22:04[wishchin](https://me.csdn.net/wishchin)阅读数：414
    原文链接：[http://www.cnblogs.com/summerRQ/articles/2406109.html](http://www.cnblogs.com/summerRQ/articles/2406109.html)
    如对内容和版权有何疑问，请拜访原作者或者通知本人。
    opencv中常见的与图像操作有关的数据容器有Mat，cvMat和IplImage，这三种类型都可以代表和显示图像，但是，Mat类型侧重于计算，数学性较高，openCV对Mat类型的计算也进行了优化。而CvMat和IplImage类型更侧重于“图像”，opencv对其中的图像操作（缩放、单通道提取、图像阈值操作等）进行了优化。在opencv2.0之前，opencv是完全用C实现的，但是，IplImage类型与CvMat类型的关系类似于面向对象中的继承关系。实际上，CvMat之上还有一个更抽象的基类----CvArr，这在源代码中会常见。
**1. IplImage**
opencv中的图像信息头，该结构体定义：
```
typedef struct _IplImage 
{ 
    int nSize;    /* IplImage大小 */
    int ID;    /* 版本 (=0)*/
    int nChannels;  /* 大多数OPENCV函数支持1,2,3 或 4 个通道 */ 
    int alphaChannel;  /* 被OpenCV忽略 */ 
    int depth;   /* 像素的位深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
                IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 可支持 */ 
    
    char colorModel[4]; /* 被OpenCV忽略 */ 
    char channelSeq[4]; /* 被OpenCV忽略 */ 
    int dataOrder;      /* 0 - 交叉存取颜色通道, 1 - 分开的颜色通道. cvCreateImage只能创建交叉存取图像 */ 
    int origin;     /* 0 - 顶—左结构,1 - 底—左结构 (Windows bitmaps 风格) */ 
    int align;     /* 图像行排列 (4 or 8). OpenCV 忽略它，使用 widthStep 代替 */ 
    
    int width;     /* 图像宽像素数 */ 
    int height;    /* 图像高像素数*/ 
    
    struct _IplROI *roi;  /* 图像感兴趣区域. 当该值非空只对该区域进行处理 */ 
    struct _IplImage *maskROI; /* 在 OpenCV中必须置NULL */ 
    void *imageId;  /* 同上*/ 
    struct _IplTileInfo *tileInfo;  /*同上*/ 
    
    int imageSize;    /* 图像数据大小(在交叉存取格式下imageSize=image->height*image->widthStep），单位字节*/ 
    char *imageData;    /* 指向排列的图像数据 */ 
    int widthStep;     /* 排列的图像行大小，以字节为单位 */ 
    int BorderMode[4];     /* 边际结束模式, 被OpenCV忽略 */ 
    int BorderConst[4];    /* 同上 */ 
    
    char *imageDataOrigin;    /* 指针指向一个不同的图像数据结构（不是必须排列的），是为了纠正图像内存分配准备的 */ 
} IplImage;
```
dataOrder中的两个取值：交叉存取颜色通道是颜色数据排列将会是BGRBGR...的交错排列。分开的颜色通道是有几个颜色通道就分几个颜色平面存储。roi是IplROI结构体，该结构体包含了xOffset,yOffset,height,width,coi成员变量，其中xOffset,yOffset是x,y坐标，coi代表channel
 of interest(感兴趣的通道)，非0的时候才有效。访问图像中的数据元素，分间接存储和直接存储，当图像元素为浮点型时，(uchar *) 改为 (float *)：
```
/*间接存取*/
IplImage* img=cvLoadImage("lena.jpg", 1);
CvScalar s;       /*sizeof(s) == img->nChannels*/
s=cvGet2D(img,i,j);  /*get the (i,j) pixel value*/
cvSet2D(img,i,j,s);   /*set the (i,j) pixel value*/
/*宏操作*/
IplImage* img; //malloc memory by cvLoadImage or cvCreateImage
for(int row = 0; row <　img->height; row++)
{
    for (int col = 0; col < img->width; col++)
    {
        b = CV_IMAGE_ELEM(img, UCHAR, row, col * img->nChannels + 0); 
        g = CV_IMAGE_ELEM(img, UCHAR, row, col * img->nChannels + 1); 
        r = CV_IMAGE_ELEM(img, UCHAR, row, col * img->nChannels + 2);
    }
}
/*直接存取*/
IplImage* img; //malloc memory by cvLoadImage or cvCreateImage
uchar b, g, r; // 3 channels
for(int row = 0; row <　img->height; row++)
{
    for (int col = 0; col < img->width; col++)
    {
        b = ((uchar *)(img->imageData + row * img->widthStep))[col * img->nChannels + 0]; 
        g = ((uchar *)(img->imageData + row * img->widthStep))[col * img->nChannels + 1]; 
        r = ((uchar *)(img->imageData + row * img->widthStep))[col * img->nChannels + 2];
    }
}
```
初始化使用IplImage *，是一个指向结构体IplImage的指针：
```
IplImage * cvLoadImage(const char * filename, int iscolor CV_DEFAULT(CV_LOAD_IMAGE_COLOR)); //load images from specified image 
IplImage * cvCreateImage(CvSize size, int depth, int channels);  //allocate memory
```
**2.CvMat**
首先，我们需要知道，第一，在OpenCV中没有向量(vector)结构。任何时候需要向量，都只需要一个列矩阵(如果需要一个转置或者共轭向量，则需要一个行矩阵)。第二，OpenCV矩阵的概念与我们在线性代数课上学习的概念相比，更抽象，尤其是矩阵的元素，并非只能取简单的数值类型，可以是多通道的值。CvMat 的结构：
```
typedef struct CvMat 
{ 
    int type;         
    int step;          /*用字节表示行数据长度*/
    int* refcount;     /*内部访问*/
    union {
        uchar*  ptr;
        short*  s;
        int*    i;
        float*  fl;
        double* db;
    } data;    /*数据指针*/
     union {
        int rows;
        int height;
    };
    union {
        int cols;   
        int width;
    };
} CvMat; /*矩阵结构头*/
```
创建CvMat数据：
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
CvMat * cvCreateMat(int rows, int cols, int type); /*创建矩阵头并分配内存*/
CV_INLine CvMat cvMat((int rows, int cols, int type, void* data CV_DEFAULT); /*用已有数据data初始化矩阵*/
CvMat * cvInitMatHeader(CvMat * mat, int rows, int cols, int type, void * data CV_DEFAULT(NULL), int step CV_DEFAULT(CV_AUTOSTEP)); /*(用已有数据data创建矩阵头)*/
```
对矩阵数据进行访问：
```
/*间接访问*/
/*访问CV_32F1和CV_64FC1*/
cvmSet( CvMat* mat, int row, int col, double value);
cvmGet( const CvMat* mat, int row, int col );
/*访问多通道或者其他数据类型: scalar的大小为图像的通道值*/
CvScalar cvGet2D(const CvArr * arr, int idx0, int idx1); //CvArr只作为函数的形参void cvSet2D(CvArr* arr, int idx0, int idx1, CvScalar value);
/*直接访问: 取决于数组的数据类型*/
/*CV_32FC1*/
CvMat * cvmat = cvCreateMat(4, 4, CV_32FC1);
cvmat->data.fl[row * cvmat->cols + col] = (float)3.0;
/*CV_64FC1*/
CvMat * cvmat = cvCreateMat(4, 4, CV_64FC1);
cvmat->data.db[row * cvmat->cols + col] = 3.0;
/*一般对于单通道*/
CvMat * cvmat = cvCreateMat(4, 4, CV_64FC1);
CV_MAT_ELEM(*cvmat, double, row, col) = 3.0; /*double是根据数组的数据类型传入,这个宏不能处理多通道*/
/*一般对于多通道*/
if (CV_MAT_DEPTH(cvmat->type) == CV_32F)
    CV_MAT_ELEM_CN(*cvmat, float, row, col * CV_MAT_CN(cvmat->type) + ch) = (float)3.0; // ch为通道值
if (CV_MAT_DEPTH(cvmat->type) == CV_64F)
    CV_MAT_ELEM_CN(*cvmat, double, row, col * CV_MAT_CN(cvmat->type) + ch) = 3.0; // ch为通道值
/*多通道数组*/
/*3通道*/
for (int row = 0; row < cvmat->rows; row++)
{    
    p = cvmat ->data.fl + row * (cvmat->step / 4);
    for (int col = 0; col < cvmat->cols; col++)   
    {       
         *p = (float) row + col;       
         *(p+1) = (float)row + col + 1;       
         *(p+2) = (float)row + col + 2;       
         p += 3;    
    }
}
/*2通道*/
CvMat * vector = cvCreateMat(1,3, CV_32SC2);CV_MAT_ELEM(*vector, CvPoint, 0, 0) = cvPoint(100,100);
/*4通道*/
CvMat * vector = cvCreateMat(1,3, CV_64FC4);CV_MAT_ELEM(*vector, CvScalar, 0, 0) = CvScalar(0, 0, 0, 0);
```
复制矩阵操作：
```
/*复制矩阵*/
CvMat* M1 = cvCreateMat(4,4,CV_32FC1);
CvMat* M2;
M2=cvCloneMat(M1);
```
**3.Mat**
Mat是opencv2.0推出的处理图像的新的数据结构，现在越来越有趋势取代之前的cvMat和lplImage，相比之下Mat最大的好处就是能够更加方便的进行内存管理，不再需要程序员手动管理内存的释放。opencv2.3中提到Mat是一个多维的密集数据数组，可以用来处理向量和矩阵、图像、直方图等等常见的多维数据。
```
class CV_EXPORTS Mat
{
 
public：
 
/*..很多方法..*/
/*............*/
 
int flags;（Note ：目前还不知道flags做什么用的）
int dims;  /*数据的维数*/
int rows,cols; /*行和列的数量;数组超过2维时为(-1，-1)*/
uchar *data;   /*指向数据*/
int * refcount;   /*指针的引用计数器; 阵列指向用户分配的数据时，指针为 NULL
 
/* 其他成员 */ 
...
 
};
```
从以上结构体可以看出Mat也是一个矩阵头，默认不分配内存，只是指向一块内存(注意读写保护)。初始化使用create函数或者Mat构造函数，以下整理自opencv2.3.1 Manual:
```
Mat(nrows, ncols, type, fillValue]); 
M.create(nrows, ncols, type);
例子：
Mat M(7,7,CV_32FC2,Scalar(1,3)); /*创建复数矩阵1+3j*/
M.create(100, 60, CV_8UC(15)); /*创建15个通道的8bit的矩阵*/
/*创建100*100*100的8位数组*/
int sz[] = {100, 100, 100}; 
Mat bigCube(3, sz, CV_8U, Scalar:all(0));
/*现成数组*/
double m[3][3] = {{a, b, c}, {d, e, f}, {g, h, i}};
Mat M = Mat(3, 3, CV_64F, m).inv();
/*图像数据*/
Mat img(Size(320,240),CV_8UC3); 
Mat img(height, width, CV_8UC3, pixels, step); /*const unsigned char* pixels,int width, int height, int step*/
/*使用现成图像初始化Mat*/
IplImage* img = cvLoadImage("greatwave.jpg", 1);
Mat mtx(img,0); // convert IplImage* -> Mat; /*不复制数据,只创建一个数据头*/
```
访问Mat的数据元素：
```
/*对某行进行访问*/
Mat M;
M.row(3) = M.row(3) + M.row(5) * 3; /*第5行扩大三倍加到第3行*/
/*对某列进行复制操作*/
Mat M1 = M.col(1);
M.col(7).copyTo(M1); /*第7列复制给第1列*/
/*对某个元素的访问*/
Mat M;
M.at<double>(i,j); /*double*/
M.at(uchar)(i,j);  /*CV_8UC1*/
Vec3i bgr1 = M.at(Vec3b)(i,j) /*CV_8UC3*/
Vec3s bgr2 = M.at(Vec3s)(i,j) /*CV_8SC3*/
Vec3w bgr3 = M.at(Vec3w)(i,j) /*CV_16UC3*/
/*遍历整个二维数组*/
double sum = 0.0f;
for(int row = 0; row < M.rows; row++)
{    
    const double * Mi = M.ptr<double>(row); 
    for (int col = 0; col < M.cols; col++)      
        sum += std::max(Mi[j], 0.);
}
/*STL iterator*/
double sum=0;
MatConstIterator<double> it = M.begin<double>(), it_end = M.end<double>();
for(; it != it_end; ++it)    
sum += std::max(*it, 0.);
```
Mat可进行Matlab风格的矩阵操作，如初始化的时候可以用initializers,zeros(), ones(), eye(). 除以上内容之外，Mat还有有3个重要的方法：
```
Mat mat = imread(const String* filename);           // 读取图像
imshow(const string frameName, InputArray mat);  //    显示图像
imwrite (const string& filename, InputArray img);    //储存图像
```
4. CvMat, Mat, IplImage之间的互相转换
```
IpIImage -> CvMat
/*cvGetMat*/
CvMat matheader;
CvMat * mat = cvGetMat(img, &matheader);
/*cvConvert*/
CvMat * mat = cvCreateMat(img->height, img->width, CV_64FC3);
cvConvert(img, mat)
IplImage -> Mat
Mat::Mat(const IplImage* img, bool copyData=false);/*default copyData=false,与原来的IplImage共享数据,只是创建一个矩阵头*/
例子：
IplImage* iplImg = cvLoadImage("greatwave.jpg", 1);
Mat mtx(iplImg); /* IplImage * -> Mat,共享数据; or : Mat mtx = iplImg;*/
 
Mat -> IplImage
Mat M
IplImage iplimage = M; /*只创建图像头，不复制数据*/
CvMat -> Mat
Mat::Mat(const CvMat* m, bool copyData=false); /*类似IplImage -> Mat，可选择是否复制数据*/
Mat -> CvMat
例子(假设Mat类型的imgMat图像数据存在)：
CvMat cvMat = imgMat;/*Mat -> CvMat, 类似转换到IplImage，不复制数据只创建矩阵头
```
