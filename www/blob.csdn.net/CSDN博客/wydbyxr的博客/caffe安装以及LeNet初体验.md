# caffe安装以及LeNet初体验 - wydbyxr的博客 - CSDN博客
2017年04月10日 20:48:59[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：280标签：[神经网络																[caffe安装](https://so.csdn.net/so/search/s.do?q=caffe安装&t=blog)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)
个人分类：[caffe](https://blog.csdn.net/wydbyxr/article/category/7151104)
# 一、caffe的编译
## 1.（只有only CPU需要这一步）首先修改配置文件，回到caffe目录
cd ~/caffe 
cp Makefile.config.example Makefile.config 
gedit Makefile.config 
这里仅需修改两处：
### i）
`# USE_CUDNN := 1 `
这里去掉#，取消注释，改为USE_CUDNN := 1 。
### ii) 修改python包目
```
PYTHON_INCLUDE := /usr/include/python2.7 \
　　/usr/lib/python2.7/dist-packages/numpy/core/include
改为
PYTHON_INCLUDE := /usr/include/python2.7 \
　　/usr/local/lib/python2.7/dist-packages/numpy/core/include
因为安装的python包目录在这里： /usr/local/lib/python2.7/dist-packages/
```
iii）直接make 
make all -j8 
make test 
make runtest 
make pycaffe
iv）最后
```java
>>>python
>>>import caffe
```
没报错，那就说明安装好了。
注意： 
新版caffe都需要从根目录上执行，不然可能会遇到错误。例如
```bash
cd $CAFFE_ROOT
./examples/mnist/train_lenet.sh
```
# 二、LeNet体验
## 网络模型的描述文件
它的神经网络模型的描述文件就是/home/wei/caffe/examples/mnist/文件夹下面的:lenet_train_test.prototxt
- Caffe默认的训练是在GPU上的，但是如果想改为CPU上，只需要在lenet_solver.prototxt中修改一行：
`# solver mode: CPU or GPUsolver_mode: CPU`- 
很多caffe的prototxt里都是颜色值乘与0.00390625(1/256),使它们在[0,1]范围内. 
transform_param { 
scale: 0.00390625 
}
- 
lr_mults是层的可学习参数的学习率调整。在这种情况下，我们将权重学习率设置为与运行时求解器给出的学习速度相同，偏置学习速率是两倍，这通常会导致更好的收敛速度. 
param { lr_mult: 1 } 
param { lr_mult: 2 }
- 
只在train中使用，即 
layer { 
// …layer definition… 
include: { phase: TRAIN } 
}
- 
最后，说一下 Caffe 为什么采用 LMDB、LEVELDB，而不是直接读取原始数据？ 
原因是，一方面，数据类型多种多样，有二进制文件、文本文件、编码后的图像文件（如 JPEG、PNG、网络爬取的数据等），不可能用一套代码实现所有类型的输入数据读取，转换为统一格式可以简化数据读取层的实现；另一方面，使用 LMDB、LEVELDB 可以提高磁盘 IO 利用率。
- 
转换格式 
下载到的原始数据为二进制文件，需要转换为 LEVELDB 或 LMDB 才能被 Caffe 识别。 
我们 Git 得到的 Caffe 中，在 examples/mnist/ 下有一个脚本文件：create_mnist.sh ，这个就可以将原始的二进制数据，生成 LMDB 格式数据。
- 
.solverstate是snapshot保存的权重等的文件
