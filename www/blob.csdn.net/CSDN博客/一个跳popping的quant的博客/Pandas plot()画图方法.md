# Pandas.plot()画图方法 - 一个跳popping的quant的博客 - CSDN博客





2018年08月13日 11:05:33[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1063








转自：[https://blog.csdn.net/claroja/article/details/73872066?utm_source=debugrun&utm_medium=referral](https://blog.csdn.net/claroja/article/details/73872066?utm_source=debugrun&utm_medium=referral)



# [Series](http://pandas.pydata.org/pandas-docs/stable/api.html#plotting)

Series.plot(kind=’line’, ax=None, figsize=None, use_index=True, title=None, grid=None, legend=False, style=None, logx=False, logy=False, loglog=False, xticks=None, yticks=None, xlim=None, ylim=None, rot=None, fontsize=None, colormap=None, table=False, yerr=None, xerr=None, label=None, secondary_y=False, **kwds)

参数：
|参数|描述|
|----|----|
|data|Series|
|kind|str:”line”,”bar”,”barh”,”hist”,”box”,”kde”,”density”,”area”,”pie”|
|ax|matplotlib axes 对象，默认使用gca()|
|figsize|tuple(宽度，高度),单位是英寸|
|use_index|boolean,默认为True，使用索引作为刻度标签|
|title|string or list,传入str直接在figure上打印，传入list则会打印在subplot上|
|grid|boolean，默认为None，使用matlab样式|
|legend|False/True/”reverse”|
|style|list or dict,matplotlib line style per column|
|logx|boolean,默认False，x轴使用log刻度|
|logy|boolean,默认False,y轴使用log刻度|
|xticks|sequence,x轴刻度标签|
|yticks|sequence,y轴刻度标签|
|xlim|2-tuple/list|
|ylim|2-tuple/list|
|rot|int 默认为None，改变刻度标签的旋转度|
|fontsize|int 默认为None，设置刻度标签的大小|
|colormap|str or matplotlib colromap对象 默认为None|
|colorbar|boolean,optional，如果为True绘制colorbar(只有在scatter和hexbin中起作用)|
|position|float,Specify relative alignments for bar plot layout. From 0 (left/bottom-end) to 1 (right/top-end). Default is 0.5 (center)|
|layout|tuple(optional)，(rows, columns) for the layout of the plot|
|table|boolean,Series or DataFrame 默认为False，If True, draw a table using the data in the DataFrame and the data will be transposed to meet matplotlib’s default layout. If a Series or DataFrame is passed, use passed data to draw a table.|
|yerr|DataFrame,Series,array-like,dict and str,See Plotting with Error Bars for detail.|
|xerr||
|lable|label argument to provide to plot|
|secondary_y|boolean or sequence of ints, default False|
|mark_right|boolean, default True|
|kwds|keywords,Options to pass to matplotlib plotting method|

# [DataFrame](http://pandas.pydata.org/pandas-docs/stable/api.html#api-dataframe-plotting)

DataFrame.plot(x=None, y=None, kind=’line’, ax=None, subplots=False, sharex=None, sharey=False, layout=None, figsize=None, use_index=True, title=None, grid=None, legend=True, style=None, logx=False, logy=False, loglog=False, xticks=None, yticks=None, xlim=None, ylim=None, rot=None, fontsize=None, colormap=None, table=False, yerr=None, xerr=None, secondary_y=False, sort_columns=False, **kwds)
|参数|描述|
|----|----|
|data|DataFrame|
|x|label or position, default None|
|y|label or position, default None|
|kind|str:‘scatter’,‘hexbin’|
|subplots|boolean, default False,为每一列单独画一个子图|
|sharex|boolean, default True if ax is None else False|
|sharey|boolean, default False|
|loglog|boolean, default False，x轴/y轴同时使用log刻度|

tips:

可以先用plt.figure()新建一个画板，然后再用pandas里面的做图命令，这样就可以修改pandas的图。



