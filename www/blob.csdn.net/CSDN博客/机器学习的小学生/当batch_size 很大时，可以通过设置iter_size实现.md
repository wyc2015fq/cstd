
# 当batch_size 很大时，可以通过设置iter_size实现 - 机器学习的小学生 - CSDN博客


2017年12月05日 15:53:44[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2073



```python
In
```
```python
the
```
```python
solver.prototxt
```
```python
add
```
```python
iter_size:
```
```python
2
```
```python
That will mean that
```
```python
it
```
```python
would
```
```python
do
```
```python
2
```
```python
iterations
```
```python
of
```
```python
batch_size:
```
```python
30
```
```python
before
```
```python
updating
```
```python
the
```
```python
weights. This means that effectively you would
```
```python
using
```
```python
a
```
```python
batch_size:
```
```python
60.
```
```python
You can change your batch_size
```
```python
and
```
```python
iter_size
```
```python
to
```
```python
define
```
```python
the
```
```python
desired batch_size.
```
参考：
[https://github.com/BVLC/caffe/issues/1396](https://github.com/BVLC/caffe/issues/1396)

