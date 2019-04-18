# 初识caffe2 - wydbyxr的博客 - CSDN博客
2017年09月05日 20:58:33[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：1025
# 基本的认识
Caffe2 中基本计算单元之一是 Operators。每个 Operator 包含给定适当数量和类型的输入和参数来计算输出所需的逻辑。caffe2 宣称是轻量级、模块化和可扩展的一个框架
之前layer的概念被弱化，数据与操作完全分开， 
网络的编写也向tf靠拢了 
最后还要说一点就是对Python的支持大大增强
Caffe2更注重模块化，在移动端、大规模部署上表现卓越。如同TensorFlow，Caffe2使用C++Eigen库，支持ARM架构。 
Caffe2延续了它对视觉类问题的支持，且增加了对自然语言处理、手写识别、时序预测有帮助的RNN和LSTM支持。
fsync（fsync函数同步内存中所有已修改的文件数据到储存设备） 
TODO:检查构建文件是否需要更新
OperatorDef& operator_def是指op名 
可视化 
pyplot.hist()：查看直方图 
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
三大要素 
Workspaces 
Operators「算子」 
Nets
与TensorFlow相比： 
数据和操作就是tf.matmul和tf.Variable；  
workspace概念很像是tf中的Session
# 主要概念
I)Blobs和Workspace，Tensors
blobs：是一个张量（可视为多维数组）。使用numpy（即python的数组库）。
Workspace：存储所有的blob。对blob输入和输出对workspace中，要使用 
x = np.random.rand(4, 3, 2) 
workspace.FeedBlob(“my_x”, x) 
x2 = workspace.FetchBlob(“my_x”)
II)Nets and Operator 
NET：是op的表
operator：需要输入，得到输出。是caffe2的基础计算单元 
使用model helpers是编程更简单。例m = cnn.CNNModelHelper(name=”my first net”) 
CNNModelHelper将创建两个相互关联的网络： 
一个初始化参数的net（参考init_net） 
一个实际训练的net（参考exec_net）
用Python构建网络，将net用C++存入缓存中，并用C++运行网络
# 主要文件夹和文件
contrib/下的是捐献，包括ios的mpscnn、torch
Dockerfile指定目标新建一个镜像
Cmakelist.txt是Cmake的配置文件。在 Linux 下进行开发很多人选择编写 makefile 文件进行项目环境搭建，而makefile 文件依赖关系复杂，工作量很大。采用自动化的项目构建工具 CMake 可以将程序员从复杂的 makefile 文件中解脱出来。
core/：中心，核心，精髓;是caffe的最基础文件。包括workspace、db、blob等
proto/：和core一样重要；它们两是caffe2的核心
scripts/:可以看到iOS、Android、Raspberry PI、windows等平台的编译脚本，而仅仅改动几行，就可以支持watchOS 
算子目录（Operators Catalogue）
查看稀疏操作（Sparse Operations）
只需要运行几行代码即可调用Caffe2中预先训练好的Model Zoo模型。  
- Caffe2框架已经应用在Facebook平台上
# caffe2的目录结构
├── binaries 
├── contrib 
│   ├── docker-ubuntu-14.04 
│   ├── gloo 
│   ├── mpscnn-fb 
│   ├── nccl 
│   ├── nervana 
│   ├── nnpack 
│   ├── prof 
│   ├── snpe-fb 
│   ├── torch 
│   └── warpctc 
├── core 
├── cuda_rtc 
├── db 
├── distributed 
├── experiments 
│   ├── operators 
│   └── python 
├── image 
├── mkl 
│   └── operators 
├── mpi 
├── operators 
├── proto 
├── python 
│   ├── docs 
│   ├── examples 
│   ├── helpers 
│   ├── layers 
│   ├── mint 
│   │   ├── static 
│   │   │   └── css 
│   │   └── templates 
│   ├── models 
│   ├── operator_test 
│   ├── predictor 
│   ├── tutorial 
│   └── tutorials 
│       ├── experimental 
│       └── images 
├── queue 
├── sgd 
├── test 
│   └── assets 
└── utils 
    ├── mkl 
    └── threadpool
48 directories
