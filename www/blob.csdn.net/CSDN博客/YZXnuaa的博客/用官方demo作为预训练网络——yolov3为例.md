# 用官方demo作为预训练网络——yolov3为例 - YZXnuaa的博客 - CSDN博客
2018年06月01日 17:25:22[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2261
１．下载权重文件并放入检测器目录下，通过该地址下载：https://pjreddie.com/media/files/yolov3.weights
**２．理解权重文件**
官方的权重文件是一个二进制文件，它以序列方式储存神经网络权重。
我们必须小心地读取权重，因为权重只是以浮点形式储存，没有其它信息能告诉我们到底它们属于哪一层。所以如果读取错误，那么很可能权重加载就全错了，模型也完全不能用。因此，只阅读浮点数，无法区别权重属于哪一层。因此，我们必须了解权重是如何存储的。
首先，权重只属于两种类型的层，即批归一化层（batch norm layer）和卷积层。这些层的权重储存顺序和配置文件中定义层级的顺序完全相同。所以，如果一个 convolutional 后面跟随着 shortcut 块，而 shortcut 连接了另一个 convolutional 块，则你会期望文件包含了先前 convolutional 块的权重，其后则是后者的权重。
当批归一化层出现在卷积模块中时，它是不带有偏置项的。然而，当卷积模块不存在批归一化，则偏置项的「权重」就会从文件中读取。下图展示了权重是如何储存的。
![](https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9zgrU2v6kd3F1yZV5TxOsdskzx9Viafk3icBhcz40YyEicsu2dicsMPCtqibGcZmt5h6lO3qQerAKcjicQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**３．加载权重**
我们写一个函数来加载权重，它是 Darknet 类的成员函数。它使用 self 以外的一个参数作为权重文件的路径。
`def load_weights(self, weightfile):`
第一个 160 比特的权重文件保存了 5 个 int32 值，它们构成了文件的标头。
```
#Open the weights file
 fp = open(weightfile, "rb")
#The first 5 values are header information 
# 1. Major version number
# 2. Minor Version Number
# 3. Subversion number 
# 4,5. Images seen by the network (during training)
 header = np.fromfile(fp, dtype = np.int32, count = 5)
 self.header = torch.from_numpy(header)
 self.seen = self.header[3]
```
之后的比特代表权重，按上述顺序排列。权重被保存为 float32 或 32 位浮点数。我们来加载 np.ndarray 中的剩余权重。
```
weights = np.fromfile(fp, dtype = np.float32)
```
现在，我们迭代地加载权重文件到网络的模块上
```
ptr = 0
for i in range(len(self.module_list)):
 module_type = self.blocks[i + 1]["type"]
#If module_type is convolutional load weights
#Otherwise ignore.
```
在循环过程中，我们首先检查 convolutional 模块是否有 batch_normalize（True）。基于此，我们加载权重。
```
if module_type == "convolutional":
 model = self.module_list[i]
try:
 batch_normalize = int(self.blocks[i+1]["batch_normalize"])
except:
 batch_normalize = 0
 conv = model[0]
```
我们保持一个称为 ptr 的变量来追踪我们在权重数组中的位置。现在，如果 batch_normalize 检查结果是 True，则我们按以下方式加载权重：
```
if (batch_normalize):
 bn = model[1]
#Get the number of weights of Batch Norm Layer
 num_bn_biases = bn.bias.numel()
#Load the weights
 bn_biases = torch.from_numpy(weights[ptr:ptr + num_bn_biases])
 ptr += num_bn_biases
 bn_weights = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
 ptr += num_bn_biases
 bn_running_mean = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
 ptr += num_bn_biases
 bn_running_var = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
 ptr += num_bn_biases
#Cast the loaded weights into dims of model weights. 
 bn_biases = bn_biases.view_as(bn.bias.data)
 bn_weights = bn_weights.view_as(bn.weight.data)
 bn_running_mean = bn_running_mean.view_as(bn.running_mean)
 bn_running_var = bn_running_var.view_as(bn.running_var)
#Copy the data to model
 bn.bias.data.copy_(bn_biases)
 bn.weight.data.copy_(bn_weights)
 bn.running_mean.copy_(bn_running_mean)
 bn.running_var.copy_(bn_running_var)
```
如果 batch_normalize 的检查结果不是 True，只需要加载卷积层的偏置项。
```
else:
#Number of biases
 num_biases = conv.bias.numel()
#Load the weights
 conv_biases = torch.from_numpy(weights[ptr: ptr + num_biases])
 ptr = ptr + num_biases
#reshape the loaded weights according to the dims of the model weights
 conv_biases = conv_biases.view_as(conv.bias.data)
#Finally copy the data
 conv.bias.data.copy_(conv_biases)
```
最后，我们加载卷积层的权重
```
#Let us load the weights for the Convolutional layers
num_weights = conv.weight.numel()
#Do the same as above for weights
conv_weights = torch.from_numpy(weights[ptr:ptr+num_weights])
ptr = ptr + num_weights
conv_weights = conv_weights.view_as(conv.weight.data)
conv.weight.data.copy_(conv_weights)
```
该函数的介绍到此为止，你现在可以通过调用 darknet 对象上的 load_weights 函数来加载 Darknet 对象中的权重。
```
model = Darknet("cfg/yolov3.cfg")
model.load_weights("yolov3.weights")
```
通过模型构建和权重加载，我们终于可以开始进行目标检测了。未来，我们还将介绍如何利用 objectness 置信度阈值和非极大值抑制生成最终的检测结
