
# Normalize Layer in Caffe - 机器学习的小学生 - CSDN博客


2018年05月12日 21:40:17[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1861



```python
message NormalizeParameter {
  optional bool across_spatial =
```
```python
1
```
```python
[
```
```python
default
```
```python
=
```
```python
true
```
```python
];
```
```python
//
```
```python
Initial value
```
```python
of
```
```python
scale. Default
```
```python
is
```
```python
1.0
```
```python
for
```
```python
all
  optional FillerParameter scale_filler =
```
```python
2
```
```python
;
```
```python
//
```
```python
Whether
```
```python
or
```
```python
not
```
```python
scale parameters are shared across channels.
  optional bool channel_shared =
```
```python
3
```
```python
[
```
```python
default
```
```python
=
```
```python
true
```
```python
];
```
```python
//
```
```python
Epsilon
```
```python
for
```
```python
not
```
```python
dividing
```
```python
by
```
```python
zero
```
```python
while
```
```python
normalizing variance
  optional float eps =
```
```python
4
```
```python
[
```
```python
default
```
```python
=
```
```python
1e-10
```
```python
];
}
```
下面进行实验，主要检查三个参数的具体含义，即across_spatial , scale_filler ,channel_shared。
across_spatial: 对于每个样本，其到达norm层的张量形状表示为(1,c,h,w)，那么across_spatial用来指示标准化是否要跨空间位置 (即(h,w))。如果across_spatial=False,则表示分别对(h,w)空间中的每个位置的c个通道的元素（也就是该位置的特征描述矢量）进行单独的标准化。如果across_spatial=True， 则基于所有的c*h*w个元素进行标准化。
scale_filler: 包含可学习的参数(见下面的prototxt)。和卷积层参数一样，可以通过设置学习率来决定是否对该参数进行更新，例如设置学习率为0，实现常数的缩放。
channel_shared: 用来控制scale_filler的参数是否被多个通道共享。如果channel_shared=True, 那么scale_filler中参数的形状为(1,)也就是一个待学习的标量；如果channel_shared=False, 那么scale_filler中参数的形状/长度为(c,), c为通道数。
## 检查1：前向
**deploy.prototxt:**
```python
name:
```
```python
"demo"
```
```python
layer {
  name:
```
```python
"data"
```
```python
type:
```
```python
"Input"
```
```python
top:
```
```python
"data"
```
```python
input_param { shape: {
```
```python
dim
```
```python
:
```
```python
2
```
```python
dim
```
```python
:
```
```python
3
```
```python
dim
```
```python
:
```
```python
2
```
```python
dim
```
```python
:
```
```python
3
```
```python
} }
}
layer {
  name:
```
```python
"norm"
```
```python
type:
```
```python
"Normalize"
```
```python
bottom:
```
```python
"data"
```
```python
top:
```
```python
"norm"
```
```python
norm_param {
    across_spatial:
```
```python
True
```
```python
scale_filler {
      type:
```
```python
"constant"
```
```python
value:
```
```python
6
```
```python
}
    channel_shared:
```
```python
True
```
```python
}
}
```
**main.py**
```python
#coding=UTF-
```
```python
8
```
```python
import caffe
import numpy as np
caffe.set_mode_cpu()
input_data = np.zeros(shape=(
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
),dtype=np.float32)
input_data[
```
```python
0
```
```python
,
```
```python
0
```
```python
,:,:] = np.array(
```
```python
[[1,2,3],[4,5,6]]
```
```python
)
input_data[
```
```python
0
```
```python
,
```
```python
1
```
```python
,:,:] = np.array(
```
```python
[[1,1,2],[4,5,6]]
```
```python
)
input_data[
```
```python
0
```
```python
,
```
```python
2
```
```python
,:,:] = np.array(
```
```python
[[1,2,2],[4,5,6]]
```
```python
)

input_data[
```
```python
1
```
```python
,
```
```python
0
```
```python
,:,:] = np.array(
```
```python
[[1,2,3],[4,5,6]]
```
```python
)
input_data[
```
```python
1
```
```python
,
```
```python
1
```
```python
,:,:] = np.array(
```
```python
[[1,2,3],[4,4,6]]
```
```python
)
input_data[
```
```python
1
```
```python
,
```
```python
2
```
```python
,:,:] = np.array(
```
```python
[[1,2,3],[4,5,5]]
```
```python
)

deploy_pro =
```
```python
'deploy.prototxt'
```
```python
# specify any model
```
```python
and
```
```python
make sure any weight of layers is
```
```python
not
```
```python
loaded 
weight_file =
```
```python
'../pytorch-caffe-master/ZOO_AlexNet/bvlc_alexnet.caffemodel'
```
```python
#
```
```python
not
```
```python
use
net = caffe.Net(deploy_pro,weight_file,caffe.TEST)
shape = input_data.shape
net.blobs[
```
```python
'data'
```
```python
].reshape(shape[
```
```python
0
```
```python
],shape[
```
```python
1
```
```python
],shape[
```
```python
2
```
```python
],shape[
```
```python
3
```
```python
])
net.blobs[
```
```python
'data'
```
```python
].data[...] = input_data
net.forward()
result = net.blobs[
```
```python
'norm'
```
```python
].data
```
```python
print
```
```python
(result)
from caffe.proto import caffe_pb2
import google.protobuf.text_format
net = caffe_pb2.NetParameter()
f = open(deploy_pro,
```
```python
'r'
```
```python
)
net = google.protobuf.text_format.Merge(str(f.read()), net)
f.close()
across_spatial = True
channel_shared = True
scale_type     =
```
```python
''
```
```python
scale_value    =
```
```python
0
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
, len(net.layer)):
```
```python
if
```
```python
net.layer[i].
```
```python
type
```
```python
==
```
```python
'Normalize'
```
```python
:
```
```python
if
```
```python
net.layer[i].norm_param.across_spatial == True: # bias term,
```
```python
for
```
```python
example
            across_spatial = True
```
```python
else
```
```python
:
            across_spatial = False
```
```python
if
```
```python
net.layer[i].norm_param.channel_shared == True:
            channel_shared = True
```
```python
else
```
```python
:
            channel_shared = False
        scale_type = net.layer[i].norm_param.scale_filler.
```
```python
type
```
```python
scale_value = net.layer[i].norm_param.scale_filler.value
        #
```
```python
break
```
```python
print
```
```python
(
```
```python
'The parameters in Normalize layer:'
```
```python
)
```
```python
print
```
```python
(across_spatial)
```
```python
print
```
```python
(channel_shared)
```
```python
print
```
```python
(scale_type)
```
```python
print
```
```python
(scale_value)
```
```python
if
```
```python
across_spatial == False
```
```python
and
```
```python
channel_shared == True
```
```python
and
```
```python
abs(scale_value-
```
```python
1.0
```
```python
)<
```
```python
1e-10
```
```python
:
```
```python
print
```
```python
(
```
```python
'when: across_spatial == False, channel_shared == False, scale_value = 1 '
```
```python
)
    # across_spatial = False, represent: normalize each position(x,y) across three channels
    # check i-th sample
    i =
```
```python
0
```
```python
# i = o
```
```python
or
```
```python
i =
```
```python
1
```
```python
in
```
```python
our case
    position = [
```
```python
0
```
```python
,
```
```python
2
```
```python
]  # the position
```
```python
in
```
```python
heatmap, n * c * h * w, that is,
```
```python
in
```
```python
h*w
    # the result computed by hand 
    temp_result = input_data[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]] 
    result_byhand = temp_result / np.sqrt(np.sum(temp_result**
```
```python
2
```
```python
))
    # the result computed by normalized layer
    result_bylayer = result[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]]
```
```python
print
```
```python
(result_byhand)
```
```python
print
```
```python
(result_bylayer)
```
```python
if
```
```python
across_spatial == True
```
```python
and
```
```python
channel_shared == True
```
```python
and
```
```python
abs(scale_value-
```
```python
1.0
```
```python
)<
```
```python
1e-10
```
```python
:
```
```python
print
```
```python
(
```
```python
'when: across_spatial == True, channel_shared == True, scale_value = 1, check for across_spatial'
```
```python
)
    # across_spatial = False, represent: normalize each position(x,y) across three channels
    # check i-th sample
    i =
```
```python
0
```
```python
# i = o
```
```python
or
```
```python
i =
```
```python
1
```
```python
in
```
```python
our case
    position = [
```
```python
0
```
```python
,
```
```python
2
```
```python
]  # the position
```
```python
in
```
```python
heatmap, n * c * h * w, that is,
```
```python
in
```
```python
h*w
    # the result computed by hand 
    temp_result = input_data[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]] 
    result_byhand = temp_result / np.sqrt(np.sum(input_data[i,:,:,:]**
```
```python
2
```
```python
))  # input_data[i,:,:,:] stand
```
```python
for
```
```python
across space/position/seat
    # the result computed by normalized layer
    result_bylayer = result[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]]
```
```python
print
```
```python
(result_byhand)
```
```python
print
```
```python
(result_bylayer)
```
```python
if
```
```python
across_spatial == True
```
```python
and
```
```python
channel_shared == True
```
```python
and
```
```python
abs(scale_value-
```
```python
1.0
```
```python
) >=
```
```python
0.5
```
```python
: # set scale_value =
```
```python
0.5
```
```python
print
```
```python
(
```
```python
'when: across_spatial == True, channel_shared == True, scale_value != 1 , check for scale_value'
```
```python
)
    # across_spatial = False, represent: normalize each position(x,y) across three channels
    # check i-th sample
    i =
```
```python
0
```
```python
# i = o
```
```python
or
```
```python
i =
```
```python
1
```
```python
in
```
```python
our case
    position = [
```
```python
0
```
```python
,
```
```python
2
```
```python
]  # the position
```
```python
in
```
```python
heatmap, n * c * h * w, that is,
```
```python
in
```
```python
h*w
    # the result computed by hand 
    temp_result = input_data[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]] 
    result_byhand = temp_result / np.sqrt(np.sum(input_data[i,:,:,:]**
```
```python
2
```
```python
))  # input_data[i,:,:,:] stand
```
```python
for
```
```python
across space/position/seat
    result_byhand = result_byhand * scale_value
    # the result computed by normalized layer
    result_bylayer = result[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]]
```
```python
print
```
```python
(result_byhand)
```
```python
print
```
```python
(result_bylayer)
```
```python
if
```
```python
across_spatial == True
```
```python
and
```
```python
channel_shared == False
```
```python
and
```
```python
abs(scale_value-
```
```python
1.0
```
```python
) >=
```
```python
0.5
```
```python
: # set scale_value =
```
```python
0.5
```
```python
print
```
```python
(
```
```python
'when: across_spatial == True, channel_shared == False, scale_value != 1 , check for channel_shared'
```
```python
)
    #####################################################################
    # need back propagation 
    #####################################################################
    # across_spatial = False, represent: normalize each position(x,y) across three channels
    # check i-th sample
    i =
```
```python
0
```
```python
# i = o
```
```python
or
```
```python
i =
```
```python
1
```
```python
in
```
```python
our case
    position = [
```
```python
0
```
```python
,
```
```python
2
```
```python
]  # the position
```
```python
in
```
```python
heatmap, n * c * h * w, that is,
```
```python
in
```
```python
h*w
    # the result computed by hand 
    temp_result = input_data[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]] 
    result_byhand = temp_result / np.sqrt(np.sum(input_data[i,:,:,:]**
```
```python
2
```
```python
))  # input_data[i,:,:,:] stand
```
```python
for
```
```python
across space/position/seat
    result_byhand = result_byhand * scale_value
    # the result computed by normalized layer
    result_bylayer = result[i,:,position[
```
```python
0
```
```python
],position[
```
```python
1
```
```python
]]
```
```python
print
```
```python
(result_byhand)
```
```python
print
```
```python
(result_bylayer)
```
## 检查2：scale的更新
和普通的卷积层参数一样，scale_filler 中的value值会随着迭代的更新而更新，例如，在AlexNet网络的pool5层(其中pool5层输出的blob形状为(n*c*h*w)=(n*256*6*6))后面添加如上的一个norm层，然后在每次迭代后输出：
```python
scale_value = mysolver.net.params[
```
```python
'norm'
```
```python
][
```
```python
0
```
```python
].data
print(scale_value)
```
结果：
```python
5.9999228
5.999773
5.999566
5.99917
5.9985614
5.997842
5.9970937
```
如果想将scale_value设置为固定的值，那么和卷积层参数一样，将学习率设置为0:，如下：
```python
layer {
  name:
```
```python
"norm"
```
```python
type:
```
```python
"Normalize"
```
```python
bottom:
```
```python
"pool5"
```
```python
top:
```
```python
"norm"
```
```python
param {
    lr_mult:
```
```python
0
```
```python
# 学习率设置为0
```
```python
decay_mult:
```
```python
0
```
```python
}
  norm_param {
    across_spatial:
```
```python
True
```
```python
scale_filler {
      type:
```
```python
"constant"
```
```python
value:
```
```python
6
```
```python
}
    channel_shared:
```
```python
True
```
```python
}
}
```
## 检测3：channel_shared
默认情况下，channel_shared设置为True，那么我们在prototxt中将其修改为False。
```python
layer {
  name:
```
```python
"norm"
```
```python
type:
```
```python
"Normalize"
```
```python
bottom:
```
```python
"pool5"
```
```python
top:
```
```python
"norm"
```
```python
param {
    lr_mult:
```
```python
0
```
```python
# 学习率设置为0
```
```python
decay_mult:
```
```python
0
```
```python
}
  norm_param {
    across_spatial:
```
```python
True
```
```python
scale_filler {
      type:
```
```python
"constant"
```
```python
value:
```
```python
6
```
```python
}
    channel_shared:
```
```python
False
```
```python
}
}
```
然后，我们在每次迭代后打印scale_value或者shape或者长度，按照参考文献[1]和[2]描述，其长度应该为256，即每个通道对应一个scale_value值。
```python
scale_value = mysolver.net.params[
```
```python
'norm'
```
```python
][
```
```python
0
```
```python
].data
 print(scale_value.shape)
```
```python
#结果：(256,)
```
参考文献:
1.[https://blog.csdn.net/zqjackking/article/details/69938901[caffe](https://blog.csdn.net/zqjackking/article/details/69938901[caffe)中的normalization_layer]
2.[https://blog.csdn.net/weixin_35653315/article/details/72715367](https://blog.csdn.net/weixin_35653315/article/details/72715367)[Normalization on conv4_3 in SSD]
3.Exploit All the Layers: Fast and Accurate CNN Object Detector with Scale Dependent Pooling and Cascaded Rejection Classifiers
4.SSD 中的 test_normalize_layer.cpp.

