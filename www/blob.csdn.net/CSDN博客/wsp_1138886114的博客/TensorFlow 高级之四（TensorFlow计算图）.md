# TensorFlow 高级之四（TensorFlow计算图） - wsp_1138886114的博客 - CSDN博客





2018年08月05日 13:07:05[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：299








##  TensorFlow计算图

#### 一、 创建Session 以及变量初始化

```
TensorFlow计算图运行在一个session中，计算之前需要先创建这个session，并初始化其中的一些变量（w 和 b）， 
TensorFlow使用session.run()来运行计算图。 

session = tf.Session()                           # 创建session
session.run(tf.global_variables_initializer())   # 变量初始化
```

#### 二、执行优化的帮助函数

```python
定义批处理函数，这个函数在优化函数中被调用。
def next_batch(num, data, labels):
    '''
    Return a total of `num` random samples and labels. 
    '''
    idx = np.arange(0 , len(data))
    np.random.shuffle(idx)
    idx = idx[:num]
    data_shuffle = [data[i] for i in idx]
    labels_shuffle = [labels[i] for i in idx]

    return np.asarray(data_shuffle), np.asarray(labels_shuffle)

定义优化函数，并增加输出状态信息。
train_batch_size = 64
total_iterations = 0           # 统计目前总迭代次数

def optimize(num_iterations):

    global total_iterations    # 保证更新全局变量.
    start_time = time.time()   # 用来输出用时.

    for i in range(total_iterations, total_iterations + num_iterations):
        # 获取一批数据，放入dict
        #x_batch, y_true_batch = data.train.next_batch(train_batch_size)
        x_batch, y_true_batch = next_batch(train_batch_size, X_train, y_train01)
        #x_batch = X_train.next_batch(train_batch_size)
        #y_true_batch = y_test01.next_batch(train_batch_size)
        feed_dict_train = {x: x_batch,
                          y_true: y_true_batch}
        # 运行优化器
        session.run(optimizer, feed_dict=feed_dict_train)

        if i % 100 == 0:                                            # 每100轮迭代输出状态
            acc = session.run(accuracy, feed_dict=feed_dict_train)  # 计算训练集准确率.
            msg = "迭代轮次: {0:>6}, 训练准确率: {1:>6.1%}"
            print(msg.format(i + 1, acc))

    total_iterations += num_iterations
    end_time = time.time()
    time_dif = end_time - start_time

    # 输出用时.
    print("用时: " + str(timedelta(seconds=int(round(time_dif)))))
```

#### 三、 执行优化的帮助函数

```python
def plot_example_errors(cls_pred, correct):
    # 计算错误情况
    incorrect = (correct == False)
    images = X_test[incorrect]
    cls_pred = cls_pred[incorrect]
    cls_true = test[incorrect]

    # 随机挑选9个
    indices = np.arange(len(images))
    np.random.shuffle(indices)
    indices = indices[:9] 

    plot_images(images[indices], cls_true[indices], cls_pred[indices])

def plot_confusion_matrix(cls_pred):
    cls_true = test          # 真实类别  

    # 使用scikit-learn的confusion_matrix来计算混淆矩阵
    cm = confusion_matrix(y_true=cls_true, y_pred=cls_pred)

    print(cm)                # 打印混淆矩阵

    # 将混淆矩阵输出为图像
    plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Blues)
    plt.tight_layout()      # 调整图像
    plt.colorbar()
    tick_marks = np.arange(num_classes)
    plt.xticks(tick_marks, range(num_classes))
    plt.yticks(tick_marks, range(num_classes))
    plt.xlabel('Predicted')
    plt.ylabel('True')
    plt.show()
```

#### 四、 显示性能的帮助函数

> 
```
用来输出测试准确率的的函数。计算所有图像的分类需要一定的时间，
   因此我们在上面定义的一些函数中重用了分类结果。 
这个函数会占据大量的内存，所以将测试集分成了多个小的批次。如果你的机器内存太小， 
   你可以尝试减小batch_size。
```

```python
test_batch_size = 256              # 将测试集分成更小的批次   

def print_test_accuracy(show_example_errors=False,
                        show_confusion_matrix=False):
    num_test = len(X_test)         # 测试集图像数量.

    cls_pred = np.zeros(shape=num_test, dtype=np.int)    # 为预测结果申请一个数组.

    i = 0                          # 数据集的起始id为0
    while i < num_test:
        j = min(i + test_batch_size, num_test)            # j为下一批次的截止id
        images = X_test[i:j, :]    # 获取i，j之间的图像
        labels = y_test01[i:j, :]  # 获取相应标签.

        feed_dict = {x: images,   y_true: labels}   # 创建feed_dict

        # 计算预测结果
        cls_pred[i:j] = session.run(y_pred_cls, feed_dict=feed_dict)
        i = j                     # 设定为下一批次起始值.

    cls_true = test
    correct = (cls_true == cls_pred)       # 正确的分类
    correct_sum = correct.sum()            # 正确分类的数量

    acc = float(correct_sum) / num_test    # 分类准确率
    msg = "测试集准确率: {0:.1%}({1}/{2})"  # 打印准确率.
    print(msg.format(acc, correct_sum, num_test))

    if show_example_errors:                # 打印部分错误样例.
        print("Example errors:")
plot_example_errors(cls_pred=cls_pred, correct=correct)

    if show_confusion_matrix:              # 打印混淆矩阵.
        print("Confusion Matrix:")
plot_confusion_matrix(cls_pred=cls_pred)

优化前的性能测试
print_test_accuracy()
测试集准确率: 10.2% (1022 / 10000)
可以看到，测试的准确率极低，但是函数的功能正常。
```


#### 五、 执行优化后性能

> 
###### 5.1 执行一轮优化后的性能

```
optimize(num_iterations=1)
print_test_accuracy()
迭代轮次:      1, 训练准确率:  12.5%
用时: 0:00:00
测试集准确率: 10.8% (1076 / 10000)
```

###### 5.2  100轮优化后的性能

```
optimize(num_iterations=99)
print_test_accuracy()
用时: 0:00:14
测试集准确率: 80.1% (8010 / 10000)
可以看到，执行100轮迭代后，性能存在大幅度提升。
```

###### 5.3  1000轮优化后的性能

```
optimize(num_iterations=900)
print_test_accuracy(show_example_errors=True)
迭代轮次:    101, 训练准确率:  76.6%
迭代轮次:    201, 训练准确率:  76.6%
迭代轮次:    301, 训练准确率:  93.8%
迭代轮次:    401, 训练准确率:  87.5%
迭代轮次:    501, 训练准确率:  90.6%
迭代轮次:    601, 训练准确率:  96.9%
迭代轮次:    701, 训练准确率:  93.8%
迭代轮次:    801, 训练准确率:  98.4%
迭代轮次:    901, 训练准确率:  96.9%
用时: 0:02:07
测试集准确率: 95.6% (9563 / 10000)
Example errors:
```

![这里写图片描述](https://img-blog.csdn.net/2018061511085031?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 5.4 10000轮优化后的性能

```
optimize(num_iterations=9000)
print_test_accuracy(show_example_errors=True,
                    show_confusion_matrix=True)
迭代轮次:   8701, 训练准确率: 100.0%
迭代轮次:   8801, 训练准确率: 100.0%
迭代轮次:   8901, 训练准确率: 100.0%
迭代轮次:   9001, 训练准确率: 100.0%
迭代轮次:   9101, 训练准确率:  98.4%
迭代轮次:   9201, 训练准确率: 100.0%
迭代轮次:   9301, 训练准确率:  95.3%
迭代轮次:   9401, 训练准确率: 100.0%
迭代轮次:   9501, 训练准确率: 100.0%
迭代轮次:   9601, 训练准确率: 100.0%
迭代轮次:   9701, 训练准确率: 100.0%
迭代轮次:   9801, 训练准确率: 100.0%
迭代轮次:   9901, 训练准确率: 100.0%
用时: 0:21:04
测试集准确率: 98.3% (9833 / 10000)
Example errors:
```

![这里写图片描述](https://img-blog.csdn.net/2018061511133964?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
Confusion Matrix:
[[ 977    0    2    0    0    0    0    1    0    0]
 [   0 1129    5    0    0    0    0    1    0    0]
 [   0    1 1028    0    1    0    0    2    0    0]
 [   0    0    4  996    0    9    0    0    0    1]
 [   0    1    2    0  973    0    0    0    0    6]
 [   1    0    0    4    0  886    1    0    0    0]
 [   6    2    2    1    3    8  935    0    1    0]
 [   1    2   14    2    0    0    0 1002    2    5]
 [  12    0   17    5    5    9    1    2  914    9]
 [   2    0    2    2    3    6    0    1    0  993]]
```

![这里写图片描述](https://img-blog.csdn.net/20180615111609126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




