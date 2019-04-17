# 人脸检测（三）--Haar特征原理及实现 - 工作笔记 - CSDN博客





2016年05月20日 14:55:37[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7532
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









本文主要由于OpenCV的haartraining程序，对haar特征的补充及代码注释。

原文：[http://www.aiuxian.com/article/p-2476165.html](http://www.aiuxian.com/article/p-2476165.html)

**Haar特征的原理是什么？**



Haar特征分为三类：边缘特征、线性特征、中心特征和对角线特征，组合成特征模板。特征模板内有白色和黑色两种矩形，并定义该模板的特征值为白色矩形像素和减去黑色矩形像素和（在opencv实现中为黑色-白色）。Haar特征值反映了图像的灰度变化情况。例如：脸部的一些特征能由矩形特征简单的描述，如：眼睛要比脸颊颜色要深，鼻梁两侧比鼻梁颜色要深，嘴巴比周围颜色要深等。但矩形特征只对一些简单的图形结构，如边缘、线段较敏感，所以只能描述特定走向（水平、垂直、对角）的结构。（**本段文字及下面两幅图引用自**[http://www.aiuxian.com/article/p-1897852.html](http://www.aiuxian.com/article/p-1897852.html)）

Viola提出的haar特征：



![](http://img.aiuxian.com/tech/000/001/891/323_fc9_427.jpg)

图片分享：







Lienhart等牛们提出的Haar-like特征：



![](http://img.aiuxian.com/tech/000/001/891/324_3c3_2fd.jpg)

图片分享：







矩形特征可位于图像任意位置，大小也可以任意改变，所以矩形特征值是矩形模版类别、矩形位置和矩形大小这三个因素的函数，当然对于新提出的有旋转角度的haar特征，还要把旋转的因素考虑进去。

所以一个Haar特征的数据结构应该包含以下内容：

*haar特征模板类型

*是否有旋转

*矩阵位置及大小



**CvIntHaarFeatures是如何构成的？**

在Opencv中，我们用CvTHaarFeature和CvFastHaarFeature作为描述单个特征的数据结构，用CvIntHaarFeatures作为一个封装的类型，通过这个类型中的两个指针（分别是CvTHaarFeature*和CvFastHaarFeature*指针）可以间接遍寻到存储的所有的特征。下面来看下它们的具体构造








**CvTHaarFeature的数据结构：**

//CvTHaarFeature：由（至多三个）矩形表示特征位置

typedef struct CvTHaarFeature

{

    char desc[CV_HAAR_FEATURE_DESC_MAX];    //描述haar特征模板类型的变量

    int  tilted; //标识是否有旋转，通过desc字符数组开头是否为tilted判断

    struct

    {

        CvRect r;

        float weight;

    } rect[CV_HAAR_FEATURE_MAX];             //三个矩形来描述特征位置

} CvTHaarFeature;





**创建一个CvTHaarFeature特征：**

/*例：haarFeature = cvHaarFeature("tilted_haar_y2",

                                    x, y, dx,2*dy, -1,

                                    x, y,dx,   dy, +2 );*/

CV_INLINECvTHaarFeature cvHaarFeature(constchar* desc,

 int x0, int y0, int w0,int h0,float wt0,

 int x1, int y1, int w1,int h1,float wt1,

 int x2, int y2, int w2,int h2,float wt2 )

{

    CvTHaarFeature hf;



    assert( CV_HAAR_FEATURE_MAX >= 3 );

    assert( strlen( desc ) <CV_HAAR_FEATURE_DESC_MAX );



    strcpy( &(hf.desc[0]), desc );

    hf.tilted = ( hf.desc[0] ==  't' );



    hf.rect[0].r.x = x0;

    hf.rect[0].r.y = y0;

    hf.rect[0].r.width  = w0;

    hf.rect[0].r.height = h0;

    hf.rect[0].weight   = wt0;



    hf.rect[1].r.x = x1;

    hf.rect[1].r.y = y1;

    hf.rect[1].r.width  = w1;

    hf.rect[1].r.height = h1;

    hf.rect[1].weight   = wt1;



    hf.rect[2].r.x = x2;

    hf.rect[2].r.y = y2;

    hf.rect[2].r.width  = w2;

    hf.rect[2].r.height = h2;

    hf.rect[2].weight   = wt2;



return hf;

}





**CvFastHaarFeature的数据结构：**

//与CvTHaarFeature类似，不同的是通过4个点来描述特征矩形的位置大小信息

typedef struct CvFastHaarFeature

{

    int tilted;

    struct

    {

        int p0, p1, p2, p3;

        float weight;

    } rect[CV_HAAR_FEATURE_MAX];

} CvFastHaarFeature;



**CvIntHaarFeatures的数据结构：**

typedef struct CvIntHaarFeatures

{

    CvSize winsize;

    int count;

    CvTHaarFeature* feature;

    CvFastHaarFeature* fastfeature;

} CvIntHaarFeatures;




**了解了如何构成，我们就来创建，icvCreateIntHaarFeatures（）方法的具体实现：**



接下来就是最重要的一步，如何创建我们想要得到的所有特征信息及CvIntHaarFeatures，下面是icvCreateIntHaarFeatures方法的具体实现和详细注释

由于opencv和C++都是初学，用了很长时间写了大量注释，0基础也绝对能看懂，希望能对大家有帮助


||/** icvCreateIntHaarFeatures** Create internal representation of haar features** mode:* 0 - BASIC = Viola提出的原始举行特征* 1 - CORE = All upright 所有垂直的haar特征* 2 - ALL = All features 所有haar特征*symmetric: 目标图形是否为垂直对称*/staticCvIntHaarFeatures* icvCreateIntHaarFeatures( CvSize winsize,int mode,int symmetric ){CvIntHaarFeatures* features = NULL;CvTHaarFeature haarFeature;/*内存存储器是一个可用来存储诸如序列,轮廓,图形,子划分等动态增长数据结构的底层结构。它是由一系列以同等大小的内存块构成,呈列表型*/CvMemStorage* storage = NULL;CvSeq* seq = NULL;CvSeqWriter writer;int s0 = 36; /* minimum total area size of basic haar feature */int s1 = 12; /* minimum total area size of tilted（倾斜的） haar features 2 */int s2 = 18; /* minimum total area size of tilted haar features 3 */int s3 = 24; /* minimum total area size of tilted haar features 4 */int x = 0;int y = 0;int dx = 0;int dy = 0;#if 0float factor = 1.0F;factor = ((float) winsize.width) * winsize.height / (24 * 24);s0 = (int) (s0 * factor);s1 = (int) (s1 * factor);s2 = (int) (s2 * factor);s3 = (int) (s3 * factor);#else//程序必然走这边，为什么这么写？s0 = 1;s1 = 1;s2 = 1;s3 = 1;#endif/* CV_VECTOR_CREATE( vec, CvIntHaarFeature, size, maxsize ) */storage = cvCreateMemStorage();//功能：创建新序列，并初始化写入部分/*我的理解：这里其实是定义了writer工具每次写入数据的大小，以及写入到哪个内存存储器在之后调用 CV_WRITE_SEQ_ELEM( haarFeature, writer )时就可以自动将一个haarFeature类型的数据写入内存存储器中*/cvStartWriteSeq( 0, sizeof( CvSeq ), sizeof( haarFeature ), storage, &writer );/*矩形特征可位于图像任意位置，大小也可以任意改变，所以矩形特征值是矩形模版类别、矩形位置和矩形大小这三个因素的函数*/for( x = 0; x < winsize.width; x++ ){for( y = 0; y < winsize.height; y++ ){//x，y确定了特征矩形的左上角坐标for( dx = 1; dx <= winsize.width; dx++ ){for( dy = 1; dy <= winsize.height; dy++ ){//dx，dy确定了特征矩形的大小//下面需要按照不同的特征模板类型分别讨论，在模板不越界的情况下，添加该特征// haar_x2 对应上图中的（a）特征模板，黑色为+，白色为-if ( (x+dx*2 <= winsize.width) && (y+dy <= winsize.height) ) {if (dx*2*dy < s0) continue;if (!symmetric || (x+x+dx*2 <=winsize.width)){//目标图像不为垂直对称或目标垂直对称但满足上式条件//若目标不垂直对称，显然要计算当前矩形特征的特征值//若对称，则只计算左半部分全部位于标准样本左半边的矩形特征的特征值haarFeature = cvHaarFeature( "haar_x2",x, y, dx*2, dy, -1,x+dx, y, dx , dy, +2 );/* CV_VECTOR_PUSH( vec, CvIntHaarFeature, haarFeature, size, maxsize, step ) */CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// haar_y2 对应上图中的（b）特征模板if ( (x+dx <= winsize.width) && (y+dy*2 <= winsize.height) ) {if (dx*2*dy < s0) continue;if (!symmetric || (x+x+dx <= winsize.width)) {haarFeature = cvHaarFeature( "haar_y2",x, y, dx, dy*2, -1,x, y+dy, dx, dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// haar_x3 对应上图中的（c）特征模板if ( (x+dx*3 <= winsize.width) && (y+dy <= winsize.height) ) {if (dx*3*dy < s0) continue;if (!symmetric || (x+x+dx*3 <=winsize.width)) {haarFeature = cvHaarFeature( "haar_x3",x, y, dx*3, dy, -1,x+dx, y, dx, dy, +3 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// haar_y3 对应上图中的（d）特征模板if ( (x+dx <= winsize.width) && (y+dy*3 <= winsize.height) ) {if (dx*3*dy < s0) continue;if (!symmetric || (x+x+dx <= winsize.width)) {haarFeature = cvHaarFeature( "haar_y3",x, y, dx, dy*3, -1,x, y+dy, dx, dy, +3 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}if( mode != 0 /*BASIC*/ ) {// haar_x4 对应上图中的（2b）特征模板if ( (x+dx*4 <= winsize.width) && (y+dy <= winsize.height) ) {if (dx*4*dy < s0) continue;if (!symmetric || (x+x+dx*4 <=winsize.width)) {haarFeature = cvHaarFeature( "haar_x4",x, y, dx*4, dy, -1,x+dx, y, dx*2, dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// haar_y4 对应上图中的（2d）特征模板if ( (x+dx <= winsize.width ) && (y+dy*4 <= winsize.height) ) {if (dx*4*dy < s0) continue;if (!symmetric || (x+x+dx <=winsize.width)) {haarFeature = cvHaarFeature( "haar_y4",x, y, dx, dy*4, -1,x, y+dy, dx, dy*2, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}}// x2_y2 对应上图中的（e）特征模板if ( (x+dx*2 <= winsize.width) && (y+dy*2 <= winsize.height) ) {if (dx*4*dy < s0) continue;if (!symmetric || (x+x+dx*2 <=winsize.width)) {haarFeature = cvHaarFeature( "haar_x2_y2",x , y, dx*2, dy*2, -1,x , y , dx , dy, +2,x+dx, y+dy, dx , dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}if (mode != 0 /*BASIC*/) {// point 对应上图中的（3a）特征模板if ( (x+dx*3 <= winsize.width) && (y+dy*3 <= winsize.height) ) {if (dx*9*dy < s0) continue;if (!symmetric || (x+x+dx*3 <=winsize.width)) {haarFeature = cvHaarFeature( "haar_point",x , y, dx*3, dy*3, -1,x+dx, y+dy, dx , dy , +9);CV_WRITE_SEQ_ELEM( haarFeature, writer );}}}if (mode == 2 /*ALL*/) {// tilted haar_x2 (x, y, w, h, b, weight)//对应上图中的（1c）特征模板if ( (x+2*dx <= winsize.width) && (y+2*dx+dy <= winsize.height) && (x-dy>= 0) ) {if (dx*2*dy < s1) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_x2",x, y, dx*2, dy, -1,x, y, dx , dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// tilted haar_y2 (x, y, w, h, b, weight)//对应上图中的（1d）特征模板if ( (x+dx <= winsize.width) && (y+dx+2*dy <= winsize.height) && (x-2*dy>= 0) ) {if (dx*2*dy < s1) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_y2",x, y, dx, 2*dy, -1,x, y, dx, dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// tilted haar_x3 (x, y, w, h, b, weight)if ( (x+3*dx <= winsize.width) && (y+3*dx+dy <= winsize.height) && (x-dy>= 0) ) {if (dx*3*dy < s2) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_x3",x, y, dx*3, dy, -1,x+dx, y+dx, dx , dy, +3 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// tilted haar_y3 (x, y, w, h, b, weight)if ( (x+dx <= winsize.width) && (y+dx+3*dy <= winsize.height) && (x-3*dy>= 0) ) {if (dx*3*dy < s2) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_y3",x, y, dx, 3*dy, -1,x-dy, y+dy, dx, dy, +3 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// tilted haar_x4 (x, y, w, h, b, weight)if ( (x+4*dx <= winsize.width) && (y+4*dx+dy <= winsize.height) && (x-dy>= 0) ) {if (dx*4*dy < s3) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_x4",x, y, dx*4, dy, -1,x+dx, y+dx, dx*2, dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}// tilted haar_y4 (x, y, w, h, b, weight)if ( (x+dx <= winsize.width) && (y+dx+4*dy <= winsize.height) && (x-4*dy>= 0) ) {if (dx*4*dy < s3) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_y4",x, y, dx, 4*dy, -1,x-dy, y+dy, dx, 2*dy, +2 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}/*// tilted pointif ( (x+dx*3 <= winsize.width - 1) && (y+dy*3 <= winsize.height - 1) && (x-3*dy>= 0)) {if (dx*9*dy < 36) continue;if (!symmetric || (x <= (winsize.width / 2) )) {haarFeature = cvHaarFeature( "tilted_haar_point",x, y, dx*3, dy*3, -1,x, y+dy, dx , dy, +9 );CV_WRITE_SEQ_ELEM( haarFeature, writer );}}*/}}}}}/*我的理解：当前已经完成了数据的写入，但是是存储在内存存储器中的，调用此方法将存储器中的所有数据转移到cvSeq中*/seq = cvEndWriteSeq( &writer );在OpenCV中临时缓存用cvAlloc和cvFree函数分配和回收.函数应注意适当对齐,对未释放的内存保持跟踪，检查溢出。features = (CvIntHaarFeatures*) cvAlloc( sizeof( CvIntHaarFeatures ) +( sizeof( CvTHaarFeature ) + sizeof( CvFastHaarFeature ) ) * seq->total );features->feature = (CvTHaarFeature*) (features + 1);features->fastfeature = (CvFastHaarFeature*) ( features->feature + seq->total );features->count = seq->total;features->winsize = winsize;cvCvtSeqToArray( seq, (CvArr*) features->feature );cvReleaseMemStorage( &storage );//特征的rect由坐标表示转换为由像素索引表示icvConvertToFastHaarFeature( features->feature, features->fastfeature,features->count, (winsize.width + 1) );return features;}|
|----|----|



这边有一个新版分类器harr特征训练的解释。

[http://blog.csdn.net/beerbuddys/article/details/40712957](http://blog.csdn.net/beerbuddys/article/details/40712957)





