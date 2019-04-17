# 相机模型与标定（十）--RANSAC算法 - 工作笔记 - CSDN博客





2016年07月26日 15:49:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6758
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自王先荣先生：

http://www.cnblogs.com/xrwang/archive/2011/03/09/ransac-1.html

本文翻译自维基百科，英文原文地址是：http://en.wikipedia.org/wiki/ransac，如果您英语不错，建议您直接查看原文。

    RANSAC是“RANdom SAmple Consensus（随机抽样一致）”的缩写。它可以从一组包含“局外点”的观测数据集中，通过迭代方式估计数学模型的参数。它是一种不确定的算法——它有一定的概率得出一个合理的结果；为了提高概率必须提高迭代次数。该算法最早由Fischler和Bolles于1981年提出。

    RANSAC的基本假设是：

（1）数据由“局内点”组成，例如：数据的分布可以用一些模型参数来解释；

（2）“局外点”是不能适应该模型的数据；

（3）除此之外的数据属于噪声。

    局外点产生的原因有：噪声的极值；错误的测量方法；对数据的错误假设。

    RANSAC也做了以下假设：给定一组（通常很小的）局内点，存在一个可以估计模型参数的过程；而该模型能够解释或者适用于局内点。
**本文内容**

1 示例

2 概述

3 算法

4 参数

5 优点与缺点

6 应用

7 参考文献

8 外部链接

**一、示例**

    一个简单的例子是从一组观测数据中找出合适的2维直线。假设观测数据中包含局内点和局外点，其中局内点近似的被直线所通过，而局外点远离于直线。简单的最小二乘法不能找到适应于局内点的直线，原因是最小二乘法尽量去适应包括局外点在内的所有点。相反，RANSAC能得出一个仅仅用局内点计算出模型，并且概率还足够高。但是，RANSAC并不能保证结果一定正确，为了保证算法有足够高的合理概率，我们必须小心的选择算法的参数。
![](http://pic002.cnblogs.com/images/2011/21602/2011030818152013.png)![](http://pic002.cnblogs.com/images/2011/21602/2011030818153136.png)

左图：包含很多局外点的数据集       右图：RANSAC找到的直线（局外点并不影响结果）

**二、概述**

    RANSAC算法的输入是一组观测数据，一个可以解释或者适应于观测数据的参数化模型，一些可信的参数。

    RANSAC通过反复选择数据中的一组随机子集来达成目标。被选取的子集被假设为局内点，并用下述方法进行验证：

    1.有一个模型适应于假设的局内点，即所有的未知参数都能从假设的局内点计算得出。

    2.用1中得到的模型去测试所有的其它数据，如果某个点适用于估计的模型，认为它也是局内点。

    3.如果有足够多的点被归类为假设的局内点，那么估计的模型就足够合理。

    4.然后，用所有假设的局内点去重新估计模型，因为它仅仅被初始的假设局内点估计过。

    5.最后，通过估计局内点与模型的错误率来评估模型。

    这个过程被重复执行固定的次数，每次产生的模型要么因为局内点太少而被舍弃，要么因为比现有的模型更好而被选用。

**三、算法**

    伪码形式的算法如下所示：

输入：

data —— 一组观测数据

model —— 适应于数据的模型

n —— 适用于模型的最少数据个数

k —— 算法的迭代次数

t —— 用于决定数据是否适应于模型的阀值

d —— 判定模型是否适用于数据集的数据数目

输出：

best_model —— 跟数据最匹配的模型参数（如果没有找到好的模型，返回null）

best_consensus_set —— 估计出模型的数据点

best_error —— 跟数据相关的估计出的模型错误


iterations = 0

best_model = null

best_consensus_set = null

best_error = 无穷大

while ( iterations < k )

    maybe_inliers = 从数据集中随机选择n个点

    maybe_model = 适合于maybe_inliers的模型参数

    consensus_set = maybe_inliers


    for ( 每个数据集中不属于maybe_inliers的点 ）

        if ( 如果点适合于maybe_model，且错误小于t ）

            将点添加到consensus_set

    if （ consensus_set中的元素数目大于d ）

        已经找到了好的模型，现在测试该模型到底有多好

        better_model = 适合于consensus_set中所有点的模型参数

        this_error = better_model究竟如何适合这些点的度量

        if ( this_error < best_error )

            我们发现了比以前好的模型，保存该模型直到更好的模型出现

            best_model =  better_model

            best_consensus_set = consensus_set

            best_error =  this_error

    增加迭代次数

返回 best_model, best_consensus_set, best_error


    RANSAC算法的可能变化包括以下几种：

    （1）如果发现了一种足够好的模型（该模型有足够小的错误率），则跳出主循环。这样可能会节约计算额外参数的时间。

    （2）直接从maybe_model计算this_error，而不从consensus_set重新估计模型。这样可能会节约比较两种模型错误的时间，但可能会对噪声更敏感。

四、参数

    我们不得不根据特定的问题和数据集通过实验来确定参数t和d。然而参数k（迭代次数）可以从理论结果推断。当我们从估计模型参数时，用p表示一些迭代过程中从数据集内随机选取出的点均为局内点的概率；此时，结果模型很可能有用，因此p也表征了算法产生有用结果的概率。用w表示每次从数据集中选取一个局内点的概率，如下式所示：

    w = 局内点的数目 / 数据集的数目

    通常情况下，我们事先并不知道w的值，但是可以给出一些鲁棒的值。假设估计模型需要选定n个点，wn是所有n个点均为局内点的概率；1 −wn是n个点中至少有一个点为局外点的概率，此时表明我们从数据集中估计出了一个不好的模型。
 (1 −wn)k表示算法永远都不会选择到n个点均为局内点的概率，它和1-p相同。因此，
1 − p = (1 − wn)k

    我们对上式的两边取对数，得出
![](http://pic002.cnblogs.com/images/2011/21602/2011030818233619.png)

    值得注意的是，这个结果假设n个点都是独立选择的；也就是说，某个点被选定之后，它可能会被后续的迭代过程重复选定到。这种方法通常都不合理，由此推导出的k值被看作是选取不重复点的上限。例如，要从上图中的数据集寻找适合的直线，RANSAC算法通常在每次迭代时选取2个点，计算通过这两点的直线maybe_model，要求这两点必须唯一。

    为了得到更可信的参数，标准偏差或它的乘积可以被加到k上。k的标准偏差定义为：
![](http://pic002.cnblogs.com/images/2011/21602/2011030818234870.png)
五、优点与缺点

    RANSAC的优点是它能鲁棒的估计模型参数。例如，它能从包含大量局外点的数据集中估计出高精度的参数。RANSAC的缺点是它计算参数的迭代次数没有上限；如果设置迭代次数的上限，得到的结果可能不是最优的结果，甚至可能得到错误的结果。RANSAC只有一定的概率得到可信的模型，概率与迭代次数成正比。RANSAC的另一个缺点是它要求设置跟问题相关的阀值。

    RANSAC只能从特定的数据集中估计出一个模型，如果存在两个（或多个）模型，RANSAC不能找到别的模型。


六、应用

    RANSAC算法经常用于计算机视觉，例如同时求解相关问题与估计立体摄像机的基础矩阵。


七、参考文献- 
Martin A. Fischler and Robert C. Bolles (June 1981). "Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography".Comm. of the ACM24: 381–395.[](http://en.wikipedia.org/wiki/Digital_object_identifier)doi:[](http://dx.doi.org/10.1145%2F358669.358692)10.1145/358669.358692.- 
David A. Forsyth and Jean Ponce (2003). Computer Vision, a modern approach. Prentice Hall.[](http://en.wikipedia.org/wiki/International_Standard_Book_Number)ISBN[](http://en.wikipedia.org/wiki/Special:BookSources/0-13-085198-1)0-13-085198-1.- 
Richard Hartley and [](http://en.wikipedia.org/wiki/Andrew_Zisserman)
Andrew Zisserman (2003). Multiple View Geometry in Computer Vision (2nd ed.). Cambridge University Press.- 
P.H.S. Torr and D.W. Murray (1997). "The Development and Comparison of Robust Methods for Estimating the Fundamental Matrix".International Journal of Computer Vision24: 271–300.[](http://en.wikipedia.org/wiki/Digital_object_identifier)doi:[](http://dx.doi.org/10.1023%2FA%3A1007927408552)10.1023/A:1007927408552.- 
Ondrej Chum (2005). [](http://cmp.felk.cvut.cz/~chum/Teze/Chum-PhD.pdf)
"Two-View Geometry Estimation by Random Sample and Consensus". PhD Thesis.[](http://cmp.felk.cvut.cz/~chum/Teze/Chum-PhD.pdf)http://cmp.felk.cvut.cz/~chum/Teze/Chum-PhD.pdf- 
Sunglok Choi, Taemin Kim, and Wonpil Yu (2009).[](http://www.bmva.org/bmvc/2009/Papers/Paper355/Paper355.pdf)"Performance Evaluation of RANSAC Family".In
 Proceedings of the British Machine Vision Conference (BMVC).[](http://www.bmva.org/bmvc/2009/Papers/Paper355/Paper355.pdf)http://www.bmva.org/bmvc/2009/Papers/Paper355/Paper355.pdf.

八、外部链接- 
[](http://vision.ece.ucsb.edu/~zuliani/Code/Code.html)RANSAC Toolbox for MATLAB. A research (and didactic) oriented toolbox to explore the RANSAC algorithm in[](http://en.wikipedia.org/wiki/MATLAB)MATLAB.
 It is highly configurable and contains the routines to solve a few relevant estimation problems.- 
[](http://www.mrpt.org/RANSAC_C++_examples)Implementation in C++ as a generic template.- 
[](http://vision.ece.ucsb.edu/~zuliani/Research/RANSAC/docs/RANSAC4Dummies.pdf)RANSAC for Dummies A simple tutorial with many examples that uses the RANSAC Toolbox for MATLAB.- 
[](http://cmp.felk.cvut.cz/ransac-cvpr2006/)25 Years of RANSAC Workshop

九、后话

    本文在翻译的过程中参考了沈乐君的文章《[](http://www.shenlejun.cn/my/article/show.asp?id=43)随机抽样一致性算法RANSAC源程序和教程》。Ziv Yaniv已经用C++实现了RANSAC，您可以[](http://www.shenlejun.cn/my/article/UploadPic/2009-6/2009628254132153.rar)点击这里下载源程序。

不过，如果时间允许的话，我打算自己动手用C#去实现RANSAC算法，原因有两个：

    （1）熟悉算法的最佳途径是自己去实现它；

    （2）方便使用.net的同志们利用RANSAC。

    感谢您耐心看完我的蹩脚翻译，希望对您有所帮助。




以下转自：

http://blog.csdn.net/laobai1015/article/details/51683076

本文将openCV中的RANSAC代码全部挑选出来，进行分析和讲解，以便大家更好的理解RANSAC[](http://lib.csdn.net/base/31)算法。代码我都试过，可以直接运行。


在计算机视觉和图像处理等很多领域，都需要用到RANSAC算法。openCV中也有封装好的RANSAC算法，以便于人们使用。关于RANSAC算法的一些应用，可以看我的另一篇博客：

[](http://blog.csdn.net/qq_25352981/article/details/46914837)利用SIFT和RANSAC算法（openCV框架）实现物体的检测与定位，并求出变换矩阵（findFundamentalMat和findHomography的比较）


但是前几天师弟在使用openCV自带的RANSAC算法时，发现实验的运行时间并不会随着输入数据的增加而增加，感觉和理论上的不太相符。所以我就花了点时间，把openCV中关于RANSAC的源代码全部复制出来研究了一下。以便我们更加清晰的了解RANSAC算法的实际运行过程。


首先看两个类







[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
//模型估计的基类，提供了估计矩阵的各种虚函数- 
//置信度设为0。99 循环次数设置为了2000- 
class CvModelEstimator2  - 
{  - 
public:  - 
    CvModelEstimator2(int _modelPoints, CvSize _modelSize, int _maxBasicSolutions);  - 
virtual ~CvModelEstimator2();  - 
- 
virtualint runKernel( const CvMat* m1, const CvMat* m2, CvMat* model )=0;  - 
//virtual bool runLMeDS( const CvMat* m1, const CvMat* m2, CvMat* model,- 
// CvMat* mask, double confidence=0.99, int maxIters=2000 );- 
virtualbool runRANSAC( const CvMat* m1, const CvMat* m2, CvMat* model,  - 
                            CvMat* mask, double threshold,  - 
double confidence=0.99, int maxIters=2000 );  - 
virtualbool refine( const CvMat*, const CvMat*, CvMat*, int ) { returntrue; }  - 
//virtual void setSeed( int64 seed );- 
- 
protected:  - 
virtualvoid computeReprojError( const CvMat* m1, const CvMat* m2,  - 
const CvMat* model, CvMat* error ) = 0;  - 
virtualint findInliers( const CvMat* m1, const CvMat* m2,  - 
const CvMat* model, CvMat* error,  - 
                             CvMat* mask, double threshold );  - 
virtualbool getSubset( const CvMat* m1, const CvMat* m2,  - 
                            CvMat* ms1, CvMat* ms2, int maxAttempts=1000 );  - 
virtualbool checkSubset( const CvMat* ms1, int count );  - 
- 
    CvRNG rng;  - 
int modelPoints;  - 
    CvSize modelSize;  - 
int maxBasicSolutions;  - 
bool checkPartialSubsets;  - 
};  - 
//单应矩阵估计的子类- 
class CvHomographyEstimator : public CvModelEstimator2  - 
{  - 
public:  - 
    CvHomographyEstimator( int modelPoints );  - 
- 
virtualint runKernel( const CvMat* m1, const CvMat* m2, CvMat* model );  - 
virtualbool refine( const CvMat* m1, const CvMat* m2,  - 
                         CvMat* model, int maxIters );  - 
- 
- 
protected:  - 
virtualvoid computeReprojError( const CvMat* m1, const CvMat* m2,  - 
const CvMat* model, CvMat* error );  - 
};  
上面的两个类中，CvModelEstimator2是一个基类，从名字就可以看出，这个类是用来估计模型的。可以看到里面提供了许多虚函数，这些函数有许多，比如runRANSAC是利用RANSAC方法计算单应矩阵，而runLMeDS是利用LMeDS方法计算单应矩阵，我们这里仅仅讲解RANSAC方法，所以其他不需要的内容我就直接注释掉了




CvHomographyEstimator继承自CvModelEstimator2，同样的，从名字也就可以看出，这个类使用来估计单应矩阵的。



接下来是两个类的构造函数和析构函数，这个没啥好说的了，基本都是默认的。







[html][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
<prename="code"class="cpp">//构造函数  - 
CvModelEstimator2::CvModelEstimator2(int _modelPoints, CvSize _modelSize, int _maxBasicSolutions)  - 
{  - 
modelPoints = _modelPoints;  - 
modelSize = _modelSize;  - 
maxBasicSolutions = _maxBasicSolutions;  - 
checkPartialSubsets = true;  - 
rng = cvRNG(-1);  - 
}  - 
//析构函数  - 
CvModelEstimator2::~CvModelEstimator2()  - 
{  - 
}  - 
- 
CvHomographyEstimator::CvHomographyEstimator(int _modelPoints)  - 
    : CvModelEstimator2(_modelPoints, cvSize(3,3), 1)  - 
{  - 
    assert( _modelPoints == 4 || _modelPoints == 5 );  - 
checkPartialSubsets = false;  - 
- 
}  


接下来到重点了。runRANSAC方法就是通过RANSAC来计算矩阵









[html][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
<prename="code"class="cpp">bool CvModelEstimator2::runRANSAC( const CvMat* m1, const CvMat* m2, CvMat* model,  - 
                                    CvMat* mask0, double reprojThreshold,  - 
                                    double confidence, int maxIters )  - 
{  - 
    bool result = false;  - 
    cv::Ptr<CvMat>mask = cvCloneMat(mask0);   //标记矩阵，标记内点和外点  - 
    cv::Ptr<CvMat> models, err, tmask;  - 
    cv::Ptr<CvMat> ms1, ms2;  - 
- 
    int iter, niters = maxIters;   //这是迭代次数，默认最大的迭代次数为2000次  - 
    int count = m1->rows*m1->cols, maxGoodCount = 0;  - 
    CV_Assert( CV_ARE_SIZES_EQ(m1, m2) && CV_ARE_SIZES_EQ(m1, mask) );  - 
- 
    if( count <modelPoints )  //使用RANSAC算法时，modelPoints为4  - 
        return false;  - 
- 
models = cvCreateMat( modelSize.height*maxBasicSolutions, modelSize.width, CV_64FC1 );  - 
err = cvCreateMat( 1, count, CV_32FC1 );  - 
tmask = cvCreateMat( 1, count, CV_8UC1 );  - 
- 
    if( count > modelPoints )  - 
    {  - 
ms1 = cvCreateMat( 1, modelPoints, m1->type );  - 
ms2 = cvCreateMat( 1, modelPoints, m2->type );  - 
    }  - 
    else  - 
    {  - 
niters = 1;  - 
ms1 = cvCloneMat(m1);  - 
ms2 = cvCloneMat(m2);  - 
    }  - 
- 
    for( iter = 0; iter <niters; iter++ )  - 
    {  - 
        int i, goodCount, nmodels;  - 
- 
        if( count > modelPoints )  - 
        {  - 
            bool found = getSubset( m1, m2, ms1, ms2, 300 );//调用函数，300是循环次数，这个函数  - 
            if( !found )                                    //就是为了从序列中随机选取4组，以便  - 
            {                                               //以便下一步计算单应矩阵  - 
                if( iter == 0 )  - 
                    return false;  - 
                break;  - 
            }  - 
        }  - 
        printf("------");  - 
- 
nmodels = runKernel( ms1, ms2, models );//这个函数是通过给定的4组序列计算出矩阵  - 
- 
        if( nmodels <= 0 )  - 
            continue;  - 
        for( i = 0; i <nmodels; i++ )  - 
        {  - 
            CvMat model_i;  - 
            cvGetRows( models, &model_i, i*modelSize.height, (i+1)*modelSize.height );  - 
goodCount = findInliers( m1, m2, &model_i, err, tmask, reprojThreshold );  - 
            //输出看看一共循环了多少次  - 
            printf("%5d %5d %5d %5d\n",iter,niters,goodCount,maxGoodCount);  - 
            if( goodCount > MAX(maxGoodCount, modelPoints-1) )  - 
            {  - 
                std::swap(tmask, mask);  - 
                cvCopy( &model_i, model );  - 
maxGoodCount = goodCount;  - 
                //循环的次数会发生变化，原来原因在这里  - 
niters = cvRANSACUpdateNumIters( confidence,  - 
                    (double)(count - goodCount)/count, modelPoints, niters );  - 
            }  - 
        }  - 
- 
    }  - 
- 
    //printf("RANSAC算法实际循环了%d次\n",niters);  - 
- 
    if( maxGoodCount > 0 )  - 
    {  - 
        if( mask != mask0 )  - 
            cvCopy( mask, mask0 );  - 
result = true;  - 
    }  - 
- 
    return result;  - 
}  



在这个函数参数中，输入的m1和m2是两个对应的序列，这两组序列的每一对数据一一匹配，其中既有正确的匹配，也有错误的匹配，正确的可以称为内点，错误的称为外点，RANSAC方法就是从这些包含错误匹配的数据中，分离出正确的匹配，并且求得单应矩阵。model就是我们需要求解的单应矩阵，mask我们可以称为标记矩阵，他和m1，m2的长度一样，当一个m1和m2中的点为内点时，mask相应的标记为1，反之为0，说白了，通过mask我们最终可以知道序列中哪些是内点，哪些是外点。reprojThreshold为阈值，当某一个匹配与估计的假设小于阈值时，则被认为是一个内点，这个阈值，openCV默认给的是3，后期使用的时候自己也可以修改。confidence为置信度，其实也就是人为的规定了一个数值，这个数值可以大致表示RANSAC结果的准确性，这个具体有啥用后面咱们再说。这个值初始时被设置为0.995. maxIters为初始迭代次数，RANSAC算法核心就是不断的迭代，这个值就是迭代的次数，默认设为了2000




这个函数的前期，主要是设置了一些变量然后赋初值，然后转换相应的格式等等。最关键的部分，是那个for循环。我们把这个for循环单独拿出来分析一下。代码如下。









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
for( iter = 0; iter < niters; iter++ )  - 
 {  - 
int i, goodCount, nmodels;  - 
- 
if( count > modelPoints )  - 
     {  - 
bool found = getSubset( m1, m2, ms1, ms2, 300 );//调用函数，300是循环次数，这个函数- 
if( !found )                                    //就是为了从序列中随机选取4组，以便- 
         {                                               //以便下一步计算单应矩阵- 
if( iter == 0 )  - 
returnfalse;  - 
break;  - 
         }  - 
     }   - 
     nmodels = runKernel( ms1, ms2, models );//这个函数是通过给定的4组序列计算出矩阵- 
- 
if( nmodels <= 0 )  - 
continue;  - 
for( i = 0; i < nmodels; i++ )  - 
     {  - 
         CvMat model_i;  - 
         cvGetRows( models, &model_i, i*modelSize.height, (i+1)*modelSize.height );  - 
         goodCount = findInliers( m1, m2, &model_i, err, tmask, reprojThreshold );  - 
//输出看看一共循环了多少次- 
printf("%5d %5d %5d %5d\n",iter,niters,goodCount,maxGoodCount);  - 
if( goodCount > MAX(maxGoodCount, modelPoints-1) )  - 
         {  - 
             std::swap(tmask, mask);  - 
             cvCopy( &model_i, model );  - 
             maxGoodCount = goodCount;  - 
//循环的次数会发生变化，原来原因在这里- 
             niters = cvRANSACUpdateNumIters( confidence,  - 
                 (double)(count - goodCount)/count, modelPoints, niters );  - 
         }  - 
     }  - 
- 
 }  








niters最初的值为2000，这就是初始时的RANSAC算法的循环次数，getSubset（）函数是从一组对应的序列中随机的选出4组（因为要想计算出一个3X3的矩阵，至少需要4组对应的坐标），m1和m2是我们输入序列，ms1和ms2是随机选出的对应的4组匹配。


随机的选出4组匹配后，就应该根据这4个匹配计算相应的矩阵，所以函数runKernel（）就是根据4组匹配计算矩阵，参数里的models就是得到的矩阵。这个矩阵只是一个假设，为了验证这个假设，需要用其他的点去计算，看看其他的点是内点还是外点。


findInliers（）函数就是用来计算内点的。利用前面得到的矩阵，把所有的序列带入，计算得出哪些是内点，哪些是外点，函数的返回值为goodCount，就是此次计算的内点的个数。函数中还有一个值为maxGoodCount，每次循环的内点个数的最大值保存在这个值中，一个估计的矩阵如果有越多的内点，那么这个矩阵就越有可能是正确的。所以计算内点个数以后，紧接着判断一下goodCount和maxGoodCount的大小关系，如果goodCount>maxGoodCount，则把goodCount赋值给maxGoodCount。赋值之后的一行代码非常关键，我们单独拿出来说一下，代码如下：





[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
niters = cvRANSACUpdateNumIters( confidence,  - 
                   (double)(count - goodCount)/count, modelPoints, niters );  
niters本来是迭代的次数，也就是循环的次数。但是通过这行代码我们发现，每次循环后，都会对niters这个值进行更新，也就是每次循环后都会改变循环的总次数。cvRANSACUpdateNumIters（）函数利用confidence（置信度）count（总匹配个数）goodCount（当前内点个数）niters（当前的总迭代次数）这几个参数，来动态的改变总迭代次数的大小。该函数的中心思想就是当内点占的比例较多时，那么很有可能已经找到了正确的估计，所以就适当的减少迭代次数来节省时间。这个迭代次数的减少是以指数形式减少的，所以节省的时间开销也是非常的可观。因此最初设计的2000的迭代次数，可能最终的迭代次数只有几十。同样的，如果你自己一开始把迭代次数设置成10000或者更大，进过几次迭代后，niters又会变得非常小了。所以初始时的niters设置的再大，其实对最终的运行时间也没什么影响。我用我自己的程序简答试了一下，无论初值设为2000,10000,20000，最终的迭代次数都变成了58！！！




所以，现在应该清楚为什么输入数据增加，而算法运行时间不会增加了。openCV的RANSAC算法首先把迭代的次数设置为2000，然后再迭代的过程中，动态的改变总迭代次数，无论输入数据有多少，总的迭代次数不会增加，并且通过4个匹配计算出估计的矩阵这个时间是不变的，通过估计矩阵来计算内点，这方面的增加的时间开销基本上可以忽略。所以导致的最终结果就是，无论输入点有多少，运算时间基本不会有太大变化。





以上就是RANSAC算法的核心代码，其中用到的一些函数，下面一一给出。





1. 转换为齐次坐标，看上去很长，但是完成的功能就是把一般的坐标转换成齐次坐标以方便以后的计算







[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
CV_IMPL void cvConvertPointsHomogeneous( const CvMat* src, CvMat* dst )  - 
{  - 
    Ptr<CvMat> temp, denom;  - 
- 
int i, s_count, s_dims, d_count, d_dims;  - 
    CvMat _src, _dst, _ones;  - 
    CvMat* ones = 0;  - 
- 
if( !CV_IS_MAT(src) )  - 
        CV_Error( !src ? CV_StsNullPtr : CV_StsBadArg,  - 
"The input parameter is not a valid matrix" );  - 
- 
if( !CV_IS_MAT(dst) )  - 
        CV_Error( !dst ? CV_StsNullPtr : CV_StsBadArg,  - 
"The output parameter is not a valid matrix" );  - 
- 
if( src == dst || src->data.ptr == dst->data.ptr )  - 
    {  - 
if( src != dst && (!CV_ARE_TYPES_EQ(src, dst) || !CV_ARE_SIZES_EQ(src,dst)) )  - 
            CV_Error( CV_StsBadArg, "Invalid inplace operation" );  - 
return;  - 
    }  - 
- 
if( src->rows > src->cols )  - 
    {  - 
if( !((src->cols > 1) ^ (CV_MAT_CN(src->type) > 1)) )  - 
            CV_Error( CV_StsBadSize, "Either the number of channels or columns or rows must be =1" );  - 
- 
        s_dims = CV_MAT_CN(src->type)*src->cols;  - 
        s_count = src->rows;  - 
    }  - 
else- 
    {  - 
if( !((src->rows > 1) ^ (CV_MAT_CN(src->type) > 1)) )  - 
            CV_Error( CV_StsBadSize, "Either the number of channels or columns or rows must be =1" );  - 
- 
        s_dims = CV_MAT_CN(src->type)*src->rows;  - 
        s_count = src->cols;  - 
    }  - 
- 
if( src->rows == 1 || src->cols == 1 )  - 
        src = cvReshape( src, &_src, 1, s_count );  - 
- 
if( dst->rows > dst->cols )  - 
    {  - 
if( !((dst->cols > 1) ^ (CV_MAT_CN(dst->type) > 1)) )  - 
            CV_Error( CV_StsBadSize,  - 
"Either the number of channels or columns or rows in the input matrix must be =1" );  - 
- 
        d_dims = CV_MAT_CN(dst->type)*dst->cols;  - 
        d_count = dst->rows;  - 
    }  - 
else- 
    {  - 
if( !((dst->rows > 1) ^ (CV_MAT_CN(dst->type) > 1)) )  - 
            CV_Error( CV_StsBadSize,  - 
"Either the number of channels or columns or rows in the output matrix must be =1" );  - 
- 
        d_dims = CV_MAT_CN(dst->type)*dst->rows;  - 
        d_count = dst->cols;  - 
    }  - 
- 
if( dst->rows == 1 || dst->cols == 1 )  - 
        dst = cvReshape( dst, &_dst, 1, d_count );  - 
- 
if( s_count != d_count )  - 
        CV_Error( CV_StsUnmatchedSizes, "Both matrices must have the same number of points" );  - 
- 
if( CV_MAT_DEPTH(src->type) < CV_32F || CV_MAT_DEPTH(dst->type) < CV_32F )  - 
        CV_Error( CV_StsUnsupportedFormat,  - 
"Both matrices must be floating-point (single or double precision)" );  - 
- 
if( s_dims < 2 || s_dims > 4 || d_dims < 2 || d_dims > 4 )  - 
        CV_Error( CV_StsOutOfRange,  - 
"Both input and output point dimensionality must be 2, 3 or 4" );  - 
- 
if( s_dims < d_dims - 1 || s_dims > d_dims + 1 )  - 
        CV_Error( CV_StsUnmatchedSizes,  - 
"The dimensionalities of input and output point sets differ too much" );  - 
- 
if( s_dims == d_dims - 1 )  - 
    {  - 
if( d_count == dst->rows )  - 
        {  - 
            ones = cvGetSubRect( dst, &_ones, cvRect( s_dims, 0, 1, d_count ));  - 
            dst = cvGetSubRect( dst, &_dst, cvRect( 0, 0, s_dims, d_count ));  - 
        }  - 
else- 
        {  - 
            ones = cvGetSubRect( dst, &_ones, cvRect( 0, s_dims, d_count, 1 ));  - 
            dst = cvGetSubRect( dst, &_dst, cvRect( 0, 0, d_count, s_dims ));  - 
        }  - 
    }  - 
- 
if( s_dims <= d_dims )  - 
    {  - 
if( src->rows == dst->rows && src->cols == dst->cols )  - 
        {  - 
if( CV_ARE_TYPES_EQ( src, dst ) )  - 
                cvCopy( src, dst );  - 
else- 
                cvConvert( src, dst );  - 
        }  - 
else- 
        {  - 
if( !CV_ARE_TYPES_EQ( src, dst ))  - 
            {  - 
                temp = cvCreateMat( src->rows, src->cols, dst->type );  - 
                cvConvert( src, temp );  - 
                src = temp;  - 
            }  - 
            cvTranspose( src, dst );  - 
        }  - 
- 
if( ones )  - 
            cvSet( ones, cvRealScalar(1.) );  - 
    }  - 
else- 
    {  - 
int s_plane_stride, s_stride, d_plane_stride, d_stride, elem_size;  - 
- 
if( !CV_ARE_TYPES_EQ( src, dst ))  - 
        {  - 
            temp = cvCreateMat( src->rows, src->cols, dst->type );  - 
            cvConvert( src, temp );  - 
            src = temp;  - 
        }  - 
- 
        elem_size = CV_ELEM_SIZE(src->type);  - 
- 
if( s_count == src->cols )  - 
            s_plane_stride = src->step / elem_size, s_stride = 1;  - 
else- 
            s_stride = src->step / elem_size, s_plane_stride = 1;  - 
- 
if( d_count == dst->cols )  - 
            d_plane_stride = dst->step / elem_size, d_stride = 1;  - 
else- 
            d_stride = dst->step / elem_size, d_plane_stride = 1;  - 
- 
        denom = cvCreateMat( 1, d_count, dst->type );  - 
- 
if( CV_MAT_DEPTH(dst->type) == CV_32F )  - 
        {  - 
constfloat* xs = src->data.fl;  - 
constfloat* ys = xs + s_plane_stride;  - 
constfloat* zs = 0;  - 
constfloat* ws = xs + (s_dims - 1)*s_plane_stride;  - 
- 
float* iw = denom->data.fl;  - 
- 
float* xd = dst->data.fl;  - 
float* yd = xd + d_plane_stride;  - 
float* zd = 0;  - 
- 
if( d_dims == 3 )  - 
            {  - 
                zs = ys + s_plane_stride;  - 
                zd = yd + d_plane_stride;  - 
            }  - 
- 
for( i = 0; i < d_count; i++, ws += s_stride )  - 
            {  - 
float t = *ws;  - 
                iw[i] = fabs((double)t) > FLT_EPSILON ? t : 1.f;  - 
            }  - 
- 
            cvDiv( 0, denom, denom );  - 
- 
if( d_dims == 3 )  - 
for( i = 0; i < d_count; i++ )  - 
                {  - 
float w = iw[i];  - 
float x = *xs * w, y = *ys * w, z = *zs * w;  - 
                    xs += s_stride; ys += s_stride; zs += s_stride;  - 
                    *xd = x; *yd = y; *zd = z;  - 
                    xd += d_stride; yd += d_stride; zd += d_stride;  - 
                }  - 
else- 
for( i = 0; i < d_count; i++ )  - 
                {  - 
float w = iw[i];  - 
float x = *xs * w, y = *ys * w;  - 
                    xs += s_stride; ys += s_stride;  - 
                    *xd = x; *yd = y;  - 
                    xd += d_stride; yd += d_stride;  - 
                }  - 
        }  - 
else- 
        {  - 
constdouble* xs = src->data.db;  - 
constdouble* ys = xs + s_plane_stride;  - 
constdouble* zs = 0;  - 
constdouble* ws = xs + (s_dims - 1)*s_plane_stride;  - 
- 
double* iw = denom->data.db;  - 
- 
double* xd = dst->data.db;  - 
double* yd = xd + d_plane_stride;  - 
double* zd = 0;  - 
- 
if( d_dims == 3 )  - 
            {  - 
                zs = ys + s_plane_stride;  - 
                zd = yd + d_plane_stride;  - 
            }  - 
- 
for( i = 0; i < d_count; i++, ws += s_stride )  - 
            {  - 
double t = *ws;  - 
                iw[i] = fabs(t) > DBL_EPSILON ? t : 1.;  - 
            }  - 
- 
            cvDiv( 0, denom, denom );  - 
- 
if( d_dims == 3 )  - 
for( i = 0; i < d_count; i++ )  - 
                {  - 
double w = iw[i];  - 
double x = *xs * w, y = *ys * w, z = *zs * w;  - 
                    xs += s_stride; ys += s_stride; zs += s_stride;  - 
                    *xd = x; *yd = y; *zd = z;  - 
                    xd += d_stride; yd += d_stride; zd += d_stride;  - 
                }  - 
else- 
for( i = 0; i < d_count; i++ )  - 
                {  - 
double w = iw[i];  - 
double x = *xs * w, y = *ys * w;  - 
                    xs += s_stride; ys += s_stride;  - 
                    *xd = x; *yd = y;  - 
                    xd += d_stride; yd += d_stride;  - 
                }  - 
        }  - 
    }  - 
}  

2. 对迭代值进行更新的函数。这个函数就是对总的迭代次数进行更新，从中可以看到，迭代值以指数形式减少。最初的为2000的迭代次数，有的时候可能经过不断的更新，最终结果成了几十了。









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
CV_IMPL int- 
cvRANSACUpdateNumIters( double p, double ep,  - 
int model_points, int max_iters )  - 
{  - 
if( model_points <= 0 )  - 
        CV_Error( CV_StsOutOfRange, "the number of model points should be positive" );  - 
- 
    p = MAX(p, 0.);  - 
    p = MIN(p, 1.);  - 
    ep = MAX(ep, 0.);  - 
    ep = MIN(ep, 1.);  - 
- 
// avoid inf's & nan's- 
double num = MAX(1. - p, DBL_MIN);  - 
double denom = 1. - pow(1. - ep,model_points);  - 
if( denom < DBL_MIN )  - 
return 0;  - 
- 
    num = log(num);  - 
    denom = log(denom);  - 
- 
return denom >= 0 || -num >= max_iters*(-denom) ?  - 
        max_iters : cvRound(num/denom);  - 
}  

3. 通过4个匹配，计算单应矩阵，就是给你了4个匹配，你把和这四个匹配相符的矩阵计算出来









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
//通过四个匹配，计算符合要求的单应矩阵- 
int CvHomographyEstimator::runKernel( const CvMat* m1, const CvMat* m2, CvMat* H )  - 
{  - 
int i, count = m1->rows*m1->cols;  - 
const CvPoint2D64f* M = (const CvPoint2D64f*)m1->data.ptr;  - 
const CvPoint2D64f* m = (const CvPoint2D64f*)m2->data.ptr;  - 
- 
double LtL[9][9], W[9][1], V[9][9];  - 
    CvMat _LtL = cvMat( 9, 9, CV_64F, LtL );  - 
    CvMat matW = cvMat( 9, 1, CV_64F, W );  - 
    CvMat matV = cvMat( 9, 9, CV_64F, V );  - 
    CvMat _H0 = cvMat( 3, 3, CV_64F, V[8] );  - 
    CvMat _Htemp = cvMat( 3, 3, CV_64F, V[7] );  - 
    CvPoint2D64f cM={0,0}, cm={0,0}, sM={0,0}, sm={0,0};  - 
- 
for( i = 0; i < count; i++ )  - 
    {  - 
        cm.x += m[i].x; cm.y += m[i].y;  - 
        cM.x += M[i].x; cM.y += M[i].y;  - 
    }  - 
- 
    cm.x /= count; cm.y /= count;  - 
    cM.x /= count; cM.y /= count;  - 
- 
for( i = 0; i < count; i++ )  - 
    {  - 
        sm.x += fabs(m[i].x - cm.x);  - 
        sm.y += fabs(m[i].y - cm.y);  - 
        sM.x += fabs(M[i].x - cM.x);  - 
        sM.y += fabs(M[i].y - cM.y);  - 
    }  - 
- 
if( fabs(sm.x) < DBL_EPSILON || fabs(sm.y) < DBL_EPSILON ||  - 
        fabs(sM.x) < DBL_EPSILON || fabs(sM.y) < DBL_EPSILON )  - 
return 0;  - 
    sm.x = count/sm.x; sm.y = count/sm.y;  - 
    sM.x = count/sM.x; sM.y = count/sM.y;  - 
- 
double invHnorm[9] = { 1./sm.x, 0, cm.x, 0, 1./sm.y, cm.y, 0, 0, 1 };  - 
double Hnorm2[9] = { sM.x, 0, -cM.x*sM.x, 0, sM.y, -cM.y*sM.y, 0, 0, 1 };  - 
    CvMat _invHnorm = cvMat( 3, 3, CV_64FC1, invHnorm );  - 
    CvMat _Hnorm2 = cvMat( 3, 3, CV_64FC1, Hnorm2 );  - 
- 
    cvZero( &_LtL );  - 
for( i = 0; i < count; i++ )  - 
    {  - 
double x = (m[i].x - cm.x)*sm.x, y = (m[i].y - cm.y)*sm.y;  - 
double X = (M[i].x - cM.x)*sM.x, Y = (M[i].y - cM.y)*sM.y;  - 
double Lx[] = { X, Y, 1, 0, 0, 0, -x*X, -x*Y, -x };  - 
double Ly[] = { 0, 0, 0, X, Y, 1, -y*X, -y*Y, -y };  - 
int j, k;  - 
for( j = 0; j < 9; j++ )  - 
for( k = j; k < 9; k++ )  - 
                LtL[j][k] += Lx[j]*Lx[k] + Ly[j]*Ly[k];  - 
    }  - 
    cvCompleteSymm( &_LtL );  - 
- 
//cvSVD( &_LtL, &matW, 0, &matV, CV_SVD_MODIFY_A + CV_SVD_V_T );- 
    cvEigenVV( &_LtL, &matV, &matW );  - 
    cvMatMul( &_invHnorm, &_H0, &_Htemp );  - 
    cvMatMul( &_Htemp, &_Hnorm2, &_H0 );  - 
    cvConvertScale( &_H0, H, 1./_H0.data.db[8] );  - 
- 
return 1;  - 
}  

4. 给定输入序列后，从中随机的选出4对匹配









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
bool CvModelEstimator2::getSubset( const CvMat* m1, const CvMat* m2,  - 
                                   CvMat* ms1, CvMat* ms2, int maxAttempts )  //maxAttempts被设为300- 
{  - 
    cv::AutoBuffer<int> _idx(modelPoints);  - 
int* idx = _idx;  - 
int i = 0, j, k, idx_i, iters = 0;  - 
int type = CV_MAT_TYPE(m1->type), elemSize = CV_ELEM_SIZE(type);  - 
constint *m1ptr = m1->data.i, *m2ptr = m2->data.i;  - 
int *ms1ptr = ms1->data.i, *ms2ptr = ms2->data.i;  - 
int count = m1->cols*m1->rows;  - 
- 
    assert( CV_IS_MAT_CONT(m1->type & m2->type) && (elemSize % sizeof(int) == 0) );  - 
    elemSize /= sizeof(int);  - 
- 
for(; iters < maxAttempts; iters++)  - 
    {  - 
for( i = 0; i < modelPoints && iters < maxAttempts; )  - 
        {  - 
            idx[i] = idx_i = cvRandInt(&rng) % count;    //产生count以内的随机数，count是序列长度- 
for( j = 0; j < i; j++ )                    //保证产生的随机数没有重复的- 
if( idx_i == idx[j] )  - 
break;  - 
if( j < i )  - 
continue;  - 
for( k = 0; k < elemSize; k++ )  - 
            {    - 
                ms1ptr[i*elemSize + k] = m1ptr[idx_i*elemSize + k];   //把随机产生的数给了ms1和ms2- 
                ms2ptr[i*elemSize + k] = m2ptr[idx_i*elemSize + k];  - 
            }  - 
if( checkPartialSubsets && (!checkSubset( ms1, i+1 ) || !checkSubset( ms2, i+1 )))   //调用函数checkSubset- 
            {  - 
                iters++;  - 
continue;  - 
            }  - 
            i++;  - 
        }  - 
if( !checkPartialSubsets && i == modelPoints &&  - 
            (!checkSubset( ms1, i ) || !checkSubset( ms2, i )))  - 
continue;  - 
break;  - 
    }  - 
- 
return i == modelPoints && iters < maxAttempts;  - 
}  

5. 对生成的4组匹配进行检验，观察其是否合乎要求。









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
bool CvModelEstimator2::checkSubset( const CvMat* m, int count )  - 
{  - 
int j, k, i, i0, i1;  - 
    CvPoint2D64f* ptr = (CvPoint2D64f*)m->data.ptr;  - 
- 
    assert( CV_MAT_TYPE(m->type) == CV_64FC2 );  - 
- 
if( checkPartialSubsets )  - 
        i0 = i1 = count - 1;  - 
else- 
        i0 = 0, i1 = count - 1;  - 
- 
for( i = i0; i <= i1; i++ )  - 
    {  - 
// check that the i-th selected point does not belong- 
// to a line connecting some previously selected points- 
for( j = 0; j < i; j++ )  - 
        {  - 
double dx1 = ptr[j].x - ptr[i].x;  - 
double dy1 = ptr[j].y - ptr[i].y;  - 
for( k = 0; k < j; k++ )  - 
            {  - 
double dx2 = ptr[k].x - ptr[i].x;  - 
double dy2 = ptr[k].y - ptr[i].y;  - 
if( fabs(dx2*dy1 - dy2*dx1) <= FLT_EPSILON*(fabs(dx1) + fabs(dy1) + fabs(dx2) + fabs(dy2)))  - 
break;  - 
            }  - 
if( k < j )  - 
break;  - 
        }  - 
if( j < i )  - 
break;  - 
    }  - 
- 
return i >= i1;  - 
}  
6. 计算内点的个数并且标记序列中哪些点是内点。









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
int CvModelEstimator2::findInliers( const CvMat* m1, const CvMat* m2,  - 
const CvMat* model, CvMat* _err,  - 
                                    CvMat* _mask, double threshold )  - 
{  - 
int i, count = _err->rows*_err->cols, goodCount = 0;  - 
constfloat* err = _err->data.fl;  - 
    uchar* mask = _mask->data.ptr;  - 
- 
    computeReprojError( m1, m2, model, _err );  //_err里面是计算后的矩阵的大小，用于与阈值比较- 
    threshold *= threshold;  - 
for( i = 0; i < count; i++ )  - 
        goodCount += mask[i] = err[i] <= threshold;//goodCount为计算出的内点的个数- 
return goodCount;  - 
}  


7.上面的函数调用的一些函数，这些函数不难，所以下面相应的列举一下









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
bool CvHomographyEstimator::refine( const CvMat* m1, const CvMat* m2, CvMat* model, int maxIters )  - 
{  - 
    CvLevMarq solver(8, 0, cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, maxIters, DBL_EPSILON));  - 
int i, j, k, count = m1->rows*m1->cols;  - 
const CvPoint2D64f* M = (const CvPoint2D64f*)m1->data.ptr;  - 
const CvPoint2D64f* m = (const CvPoint2D64f*)m2->data.ptr;  - 
    CvMat modelPart = cvMat( solver.param->rows, solver.param->cols, model->type, model->data.ptr );  - 
    cvCopy( &modelPart, solver.param );  - 
- 
for(;;)  - 
    {  - 
const CvMat* _param = 0;  - 
        CvMat *_JtJ = 0, *_JtErr = 0;  - 
double* _errNorm = 0;  - 
- 
if( !solver.updateAlt( _param, _JtJ, _JtErr, _errNorm ))  - 
break;  - 
- 
for( i = 0; i < count; i++ )  - 
        {  - 
constdouble* h = _param->data.db;  - 
double Mx = M[i].x, My = M[i].y;  - 
double ww = h[6]*Mx + h[7]*My + 1.;  - 
            ww = fabs(ww) > DBL_EPSILON ? 1./ww : 0;  - 
double _xi = (h[0]*Mx + h[1]*My + h[2])*ww;  - 
double _yi = (h[3]*Mx + h[4]*My + h[5])*ww;  - 
double err[] = { _xi - m[i].x, _yi - m[i].y };  - 
if( _JtJ || _JtErr )  - 
            {  - 
double J[][8] =  - 
                {  - 
                    { Mx*ww, My*ww, ww, 0, 0, 0, -Mx*ww*_xi, -My*ww*_xi },  - 
                    { 0, 0, 0, Mx*ww, My*ww, ww, -Mx*ww*_yi, -My*ww*_yi }  - 
                };  - 
- 
for( j = 0; j < 8; j++ )  - 
                {  - 
for( k = j; k < 8; k++ )  - 
                        _JtJ->data.db[j*8+k] += J[0][j]*J[0][k] + J[1][j]*J[1][k];  - 
                    _JtErr->data.db[j] += J[0][j]*err[0] + J[1][j]*err[1];  - 
                }  - 
            }  - 
if( _errNorm )  - 
                *_errNorm += err[0]*err[0] + err[1]*err[1];  - 
        }  - 
    }  - 
- 
    cvCopy( solver.param, &modelPart );  - 
returntrue;  - 
}  - 
- 
void CvHomographyEstimator::computeReprojError( const CvMat* m1, const CvMat* m2,  - 
const CvMat* model, CvMat* _err )  - 
{  - 
int i, count = m1->rows*m1->cols;  - 
const CvPoint2D64f* M = (const CvPoint2D64f*)m1->data.ptr;  - 
const CvPoint2D64f* m = (const CvPoint2D64f*)m2->data.ptr;  - 
constdouble* H = model->data.db;  - 
float* err = _err->data.fl;  - 
- 
for( i = 0; i < count; i++ )  - 
    {  - 
double ww = 1./(H[6]*M[i].x + H[7]*M[i].y + 1.);  - 
double dx = (H[0]*M[i].x + H[1]*M[i].y + H[2])*ww - m[i].x;  - 
double dy = (H[3]*M[i].x + H[4]*M[i].y + H[5])*ww - m[i].y;  - 
        err[i] = (float)(dx*dx + dy*dy);  - 
    }  - 
}  

8，最后一部分是比较关键的。就是FindHomography函数本身。这个函数又去调用了cvFindHomography函数，估计就是openCV不同版本的函数吧，其实现的功能和思想都是一样的。这个函数内部基本上也就是做一些判断防止溢出，排查错误，检验变量以及变换格式等辅助性的内容，真正的方法性质的代码还是在上面的提到的CvHomographyEstimator类中。









[cpp][](http://blog.csdn.net/qq_25352981/article/details/51530751#)view
 plain[](http://blog.csdn.net/qq_25352981/article/details/51530751#)copy



[](https://code.csdn.net/snippets/1700555)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1700555/fork)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- 
cv::Mat findHomography( InputArray _points1, InputArray _points2,  - 
int method, double ransacReprojThreshold, OutputArray _mask)  - 
{  - 
    Mat points1 = _points1.getMat(), points2 = _points2.getMat();  - 
int npoints = points1.checkVector(2);//返回矩阵的序列个数- 
    CV_Assert( npoints >= 0 && points2.checkVector(2) == npoints &&  - 
               points1.type() == points2.type());  //检验初始条件是否正确- 
- 
    Mat H(3, 3, CV_64F);  - 
    CvMat _pt1 = points1, _pt2 = points2;  - 
    CvMat matH = H, c_mask, *p_mask = 0;  - 
if( _mask.needed() )  - 
    {  - 
        _mask.create(npoints, 1, CV_8U, -1, true);  - 
        p_mask = &(c_mask = _mask.getMat());  - 
    }  - 
bool ok = cvFindHomography( &_pt1, &_pt2, &matH, method, ransacReprojThreshold, p_mask ) > 0;  //函数调用- 
if( !ok )  - 
        H = Scalar(0);  - 
return H;  - 
}  - 
- 
- 
- 
- 
- 
CV_IMPL int- 
cvFindHomography( const CvMat* objectPoints, const CvMat* imagePoints,  - 
                  CvMat* __H, int method, double ransacReprojThreshold,  - 
                  CvMat* mask )  - 
{  - 
constdouble confidence = 0.995;  - 
constint maxIters = 2000;                           //修改这里来修改迭代次数- 
constdouble defaultRANSACReprojThreshold = 3;  - 
bool result = false;  - 
    Ptr<CvMat> m, M, tempMask;  - 
- 
double H[9];  - 
    CvMat matH = cvMat( 3, 3, CV_64FC1, H );    //这就是单应矩阵，矩阵初始化- 
int count;   - 
- 
    CV_Assert( CV_IS_MAT(imagePoints) && CV_IS_MAT(objectPoints) );  - 
- 
    count = MAX(imagePoints->cols, imagePoints->rows);    //序列个数- 
    CV_Assert( count >= 4 );  - 
if( ransacReprojThreshold <= 0 )  - 
        ransacReprojThreshold = defaultRANSACReprojThreshold;  - 
- 
    m = cvCreateMat( 1, count, CV_64FC2 );  - 
    cvConvertPointsHomogeneous( imagePoints, m );  //转换齐次坐标- 
- 
    M = cvCreateMat( 1, count, CV_64FC2 );  - 
    cvConvertPointsHomogeneous( objectPoints, M );  - 
- 
if( mask )  - 
    {  - 
        CV_Assert( CV_IS_MASK_ARR(mask) && CV_IS_MAT_CONT(mask->type) &&  - 
            (mask->rows == 1 || mask->cols == 1) &&  - 
            mask->rows*mask->cols == count );  - 
    }  - 
if( mask || count > 4 )  - 
        tempMask = cvCreateMat( 1, count, CV_8U );  - 
if( !tempMask.empty() )  - 
        cvSet( tempMask, cvScalarAll(1.) );  - 
- 
        CvHomographyEstimator estimator( MIN(count, 4) );   //参数是一个小于等于4的值，只有大于4，才能用RANSAC计算- 
if( count == 4 )  - 
        method = 0;  - 
if( method == CV_LMEDS )  - 
//result = estimator.runLMeDS( M, m, &matH, tempMask, confidence, maxIters );- 
        printf("");  - 
elseif( method == CV_RANSAC )  - 
        result = estimator.runRANSAC( M, m, &matH, tempMask, ransacReprojThreshold, confidence, maxIters);  - 
else- 
        result = estimator.runKernel( M, m, &matH ) > 0;  - 
- 
if( result && count > 4 )  - 
    {  - 
        icvCompressPoints( (CvPoint2D64f*)M->data.ptr, tempMask->data.ptr, 1, count );  //压缩，使序列紧凑- 
        count = icvCompressPoints( (CvPoint2D64f*)m->data.ptr, tempMask->data.ptr, 1, count );  - 
        M->cols = m->cols = count;    //筛选过后，这个count是内点的个数- 
if( method == CV_RANSAC )  - 
            estimator.runKernel( M, m, &matH );  //重新计算最终的单应矩阵，matH- 
        estimator.refine( M, m, &matH, 10 );  - 
    }  - 
- 
if( result )  - 
        cvConvert( &matH, __H );  - 
- 
if( mask && tempMask )  - 
    {  - 
if( CV_ARE_SIZES_EQ(mask, tempMask) )    //复制这个矩阵- 
           cvCopy( tempMask, mask );  - 
else- 
           cvTranspose( tempMask, mask );        //行列调换的 复制这个矩阵- 
    }  - 
- 
return (int)result;  - 
}  - 



[](http://blog.csdn.net/laobai1015/article/details/51683076#)[](http://blog.csdn.net/laobai1015/article/details/51683076#)[](http://blog.csdn.net/laobai1015/article/details/51683076#)[](http://blog.csdn.net/laobai1015/article/details/51683076#)[](http://blog.csdn.net/laobai1015/article/details/51683076#)[](http://blog.csdn.net/laobai1015/article/details/51683076#)






