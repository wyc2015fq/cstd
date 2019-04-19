# WWN1：where-what network 1 - sinat_31425585的博客 - CSDN博客
2018年12月17日 13:09:05[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：59
    where-what networks系列模型是翁巨扬教授提出自主心智发育的第二个框架，类比于现有网络结构，之前说的multi-layer in-place learning network 的结构相当于传统神经网络，后面where-what network的结构相当于现在流行的卷积神经网络，当然，内部的学习机制、层与层之间连接方式及信息在网络中流动的形式都有大大的不同。
**    1、模型结构**
    其实一个视觉任务，最重要的部分其实就是回答两个问题——在哪里？（where）和是什么？（what）。
    WWN-1的网络结构如图1所示。网络分为4层：Layer 0，Layer 1， Layer 2和Layer 3。Layer 0 对应数据输入，其中图像大小为40x40，而目标物体大小为20x20；Layer 1 和Layer 2 对应中间层（隐层）用于提取特征；Layer 3 对应动作层，可以分为where Motor（指示当前情况下，目标物体的位置，回答在哪里的问题）和what Motor（指示当前情况下，目标物体的类别，回答是什么的问题）。
![](https://img-blog.csdn.net/20181003075004750?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                     图1 WWN-1 网络模型结构示意图
    不同于之前描述的MILN网络，WWN-1网络中，每层中神经元都是局部连接的，这种局部连接的堆叠，就能形成一种感受野由小变大的过程（类似于卷积神经网络中的卷积结构），这个过程如图2所示。
![](https://img-blog.csdn.net/20181003080816923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                         图2 WWN1 局部连接结构形成感受野过程图
    Layer1 中，每个神经元对应于Layer 0 中神经元个数为 3x3=9，所以Layer1 中每个神经元的感受野大小为9，Layer 2 中每个神经元对应于Layer 1 中神经元个数同样为 3x3=9，但是这3x3个神经元感受野之间存在重叠部分，最后每个神经元对应于Layer 0 中神经元个数就变成了17，同样道理，Layer3 中每个神经元的感受野大小为25。
参考文献：
[1] Ji Z , Weng J , Prokhorov D . Where-what network 1: “Where” and “what” assist each other through top-down connections[C]. IEEE International Conference on Development & Learning. IEEE, 2008.
~~~未完待续~~~
