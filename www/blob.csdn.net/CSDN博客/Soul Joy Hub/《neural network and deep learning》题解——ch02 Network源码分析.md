# 《neural network  and deep learning》题解——ch02 Network源码分析 - Soul Joy Hub - CSDN博客

2017年07月12日 12:24:13[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：932
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/75008380](http://blog.csdn.net/u011239443/article/details/75008380)

完整代码：[https://github.com/xiaoyesoso/neural-networks-and-deep-learning/blob/master/src/network.py](https://github.com/xiaoyesoso/neural-networks-and-deep-learning/blob/master/src/network.py)

# 初始化

```python
# sizes 是每层节点数的数组
    def __init__(self, sizes):
        self.num_layers = len(sizes)
        self.sizes = sizes
        # randn 产生 高斯分布的随机数值矩阵
        # 输入层 有没有 biases
        self.biases = [np.random.randn(y, 1) for y in sizes[1:]]
        # 层与层之间 都有 weights
        # y 是下一层的节点数，x 是上一层的节点数
        self.weights = [np.random.randn(y, x)
for x, y in zip(sizes[:-1], sizes[1:])]
```

# 训练

```python
# training_data 训练数据
    # epochs 迭代次数
    # mini_batch_size 小批数据大小
    # test_data 测试数据
    def SGD(self, training_data, epochs, mini_batch_size, eta,
            test_data=None):
        if test_data: n_test = len(test_data)
        n = len(training_data)
        for j in xrange(epochs):
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in xrange(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                # 这里更新模型
                self.update_mini_batch(mini_batch, eta)
            if test_data:
               # 若 test_data ！= None，
               # 预测 验证
                print "Epoch {0}: {1} / {2}".format(
                    j, self.evaluate(test_data), n_test)
            else:
print "Epoch {0} complete".format(j)
```

## 更新模型：

```python
def update_mini_batch(self, mini_batch, eta):
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x, y in mini_batch:
         # 得到反向传播调整
            delta_nabla_b, delta_nabla_w = self.backprop(x, y)
            nabla_b = [nb+dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
            nabla_w = [nw+dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
         # 更新参数
        self.weights = [w-(eta/len(mini_batch))*nw
                        for w, nw in zip(self.weights, nabla_w)]
        self.biases = [b-(eta/len(mini_batch))*nb
for b, nb in zip(self.biases, nabla_b)]
```

## 反向传播

可以先回顾下方向传播的四个公式：[http://blog.csdn.net/u011239443/article/details/74859614](http://blog.csdn.net/u011239443/article/details/74859614)

```python
def backprop(self, x, y):
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        # activation 为每层的激活函数
        # 输入层没有激活函数
        activation = x
        activations = [x] 
        # zs 为除了第一层外的每一层的输入
        zs = [] 
        for b, w in zip(self.biases, self.weights):
            z = np.dot(w, activation)+b
            zs.append(z)
            activation = sigmoid(z)
            activations.append(activation)
        # cost_derivative 就是求两者的误差
        # sigmoid_prime 为 sigmoid 的导数
        # 可见 公式（BP1）
        delta = self.cost_derivative(activations[-1], y) * \
            sigmoid_prime(zs[-1])
        # 可见 公式（BP3）
        nabla_b[-1] = delta
        # 可见 公式（BP4）
        nabla_w[-1] = np.dot(delta, activations[-2].transpose())
        for l in xrange(2, self.num_layers):
            z = zs[-l]
            sp = sigmoid_prime(z)
            # 可见 公式（BP2）
            delta = np.dot(self.weights[-l+1].transpose(), delta) * sp
            nabla_b[-l] = delta
            nabla_w[-l] = np.dot(delta, activations[-l-1].transpose())
return (nabla_b, nabla_w)
```

# 测试

回到`SGD`中的`def evaluate`：

```python
def evaluate(self, test_data):
     # np.argmax(self.feedforward(x)) 预测结果并取整
        test_results = [(np.argmax(self.feedforward(x)), y)
                        for (x, y) in test_data]
return sum(int(x == y) for (x, y) in test_results)
```

```python
def feedforward(self, a):
      # 把测试数据代入训练好的网络
        for b, w in zip(self.biases, self.weights):
            a = sigmoid(np.dot(w, a)+b)
return a
```

![这里写图片描述](http://upload-images.jianshu.io/upload_images/1621805-e34221289ab19bd4?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

