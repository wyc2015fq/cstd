# 【Tensorflow slim】slim losses包 - zj360202的专栏 - CSDN博客





2017年11月21日 17:27:30[zj360202](https://me.csdn.net/zj360202)阅读数：3467







损失函数定义了我们想要最小化的数量。 对于分类问题，这通常是跨分类的真实分布和预测概率分布之间的交叉熵。 对于回归问题，这通常是预测值和真值之间的平方和差异。


某些模型（如多任务学习模型）需要同时使用多个损失函数。 换句话说，最终被最小化的损失函数是各种其他损失函数的总和。 例如，考虑预测图像中的场景类型以及每个像素的相机深度的模型。 这个模型的损失函数将是分类损失和深度预测损失的总和。


TF-Slim提供了一个易于使用的机制，通过损失模块定义和跟踪损失功能。 考虑一下我们想要训练VGG网络的简单情况：



```python
import tensorflow as tf
vgg = tf.contrib.slim.nets.vgg

# Load the images and labels.
images, labels = ...

# Create the model.
predictions, _ = vgg.vgg_16(images)

# Define the loss functions and get the total loss.
loss = slim.losses.softmax_cross_entropy(predictions, labels)
```

在这个例子中，我们首先创建模型（使用TF-Slim的VGG实现），并添加标准分类损失。 现在，让我们假设有一个多任务模型，产生多个输出的情况：



```python
# Load the images and labels.
images, scene_labels, depth_labels = ...

# Create the model.
scene_predictions, depth_predictions = CreateMultiTaskModel(images)

# Define the loss functions and get the total loss.
classification_loss = slim.losses.softmax_cross_entropy(scene_predictions, scene_labels)
sum_of_squares_loss = slim.losses.sum_of_squares(depth_predictions, depth_labels)

# The following two lines have the same effect:
total_loss = classification_loss + sum_of_squares_loss
total_loss = slim.losses.get_total_loss(add_regularization_losses=False)
```

在这个例子中，我们有两个损失，我们通过调用slim.losses.softmax_cross_entropy和slim.losses.sum_of_squares来添加。 我们可以通过将它们相加（total_loss）或调用slim.losses.get_total_loss（）来获得全部损失。 这是如何工作的？ 当您通过TF-Slim创建loss function时，TF-Slim将损失添加到损失函数中特殊的TensorFlow集合中。 这使您可以手动管理全部损失，或允许TF-Slim为您管理它们。


如果你想让TF-Slim管理你的损失，通过一个自定义的损失函数呢？ loss_ops.py也有一个功能，把这个损失添加到TF-Slims集合中。 例如：




```python
# Load the images and labels.
images, scene_labels, depth_labels, pose_labels = ...

# Create the model.
scene_predictions, depth_predictions, pose_predictions = CreateMultiTaskModel(images)

# Define the loss functions and get the total loss.
classification_loss = slim.losses.softmax_cross_entropy(scene_predictions, scene_labels)
sum_of_squares_loss = slim.losses.sum_of_squares(depth_predictions, depth_labels)
pose_loss = MyCustomLossFunction(pose_predictions, pose_labels)
slim.losses.add_loss(pose_loss) # Letting TF-Slim know about the additional loss.

# The following two ways to compute the total loss are equivalent:
regularization_loss = tf.add_n(slim.losses.get_regularization_losses())
total_loss1 = classification_loss + sum_of_squares_loss + pose_loss + regularization_loss

# (Regularization Loss is included in the total loss by default).
total_loss2 = slim.losses.get_total_loss()
```
在这个例子中，我们可以再次手动产生总损失函数，或者让TF-Slim知道额外的损失，让TF-Slim处理损失。







