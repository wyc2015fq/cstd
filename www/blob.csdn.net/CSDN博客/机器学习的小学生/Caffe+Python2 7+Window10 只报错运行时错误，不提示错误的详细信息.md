
# Caffe+Python2.7+Window10 只报错运行时错误，不提示错误的详细信息 - 机器学习的小学生 - CSDN博客


2018年05月08日 19:22:00[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：128


使用：Caffe+Python2.7+Window10 时，在eclipse或者pycharm的环境下，Caffe报错不提示错误的详细信息，仅给出下面的错误信息：
```python
I0508
```
```python
11
```
```python
:
```
```python
22
```
```python
:
```
```python
13.968909
```
```python
12280
```
```python
net.cpp:
```
```python
131
```
```python
] Top shape:
```
```python
64
```
```python
4096
```
```python
(
```
```python
262144
```
```python
)
I0508
```
```python
11
```
```python
:
```
```python
22
```
```python
:
```
```python
13.968909
```
```python
12280
```
```python
net.cpp:
```
```python
139
```
```python
] Memory require*** Check failure stack trace: ***
```
但是：Caffe + Python2.7 + Window 7 就会给出详细报错的原因，例如：
```python
F0508
```
```python
11
```
```python
:
```
```python
16
```
```python
:
```
```python
47.186162
```
```python
7772
```
```python
net.cpp:
```
```python
377
```
```python
]
```
```python
Top
```
```python
blob
```
```python
'fc7'
```
```python
produced by multiple sources.
***
```
```python
Check
```
```python
failure stack trace: ***
```
我们看到上面给出了详细错误的原因是，多个 Top ‘fc7’。
如果系统必须选择Window 10， 那么可以将Python2.7 换成 Python3.5 就提示错误相信信息了。
**总的来说解决方案就是**：
```python
Caffe + Python2.7 + Window 7
Caffe + Python3.5 + Window 10
```

