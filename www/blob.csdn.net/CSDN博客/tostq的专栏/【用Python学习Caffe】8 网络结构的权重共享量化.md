# 【用Python学习Caffe】8. 网络结构的权重共享量化 - tostq的专栏 - CSDN博客





2017年06月22日 22:28:01[tostq](https://me.csdn.net/tostq)阅读数：3722
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 8. 网络结构的权重共享量化

网络权重共享量化也是一类重要的网络压缩方法，其本质在于先通过聚类方法得到该层权重的聚类中心，然后通过聚类中心值来表示原权重值。因此权重值并不是由32位的浮点数来表示，而是由其对应的聚类中心的序号表示，如果聚类级别为8位，此时权重值只需要用8位就能表示。

对于网络权重量化也有三个问题：
- 量化级别的确定，同修剪率一样，可以通过试错的试验的方法来确定
- 量化后网络重新训练问题
- 量化中心的初始选择问题：聚类中心采用线性方法初始化，将初始点均匀分散，这种初始化方法不仅操作简单，而且能够将对网络影响较大但实际分布较少的较大权重值也包含到初始中心点中，因此不容易造成较大权重的丢失。

### 8.1 Kmean聚类得到每层的聚类中心

对于Kmean聚类方法，这里调用的是scipy库的聚类函数
    # 获得各层的量化码表
    def kmeans_net(net, layers, num_c=16, initials=None):
        # net: 网络
        # layers: 需要量化的层
        # num_c: 各层的量化级别
        # initials: 初始聚类中心
        codebook = {} # 量化码表
        if type(num_c) == type(1):
            num_c = [num_c] * len(layers)
        else:
            assert len(num_c) == len(layers)

        # 对各层进行聚类分析
        print "==============Perform K-means============="
        for idx, layer in enumerate(layers):
            print "Eval layer:", layer
            W = net.params[layer][0].data.flatten()
            W = W[np.where(W != 0)] # 筛选不为0的权重
            # 默认情况下，聚类中心为线性分布中心
            if initials is None:  # Default: uniform sample
                min_W = np.min(W)
                max_W = np.max(W)
                initial_uni = np.linspace(min_W, max_W, num_c[idx] - 1)
                codebook[layer], _ = scv.kmeans(W, initial_uni)
            elif type(initials) == type(np.array([])):
                codebook[layer], _ = scv.kmeans(W, initials)
            elif initials == 'random':
                codebook[layer], _ = scv.kmeans(W, num_c[idx] - 1)
            else:
                raise Exception

            # 将0权重值附上
            codebook[layer] = np.append(0.0, codebook[layer])
            print "codebook size:", len(codebook[layer])

        return codebook

### 8.2 量化各层

通过各层聚类来进行各层权重的量化
    def quantize_net_with_dict(net, layers, codebook, use_stochastic=False, timing=False):
        start_time = time.time()
        codeDict = {} # 记录各个量化中心所处的位置
        maskCode = {} # 各层量化结果
        for layer in layers:
            print "Quantize layer:", layer
            W = net.params[layer][0].data
            if use_stochastic:
                codes = stochasitc_quantize2(W.flatten(), codebook[layer])
            else:
                codes, _ = scv.vq(W.flatten(), codebook[layer])
            W_q = np.reshape(codebook[layer][codes], W.shape)
            net.params[layer][0].data[...] = W_q

            maskCode[layer] = np.reshape(codes, W.shape)
            codeBookSize = len(codebook[layer])
            a = maskCode[layer].flatten()
            b = xrange(len(a))

            codeDict[layer] = {}
            for i in xrange(len(a)):
                codeDict[layer].setdefault(a[i], []).append(b[i])

        if timing:
            print "Update codebook time:%f" % (time.time() - start_time)

        return codeDict, maskCode

### 8.3 重新训练及聚类中心的更新
    @static_vars(step_cache={}, step_cache2={}, count=0)
    def update_codebook_net(net, codebook, codeDict, maskCode, args, update_layers=None, snapshot=None):

        start_time = time.time()
        extra_lr = args['lr'] # 基础学习速率
        decay_rate = args['decay_rate'] # 衰减速率
        momentum = args['momentum'] # 遗忘因子
        update_method = args['update'] # 更新方法
        smooth_eps = 0

        normalize_flag = args['normalize_flag'] # 是否进行归一化


        if update_method == 'rmsprop':
            extra_lr /= 100

        # 对码表与量化结果的初始化
        if update_codebook_net.count == 0:
            step_cache2 = update_codebook_net.step_cache2
            step_cache = update_codebook_net.step_cache
            if update_method == 'adadelta':
                for layer in update_layers:
                    step_cache2[layer] = {}
                    for code in xrange(1, len(codebook[layer])):
                        step_cache2[layer][code] = 0.0
                smooth_eps = 1e-8

            for layer in update_layers:
                step_cache[layer] = {}
                for code in xrange(1, len(codebook[layer])):
                    step_cache[layer][code] = 0.0

            update_codebook_net.count = 1

        else:
            # 读入上次运算的结果
            step_cache2 = update_codebook_net.step_cache2
            step_cache = update_codebook_net.step_cache
            update_codebook_net.count += 1

        # 所有层名
        total_layers = net.params.keys()
        if update_layers is None: # 所有层都需要进行更新
            update_layers = total_layers

        # 权重码表的更新
        for layer in total_layers:
            if layer in update_layers:
                diff = net.params[layer][0].diff.flatten() # 误差梯度
                codeBookSize = len(codebook[layer])
                dx = np.zeros((codeBookSize)) # 编码表的误差更新
                for code in xrange(1, codeBookSize):
                    indexes = codeDict[layer][code] # codeDict保存属于某编码的权重的序号
                    #diff_ave = np.sum(diff[indexes]) / len(indexes)
                    diff_ave = np.sum(diff[indexes]) # 统计该编码所有的误差更新和

                    # 针对于不同方法进行更新
                    if update_method == 'sgd':
                        dx[code] = -extra_lr * diff_ave
                    elif update_method == 'momentum':
                        if code in step_cache[layer]:
                            dx[code] = momentum * step_cache[layer][code] - (1 - momentum) * extra_lr * diff_ave
                            step_cache[layer][code] = dx
                    elif update_method == 'rmsprop':
                        if code in step_cache[layer]:
                            step_cache[layer][code] = decay_rate * step_cache[layer][code] + (1.0 - decay_rate) * diff_ave ** 2
                            dx[code] = -(extra_lr * diff_ave) / np.sqrt(step_cache[layer][code] + 1e-6)
                    elif update_method == 'adadelta':
                        if code in step_cache[layer]:
                            step_cache[layer][code] = step_cache[layer][code] * decay_rate + (1.0 - decay_rate) * diff_ave ** 2
                            dx[code] = -np.sqrt((step_cache2[layer][code] + smooth_eps) / (step_cache[layer][code] + smooth_eps)) * diff_ave
                            step_cache2[layer][code] = step_cache2[layer][code] * decay_rate + (1.0 - decay_rate) * (dx[code] ** 2)

                # 是否需要进行归一化更新参数
                if normalize_flag:
                    codebook[layer] += extra_lr * np.sqrt(np.mean(codebook[layer] ** 2)) / np.sqrt(np.mean(dx ** 2)) * dx
                else:
                    codebook[layer] += dx
            else:
                pass

            # maskCode保存编码结果
            W2 = codebook[layer][maskCode[layer]]
            net.params[layer][0].data[...] = W2 # 量化后权重值

        print "Update codebook time:%f" % (time.time() - start_time)

重新训练时，其精度的变化图，可以看到随着迭代次数增加，其精度也逐渐提升

![](http://i.imgur.com/BAjBgjA.png)

### 8.4 网络压缩未来的方向

从上面可以看出来，在训练中，各网络中的权重仍是32位的浮点数，而不是用8位来表示，而即使在实际运行中，也必须通过聚类中心表将量化后权重值转换为32位的浮点数，因此并不能在减少网络的实际运行内存，只是减少网络的内存消耗。

要真正减少网络内存消耗，从而达到网络实际运行速度的提高，目前有两类主流方法：
- 
让网络保证结构上的稀疏性，而不是随机分布的稀疏性。可以参考论文： 

Wen W, Wu C, Wang Y, et al. **Learning Structured Sparsity in Deep Neural Networks[J]**. 2016.

- 
设计量化运算的网络，比如QNN及BNN等等。可以参考论文：Hubara I, Courbariaux M, Soudry D, et al. **Quantized neural networks: Training neural networks with low precision weights and activations[J]**. Journal of Machine Learning Research. 2016, 1: 1–29.


### 8.5 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[quantize.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/quantize.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



