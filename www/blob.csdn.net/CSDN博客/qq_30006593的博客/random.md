# random - qq_30006593的博客 - CSDN博客





2017年08月19日 11:26:52[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：134标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/qq_30006593/article/category/7099907)









random.ranint(begin, end) : 产生begin<=n<=end

产生有界的正太分布：

from scipy.stats import truncnorm



sorted(truncnorm(a=0/32, b=32/32, scale=32).rvs(size=32).round().astype(int))





