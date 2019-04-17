# Capsule官方代码开源之后，机器之心做了份核心代码解读 - 知乎
# 



**机器之心原创**

**作者：蒋思源**

> 前几天，Sara Sabour 开源了一份 Capsule 代码，该代码是论文 Dynamic Routing between Capsules 中所采用的实现。其实早在去年刚公布此论文，机器之心就曾详解解读过核心思想与基本代码，我们采用的代码也是各研究者尝试复现论文结果的模型。而最近 Sara 开放的代码是标准的官方实现，因此我们希望能解读部分核心代码，并探讨其与 naturomics 等人实现过程的差异。

Sara 实现地址：[https://github.com/Sarasra/models/tree/master/research/capsules](https://link.zhihu.com/?target=https%3A//github.com/Sarasra/models/tree/master/research/capsules)

我们主要根据 Sara 的代码解释了 CapsNet 的架构与实现方法，包括 Squash 非线性函数、动态路由更新方法、PrimaryCaps 层与 DigitCaps 的实现过程，还有最后的 Margin Loss 度量函数。我们希望入门读者先了解 Capsule 的概念与 CapsNet 的基本架构，以下我们提供了 Capsule 论文解读与基本概念。
- [先读懂 CapsNet 架构然后用 TensorFlow 实现，这应该是最详细的教程了](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650732855%26idx%3D1%26sn%3D87319e9390200f24dfd2faff4d7d364a%26chksm%3D871b3d49b06cb45fd8a68d003310b05562d9f8ff094ed08345f112e4450f7e66e6cf71c5b571%26scene%3D21%23wechat_redirect)
- [终于，Geoffrey Hinton 那篇备受关注的 Capsule 论文公开了](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650732472%26idx%3D1%26sn%3D259e5aa77b62078ffa40be9655da0802%26chksm%3D871b33c6b06cbad0748571c9cb30d15e9658c7509c3a6e795930eb86a082c270d0a7af1e3aa2%26scene%3D21%23wechat_redirect)
- [浅析 Geoffrey Hinton 最近提出的 Capsule 计划](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650731207%26idx%3D1%26sn%3Ddb9b376df658d096f3d1ee71179d9c8a%26chksm%3D871b36b9b06cbfafb152abaa587f6730716c5069e8d9be4ee9def055bdef089d98424d7fb51b%26scene%3D21%23wechat_redirect)

本文章所解释的代码均来自于 capsule_model.py 和 layers.py 两个文件，它们也是整个实现的核心部分。下面，我们将从基本的 Capsule 概念与 Squash 非线性函数开始解析 Sara 所完成的实现。

在论文中，Geoffrey Hinton 介绍 Capsule 为：「Capsule 是一组神经元，其输入输出向量表示特定实体类型的实例化参数（即特定物体、概念实体等出现的概率与某些属性）。我们使用输入输出向量的长度表征实体存在的概率，向量的方向表示实例化参数（即实体的某些图形属性）。同一层级的 capsule 通过变换矩阵对更高级别的 capsule 的实例化参数进行预测。当多个预测一致时（本论文使用动态路由使预测一致），更高级别的 capsule 将变得活跃。」

Capsule 中神经元的激活情况表示了图像中存在的特定实体的各种性质。这些性质可以包含很多种不同的参数，例如姿势（位置、大小、方向）、变形、速度、反射率、色彩、纹理等等。而输入输出向量的长度表示了某个实体出现的概率，所以它的值必须在 0 到 1 之间。

为了实现这种压缩，并完成 Capsule 层级的激活功能，Hinton 等人使用了一个被称为「squashing」的非线性函数。该非线性函数确保短向量的长度能够缩短到几乎等于零，而长向量的长度压缩到接近但不超过 1 的情况。以下是该非线性函数的表达式：
![](https://pic2.zhimg.com/v2-129ee4e815bb819c7b2b39f4e6f44915_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='180'></svg>)
其中 v_j 为 Capsule j 的输出向量，s_j 为上一层所有 Capsule 输出到当前层 Capsule j 的向量加权和，简单说 s_j 是 Capsule j 的输入向量。

在 Sara 提供的实现中，她们使用以下方法定义非线性激活函数。其中输入张量对于全连接 Capsule 层来说维度为 [batch, num_channels, num_atoms]，对于卷积 Capsule 层来说，输入的维度为 [batch, num_channels, num_atoms, height, width]。该函数将输出一组激活张量 v_j，其维度等于输入张量的维度。





```
def _squash(input_tensor):
"""Applies norm nonlinearity (squash) to a capsule layer.
 Args:
   input_tensor: Input tensor. Shape is [batch, num_channels, num_atoms] for a
     fully connected capsule layer or
     [batch, num_channels, num_atoms, height, width] for a convolutional
     capsule layer.
 Returns:
   A tensor with same shape as input (rank 3) for output of this layer.
 """
with tf.name_scope('norm_non_linearity'):
   norm = tf.norm(input_tensor, axis=2, keep_dims=True)
   norm_squared = norm * norm
return (input_tensor / norm) * (norm_squared / (1 + norm_squared))
```


因为按照 Hinton 的思想，找到最好的处理路径就等价于正确处理了图像，所以在 Capsule 中加入 Routing 机制可以找到一组系数 c_ij，它们能令预测向量 u_j|i hat 最符合输出向量 v_j，即最符合输出的输入向量，这样我们就找到了最好的路径。
![](https://pic4.zhimg.com/v2-1254498b17f24925517c09256739e4af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='635'></svg>)
*图1：该图展示了 Capsule 的层级结构与动态 Routing 的过程*

按照论文 Dynamic Routing between Capsules 所述，c_ij 为耦合系数，该系数由动态 Routing 过程迭代地更新与确定。Capsule i 和后一层级所有 Capsule 间的耦合系数和为 1。此外，该耦合系数由「routing softmax」决定，且 softmax 函数中的 logits b_ij 初始化为 0，耦合系数 c_ij 的 softmax 计算方式为：
![](https://pic3.zhimg.com/v2-f4fb0497f0e7d470baf1b9fd27a1eb0a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='850' height='131'></svg>)
b_ij 依赖于两个 Capsule 的位置与类型，但不依赖于当前的输入图像。我们可以通过测量后面层级中每一个 Capsule j 的当前输出 v_j 和 前面层级 Capsule i 的预测向量间的一致性，然后借助该测量的一致性迭代地更新耦合系数。本论文简单地通过内积度量这种一致性，即 
![](https://pic4.zhimg.com/v2-fea85dc4ac5dd49ee3c789d4e6799fd3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='194' height='41'></svg>)
，这一部分也就涉及到使用 Routing 更新耦合系数。

有意思的是，Sara 的实现会添加一个 leaky_routing 函数，按照该函数的定义，它会添加额外的维度以路由分对数（logits）。如果需要执行路由的张量维度与上层任意 Capsule 单元不匹配，那么该函数将允许激活的 Capsule 单元在额外的维度中进行路由。如下参数 logits 为需要路由的张量，其中它的维度在全连接层的情况下为 [input_capsule_num, output_capsule_num]，在卷积层的情况下回多增加两个维度。output_dim 为分对数的第二个维度，即输出的 Capsule 单元数。


```
def _leaky_routing(logits, output_dim):
"""Adds extra dimmension to routing logits.
 This enables active capsules to be routed to the extra dim if they are not a
 good fit for any of the capsules in layer above.
 Args:
   logits: The original logits. shape is
     [input_capsule_num, output_capsule_num] if fully connected. Otherwise, it
     has two more dimmensions.
   output_dim: The number of units in the second dimmension of logits.
 Returns:
   Routing probabilities for each pair of capsules. Same shape as logits.
 """

# leak is a zero matrix with same shape as logits except dim(2) = 1 because
# of the reduce_sum.
 leak = tf.zeros_like(logits, optimize=True)
 leak = tf.reduce_sum(leak, axis=2, keep_dims=True)
 leaky_logits = tf.concat([leak, logits], axis=2)
 leaky_routing = tf.nn.softmax(leaky_logits, dim=2)
return tf.split(leaky_routing, [1, output_dim], 2)[1]
```


如上所示，tf.zeros_like 将构建一个与 logits 维度相同的张量，其中每个元素都为 0。在求和处理后，leaky_logits 将在第三个维度拼接 leak 和 logits 张量。在对 leaky_logits 第 3 个维度进行 Softmax 后就相当于计算了以下伪代码中的耦合系数 c_ij，我们需要使用它执行进一步的路由。此外，_leaky_routing 应该是没有应用 Squash 非线性激活。因此该函数真实的意义与过程可能需要进一步探讨，这里的理解不是很完全。

以上_leaky_routing 函数会在完整执行路由和非线性压缩的_update_routing 函数中调用，所以在我们查看完整的路由函数前可以先复习以下原论文中所描述的动态路由伪代码。

Routing 过程就是图1右边表述的更新过程，我们会计算 v_j 与 u_j|i hat 的乘积并将它与原来的 b_ij 相加而更新 b_ij，然后利用 softmax(b_ij) 更新 c_ij 而进一步修正了后一层的 Capsule 输入 s_j。当输出新的 v_j 后又可以迭代地更新 c_ij，这样我们不需要反向传播而直接通过计算输入与输出的一致性更新参数。
![](https://pic3.zhimg.com/v2-6914e2ee727b61ab845e63471e303b86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='317'></svg>)
对于所有在 l 层的 Capsule i 和在 l+1 层的 Capsule j，先初始化 b_ij 等于零。然后迭代 r 次，每次先根据 b_i 计算 c_i，然后在利用 c_ij 与 u_j|i hat 计算 s_j 与 v_j。利用计算出来的 v_j 更新 b_ij 以进入下一个迭代循环更新 c_ij。该 Routing 算法十分容易收敛，基本上通过 3 次迭代就能有不错的效果。

在以下定义的路由更新过程中，_update_routing 函数会对经精炼的输入张量求和并执行 Squash 非线性变换。它的输出激活值可作为 PrimaryCaps 层和 DigitCaps 层的最终输出，后面我们将详细讨论这两个层的实现。按照 _update_routing 函数的说明，它会基于当前层的激活值与前一层投票结果（即线性组合结果）之间的相似性，迭代地更新 logits 路由结果，即对输入张量进行精炼。


```
def _update_routing(votes, biases, logit_shape, num_dims, input_dim, output_dim,
                   num_routing, leaky):
 votes_t_shape = [3, 0, 1, 2]
for i in range(num_dims - 4):
   votes_t_shape += [i + 4]
 r_t_shape = [1, 2, 3, 0]
for i in range(num_dims - 4):
   r_t_shape += [i + 4]
 votes_trans = tf.transpose(votes, votes_t_shape)

def _body(i, logits, activations):
"""Routing while loop."""
# route: [batch, input_dim, output_dim, ...]
if leaky:
     route = _leaky_routing(logits, output_dim)
else:
     route = tf.nn.softmax(logits, dim=2)
   preactivate_unrolled = route * votes_trans
   preact_trans = tf.transpose(preactivate_unrolled, r_t_shape)
   preactivate = tf.reduce_sum(preact_trans, axis=1) + biases
   activation = _squash(preactivate)
   activations = activations.write(i, activation)
# distances: [batch, input_dim, output_dim]
   act_3d = tf.expand_dims(activation, 1)
   tile_shape = np.ones(num_dims, dtype=np.int32).tolist()
   tile_shape[1] = input_dim
   act_replicated = tf.tile(act_3d, tile_shape)
   distances = tf.reduce_sum(votes * act_replicated, axis=3)
   logits += distances
return (i + 1, logits, activations)

 activations = tf.TensorArray(
     dtype=tf.float32, size=num_routing, clear_after_read=False)
 logits = tf.fill(logit_shape, 0.0)
 i = tf.constant(0, dtype=tf.int32)
 _, logits, activations = tf.while_loop(
lambda i, logits, activations: i < num_routing,
     _body,
     loop_vars=[i, logits, activations],
     swap_memory=True)

return activations.read(num_routing - 1)
```


如上所示，votes 为前一层经转换的输出张量。num_dims 为输入 votes 的维度数量，对于全连接 Capsule 层来说，它的维度为 4，对于卷积层来说，它的维度为 6。input_dim 为 输入层的 Capsule 单元数，output_dim 为输出层的 Capsule 单元数。num_routing 为路由的迭代次数，而 leaky 则代表着是否使用前面定义的渗漏路由 _leaky_routing。

以上  _update_routing 函数最终会输出一个激活张量，即上面原论文伪代码中的 v_j。在初步讨论了路由算法后，我们可以查看它到底用在了哪些地方，即哪些运算需要执行路由算法。从 Sara 的代码上看，CapsNet 应该使用了两次 Routing，即在第二层的卷积层和第三层的 Capsule 全连接层后各调用了一次。这一点与论文的描述和其他研究者所实现的复现有所不同。

下面，我们将依据原论文与 Sara 开源的实现讨论 CapsNet 主体架构和 Margin loss 度量。这一部分是该论文与实现的核心，因此我们将重点关注这一部分而忽略后面构建的重构网络与重构损失。

以下是 CapsNet 的整体架构：
![](https://pic2.zhimg.com/v2-1339695ac16d25ba19f1a6a500fb5705_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='327'></svg>)
第一个卷积层使用了 256 个 9×9 卷积核，步幅为 1，且使用了 ReLU 激活函数。该卷积操作应该没有使用 Padding，输出的张量才能是 20×20×256。此外，CapsNet 的卷积核感受野使用的是 9×9，相比于其它 3×3 或 5×5 的要大一些，这个能是因为较大的感受野在 CNN 层次较少的情况下能感受的信息越多。这两层间的权值数量应该为 9×9×256+256=20992。

随后，第二个卷积层 PrimaryCaps 开始作为 Capsule 层的输入而构建相应的张量结构，我们可以从上图看出第二层卷积操作后生成的张量维度为 6×6×8×32。如果我们先考虑 32 个（32 channel）9×9 的卷积核在步幅为 2 的情况下做卷积，那么实际上得到的是传统的 6×6×32 的张量，即等价于 6×6×1×32。因为传统卷积操作每次计算的输出都是一个标量，而 PrimaryCaps 的输出需要是一个长度为 8 的向量，因此传统卷积下的三维输出张量 6×6×1×32 就需要变化为四维输出张量 6×6×8×32。

第三层 DigitCaps 在第二层输出的向量基础上进行传播与 Routing 更新。第二层共输出 6×6×32=1152 个向量，每一个向量的维度为 8，即第 i 层共有 1152 个 Capsule 单元。而第三层 j 有 10 个标准的 Capsule 单元，每个 Capsule 的输出向量有 16 个元素。前一层的 Capsule 单元数是 1152 个，那么 w_ij 将有 1152×10 个，且每一个 w_ij 的维度为 8×16。当 u_i 与对应的 w_ij 相乘得到预测向量后，我们会有 1152×10 个耦合系数 c_ij，对应加权求和后会得到 10 个 16×1 的输入向量。将该输入向量输入到「squashing」非线性函数中求得最终的输出向量 v_j，其中 v_j 的长度就表示识别为某个类别的概率。

如下所示定义了构建 Capsule 层级的主体函数：


```
def _build_capsule(self, input_tensor, num_classes):

   capsule1 = layers.conv_slim_capsule(
       input_tensor,
       input_dim=1,
       output_dim=self._hparams.num_prime_capsules,
       layer_name='conv_capsule1',
       num_routing=1,
       input_atoms=256,
       output_atoms=8,
       stride=2,
       kernel_size=9,
       padding=self._hparams.padding,
       leaky=self._hparams.leaky,)
   capsule1_atom_last = tf.transpose(capsule1, [0, 1, 3, 4, 2])
   capsule1_3d = tf.reshape(capsule1_atom_last,
                            [tf.shape(input_tensor)[0], -1, 8])
   _, _, _, height, width = capsule1.get_shape()
   input_dim = self._hparams.num_prime_capsules * height.value * width.value
return layers.capsule(
       input_tensor=capsule1_3d,
       input_dim=input_dim,
       output_dim=num_classes,
       layer_name='capsule2',
       input_atoms=8,
       output_atoms=16,
       num_routing=self._hparams.routing,
       leaky=self._hparams.leaky,)
```


该函数在输入 5 维张量 [batch, 1, 256, height, width] 和目标类别的数量 num_classes 后会输出一个 3 维张量，它将表示 10 个类别的 Capsule 嵌入向量。也就是说，该函数将构建完整的 CapsNet 架构，并输出 DigitCaps 层最后得到的 10 个 16 维向量。

该主体函数主要调用了一个 slim 卷积 Capsule 层和一个 Capsule 层。slim 卷积 Capsule 层主要将输入张量转换为 Capsule 格式，即上图的 PrimaryCaps 层。而为了连接卷积 Capsule 层和顶部的全连接 Capsule 层，卷积 Capsule 层的网格位置将与不同类型的 Capsule 维度相合并，并且 Capsule 将为嵌入向量学习不同的变换。下面我们会详细讨论构建以上网络主体的两个函数。

在主体模型的代码中（capsule_model.py 第 54 行），第二个卷积层需要通过调用 Sara 等人定义的 conv_slim_capsule 函数实现，以下的代码构建了原论文中的 PrimaryCaps 层，其中 input_tensor 为原图片经过一次卷积后的特征图，并增加一个维度以作为一个 Capsule 单元包含神经元的个数（构成向量）。


```
capsule1 = layers.conv_slim_capsule(
       input_tensor,
       input_dim=1,
       output_dim=self._hparams.num_prime_capsules,
       layer_name='conv_capsule1',
       num_routing=1,
       input_atoms=256,
       output_atoms=8,
       stride=2,
       kernel_size=9,
       padding=self._hparams.padding,
       leaky=self._hparams.leaky,)
```


在上面这些参数中，input_tensor 为五维张量即标准卷积的四维张量再加上一维 Capsule 单元数（capsule_model.py 第 194 行）。input_dim 为上一个 Capsule 层的单元数或维度，output_dim 为多个并行卷积操作后所得到的 Capsule 单元数或维度。input_atoms 为前一层 Capsule 单元的元素数，即一个 Capsule 单元包含的神经元数量，这里 256 代表第一个卷积层所产生的 256 张特征图，而 output_atoms 表示当前层的 Capsule 单元元素数，这里的 8 可以是代表 8 张 6×6×1×32 的特征图。其实小编认为我们可以如 Sara 等人的实现将 PrimaryCaps 层看成 32 个 Capsule 单元，每个单元包含 8 个标量神经元，或者将其看成 8 个 Capsule 单元，每个单元包含 32 个标量神经元，这两种表示方法应该是等价的。剩下其它的参数就和标准卷积层所定义的参数意义一样，所以读者可以阅读源代码详细地了解。

在 layers.py 文件中（268 行），Sara 等人定义了 conv_slim_capsule 函数以完成 PrimaryCaps 层的构建。该函数使用 slim 对给定五维的输入张量执行二维卷积，输入张量的维度一般为 [batch, input_dim, input_atoms, input_height, input_width]。然后该函数将使用动态路由算法精炼前面卷积运算的结果，并对每一个 Capsule 单元应用非线性 Squash 函数。

conv_slim_capsule 函数所输出的激活值张量维度为 [batch, output_dim, output_atoms, out_height, out_width]。如果 Padding 选择的是『SAME』，那么输出特征图的高和宽就与输入张量的宽和高。我们注意到执行卷积操作具体的函数为前面定义的_depthwise_conv3d，该函数将返回经过 2 维卷积的 6 维张量。

_depthwise_conv3d 函数在给定一个 5 维输入张量的情况下会执行 2 维卷积运算，输入张量的维度与 conv_slim_capsule 函数的输入相同。_depthwise_conv3d 函数会将输入 5 维张量中 Batch 和 input_dim 的乘积作为 1 维而压缩为 4 维张量，即压缩输入张量的第一维与第二维为一个维度。之所以需要压缩为 4 维，是因为我们需要将其作为 tf.nn.conv2d 的输入。在执行卷积后，我们需要重新将这 4 维张量分解为 6 维张量。即将第一维分解为 Batch 和 input_dim，将第二维分解为 output_dim 和 output_atom。


```
def _depthwise_conv3d(input_tensor,
                     kernel,
                     input_dim,
                     output_dim,
                     input_atoms=8,
                     output_atoms=8,
                     stride=2,
                     padding='SAME'):

with tf.name_scope('conv'):
   input_shape = tf.shape(input_tensor)
   _, _, _, in_height, in_width = input_tensor.get_shape()
# Reshape input_tensor to 4D by merging first two dimmensions.
# tf.nn.conv2d only accepts 4D tensors.

   input_tensor_reshaped = tf.reshape(input_tensor, [
       input_shape[0] * input_dim, input_atoms, input_shape[3], input_shape[4]
   ])
   input_tensor_reshaped.set_shape((None, input_atoms, in_height.value,
                                    in_width.value))
   conv = tf.nn.conv2d(
       input_tensor_reshaped,
       kernel,
       [1, 1, stride, stride],
       padding=padding,
       data_format='NCHW')
   conv_shape = tf.shape(conv)
   _, _, conv_height, conv_width = conv.get_shape()
# Reshape back to 6D by splitting first dimmension to batch and input_dim
# and splitting second dimmension to output_dim and output_atoms.

   conv_reshaped = tf.reshape(conv, [
       input_shape[0], input_dim, output_dim, output_atoms, conv_shape[2],
       conv_shape[3]
   ])
   conv_reshaped.set_shape((None, input_dim, output_dim, output_atoms,
                            conv_height.value, conv_width.value))
return conv_reshaped, conv_shape, input_shape
```


如上所示为 _depthwise_conv3d 函数，其中参数 input_dim、output_dim 和 input_atoms 等参数的意义与 conv_slim_capsule 函数一致。该函数会返回 6 维张量 [batch, input_dim, output_dim, output_atoms, out_height, out_width]、卷积后的维度和输入张量的维度，并在 conv_slim_capsule 函数中做进一步处理，下面我们将回头继续讨论构建 PrimaryCaps 层的函数。

如下定义了 conv_slim_capsule 函数，其层级的每一个 Capsule 单元都为卷积单元，它们在位置网格和不同的下层 Capsule 单元间共享卷积核权重。因此，该函数可训练的变量为卷积核权重 [kernel_size, kernel_size, input_atoms, output_dim * output_atoms] 和偏置项 [output_dim, output_atoms]。二维卷积的输出为一个单 Capsule 单元，其通道为 Capsule 单元（atoms）的数量。因此，conv_slim_capsule 函数可以构建在二维卷积层的顶部，其中该二维卷积的 num_routing=1、input_dim=1 和 input_atoms=conv_channels。

通过观察定义卷积权重的 kernel 变量，我们可以了解该运算本质上就是执行上一层特征图数为 input_atoms、本层卷积核数为 output_dim * output_atoms 的卷积操作。在完成卷积运算后，Sara 的实现接着调用了一次前面定义的路由算法，这似乎与 naturomics 等人复现的代码有一些不同，他们在实现卷积后会将卷积结果直接投入 Squash 非线性函数。当然，原论文似乎也没有体现这一点，我们都以为只有在 DigitCaps 层才会进行动态路由过程。

在后面的 capsule 函数中（layers.py 第 138 行），Sara 确实又调用了一次动态路由算法，我们会在后面讨论该函数。因为路由算法计算了 Squash 值，因此返回的激活值可作为 PrimaryCaps 层的输出。


```
def conv_slim_capsule(input_tensor,
                     input_dim,
                     output_dim,
                     layer_name,
                     input_atoms=8,
                     output_atoms=8,
                     stride=2,
                     kernel_size=5,
                     padding='SAME',
                     **routing_args):

with tf.variable_scope(layer_name):
   kernel = variables.weight_variable(shape=[
       kernel_size, kernel_size, input_atoms, output_dim * output_atoms
   ])
   biases = variables.bias_variable([output_dim, output_atoms, 1, 1])
   votes, votes_shape, input_shape = _depthwise_conv3d(
       input_tensor, kernel, input_dim, output_dim, input_atoms, output_atoms,
       stride, padding)

with tf.name_scope('routing'):
     logit_shape = tf.stack([
         input_shape[0], input_dim, output_dim, votes_shape[2], votes_shape[3]
     ])
     biases_replicated = tf.tile(biases,
                                 [1, 1, votes_shape[2], votes_shape[3]])
     activations = _update_routing(
         votes=votes,
         biases=biases_replicated,
         logit_shape=logit_shape,
         num_dims=6,
         input_dim=input_dim,
         output_dim=output_dim,
         **routing_args)
return activations
```


在返回 PrimaryCaps 层的 5 维张量后，主体函数会将其转换维 3 维张量并馈送到 capsule 函数，从而构建一个全连接 Capsule 层。在给定输入张量 [batch, input_dim, input_atoms] 后，Capsule 层将执行以下操作：
- 对于每一个输入 Capsule 单元，将它与权重变量相乘以得到线性组合的结果（函数中表示为 votes 变量），这一步将得到原论文所述的 u_j|i hat，即 u_j|i hat = W_ij * u_i。线性组合后的结果维度为 [batch, input_dim, output_dim, output_atoms]。
- 通过迭代地执行路由过程更新与精炼前面线性组合的结果，即原论文中的 s_j = ∑ c_ij * u_j|i hat，其中 c_ij = softmax(b_ij)。
- 最后使用 Squash 函数将每个 Capsule 单元的输出压缩到 L2 范数小于 1 的情况。

此外，当前层的每一个 Capsule 单元对前一层的 Capsule 单元都保留一个权重张量。因此在训练中，capsule 函数的权重 [input_dim * num_in_atoms, output_dim * num_out_atoms] 和偏置项 [output_dim * num_out_atoms] 都是需要更新的参数。

如下展示了 capsule 函数的定义，其中 input_dim 同样为前一层的 Capsule 单元数，input_atoms 同样为前一层每个 Capsule 单元的元素数，其它参数的意义与上面几个函数都差不多。该函数将输出张量 [batch, output_dim, output_atoms]。


```
def capsule(input_tensor,
           input_dim,
           output_dim,
           layer_name,
           input_atoms=8,
           output_atoms=8,
           **routing_args):

with tf.variable_scope(layer_name):
# weights variable will hold the state of the weights for the layer
   weights = variables.weight_variable(
       [input_dim, input_atoms, output_dim * output_atoms])
   biases = variables.bias_variable([output_dim, output_atoms])
with tf.name_scope('Wx_plus_b'):
# Depthwise matmul: [b, d, c] ** [d, c, o_c] = [b, d, o_c]
# To do this: tile input, do element-wise multiplication and reduce
# sum over input_atoms dimmension.
     input_tiled = tf.tile(
         tf.expand_dims(input_tensor, -1),
         [1, 1, 1, output_dim * output_atoms])
     votes = tf.reduce_sum(input_tiled * weights, axis=2)
     votes_reshaped = tf.reshape(votes,
                                 [-1, input_dim, output_dim, output_atoms])
with tf.name_scope('routing'):
     input_shape = tf.shape(input_tensor)
     logit_shape = tf.stack([input_shape[0], input_dim, output_dim])
     activations = _update_routing(
         votes=votes_reshaped,
         biases=biases,
         logit_shape=logit_shape,
         num_dims=4,
         input_dim=input_dim,
         output_dim=output_dim,
         **routing_args)
return activations
```


如上所示，tf.expand_dims 会将输入张量在最后扩充一维，而 tf.tile 会将扩充后的 4 维张量在最后一维复制 output_dim * output_atoms 次。在执行逐元素的乘积后，沿着第三维 input_atoms 对乘积结果求和。求和后的张量可作为最后的线性组合结果而投入路由算法中进行迭代更新，返回的张量即 DigitCaps 层最终输出的 10 个 16 维的向量，每个向量编码并表征着 10 类手写数字。

在调用完 capsule 函数后，整个_build_capsule 函数所构建的 CapsNet 架构就完成了。原论文使用了 Margin loss 来衡量这 10 个输出向量预测类别的准确度，而后面也可以使用全连接网络将这 10 个向量重构为不同手写数字的图像，并使用欧几里得距离度量重构损失。

在论文解读中，我们已经了解 DigitCaps 层输出向量的长度即某个类别的概率，那么我们该如何构建损失函数，并根据该损失函数迭代地更新整个网络？前面我们耦合系数 c_ij 是通过一致性 Routing 进行更新的，他并不需要根据损失函数更新，但整个网络其它的卷积参数和 Capsule 内的 W_ij 都需要根据损失函数进行更新。一般我们就可以对损失函数直接使用标准的反向传播更新这些参数，而在原论文中，作者采用了 SVM 中常用的 Margin loss，该损失函数的表达式为：
![](https://pic1.zhimg.com/v2-96143d2fa1224259f905230717ad424c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='993' height='79'></svg>)
其中 c 是分类类别，T_c 为分类的指示函数（c 存在为 1，c 不存在为 0），m+ 为上边界，m- 为下边界。此外，v_c 的模即向量的 L2 距离。

因为实例化向量的长度来表示 Capsule 要表征的实体是否存在，所以当且仅当图片里出现属于类别 k 的手写数字时，我们希望类别 k 的最顶层 Capsule 的输出向量长度很大（在本论文 CapsNet 中为 DigitCaps 层的输出）。为了允许一张图里有多个数字，我们对每一个表征数字 k 的 Capsule 分别给出单独的 Margin loss。

以下_margin_loss 定义了 CapsNet 的损失函数，它会惩罚每个输入分对数偏离边缘的程度。如函数说明所示，该函数将衡量每一个错误分对数对于边缘的距离。对于负的分对数来说，边缘为 0.1，对于正的分对数，边缘为 0.9。若我们同时对这两个边缘先减去 0.5，那么当前的边缘将会都变为 0.4。


```
def _margin_loss(labels, raw_logits, margin=0.4, downweight=0.5):
"""Penalizes deviations from margin for each logit.
 Each wrong logit costs its distance to margin. For negative logits margin is
 0.1 and for positives it is 0.9. First subtract 0.5 from all logits. Now
 margin is 0.4 from each side.
 Args:
   labels: tensor, one hot encoding of ground truth.
   raw_logits: tensor, model predictions in range [0, 1]
   margin: scalar, the margin after subtracting 0.5 from raw_logits.
   downweight: scalar, the factor for negative cost.
 Returns:
   A tensor with cost for each data point of shape [batch_size].
 """
 logits = raw_logits - 0.5
 positive_cost = labels * tf.cast(tf.less(logits, margin),
                                  tf.float32) * tf.pow(logits - margin, 2)
 negative_cost = (1 - labels) * tf.cast(
     tf.greater(logits, -margin), tf.float32) * tf.pow(logits + margin, 2)
return 0.5 * positive_cost + downweight * 0.5 * negative_cost
```


如上所示，margin 为 raw_logits 减去 0.5 的边缘，而 downweight 负成本的因素。

以上是 CapsNet 的主体代码，也是整个 Capsule 的核心。Sara 开源的实现还有很多重要的代码与函数，我们也将继续探讨与思考其中具体的过程，尤其是理解_leaky_routing 的作用。我们也希望有读者和我们一起解析与分析 Sara 的实现，并探讨其最终实现的基线结果。

以下是 Sara 实现的说明，其使用的是 Python 2.7，不过 naturomics 等研究者已经在 GitHub 上修改为了 Python 3。读者可进一步测试与实现它们：




**要求：**
- TensorFlow
- NumPy
- GPU

运行以下命令以测试配置是否正确：

`python layers_test.py`

若我们下载了 Sara 等人提供的 MNIST 数据集和预训练模型，并把它们放入$DATA_DIR/和$CKPT_DIR/ 目录下。那么我们可以运行以下命令而快速获得 CapsNet 在 MNIST 上的测试结果。
- 数据集：[https://storage.googleapis.com/capsule_toronto/mnist_data.tar.gz](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/capsule_toronto/mnist_data.tar.gz)
- 模型：[https://storage.googleapis.com/capsule_toronto/mnist_checkpoints.tar.gz](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/capsule_toronto/mnist_checkpoints.tar.gz)


```
python experiment.py --data_dir=$DATA_DIR/mnist_data/ --train=false \
--summary_dir=/tmp/ --checkpoint=$CKPT_DIR/mnist_checkpoint/model.ckpt-1
```


下载并抽取二进制版的 cifar10 数据集到 $DATA_DIR/，cifar 10 预训练模型到 $CKPT_DIR/。
- 数据集：[https://www.cs.toronto.edu/~kriz/cifar.html](https://link.zhihu.com/?target=https%3A//www.cs.toronto.edu/~kriz/cifar.html)
- 模型：[https://storage.googleapis.com/capsule_toronto/cifar_checkpoints.tar.gz](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/capsule_toronto/cifar_checkpoints.tar.gz)


```
python experiment.py --data_dir=$DATA_DIR --train=false --dataset=cifar10 \
--hparams_override=num_prime_capsules=64,padding=SAME,leaky=true,remake=false \
--summary_dir=/tmp/ --checkpoint=$CKPT_DIR/cifar/cifar{}/model.ckpt-600000 \
--num_trials=7
```


Sample CIFAR10 训练命令：


```
python experiment.py --data_dir=$DATA_DIR --dataset=cifar10 --max_steps=600000\
--hparams_override=num_prime_capsules=64,padding=SAME,leaky=true,remake=false \
--summary_dir=/tmp/
```


Sample MNIST 全部训练命令：
- 训练-验证传播模式： --validate=true
- 在更多 GPU 上训练：—num_gpus=NUM_GPUS


```
python experiment.py --data_dir=$DATA_DIR/mnist_data/ --max_steps=300000\
--summary_dir=/tmp/attempt0/
```


Sample MNIST 基线训练命令：


```
python experiment.py --data_dir=$DATA_DIR/mnist_data/ --max_steps=300000\
--summary_dir=/tmp/attempt1/ --model=baseline
```


该实现的详细使用方法请查阅 README 文件，我们以上只简要介绍了一部分。我们也尝试着实现 Sara 开源的模型，最开始是 xrange 等 Python 2.7 的函数会报错，在改为 Python 3 后运行仍然会有问题。所以我们更希望有读者能完成该模型的测试，并向大家展示 Dynamic Routing Between Capsules 论文所实现的结果。 
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
**本文为机器之心原创，转载请联系本公众号获得授权。**




