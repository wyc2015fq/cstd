# Matlab移植到Eigen用到的词条 - wishchinYang的专栏 - CSDN博客
2015年07月07日 17:49:13[wishchin](https://me.csdn.net/wishchin)阅读数：2760
         同型矩阵运算满足加法交换律、结合律；并存在单位元、逆元、和0元，为同型矩阵对加法的交换环。
         Eigen的简单运算参考：[http://blog.163.com/jiaqiang_wang/blog/static/1188961532013625102721873/](http://blog.163.com/jiaqiang_wang/blog/static/1188961532013625102721873/)
**一、For Eigen**
**1.零初始化、单位阵、全1矩阵**
```cpp
//初始化状态向量和状态协方差
int initialize_x_and_p(Eigen::MatrixXf   &x_k_k,Eigen::MatrixXf  &p_k_k)
{
	//Initial velocity values
	float	    v_0 = 0;
	float	std_v_0 = 0.025;
	float	w_0     = 1e-15;
	float	std_w_0 = 0.025; 
	// Initial state vector and covariance matrix
	x_k_k = Eigen::MatrixXf::Zero(1,13) ;
	x_k_k<<0,0,0,1,0,0,0,v_0,v_0,v_0,w_0,w_0,w_0;
	p_k_k= Eigen::MatrixXf::Zero(13,13);
	//eps为MATLAB的最小精度；eps是浮点相对误差限,是指计算机用于区分两个数的差的最小常数
	//最小精度eps(N)随着数量级增大，eps也在逐渐增大
	p_k_k(1,1)=eps;
	p_k_k(2,2)=eps;
	p_k_k(3,3)=eps;
	p_k_k(4,4)=eps;
	p_k_k(5,5)=eps;
	p_k_k(6,6)=eps;
	p_k_k(7,7)=eps;
	p_k_k(8,8)    =std_v_0 * std_v_0;
	p_k_k(9,9)    =std_v_0 * std_v_0;
	p_k_k(10,10)  =std_v_0 * std_v_0;
	p_k_k(11,11)  =std_w_0 * std_w_0;
	p_k_k(12,12)  =std_w_0 * std_w_0;
	p_k_k(13,13)  =std_w_0 * std_w_0;
	return 1;
}
```
**2. 矩阵元的运算**
size（）：获取矩阵的行数和列数 
r=size(A,1)该语句返回的时矩阵A的行数， c=size(A,2) 该语句返回的时矩阵A的列数。
size( yi, 2 )  转译为 yi.cols()
**3.随机矩阵生成**
```cpp
Matlab :   M= rand( 1, nPointsRand )-0.5;   //生成从0到1的nPointsRand个随机数，再减去0.5即：（-0.5，.0.5）
```
翻译为Eigen： 
```cpp
X=  Eigen::MatrixXf::Random(1,nPointsRand)  //默认生成 1行 nPointsRand列，率属于（-0.5，.0.5）的随机数
```
**4.矩阵复制及行列复制**
```cpp
<pre name="code" class="cpp">	int nPointsRand = 12;
	Eigen::MatrixXf X = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf Y = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf Z = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf theta  = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf phi    = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf lambda = Eigen::MatrixXf::Random(1,nPointsRand);
	Eigen::MatrixXf randSphere6D =Eigen::MatrixXf (6,nPointsRand);//randSphere6D 是6*nPointsRandDouble！
        //以下两种赋值方法都可以
	//randSphere6D(0)=X(0);
	//randSphere6D(1)=Y(0);
	//randSphere6D(2)=Z(0);
	//randSphere6D(3)= theta(0);
	//randSphere6D(4)=   phi(0);
	//randSphere6D(5)=lambda(0);
	randSphere6D<< X,Y,Z,theta,phi,lambda;
       //释放6个矩阵！
       X.resize(0,0);Y.resize(0,0);Z.resize(0,0);theta.resize(0,0);phi.resize(0,0);lambda.resize(0,0);
```
**5.四舍五入、ceil、floor**
```cpp
Matlab: round
```
**6.三角函数**
atan2(X,Y)的含义和atan(X/Y)的含义是一样的。就是求正切值 X/Y 对应的弧度值。
但ATAN2(a, b) 与 ATAN(a/b)值域稍有不同，ATAN2(a,b)的取值范围介于 -pi 到 pi 之间（不包括 -pi），而ATAN(a/b)的取值范围介于-pi/2到pi/2之间（不包括±pi/2)。
```cpp
atan2( hrl( 1, : ), hrl( 3, : ) )
```
翻译为Eigen：//wishchin！
```cpp
atan(  hrl.row( 1)*hrl.row( 3)/(hrl.row(1).norm()*hrl.row( 3).norm() )   )
```
**7.数组运算符，只是在矩阵运算符前面增加一点“.”来表示**
```cpp
SNIP........................
```
**8.矩阵整体运算**
```cpp
matlab  inv（M）
```
翻译为：
```cpp
Eigen： M.Array().inverse()// 值得注意的是：不能达到矩阵逆的实现
Eigen： M.inverse()        //此函数可以实现，但有时会失效，这是为何？
```
**注意事项：**
```cpp
<pre name="code" class="cpp">#include <Eigen/Dense>
```
```cpp
Eigen： M.inverse()        //包含此头文件才能实现矩阵求逆。
```
**9.多维重排**
函数功能：在[MATLAB](http://baike.baidu.com/view/10598.htm)、[FreeMat](http://baike.baidu.com/view/3688496.htm)中，该函数按指定的向量来重新排列一个[数组](http://baike.baidu.com/view/209670.htm)。
语法格式：
```cpp
B = permute(A, order) ;
```
按照向量order指定的顺序重排A的各维。B中元素和A中元素完全相同。
但由于经过重新排列，在A、B访问同一个元素使用的下标就不一样了。order中的元素必须各不相同。
**10.矩阵块的获取**
```cpp
Eigen： P.block(i, j, rows, cols)        
Matlab： P(i+1 : i+rows, j+1 : j+cols)
```
实例：
```cpp
Eigen::MatrixXf  P_xv(13,13); //= P( 1:13, 1:13 );
P_xv=P.block(0,0,13,13);
Eigen::MatrixXf  P_yxv = P.block(13,0,P.rows()-13,13 );// P( 14:end, 1:13 );
Eigen::MatrixXf  P_y   = P.block(13,13,P.rows()-13,P.cols()-13 );//P( 14:end, 14:end );
Eigen::MatrixXf  P_xvy = P.block(0,13,P.rows()-13,P.cols()-13 );//P( 1:13, 14:end );
```
**11.关于Matlab reshape**
B = reshape(A,m,n)
返回一个m*n的[矩阵](http://baike.baidu.com/view/10337.htm)B， B中元素是按列从A中得到的。如果A中元素个数没有m*n个， 则会引发错误。
引用方法：列优先，逐个复制 到新的矩阵.......
**12.Matlab dot(A,B)和cross(A,B)**
MATLAB中 dot(x, y)和cross(x, y)是向量内积和外积 
Matlab: 
```cpp
cross(x,y)
```
Eigen:
```cpp
x.cross(y)
```
~~ Eigen还提供了dot()点乘函数和cross()叉乘函数。其中dot可以针对任意两个长度相等的向量，而叉乘只能是两个三维向量，例如Vector3d v(1, 2, 3); Vector3d w(0, 1, 2); 那么v.dot(w) 得到的结果是8（等价于v.adjoint() * w），v.corss(w)得到的结果是(1;-2;1)。~~
13.对于这种形式：X_km1_k = [ Xv_km1_k; X_k( 14:end,: ) ];
直接对新矩阵进行添加...
**14.Matlab sparse**
Matlab里面有两个非常有用函数：randperm()与sparse()。
randperm(n)会返回一个1维数组（向量），该数组是整数1到n的一组随机排列，彼此无重复，顺序随机。
sparse(X)会返回一个矩阵X的稀疏矩阵的表达形式。在Matlab的Workspace中，稀疏矩阵的图标是![Matlab使用手记（一）——randperm与sparse](http://s7.sinaimg.cn/middle/489b422ag75a9212ea1f6&690)，不同于一般的结构体、矩阵等。
**15.Matlab eig函数**
Matlab里面的eig(M)函数给出了求特征值的方法：
```cpp
M = eig(S);
```
翻译成Eigen为：
```cpp
Eigen::MatrixXf S = features_info[i_feature].S;
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf>  eigensolver(S);
	Eigen::MatrixXf E = eigensolver.eigenvalues()
```
**16.相关系数 corrcoef**
Matlab相关系数的意义：
```cpp
Eigen::MatrixXf  correlation_matrix = corrcoef( LocM );
```
对行向量求相关系数 , 与列数无关，返回 cols()*cols() 矩阵...
翻译成Eigen：
还是自己写个函数吧
**//1.求协方差**
```cpp
Eigen::MatrixXf CIcSearchM::cov(Eigen::MatrixXf &d1, Eigen::MatrixXf &d2)
{
    Eigen::MatrixXf  CovM(1,1);
    assert(1 ==d1.cols() && 1 ==d2.cols() &&d1.cols()==d2.cols()  );
    //求协方差
    float Ex =0;float Ey=0;
    for (int i=0;i< d1.rows();++i){
        Ex +=d1(i);
        Ey +=d2(i);
    }
    Ex /=d1.rows();
    Ey /=d2.rows();
    for (int i=0;i< d1.rows();++i){
        CovM(0) += (d1(i)-Ex)*(d2(i)-Ey);
    }
    CovM(0) /= d1.rows() -1;
    return CovM;
}
```
**//2.写入方差矩阵**
```cpp
//求矩阵的相关系数！
//返回矩阵A的列向量的相关系数矩阵//对行向量求相关系数 , 与行数无关，返回 cols()*cols() 矩阵...
Eigen::MatrixXf CIcSearchM::corrcoef(Eigen::MatrixXf &M)
{
	// C(i,j)/SQRT(C(i,i)*C(j,j)).//C is the covariation Matrix
	int Row= M.rows();
	int Col= M.cols();
	int Order= Col;//int Order= (std::max)(Row,Col);
	Eigen::MatrixXf Coef(Order,Order);
	for (int i=0;i<Order;++i){
		for (int j=0;j<Order;++j){
			Coef(i,j)= cov((Eigen::MatrixXf)M.col(i),(Eigen::MatrixXf)M.col(j))(0);
		}
	}
	return Coef;
}
```
**17.Matlab logical函数**
Matlab logical：把非0元素转化为整数/bool型数1
```cpp
M =
    0.4300   -0.0300   -0.0400   -0.1600   -0.2500   -0.6500   -0.6700
   -0.0300    0.0400    0.1600    0.2500    0.6500    0.6700    0.2500
>> x =logical(M)
x =
     1     1     1     1     1     1     1
     1     1     1     1     1     1     1
>> M(2,2)=0
M =
    0.4300   -0.0300   -0.0400   -0.1600   -0.2500   -0.6500   -0.6700
   -0.0300         0    0.1600    0.2500    0.6500    0.6700    0.2500
<pre name="code" class="cpp">>> x =logical(M)
x =
     1     1     1     1     1     1     1
     1     0     1     1     1     1     1
```
转化为Eigen:
```cpp
//代替Matlab的逻辑转换函数！把非0值转换为1，0值为0!
Eigen::MatrixXf CRansHyper::logical(Eigen::MatrixXf &M)
{
	Eigen::MatrixXf MD(M.rows(),M.cols());
	for (int i=0;i<M.rows();++i)
	{
		for (int j=0;j< M.cols();++j)
		{
			if (M(i,j)!=0 ){
				MD(i,j) =1;
			}
			else
				MD(i,j) =0;
		}
	}
	return  MD;
}
```
**18.Matlab repmat函数**
repmat 重复填充一个小矩阵形成一个大矩阵
```cpp
>> M=[45,6,6;6,7,8]
M =
    45     6     6
     6     7     8
>> N =repmat(M,2,3)
N =
    45     6     6    45     6     6    45     6     6
     6     7     8     6     7     8     6     7     8
    45     6     6    45     6     6    45     6     6
     6     7     8     6     7     8     6     7     8
```
释义：把矩阵M(2,3)以两行三列的形式填充成大矩阵N(4,9)
转换成Eigen:
```cpp
//用小矩阵填充成大矩阵！wishchin！！
Eigen::MatrixXf CRansHyper::repmat(Eigen::MatrixXf &M,int rowM,int colM)
{
	Eigen::MatrixXf MD(rowM*M.rows(), colM*M.cols() );
	for (int i=0;i< rowM;++i){
		for (int j=0;j< colM;++j){
			for (int m=0;m< M.rows();++m){
				for (int n=0;n<M.cols();++n){
					MD(i*M.rows()+m, j*M.cols()+n)= M(m,n);
				}
			}
		}
	}
	return MD;
}
```
**19.Matlab 三角函数**
matlab三角函数对数组求值相当于对每一个元素求值再填入矩阵
```cpp
K>> x=[5,34,5,6]
x =
     5    34     5     6
K>> sin(x)
ans =
   -0.9589    0.5291   -0.9589   -0.2794
K>> sin(5)
ans =
   -0.9589
```
**20.Matlab  find函数**
matlab: 语法：
```cpp
/* 找出矩阵X中的所有非零元素，并将这些元素的线性索引值（linear indices：按列）返回到向量ind中。
如果X是一个行向量，则ind是一个行向量；否则，ind是一个列向量。
如果X不含非零元素或是一个空矩阵，则ind是一个空矩阵。 */
ind = find(X)
 /* 返回第一个非零元素k的索引值。
k必须是一个正数，但是它可以是任何数字数值类型。 */
 ind = find(X, k)
 /* 返回X中非零元素行索引值的向量*/
 ind = find(X, k, 'first') 
 /* 返回最后一个非零元素k的索引值。*/
ind = find(X, k, 'last')
 /* 返回矩阵X中非零元素的行和列的索引值。
这个语法对于处理稀疏矩阵尤其有用。
如果X是一个N（N>2）维矩阵，col包括列的线性索引。
例如，一个5*7*3的矩阵X，有一个非零元素X（4,2,3），find函数将返回row=4和col=16。也就是说，（第1页有7列）+（第2页有7列）+（第3页有2列）=16。*/
 [row,col] = find(X, ...)
 /* 返回X中非零元素的一个列或行向量v，同时返回行和列的索引值。
如果X是一个逻辑表示，则v是一个逻辑矩阵。
输出向量v(稀疏矩阵)包含通过评估X表示得到的逻辑矩阵的非零元素。*/
  [row,col,v] = find(X, ...)
```
改写其中的一个：
```cpp
/* 找出矩阵X中的所有非零元素，并将这些元素的线性索引值（linear indices：按列）返回到向量中。
如果X是一个行向量，则ind是一个行向量；否则，ind是一个列向量。
如果X不含非零元素或是一个空矩阵，则ind是一个空矩阵。 */
//默认返回大于0的值//为X *1的矩阵，行优先！
Eigen::MatrixXf CEkfSlam::find(Eigen::MatrixXf  &M)
{
	assert(1 == M.rows() );
	int counter =0;
	//返回数组中大于value的下标，返回索引
	//使用Push太慢了，还是遍历两次吧
	for (int i =0;i< M.cols();++i)
	{
		if (M(i)> 1)
		{
			++counter;
		}
	}
	Eigen::MatrixXf  index(counter,1);
	counter=0;
	for (int i =0;i< M.cols();++i)
	{
		if (M(i)> 1)
		{
			M(counter) =counter;
			++counter;
		}
	}
	return index;
}
```
**二、For  OpenCV—Mat**
**1. 三角函数**
atan2(X,Y)的含义和atan(X/Y)的含义是一样的。就是求正切值 X/Y 对应的弧度值。
但ATAN2(a, b) 与 ATAN(a/b)值域稍有不同，ATAN2(a,b)的取值范围介于 -pi 到 pi 之间（不包括 -pi），而ATAN(a/b)的取值范围介于-pi/2到pi/2之间（不包括±pi/2)。
```cpp
atan2( hrl( 1, : ), hrl( 3, : ) )
```
**2.图像块获取**
Matlab可以直接使用：和索引获取矩阵块，
翻译成OpenCV：
```cpp
//使用OpenCV getRect函数 //GetRectSubPix 从图像中提取象素矩形，使用子象素精度 
std::vector<CvPoint>  cs(0);
cv::Mat             RectImg; 
const double     Thres =100;
CvSize   Rect;
Rect.height=initializing_box_semisize.height;
Rect.width =initializing_box_semisize.width ;
cv::getRectSubPix(im_k,Rect,search_region_center,RectImg,-1);
```
**3.Matlab到OpenCV Mat型的转化**
     OpenCV的Mat型提供了各种矩阵运算，其中一部分如下
```cpp
static Matx all(_Tp alpha);
    static Matx zeros();
    static Matx ones();
    static Matx eye();
    static Matx <span style="color:#FF0000;">diag</span>(const diag_type& d);
    static Matx randu(_Tp a, _Tp b);
    static Matx randn(_Tp a, _Tp b);
    //! dot product computed with the default precision
    _Tp dot(const Matx<_Tp, m, n>& v) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Matx<_Tp, m, n>& v) const;
    //! conversion to another data type
    template<typename T2> operator Matx<T2, m, n>() const;
    //! change the matrix shape
    template<int m1, int n1> Matx<_Tp, m1, n1> reshape</span>() const;
    //! extract part of the matrix
    template<int m1, int n1> Matx<_Tp, m1, n1> get_minor(int i, int j) const;
    //! extract the matrix row
    Matx<_Tp, 1, n> row(int i) const;
    //! extract the matrix column
    Matx<_Tp, m, 1> col(int i) const;
    //! extract the matrix diagonal
    diag_type diag() const;
    //! transpose the matrix
    Matx<_Tp, n, m> t() const;
    //! invert matrix the matrix
    Matx<_Tp, n, m> inv (int method=DECOMP_LU) const;
    //! solve linear system
    template<int l> Matx<_Tp, n, l> solve(const Matx<_Tp, m, l>& rhs, int flags=DECOMP_LU) const;
    Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const;
    //! multiply two matrices element-wise
    Matx<_Tp, m, n> mul(const Matx<_Tp, m, n>& a) const;
    Vec cross(const Vec& v) const;
```
**三、for C++**
**1.关于运算符重载**
        要把需要重载的运算符的函数体，写在CPP文件里。
**2.生成随机数**
```cpp
Y = rand(n)
Y = rand(m,n)
Y = rand([m n])
Y = rand(m,n,p,...)
Y = rand([m n p...])
Y = rand(size(A))
```
 rand
s = rand('state')
描述：
rand函数产生由在(0, 1)之间均匀分布的随机数组成的数组。
```cpp
Y = rand(n) 返回一个n x n的随机矩阵。如果n不是数量，则返回错误信息。
Y = rand(m,n) 或 Y = rand([m n]) 返回一个m x n的随机矩阵。
Y = rand(m,n,p,...) 或 Y = rand([m n p...]) 产生随机数组。
Y = rand(size(A)) 返回一个和A有相同尺寸的随机矩阵。
```
翻译成C++：
```cpp
v1 = rand() % 100;         // v1 in the range 0 to 99
    v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
    v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
```
**3.Matlab meshgrid**
对于数据：
uv_predSrc:
```cpp
47.9999999999998    48.9999999999998    49.9999999999998    50.9999999999998    51.9999999999998    52.9999999999998    53.9999999999998
```
为 1*7 的矩阵
经过转换：
```cpp
[u_pred,v_pred]=meshgrid( uv_predSrc );
```
获取的结果为：
u_pred：
```cpp
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
47.9999999999998	48.9999999999998	49.9999999999998	50.9999999999998	51.9999999999998	52.9999999999998	53.9999999999998
```
v_pred：
```cpp
47.9999999999998	47.9999999999998	47.9999999999998	47.9999999999998	47.9999999999998	47.9999999999998	47.9999999999998
48.9999999999998	48.9999999999998	48.9999999999998	48.9999999999998	48.9999999999998	48.9999999999998	48.9999999999998
49.9999999999998	49.9999999999998	49.9999999999998	49.9999999999998	49.9999999999998	49.9999999999998	49.9999999999998
50.9999999999998	50.9999999999998	50.9999999999998	50.9999999999998	50.9999999999998	50.9999999999998	50.9999999999998
51.9999999999998	51.9999999999998	51.9999999999998	51.9999999999998	51.9999999999998	51.9999999999998	51.9999999999998
52.9999999999998	52.9999999999998	52.9999999999998	52.9999999999998	52.9999999999998	52.9999999999998	52.9999999999998
53.9999999999998	53.9999999999998	53.9999999999998	53.9999999999998	53.9999999999998	53.9999999999998	53.9999999999998
```
程序功能为：
        把一系列下标转化为两个下标方阵，前一个为行复制，后一个为列复制。
转换为C++写一个循环就可以了.
```cpp
//返回两个行列方阵矩阵
double  CIcSearchM::meshgrid(
	Eigen::MatrixXf &MSrc,Eigen::MatrixXf &MU,Eigen::MatrixXf &MV)
{
	Eigen::MatrixXf MSrcCol(MSrc.rows() ,1);
	for (int i =0;i< MSrc.rows();++i)
	{
		MSrcCol(i) = MSrc(i);
	}
	MU.resize( MSrc.rows(), MSrc.rows() );
	MV.resize( MSrc.rows(), MSrc.rows() );
	for (int i=0;i< MSrc.rows();++i)
	{
		MU.row(i) = MSrc.row(0);
		MV.col(i) = MSrcCol.col(0);
	}
	return 1;
}
```
**4.Matlab interp2**

**四、for  OpenGL**
**1.画出射线 **
**Matlab:**
```cpp
function vectarrow(p0,p1,color)
  if max(size(p0))==3
      if max(size(p1))==3
          x0 = p0(1);
          y0 = p0(2);
          z0 = p0(3);
          x1 = p1(1);
          y1 = p1(2);
          z1 = p1(3);
          plot3([x0;x1],[y0;y1],[z0;z1],'color',color);   % Draw a line between p0 and p1
```
 画出从p0到p1的射线
转换为OpenGL：
一大堆代码：
```cpp
</pre><pre name="code" class="cpp">this->vectArrow(PointS,PointE,ColorType);
int CPlot::vectArrow(
    Eigen::MatrixXf  &PointS, Eigen::MatrixXf  &PointE,std::string type)
{
    Eigen::MatrixXf  EndPoints(6,1);
    EndPoints<<
        PointS(0),PointS(1),PointS(2),
        PointE(0),PointE(1),PointE(2);
    this->openglViewer->GetVectorPoint(EndPoints);//
    return 1;
}
drawTransAxis(this->m_PointSeries);
int OpenGLViewer::GetVectorPoint(Eigen::MatrixXf  &endPoints)
 {
     Eigen::MatrixXf NewEndPoint= endPoints;
     this->m_PointSeries.push_back( NewEndPoint);
     return 1;
 }
void OpenGLViewer::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->drawWorldAxis();
    this->drawTransAxis(this->m_PointSeries);
    this->drawTriangle(this->m_TriangleSeries);
    this->drawCloud();
}
void OpenGLViewer::drawTransAxis(std::vector<Eigen::MatrixXf>  &PointSeries)
{
    GLfloat x = GLfloat(640) / 480;
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    gluLookAt(-2.0 , 6.0, -4.0, 0.0 , 0.0 , 0.0 , 0.0, 1.0 , 0.0);
    float len = 0.2;
    glTranslatef(transX,transY,-transZ);
    glRotatef(rotationX , 1.0,0.0,0.0);
    glRotatef(rotationY , 0.0,1.0,0.0);
    glRotatef(rotationZ , 0.0,0.0,1.0);
    glScalef(xscale, yscale, zscale);
    //The Cube aixs / coordinate system！
    //1. The axis line! 
    glColor3f(1.0f,1.0f,1.0f);  
    glBegin(GL_LINES);  
    glVertex3f(-1.9f,00.0f,0.0f);  
    glVertex3f(1.9f,0.0f,0.0f);  
    glVertex3f(0.0f,-1.9f,0.0f);  
    glVertex3f(0.0f,1.9f,0.0f);  
    glVertex3f(0.0f,0.0f,-1.9f);  
    glVertex3f(0.0f,0.0f,1.9f);  
    //画出多个线段 
    for (int i=0;i< PointSeries.size();++i)
    {
        glVertex3f(PointSeries[i](0),PointSeries[i](1),PointSeries[i](2) );  
        glVertex3f(PointSeries[i](3),PointSeries[i](4),PointSeries[i](5) ); 
    }
    glEnd();  
    //The Cube aixs / coordinate system！
    //2. The axis arrow!
    glColor3f(1.0f,0.0f,0.0f); //x arrow
    glPushMatrix();  
    glTranslatef(1.2f,0.0f,0.0f);  
    glRotatef(90.0f,0.0f,1.0f,0.0f);  
    glutSolidCone(0.05,0.15,10,10);  
    glPopMatrix();  
    glColor3f(0.0f,1.0f,0.0f); // y  
    glPushMatrix();  
    glTranslatef(0.0f,1.2f,0.0f);  
    glRotatef(-90.0f,1.0f,0.0f,0.0f);  
    glutSolidCone(0.05,0.15,10,10);  
    glPopMatrix();  
    glColor3f(0.0f,0.0f,1.0f); // z  
    glPushMatrix();  
    glTranslatef(0.0f,0.0f,1.2f);  
    glRotatef(90.0f,0.0f,0.0f,1.0f);  
    glutSolidCone(0.05,0.15,10,10);  
    glPopMatrix(); 
    glFlush();
}
```
**五、常见错误集合**
1. 错误    52    error C2338: THIS_METHOD_IS_ONLY_FOR_VECTORS_OF_A_SPECIFIC_SIZE    c:\tools\eigen\include\eigen\src\core\matrix.h    265
引发错误的代码段：
```cpp
Eigen::MatrixXf x_features(1,x_k_km1.rows()-13,x_k_km1.cols() );// = x_k_km1(14:end);
x_features =x_k_km1.block(13,0,x_k_km1.rows()-13 ,x_k_km1.cols() );
```
错误原因：使用了三个参数，真郁闷...
```cpp
(1,x_k_km1.rows()-13,x_k_km1.cols() )
```
修改为：
```cpp
(x_k_km1.rows()-13,x_k_km1.cols() )
```
就可以了！
