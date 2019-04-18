# TensorFlow入门教程 - jiangjingxuan的博客 - CSDN博客





2017年02月17日 19:52:01[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2081








# TensorFlow

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E4%B8%80tensorflow%E4%BB%8B%E7%BB%8D)一、TensorFlow介绍

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E4%BB%80%E4%B9%88%E6%98%AFtensorflow)1、什么是TensorFlow
- 官网：[https://www.tensorflow.org/](https://www.tensorflow.org/)
- TensorFlow是Google开发的一款神经网络的Python外部的结构包, 也是一个采用数据流图来进行数值计算的开源软件库.
- 先绘制计算结构图, 也可以称是一系列可人机交互的计算操作, 然后把编辑好的Python文件 转换成 更高效的C++, 并在后端进行计算.

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2tensorflow%E5%BC%BA%E5%A4%A7%E4%B9%8B%E5%A4%84)2、TensorFlow强大之处
- 擅长的任务就是训练深度神经网络
- 快速的入门神经网络,大大降低了深度学习（也就是深度神经网络）的开发成本和开发难度
- TensorFlow 的开源性, 让所有人都能使用并且维护

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#3%E5%AE%89%E8%A3%85tensorflow)3、安装TensorFlow
- 暂不支持Windows下安装TensorFlow,可以在虚拟机里使用或者安装Docker安装
- 这里在CentOS6.5下进行安装
- 
安装**Python2.7**，默认CentOS中安装的是**Python2.6**
- 先安装**zlib**的依赖，下面安装**easy_install**时会用到

```
yum install zlib
yum install zlib-devel
```
- 在安装**openssl**的依赖，下面安装**pip**时会用到

```
yum install openssl
yum install openssl-devel
```
- 下载安装包，我传到`github`上的安装包，`https`协议后面加上`--no-check-certificate`，：

```
wget https://raw.githubusercontent.com/lawlite19/LinuxSoftware/master/python/Python-2.7.12.tgz --no-check-certificate
```
- 解压缩：`tar -zxvf xxx`
- 进入，配置：`./configure --prefix=/usr/local/python2.7`
- 编译并安装：`make && make install`
- 创建链接来使系统默认python变为python2.7,`ln -fs /usr/local/python2.7/bin/python2.7 /usr/bin/python`
- 修改一下**yum**，因为yum的执行文件还是需要原来的**python2.6**,`vim /usr/bin/yum`

```
#!/usr/bin/python
```

修改为系统原有的python版本地址

```
#!/usr/bin/python2.6
```

- 
安装**easy_install**
- 下载：`wget https://raw.githubusercontent.com/lawlite19/LinuxSoftware/blob/master/python/setuptools-26.1.1.tar.gz --no-check-certificate`
- 解压缩：`tar -zxvf xxx`
- `python setup.py build` #注意这里python是新的python2.7
- `python setup.py install`
- 到`/usr/local/python2.7/bin`目录下查看就会看到`easy_install`了
- 创建一个软连接：`ln -s /usr/local/python2.7/bin/easy_install /usr/local/bin/easy_install`
- 就可以使用`easy_install 包名` 进行安装

- 
安装**pip**
- 下载:
- 解压缩：`tar -zxvf xxx`
- 安装：`python setup.py install`
- 到`/usr/local/python2.7/bin`目录下查看就会看到`pip`了
- 同样创建软连接：`ln -s /usr/local/python2.7/bin/pip /usr/local/bin/pip`
- 就可以使用`pip install 包名`进行安装包了

- 
安装**wingIDE**
- 默认安装到`/usr/local/lib`下，进入，执行`./wing`命令即可执行
- 创建软连接：`ln -s /usr/local/lib/wingide5.1/wing /usr/local/bin/wing`
- 破解：

- 
[另]安装**VMwareTools**，可以在windows和Linux之间复制粘贴
- 启动CentOS
- 选择VMware中的虚拟机-->安装VMware Tools
- 会自动弹出VMware Tools的文件夹
- 拷贝一份到root目录下 `cp VMwareTools-9.9.3-2759765.tar.gz /root`
- 解压缩 `tar -zxvf VMwareTools-9.9.3-2759765.tar.gz`
- 进入目录执行，`vmware-install.pl`，一路回车下去即可
- 重启CentOS即可

- 
安装**numpy**
- 直接安装没有出错

- 
安装**scipy**
- 安装依赖：`yum install bzip2-devel pcre-devel ncurses-devel readline-devel tk-devel gcc-c++ lapack-devel`
- 安装即可：`pip install scipy`

- 
安装**matplotlib**
- 安装依赖：`yum install libpng-devel`
- 安装即可：`pip install matplotlib`
- 运行可能有以下的错误：

```
ImportError: No module named _tkinter
```

安装：`tcl8.5.9-src.tar.gz`
- 进入安装即可,`./confgiure make make install`安装：`tk8.5.9-src.tar.gz`
- 进入安装即可。
- **[注意]**要重新安装一下**Pyhton2.7**才能链接到`tkinter`

- 
安装**scikit-learn**
- 直接安装没有出错，但是缺少包`bz2`
- 将系统中`python2.6`的`bz2`复制到`python2.7`对应文件夹下

```
cp /usr/lib/python2.6/lib-dynload/bz2.so /usr/local/python2.7/lib/python2.7/lib-dynload
```

- 
安装**TensorFlow**
- [官网点击](https://www.tensorflow.org/)
- 选择对应的版本

```
# Ubuntu/Linux 64-bit, CPU only, Python 2.7
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp27-none-linux_x86_64.whl

  # Ubuntu/Linux 64-bit, GPU enabled, Python 2.7
  # Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp27-none-linux_x86_64.whl

  # Mac OS X, CPU only, Python 2.7:
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.12.0rc0-py2-none-any.whl

  # Mac OS X, GPU enabled, Python 2.7:
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/gpu/tensorflow_gpu-0.12.0rc0-py2-none-any.whl

  # Ubuntu/Linux 64-bit, CPU only, Python 3.4
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp34-cp34m-linux_x86_64.whl

  # Ubuntu/Linux 64-bit, GPU enabled, Python 3.4
  # Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp34-cp34m-linux_x86_64.whl

  # Ubuntu/Linux 64-bit, CPU only, Python 3.5
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp35-cp35m-linux_x86_64.whl

  # Ubuntu/Linux 64-bit, GPU enabled, Python 3.5
  # Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp35-cp35m-linux_x86_64.whl

  # Mac OS X, CPU only, Python 3.4 or 3.5:
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.12.0rc0-py3-none-any.whl

  # Mac OS X, GPU enabled, Python 3.4 or 3.5:
  $ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/gpu/tensorflow_gpu-0.12.0rc0-py3-none-any.whl
```
- 对应`python`版本

```
# Python 2
  $ sudo pip install --upgrade $TF_BINARY_URL

  # Python 3
  $ sudo pip3 install --upgrade $TF_BINARY_URL
```
- 可能缺少依赖`glibc`,看对应提示的版本，
- 还有可能报错

```
ImportError: /usr/lib64/libstdc++.so.6: version `GLIBCXX_3.4.19' not found (required by /usr/local/python2.7/lib/python2.7/site-packages/tensorflow/python/_pywrap_tensorflow.so)
```

- 
安装对应版本的**glibc**
- 查看现有版本的glibc, `strings /lib64/libc.so.6 |grep GLIBC`
- 下载对应版本：`wget http://ftp.gnu.org/gnu/glibc/glibc-2.17.tar.gz`
- 解压缩：`tar -zxvf glibc-2.17`
- 进入文件夹创建`build`文件夹`cd glibc-2.17 && mkdir build`
- 配置：

```
../configure  \
  --prefix=/usr          \
  --disable-profile      \
  --enable-add-ons       \
  --enable-kernel=2.6.25 \
  --libexecdir=/usr/lib/glibc
```
- 编译安装：`make && make install`
- 可以再用命令：`strings /lib64/libc.so.6 |grep GLIBC`查看

- 
添加**GLIBCXX_3.4.19**的支持
- 复制到`/usr/lib64`文件夹下：`cp libstdc++.so.6.0.20 /usr/lib64/`
- 添加执行权限：`chmod +x /usr/lib64/libstdc++.so.6.0.20`
- 删除原来的：`rm -rf /usr/lib64/libstdc++.so.6`
- 创建软连接：`ln -s /usr/lib64/libstdc++.so.6.0.20 /usr/lib64/libstdc++.so.6`
- 可以查看是否有个版本：`strings /usr/lib64/libstdc++.so.6 | grep GLIBCXX`

- 
运行还可能报错编码的问题，这里安装`0.10.0`版本:`pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0rc0-cp27-none-linux_x86_64.whl`


## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E4%BA%8Ctensorflow%E5%9F%BA%E7%A1%80%E6%9E%B6%E6%9E%84)二、TensorFlow基础架构

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E5%A4%84%E7%90%86%E7%BB%93%E6%9E%84)1、处理结构
- Tensorflow 首先要定义神经网络的结构,然后再把数据放入结构当中去运算和 training
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensors_flowing.gif)
- TensorFlow是采用数据流图（data　flow　graphs）来计算
- 首先我们得创建一个数据流流图
- 然后再将我们的数据（数据以张量(tensor)的形式存在）放在数据流图中计算
- 张量（tensor):
- 张量有多种. 零阶张量为 纯量或标量 (scalar) 也就是一个数值. 比如 [1](https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/images/tensors_flowing.gif)
- 一阶张量为 向量 (vector), 比如 一维的 [1, 2, 3]
- 二阶张量为 矩阵 (matrix), 比如 二维的 [[1, 2, 3],[4, 5, 6],[7, 8, 9]]
- 以此类推, 还有 三阶 三维的 …


### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90)2、一个例子
- 
求`y=1*x+3`中的权重`1`和偏置`3`
- 定义这个函数

```
x_data = np.random.rand(100).astype(np.float32)
  y_data = x_data*1.0+3.0
```
- 创建TensorFlow结构

```
Weights = tf.Variable(tf.random_uniform([1], -1.0, 1.0)) # 创建变量Weight是，范围是 -1.0~1.0
  biases = tf.Variable(tf.zeros([1]))                      # 创建偏置，初始值为0
  y = Weights*x_data+biases                                # 定义方程
  loss = tf.reduce_mean(tf.square(y-y_data))               # 定义损失，为真实值减去我们每一步计算的值
  optimizer = tf.train.GradientDescentOptimizer(0.5)       # 0.5 是学习率
  train = optimizer.minimize(loss)                         # 使用梯度下降优化
  init = tf.initialize_all_variables()                     # 初始化所有变量
```
- 定义`Session`

```
sess = tf.Session()
  sess.run(init)
```
- 输出结果

```
for i in range(201):
  sess.run(train)
  if i%20 == 0:
      print i,sess.run(Weights),sess.run(biases)
```

结果为：

```
0 [ 1.60895896] [ 3.67376709]
20 [ 1.04673827] [ 2.97489643]
40 [ 1.011392] [ 2.99388123]
60 [ 1.00277638] [ 2.99850869]
80 [ 1.00067675] [ 2.99963641]
100 [ 1.00016499] [ 2.99991131]
120 [ 1.00004005] [ 2.99997854]
140 [ 1.00000978] [ 2.99999475]
160 [ 1.0000025] [ 2.99999857]
180 [ 1.00000119] [ 2.99999928]
200 [ 1.00000119] [ 2.99999928]
```


### [](https://github.com/lawlite19/MachineLearning_TensorFlow#3session%E4%BC%9A%E8%AF%9D%E6%8E%A7%E5%88%B6)3、Session会话控制
- 运行 `session.run()` 可以获得你要得知的运算结果, 或者是你所要运算的部分
- 定义常量矩阵：`tf.constant([[3,3]])`
- 矩阵乘法 ：`tf.matmul(matrix1,matrix2)`
- 运行Session的两种方法：
- 手动关闭

```
sess = tf.Session()
  print sess.run(product)
  sess.close()
```
- 使用`with`，执行完会自动关闭

```
with tf.Session() as sess:
  print sess.run(product)
```


### [](https://github.com/lawlite19/MachineLearning_TensorFlow#4variable%E5%8F%98%E9%87%8F)4、`Variable`变量
- 定义变量：`tf.Variable()`
- 初始化所有变量：`init = tf.initialize_all_variables()`
- 需要再在 sess 里, `sess.run(init)` , 激活变量
- 输出时，一定要把 sess 的指针指向变量再进行 `print` 才能得到想要的结果

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#5placeholder%E4%BC%A0%E5%85%A5%E5%80%BC)5、`Placeholder`传入值
- 首先定义`Placeholder`，然后在`Session.run()`的时候输入值
- `placeholder` 与 `feed_dict={}` 是绑定在一起出现的

```
input1 = tf.placeholder(tf.float32) #在 Tensorflow 中需要定义 placeholder 的 type ，一般为 float32 形式
input2 = tf.placeholder(tf.float32)

output = tf.mul(input1,input2)  # 乘法运算

with tf.Session() as sess:
    print sess.run(output,feed_dict={input1:7.,input2:2.}) # placeholder 与 feed_dict={} 是绑定在一起出现的
```

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E4%B8%89%E5%AE%9A%E4%B9%89%E4%B8%80%E4%B8%AA%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)三、定义一个神经网络

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E6%B7%BB%E5%8A%A0%E5%B1%82%E5%87%BD%E6%95%B0add_layer)1、添加层函数`add_layer()`

```
'''参数：输入数据，前一层size，当前层size，激活函数'''
def add_layer(inputs,in_size,out_size,activation_function=None):
    Weights = tf.Variable(tf.random_normal([in_size,out_size]))  #随机初始化权重
    biases = tf.Variable(tf.zeros([1,out_size]) + 0.1)  # 初始化偏置，+0.1
    Ws_plus_b = tf.matmul(inputs,Weights) + biases      # 未使用激活函数的值
    if activation_function is None:
        outputs = Ws_plus_b
    else:
        outputs = activation_function(Ws_plus_b)   # 使用激活函数激活
    return outputs
```

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E6%9E%84%E5%BB%BA%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)2、构建神经网络
- 定义二次函数

```
x_data = np.linspace(-1,1,300,dtype=np.float32)[:,np.newaxis]
noise = np.random.normal(0,0.05,x_data.shape).astype(np.float32)
y_data = np.square(x_data)-0.5+noise
```
- 定义`Placeholder`,用于后期输入数据

```
xs = tf.placeholder(tf.float32,[None,1]) # None代表无论输入有多少都可以,只有一个特征，所以这里是1
ys = tf.placeholder(tf.float32,[None,1])
```
- 定义神经层`layer`

```
layer1 = add_layer(xs, 1, 10, activation_function=tf.nn.relu) # 第一层，输入层为1，隐含层为10个神经元，Tensorflow 自带的激励函数tf.nn.relu
```
- 定义输出层

```
prediction = add_layer(layer1, 10, 1) # 利用上一层作为输入
```
- 计算`loss`损失

```
loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1])) # 对二者差的平方求和再取平均
```
- 梯度下降最小化损失

```
train = tf.train.GradientDescentOptimizer(0.1).minimize(loss)
```
- 初始化所有变量

```
init = tf.initialize_all_variables()
```
- 定义Session

```
sess = tf.Session()
sess.run(init)
```
- 输出

```
for i in range(1000):
    sess.run(train,feed_dict={xs:x_data,ys:y_data})
    if i%50==0:
        print sess.run(loss,feed_dict={xs:x_data,ys:y_data})
```

结果：

```
0.45402
0.0145364
0.00721318
0.0064215
0.00614493
0.00599307
0.00587578
0.00577039
0.00567172
0.00558008
0.00549546
0.00541595
0.00534059
0.00526139
0.00518873
0.00511403
0.00504063
0.0049613
0.0048874
0.004819
```

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#3%E5%8F%AF%E8%A7%86%E5%8C%96%E7%BB%93%E6%9E%9C)3、可视化结果
- 显示数据

```
fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(x_data,y_data)
    plt.ion()   # 绘画之后不暂停
    plt.show()
```

![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/example_01.png)
- 动态绘画

```
try:
            ax.lines.remove(lines[0])   # 每次绘画需要移除上次绘画的结果，放在try catch里因为第一次执行没有，所以直接pass
        except Exception:
            pass
        prediction_value = sess.run(prediction, feed_dict={xs: x_data})
        # plot the prediction
        lines = ax.plot(x_data, prediction_value, 'r-', lw=3)  # 绘画
        plt.pause(0.1)  # 停0.1s
```

![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/example_02.gif)

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E5%9B%9Btensorflow%E5%8F%AF%E8%A7%86%E5%8C%96)四、TensorFlow可视化

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1tensorflow%E7%9A%84%E5%8F%AF%E8%A7%86%E5%8C%96%E5%B7%A5%E5%85%B7tensorboard%E5%8F%AF%E8%A7%86%E5%8C%96%E7%A5%9E%E7%BB%8F%E7%BD%91%E8%B7%AF%E9%A2%9D%E7%BB%93%E6%9E%84)1、TensorFlow的可视化工具`tensorboard`，可视化神经网路额结构
- 输入`input`

```
with tf.name_scope('input'):
    xs = tf.placeholder(tf.float32,[None,1],name='x_in')  # 
    ys = tf.placeholder(tf.float32,[None,1],name='y_in')
```

![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensorboard_01.png)
- `layer`层

```
def add_layer(inputs,in_size,out_size,activation_function=None):
    with tf.name_scope('layer'):
        with tf.name_scope('Weights'):
            Weights = tf.Variable(tf.random_normal([in_size,out_size]),name='W')
        with tf.name_scope('biases'):
            biases = tf.Variable(tf.zeros([1,out_size]) + 0.1,name='b')
        with tf.name_scope('Ws_plus_b'):
            Ws_plus_b = tf.matmul(inputs,Weights) + biases
        if activation_function is None:                                       outputs = Ws_plus_b
        else:                                                            
            outputs = activation_function(Ws_plus_b)  
        return outputs
```

![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensorboard_02.png)
- `loss`和`train`

```
with tf.name_scope('loss'):
    loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1]))

with tf.name_scope('train'):
    train = tf.train.GradientDescentOptimizer(0.1).minimize(loss)
```

![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensorboard_03.png)
- 写入文件中

```
writer = tf.train.SummaryWriter("logs/", sess.graph)
```
- 浏览器中查看（chrome浏览器）
- 在终端输入：`tensorboard --logdir='logs/'`，它会给出访问地址
- 浏览器中查看即可。
- `tensorboard`命令在安装**python**目录的**bin**目录下，可以创建一个软连接


### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E5%8F%AF%E8%A7%86%E5%8C%96%E8%AE%AD%E7%BB%83%E8%BF%87%E7%A8%8B)2、可视化训练过程
- 
可视化Weights权重和biases偏置
- 每一层起个名字

```
layer_name = 'layer%s'%n_layer
```
- tf.histogram_summary(name,value)

```
def add_layer(inputs,in_size,out_size,n_layer,activation_function=None):
  layer_name = 'layer%s'%n_layer
  with tf.name_scope(layer_name):
      with tf.name_scope('Weights'):
          Weights = tf.Variable(tf.random_normal([in_size,out_size]),name='W')
          tf.histogram_summary(layer_name+'/weights', Weights)
      with tf.name_scope('biases'):
          biases = tf.Variable(tf.zeros([1,out_size]) + 0.1,name='b')
          tf.histogram_summary(layer_name+'/biases',biases)
      with tf.name_scope('Ws_plus_b'):
          Ws_plus_b = tf.matmul(inputs,Weights) + biases

      if activation_function is None:             
          outputs = Ws_plus_b 
      else:                                                         
          outputs = activation_function(Ws_plus_b)      
      tf.histogram_summary(layer_name+'/outputs',outputs)
      return outputs
```
- merge所有的summary

```
merged =tf.merge_all_summaries()
```
- 写入文件中

```
writer = tf.train.SummaryWriter("logs/", sess.graph)
```
- 训练1000次，每50步显示一次：

```
for i in range(1000):
  sess.run(train,feed_dict={xs:x_data,ys:y_data})
  if i%50==0:
      summary = sess.run(merged, feed_dict={xs: x_data, ys:y_data})
      writer.add_summary(summary, i)
```
- 同样适用`tensorboard`查看
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensorboard_04.png)

- 
可视化损失函数（代价函数）
- 添加：`tf.scalar_summary('loss',loss)`
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/tensorboard_05.png)


## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E4%BA%94%E6%89%8B%E5%86%99%E6%95%B0%E5%AD%97%E8%AF%86%E5%88%AB_1)五、手写数字识别_1

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E8%AF%B4%E6%98%8E)1、说明
- [全部代码](https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_01/mnist.py)：`https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_02/mnist.py`
- 自己的数据集，没有使用tensorflow中mnist数据集，
- 之前在机器学习中用Python实现过，地址：`https://github.com/lawlite19/MachineLearning_Python`,这里使用`tensorflow`实现
- 神经网络只有两层

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E4%BB%A3%E7%A0%81%E5%AE%9E%E7%8E%B0)2、代码实现
- 添加一层

```
'''添加一层神经网络'''
def add_layer(inputs,in_size,out_size,activation_function=None):
    Weights = tf.Variable(tf.random_normal([in_size,out_size]))    # 权重，in*out 
    biases = tf.Variable(tf.zeros([1,out_size]) + 0.1)  
    Ws_plus_b = tf.matmul(inputs,Weights) + biases   # 计算权重和偏置之后的值                         
    if activation_function is None:                                     
        outputs = Ws_plus_b                                               
    else:                                                         
        outputs = activation_function(Ws_plus_b)    # 调用激励函数运算    
    return outputs
```
- 运行函数

```
'''运行函数'''
def NeuralNetwork():
    data_digits = spio.loadmat('data_digits.mat')
    X = data_digits['X']
    y = data_digits['y']
    m,n = X.shape
    class_y = np.zeros((m,10))      # y是0,1,2,3...9,需要映射0/1形式
    for i in range(10):
        class_y[:,i] = np.float32(y==i).reshape(1,-1) 

    xs = tf.placeholder(tf.float32, shape=[None,400])  # 像素是20x20=400，所以有400个feature
    ys = tf.placeholder(tf.float32, shape=[None,10])   # 输出有10个

    prediction = add_layer(xs, 400, 10, activation_function=tf.nn.softmax) # 两层神经网络，400x10
    #prediction = add_layer(layer1, 25, 10, activation_function=tf.nn.softmax)

    #loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1]))
    loss = tf.reduce_mean(-tf.reduce_sum(ys*tf.log(prediction),reduction_indices=[1]))  # 定义损失函数（代价函数），
    train = tf.train.GradientDescentOptimizer(learning_rate=0.5).minimize(loss)     # 使用梯度下降最小化损失
    init = tf.initialize_all_variables()   # 初始化所有变量

    sess = tf.Session()  # 创建Session
    sess.run(init)

    for i in range(4000): # 迭代训练4000次
        sess.run(train, feed_dict={xs:X,ys:class_y})  # 训练train，填入数据
        if i%50==0:  # 每50次输出当前的准确度
            print(compute_accuracy(xs,ys,X,class_y,sess,prediction))
```
- 计算准确度

```
'''计算预测准确度'''  
def compute_accuracy(xs,ys,X,y,sess,prediction):
    y_pre = sess.run(prediction,feed_dict={xs:X}) 
    correct_prediction = tf.equal(tf.argmax(y_pre,1),tf.argmax(y,1))  #tf.argmax 给出某个tensor对象在某一维上的其数据最大值所在的索引值,即为对应的数字，tf.equal 来检测我们的预测是否真实标签匹配
    accuracy = tf.reduce_mean(tf.cast(correct_prediction,tf.float32)) # 平均值即为准确度
    result = sess.run(accuracy,feed_dict={xs:X,ys:y})
    return result
```
- 输出每一次预测的结果准确度
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/Mnist_01.png)

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E5%85%AD%E6%89%8B%E5%86%99%E6%95%B0%E5%AD%97%E8%AF%86%E5%88%AB_2)六、手写数字识别_2

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E8%AF%B4%E6%98%8E-1)1、说明
- [全部代码](https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_02/mnist.py)：`https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_02/mnist.py`
- 采用TensorFlow中的mnist数据集（可以取网站下载它的数据集，[http://yann.lecun.com/exdb/mnist/）](http://yann.lecun.com/exdb/mnist/%EF%BC%89)
- 实现代码与上面类似，它有专门的测试集

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E4%BB%A3%E7%A0%81)2、代码
- 随机梯度下降`SGD`,每次选出`100`个数据进行训练

```
for i in range(2000):
        batch_xs, batch_ys = minist.train.next_batch(100)
        sess.run(train_step,feed_dict={xs:batch_xs,ys:batch_ys})
        if i%50==0:
            print(compute_accuracy(xs,ys,minist.test.images, minist.test.labels,sess,prediction))
```
- 输出每一次预测的结果准确度
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/Mnist_02.png)

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E4%B8%83%E6%89%8B%E5%86%99%E6%95%B0%E5%AD%97%E8%AF%86%E5%88%AB_3_cnn%E5%8D%B7%E7%A7%AF%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)七、手写数字识别_3_CNN卷积神经网络

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E8%AF%B4%E6%98%8E-2)1、说明
- 关于**卷积神经网络CNN**可以查看[我的博客](http://blog.csdn.net/u013082989/article/details/53673602)：[http://blog.csdn.net/u013082989/article/details/53673602](http://blog.csdn.net/u013082989/article/details/53673602)- 或者[github](https://github.com/lawlite19/DeepLearning_Python)：[https://github.com/lawlite19/DeepLearning_Python](https://github.com/lawlite19/DeepLearning_Python)

- [全部代码](https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_03_CNN/mnist_cnn.py)：`https://github.com/lawlite19/MachineLearning_TensorFlow/blob/master/Mnist_03_CNN/mnist_cnn.py`
- 采用TensorFlow中的mnist数据集（可以取网站下载它的数据集，[http://yann.lecun.com/exdb/mnist/）](http://yann.lecun.com/exdb/mnist/%EF%BC%89)

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E4%BB%A3%E7%A0%81%E5%AE%9E%E7%8E%B0-1)2、代码实现
- 权重和偏置初始化函数
- 权重使用的`truncated_normal`进行初始化,`stddev`标准差定义为0.1
- 偏置初始化为常量0.1


```
'''权重初始化函数'''
def weight_variable(shape):
    inital = tf.truncated_normal(shape, stddev=0.1)  # 使用truncated_normal进行初始化
    return tf.Variable(inital)

'''偏置初始化函数'''
def bias_variable(shape):
    inital = tf.constant(0.1,shape=shape)  # 偏置定义为常量
    return tf.Variable(inital)
```
- 卷积函数
- `strides[0]`和`strides[3]`的两个1是默认值，中间两个1代表padding时在x方向运动1步，y方向运动1步
- `padding='SAME'`代表经过卷积之后的输出图像和原图像大小一样


```
'''卷积函数'''
def conv2d(x,W):#x是图片的所有参数，W是此卷积层的权重
    return tf.nn.conv2d(x,W,strides=[1,1,1,1],padding='SAME')#strides[0]和strides[3]的两个1是默认值，中间两个1代表padding时在x方向运动1步，y方向运动1步
```
- 池化函数
- `ksize`指定池化核函数的大小
- 根据池化核函数的大小定义`strides`的大小


```
'''池化函数'''
def max_pool_2x2(x):
    return tf.nn.max_pool(x,ksize=[1,2,2,1],
                          strides=[1,2,2,1],                          padding='SAME')#池化的核函数大小为2x2，因此ksize=[1,2,2,1]，步长为2，因此strides=[1,2,2,1]
```
- 加载`mnist`数据和定义`placeholder`- 输入数据`x_image`最后一个`1`代表`channel`的数量,若是`RGB`3个颜色通道就定义为3
- `keep_prob` 用于**dropout**防止过拟合


```
mnist = input_data.read_data_sets('MNIST_data', one_hot=True)  # 下载数据

    xs = tf.placeholder(tf.float32,[None,784])  # 输入图片的大小，28x28=784
    ys = tf.placeholder(tf.float32,[None,10])   # 输出0-9共10个数字
    keep_prob = tf.placeholder(tf.float32)      # 用于接收dropout操作的值，dropout为了防止过拟合
    x_image = tf.reshape(xs,[-1,28,28,1])       #-1代表先不考虑输入的图片例子多少这个维度，后面的1是channel的数量，因为我们输入的图片是黑白的，因此channel是1，例如如果是RGB图像，那么channel就是3
```
- 第一层卷积和池化
- 使用**ReLu**激活函数


```
'''第一层卷积，池化'''
    W_conv1 = weight_variable([5,5,1,32])  # 卷积核定义为5x5,1是输入的通道数目，32是输出的通道数目
    b_conv1 = bias_variable([32])          # 每个输出通道对应一个偏置
    h_conv1 = tf.nn.relu(conv2d(x_image,W_conv1)+b_conv1) # 卷积运算，并使用ReLu激活函数激活
    h_pool1 = max_pool_2x2(h_conv1)        # pooling操作
```
- 第二层卷积和池化

```
'''第二层卷积，池化'''
    W_conv2 = weight_variable([5,5,32,64]) # 卷积核还是5x5,32个输入通道，64个输出通道
    b_conv2 = bias_variable([64])          # 与输出通道一致
    h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2)+b_conv2)
    h_pool2 = max_pool_2x2(h_conv2)
```
- 全连接第一层

```
'''全连接层'''
    h_pool2_flat = tf.reshape(h_pool2, [-1,7*7*64])   # 将最后操作的数据展开
    W_fc1 = weight_variable([7*7*64,1024])            # 下面就是定义一般神经网络的操作了，继续扩大为1024
    b_fc1 = bias_variable([1024])                     # 对应的偏置
    h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat,W_fc1)+b_fc1)  # 运算、激活（这里不是卷积运算了，就是对应相乘）
```
- `dropout`防止过拟合

```
'''dropout'''
    h_fc1_drop = tf.nn.dropout(h_fc1,keep_prob)       # dropout操作
```
- 最后一层全连接预测,使用梯度下降优化**交叉熵损失函数**- 使用**softmax**分类器分类


```
'''最后一层全连接'''
    W_fc2 = weight_variable([1024,10])                # 最后一层权重初始化
    b_fc2 = bias_variable([10])                       # 对应偏置

    prediction = tf.nn.softmax(tf.matmul(h_fc1_drop,W_fc2)+b_fc2)  # 使用softmax分类器
    cross_entropy = tf.reduce_mean(-tf.reduce_sum(ys*tf.log(prediction),reduction_indices=[1]))  # 交叉熵损失函数来定义cost function
    train_step = tf.train.AdamOptimizer(1e-3).minimize(cross_entropy)  # 调用梯度下降
```
- 定义Session，使用`SGD`训练

```
'''下面就是tf的一般操作，定义Session，初始化所有变量，placeholder传入值训练'''
    sess = tf.Session()
    sess.run(tf.initialize_all_variables())

    for i in range(1000):
        batch_xs, batch_ys = mnist.train.next_batch(100)  # 使用SGD，每次选取100个数据训练
        sess.run(train_step, feed_dict={xs: batch_xs, ys: batch_ys, keep_prob: 0.5})  # dropout值定义为0.5
        if i % 50 == 0:
            print compute_accuracy(xs,ys,mnist.test.images, mnist.test.labels,keep_prob,sess,prediction)  # 每50次输出一下准确度
```
- 计算准确度函数
- 和上面的两个计算准确度的函数一致，就是多了个**dropout**的参数`keep_prob`


```
'''计算准确度函数'''
def compute_accuracy(xs,ys,X,y,keep_prob,sess,prediction):
    y_pre = sess.run(prediction,feed_dict={xs:X,keep_prob:1.0})       # 预测，这里的keep_prob是dropout时用的，防止过拟合
    correct_prediction = tf.equal(tf.argmax(y_pre,1),tf.argmax(y,1))  #tf.argmax 给出某个tensor对象在某一维上的其数据最大值所在的索引值,即为对应的数字，tf.equal 来检测我们的预测是否真实标签匹配
    accuracy = tf.reduce_mean(tf.cast(correct_prediction,tf.float32)) # 平均值即为准确度
    result = sess.run(accuracy,feed_dict={xs:X,ys:y,keep_prob:1.0})
    return result
```

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#3%E8%BF%90%E8%A1%8C%E7%BB%93%E6%9E%9C)3、运行结果
- 测试集上准确度
![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/cnn_mnist_02.png)

- 使用`top`命令查看占用的CPU和内存，还是很消耗CPU和内存的，所以上面只输出了四次我就终止了![enter description here](https://github.com/lawlite19/MachineLearning_TensorFlow/raw/master/images/cnn_mnist_01.png)

- 由于我在虚拟机里运行的`TensorFlow`程序，分配了`5G`的内存，若是内存不够会报一个错误。

## [](https://github.com/lawlite19/MachineLearning_TensorFlow#%E5%85%AB%E4%BF%9D%E5%AD%98%E5%92%8C%E6%8F%90%E5%8F%96%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)八、保存和提取神经网络

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#1%E4%BF%9D%E5%AD%98)1、保存
- 定义要保存的数据

```
W = tf.Variable(initial_value=[[1,2,3],[3,4,5]], 
               name='weights', dtype=tf.float32)   # 注意需要指定name和dtype
b = tf.Variable(initial_value=[1,2,3], 
               name='biases', dtype=tf.float32)
init = tf.initialize_all_variables()
```
- 保存

```
saver = tf.train.Saver()
with tf.Session() as sess:
    sess.run(init)
    save_path = saver.save(sess, 'my_network/save_net.ckpt') # 保存目录，注意要在当前项目下建立my_network的目录
    print ('保存到 :',save_path)
```

### [](https://github.com/lawlite19/MachineLearning_TensorFlow#2%E6%8F%90%E5%8F%96)2、提取
- 定义数据

```
W = tf.Variable(np.arange(6).reshape((2,3)), 
               name='weights', dtype=tf.float32) # 注意与之前保存的一致
b = tf.Variable(np.arange((3)), 
               name='biases', dtype=tf.float32)
```
- `restore`提取

```
saver = tf.train.Saver() 
with tf.Session() as sess:
    saver.restore(sess,'my_network/save_net.ckpt')  
    print('weights:',sess.run(W))  # 输出一下结果
    print('biases:',sess.run(b))
```



