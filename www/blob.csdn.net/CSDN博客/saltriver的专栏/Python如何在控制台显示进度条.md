
# Python如何在控制台显示进度条 - saltriver的专栏 - CSDN博客


2016年11月06日 19:38:42[saltriver](https://me.csdn.net/saltriver)阅读数：17089标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[控制台																](https://so.csdn.net/so/search/s.do?q=控制台&t=blog)[进度条																](https://so.csdn.net/so/search/s.do?q=进度条&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=控制台&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)
[
																								](https://so.csdn.net/so/search/s.do?q=python&t=blog)


我们在写一些python程序时，如果程序运行时间较长，我们希望能够有一个进度条来动态的展示程序运行进度。
首先能想到的做法是用print将执行到第几步打印出来，但这样显然不是我们想要的进度条，显示进度占用了很多行，原因是print在末尾默认加上了”\n”换行符。详见[《你应该知道的print》](http://blog.csdn.net/saltriver/article/details/52194914)。
我们对进度条的需求：一是进度信息在一行中显示；二是每次都能够动态擦除一行中上一次的内容。那么很自然的想到print有没有相应的转义符，还真有这么一个，”\r”。
```python
import
```
```python
time
```
```python
N =
```
```python
1000
```
```python
for
```
```python
i
```
```python
in
```
```python
range(N):
    print(
```
```python
"进度:{0}%"
```
```python
.
```
```python
format
```
```python
(
```
```python
round
```
```python
((i +
```
```python
1
```
```python
) *
```
```python
100
```
```python
/ N)),
```
```python
end
```
```python
="\
```
```python
r
```
```python
")
```
```python
time
```
```python
.sleep(
```
```python
0.01
```
```python
)
```
输出格式：
100%
进度显示的问题解决了，有时候我们需要有一些时间方面的显示；例如已经耗费了多长时间，预计还剩下多少时间等。
```python
import
```
```python
time
```
```python
N =
```
```python
1000
```
```python
st =
```
```python
time
```
```python
.clock()
```
```python
for
```
```python
i
```
```python
in
```
```python
range(N):
    p =
```
```python
round
```
```python
((i +
```
```python
1
```
```python
) *
```
```python
100
```
```python
/ N)
    duration =
```
```python
round
```
```python
(
```
```python
time
```
```python
.clock() - st,
```
```python
2
```
```python
)
    remaining =
```
```python
round
```
```python
(duration *
```
```python
100
```
```python
/ (
```
```python
0.01
```
```python
+ p) - duration,
```
```python
2
```
```python
)
    print(
```
```python
"进度:{0}%，已耗时:{1}s，预计剩余时间:{2}s"
```
```python
.
```
```python
format
```
```python
(p, duration, remaining),
```
```python
end
```
```python
="\
```
```python
r
```
```python
")
```
```python
time
```
```python
.sleep(
```
```python
0.01
```
```python
)
```
输出格式：进度:100%，已耗时:10s，预计剩余时间:10s
上面代码中计算remaining时p加上了0.01，是为了避免除0问题。
至此，控制台进度条可以工作了，但代码在不同地方使用时会比较冗余，理解了原理，可以封装成一个库。
实际上早就有人已经做好这个工作了，而且功能更加强大丰富。这里推荐progressbar，下载地址为[https://pypi.python.org/pypi/progressbar2](https://pypi.python.org/pypi/progressbar2)
简单用法1
```python
import
```
```python
time
```
```python
import progressbar
p = progressbar.ProgressBar()
N =
```
```python
1000
```
```python
for
```
```python
i
```
```python
in
```
```python
p(
```
```python
range
```
```python
(N)):
```
```python
time
```
```python
.sleep(
```
```python
0.01
```
```python
)
```
简单用法2
```python
import time
import progressbar
p = progressbar
```
```python
.ProgressBar
```
```python
()
N =
```
```python
1000
```
```python
p
```
```python
.start
```
```python
(N)
for i
```
```python
in
```
```python
range(N):
    time
```
```python
.sleep
```
```python
(
```
```python
0.01
```
```python
)
    p
```
```python
.update
```
```python
(i+
```
```python
1
```
```python
)
p
```
```python
.finish
```
```python
()
```
两种简单用法的输出格式都是：
100% (1000 of 1000) |\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#| Elapsed Time: 0:00:10 Time: 0:00:10
包含了百分百进度，数量进度，\#号前进条，耗费时间，剩余时间。
高级点的用法（自己配置输出格式）
```python
import
```
```python
time
```
```python
import
```
```python
progressbar
bar = progressbar.ProgressBar(widgets=[
```
```python
' ['
```
```python
, progressbar.Timer(),
```
```python
'] '
```
```python
,
    progressbar.Percentage(),
```
```python
' ('
```
```python
, progressbar.ETA(),
```
```python
') '
```
```python
,
])
```
```python
for
```
```python
i
```
```python
in
```
```python
bar(range(
```
```python
1000
```
```python
)):
    time.sleep(
```
```python
0.01
```
```python
)
```
输出格式：
[Elapsed Time: 0:00:10] 100% (Time: 0:00:10)
主要方式有如下几种，可自由搭配：
```python
'Timer'
```
```python
,
```
```python
# 计时器
```
```python
'ETA'
```
```python
,
```
```python
# 预计剩余时间
```
```python
'AbsoluteETA'
```
```python
,
```
```python
# 预计结束的绝对时间，耗时很长时使用较方便
```
```python
'Percentage'
```
```python
,
```
```python
# 百分比进度，30%
```
```python
'SimpleProgress'
```
```python
,
```
```python
# 计数进度，300/1000
```
```python
'Counter'
```
```python
,
```
```python
# 单纯计数
```
```python
'Bar'
```
```python
# “#”号进度条
```
例如：
```python
import
```
```python
time
```
```python
import
```
```python
progressbar
bar = progressbar.ProgressBar(widgets=[
    progressbar.Percentage(),
```
```python
' ('
```
```python
, progressbar.SimpleProgress(),
```
```python
') '
```
```python
,
```
```python
' ('
```
```python
, progressbar.AbsoluteETA(),
```
```python
') '
```
```python
,])
```
```python
for
```
```python
i
```
```python
in
```
```python
bar(range(
```
```python
1000
```
```python
)):
    time.sleep(
```
```python
0.01
```
```python
)
```
输出示例：
54% ( 545 of 1000)  (Estimated finish time: 2016-11-06 19:26:15)

