
# caffe,tensorflow,theano,pytorch  输入数据形状 - 机器学习的小学生 - CSDN博客


2017年11月11日 15:29:18[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：383


**caffe :**
```python
nsample * channels *
```
```python
height
```
```python
*
```
```python
width
```
labels start from 0, such as for 5 classes, [0,1,2,3,4]; channels: BGR
**tensorflow:**
```python
nsample *
```
```python
height
```
```python
*
```
```python
width
```
```python
* channels
```
labels start from 0, such as for 5 classes, [0,1,2,3,4]; channels: BGR
**theano:**
```python
nsample * channels *
```
```python
height
```
```python
*
```
```python
width
```
labels start from 0, such as 5 classes ,[0,1,2,3,4];
**pytorch:**
```python
nsample * channels *
```
```python
height
```
```python
*
```
```python
width
```
labels: nsample * 1, have the same channel with Caffe: BGR
参考文献：
1.[https://discuss.pytorch.org/t/convert-caffe-to-pytorch/10261](https://discuss.pytorch.org/t/convert-caffe-to-pytorch/10261)

