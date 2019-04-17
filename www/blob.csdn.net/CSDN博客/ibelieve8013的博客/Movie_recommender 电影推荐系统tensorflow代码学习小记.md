# Movie_recommender 电影推荐系统tensorflow代码学习小记 - ibelieve8013的博客 - CSDN博客





2019年02月19日 10:33:42[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：84










TensorFlow一向用得不多，只是稍微了解，推荐系统的项目也没有正经做过，现在就拿这个项目，好好入门一下推荐系统。

1.map 函数：

![](https://img-blog.csdnimg.cn/20190219103325476.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

另外，map还可以这么用：

如要改变User数据中性别和年龄

    gender_map = {'F':0, 'M':1}

users['Gender'] = users['Gender'].map(gender_map)



2. enumerate() 函数用于将一个可遍历的数据对象(如列表、元组或字符串)组合为一个索引序列，同时列出数据和数据下标，一般用在 for 循环当中。

![](https://img-blog.csdnimg.cn/20190219103325598.png)

说明：这个函数用在将电影类型，电影title转为数字字典的时候很有用



3.pickle.dump()

Python中的Pickle模块实现了基本的数据序列与反序列化。

3.1 dump()方法

pickle.dump(obj, file, [,protocol])

注释：序列化对象，将对象obj保存到文件file中去。参数protocol是序列化模式，默认是0（ASCII协议，表示以文本的形式进行序列化），protocol的值还可以是1和2（1和2表示以二进制的形式进行序列化。其中，1是老式的二进制协议；2是新二进制协议）。file表示保存到的类文件对象，file必须有write()接口，file可以是一个以'w'打开的文件或者是一个StringIO对象，也可以是任何可以实现write()接口的对象。







3.2 load()方法

pickle.load(file)

注释：反序列化对象，将文件中的数据解析为一个python对象。file中有read()接口和readline()接口



4. tf.nn.embedding_lookup(tensor, id)

tensor就是输入张量，id就是张量对应的索引

用法主要是选取一个张量里面索引对应的元素。

5. tf.variable_scope和tf.name_scope的用法

tf.variable_scope可以让变量有相同的命名，包括tf.get_variable得到的变量，还有tf.Variable的变量



tf.name_scope可以让变量有相同的命名，只是限于tf.Variable的变量。



6. tf.reduce_sum应该理解为压缩求和，用于降维

# 'x' is [[1, 1, 1]

#         [1, 1, 1]]

#求和

tf.reduce_sum(x) ==> 6



#按列求和

tf.reduce_sum(x, 0) ==> [2, 2, 2]



#按行求和

tf.reduce_sum(x, 1) ==> [3, 3]



7. tf.expand_dims(input, axis=None, name=None, dim=None)

为张量+1维。官网的例子：’t’ is a tensor of shape [2] 维度为2

shape(expand_dims(t, 0)) ==> [1, 2] ，变成了1行2列

shape(expand_dims(t, 1)) ==> [2, 1] ，变2行1列

shape(expand_dims(t, -1)) ==> [2, 1] ，在最后一个维度增加一维



多的总结在这里[https://blog.csdn.net/u014595019/article/details/52805444](https://blog.csdn.net/u014595019/article/details/52805444)



8. tf.nn.dropout

tf.nn.dropout是TensorFlow里面为了防止或减轻过拟合而使用的函数，它**一般用在全连接层**。

tf.nn.dropout(x, keep_prob, noise_shape=None, seed=None,name=None) 



上面方法中常用的是前两个参数：



第一个参数x：指输入



第二个参数keep_prob: 设置神经元被选中的概率,在初始化时keep_prob是一个占位符,  keep_prob = tf.placeholder(tf.float32) 。tensorflow在run时设置keep_prob具体的值，例如keep_prob: 0.5

第五个参数name：指定该操作的名字



9. tf.layers.dense() 和tf.contrib.layers.fully_connected()

效果都是添加一个全连接层，后面的这个函数调用的前面的函数。而且后面的这个函数在dense()基础上添加了一些新功能，如正则化和激活函数。（默认都是relu）



10. optimizer = tf.train.AdamOptimizer(lr)

   gradients = optimizer.compute_gradients(loss)  #cost

作用：计算损失函数对于各个变量列表中各个变量的梯度。`minimize()` 的第一部分就是计算梯度的这个函数。返回的是一个`(gradient, variable)`对的列表当然对于某个给定的变量要是没有梯度的话，就是`None` 。 

   train_op = optimizer.apply_gradients(gradients, global_step=global_step)

Apply gradients to variables.This is the second part of minimize(). It returns an Operation that applies gradients. 



训练网络的一些函数：

11. tf.summary.histogram()

在训练神经网络时，当需要查看一个张量在训练过程中值的分布情况时，可通过tf.summary.histogram()将其分布情况以直方图的形式在TensorBoard直方图[仪表板](https://www.baidu.com/s?wd=%E4%BB%AA%E8%A1%A8%E6%9D%BF&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)上显示．

12. tf.summary.scalar()

用来显示标量信息，其格式为：

tf.summary.scalar(tags, values, collections=None, name=None)

一般在画loss,accuary时会用到这个函数。



13.获得文件当前路径os.path.abspath()，获得文件的父目录os.path.dirname()

14. Tqdm 是一个快速，可扩展的Python进度条，可以在 Python 长循环中添加一个进度提示信息，用户只需要封装任意的迭代器 tqdm(iterator)。



