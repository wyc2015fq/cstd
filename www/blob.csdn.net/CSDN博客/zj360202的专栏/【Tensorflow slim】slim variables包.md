# 【Tensorflow slim】slim variables包 - zj360202的专栏 - CSDN博客





2017年11月21日 17:57:09[zj360202](https://me.csdn.net/zj360202)阅读数：1177







在本地tensorflow中创建变量需要预定义值或初始化机制（例如，从高斯随机采样）。 此外，如果需要在特定设备（如GPU）上创建变量，则必须明确规定。 为了减少变量创建所需的代码，TF-Slim在variables.py中提供了一组简单的包装函数，这些函数允许调用者轻松定义变量。


例如，要创建权重变量，使用截断的正态分布对其进行初始化，用l2_loss调整它，并将其放在CPU上，只需声明以下内容：



```python
weights = slim.variable('weights',
                             shape=[10, 10, 3 , 3],
                             initializer=tf.truncated_normal_initializer(stddev=0.1),
                             regularizer=slim.l2_regularizer(0.05),
                             device='/CPU:0')
```

请注意，在本地TensorFlow中，有两种类型的变量：常规变量和本地（瞬态）变量。绝大多数变量是常规变量：一旦创建，它们可以使用保存程序保存到磁盘。局部变量是只在会话期间存在的变量，并不保存到磁盘。


TF-Slim通过定义模型变量来进一步区分变量，模型变量是表示模型参数的变量。模型变量在学习期间被训练或微调，并在评估或推断期间从检查点加载。示例包括由slim.fully_connected或slim.conv2d图层创建的变量。非模型变量是在学习或评估期间使用的所有其他变量，但对于实际执行推理不是必需的。例如，global_step是学习和评估期间使用的变量，但实际上并不是模型的一部分。同样，移动平均值变量可能反映模型变量，但移动平均值本身不是模型变量。


模型变量和常规变量都可以通过TF-Slim轻松创建和检索：



```python
# Model Variables
weights = slim.model_variable('weights',
                              shape=[10, 10, 3 , 3],
                              initializer=tf.truncated_normal_initializer(stddev=0.1),
                              regularizer=slim.l2_regularizer(0.05),
                              device='/CPU:0')
model_variables = slim.get_model_variables()

# Regular variables
my_var = slim.variable('my_var',
                       shape=[20, 1],
                       initializer=tf.zeros_initializer())
regular_variables_and_model_variables = slim.get_variables()
```




这个怎么用？ 当您通过TF-Slim的图层或直接通过slim.model_variable函数创建模型变量时，TF-Slim将该变量添加到tf.GraphKeys.MODEL_VARIABLES集合中。 如果你有自己的自定义图层或变量创建例程，但仍然希望TF-Slim管理或知道你的模型变量呢？ TF-Slim提供了一个方便的功能来将模型变量添加到其集合中：

```python
my_model_variable = CreateViaCustomCode()

# Letting TF-Slim know about the additional variable.
slim.add_model_variable(my_model_variable)
```





