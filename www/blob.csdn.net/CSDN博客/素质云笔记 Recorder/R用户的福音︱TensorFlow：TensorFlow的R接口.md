# R用户的福音︱TensorFlow：TensorFlow的R接口 - 素质云笔记/Recorder... - CSDN博客





2016年10月05日 11:01:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8676标签：[TensorFlow																[R																[R语言																[接口																[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)
个人分类：[图像︱caffe](https://blog.csdn.net/sinat_26917383/article/category/6666555)










————————————————————————————————————————————————————————————




**Matt︱R语言调用深度学习架构系列引文**




[R语言︱H2o深度学习的一些R语言实践——H2o包](http://blog.csdn.net/sinat_26917383/article/details/51219025)




# [R用户的福音︱TensorFlow：TensorFlow的R接口](http://blog.csdn.net/sinat_26917383/article/details/52737913)



# [](http://blog.csdn.net/sinat_26917383/article/details/52737913)

# [mxnet：结合R与GPU加速深度学习](http://blog.csdn.net/sinat_26917383/article/details/52737901)





# [](http://blog.csdn.net/sinat_26917383/article/details/52737901)

# 碎片︱R语言与深度学习




[sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](http://blog.csdn.net/sinat_26917383/article/details/52749138)


—————————————————————————————————————










# TensorFlow




[TensorFlow™](https://tensorflow.org/) is an open source software library for numerical computation using data flow graphs. Nodes in the graph represent mathematical operations, while the graph edges represent the multidimensional data arrays (tensors) communicated between them. The flexible architecture allows you to deploy computation to one or more CPUs or GPUs in a desktop, server, or mobile device with a single API. TensorFlow was originally developed by researchers and engineers working on the Google Brain Team within Google’s Machine Intelligence research organization for the purposes of conducting machine learning and deep neural networks research, but the system is general enough to be applicable in a wide variety of other domains as well.






## Using TensorFlow with R

The [TensorFlow API](https://www.tensorflow.org/api_docs/python/index.html) is composed of a set of Python modules that enable constructing and executing TensorFlow graphs. The tensorflow package provides access to the complete TensorFlow API from within R. Here’s a simple example of making up some data in two dimensions and then fitting a line to it:

```
library(tensorflow)

# Create 100 phony x, y data points, y = x * 0.1 + 0.3
x_data <- runif(100, min=0, max=1)
y_data <- x_data * 0.1 + 0.3

# Try to find values for W and b that compute y_data = W * x_data + b
# (We know that W should be 0.1 and b 0.3, but TensorFlow will
# figure that out for us.)
W <- tf$Variable(tf$random_uniform(shape(1L), -1.0, 1.0))
b <- tf$Variable(tf$zeros(shape(1L)))
y <- W * x_data + b

# Minimize the mean squared errors.
loss <- tf$reduce_mean((y - y_data) ^ 2)
optimizer <- tf$train$GradientDescentOptimizer(0.5)
train <- optimizer$minimize(loss)

# Launch the graph and initialize the variables.
sess = tf$Session()
sess$run(tf$initialize_all_variables())

# Fit the line (Learns best fit is W: 0.1, b: 0.3)
for (step in 1:201) {
  sess$run(train)
  if (step %% 20 == 0)
    cat(step, "-", sess$run(W), sess$run(b), "\n")
}
```

The first part of this code builds the data flow graph. TensorFlow does not actually run any computation until the session is created and the `run`function is called.



## MNIST Tutorials

To whet your appetite further, we suggest you check out what a classical machine learning problem looks like in TensorFlow. In the land of neural networks the most “classic” classical problem is the MNIST handwritten digit classification. We offer two introductions here, one for machine learning newbies, and one for pros. If you’ve already trained dozens of MNIST models in other software packages, please take the red pill. If you’ve never even heard of MNIST, definitely take the blue pill. If you’re somewhere in between, we suggest skimming blue, then red.


![MNIST for machine learning beginners tutorial](https://rstudio.github.io/tensorflow/images/blue_pill.png)![Deep MNIST for machine learning experts tutorial](https://rstudio.github.io/tensorflow/images/red_pill.png)


Images licensed CC BY-SA 4.0; original by W. Carter

If you’re already sure you want to learn and install TensorFlow you can skip these and charge ahead. Don’t worry, you’ll still get to see MNIST – we’ll also use MNIST as an example in our technical tutorial where we elaborate on TensorFlow features.



## Download and Setup


### Installing TensorFlow

You can install the main TensorFlow distribution from here:

[https://www.tensorflow.org/get_started/os_setup.html#download-and-setup](https://www.tensorflow.org/get_started/os_setup.html#download-and-setup)

NOTE: You should NOT install TensorFlow with Anaconda as there [are issues with](https://github.com/ContinuumIO/anaconda-issues/issues/498) the way Anaconda builds the python shared library that prevent dynamic linking from R.

If you install TensorFlow within a Virtualenv environment you’ll need to be sure to use that same environment when installing the tensorflow R package (see below for details).



### Installing the R Package

If you installed TensorFlow via pip with your system default version of python then you can install the tensorflow R package as follows:
`devtools::install_github("rstudio/tensorflow")`
If you are using a different version of python for TensorFlow, you should set the `TENSORFLOW_PYTHON` environment variable to the full path of the python binary before installing, for example:

```
Sys.setenv(TENSORFLOW_PYTHON="/usr/local/bin/python")
devtools::install_github("rstudio/tensorflow")
```

If you only need to customize the version of python used (for example specifing python 3 on an Ubuntu system), you can set the`TENSORFLOW_PYTHON_VERSION` environment variable before installation:

```
Sys.setenv(TENSORFLOW_PYTHON_VERSION = 3)
devtools::install_github("rstudio/tensorflow")
```



### Verifying Installation

You can verify that your installation is working correctly by running this script:

```
library(tensorflow)
sess = tf$Session()
hello <- tf$constant('Hello, TensorFlow!')
sess$run(hello)
```



### RStudio IDE

The tensorflow package provides code completion and inline help for the TensorFlow API when running within the RStudio IDE. In order to take advantage of these features you should also install the current [Preview Release](https://www.rstudio.com/products/rstudio/download/preview/) of RStudio.




## Recommended Next Steps

Once you’ve installed the base TensorFlow system and the tensorflow R package, you will likely want work though the series of tutorials that cover TensorFlow basics:
- [MNIST For ML Beginners](https://rstudio.github.io/tensorflow/tutorial_mnist_beginners.html)
- [Deep MNIST for Experts](https://rstudio.github.io/tensorflow/tutorial_mnist_pros.html)
- [TensorFlow Mechanics 101](https://rstudio.github.io/tensorflow/tutorial_tensorflow_mechanics.html)

These articles cover the core concepts of TensorFlow in more depth as well describe the details of using the TensorFlow API from R:
- [TensorFlow API Basic Usage](https://rstudio.github.io/tensorflow/basic_usage.html)
- [Using the TensorFlow API from R](https://rstudio.github.io/tensorflow/using_tensorflow_api.html)

These articles provide more in depth treatments of various topics:
- [Variables: Creation, Initialization, Saving, and Loading](https://rstudio.github.io/tensorflow/howto_variables.html)
- [TensorFlow Mechanics 101](https://rstudio.github.io/tensorflow/tutorial_tensorflow_mechanics.html)
- [TensorBoard: Visualizing Learning](https://rstudio.github.io/tensorflow/howto_summaries_and_tensorboard.html)
- [TensorBoard: Graph Visualization](https://rstudio.github.io/tensorflow/howto_graph_viz.html)

Finally, to learn more about neural networks you might enjoy the [TensorFlow playground](http://playground.tensorflow.org/), which lets you tinker with a neural network in your browser.










网友尝鲜：[在Rstudio运用R实现Tensorflow的一般步骤](http://mp.weixin.qq.com/s?__biz=MzIzMTE4NDY1OA==&mid=2659758950&idx=1&sn=347cf37919a1321052895b9fc4bf98ae&chksm=f3d4131bc4a39a0d3dd2d4bba715909f189b24c73225b74858b8c273dd0269ee9d30a8aa1a22&mpshare=1&scene=1&srcid=0315MlXveLHybzyWhZ3VvUaY#rd)


`    按照TensorFlow for R官网说明，运用R实现Tensorflow主要包含4步(如下)，其中关键步骤在于安装好Tensorflow以及tensorflow R 包与python的连接。`- 
从TensorFlow官网安装TensorFlow.

- 
安装tensorflow R 包

`devtools::install_github("rstudio/tensorflow")`- 
载入TensorFlow


```
Sys.setenv(TENSORFLOW_PYTHON="/usr/local/bin/python")
**library**(tensorflow)
```
- 
验证安装


```
**library**(tensorflow)
sess = tf$Session()
hello <- tf$constant('Hello, TensorFlow!')
sess$run(hello)
```](https://so.csdn.net/so/search/s.do?q=接口&t=blog)](https://so.csdn.net/so/search/s.do?q=R语言&t=blog)](https://so.csdn.net/so/search/s.do?q=R&t=blog)](https://so.csdn.net/so/search/s.do?q=TensorFlow&t=blog)




