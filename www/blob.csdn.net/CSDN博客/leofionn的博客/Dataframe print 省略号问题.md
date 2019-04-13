
# Dataframe print 省略号问题 - leofionn的博客 - CSDN博客


2019年01月28日 17:04:05[leofionn](https://me.csdn.net/qq_36142114)阅读数：36


通过print输出Dataframe中的数据，当Dataframe行数很多时，中间部分显示省略号.
通过查看pandas的官方文档可知，pandas.set_option() 可以设置pandas相关的参数，从而改变默认参数。 打印pandas数据时，默认是输出100行，多的话中间数据会输出省略号。
在代码中添加以下两行代码，可以改变显示宽度和行数，这样就能完整地查看数据了
pd.set_option('display.width', 1000) \# 设置字符显示宽度
pd.set_option('display.max_rows', None) \# 设置显示最大行
pd.set_option('display.max_columns', None) \# 设置显示最大行



