# 头部姿态估计——multi-loss

2018年10月06日 11:13:13 [Peanut_范](https://me.csdn.net/u013841196) 阅读数：1001



 版权声明：	https://blog.csdn.net/u013841196/article/details/82949739

- 《Fine-Grained Head Pose Estimation Without Keypoints》
  2018，Nataniel Ruiz Eunji Chong James M. Rehg. multi-loss
  代码链接：<https://github.com/natanielruiz/deep-head-pose>

**1.引言：**
本文提出了一种简洁和鲁棒的方式来确定姿态，通过训练一个multi-loss的卷积神经网络。
直接使用RGB结合分类和回归损失来预测Euler angles（yaw，pitch and roll）。

**2.网络结构：**
![在这里插入图片描述](https://img-blog.csdn.net/20181006110240267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
本文提出使用3个分离的losses，为每一个角度。每个loss由两部分组成：a binned pose classification and a regression component 组成。

**最后为每一个欧拉角的损失为：**
![在这里插入图片描述](https://img-blog.csdn.net/20181006110349883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实现细节：**
**1）** 对欧拉角（Yaw，Pitch，Roll）按角度区间进行分类，比如3度，那么Yaw：-90-+90，可以分成180/3= 60个类别，Pitch和Roll同Yaw角类似。这样就可以进行分类任务了。
**2）** 对分类的结果恢复成实际的角度，类别*3-90，在和实际的角度计算回归损失。
**3）** 最后将回归损失和分类损失进行合并来得到最后的损失，回归损失的前面增加了一个权重系数α。

**3.测试：**
**1）不同测试集的测试结果**
![在这里插入图片描述](https://img-blog.csdn.net/20181006110606768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2）不同网络结构设置α参数的结果比较**
![在这里插入图片描述](https://img-blog.csdn.net/20181006110648817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不同的网络结构需要自行去调节α进行训练。

**4.小结：**
使用分类和回归的方式进行约束，可以提升姿态估计的准确率。
但数据集构建比较麻烦。

**5.部分参考代码：**

```
伪代码：
# 类别数（yaw：60类）
idx_tensor1 = [idx for idx in xrange(60)]
idx_tensor1 = tf.convert_to_tensor(idx_tensor1,dtype=tf.float32)

# yaw角groundtruth获取
# continous labels
label_yaw_cont=ordinal_batch[:,0]
# binned labels				
label_yaw_bin=nominal_batch[:,0]

# 分类损失
# Cross entropy loss
loss_yaw_cl =tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits (labels=tf.cast(label_yaw_bin,tf.int64), logits=logits_nominal_yaw))

# 回归损失
# MSE loss
yaw_predict = tf.nn.softmax(logits_nominal_yaw)
logits_ordinal_yaw = tf.reduce_mean(yaw_predict*idx_tensor1,1)*3-90
loss_yaw_reg = tf.reduce_mean((logits_ordinal_yaw-label_yaw_cont)**2)

# 总损失	
#total loss
alpha=FLAGS.alpha
loss_yaw =tf.add_n([loss_yaw_cl,alpha * loss_yaw_reg])
12345678910111213141516171819202122232425
```

------

###### 注：博众家之所长，集群英之荟萃。