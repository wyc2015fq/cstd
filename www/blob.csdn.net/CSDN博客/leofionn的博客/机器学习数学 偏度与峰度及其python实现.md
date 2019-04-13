
# 机器学习数学|偏度与峰度及其python实现 - leofionn的博客 - CSDN博客


2018年06月04日 11:52:23[leofionn](https://me.csdn.net/qq_36142114)阅读数：420


目录
[偏度(skewness)和峰度(kurtosis)：](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label0)
[偏度的定义：](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label1)
[样本偏度的计算方法：](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label2)
[峰度的定义：](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label3)
[样本的峰度计算方法：](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label4)
[python使用pandas来计算偏度和峰度](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label5)[   Kurtosis  维基百科给出峰度的计算公式](https://www.cnblogs.com/jiaxin359/p/8977333.html#_label5_0)

正文
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## 偏度(skewness)和峰度(kurtosis)：
偏度能够反应分布的对称情况，右偏（也叫正偏），在图像上表现为数据右边脱了一个长长的尾巴，这时大多数值分布在左侧，有一小部分值分布在右侧。
峰度反应的是图像的尖锐程度：峰度越大，表现在图像上面是中心点越尖锐。在相同方差的情况下，中间一大部分的值方差都很小，为了达到和正太分布方差相同的目的，必须有一些值离中心点越远，所以这就是所说的“厚尾”，反应的是异常点增多这一现象。
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## 偏度的定义：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501214401604-89047351.png)
样本X的偏度为样本的三阶标准矩
其中μμ是均值，δδ为标准差，E是均值操作。μ3μ3是三阶中心距，κtκt是tthtth累积量
偏度可以由三阶原点矩来进行表示：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501214850772-1964566790.png)
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## 样本偏度的计算方法：
一个容量为n的数据，一个典型的偏度计算方法如下：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501214851667-1460260549.png)
其中x¯x¯为样本的均值（和μμ的区别是，μμ是整体的均值，x¯x¯为样本的均值）。s是样本的标准差，m3m3是样本的3阶中心距。
另外一种定义如下：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501221738722-1218524864.png)
k3k3是三阶累积量κ3κ3的唯一对称无偏估计(unique symmetric unbiased estimator)（k3k3和κ3κ3写法不一样）。k2=s2k2=s2是二阶累积量的对称无偏估计。
大多数软件当中使用G1G1来计算skew，如Excel，Minitab，SAS和SPSS。
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## 峰度的定义：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501222450299-1166771113.png)
峰度定义为四阶标准矩，可以看出来和上面偏度的定义非常的像，只不过前者是三阶的。
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## 样本的峰度计算方法：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180501222450800-1166177054.png)
样本的峰度还可以这样计算：
![image](https://images2018.cnblogs.com/blog/890640/201805/890640-20180502103440884-593975463.png)
其中k4k4是四阶累积量的唯一对称无偏估计，k2k2是二阶累积量的无偏估计（等同于样本方差），m4m4是样本四阶平均距，m2m2是样本二阶平均距。
同样，大多数程序都是采用G2G2来计算峰度。
[回到顶部](https://www.cnblogs.com/jiaxin359/p/8977333.html#_labelTop)
## python使用pandas来计算偏度和峰度
importpandas as pd
x= [53, 61, 49, 66, 78, 47]
s=pd.Series(x)print(s.skew())print(s.kurt())
它是用上面的G1G1来计算偏度G2G2来计算峰度，结果如下：
0.7826325504212567
-0.2631655441038463



# 矩
对于随机变量X,X的K阶原点矩为
E(Xk)E(Xk)
X的K阶中心矩为
E([X−E(X)]k)E([X−E(X)]k)
期望实际上是随机变量X的1阶原点矩,方差实际上是随机变量X的2阶中心矩
变异系数(Coefficient of Variation):标准差与均值(期望)的比值称为变异系数,记为C.V
偏度Skewness(三阶)
峰度Kurtosis(四阶)
## 偏度与峰度
![](http://ww1.sinaimg.cn/large/0060lm7Tly1flhephvtbqj312a2104qp.jpg)
## 利用matplotlib模拟偏度和峰度
### 计算期望和方差
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
import
```
```python
math
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
def
```
```python
calc
```
```python
(data)
```
```python
:
```
```python
n=len(data)
```
```python
# 10000个数
```
```python
niu=
```
```python
0.0
```
```python
# niu表示平均值,即期望.
```
```python
niu2=
```
```python
0.0
```
```python
# niu2表示平方的平均值
```
```python
niu3=
```
```python
0.0
```
```python
# niu3表示三次方的平均值
```
```python
for
```
```python
a
```
```python
in
```
```python
data:
        niu += a
        niu2 += a**
```
```python
2
```
```python
niu3 += a**
```
```python
3
```
```python
niu /= n  
    niu2 /= n
    niu3 /= n
    sigma = math.sqrt(niu2 - niu*niu)
```
```python
return
```
```python
[niu,sigma,niu3]
```
niu=Xi¯即期望niu=Xi¯即期望

niu2=∑ni=1X2inniu2=∑i=1nXi2n

niu3=∑ni=1X3inniu3=∑i=1nXi3n
sigma表示标准差公式为
σ=E(x2)−E(x)2−−−−−−−−−−−−√σ=E(x2)−E(x)2
用python语言表示即为sigma=math.sqrt(niu2−niu∗niu)用python语言表示即为sigma=math.sqrt(niu2−niu∗niu)
返回值为[期望,标准差,E(x3)E(x3)]
PS:我们知道期望E(X)的计算公式为
E(X)=∑i=1np(i)x(i)−−−−−(1)E(X)=∑i=1np(i)x(i)−−−−−(1)
这里我们X一个事件p(i)表示事件出现的概率,x(i)表示事件所给予事件的权值.
我们直接利用
E(x)=Xi¯−−−−(2)E(x)=Xi¯−−−−(2)
表示期望应当明确
(2)公式中Xi是利用numpy中的伪随机数生成的,其均值用于表示期望Xi是利用numpy中的伪随机数生成的,其均值用于表示期望
此时(1)公式中对事件赋予的权值默认为1,即公式的本来面目为
E(x)=(Xi∗1)¯E(x)=(Xi∗1)¯

### 计算偏度和峰度
```python
def
```
```python
calc_stat
```
```python
(data)
```
```python
:
```
```python
[niu, sigma, niu3]=calc(data)
    n=len(data)
    niu4=
```
```python
0.0
```
```python
# niu4计算峰度计算公式的分子
```
```python
for
```
```python
a
```
```python
in
```
```python
data:
        a -= niu
        niu4 += a**
```
```python
4
```
```python
niu4 /= n
    skew =(niu3 -
```
```python
3
```
```python
*niu*sigma**
```
```python
2
```
```python
-niu**
```
```python
3
```
```python
)/(sigma**
```
```python
3
```
```python
)
```
```python
# 偏度计算公式
```
```python
kurt=niu4/(sigma**
```
```python
4
```
```python
)
```
```python
# 峰度计算公式:下方为方差的平方即为标准差的四次方
```
```python
return
```
```python
[niu, sigma,skew,kurt]
```
### 利用matplotlib模拟图像
```python
if
```
```python
__name__ ==
```
```python
"__main__"
```
```python
:
    data =  list(np.random.randn(
```
```python
10000
```
```python
))
```
```python
# 满足高斯分布的10000个数
```
```python
data2 = list(
```
```python
2
```
```python
*np.random.randn(
```
```python
10000
```
```python
))
```
```python
# 将满足好高斯分布的10000个数乘以两倍,方差变成四倍
```
```python
data3 =[x
```
```python
for
```
```python
x
```
```python
in
```
```python
data
```
```python
if
```
```python
x>-
```
```python
0.5
```
```python
]
```
```python
# 取data中>-0.5的值
```
```python
data4 = list(np.random.uniform(
```
```python
0
```
```python
,
```
```python
4
```
```python
,
```
```python
10000
```
```python
))
```
```python
# 取0~4的均匀分布
```
```python
[niu, sigma, skew, kurt] = calc_stat(data)
    [niu_2, sigma2, skew2, kurt2] = calc_stat(data2)
    [niu_3, sigma3, skew3, kurt3] = calc_stat(data3)
    [niu_4, sigma4, skew4, kurt4] = calc_stat(data4)
```
```python
print
```
```python
(niu, sigma, skew, kurt)
```
```python
print
```
```python
(niu2, sigma2, skew2, kurt2)
```
```python
print
```
```python
(niu3, sigma3, skew3, kurt3)
```
```python
print
```
```python
(niu4, sigma4, skew4, kurt4)
    info =
```
```python
r'$\mu=%.2f,\ \sigma=%.2f,\ skew=%.2f,\ kurt=%.2f$'
```
```python
%(niu,sigma, skew, kurt)
```
```python
# 标注
```
```python
info2 =
```
```python
r'$\mu=%.2f,\ \sigma=%.2f,\ skew=%.2f,\ kurt=%.2f$'
```
```python
%(niu_2,sigma2, skew2, kurt2)
    info3 =
```
```python
r'$\mu=%.2f,\ \sigma=%.2f,\ skew=%.2f,\ kurt=%.2f$'
```
```python
%(niu_3,sigma3, skew3, kurt3)
    plt.text(
```
```python
1
```
```python
,
```
```python
0.38
```
```python
,info,bbox=dict(facecolor=
```
```python
'red'
```
```python
,alpha=
```
```python
0.25
```
```python
))
    plt.text(
```
```python
1
```
```python
,
```
```python
0.35
```
```python
,info2,bbox=dict(facecolor=
```
```python
'green'
```
```python
,alpha=
```
```python
0.25
```
```python
))
    plt.text(
```
```python
1
```
```python
,
```
```python
0.32
```
```python
,info3,bbox=dict(facecolor=
```
```python
'blue'
```
```python
,alpha=
```
```python
0.25
```
```python
))
    plt.hist(data,
```
```python
100
```
```python
,normed=
```
```python
True
```
```python
,facecolor=
```
```python
'r'
```
```python
,alpha=
```
```python
0.9
```
```python
)
    plt.hist(data2,
```
```python
100
```
```python
,normed=
```
```python
True
```
```python
,facecolor=
```
```python
'g'
```
```python
,alpha=
```
```python
0.8
```
```python
)
    plt.hist(data4,
```
```python
100
```
```python
,normed=
```
```python
True
```
```python
,facecolor=
```
```python
'b'
```
```python
,alpha=
```
```python
0.7
```
```python
)
    plt.grid(
```
```python
True
```
```python
)
    plt.show()
```
![](http://ww3.sinaimg.cn/large/0060lm7Tly1flhm6bou4zj30lw0ef79m.jpg)
图形表示的是利用numpy随机数生成函数生成的随机数的统计分布,利用matplotlib.pyplot.hist绘制的直方图.即是出现数字的分布统计,并且是归一化到0~1区间后的结果.
即横轴表示数字,纵轴表示在1000个随机数中横轴对应的数出现的百分比.若不使用归一化横轴表示数字(normed=False),纵轴表示出现的次数.
若不使用归一化–纵轴表示出现次数
![](http://ww4.sinaimg.cn/large/0060lm7Tly1flhmgnizi7j30ka0ap3zn.jpg)
关于matplotlib.pyplot.hist函数
```python
n, bins, patches = plt.hist(arr, bins=
```
```python
10
```
```python
, normed=
```
```python
0
```
```python
, facecolor=
```
```python
'black'
```
```python
, edgecolor=
```
```python
'black'
```
```python
,alpha=
```
```python
1
```
```python
，histtype=
```
```python
'b'
```
```python
)
hist的参数非常多，但常用的就这六个，只有第一个是必须的，后面四个可选
arr: 需要计算直方图的一维数组
bins: 直方图的柱数，可选项，默认为
```
```python
10
```
```python
normed: 是否将得到的直方图向量归一化。默认为
```
```python
0
```
```python
facecolor: 直方图颜色
edgecolor: 直方图边框颜色
alpha: 透明度
histtype: 直方图类型，‘bar’, ‘barstacked’, ‘step’, ‘stepfilled’
返回值 ：
n: 直方图向量，是否归一化由参数normed设定
bins: 返回各个bin的区间范围
patches: 返回每个bin里面包含的数据，是一个list
```
> [关于matplotlib.pyplot.hist函数](http://blog.csdn.net/denny2015/article/details/50581784)


