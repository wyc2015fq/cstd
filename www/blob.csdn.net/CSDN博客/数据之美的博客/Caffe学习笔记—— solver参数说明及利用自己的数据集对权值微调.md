# Caffe学习笔记—— solver参数说明及利用自己的数据集对权值微调 - 数据之美的博客 - CSDN博客
2017年04月12日 19:07:57[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：482
本文主要介绍：在进行网络训练和权值微调过程中，需要对solver中的参数进行设置，本文主要介绍solver中的参数设置，以及如何利用自己的数据集，对权值进行微调。
**1. solver参数设置**
**[cpp]**[view
 plain](http://blog.csdn.net/hong__fang/article/details/52433300#)[copy](http://blog.csdn.net/hong__fang/article/details/52433300#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1865649/fork)
- net: "train_val.prototxt"//深度学习模型的网络结构文件
- test_iter: 1000     //1000指的是测试的批次，测试样本较多时，一次性执行全部数据，效率较低，因此分几个批次进行执行，
- //如果每个批次数量设为batch_size,则有以下关系：test_iter = 测试样本总数/batch_size
- 
- test_interval: 1000 //测试间隔，即每训练1000次，进行一次测试 
- 
- base_lr: 0.01       // 基础学习速率（初始学习率），迭代过程中可以对基础学习速率进行调整，调整策略通过lr_policy来设置
- lr_policy: "step"// lr_policy设置参数：
- //- fixed:　　 保持base_lr不变.
- //- step: 　　 如果设置为step,则还需要设置一个stepsize,  返回 base_lr * gamma ^ (floor(iter / stepsize)),其中iter表示当前的迭代次数
- //- exp:   　　返回base_lr * gamma ^ iter， iter为当前迭代次数
- //- inv:　　    如果设置为inv,还需要设置一个power, 返回base_lr * (1 + gamma * iter) ^ (- power)
- //- multistep: 如果设置为multistep,则还需要设置一个stepvalue。这个参数和step很相似，step是均匀等间隔变化，而multistep则是根据                                 stepvalue值变化
- //- poly: 　　  学习率进行多项式误差, 返回 base_lr (1 - iter/max_iter) ^ (power)
- //- sigmoid:　学习率进行sigmod衰减，返回 base_lr ( 1/(1 + exp(-gamma * (iter - stepsize))))                  
- gamma: 0.1          //学习率变化的比率
- stepsize: 3000      //每迭代3000次，调整一次学习率  
- 
- 
- display: 20  //每20次迭代，显示一次  
- max_iter: 12000 //最大迭代次数。这个数设置太小，会导致没有收敛，精确度很低。设置太大，会导致震荡，浪费时间。
- momentum: 0.9   //学习的参数，不用变
- weight_decay: 0.0005  //权重衰减项，防止过拟合的一个参数
- snapshot: 5000  //每迭代5000次，保存一次训练权值
- snapshot_prefix: "caffenet_train"//设置保存训练权值的路径 
- solver_mode: GPU  //选择使用CPU还是GPU运行
注：详细的介绍见 [http://blog.csdn.net/langb2014/article/details/50457774](http://blog.csdn.net/langb2014/article/details/50457774)
**2. 权值微调**
**2.1 数据转化及网络设置**
该过程参考 [http://blog.csdn.net/hong__fang/article/details/52425105](http://blog.csdn.net/hong__fang/article/details/52425105) 中的1到3.2；
**2.2 参数微调**
（1）创建 finetune.txt ，添加内容
**[cpp]**[view
 plain](http://blog.csdn.net/hong__fang/article/details/52433300#)[copy](http://blog.csdn.net/hong__fang/article/details/52433300#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1865649/fork)
- SET GLOG_logtostderr=1  
- ..\caffe-windows-master\bin\caffe.exe   train --solver=.\model\solver.prototxt  --weights=.\model\bvlc_reference_caffenet.caffemodel  
- pause  
第二行有三个参数，分别为：
**[cpp]**[view
 plain](http://blog.csdn.net/hong__fang/article/details/52433300#)[copy](http://blog.csdn.net/hong__fang/article/details/52433300#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1865649/fork)
- ..\caffe-windows-master\bin\caffe.exe    //安装配置时生成的caffe.exe路径
- train                                    //表示训练过程
- --solver=.\model\solver.prototxt         //参数设置文件
- --weights=.\model\bvlc_reference_caffenet.caffemodel //现有权值文件，在此权值基础上进行权值微调
注：
<1>以上路径均为相对路径；
<2>路径中不能含有中文。
（2）将finetune.txt 改名为finetune.bat，双击运行。
