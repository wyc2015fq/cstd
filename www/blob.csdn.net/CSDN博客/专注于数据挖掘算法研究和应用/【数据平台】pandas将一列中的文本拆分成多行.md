# 【数据平台】pandas将一列中的文本拆分成多行 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年10月31日 17:10:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2710
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)










1、场景：如下图，要将cont中的字符串分割出词汇并单独成一行

![](https://img-blog.csdn.net/20171031171203911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、一列中的文本拆分成多行的效果：

![](https://img-blog.csdn.net/20171031171319711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3、实现方法：


方法一：



```python
df=df.drop('cont', axis=1).join(df['cont'].str.split('/', expand=True).stack().reset_index(level=1, drop=True).rename('tag'))
```



方法二：



```python
df=df['cont'].str.split('/', expand=True).stack().reset_index(level=0).set_index('level_0').rename(columns={0:'tag'}).join(df.drop('cont', axis=1))
```



参考：https://zhuanlan.zhihu.com/p/28337202


比较好用！






