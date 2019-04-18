# Matlab矩阵填充--Matlab interp2 - wishchinYang的专栏 - CSDN博客
2015年12月14日 12:02:51[wishchin](https://me.csdn.net/wishchin)阅读数：4940
    Matlab interp2 为Matlab的矩阵填充函数，
填充关系：
```
x=1:11;  
y=1:13;  
x1=1:0.1:12;  
y1=1:0.1:14;  
[x2,y2]=meshgrid(x1,y1); 
t1=interp2(x,y,t,x2,y2,'cubic');
```
意义：
进行十倍差值，使用**双三次插值** 方法。
**用指定的算法method 计算二维插值：’linear’：双线性插值算法(缺省算法);’nearest’：最临近插值;’spline’：三次样条插值;’cubic’：双三次插值。book.iLoveMatlab.cn**
相关知识
    参考链接：[http://blog.sina.com.cn/s/blog_5cd733a50100o4d8.html](http://blog.sina.com.cn/s/blog_5cd733a50100o4d8.html)
    在生产和科学实验中，自变量 与因变量 间的函数关系有时不能写出解析表达式，而只能得到函数在若干点的函数值或导数值，或者表达式过于复杂而需要较大的计算量。当要求知道其它点的函数值时，需要估计函数值在该点的值。
    为了完成这样的任务，需要构造一个比较简单的函数，使函数在观测点的值等于已知的值，或使函数在该点的导数值等于已知的值，寻找这样的函数有很多方法。根据测量数据的类型有以下两类处理观测数据的方法。
（1）测量值是准确的，没有误差，一般用插值。
（2）测量值与真实值有误差，一般用曲线拟合。
在MATLAB中，无论是插值还是拟合，都有相应的函数来处理。
**一、插值**
**1、一维插值**：
已知离散点上的数据集 ，即已知在点集X= 上的函数值Y=，构造一个解析函数（其图形为一曲线）通过这些点，并能够求出这些点之间的值，这一过程称为一维插值。
MATLAB命令：yi=interp1(X, Y, xi, method)
该命令用指定的算法找出一个一元函数 ，然后以 给出处的值。xi可以是一个标量，也可以是一个向量，是向量时，必须单调，method可以下列方法之一：
‘nearest’：最近邻点插值，直接完成计算；
‘spline’：三次样条函数插值；
‘linear’：线性插值（缺省方式），直接完成计算；
‘cubic’：三次函数插值；
对于[min{xi},max{xi}]外的值，MATLAB使用外推的方法计算数值。
例1：已知某产品从1900年到2010年每隔10年的产量为：75.995, 91.972, 105.711, 123.203,131.699, 150.697, 179.323, 203.212, 226.505, 249.633, 256.344,267.893，计算出1995年的产量，用三次样条插值的方法，画出每隔一年的插值曲线图形，同时将原始的数据画在同一图上。
解：程序如下
```
year=1900:10:2010;
product=[75.995, 91.972, 105.711, 123.203, 131.699, 150.697,179.323, 203.212, 226.505, 249.633, 256.344, 267.893]
p1995=interp1(year,product,1995)
x=1900:2010;
y=interp1(year,product,x,'cubic');
plot(year,product,'o',x,y);
```
计算结果为：
`p1995=252.9885。`
**2、二维插值**
    已知离散点上的数据集 ，即已知在点集 上的函数值，构造一个解析函数（其图形为一曲面）通过这些点，并能够求出这些已知点以外的点的函数值，这一过程称为二维插值。
    MATLAB函数：`    Zi=interp2(X,Y,Z,Xi,Yi,method)`
    该命令用指定的算法找出一个二元函数 ，然后以 给出 处的值。返回数据矩阵 ，Xi，Yi是向量，且必须单调，和meshgrid(Xi,Yi)是同类型的。
    method可以下列方法之一：
‘nearest’：最近邻点插值，直接完成计算；
‘spline’：三次样条函数插值；
‘linear’：线性插值（缺省方式），直接完成计算；
‘cubic’：三次函数插值；
例2：已知1950年到1990年间每隔10年，服务年限从10年到30年每隔10年的劳动报酬表如下：
表：某企业工作人员的月平均工资（元）
年份 1950 1960 1970 1980 1990
服务年限
10 150.697 179.323 203.212 226.505 249.633
20 169.592 195.072 239.092 273.706 370.281
30 187.652 250.287 322.767 426.730 598.243
试计算1975年时，15年工龄的工作人员平均工资。
解：程序如下：
```
years=1950:10:1990;
service=10:10:30;
wage=[150.697 169.592 187.652
179.323 195.072 250.287
203.212 239.092 322.767
226.505 273.706 426.730
249.633 370.281 598.243];
mesh(service,years,wage) %绘原始数据图
w=interp2(service,years,wage,15,1975); %求点(15,1975)处的值
```
计算结果为：235.6288
例3：设有数据x=1,2,3,4,5,6，y=1,2,3,4，在由x,y构成的网格上，数据为：
12,10,11,11,13,15
16,22,28,35,27,20
18,21,26,32,28,25
20,25,30,33,32,20
求通过这些点的插值曲面。
**解**：程序为：
```
x=1:6;
y=1:4;
t=[12,10,11,11,13,15
16,22,28,35,27,20
18,21,26,32,28,25;
20,25,30,33,32,20]
subplot(1,2,1)
mesh(x,y,t)
x1=1:0.1:6;
y1=1:0.1:4;
[x2,y2]=meshgrid(x1,y1);
t1=interp2(x,y,t,x2,y2,'cubic');
subplot(1,2,2)
mesh(x1,y1,t1);
```
结果如下图。
![](https://img-blog.csdn.net/20151214151531009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
作业：已知某处山区地形选点测量坐标数据为：
x=0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5
y=0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6
海拔高度数据为：
z=89 90 87 85 92 91 96 93 90 87 82
92 96 98 99 95 91 89 86 84 82 84
96 98 95 92 90 88 85 84 83 81 85
80 81 82 89 95 96 93 92 89 86 86
82 85 87 98 99 96 97 88 85 82 83
82 85 89 94 95 93 92 91 86 84 88
88 92 93 94 95 89 87 86 83 81 92
92 96 97 98 96 93 95 84 82 81 84
85 85 81 82 80 80 81 85 90 93 95
84 86 81 98 99 98 97 96 95 84 87
80 81 85 82 83 84 87 90 95 86 88
80 82 81 84 85 86 83 82 81 80 82
87 88 89 98 99 97 96 98 94 92 87
1、 画出原始数据图;
2、 画出加密后的地貌图，并在图中标出原始数据
```
x=1:11;  
y=1:13;  
t=[
89, 90, 87, 85, 92, 91, 96, 93 ,90 ,87, 82,
92 ,96 ,98 ,99 ,95 ,91, 89, 86 ,84, 82 ,84,
96 ,98 ,95 ,92 ,90, 88 ,85 ,84, 83, 81, 85,
80 ,81, 82, 89, 95 ,96 ,93 ,92, 89, 86, 86,
82 ,85, 87, 98, 99, 96, 97 ,88 ,85 ,82, 83,
82 ,85 ,89 ,94, 95, 93, 92, 91, 86 ,84, 88,
88 ,92, 93, 94, 95, 89 ,87 ,86, 83, 81, 92,
92 ,96 ,97 ,98, 96, 93, 95, 84, 82, 81, 84,
85 ,85, 81, 82, 80 ,80, 81, 85, 90, 93, 95,
84 ,86 ,81 ,98 ,99 ,98, 97, 96, 95, 84, 87,
80, 81, 85 ,82, 83 ,84 ,87 ,90, 95 ,86, 88,
80, 82, 81 ,84, 85 ,86, 83, 82, 81, 80, 82,
87 ,88 ,89 ,98 ,99, 97 ,96, 98, 94, 92, 87]  
subplot(1,2,1)  
mesh(x,y,t)  
x1=1:0.1:12;  
y1=1:0.1:14;  
[x2,y2]=meshgrid(x1,y1);  
t1=interp2(x,y,t,x2,y2,'cubic');  
subplot(1,2,2)  
mesh(x1,y1,t1);
```
结果如下图：
![](https://img-blog.csdn.net/20151214153626761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**二、使用opencv取代**
使用opencv改写，函数为：
```cpp
//使用OpenCV方法进行插值
//使用opencv resize函数
//Matlab函数！//function zi = interp2(varargin)
//varargin[0]:  Src AxisX
//varargin[1]:  Src AxisY
//varargin[2]:  SrcData
//varargin[3]:  Dst AxisX
//varargin[4]:  Dst AxisY
//method: interpolation method
Eigen::MatrixXf CIcSearchM::interp2Cv(
	std::vector<Eigen::MatrixXf> &varargin,
	int  method)
{
	assert(varargin.size() >=   5);
	Eigen::MatrixXf zi;
	if (0 ==varargin[0].size() ||	0 ==varargin[1].size()
		|| 0 ==varargin[3].size() ||	0 ==varargin[4].size())
	{
		zi.resize(0,0);
		return zi;
	} 
	else
	{
		zi.resize(varargin[3].size() , varargin[4].size());
		if (varargin[2].rows()!= zi.rows() ||  varargin[2].cols()!=zi.cols()  )
		{
			zi.resize(0,0);
			return zi;
		} 
		else
		{
			//Eigen::MatrixXf Mat; 
			cv::Mat Img;
			cv::Mat ImgRe(zi.rows(),zi.cols(),CV_32F);
			m_Slamer.copyCvMat( varargin[3], Img);
			cv::resize( Img, ImgRe,ImgRe.size(),0,0,method );
			m_Slamer.copyCvMat( ImgRe, zi);
		}
	}
	return zi;
}
```
使用方法：
```cpp
std::vector<Eigen::MatrixXf>  varargin;
		varargin.push_back(u_fea);
		varargin.push_back(v_fea);
		varargin.push_back(patch_p_f); 
		varargin.push_back(u_pred_imak_dist);;
		varargin.push_back(v_pred_imak_dist );;
		patch_pred = this->interp2Cv( varargin,CV_INTER_CUBIC);
```
void cvResize( const CvArr* src, CvArr* dst, int interpolation=CV_INTER_LINEAR );src输入图像.dst输出图像.interpolation插值方法:
- CV_INTER_NN - 最近邻插值,
- CV_INTER_LINEAR - 双线性插值 (缺省使用)
- CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大，类似于 CV_INTER_NN 方法..
- CV_INTER_CUBIC - 立方插值.
函数 cvResize 将图像 src 改变尺寸得到与 dst 同样大小。若设定 ROI，函数将按常规支持 ROI.
