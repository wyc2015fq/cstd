# pyfolio教程3——create_interesting_times_tear_sheet - lyx的专栏 - CSDN博客





2019年03月25日 19:45:48[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：72








      我们有时候对一些时间段特别的关注，比如911、互联网泡沫、外星人入侵什么的。在pyfolio中，有一个interesting times tear sheet，其实就是把某些特殊的时间段放大了给大家看看。

      至于特殊时间段的定义：

![](https://img-blog.csdnimg.cn/20190322222708152.png)

      在pyfolio的这个文件夹下面，有一个这个py文件，在里面进行设置就可以了：

```python
# Dotcom bubble
PERIODS['Dotcom'] = (pd.Timestamp('20000310'), pd.Timestamp('20000910'))

# Lehmann Brothers
PERIODS['Lehman'] = (pd.Timestamp('20080801'), pd.Timestamp('20081001'))

# 9/11
PERIODS['9/11'] = (pd.Timestamp('20010911'), pd.Timestamp('20011011'))

# 05/08/11  US down grade and European Debt Crisis 2011
PERIODS[
    'US downgrade/European Debt Crisis'] = (pd.Timestamp('20110805'),
                                            pd.Timestamp('20110905'))

自己设置一个
PERIODS['lyx'] = (pd.Timestamp('20180301'), pd.Timestamp('20180801'))
```

然后：

```python
pf.create_interesting_times_tear_sheet(return_ser['return'], benchmark_rets=return_ser['bench'])
```

      我们来看一下这样的结果：

![](https://img-blog.csdnimg.cn/20190322223231126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      所以，这个很简单，就是看了一下细节。











