
# python 常用库的安装 - 机器学习的小学生 - CSDN博客


2017年06月15日 15:08:08[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：734



# 读图片
```python
from
```
```python
scipy.misc.pilutil
```
```python
import
```
```python
*
```
```python
# read image
```
```python
img = imread(
```
```python
'lena.jpg'
```
```python
)
```
其是通过PIL的Image类实现的读图片功能，因此安装scipy库的同时也要安装PIL库.
# dlib for python2.7
例如利用dlib库中的人脸检测算法。
```python
conda install -c menpo dlib
```
```python
=
```
```python
18.18
```
# python2 和python3同时安装
使用miniconda2和miniconda3管理。分别对miniconda2和miniconda3的script目录下conda.exe和对应的conda-script.py 重命名为conda2.exe，conda2-script.py和conda3.exe,conda3-script.py。则可以在dos环境下，利用conda2 install或者conda3 install 命令的形式分别安装。
# yaml安装
```python
conda
```
```python
install
```
```python
pyyaml
```

