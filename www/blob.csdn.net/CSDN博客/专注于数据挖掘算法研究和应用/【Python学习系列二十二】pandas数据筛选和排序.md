# 【Python学习系列二十二】pandas数据筛选和排序 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月04日 10:43:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5013
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









在Pandas中通过.sort和.loc函数也可以实现这两 个功能。.sort函数可以实现对数据表的排序操作，.loc函数可以实现对数据表的筛选操作。

1、排序

sort函数主要包含6个参数：columns为要进行排序的列名称； ascending为排序的方式true为升序，False为降序，默认为true；axis为排序的轴，0表示index，1表示columns，当对数据列进行排序时，axis必须设置为0；inplace默认为False，表示对数据 表进行排序，不创建新实例；Kind可选择排序的方式，如快速排序等；na_position对NaN值的处理方式，可以选择first和last两种方式，默认为last，也就是将NaN值放在排序的结尾。


![](http://bluewhale.cc/wp-content/uploads/2016/08/sort%E5%AE%98%E6%96%B9%E8%AF%B4%E6%98%8E-1024x275.jpg)


1）单列数据排序

df.sort(["score"],ascending=True)#升序


df.sort(["score"],ascending=False)#降序


2）多列排序



df.sort(["score","age"],ascending=True)#升序


df.sort(["score","age"],ascending=False)#降序

优先第一列的排序

2、筛选

在筛选功能上Pandas使用的是.loc函数。

![](http://bluewhale.cc/wp-content/uploads/2016/08/loc%E5%AE%98%E6%96%B9%E8%AF%B4%E6%98%8E-1024x231.jpg)


案例如下：



```python
ds_0=label_ds.loc[(label_ds["action_type"] == 0) &\
                       (label_ds["u_brand_num"]>0) & (label_ds["u_cat_num"]>0) & \
                       (label_ds["u_ratio"]>0) & (label_ds["u_spu_ratio"]>0),\
                      ['u_brand_num','u_cat_num','u_spu_ratio','u_ratio','action_type']].sort(["u_brand_num"],ascending=False)
```


实现多列筛选，并显示指定列，并以特定列排序。条件中可以与或非。

可以按筛选后条件进行求和和平均，如下：




lc.loc[(lc["grade"] == "B") & (lc["loan_amnt"] > 5000)].loan_amnt.sum() #求和


lc.loc[(lc["grade"] == "B") & (lc["loan_amnt"] > 5000)].loan_amnt.count()#计数


lc.loc[(lc["grade"] == "B") | (lc["loan_amnt"] > 5000)].loan_amnt.mean()#均值


lc.loc[lc["grade"] == "B"].loan_amnt.max()#最大值


lc.loc[lc["grade"] != "B"].loan_amnt.min()#最小值

















