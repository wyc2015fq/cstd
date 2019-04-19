# easyUI datagrid 前端真分页 - 左直拳的马桶_日用桶 - CSDN博客
2015年02月06日 14:52:29[左直拳](https://me.csdn.net/leftfist)阅读数：3062
前文再续，书接上一回。[easyUI datagrid 前端假分页](http://blog.csdn.net/leftfist/article/details/43164977)[http://blog.csdn.net/leftfist/article/details/43164977](http://blog.csdn.net/leftfist/article/details/43164977)
真分页是easyUI datagrid 的默认方式。所谓真分页，就是后台传给前端的数据，真的是分页后的数据。而假分页，是全部数据。
easyUI datagrid 向后台请求数据的时候，会自动将页号(page)、每页数量（rows）作为参数附在url后面发到后台，在后台断点调试的时候可以看到这一点。
然后前端这里无须我们作更多的干预，假分页方式的话，还要定义自己的加载函数。
同样的，真分页模式下，后台传给前端的数据，格式与假分页是一样的，都要包含总记录数，以及当前分页记录：
｛"total":106,"rows":[{},{},{}.....]｝
