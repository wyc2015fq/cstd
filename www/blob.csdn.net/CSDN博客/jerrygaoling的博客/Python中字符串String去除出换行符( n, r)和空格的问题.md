
# Python中字符串String去除出换行符(-n,-r)和空格的问题 - jerrygaoling的博客 - CSDN博客


2018年07月15日 11:32:24[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：35431


# Python中字符串String去除出换行符和空格的问题（\n,\r）
在Python的编写过程中，获取到的字符串进场存在不明原因的换行和空格，如何整合成一个单句，成为问题。
方法：
一、去除空格
“  ·  ”代表的为空格
strip()
```python
"···xyz···"
```
```python
.strip()
```
```python
# returns "xyz"
```
```python
"···xyz···"
```
```python
.lstrip()
```
```python
# returns "xyz···"
```
```python
"···xyz···"
```
```python
.rstrip()
```
```python
# returns "···xyz"
```
```python
"··x·y·z··"
```
```python
.replace(
```
```python
' '
```
```python
,
```
```python
''
```
```python
)
```
```python
# returns "xyz"
```
二、替换 replace(“space”,”“)
用replace(“\n”, “”)，与replace(“\r”, “”)，后边的内容替换掉前边的。
实际问题：
如图：
string中内容
![这里写图片描述](https://img-blog.csdn.net/20180715113106561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中，“  ·  ”代表的为空格，一段话被换行成了几段。
1.使用==.strip()==只能够去除字符串首尾的空格，不能够去除中间的空格。如图：
![这里写图片描述](https://img-blog.csdn.net/20180715113056973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以需要使用==.replace(’ ‘, ”)==来替换空格项。string.replace(’ ‘, ”)。如图：
![这里写图片描述](https://img-blog.csdn.net/20180715113031300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180715113031300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.使用==.replace(‘\n’, ”)==去除换行。如图：并不能达到效果。
[
](https://img-blog.csdn.net/20180715113031300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180715112957749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
原因在于：在python中存在继承了==回车符\r==和==换行符\n==两种标记。
\r 和 \n 都是以前的那种打字机传承来的。
\r 代表回车，也就是打印头归位，回到某一行的开头。
\n代表换行，就是走纸，下一行。
linux只用\n换行。
win下用\r\n表示换行。
python中同样一句话：print  (u’前面的内容\r只显示后面的内容’)
所以，在去除换行是，需要同时去除两者才行，即使用==.replace(‘\n’, ”).replace(‘\n’, ”)==
结果如图：
![这里写图片描述](https://img-blog.csdn.net/2018071511293694?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180715113031300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
