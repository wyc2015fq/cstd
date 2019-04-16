# 计量经济与时间序列_ACF与PACF标准差(均标准误)的计算(含代码) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [计量经济与时间序列_ACF与PACF标准差(均标准误)的计算(含代码)](https://www.cnblogs.com/noah0532/p/8453959.html)





1   我们对于acf和pacf值计算完毕之后，在需要计算两个数值的标准差。

2   acf和pacf的标准差计算略有不同。acf的标准差是一个移动过程，而pacf是一个相对固定过程。

3   我们继续引用这篇博文中最后的到的数值http://www.cnblogs.com/noah0532/p/8451375.html。来计算他们的标准差。代码如下：

```
1 # 求acf的标准差
 2 import  math
 3 se = 0
 4 acf_tmp0 = []
 5 while se < len(AcfValue):
 6     acf_tmp0.append(AcfValue[se] * AcfValue[se] * 2) # 对应每一个平方先计算出来然后保留至列表
 7     se += 1
 8 # print(acf_tmp0)
 9 
10 se = 0
11 acf_tmp1 = 0
12 acf_se = []
13 while se < len(acf_tmp0):
14     acf_tmp1 = ((acf_tmp0[se] + acf_tmp1))
15     acf_se.append(math.sqrt((1+acf_tmp1) / len(TimeSeries)))
16     se += 1
17 print("acf的标准差分别为：", acf_se)
18 # 输出结果：acf的标准差分别为： [0.32721369164507774, 0.3393361165570208, 0.3488569907569576, 0.3538163365363448, 0.38267636456933163, 0.3862978125747025, 0.38691885454187935, 0.38772851326669283, 0.388183204170402]
19 
20 
21 # 求pacf的标准差
22 se = 0
23 pacf_se = []
24 while se < len(PacfValue):
25     pacf_se.append(math.sqrt(1/(len(TimeSeries))))
26     se += 1
27 print("pacf的标准差分别为：",pacf_se)
28 # 输出结果：pacf的标准差分别为： [0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794, 0.31622776601683794]
```














