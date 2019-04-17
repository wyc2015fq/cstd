# 【用Python学习Caffe】7. 网络结构的修剪 - tostq的专栏 - CSDN博客





2017年06月22日 22:23:06[tostq](https://me.csdn.net/tostq)阅读数：5157
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 7. 网络结构的修剪

网络结构的压缩是近年来研究热点，接下来的两节，我们将介绍Deep Compression的两个策略网络修剪和网络权重共享量化的实现方法，我们通过mnist的LeNet5作为例子，而其他网络的实现也是类似的。

关于Deep Compression的原理，可以参见其论文：Han S, Mao H, Dally W J. **Deep compression: Compressing deep neural networks with pruning, trained quantization and huffman coding[C]**. In Proc. International Conference on Learning Representations. 2016.

所谓的网络修剪的概念，并不复杂，其大体思想是将不重要的权重连接删除，只保留最重要的连接，而什么是最重要的连接，一般作为是权重值接近0的连接越不重要，有些也认为权重值的Hession值越小的越不重要，而计算Hession值的计算太复杂了，这里参考Deep Compression通用思想将权重值接近0的视为不重要的权重的连接。

网络修剪的第二个问题是，网络修剪率如何设置，对某层到底应该删除多少连接。直观上说，如何某层连接越多，其该删除的连接也越多，比如全连接层的修剪率就要比卷积层多。但如何设置呢？目前的方法一般是采用试错实验来考察不同修剪率对网络的影响来确定合适的修剪率。

网络修剪的第三个问题是如何在修剪后，保证网络精确度不变。可以明确的说，在直接删除网络部分连接后，网络精度肯定会下降的。因此要保证网络精确度不变，需要对修剪后的网络进行重新训练，在多次重新训练后，网络的精度会提升，达到原始网络的精度，甚至在一些情况下，由于网络的稀疏度提高，减少了网络的过拟合，从而达到提高网络精度的效果。

> 
值得注意的是，修剪后网络中，值为0的权重连接在重新训练过程中，将会一直保持为0。


### 7.1 网络权重的修剪
    def prune(threshold, test_net, layers):
        sqarse_net = {}

        for i, layer in enumerate(layers):

            print '\n============  Pruning %s : threshold=%0.2f   ============' % (layer,threshold[i])
            W = test_net.params[layer][0].data
            b = test_net.params[layer][1].data
            hi = np.max(np.abs(W.flatten()))
            hi = np.sort(-np.abs(W.flatten()))[int((len(W.flatten())-1)* threshold[i])]

            # abs(val)  = 0         ==> 0
            # abs(val) >= threshold ==> 1
            interpolated = np.interp(np.abs(W), [0, hi * threshold[i], 999999999.0], [0.0, 1.0, 1.0])

            # 小于阈值的权重被随机修剪
            random_samps = np.random.rand(len(W.flatten()))
            random_samps.shape = W.shape

            # 修剪阈值
            # mask = (random_samps 
### 7.2 考察不同修剪率下的网络精度变化
    def eval_prune_threshold(threshold_list, test_prototxt, caffemodel, prune_layers):
        def net_prune(threshold, test_prototx, caffemodel, prune_layers):
            test_net = caffe.Net(test_prototx, caffemodel, caffe.TEST)
            return prune(threshold, test_net, prune_layers)

        accuracy = []
        for threshold in threshold_list:
            results = net_prune(threshold, test_prototxt, caffemodel, prune_layers)
            print 'threshold: ', results[0]
            print '\ntotal_percentage: ', results[1]
            print '\npercentage_list: ', results[2]
            print '\ntest_loss: ', results[3]
            print '\naccuracy: ', results[4]
            accuracy.append(results[4])
        plt.plot(accuracy,'r.')
        plt.show()

下图显示不同层的不同修剪率对整个网络精度的影响，以下是修剪率实验设置
    test_threshold_list = [
    [0.3, 1 ,1 ,1], [0.4, 1 ,1 ,1], [0.5, 1 ,1 ,1], [0.6, 1 ,1 ,1], [0.7, 1 ,1 ,1],
    [1, 0.05, 1, 1], [1, 0.1, 1, 1], [1, 0.15, 1, 1], [1, 0.2, 1, 1], [1, 0.3, 1, 1],
    [1, 1, 0.05, 1], [1, 1, 0.1, 1], [1, 1, 0.15, 1], [1, 1, 0.2, 1], [1, 1, 0.3, 1],
    [1, 1, 1, 0.05], [1, 1, 1, 0.1], [1, 1, 1, 0.15], [1, 1, 1, 0.2], [1, 1, 1, 0.3]]

上面每个数组都有4个值，分别表示`'conv1','conv2','ip1','ip2'`各层的修剪率，为1表示不修剪，为0.3表示只保留权重值最大的30%的连接。

![](http://i.imgur.com/2ufJ6jr.png)

根据图上，我们可以选择`'conv1','conv2','ip1','ip2'`各层的修剪率分别为`[0.3, 0.1, 0.01, 0.2]`

### 7.3 修剪网络的重新训练
    # 迭代训练修剪后网络
    def retrain_pruned(solver, pruned_caffemodel, threshold, prune_layers):
        #solver = caffe.SGDSolver(solver_proto)
        retrain_iter = 20

        accuracys = []
        for i in range(retrain_iter):
            solver.net.copy_from(pruned_caffemodel)
            # solver.solve()
            solver.step(500)
            _,_,_,_,accuracy=prune(threshold, solver.test_nets[0], prune_layers)
            solver.test_nets[0].save(pruned_caffemodel)
            accuracys.append(accuracy)

        plt.plot(accuracys, 'r.-')
        plt.show()

重新迭代训练时，其精度的变化图，可以看出随着迭代次数增加，其精确度逐渐增加。最终大概只保留了2%左右的权重连接，就达到了原来的精确度。

![](http://i.imgur.com/NbvVmf7.png)

### 7.4 稀疏结构的存储

实际上这里的网络修剪并不会在实际内存上减少网络的大小，只会减少网络模型的存储空间，因为该稀疏结构并不是一个通用结构，而是一组随机分布的结构，因此该稀疏结构我们是通过spicy的CSC格式来存储的。

> 
所谓CSC格式，即为按行展开的形式，其将稀疏的矩阵按行展开成一列，只保存不为0的权重值及该值在矩阵中的相对位置。同理还有按列展开的形式CSR。

        test_net.params[layer][0].data[...] = W
        # net.params[layer][0].mask[...] = mask
        csc_W, csc_W_indx = dense_to_sparse_csc(W.flatten(), 8)
        dense_W = sparse_to_dense_csc(csc_W, csc_W_indx)
        sqarse_net[layer + '_W'] = csc_W
        sqarse_net[layer + '_W_indx'] = csc_W_indx

### 7.5 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[prune.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/prune.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



