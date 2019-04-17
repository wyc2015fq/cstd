# 图像局部特征（十四）--MSER特征 - 工作笔记 - CSDN博客





2016年07月18日 17:10:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8237
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文:

http://blog.csdn.net/zhaocj/article/details/40742191

最大稳定极值区域（MSER-Maximally Stable Extremal Regions）可以用于图像的斑点区域检测。该算法最早是由Matas等人于2002年提出，它是基于分水岭的概念。

MSER的基本原理是对一幅灰度图像（灰度值为0～255）取阈值进行二值化处理，阈值从0到255依次递增。阈值的递增类似于分水岭算法中的水面的上升，随着水面的上升，有一些较矮的丘陵会被淹没，如果从天空往下看，则大地分为陆地和水域两个部分，这类似于二值图像。在得到的所有二值图像中，图像中的某些连通区域变化很小，甚至没有变化，则该区域就被称为最大稳定极值区域。这类似于当水面持续上升的时候，有些被水淹没的地方的面积没有变化。它的数学定义为：

*q*(*i*)=|*Qi*+△-*Qi*-△|/|*Qi*|            （1）

其中，*Qi*表示阈值为*i*时的某一连通区域，△为灰度阈值的微小变化量，*q*(*i*)为阈值是*i*时的区域*Qi*的变化率。当*q*(*i*)为局部极小值时，则*Qi*为最大稳定极值区域。

需要说明的是，上述做法只能检测出灰度图像的黑色区域，不能检测出白色区域，因此还需要对原图进行反转，然后再进行阈值从0～255的二值化处理过程。这两种操作又分别称为MSER+和MSER-。

MSER具有以下特点：

1、对图像灰度具有仿射变换的不变性；

2、稳定性：具有相同阈值范围内所支持的区域才会被选择；

3、无需任何平滑处理就可以实现多尺度检测，即小的和大的结构都可以被检测到。

MSER的原理比较简单，但要更快更好的实现它，是需要一定的算法、数据结构和编程技巧的。David Nister等人于2008年提出了Linear Time Maximally Stable Extremal Regions算法，该算法要比原著提出的算法快，opencv就是利用该算法实现MSER的。但这里要说明一点的是，opencv不是利用公式1计算MSER的，而是利用更易于实现的改进方法：

*q*(*i*)=|*Qi*-*Qi*-△|/|*Qi*-△|            （2）

David Nister提出的算法是基于改进的分水岭算法，即当往一个固定的地方注水的时候，只有当该地方的沟壑被水填满以后，水才会向其四周溢出，随着注水量的不断增加，各个沟壑也会逐渐被水淹没，但各个沟壑的水面不是同时上升的，它是根据水漫过地方的先后顺序，一个沟壑一个沟壑地填满水，只有当相邻两个沟壑被水连通在一起以后，水面对于这两个沟壑来说才是同时上升的。该算法的具体步骤如下：

1、初始化栈和堆，栈用于存储组块（组块就是区域，就相当于水面，水漫过的地方就会出现水面，水面的高度就是图像的灰度值，因此用灰度值来表示组块的值），堆用于存储组块的边界像素，相当于水域的岸边，岸边要高于水面的，因此边界像素的灰度值一定不小于它所包围的区域（即组块）的灰度值。首先向栈内放入一个虚假的组块，当该组块被弹出时意味着程序的结束；

2、把图像中的任意一个像素（一般选取图像的左上角像素）作为源像素，标注该像素为已访问过，并且把该像素的灰度值作为当前值。这一步相当于往源像素这一地点注水；

3、向栈内放入一个空组块，该组块的值是当前值；

4、按照顺序搜索当前值的4-领域内剩余的边缘，对于每一个邻域，检查它是否已经被访问过，如果没有，则标注它为已访问过并检索它的灰度值，如果灰度值不小于当前值，则把它放入用于存放边界像素的堆中。另一方面，如果领域灰度值小于当前值，则把当前值放入堆中，而把领域值作为当前值，并回到步骤3；

5、累计栈顶组块的像素个数，即计算区域面积，这是通过循环累计得到的，这一步相当于水面的饱和；

6、弹出堆中的边界像素。如果堆是空的，则程序结束；如果弹出的边界像素的灰度值等于当前值，则回到步骤4；

7、从堆中得到的像素值会大于当前值，因此我们需要处理栈中所有的组块，直到栈中的组块的灰度值大于当前边界像素灰度值为止。然后回到步骤4。

至于如何处理组块，则需要进入**处理栈**子模块中，传入该子模块的值为步骤7中从堆中提取得到的边界像素灰度值。子模块的具体步骤为：

1）、处理栈顶的组块，即根据公式2计算最大稳定区域，判断其是否为极值区域；

2）、如果边界像素灰度值小于距栈顶第二个组块的灰度值，那么设栈顶组块的灰度值为边界像素灰度值，并退出该子模块。之所以会出现这种情况，是因为在栈顶组块和第二个组块之间还有组块没有被检测处理，因此我们需要改变栈顶组块的灰度值为边界像素灰度值（相当于这两层的组块进行了合并），并回到主程序，再次搜索组块；

3）、弹出栈顶组块，并与目前栈顶组块合并；

4）、如果边界像素灰度值大于栈顶组块的灰度值，则回到步骤1。

在opencv2.4.9中，MSER算法是用类的方法给出的：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- class MserFeatureDetector : public FeatureDetector  
- {  
- public:  
- MserFeatureDetector( CvMSERParams params=cvMSERParams() );  
- MserFeatureDetector( int delta, int minArea, int maxArea,  
- double maxVariation, double minDiversity,  
- int maxEvolution, double areaThreshold,  
- double minMargin, int edgeBlurSize );  
- virtual void read( const FileNode& fn );  
- virtual void write( FileStorage& fs ) const;  
- protected:  
- ...  
- };  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
class MserFeatureDetector : public FeatureDetector
{
public:
MserFeatureDetector( CvMSERParams params=cvMSERParams() );
MserFeatureDetector( int delta, int minArea, int maxArea,
double maxVariation, double minDiversity,
int maxEvolution, double areaThreshold,
double minMargin, int edgeBlurSize );
virtual void read( const FileNode& fn );
virtual void write( FileStorage& fs ) const;
protected:
...
};
```

而具体的MSER类为：





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- class MSER : public CvMSERParams  
- {  
- public:  
- // default constructor  
- //缺省的构造函数  
- MSER();  
- // constructor that initializes all the algorithm parameters  
- //带有所有算法参数的构造函数  
- MSER( int _delta, int _min_area, int _max_area,  
- float _max_variation, float _min_diversity,  
- int _max_evolution, double _area_threshold,  
- double _min_margin, int _edge_blur_size );  
- // runs the extractor on the specified image; returns the MSERs,  
- // each encoded as a contour (vector<Point>, see findContours)  
- // the optional mask marks the area where MSERs are searched for  
- void operator()( const Mat& image, vector<vector<Point> >& msers, const Mat& mask ) const;  
- };  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
class MSER : public CvMSERParams
{
public:
// default constructor
//缺省的构造函数
MSER();
// constructor that initializes all the algorithm parameters
//带有所有算法参数的构造函数
MSER( int _delta, int _min_area, int _max_area,
float _max_variation, float _min_diversity,
int _max_evolution, double _area_threshold,
double _min_margin, int _edge_blur_size );
// runs the extractor on the specified image; returns the MSERs,
// each encoded as a contour (vector<Point>, see findContours)
// the optional mask marks the area where MSERs are searched for
void operator()( const Mat& image, vector<vector<Point> >& msers, const Mat& mask ) const;
};
```

**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- MSER算法所需要的参数较多：  
- delta为灰度值的变化量，即公式1和2中的△；  
- _min_area和_max_area为检测到的组块面积的范围；  
- _max_variation为最大的变化率，即如果公式1和2中的q(i)小于该值，则被认为是最大稳定极值区域；  
- _min_diversity为稳定区域的最小变换量。  
- 其他的参数用于对彩色图像的MSER检测，这里不做介绍。  
- MSER类通过重载( )运算符，得到了最大稳定极值区域的点集msers，其中image为输入图像，mask为掩码矩阵。  
- 下面我们就对MSER类的源码进行分析：  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
MSER算法所需要的参数较多：
delta为灰度值的变化量，即公式1和2中的△；
_min_area和_max_area为检测到的组块面积的范围；
_max_variation为最大的变化率，即如果公式1和2中的q(i)小于该值，则被认为是最大稳定极值区域；
_min_diversity为稳定区域的最小变换量。
其他的参数用于对彩色图像的MSER检测，这里不做介绍。
MSER类通过重载( )运算符，得到了最大稳定极值区域的点集msers，其中image为输入图像，mask为掩码矩阵。
下面我们就对MSER类的源码进行分析：
```

**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- void MSER::operator()( const Mat& image, vector<vector<Point> >& dstcontours, const Mat& mask ) const  
- {  
-     CvMat _image = image, _mask, *pmask = 0;  
-     if( mask.data )  
-         pmask = &(_mask = mask);  
-     MemStorage storage(cvCreateMemStorage(0));  
-     Seq<CvSeq*> contours;  
-     //调用extractMSER函数，得到MSER的区域点集序列contours.seq  
-     //MSERParams为MSER所需要的参数  
-     extractMSER( &_image, pmask, &contours.seq, storage,  
-                  MSERParams(delta, minArea, maxArea, maxVariation, minDiversity,  
-                             maxEvolution, areaThreshold, minMargin, edgeBlurSize));  
-     SeqIterator<CvSeq*> it = contours.begin();  
-     size_t i, ncontours = contours.size();  
-     dstcontours.resize(ncontours);  
-     //复制点集序列  
-     for( i = 0; i < ncontours; i++, ++it )  
-         Seq<Point>(*it).copyTo(dstcontours[i]);  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void MSER::operator()( const Mat& image, vector<vector<Point> >& dstcontours, const Mat& mask ) const
{
    CvMat _image = image, _mask, *pmask = 0;
    if( mask.data )
        pmask = &(_mask = mask);
    MemStorage storage(cvCreateMemStorage(0));
    Seq<CvSeq*> contours;
    //调用extractMSER函数，得到MSER的区域点集序列contours.seq
    //MSERParams为MSER所需要的参数
    extractMSER( &_image, pmask, &contours.seq, storage,
                 MSERParams(delta, minArea, maxArea, maxVariation, minDiversity,
                            maxEvolution, areaThreshold, minMargin, edgeBlurSize));
    SeqIterator<CvSeq*> it = contours.begin();
    size_t i, ncontours = contours.size();
    dstcontours.resize(ncontours);
    //复制点集序列
    for( i = 0; i < ncontours; i++, ++it )
        Seq<Point>(*it).copyTo(dstcontours[i]);
}
```

extractMSER函数首先定义了一些变量，并进行参数的判断。然后根据输入图像的类型分别调用不同的函数：





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- static void  
- extractMSER( CvArr* _img,  
-            CvArr* _mask,  
-            CvSeq** _contours,  
-            CvMemStorage* storage,  
-            MSERParams params )  
- {  
-     CvMat srchdr, *src = cvGetMat( _img, &srchdr );  
-     CvMat maskhdr, *mask = _mask ? cvGetMat( _mask, &maskhdr ) : 0;  
-     CvSeq* contours = 0;  
- 
-     CV_Assert(src != 0);  
-     CV_Assert(CV_MAT_TYPE(src->type) == CV_8UC1 || CV_MAT_TYPE(src->type) == CV_8UC3);  
-     CV_Assert(mask == 0 || (CV_ARE_SIZES_EQ(src, mask) && CV_MAT_TYPE(mask->type) == CV_8UC1));  
-     CV_Assert(storage != 0);  
- 
-     contours = *_contours = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSeq*), storage );  
- 
-     // choose different method for different image type  
-     // for grey image, it is: Linear Time Maximally Stable Extremal Regions  
-     // for color image, it is: Maximally Stable Colour Regions for Recognition and Matching  
-     switch ( CV_MAT_TYPE(src->type) )  
-     {  
-         case CV_8UC1:    //处理灰度图像  
-             extractMSER_8UC1( src, mask, contours, storage, params );  
-             break;  
-         case CV_8UC3:    //处理彩色图像  
-             extractMSER_8UC3( src, mask, contours, storage, params );  
-             break;  
-     }  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
static void
extractMSER( CvArr* _img,
           CvArr* _mask,
           CvSeq** _contours,
           CvMemStorage* storage,
           MSERParams params )
{
    CvMat srchdr, *src = cvGetMat( _img, &srchdr );
    CvMat maskhdr, *mask = _mask ? cvGetMat( _mask, &maskhdr ) : 0;
    CvSeq* contours = 0;

    CV_Assert(src != 0);
    CV_Assert(CV_MAT_TYPE(src->type) == CV_8UC1 || CV_MAT_TYPE(src->type) == CV_8UC3);
    CV_Assert(mask == 0 || (CV_ARE_SIZES_EQ(src, mask) && CV_MAT_TYPE(mask->type) == CV_8UC1));
    CV_Assert(storage != 0);

    contours = *_contours = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSeq*), storage );

    // choose different method for different image type
    // for grey image, it is: Linear Time Maximally Stable Extremal Regions
    // for color image, it is: Maximally Stable Colour Regions for Recognition and Matching
    switch ( CV_MAT_TYPE(src->type) )
    {
        case CV_8UC1:    //处理灰度图像
            extractMSER_8UC1( src, mask, contours, storage, params );
            break;
        case CV_8UC3:    //处理彩色图像
            extractMSER_8UC3( src, mask, contours, storage, params );
            break;
    }
}
```

灰度图像的MSER处理方法就是应用本文介绍的方法：





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- static void extractMSER_8UC1( CvMat* src,  
-              CvMat* mask,  
-              CvSeq* contours,  
-              CvMemStorage* storage,  
-              MSERParams params )  
- {  
-     //为了加快运算速度，把原图的宽扩展成高度复合数，即2^N的形式  
-     //step为扩展后的宽，初始值为8  
-     int step = 8;  
-     //stepgap为N，初始值为3，即8=2^3  
-     int stepgap = 3;  
-     //通过step向左移位的方式扩展原图的宽  
-     while ( step < src->step+2 )  
-     {  
-         step <<= 1;  
-         stepgap++;  
-     }  
-     int stepmask = step-1;  
- 
-     // to speedup the process, make the width to be 2^N  
-     //创建扩展后的图像矩阵，宽为2^N，高为原图高+2  
-     CvMat* img = cvCreateMat( src->rows+2, step, CV_32SC1 );  
-     //定义第二行地址  
-     int* ioptr = img->data.i+step+1;  
-     int* imgptr;  
- 
-     // pre-allocate boundary heap  
-     //步骤1，初始化堆和栈  
-     //定义堆，用于存储组块的边界像素  
-     int** heap = (int**)cvAlloc( (src->rows*src->cols+256)*sizeof(heap[0]) );  
-     int** heap_start[256];  
-     //heap_start为三指针变量，heap_start为边界像素的灰度值，因此它数量为256个；*heap_start表示边界像素中该灰度值的数量；**heap_start表示边界像素中该灰度值中第*heap_start个所对应的像素地址指针。  
-     heap_start[0] = heap;  
- 
-     // pre-allocate linked point and grow history  
-     //pts表示组块内像素链表，即像素间相互链接  
-     LinkedPoint* pts = (LinkedPoint*)cvAlloc( src->rows*src->cols*sizeof(pts[0]) );  
-     //history表示每个组块生长的状况，即随着阈值的增加，组块的大小是在不断扩大的  
-     MSERGrowHistory* history = (MSERGrowHistory*)cvAlloc( src->rows*src->cols*sizeof(history[0]) );  
-     //comp表示图像内连通的组块（即区域），原则上每一个灰度值就会有一个组块，但之所以组块的个数是257个，是因为有一个组块是虚假组块，用于表示程序的结束。用栈的形式来管理各个组块  
-     MSERConnectedComp comp[257];  
- 
-     // darker to brighter (MSER-)  
-     //MSER-  
-     //先对原图进行预处理  
-     imgptr = preprocessMSER_8UC1( img, heap_start, src, mask );  
-     //执行MSER操作  
-     extractMSER_8UC1_Pass( ioptr, imgptr, heap_start, pts, history, comp, step, stepmask, stepgap, params, -1, contours, storage );  
-     // brighter to darker (MSER+)  
-     //MSER+  
-     imgptr = preprocessMSER_8UC1( img, heap_start, src, mask );  
-     extractMSER_8UC1_Pass( ioptr, imgptr, heap_start, pts, history, comp, step, stepmask, stepgap, params, 1, contours, storage );  
- 
-     // clean up  
-     //清理内存  
-     cvFree( &history );  
-     cvFree( &heap );  
-     cvFree( &pts );  
-     cvReleaseMat( &img );  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
static void extractMSER_8UC1( CvMat* src,
             CvMat* mask,
             CvSeq* contours,
             CvMemStorage* storage,
             MSERParams params )
{
    //为了加快运算速度，把原图的宽扩展成高度复合数，即2^N的形式
    //step为扩展后的宽，初始值为8
    int step = 8;
    //stepgap为N，初始值为3，即8=2^3
    int stepgap = 3;
    //通过step向左移位的方式扩展原图的宽
    while ( step < src->step+2 )
    {
        step <<= 1;
        stepgap++;
    }
    int stepmask = step-1;

    // to speedup the process, make the width to be 2^N
    //创建扩展后的图像矩阵，宽为2^N，高为原图高+2
    CvMat* img = cvCreateMat( src->rows+2, step, CV_32SC1 );
    //定义第二行地址
    int* ioptr = img->data.i+step+1;
    int* imgptr;

    // pre-allocate boundary heap
    //步骤1，初始化堆和栈
    //定义堆，用于存储组块的边界像素
    int** heap = (int**)cvAlloc( (src->rows*src->cols+256)*sizeof(heap[0]) );
    int** heap_start[256];
    //heap_start为三指针变量，heap_start为边界像素的灰度值，因此它数量为256个；*heap_start表示边界像素中该灰度值的数量；**heap_start表示边界像素中该灰度值中第*heap_start个所对应的像素地址指针。
    heap_start[0] = heap;

    // pre-allocate linked point and grow history
    //pts表示组块内像素链表，即像素间相互链接
    LinkedPoint* pts = (LinkedPoint*)cvAlloc( src->rows*src->cols*sizeof(pts[0]) );
    //history表示每个组块生长的状况，即随着阈值的增加，组块的大小是在不断扩大的
    MSERGrowHistory* history = (MSERGrowHistory*)cvAlloc( src->rows*src->cols*sizeof(history[0]) );
    //comp表示图像内连通的组块（即区域），原则上每一个灰度值就会有一个组块，但之所以组块的个数是257个，是因为有一个组块是虚假组块，用于表示程序的结束。用栈的形式来管理各个组块
    MSERConnectedComp comp[257];

    // darker to brighter (MSER-)
    //MSER-
    //先对原图进行预处理
    imgptr = preprocessMSER_8UC1( img, heap_start, src, mask );
    //执行MSER操作
    extractMSER_8UC1_Pass( ioptr, imgptr, heap_start, pts, history, comp, step, stepmask, stepgap, params, -1, contours, storage );
    // brighter to darker (MSER+)
    //MSER+
    imgptr = preprocessMSER_8UC1( img, heap_start, src, mask );
    extractMSER_8UC1_Pass( ioptr, imgptr, heap_start, pts, history, comp, step, stepmask, stepgap, params, 1, contours, storage );

    // clean up
    //清理内存
    cvFree( &history );
    cvFree( &heap );
    cvFree( &pts );
    cvReleaseMat( &img );
}
```



preprocessMSER_8UC1函数为预处理的过程，主要就是对宽度扩展以后的图像矩阵进行赋值，每一位都赋予不同的含义：

// to preprocess src image to followingformat

// 32-bit image

// > 0 is available, < 0 is visited

// 17~19 bits is the direction

// 8~11 bits is the bucket it falls to (forBitScanForward)

// 0~8 bits is the color

/*定义图像矩阵的数据格式为有符号32位整型，最高一位表示是否被访问过，0表示没有被访问过，1表示被访问过（因为是有符号数，所以最高一位是1也就是负数）；16～18位（这里源码注解有误）表示4邻域的方向；0～7位表示该像素的灰度值*/

//img代表图像矩阵，MSER处理的是该矩阵

//src为输入原图

//heap_cur为边界像素堆

//mask为掩码



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- static int* preprocessMSER_8UC1( CvMat* img,  
-             int*** heap_cur,  
-             CvMat* src,  
-             CvMat* mask )  
- {  
-     int srccpt = src->step-src->cols;  
-     //由于图像矩阵的宽经过了2^N处理，所以它的宽比原图的宽要长，cpt_1代表两个宽度的差值  
-     int cpt_1 = img->cols-src->cols-1;  
-     //图像矩阵的地址指针  
-     int* imgptr = img->data.i;  
-     //定义真实的图像起始地址  
-     int* startptr;  
- 
-     //定义并初始化灰度值数组  
-     int level_size[256];  
-     for ( int i = 0; i < 256; i++ )  
-         level_size[i] = 0;  
-     //为图像矩阵的第一行赋值  
-     for ( int i = 0; i < src->cols+2; i++ )  
-     {  
-         *imgptr = -1;  
-         imgptr++;  
-     }  
-     //地址指针指向图像矩阵的第二行  
-     imgptr += cpt_1-1;  
-     //原图首地址指针  
-     uchar* srcptr = src->data.ptr;  
-     if ( mask )    //如果定义了掩码矩阵  
-     {  
-         startptr = 0;  
-         //掩码矩阵首地址指针  
-         uchar* maskptr = mask->data.ptr;  
-         //遍历整个原图  
-         for ( int i = 0; i < src->rows; i++ )  
-         {  
-             *imgptr = -1;  
-             imgptr++;  
-             for ( int j = 0; j < src->cols; j++ )  
-             {  
-                 if ( *maskptr )  
-                 {  
-                     if ( !startptr )  
-                         startptr = imgptr;    //赋值  
-                     *srcptr = 0xff-*srcptr;    //反转图像的灰度值  
-                     level_size[*srcptr]++;    //为灰度值计数  
-                     //为图像矩阵赋值，它的低8位是原图灰度值，8～10位是灰度值的高3位  
-                     *imgptr = ((*srcptr>>5)<<8)|(*srcptr);  
-                 } else {    //掩码覆盖的像素值为-1  
-                     *imgptr = -1;  
-                 }  
-                 //地址加1  
-                 imgptr++;  
-                 srcptr++;  
-                 maskptr++;  
-             }  
-             //跳过图像矩阵比原图多出的部分  
-             *imgptr = -1;  
-             imgptr += cpt_1;  
-             srcptr += srccpt;  
-             maskptr += srccpt;  
-         }  
-     } else {    //没有定义掩码  
-         startptr = imgptr+img->cols+1;    //赋值  
-         //遍历整幅图像，为图像矩阵赋值  
-         for ( int i = 0; i < src->rows; i++ )  
-         {  
-             *imgptr = -1;  
-             imgptr++;  
-             for ( int j = 0; j < src->cols; j++ )  
-             {  
-                 *srcptr = 0xff-*srcptr;  
-                 level_size[*srcptr]++;  
-                 *imgptr = ((*srcptr>>5)<<8)|(*srcptr);  
-                 imgptr++;  
-                 srcptr++;  
-             }  
-             *imgptr = -1;  
-             imgptr += cpt_1;  
-             srcptr += srccpt;  
-         }  
-     }  
-     //为图像矩阵的最后一行赋值  
-     for ( int i = 0; i < src->cols+2; i++ )  
-     {  
-         *imgptr = -1;  
-         imgptr++;  
-     }  
-     //定义边界像素堆的大小  
-     //heap_cur[]对应灰度值，heap_cur[][]对应该灰度值的个数  
-     //根据灰度值的个数定义heap_cur的长度  
-     heap_cur[0][0] = 0;  
-     for ( int i = 1; i < 256; i++ )  
-     {  
-         heap_cur[i] = heap_cur[i-1]+level_size[i-1]+1;  
-         heap_cur[i][0] = 0;  
-     }  
-     //返回在图像矩阵中第一个真正的像素点的地址  
-     return startptr;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
static int* preprocessMSER_8UC1( CvMat* img,
            int*** heap_cur,
            CvMat* src,
            CvMat* mask )
{
    int srccpt = src->step-src->cols;
    //由于图像矩阵的宽经过了2^N处理，所以它的宽比原图的宽要长，cpt_1代表两个宽度的差值
    int cpt_1 = img->cols-src->cols-1;
    //图像矩阵的地址指针
    int* imgptr = img->data.i;
    //定义真实的图像起始地址
    int* startptr;

    //定义并初始化灰度值数组
    int level_size[256];
    for ( int i = 0; i < 256; i++ )
        level_size[i] = 0;
    //为图像矩阵的第一行赋值
    for ( int i = 0; i < src->cols+2; i++ )
    {
        *imgptr = -1;
        imgptr++;
    }
    //地址指针指向图像矩阵的第二行
    imgptr += cpt_1-1;
    //原图首地址指针
    uchar* srcptr = src->data.ptr;
    if ( mask )    //如果定义了掩码矩阵
    {
        startptr = 0;
        //掩码矩阵首地址指针
        uchar* maskptr = mask->data.ptr;
        //遍历整个原图
        for ( int i = 0; i < src->rows; i++ )
        {
            *imgptr = -1;
            imgptr++;
            for ( int j = 0; j < src->cols; j++ )
            {
                if ( *maskptr )
                {
                    if ( !startptr )
                        startptr = imgptr;    //赋值
                    *srcptr = 0xff-*srcptr;    //反转图像的灰度值
                    level_size[*srcptr]++;    //为灰度值计数
                    //为图像矩阵赋值，它的低8位是原图灰度值，8～10位是灰度值的高3位
                    *imgptr = ((*srcptr>>5)<<8)|(*srcptr);
                } else {    //掩码覆盖的像素值为-1
                    *imgptr = -1;
                }
                //地址加1
                imgptr++;
                srcptr++;
                maskptr++;
            }
            //跳过图像矩阵比原图多出的部分
            *imgptr = -1;
            imgptr += cpt_1;
            srcptr += srccpt;
            maskptr += srccpt;
        }
    } else {    //没有定义掩码
        startptr = imgptr+img->cols+1;    //赋值
        //遍历整幅图像，为图像矩阵赋值
        for ( int i = 0; i < src->rows; i++ )
        {
            *imgptr = -1;
            imgptr++;
            for ( int j = 0; j < src->cols; j++ )
            {
                *srcptr = 0xff-*srcptr;
                level_size[*srcptr]++;
                *imgptr = ((*srcptr>>5)<<8)|(*srcptr);
                imgptr++;
                srcptr++;
            }
            *imgptr = -1;
            imgptr += cpt_1;
            srcptr += srccpt;
        }
    }
    //为图像矩阵的最后一行赋值
    for ( int i = 0; i < src->cols+2; i++ )
    {
        *imgptr = -1;
        imgptr++;
    }
    //定义边界像素堆的大小
    //heap_cur[]对应灰度值，heap_cur[][]对应该灰度值的个数
    //根据灰度值的个数定义heap_cur的长度
    heap_cur[0][0] = 0;
    for ( int i = 1; i < 256; i++ )
    {
        heap_cur[i] = heap_cur[i-1]+level_size[i-1]+1;
        heap_cur[i][0] = 0;
    }
    //返回在图像矩阵中第一个真正的像素点的地址
    return startptr;
}
```

extractMSER_8UC1_Pass函数执行MSER算法，MSER-和MSER+都执行该函数，但可以通过参数color来区分。





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- static void extractMSER_8UC1_Pass( int* ioptr,  
-               int* imgptr,  
-               int*** heap_cur,  
-               LinkedPoint* ptsptr,  
-               MSERGrowHistory* histptr,  
-               MSERConnectedComp* comptr,  
-               int step,  
-               int stepmask,  
-               int stepgap,  
-               MSERParams params,  
-               int color,  
-               CvSeq* contours,  
-               CvMemStorage* storage )  
- {  
-     //设置第一个组块的灰度值为256，该灰度值是真实图像中不存在的灰度值，以区分真实图像的组块，从而判断程序是否结束  
-     comptr->grey_level = 256;  
-     //步骤2和步骤3  
-     //指向第二个组块  
-     comptr++;  
-     //设置第二个组块为输入图像第一个像素（左上角）的灰度值  
-     comptr->grey_level = (*imgptr)&0xff;  
-     //初始化该组块  
-     initMSERComp( comptr );  
-     //在最高位标注该像素为已被访问过，即该值小于0  
-     *imgptr |= 0x80000000;  
-     //得到该像素所对应的堆，即指向它所对应的灰度值  
-     heap_cur += (*imgptr)&0xff;  
-     //定义方向，即偏移量，因为是4邻域，所以该数组分别对应右、下、左、上  
-     int dir[] = { 1, step, -1, -step };  
- #ifdef __INTRIN_ENABLED__  
-     unsigned long heapbit[] = { 0, 0, 0, 0, 0, 0, 0, 0 };  
-     unsigned long* bit_cur = heapbit+(((*imgptr)&0x700)>>8);  
- #endif  
-     //死循环，退出该死循环的条件有两个：一是到达组块的栈底；二是边界像素堆中没有任何值。达到栈底也就意味着堆中没有值，在此函数中两者是一致的。  
-     for ( ; ; )  
-     {  
-         // take tour of all the 4 directions  
-         //步骤4  
-         //在4邻域内进行搜索  
-         while ( ((*imgptr)&0x70000) < 0x40000 )  
-         {  
-             // get the neighbor  
-             /* ((*imgptr)&0x70000)>>16得到第16位至第18位数据，该数据对应的4邻域的方向，再通过dir数组得到4邻域的偏移量，因此imgptr_nbr为当前像素4邻域中某一个方向上邻域的地址指针 */  
-             int* imgptr_nbr = imgptr+dir[((*imgptr)&0x70000)>>16];  
-             //检查邻域像素是否被访问过，如果被访问过，则会在第一位置1，因此该值会小于0，否则第一位为0，该值大于0  
-             if ( *imgptr_nbr >= 0 ) // if the neighbor is not visited yet  
-             {  
-                 //标注该像素已被访问过，即把第一位置1  
-                 *imgptr_nbr |= 0x80000000; // mark it as visited  
-                 //比较当前像素与邻域像素灰度值  
-                 if ( ((*imgptr_nbr)&0xff) < ((*imgptr)&0xff) )  
-                 {  
-                     //如果邻域值小于当前值，把当前值放入堆中  
-                     // when the value of neighbor smaller than current  
-                     // push current to boundary heap and make the neighbor to be the current one  
-                     // create an empty comp  
-                     //堆中该像素灰度值的数量加1，即对该灰度值像素个数计数  
-                     (*heap_cur)++;  
-                     //把当前值的地址放入堆中  
-                     **heap_cur = imgptr;  
-                     //重新标注当前值的方向位，以备下一次访问该值时搜索下一个邻域  
-                     *imgptr += 0x10000;  
-                     //定位邻域值所对应的堆的位置  
-                     //当前heap_cur所指向的灰度值为while循环搜索中的最小灰度值，即水溢过的最低点  
-                     heap_cur += ((*imgptr_nbr)&0xff)-((*imgptr)&0xff);  
- #ifdef __INTRIN_ENABLED__  
-                     _bitset( bit_cur, (*imgptr)&0x1f );  
-                     bit_cur += (((*imgptr_nbr)&0x700)-((*imgptr)&0x700))>>8;  
- #endif  
-                     imgptr = imgptr_nbr;    //邻域值换为当前值  
-                     //步骤3  
-                     comptr++;    //创建一个组块  
-                     initMSERComp( comptr );    //初始化该组块  
-                     comptr->grey_level = (*imgptr)&0xff;    //为该组块的灰度值赋值  
-                     //当某个邻域值小于当前值，则不对当前值再做任何操作，继续下次循环，在下次循环中，处理的则是该邻域值，即再次执行步骤4  
-                     continue;  
-                 } else {  
-                     //如果邻域值大于当前值，把邻域值放入堆中  
-                     // otherwise, push the neighbor to boundary heap  
-                     //找到该邻域值在堆中的灰度值位置，并对其计数，即对该灰度值像素个数计数   
-                     heap_cur[((*imgptr_nbr)&0xff)-((*imgptr)&0xff)]++;  
-                     //把该邻域像素地址放入堆中  
-                     *heap_cur[((*imgptr_nbr)&0xff)-((*imgptr)&0xff)] = imgptr_nbr;  
- #ifdef __INTRIN_ENABLED__  
-                     _bitset( bit_cur+((((*imgptr_nbr)&0x700)-((*imgptr)&0x700))>>8), (*imgptr_nbr)&0x1f );  
- #endif  
-                 }  
-             }  
-             *imgptr += 0x10000;    //重新标注当前值的领域方向  
-         }  
-         //imsk表示结束while循环后所得到的最后像素地址与图像首地址的相对距离  
-         int imsk = (int)(imgptr-ioptr);  
-         //得到结束while循环后的最后像素的坐标位置  
-         //从这里可以看出图像的宽采样2^N的好处，即imsk>>stepgap  
-         ptsptr->pt = cvPoint( imsk&stepmask, imsk>>stepgap );  
-         // get the current location  
-         //步骤5  
-         //对栈顶的组块的像素个数累加，即计算组块的面积大小，并链接组块内的像素点  
-         //结束while循环后，栈顶组块的灰度值就是该次循环后得到的最小灰度值，也就是该组块为极低点，就相当于水已经流到了最低的位置  
-         accumulateMSERComp( comptr, ptsptr );  
-         //指向下一个像素点链表位置  
-         ptsptr++;  
-         // get the next pixel from boundary heap  
-         //步骤6  
-         /*结束while循环后，如果**heap_cur有值的话，heap_cur指向的应该是while循环中得到的灰度值最小值，也就是在组块的边界像素中，有与组块相同的灰度值，因此要把该值作为当前值继续while循环，也就是相当于组块面积的扩展*/  
-         if ( **heap_cur )    //有值  
-         {  
-             imgptr = **heap_cur;    //把该像素点作为当前值  
-             (*heap_cur)--;    //像素的个数要相应的减1  
- #ifdef __INTRIN_ENABLED__  
-             if ( !**heap_cur )  
-                 _bitreset( bit_cur, (*imgptr)&0x1f );  
- #endif  
-         //步骤7  
-         //已经找到了最小灰度值的组块，并且边界像素堆中的灰度值都比组块的灰度值大，则这时需要组块，即计算最大稳定极值区域  
-         } else {  
- #ifdef __INTRIN_ENABLED__  
-             bool found_pixel = 0;  
-             unsigned long pixel_val;  
-             for ( int i = ((*imgptr)&0x700)>>8; i < 8; i++ )  
-             {  
-                 if ( _BitScanForward( &pixel_val, *bit_cur ) )  
-                 {  
-                     found_pixel = 1;  
-                     pixel_val += i<<5;  
-                     heap_cur += pixel_val-((*imgptr)&0xff);  
-                     break;  
-                 }  
-                 bit_cur++;  
-             }  
-             if ( found_pixel )  
- #else  
-             heap_cur++;    //指向高一级的灰度值  
-             unsigned long pixel_val = 0;  
-             //在边界像素堆中，找到边界像素中的最小灰度值  
-             for ( unsigned long i = ((*imgptr)&0xff)+1; i < 256; i++ )  
-             {  
-                 if ( **heap_cur )  
-                 {  
-                     pixel_val = i;    //灰度值  
-                     break;  
-                 }  
-                 //定位在堆中所对应的灰度值，与pixel_val是相等的  
-                 heap_cur++;   
-             }  
-             if ( pixel_val )    //如果找到了像素值  
- #endif  
-             {  
-                 imgptr = **heap_cur;    //从堆中提取出该像素  
-                 (*heap_cur)--;    //对应的像素个数减1  
- #ifdef __INTRIN_ENABLED__  
-                 if ( !**heap_cur )  
-                     _bitreset( bit_cur, pixel_val&0x1f );  
- #endif  
-                 //进入处理栈子模块  
-                 if ( pixel_val < comptr[-1].grey_level )  
-                 //如果从堆中提取出的最小灰度值小于距栈顶第二个组块的灰度值，则说明栈顶组块和第二个组块之间仍然有没有处理过的组块，因此在计算完MSER值后还要继续返回步骤4搜索该组块  
-                 {  
-                     // check the stablity and push a new history, increase the grey level  
-                     //利用公式2计算栈顶组块的q(i)值  
-                     if ( MSERStableCheck( comptr, params ) )    //是MSER  
-                     {  
-                         //得到组块内的像素点  
-                         CvContour* contour = MSERToContour( comptr, storage );  
-                         contour->color = color;    //标注是MSER-还是MSER+  
-                         //把组块像素点放入序列中  
-                         cvSeqPush( contours, &contour );   
-                     }  
-                     MSERNewHistory( comptr, histptr );  
-                     //改变栈顶组块的灰度值，这样就可以和上一层的组块进行合并  
-                     comptr[0].grey_level = pixel_val;  
-                     histptr++;  
-                 } else {  
-                     //从堆中提取出的最小灰度值大于等于距栈顶第二个组块的灰度值  
-                     // keep merging top two comp in stack until the grey level >= pixel_val  
-                     //死循环，用于处理灰度值相同并且相连的组块之间的合并  
-                     for ( ; ; )  
-                     {  
-                         //指向距栈顶第二个组块  
-                         comptr--;  
-                         //合并前两个组块，并把合并后的组块作为栈顶组块  
-                         MSERMergeComp( comptr+1, comptr, comptr, histptr );  
-                         histptr++;  
-                         /*如果pixel_val = comptr[0].grey_level，说明在边界上还有属于该组块的像素；如果pixel_val < comptr[0].grey_level，说明还有比栈顶组块灰度值更小的组块没有搜索到。这两种情况都需要回到步骤4中继续搜索组块*/  
-                         if ( pixel_val <= comptr[0].grey_level )  
-                             break;  
-                         //合并栈内前两个组块，直到pixel_val < comptr[-1].grey_level为止  
-                         if ( pixel_val < comptr[-1].grey_level )  
-                         {  
-                             // check the stablity here otherwise it wouldn't be an ER  
-                             if ( MSERStableCheck( comptr, params ) )  
-                             {  
-                                 CvContour* contour = MSERToContour( comptr, storage );  
-                                 contour->color = color;  
-                                 cvSeqPush( contours, &contour );  
-                             }  
-                             MSERNewHistory( comptr, histptr );  
-                             comptr[0].grey_level = pixel_val;  
-                             histptr++;  
-                             break;  
-                         }  
-                     }  
-                 }  
-             } else  
-                 //边界像素堆中没有任何像素，则退出死循环，该函数返回。  
-                 break;  
-         }  
-     }  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
static void extractMSER_8UC1_Pass( int* ioptr,
              int* imgptr,
              int*** heap_cur,
              LinkedPoint* ptsptr,
              MSERGrowHistory* histptr,
              MSERConnectedComp* comptr,
              int step,
              int stepmask,
              int stepgap,
              MSERParams params,
              int color,
              CvSeq* contours,
              CvMemStorage* storage )
{
    //设置第一个组块的灰度值为256，该灰度值是真实图像中不存在的灰度值，以区分真实图像的组块，从而判断程序是否结束
    comptr->grey_level = 256;
    //步骤2和步骤3
    //指向第二个组块
    comptr++;
    //设置第二个组块为输入图像第一个像素（左上角）的灰度值
    comptr->grey_level = (*imgptr)&0xff;
    //初始化该组块
    initMSERComp( comptr );
    //在最高位标注该像素为已被访问过，即该值小于0
    *imgptr |= 0x80000000;
    //得到该像素所对应的堆，即指向它所对应的灰度值
    heap_cur += (*imgptr)&0xff;
    //定义方向，即偏移量，因为是4邻域，所以该数组分别对应右、下、左、上
    int dir[] = { 1, step, -1, -step };
#ifdef __INTRIN_ENABLED__
    unsigned long heapbit[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned long* bit_cur = heapbit+(((*imgptr)&0x700)>>8);
#endif
    //死循环，退出该死循环的条件有两个：一是到达组块的栈底；二是边界像素堆中没有任何值。达到栈底也就意味着堆中没有值，在此函数中两者是一致的。
    for ( ; ; )
    {
        // take tour of all the 4 directions
        //步骤4
        //在4邻域内进行搜索
        while ( ((*imgptr)&0x70000) < 0x40000 )
        {
            // get the neighbor
            /* ((*imgptr)&0x70000)>>16得到第16位至第18位数据，该数据对应的4邻域的方向，再通过dir数组得到4邻域的偏移量，因此imgptr_nbr为当前像素4邻域中某一个方向上邻域的地址指针 */
            int* imgptr_nbr = imgptr+dir[((*imgptr)&0x70000)>>16];
            //检查邻域像素是否被访问过，如果被访问过，则会在第一位置1，因此该值会小于0，否则第一位为0，该值大于0
            if ( *imgptr_nbr >= 0 ) // if the neighbor is not visited yet
            {
                //标注该像素已被访问过，即把第一位置1
                *imgptr_nbr |= 0x80000000; // mark it as visited
                //比较当前像素与邻域像素灰度值
                if ( ((*imgptr_nbr)&0xff) < ((*imgptr)&0xff) )
                {
                    //如果邻域值小于当前值，把当前值放入堆中
                    // when the value of neighbor smaller than current
                    // push current to boundary heap and make the neighbor to be the current one
                    // create an empty comp
                    //堆中该像素灰度值的数量加1，即对该灰度值像素个数计数
                    (*heap_cur)++;
                    //把当前值的地址放入堆中
                    **heap_cur = imgptr;
                    //重新标注当前值的方向位，以备下一次访问该值时搜索下一个邻域
                    *imgptr += 0x10000;
                    //定位邻域值所对应的堆的位置
                    //当前heap_cur所指向的灰度值为while循环搜索中的最小灰度值，即水溢过的最低点
                    heap_cur += ((*imgptr_nbr)&0xff)-((*imgptr)&0xff);
#ifdef __INTRIN_ENABLED__
                    _bitset( bit_cur, (*imgptr)&0x1f );
                    bit_cur += (((*imgptr_nbr)&0x700)-((*imgptr)&0x700))>>8;
#endif
                    imgptr = imgptr_nbr;    //邻域值换为当前值
                    //步骤3
                    comptr++;    //创建一个组块
                    initMSERComp( comptr );    //初始化该组块
                    comptr->grey_level = (*imgptr)&0xff;    //为该组块的灰度值赋值
                    //当某个邻域值小于当前值，则不对当前值再做任何操作，继续下次循环，在下次循环中，处理的则是该邻域值，即再次执行步骤4
                    continue;
                } else {
                    //如果邻域值大于当前值，把邻域值放入堆中
                    // otherwise, push the neighbor to boundary heap
                    //找到该邻域值在堆中的灰度值位置，并对其计数，即对该灰度值像素个数计数 
                    heap_cur[((*imgptr_nbr)&0xff)-((*imgptr)&0xff)]++;
                    //把该邻域像素地址放入堆中
                    *heap_cur[((*imgptr_nbr)&0xff)-((*imgptr)&0xff)] = imgptr_nbr;
#ifdef __INTRIN_ENABLED__
                    _bitset( bit_cur+((((*imgptr_nbr)&0x700)-((*imgptr)&0x700))>>8), (*imgptr_nbr)&0x1f );
#endif
                }
            }
            *imgptr += 0x10000;    //重新标注当前值的领域方向
        }
        //imsk表示结束while循环后所得到的最后像素地址与图像首地址的相对距离
        int imsk = (int)(imgptr-ioptr);
        //得到结束while循环后的最后像素的坐标位置
        //从这里可以看出图像的宽采样2^N的好处，即imsk>>stepgap
        ptsptr->pt = cvPoint( imsk&stepmask, imsk>>stepgap );
        // get the current location
        //步骤5
        //对栈顶的组块的像素个数累加，即计算组块的面积大小，并链接组块内的像素点
        //结束while循环后，栈顶组块的灰度值就是该次循环后得到的最小灰度值，也就是该组块为极低点，就相当于水已经流到了最低的位置
        accumulateMSERComp( comptr, ptsptr );
        //指向下一个像素点链表位置
        ptsptr++;
        // get the next pixel from boundary heap
        //步骤6
        /*结束while循环后，如果**heap_cur有值的话，heap_cur指向的应该是while循环中得到的灰度值最小值，也就是在组块的边界像素中，有与组块相同的灰度值，因此要把该值作为当前值继续while循环，也就是相当于组块面积的扩展*/
        if ( **heap_cur )    //有值
        {
            imgptr = **heap_cur;    //把该像素点作为当前值
            (*heap_cur)--;    //像素的个数要相应的减1
#ifdef __INTRIN_ENABLED__
            if ( !**heap_cur )
                _bitreset( bit_cur, (*imgptr)&0x1f );
#endif
        //步骤7
        //已经找到了最小灰度值的组块，并且边界像素堆中的灰度值都比组块的灰度值大，则这时需要组块，即计算最大稳定极值区域
        } else {
#ifdef __INTRIN_ENABLED__
            bool found_pixel = 0;
            unsigned long pixel_val;
            for ( int i = ((*imgptr)&0x700)>>8; i < 8; i++ )
            {
                if ( _BitScanForward( &pixel_val, *bit_cur ) )
                {
                    found_pixel = 1;
                    pixel_val += i<<5;
                    heap_cur += pixel_val-((*imgptr)&0xff);
                    break;
                }
                bit_cur++;
            }
            if ( found_pixel )
#else
            heap_cur++;    //指向高一级的灰度值
            unsigned long pixel_val = 0;
            //在边界像素堆中，找到边界像素中的最小灰度值
            for ( unsigned long i = ((*imgptr)&0xff)+1; i < 256; i++ )
            {
                if ( **heap_cur )
                {
                    pixel_val = i;    //灰度值
                    break;
                }
                //定位在堆中所对应的灰度值，与pixel_val是相等的
                heap_cur++; 
            }
            if ( pixel_val )    //如果找到了像素值
#endif
            {
                imgptr = **heap_cur;    //从堆中提取出该像素
                (*heap_cur)--;    //对应的像素个数减1
#ifdef __INTRIN_ENABLED__
                if ( !**heap_cur )
                    _bitreset( bit_cur, pixel_val&0x1f );
#endif
                //进入处理栈子模块
                if ( pixel_val < comptr[-1].grey_level )
                //如果从堆中提取出的最小灰度值小于距栈顶第二个组块的灰度值，则说明栈顶组块和第二个组块之间仍然有没有处理过的组块，因此在计算完MSER值后还要继续返回步骤4搜索该组块
                {
                    // check the stablity and push a new history, increase the grey level
                    //利用公式2计算栈顶组块的q(i)值
                    if ( MSERStableCheck( comptr, params ) )    //是MSER
                    {
                        //得到组块内的像素点
                        CvContour* contour = MSERToContour( comptr, storage );
                        contour->color = color;    //标注是MSER-还是MSER+
                        //把组块像素点放入序列中
                        cvSeqPush( contours, &contour ); 
                    }
                    MSERNewHistory( comptr, histptr );
                    //改变栈顶组块的灰度值，这样就可以和上一层的组块进行合并
                    comptr[0].grey_level = pixel_val;
                    histptr++;
                } else {
                    //从堆中提取出的最小灰度值大于等于距栈顶第二个组块的灰度值
                    // keep merging top two comp in stack until the grey level >= pixel_val
                    //死循环，用于处理灰度值相同并且相连的组块之间的合并
                    for ( ; ; )
                    {
                        //指向距栈顶第二个组块
                        comptr--;
                        //合并前两个组块，并把合并后的组块作为栈顶组块
                        MSERMergeComp( comptr+1, comptr, comptr, histptr );
                        histptr++;
                        /*如果pixel_val = comptr[0].grey_level，说明在边界上还有属于该组块的像素；如果pixel_val < comptr[0].grey_level，说明还有比栈顶组块灰度值更小的组块没有搜索到。这两种情况都需要回到步骤4中继续搜索组块*/
                        if ( pixel_val <= comptr[0].grey_level )
                            break;
                        //合并栈内前两个组块，直到pixel_val < comptr[-1].grey_level为止
                        if ( pixel_val < comptr[-1].grey_level )
                        {
                            // check the stablity here otherwise it wouldn't be an ER
                            if ( MSERStableCheck( comptr, params ) )
                            {
                                CvContour* contour = MSERToContour( comptr, storage );
                                contour->color = color;
                                cvSeqPush( contours, &contour );
                            }
                            MSERNewHistory( comptr, histptr );
                            comptr[0].grey_level = pixel_val;
                            histptr++;
                            break;
                        }
                    }
                }
            } else
                //边界像素堆中没有任何像素，则退出死循环，该函数返回。
                break;
        }
    }
}
```



MSER就分析到这里，至于其中用到的一些子函数就不再分析。这里还要说明一点的是，MSER+和MSER-使用的都是同一个函数，两者的区别是一个组块面积增加，另一个减少。说实话，该部分的内容数据结构较为复杂，有一些内容我理解得也不透彻，比如结构MSERGrowHistory真正的含义还比较模糊。

下面就给出最大稳定极值区域检测的应用实例：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/40742191#)[copy](http://blog.csdn.net/zhaocj/article/details/40742191#)[print](http://blog.csdn.net/zhaocj/article/details/40742191#)[?](http://blog.csdn.net/zhaocj/article/details/40742191#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- #include "opencv2/core/core.hpp"  
- #include "opencv2/highgui/highgui.hpp"  
- #include "opencv2/imgproc/imgproc.hpp"  
- #include <opencv2/features2d/features2d.hpp>  
- #include <iostream>  
- using namespace cv;  
- using namespace std;  
- 
- int main(int argc, char *argv[])  
- {  
-     Mat src,gray;  
-     src = imread("lenna.bmp");  
-     cvtColor( src, gray, CV_BGR2GRAY );  
-     //创建MSER类  
-     MSER ms;  
-     //用于组块区域的像素点集  
-     vector<vector<Point>> regions;  
-     ms(gray, regions, Mat());  
-     //在灰度图像中用椭圆形绘制组块  
-     for (int i = 0; i < regions.size(); i++)  
-     {  
-         ellipse(gray, fitEllipse(regions[i]), Scalar(255));  
-     }  
-     imshow("mser", gray);  
-     waitKey(0);  
-     return 0;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat src,gray;
	src = imread("lenna.bmp");
	cvtColor( src, gray, CV_BGR2GRAY );
    //创建MSER类
    MSER ms;
    //用于组块区域的像素点集
    vector<vector<Point>> regions;
    ms(gray, regions, Mat());
    //在灰度图像中用椭圆形绘制组块
    for (int i = 0; i < regions.size(); i++)
    {
        ellipse(gray, fitEllipse(regions[i]), Scalar(255));
    }
    imshow("mser", gray);
    waitKey(0);
    return 0;
}
```

结果如下图所示。本文介绍的方法只适用于灰度图像，所以如果输入的是彩色图像，要把它转换为灰度图像。另外在创建MSER类的时候，我们使用的是缺省构造函数，因此MSER算法所需的参数是系统默认的。所以我们检测的结果与David Nister等人在原著中的检测结果略有不同。



![](https://img-blog.csdn.net/20141103160829061)





