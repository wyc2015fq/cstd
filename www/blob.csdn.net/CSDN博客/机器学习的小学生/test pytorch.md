
# test pytorch - 机器学习的小学生 - CSDN博客


2018年04月21日 12:06:03[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：183



```python
#coding=UTF-8
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
import
```
```python
torch
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Variable
```
```python
import
```
```python
time

height =
```
```python
4000
```
```python
width =
```
```python
3000
```
```python
height2 =
```
```python
6000
```
```python
width2 =
```
```python
5000
```
```python
a = np.random.random((height,width))
b = np.random.random((width,height2))
e = np.random.random((height2,width2))
time_start1 = time.time()
c  = np.dot(a,b)
time_end1 = time.time()
time_out1 = time_end1 - time_start1
print(
```
```python
'cpu: time_out: '
```
```python
,time_out1)
time_start2 = time.time()
a1 = torch.from_numpy(a)
b1 = torch.from_numpy(b)
e1 = torch.from_numpy(e)
a1 = a1.cuda()
```
```python
# copy cpu to gpu
```
```python
b1 = b1.cuda()
```
```python
# copy cpu to gpu
```
```python
checked =
```
```python
False
```
```python
if
```
```python
not
```
```python
checked:
    e1 = e1.cuda()
```
```python
# copy cpu to gpu, change e1.cuda()
```
```python
time_start3 = time.time()
c1 = torch.matmul(a1,b1)
rest = torch.matmul(c1,e1)
```
```python
# test where data-in-cpu can multipy data-in-gpu
```
```python
time_end3 = time.time()
time_out3 = time_end3 - time_start3
print(
```
```python
'gpu: time_out: '
```
```python
,time_out3)
c1 = c1.cpu()
```
```python
# copy gpu to cpu
```
```python
time_end2 = time.time()
time_out2 = time_end2 - time_start2
print(
```
```python
'gpu total: time_out: '
```
```python
,time_out2)
```
```python
############## print few results ##############
```
```python
print(
```
```python
'cpu: c: '
```
```python
,c[
```
```python
0
```
```python
,
```
```python
0
```
```python
:
```
```python
5
```
```python
])
print(
```
```python
'gpu: c1: '
```
```python
,c[
```
```python
0
```
```python
,
```
```python
0
```
```python
:
```
```python
5
```
```python
])
print(
```
```python
'cpu: c1: '
```
```python
,c[
```
```python
0
```
```python
,
```
```python
0
```
```python
:
```
```python
5
```
```python
])
```
result:
```python
cpu:
```
```python
time_out:
```
```python
0.8587839603424072
```
```python
gpu:
```
```python
time_out:
```
```python
0.15541672706604004
```
```python
gpu total: time_out:
```
```python
2.954385280609131
```
```python
cpu:
```
```python
c:  [
```
```python
759.56254243
```
```python
751.99849962
```
```python
735.25157129
```
```python
735.34527291
```
```python
730.15037367
```
```python
]
```
```python
gpu:
```
```python
c1:  [
```
```python
759.56254243
```
```python
751.99849962
```
```python
735.25157129
```
```python
735.34527291
```
```python
730.15037367
```
```python
]
```
```python
cpu:
```
```python
c1:  [
```
```python
759.56254243
```
```python
751.99849962
```
```python
735.25157129
```
```python
735.34527291
```
```python
730.15037367
```
```python
]
```

