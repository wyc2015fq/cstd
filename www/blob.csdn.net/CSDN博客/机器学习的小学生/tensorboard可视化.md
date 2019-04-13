
# tensorboard可视化 - 机器学习的小学生 - CSDN博客


2017年10月30日 17:10:35[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：266个人分类：[tensorflow																](https://blog.csdn.net/xuluhui123/article/category/6934245)



```python
tensorboard --logdir=
```
```python
E
```
```python
://logs_tensorboard
```
程序会递归的搜索所有logs_tensorboard下，以及其子目录下的.*tfevents.文件
注意上面的路径采用的是“//”，对于”\”,”/”,以及”\\”均无法检测到。

