
# 标准正态分布表（scipy.stats） - Zhang's Wikipedia - CSDN博客


2018年05月20日 12:31:52[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：29692



## 0. 标准正态分布表与常用值
![这里写图片描述](https://img-blog.csdn.net/20180520122328842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Z-score 是非标准正态分布标准化后的 x即
$$
z = \frac{x-\mu}{\sigma}
$$

表头的横向表示小数点后第二位，表头的纵向则为整数部分以及小数点后第一位；两者联合作为完整的 x，坐标轴的横轴
表中的值为图中红色区域的面积，也即 cdf，连续分布的累积概率函数，记为
$$
\Phi(x)
$$

cdf 的逆，记为
$$
\Phi^{-1}(x)
$$
，如
$$
\Phi^{-1}(3/4)
$$
，表示 x 取何值时，阴影部分的面积为 0.75，查表可知，x 介于 0.67 和  0.68 之间；
>> from scipy.stats import norm
>> norm.ppf(3/4)
0.6744897501960817
## 1. cdf 与 ppf（分位函数）
`from scipy.stats import norm`覆盖的概率范围：
![在这里插入图片描述](https://img-blog.csdn.net/20181022201131436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
>> norm.cdf(1) - norm.cdf(-1)
0.6826894921370859
>> norm.cdf(2) - norm.cdf(-2)
0.9544997361036416
>> norm.cdf(3) - norm.cdf(-3)
0.9973002039367398
$$
\Phi(x)
$$
为 累积概率密度函数，也即 cdf：
>> from scipy.stats import norm
>> norm.cdf(0)
0.5
$$
\Phi^{-1}(x)
$$
，通过 norm(x) 进行计算：
>> from scipy.stats import norm
\# Q3 分位点；
>> norm.ppf(3/4)
0.6744897501960817
\# Q1 分位点
>> norm.ppf(1/4)
-0.6744897501960817
---
[标准正态分布表](https://www.shuxuele.com/data/standard-normal-distribution-table.html)

