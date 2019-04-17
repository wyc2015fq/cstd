# 【特征匹配】PCA-SIFT原理及源码解析 - 勿在浮砂筑高台 - CSDN博客





2015年10月16日 21:26:20[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：8561标签：[PCA																[SIFT](https://so.csdn.net/so/search/s.do?q=SIFT&t=blog)](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)
个人分类：[Feature Matching																[Image Processing](https://blog.csdn.net/luoshixian099/article/category/2734445)](https://blog.csdn.net/luoshixian099/article/category/5732053)

所属专栏：[特征匹配](https://blog.csdn.net/column/details/featuresdetect.html)







> 
**相关**： 
[**SIFT原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/47377611)
[**SURF原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/47807103)
[**ORB原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/48523267)
[**FAST原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/48294967)
[**BRIEF描述子原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/48338273)
[**Harris原理与源码解析**](http://blog.csdn.net/luoshixian099/article/details/48244255)
**转载请注明出处**：***[http://blog.csdn.net/luoshixian099/article/details/49174869](http://blog.csdn.net/luoshixian099/article/details/49174869)***

PCA-SIFT是对传统SIFT算法的改进，由Yan Ke等人在《PCA-SIFT: A More Distinctive Representation for Local Image Descriptors》中提出，论文中采用PCA(Principal Component Analysis)把SIFT特征描述子从128维降到了20维，优化了描述子占用的内存，同时提高了匹配的准确性。

**复习SIFT原理步骤**： 

1.构造DOG尺度空间 2.在各尺度上定位关键点 3.为关键点分配方向角 4.形成特征描述子 。 

   由于PCA-SIFT仅与SIFT第四步不同，1,2,3步骤详细过程请参考之前的文章[SIFT原理](http://blog.csdn.net/luoshixian099/article/details/47377611)。 

步骤4：首先根据特征点所在尺度，确定一个特征点邻域（邻域大小与特征点所在尺度有关），分成4×4个子区域并旋转到主方向的位置，在每一子区域上统计梯度方向直方图（幅值采用采用高斯加权函数 ,360度范围被等分成8个方向），这样联合4×4个子区域统计信息就构成了一个特征描述子，特征描述子信息为4×4×8=128维。
—–***PCA-SIFT原理步骤***———————————————————————————————————– 

1,2,3步骤与SIFT原理的相同，第四步(PCA)： 

（1）对特征点确定一个大小为41×41的邻域，旋转这个邻域到主方向； 

（2）计算邻域内像素点的水平梯度与垂直梯度，这样每个特征点确定了一个大小为39×39×2=3042维的特征描述子; 

（3）原论文中对同一类图像集中采集了21000个特征点，这样够成了一个原始特征矩阵M，矩阵大小3042×21000,计算矩阵的协方差矩阵N; 

（4）计算协方差矩阵N的特征向量，根据特征根的大小排序，选择对应的前n个特征向量(论文中采用n=20),构成投影矩阵T； 

（5）对新的特征描述子向量，乘以投影矩阵T，得到3042维降到n维的特征向量；
实际上，步骤(3),(4)是在之前计算好，即投影矩阵是通过同一类图像集采用PCA原理提前计算出。从图像中特征点计算得到3042维的特征描述子时，只需要与投影矩阵相乘，即可达到降维。关于投影矩阵的n的选择，可以根据需要固定n的值，也可根据协方差矩阵的特征值能量值百分比，自动确定n的大小。论文中采用n=20效果最佳。

.—————————————————————————————————————————————–

**补充知识：**

PCA(Principal Component Analysis)即主成分分析，也被称为KL变换或者Hotelling变换，数据的变换可以达到分类或者压缩数据的作用，PCA-SIFT是对SIFT描述子数据进行了压缩。首先收集数据所有特征(成分)，通过变换的数据，观察数据的重要成分进行分类，也可以抛弃不重要的成分，减少或者压缩数据。如同,想表达某一种物体，而物体有很多个属性，通过变换数据，可以观察到每个属性的重要性，从而可以选择几种重要的属性去描述这一种物体，就到达了压缩数据的作用。

1 . 假如我们取到了有m个样本，每个样本有n个属性（行向量表示），则组成的数据矩阵 
$\bf M_Y=(Y_1,Y_2,Y_3,...Y_m)$,其中$\bf Y_1$$=(y_{1,1},y_{1,2},y_{1,3},...y_{1,n})^T$； 

则数据矩阵$\bf M_Y$的每一列表示某一种属性的m个测量值; 

用随机变量$X_i$表示矩阵的列向量，定义$X=(X_1,X_2,X_3,....X_n)^T$；
2 . 计算协方差矩阵：即每两个随机变量之间的协方差构成的矩阵$\bf C_Y$； 


$C_Y=\left[\begin{matrix} c_{11}      & c_{1,2}      & \cdots & c_{1,n}      \\ c_{2,1}      & c_{2,2}      & \cdots & c_{2,n}     \\ \vdots & \vdots & \ddots & \vdots \\ c_{n,1}      & c_{n,2}      & \cdots & c_{n,n}      \\\end{matrix}\right]$

其中$c_{i,j}=cov(X_i,X_j)=E[(x_i-u_i)(x_j-u_j)]$,$u_i，u_j$表示随机变量$X_i,X_j$的期望. 

写成矩阵的形式：$C_X=\frac{1}{m}[(X-E(X))^T(X-E(X)]$

协方差矩阵元素值表示两个随机变量的线性相关性，通过计算协方差可以确定两个数据集合之间是否有关系。协方差值的取值范围从0开始，0表示线性无关到反应依赖关系强烈的正值或者负值。注意，协方差只能衡量随机变量之间的线性关系，这也PCA的局限性。通过观察为0的值可以突出有利于分类的不相关特征。由协方差矩阵的定义，可知协方差矩阵是对称矩阵，而且对角线元素表示对应特征的方差。
3 . 计算协方差矩阵的特征值与特征向量 

PCA通过即找到变换矩阵$W$（也称为投影矩阵）,使得$Z=XW^T$,即原数据矩阵$X$通过投影到矩阵$Z$中，同时矩阵$Z$的协方差矩阵$C_Z$为对角阵，即除了对角线元素外其他元素都为0，在这种情况下,特征之间不相关。 

根据协方差矩阵的定义： 
$C_Z=\frac{1}{m}[(Z-E(Z))^T(Z-E(Z)]$,替换$Z=XW^T$和$Z^T=WX^T$
$=\frac{1}{m}[W(X-E(X))T(X-E(x))W]$
$=WC_XW^T$

即变换矩阵W由矩阵X的特征向量作为行向量构成；即求解矩阵X的对角化过程。 

先求得矩阵X的特征值，特征值按大小排序，并求得特征值对应的特征向量构成变换矩阵W.
4 . 较大的特征值，表明此特征范围较大，方差也越大，容易区分。通常在压缩数据中，采用前几个较大的特征值对应的特征向量，构成变换矩阵。通过$Z=XW^T$获得新特征，以缩减新特征对应的维数，且新特征是线性无关的。

5 . 有时为了从压缩后的数据重构原数据，被称为逆变换，$X=ZW$，可以重构回原始数据特征。

```
/*
data： 为原始数据矩阵
mean： 各维度的均值，可以作为输入或者输出
flags： 
      CV_COVAR_ROWS  表示每行作为一个样本；
      CV_COVAR_COLS  表示每列作为一个样本；
      CV_COVAR_USE_AVG  表示mean矩阵作为输入；
      ...采用|操作联合
maxComponents： 需要的维数，必须小于样本数据的维数；
retainedVariance: 根据特征值能量百分比自动确定维数；
*/
class CV_EXPORTS PCA
{
public:
    //! default constructor
    PCA();                                        
    //! the constructor that performs PCA
    PCA(InputArray data, InputArray mean, int flags, int maxComponents=0);   
    PCA(InputArray data, InputArray mean, int flags, double retainedVariance);
    //! operator that performs PCA. The previously stored data, if any, is released
    PCA& operator()(InputArray data, InputArray mean, int flags, int maxComponents=0);
    PCA& computeVar(InputArray data, InputArray mean, int flags, double retainedVariance);
    //! projects vector from the original space to the principal components subspace
    Mat project(InputArray vec) const;//投影函数
    //! projects vector from the original space to the principal components subspace
    void project(InputArray vec, OutputArray result) const;//投影函数
    //! reconstructs the original vector from the projection
    Mat backProject(InputArray vec) const;//逆变换操作
    //! reconstructs the original vector from the projection
    void backProject(InputArray vec, OutputArray result) const;//逆变换操作

    Mat eigenvectors; //!< eigenvectors of the covariation matrix  特征向量矩阵
    Mat eigenvalues;  // !< eigenvalues of the covariation matrix  特征值矩阵，单列矩阵
    Mat mean; //!< mean value subtracted before the projection and added after the back  projection 特征均值矩阵
};
```

```
PCA& PCA::operator()(InputArray _data, InputArray __mean, int flags, int maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    int covar_flags = CV_COVAR_SCALE;//协方差元素取均值,即除以m   ,  m为原始数据的维数
    int i, len, in_count;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    if( flags & CV_PCA_DATA_AS_COL )//表示数据样本向量按列存放
    {
        len = data.rows;  //表示数据的初始维数
        in_count = data.cols;  //表示数据的样本数
        covar_flags |= CV_COVAR_COLS;
        mean_sz = Size(1, len);//定义一个单列的矩阵尺寸
    }
    else
    {
        len = data.cols;
        in_count = data.rows;
        covar_flags |= CV_COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    int count = std::min(len, in_count), out_count = count;
    if( maxComponents > 0 )  //限制降维的数目
        out_count = std::min(count, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if( len <= in_count )//样本数大于数据的维数,否则采用"scrmbled" way,协方差矩阵in_count×in_count
        covar_flags |= CV_COVAR_NORMAL; //协方差矩阵大小为len×len

    int ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );//均值单行或者单列的矩阵

    Mat covar( count, count, ctype );

    if( _mean.data )  //采用提取计算好的均值矩阵
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
    }
   /*计算协方差矩阵保存在covar*/
    calcCovarMatrix( data, covar, mean, covar_flags, ctype );  
    eigen( covar, eigenvalues, eigenvectors );//计算对称协方差矩阵的特征值与特征向量

    if( !(covar_flags & CV_COVAR_NORMAL) )  //“scrambled” 方法计算协方差矩阵
    {
        // CV_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // CV_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        if( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        else
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(count, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & CV_PCA_DATA_AS_COL) ? CV_GEMM_B_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        for( i = 0; i < out_count; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    if( count > out_count )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_count).clone();//保存特征值与特征向量
        eigenvectors = eigenvectors.rowRange(0,out_count).clone();//保存特征值与特征向量
    }
    return *this;
}
```

```
void PCA::project(InputArray _data, OutputArray result) const
{
    Mat data = _data.getMat();
    CV_Assert( mean.data && eigenvectors.data &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols); //扩展mean矩阵到_data大小，
    int ctype = mean.type();
    if( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );//  data-mean
    }
    else
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    if( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );//输入矩阵乘以变换矩阵
    else
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}
```

```
void PCA::backProject(InputArray _data, OutputArray result) const //逆变换
{
    Mat data = _data.getMat();
    CV_Assert( mean.data && eigenvectors.data &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    if( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    else
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}
```

*参考：*
[PCA-SIFT: A More Distinctive Representation for Local Image Descriptors](http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=1315206)
[http://blog.csdn.net/songzitea/article/details/18270457](http://blog.csdn.net/songzitea/article/details/18270457)
[http://www.360doc.com/content/14/0526/06/15831056_380900310.shtml](http://www.360doc.com/content/14/0526/06/15831056_380900310.shtml)












