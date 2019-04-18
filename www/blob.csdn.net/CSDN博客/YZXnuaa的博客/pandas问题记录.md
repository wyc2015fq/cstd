# pandas问题记录 - YZXnuaa的博客 - CSDN博客
置顶2018年09月18日 01:41:38[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：99
1、pandas.read_csv()函数，读取文件数据时，由于分隔符为'::'，弹出如下警告
       警告：ParserWarning: Falling back to the 'python' engine because the 'c' engine does not support regex separators (separators > 1 char and different from '\s+' are interpreted as regex)
       解决方法：增加函数的引擎参数engine='python'，如下：
```
header = ['user_id', 'item_id', 'rating', 'timestamp']
df = pd.read_csv("D:/ratings.dat", sep='::', names=header,engine='python')
```
2 。有句代码总是报错：
`total_births=names.pivot_table('births',rows='year',cols='sex',aggfunc=sum)`- 报错信息如下：
```
Traceback (most recent call last):
  File "<ipython-input-75-8bbcd2a6b8ca>", line 1, in <module>
    total_births=names.pivot_table('births',rows='year',cols='sex',aggfunc=sum)
TypeError: pivot_table() got an unexpected keyword argument 'rows'
```
查了有关资料，将rows改成index，cols写成全名”columns”:
`total_births=names.pivot_table('births',index='year',columns='sex',aggfunc=sum) `- 便可得到正确处理结果：
![这里写图片描述](https://img-blog.csdn.net/20150817143603838)
3.
