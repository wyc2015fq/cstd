
# Pandas的DataFrame输出截断和省略问题 - saltriver的专栏 - CSDN博客


2017年09月30日 19:44:26[saltriver](https://me.csdn.net/saltriver)阅读数：4597


我们看一个现象:
```python
import
```
```python
pandas
```
```python
as
```
```python
pd
titanic = pd.read_csv(
```
```python
'titanic_data.csv'
```
```python
)
print(titanic.head())
```
Titanic_data.csv是[kaggle上的泰坦尼克数据集](https://www.kaggle.com/c/titanic)，通过pandas读入到一个dataframe中，我们看看其前5行记录。输出结果如下：
![这里写图片描述](https://img-blog.csdn.net/20170930194231937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170930194231937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到，记录被分成了3段截断输出，如果想在一行输出，该怎么办呢？这就需要设置pandas的option选项：
[
](https://img-blog.csdn.net/20170930194231937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
pd.set_option(
```
```python
'display.width'
```
```python
,
```
```python
200
```
```python
)
```
[
](https://img-blog.csdn.net/20170930194231937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)再看输出，这次5条记录在一行中显示了。
![这里写图片描述](https://img-blog.csdn.net/20170930194333126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170930194333126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同时，我们注意到，索引为1的记录中，Name有省略号，并没有显示全。这时需要调整列宽。
[
](https://img-blog.csdn.net/20170930194333126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
pd.set_option(
```
```python
'display.max_colwidth'
```
```python
,
```
```python
100
```
```python
)
```
[
](https://img-blog.csdn.net/20170930194333126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)看看输出结果，这次显示全了。
![这里写图片描述](https://img-blog.csdn.net/20170930194354818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同样，我们还可以控制max_row，max_column等参数，使得我们根据实际数据的显示要求进行设置。更多的设置项详见：
[https://pandas.pydata.org/pandas-docs/stable/generated/pandas.set_option.html](https://pandas.pydata.org/pandas-docs/stable/generated/pandas.set_option.html)

[
  ](https://img-blog.csdn.net/20170930194333126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)