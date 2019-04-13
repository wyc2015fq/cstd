
# numpy模块中的sum(axis)方法 - leofionn的博客 - CSDN博客


2018年05月10日 09:20:53[leofionn](https://me.csdn.net/qq_36142114)阅读数：94




### 1、sum函数声明
```python
sum(a, axis=
```
```python
None
```
```python
, dtype=
```
```python
None
```
```python
, out=
```
```python
None
```
```python
, keepdims=<
```
```python
class
```
```python
'
```
```python
numpy
```
```python
.
```
```python
_globals
```
```python
.
```
```python
_NoValue
```
```python
'>)
```
1
参数axis究竟是用来干嘛的？
### 2、实验
经过我的一些尝试，我发现以下规律：
#### （1） 如果
#### axis=None
#### 那么就是对所有元素求和：
```python
>>>
```
```python
np.sum([[
```
```python
0
```
```python
,
```
```python
1
```
```python
], [
```
```python
0
```
```python
,
```
```python
5
```
```python
], [
```
```python
2
```
```python
,
```
```python
5
```
```python
]], axis=
```
```python
None
```
```python
)
```
1
这条语句执行后将会输出：13，把所有元素都进行求和了！
#### （2） 如果
#### axis=0
#### 那么就是对所有在同一列的元素求和：
```python
>>>
```
```python
np.sum([[
```
```python
0
```
```python
,
```
```python
1
```
```python
], [
```
```python
0
```
```python
,
```
```python
5
```
```python
], [
```
```python
2
```
```python
,
```
```python
5
```
```python
]], axis=
```
```python
0
```
```python
)
```
1
这条语句执行后将会输出：array([ 2, 11])，把两列元素都进行求和了！
#### （3） 如果
#### axis=1
#### 那么就是对所有在同一行的元素求和：
```python
>>>
```
```python
np.sum([[
```
```python
0
```
```python
,
```
```python
1
```
```python
], [
```
```python
0
```
```python
,
```
```python
5
```
```python
], [
```
```python
2
```
```python
,
```
```python
5
```
```python
]], axis=
```
```python
1
```
```python
)
```
1
这条语句执行后将会输出：array([1, 5, 7])，把三行元素都进行求和了！
#### （4）如果axis的值
#### 大于1
#### 呢？这样将会
#### 报错
#### ！！
```python
>>>
```
```python
np.sum([[
```
```python
0
```
```python
,
```
```python
1
```
```python
], [
```
```python
0
```
```python
,
```
```python
5
```
```python
], [
```
```python
2
```
```python
,
```
```python
5
```
```python
]], axis=
```
```python
2
```
```python
)
```
1
错误信息如下：
#### umpy.core._internal.AxisError: axis 2 is out of bounds for array of dimension 2
就是axis=2超出范围了！！！


