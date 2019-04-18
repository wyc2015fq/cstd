# OpenCV矩阵运算 - 文章 - 伯乐在线
原文出处： [_chease，2013-03-19](http://blog.sina.com.cn/s/blog_afe2af380101bqhz.html)
#### 一、矩阵
Mat I,img,I1,I2,dst,A,B;
double k,alpha;
Scalar s;
**1.加法**


```
I=I1+I2;//等同add(I1,I2,I);
add(I1,I2,dst,mask,dtype);
scaleAdd(I1,scale,I2,dst);//dst=scale*I1+I2;
```
**2.减法**


```
absdiff(I1,I2,I);//I=|I1-I2|;
A-B;A-s;s-A;-A;
subtract(I1,I2,dst);
```
**3.乘法**


```
I=I.mul(I);//点乘,I.mul(I,3);-->I=3*I.^2
Mat C=A.mul(5/B);//==divide(A,B,C,5);
A*B;//矩阵相乘
I=alpha*I;
Mat::cross(Mat);//三维向量(或矩阵)的叉乘,A.cross(B)
double Mat::dot(Mat);//2个向量(或矩阵)的点乘的结果,A.dot(B)
mul-------multiply
pow(src,double p,dst);//如果p是整数dst(I)=src(I)^p;其他|src(I)|^p
```
**4.除法**


```
divide(I1,I2,dst,scale,int dtype=-1);//dst=saturate_cast(I1*scale/I2);
A/B;alpha/A;都是点除
```
**5.转换**


```
I.convertTo(I1,CV_32F);//类型转换
A.t();//转置
flip(I,dst,int flipCode);//flipCode=0是上下翻转，>0时左右翻转,<0时一起来
sqrt(I,dst);
cvtColor(I,dst,int code,int dstCn=0);
resize//对图像进行形变
```
**6.其他**


```
Scalar s=sum(I);各通道求和
norm,countNonZero,trace,determinant,repeat//都是返回Mat或者Scalar
countNonZero//用来统计非零的向量个数.(rows*cols个)
Scalar m=mean(I);//各通道求平均
Mat RowClone=C.row(1).clone();//复制第2行
addWeight(I1,alpha,I2,beta,gamma,dst,int dtype=-1);//dst=saturate(alpha*I1+beta*I2+gamma);//dtype是dst的深度
```
**7.运算符**


```
log10()
exp(I,dst);//dst=exp(I);计算每个数组元素的指数
log(I,dst);//如果Iij!=0;则dstij=log(|Iij|)
randu(I,Scalar::all(0),Scalar::all(255));
Mat::t()//转置
Mat::inv(int method=DECOMP_LU)//求逆。method=DECOMP_CHOLESKY(专门用于对称，速度是LU的2倍),DECOMP_SVD//A.inv();A.inv()*B;
invert(I1,dst,int method=DECOMP_LU);//用法同上
MatExpr abs(Mat)//求绝对值
A cmpop B;A compop alpha;alpha cmpop A;//这里cmpop表示>,>=,==,!=,<=,<等，结果是CV_8UC1的mask的0或255
A logicop B;A logicop s;s logicop A;~A;//按位运算,这里logicop代表&,|,^
bitwise_not(I,dst,mask);//inverts所有的队列
bitwise_and,bitwise_or,bitwise_xor,
min(A,B);min(A,alpha);max(A,B);max(A,alpha);//都返回MatExpr,返回的dst和A的类型一样
double determinant(Mat);//行列式
bool eigen(I1,dst,int lowindex=-1,int highindex=-1);//
bool eigen(I1,dst,I,int...);//得到特征值向量dst和对应特征值的特征向量
minMaxLoc(I1,&minVal,&maxVal,Point *minLoc=0,Point* MaxLoc=0,mask);
//minLoc是2D时距原点最小的点(未考证)
```
**8.初始化**


```
Mat I(img,Rect(10,10,100,100));//用一块地方初始化。
Mat I=img(Range:all(),Range(1,3));//所有行，1~3列
Mat I=img.clone();//完全复制
img.copyTo(I);//传递矩阵头
Mat I(2,2,CV_8UC3,Scalar(0,0,255));//I=[0,0,255,0,0,255;0,0,255,0,0,255];
Mat E=Mat::eye(4,4,CV_64F);//对角矩阵
Mat O=Mat::ones(2,2,CV_32F);//全一矩阵
Mat Z=Mat::zeros(3,3,CV_8UC1);//全零矩阵
Mat C=(Mat_(2,2)<<0,-1,2,3);//如果是简单矩阵的初始化
Mat::row(i);Mat::row(j);Mat::rowRange(start,end);Mat::colRange(start,end);//都只是创建个头
Mat::diag(int d);//d=0是是主对角线，d=1是比主低的对角线,d=-1....
static Mat Mat::diag(const Mat& matD)
Mat::setTo(Scalar &s);//以s初始化矩阵
Mat::push_back(Mat);//在原来的Mat的最后一行后再加几行
Mat::pop_back(size_t nelems=1);//移出最下面几行
```
**9.矩阵读取和修改**
(1)1个通道：


```
for(int i=0;i
for(int j=0;j
I.at(i,j)=k;
```
(2)3个通道：


```
Mat_ _I=I;//他没有4个通道寸，只有3个通道！
for(int i=0;i
for(int j=0;j
{
_I(i,j)[0]=b;
_I(i,j)[1]=g;
_I(i,j)[2]=r;
}
I=_I;
```
或者直接用I.at(i,j)[0]….


```
float *s;
for(i=0;i
{s=proImg.ptr(i);
for(j=0;j
{a1=s[3*j+1]-m1;
a2=s[3*j+2]-m2;}}
```
(3)其他机制


```
I.rows(0).setTo(Scalar(0));//把第一行清零
saturate_cast(...);//可以确保内容为0~255的整数
Mat::total();//返回一共的元素数量
size_t Mat::elemSize();//返回元素的大小:CV_16SC3-->3*sizeof(short)-->6
size_t Mat::elemSize1();//返回元素一个通道的大小CV_16SC3-->sizeof(short)-->2
int Mat::type()//返回他的类型CV_16SC3之类
int Mat::depth()//返回深度:CV_16SC3-->CV_16S
int Mat::channels()//返回通道数
size_t Mat:step1()//返回一个被elemSize1()除以过的step
Size Mat::size()//返回Size(cols,rows);如果大于2维，则返回(-1,-1)，都是先宽再高的
bool Mat::empty()//如果没有元素返回1,即Mat::total()==0或者Mat::data==NULL
uchar *Mat::ptr(int i=0)//指向第i行
Mat::at(int i)(int i,int j)(Point pt)(int i,int j,int k)
next,float RNG::uniform(float a,float b);//RNG随机类
double RNG::gaussian(double sigma);
RNG::fill(I,int distType,Mat low,Mat up);//用随机数填充
randu(I,low,high);
randn(I,Mat mean,Mat stddev);
reduce(I,dst,int dim,int reduceOp,int dtype=-1);//可以统计每行或每列的最大、最小、平均值、和
setIdentity(dst,Scalar &value=Scalar(1));//把对角线替换为value
//效果等同：Mat A=Mat::eye(4,3,CV_32F)*5;
```
**10.较复杂运算**


```
gemm(I1,I2,alpha,I3,beta,dst,int flags=0);//I1至少是浮点型,I2同I1,flags用来转置
//gemm(I1,I2,alpha,I3,beta,dst,GEMM_1_T,GEMM_3_T);-->dst=alpha*I1.t()*I2+beta*I3.t();//可用此完全代替此函数
mulTransposed(I,dst,bool aTa,Mat delta=noArray(),double scale=1,int rtype=-1);
//I是1通道的,和gemm不同,他可用于任何类型。
//如果aTa=flase时,dst=scale*(I-delta).t()*(I-delta);
//如果是true,dst=scale*(I-delta)(I-delta).t();
calcCovarMatrix(Mat,int,Mat,Mat,int,int=);calcCovarMatrix(Mat I,Mat covar,Mat mean,int flags,int=);
cartToPolar//转到极坐标
compare(I1,I2,dst,cmpop);cmpop=CMP_EQ,CMP_GT,CMP_GE,CMP_LT,CMP_LE,COM_NE
completeSymm(M,bool lowerToUpper=false);当lowerToUpper=true时Mij=Mji(ij)
convertScaleAbs(I,dst,alpha,beta);dst=saturate_cast(|alpha*I+beta|);//变成可显示图像
dct(I,dst,int flags=0);//DCT变换，1维、2维的矩阵;flags=DCT_INVERSE,DCT_ROWS
idct,dft,idft
inRange(I1,I_low,I_up,dst);//dst是CV_8UC1,在2者之间就是255
Mahalanobis(vec1,vec2,covar);
merge(vector,Mat);//把多个Mat组合成一个和split相反
double norm(...)//当src2木有时,norm可以计算出最长向量、向量距离和、向量距离和的算术平方根
//solveCubic解3次方程，solvePoly解n次方程
sort,sortIdx//排列
mixChannels();//对某个通道进行各种传递
```
**11.未懂的函数**


```
getConvertElem,extractImageCOI,LUT
magnitude(x,y,dst);//I1,I2都是1维向量,dst=sqrt(x(I)^2+y(I)^2);
meanStdDev,
MulSpectrums(I1,I2,dst,flags);//傅里叶
normalize(I,dst,alpha,beta,int normType=NORM_L2,int rtype=-1,mask);//归一化
PCA,SVD,solve,transform,transpose
```
#### 二、其他数据结构


```
Point2f P(5,1);
Point3f P3f(2,6,7);
vector v;v.push_back((float)CV_PI);v.push_back(2);v.push_back(3.01f);//不断入
vector vPoints(20);//一次定义20个
```
#### 三、常用方法


```
Mat mask=src<0;//这样很快建立一个mask了
```
四、以后可能用到的函数
randShuffle,repeat
