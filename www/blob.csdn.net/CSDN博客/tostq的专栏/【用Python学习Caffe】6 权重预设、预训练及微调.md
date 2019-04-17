# 【用Python学习Caffe】6. 权重预设、预训练及微调 - tostq的专栏 - CSDN博客





2017年06月22日 22:19:16[tostq](https://me.csdn.net/tostq)阅读数：3522
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 6. 权重预设、预训练及微调

通过上一节，我们将得到solver文件，得到该文件后，进行网络的训练及测试将变得非常简单。 

在通过`solver = caffe.SGDSolver(solver_proto)`初始化解决器后，训练一般有两种方式，一是通过`solver.solve()`直接进行训练，二是通过`solver.step(1)`进行单步训练。

### 6.1 solver直接训练及单步训练
        if is_step==False:
            # 直接完成训练
            solver.solve()
        else:
            # 迭代次数
            max_iter = 10000
            # 每隔100次收集一次数据
            display = 100

            # 每次测试进行100次解算，10000/100
            test_iter = 100
            # 每500次训练进行一次测试（100次解算），60000/64
            test_interval = 500

            # 初始化
            train_loss = np.zeros(int(math.ceil(max_iter * 1.0 / display)))
            test_loss = np.zeros(int(math.ceil(max_iter * 1.0 / test_interval)))
            test_acc = np.zeros(int(math.ceil(max_iter * 1.0 / test_interval)))

            # iteration 0，不计入
            solver.step(1)

            # 辅助变量
            _train_loss = 0
            _test_loss = 0
            _accuracy = 0

            # 分步训练
            for it in range(max_iter):
                # 进行一次解算
                solver.step(1)
                # 每迭代一次，训练batch_size张图片
                _train_loss += solver.net.blobs['loss'].data # 最后一层的损失值
                if it % display == 0:
                    # 计算平均train loss
                    train_loss[int(it / display)] = _train_loss / display
                    _train_loss = 0

                # 测试
                if it % test_interval == 0:
                    for test_it in range(test_iter):
                        # 进行一次测试
                        solver.test_nets[0].forward()
                        # 计算test loss
                        _test_loss += solver.test_nets[0].blobs['loss'].data
                        # 计算test accuracy
                        _accuracy += solver.test_nets[0].blobs['accuracy'].data
                        # 计算平均test loss
                    test_loss[it / test_interval] = _test_loss / test_iter
                    # 计算平均test accuracy
                    test_acc[it / test_interval] = _accuracy / test_iter
                    _test_loss = 0
                    _accuracy = 0

### 6.2 权重预设

当然很多情况下，我们可能需要从已经训练好的文件中重新开始训练。这种情况下，可以通过`solver.net.copy_from(caffemodel)`来导入已训练模型，从已知的权重连接处开始新的训练。

### 6.3 截取已知网络作为初始训练权重

而有时，我们只需要截取某些其他网络结构的部分参数作为权重预值，比如说SSD会截取VGG网络前段部分的参数作为SSD的特征提取层的初始参数，来减少总体的训练时间，这时我们可以通过如下方法来解决
    def crop_network(prune_proto, caffemodel, prune_caffemodel):
        # 截取已知网络的部分层
        #  caffemodel网络权重值并不要求其结构与proto相对应
        # 网络只会取train_proto中定义的结构中权重作为网络的初始权重值
        # 因此，当我们需要截取某些已训练网络的特定层作为新网络的某些层的权重初始值，只需要在其train_proto定义同名的层
        # 之后caffe将在caffemodel中找到与train_proto定义的同名结构，并将其权重作为应用权重初始值。
        # prune_deploy: 选择保留的网络结构层:prototxt
        # caffemodel: 已知网络的权重连接
        # prune_caffemodel：截断网络的权重连接文件
        net = caffe.Net(prune_proto, caffemodel, caffe.TEST)
        net.save(prune_caffemodel)

### 6.4 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[train_val.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/train_val.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



