
# sklearn预处理函数transform和fit_transform - 追求卓越,做到专业 - CSDN博客


2018年05月07日 12:58:27[Waldenz](https://me.csdn.net/enter89)阅读数：174


相同：都是对数据进行统一化处理，将数据缩放(映射)到某个固定区间，归一化，正则化；
fit_transform()函数: 先拟合数据，找到数据的整体指标，如均值、方差、最大值最小值等等，将其转化为标准形式；
transform()函数: 通过找中心和缩放等实现标准化；
结论：必须先用fit_transform(partData)，之后再transform(restData)，这样restData才可以使用同样的标准，从而实现数据的标准化归一化。

