# 稀疏矩阵 存储格式 COO CSR DIA ELL HYB - YZXnuaa的博客 - CSDN博客
2018年03月20日 16:22:16[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：382
稀疏矩阵是指矩阵中的元素大部分是0的矩阵，事实上，实际问题中大规模矩阵基本上都是稀疏矩阵，很多稀疏度在90%甚至99%以上。因此我们需要有高效的稀疏矩阵存储格式。本文总结几种典型的格式：COO,CSR,DIA,ELL,HYB。
（1）Coordinate（COO）
![image](http://images.cnitblog.com/blog/354318/201502/042300495151681.png)
这是最简单的一种格式，每一个元素需要用一个三元组来表示，分别是（行号，列号，数值），对应上图右边的一列。这种方式简单，但是记录单信息多（行列），每个三元组自己可以定位，因此空间不是最优。
（2）Compressed Sparse Row (CSR)
![image](http://images.cnitblog.com/blog/354318/201502/042300509215911.png)
CSR是比较标准的一种，也需要三类数据来表达：数值，列号，以及行偏移。CSR不是三元组，而是整体的编码方式。数值和列号与COO一致，表示一个元素以及其列号，行偏移表示某一行的第一个元素在values里面的起始偏移位置。如上图中，第一行元素1是0偏移，第二行元素2是2偏移，第三行元素5是4偏移，第4行元素6是7偏移。在行偏移的最后补上矩阵总的元素个数，本例中是9。
CSC是和CSR相对应的一种方式，即按列压缩的意思。
以上图中矩阵为例：
Values：        [1 5 7 2 6 8 3 9 4]
Row Indices：[0 2 0 1 3 1 2 2 3]
Column Offsets：[0 2 5 7 9]
再来看一个CSR的例子[4]：
![96202_2458312_1](http://images.cnitblog.com/blog/354318/201502/042300522497883.png)
（3）ELLPACK (ELL)
![image](http://images.cnitblog.com/blog/354318/201502/042300536567411.png)
用两个和原始矩阵相同行数的矩阵来存：第一个矩阵存的是列号，第二个矩阵存的是数值，行号就不存了，用自身所在的行来表示；这两个矩阵每一行都是从头开始放，如果没有元素了就用个标志比如*结束。上图中间矩阵有误，第三行应该是  0 2 3。
注：这样如果某一行很多元素，那么后面两个矩阵就会很胖，其他行结尾*很多，浪费。可以存成数组，比如上面两个矩阵就是：
0 1 * 1 2 * 0 2 3 * 1 3 *
1 7 * 2 8 * 5 3 9 * 6 4 *
但是这样要取一行就比较不方便了
（4）Diagonal (DIA)
![image](http://images.cnitblog.com/blog/354318/201502/042300550622642.png)
对角线存储法，按对角线方式存，列代表对角线，行代表行。省略全零的对角线。(从左下往右上开始：第一个对角线是零忽略，第二个对角线是5，6，第三个对角线是零忽略，第四个对角线是1，2，3，4，第五个对角线是7，8，9，第六第七个对角线忽略)。[3]
这里行对应行，所以5和6是分别在第三行第四行的，前面补上无效元素*。如果对角线中间有0，存的时候也需要补0，所以如果原始矩阵就是一个对角性很好的矩阵那压缩率会非常高，比如下图，但是如果是随机的那效率会非常糟糕。
![image](http://images.cnitblog.com/blog/354318/201502/042300558749757.png)![image](http://images.cnitblog.com/blog/354318/201502/042300570623986.png)
（5）Hybrid (HYB) ELL + COO
![image](http://images.cnitblog.com/blog/354318/201502/042300584844987.png)
为了解决（3）ELL中提到的，如果某一行特别多，造成其他行的浪费，那么把这些多出来的元素（比如第三行的9，其他每一行最大都是2个元素）用COO单独存储。
选择稀疏矩阵存储格式的一些经验[2]：
- DIA和ELL格式在进行稀疏矩阵-矢量乘积(sparse matrix-vector products)时效率最高，所以它们是应用迭代法(如共轭梯度法)解稀疏线性系统最快的格式；
- COO和CSR格式比起DIA和ELL来，更加灵活，易于操作；
- ELL的优点是快速，而COO优点是灵活，二者结合后的HYB格式是一种不错的稀疏矩阵表示格式；
- 根据[Nathan Bell的工作](http://www.bu.edu/pasi/files/2011/01/NathanBell1-10-1000.pdf)，CSR格式在存储稀疏矩阵时非零元素平均使用的字节数(Bytes per Nonzero Entry)最为稳定（float类型约为8.5，double类型约为12.5），而DIA格式存储数据的非零元素平均使用的字节数与矩阵类型有较大关系，适合于StructuredMesh结构的稀疏矩阵（float类型约为4.05，double类型约为8.10），对于Unstructured Mesh以及Random Matrix,DIA格式使用的字节数是CSR格式的十几倍；
- 从我使用过的一些线性代数计算库来说，COO格式常用于从文件中进行稀疏矩阵的读写，如matrix market即采用COO格式，而CSR格式常用于读入数据后进行稀疏矩阵计算。
一些特殊类型矩阵的存储效率（数值越小说明压缩率越高，即存储效率越高）:
Structured Mesh
![image](http://images.cnitblog.com/blog/354318/201502/042300596248903.png)
Unstructured Mesh
![image](http://images.cnitblog.com/blog/354318/201502/042301009531876.png)
Random matrix
![image](http://images.cnitblog.com/blog/354318/201502/042301024373662.png)
Power-Law Graph
![image](http://images.cnitblog.com/blog/354318/201502/042301034843848.png)
