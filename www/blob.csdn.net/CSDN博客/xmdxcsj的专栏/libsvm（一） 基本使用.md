# libsvm（一） 基本使用 - xmdxcsj的专栏 - CSDN博客





2016年05月26日 22:59:48[xmucas](https://me.csdn.net/xmdxcsj)阅读数：572








## 主要流程
- 转化数据格式
- 训练和测试数据进行scale 

训练和测试数据需要使用同样的归一化文件

```
svm-scale -l -1 -u 1 -s range train > train.scale
svm-scale -r range eval > eval.scale
```
- 选择核函数 

特征维度>>训练数据：linear kernel 

特征维度和训练数据都很大：使用LIBLINEAR库更快 

训练数据>>特征维度：RBF kernel- 选择参数c和g 

使用v-fold交叉验证，把训练数据分成v个同样大小的子集。其中一个子集作为测试集，另外v-1个子集作为训练集，最后总的准确率作为测试结果。 

采用grid-search方法查找最好的参数，参数序列使用指数递增的方式进行遍历： 


$c=2^{-5},2^{-3}...,2^{15}$


$g=2^{-15},2^{-13}...,2^{3}$

这种方法称为coarse grid search，接下来在参数值附近进行finer grid search，进行更细粒度的查找。`python grid.py train.scale`- 使用最好的参数c和g进行训练和测试 

“` powershell 

svm-train -s 0 -c 32 -t 2 -g 0.5 -e 0.1 train.scale 

svm-predict eval.scale train.scale.model eval.res





