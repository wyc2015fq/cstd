
# keras 代码解析——cifar10 - 颹蕭蕭 - CSDN博客


2019年03月29日 14:39:50[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：209


在笔记本上跑了 4 个 epoch，温度直奔 70 度，还是算了算了
`>>> nvidia-smi
Fri Mar 29 14:04:46 2019
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 399.24                 Driver Version: 399.24                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name            TCC/WDDM | Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce MX150      WDDM  | 00000000:01:00.0 Off |                  N/A |
| N/A   65C    P3    N/A /  N/A |   1679MiB /  2048MiB |     77%      Default |
+-------------------------------+----------------------+----------------------+
+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
|    0      9252      C   ...NoSpace\Miniconda3\envs\py36\python.exe N/A      |
+-----------------------------------------------------------------------------+`来解析一下代码吧！
#### 导入包
```python
import
```
```python
keras
```
```python
from
```
```python
keras
```
```python
.
```
```python
datasets
```
```python
import
```
```python
cifar10
```
```python
from
```
```python
keras
```
```python
.
```
```python
models
```
```python
import
```
```python
Sequential
```
```python
from
```
```python
keras
```
```python
.
```
```python
layers
```
```python
import
```
```python
Dense
```
```python
,
```
```python
Dropout
```
```python
,
```
```python
Activation
```
```python
,
```
```python
Flatten
```
```python
from
```
```python
keras
```
```python
.
```
```python
layers
```
```python
import
```
```python
Conv2D
```
```python
,
```
```python
MaxPooling2D
```
#### 获取数据
不用自己下载喔
```python
num_classes
```
```python
=
```
```python
10
```
```python
model_name
```
```python
=
```
```python
'cifar10.h5'
```
```python
# The data, shuffled and split between train and test sets:
```
```python
(
```
```python
x_train
```
```python
,
```
```python
y_train
```
```python
)
```
```python
,
```
```python
(
```
```python
x_test
```
```python
,
```
```python
y_test
```
```python
)
```
```python
=
```
```python
cifar10
```
```python
.
```
```python
load_data
```
```python
(
```
```python
)
```
来看看得到的数据格式：
```python
print
```
```python
(
```
```python
'type: '
```
```python
,
```
```python
type
```
```python
(
```
```python
x_train
```
```python
)
```
```python
,
```
```python
type
```
```python
(
```
```python
y_train
```
```python
)
```
```python
)
```
```python
print
```
```python
(
```
```python
'shape: '
```
```python
,
```
```python
x_train
```
```python
.
```
```python
shape
```
```python
,
```
```python
y_train
```
```python
.
```
```python
shape
```
```python
)
```
`type:  <class 'numpy.ndarray'> <class 'numpy.ndarray'>
shape:  (50000, 32, 32, 3) (50000, 1)`keras 提供的数据源已经是 numpy 的 n 维数组类型了，可以看出训练样本是 50000 张 RGB 3通道 的 32x32大小的图片，训练目标 y 是数字 0-9 之一，表示10中类型。
#### 数据预处理
```python
# 数据预处理，把 0-255的灰度值转成 0-1 之间的浮点数
```
```python
x_train
```
```python
=
```
```python
x_train
```
```python
.
```
```python
astype
```
```python
(
```
```python
'float32'
```
```python
)
```
```python
/
```
```python
255
```
```python
x_test
```
```python
=
```
```python
x_test
```
```python
.
```
```python
astype
```
```python
(
```
```python
'float32'
```
```python
)
```
```python
/
```
```python
255
```
```python
# Convert class vectors to binary class matrices.
```
```python
y_train
```
```python
=
```
```python
keras
```
```python
.
```
```python
utils
```
```python
.
```
```python
to_categorical
```
```python
(
```
```python
y_train
```
```python
,
```
```python
num_classes
```
```python
)
```
```python
y_test
```
```python
=
```
```python
keras
```
```python
.
```
```python
utils
```
```python
.
```
```python
to_categorical
```
```python
(
```
```python
y_test
```
```python
,
```
```python
num_classes
```
```python
)
```
来看看上面两行做了啥
```python
print
```
```python
(
```
```python
'shape: '
```
```python
,
```
```python
y_train
```
```python
.
```
```python
shape
```
```python
)
```
`(50000, 10)`原来的标签是一个数，转化后变成了 10 维的 onehot 向量！
#### CNN模型搭建
```python
model
```
```python
=
```
```python
Sequential
```
```python
(
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Conv2D
```
```python
(
```
```python
32
```
```python
,
```
```python
(
```
```python
3
```
```python
,
```
```python
3
```
```python
)
```
```python
,
```
```python
padding
```
```python
=
```
```python
'same'
```
```python
,
```
```python
input_shape
```
```python
=
```
```python
x_train
```
```python
.
```
```python
shape
```
```python
[
```
```python
1
```
```python
:
```
```python
]
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Activation
```
```python
(
```
```python
'relu'
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
MaxPooling2D
```
```python
(
```
```python
pool_size
```
```python
=
```
```python
(
```
```python
2
```
```python
,
```
```python
2
```
```python
)
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Dropout
```
```python
(
```
```python
0.25
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Conv2D
```
```python
(
```
```python
64
```
```python
,
```
```python
(
```
```python
3
```
```python
,
```
```python
3
```
```python
)
```
```python
,
```
```python
padding
```
```python
=
```
```python
'same'
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Activation
```
```python
(
```
```python
'relu'
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
MaxPooling2D
```
```python
(
```
```python
pool_size
```
```python
=
```
```python
(
```
```python
2
```
```python
,
```
```python
2
```
```python
)
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Dropout
```
```python
(
```
```python
0.25
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Flatten
```
```python
(
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Dense
```
```python
(
```
```python
512
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Activation
```
```python
(
```
```python
'relu'
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Dropout
```
```python
(
```
```python
0.5
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Dense
```
```python
(
```
```python
num_classes
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
add
```
```python
(
```
```python
Activation
```
```python
(
```
```python
'softmax'
```
```python
)
```
```python
)
```
```python
model
```
```python
.
```
```python
summary
```
```python
(
```
```python
)
```
```python
# initiate RMSprop optimizer
```
```python
opt
```
```python
=
```
```python
keras
```
```python
.
```
```python
optimizers
```
```python
.
```
```python
rmsprop
```
```python
(
```
```python
lr
```
```python
=
```
```python
0.001
```
```python
,
```
```python
decay
```
```python
=
```
```python
1e
```
```python
-
```
```python
6
```
```python
)
```
```python
# train the model using RMSprop
```
```python
model
```
```python
.
```
```python
compile
```
```python
(
```
```python
loss
```
```python
=
```
```python
'categorical_crossentropy'
```
```python
,
```
```python
optimizer
```
```python
=
```
```python
opt
```
```python
,
```
```python
metrics
```
```python
=
```
```python
[
```
```python
'accuracy'
```
```python
]
```
```python
)
```
上面就是简单地搭积木，选一个优化器
来看看 summary, 总共有 2 million+ 待优化参数！
`_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
conv2d_1 (Conv2D)            (None, 32, 32, 32)        896       
_________________________________________________________________
activation_1 (Activation)    (None, 32, 32, 32)        0         
_________________________________________________________________
max_pooling2d_1 (MaxPooling2 (None, 16, 16, 32)        0         
_________________________________________________________________
dropout_1 (Dropout)          (None, 16, 16, 32)        0         
_________________________________________________________________
conv2d_2 (Conv2D)            (None, 16, 16, 64)        18496     
_________________________________________________________________
activation_2 (Activation)    (None, 16, 16, 64)        0         
_________________________________________________________________
max_pooling2d_2 (MaxPooling2 (None, 8, 8, 64)          0         
_________________________________________________________________
dropout_2 (Dropout)          (None, 8, 8, 64)          0         
_________________________________________________________________
flatten_1 (Flatten)          (None, 4096)              0         
_________________________________________________________________
dense_1 (Dense)              (None, 512)               2097664   
_________________________________________________________________
activation_3 (Activation)    (None, 512)               0         
_________________________________________________________________
dropout_3 (Dropout)          (None, 512)               0         
_________________________________________________________________
dense_2 (Dense)              (None, 10)                5130      
_________________________________________________________________
activation_4 (Activation)    (None, 10)                0         
=================================================================
Total params: 2,122,186
Trainable params: 2,122,186
Non-trainable params: 0
_________________________________________________________________`
#### 训练模型
用 40 个 epoch 训练
```python
hist
```
```python
=
```
```python
model
```
```python
.
```
```python
fit
```
```python
(
```
```python
x_train
```
```python
,
```
```python
y_train
```
```python
,
```
```python
epochs
```
```python
=
```
```python
40
```
```python
,
```
```python
shuffle
```
```python
=
```
```python
True
```
```python
)
```
```python
model
```
```python
.
```
```python
save
```
```python
(
```
```python
model_name
```
```python
)
```
#### 测试模型
```python
# evaluate
```
```python
loss
```
```python
,
```
```python
accuracy
```
```python
=
```
```python
model
```
```python
.
```
```python
evaluate
```
```python
(
```
```python
x_test
```
```python
,
```
```python
y_test
```
```python
)
```
```python
print
```
```python
(
```
```python
loss
```
```python
,
```
```python
accuracy
```
```python
)
```
看看全部输出（暴露渣机器）：
`Using TensorFlow backend.
type:  <class 'numpy.ndarray'> <class 'numpy.ndarray'>
shape:  (50000, 32, 32, 3) (50000, 1)
shape:  (50000, 10)
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
conv2d_1 (Conv2D)            (None, 32, 32, 32)        896       
_________________________________________________________________
activation_1 (Activation)    (None, 32, 32, 32)        0         
_________________________________________________________________
max_pooling2d_1 (MaxPooling2 (None, 16, 16, 32)        0         
_________________________________________________________________
dropout_1 (Dropout)          (None, 16, 16, 32)        0         
_________________________________________________________________
conv2d_2 (Conv2D)            (None, 16, 16, 64)        18496     
_________________________________________________________________
activation_2 (Activation)    (None, 16, 16, 64)        0         
_________________________________________________________________
max_pooling2d_2 (MaxPooling2 (None, 8, 8, 64)          0         
_________________________________________________________________
dropout_2 (Dropout)          (None, 8, 8, 64)          0         
_________________________________________________________________
flatten_1 (Flatten)          (None, 4096)              0         
_________________________________________________________________
dense_1 (Dense)              (None, 512)               2097664   
_________________________________________________________________
activation_3 (Activation)    (None, 512)               0         
_________________________________________________________________
dropout_3 (Dropout)          (None, 512)               0         
_________________________________________________________________
dense_2 (Dense)              (None, 10)                5130      
_________________________________________________________________
activation_4 (Activation)    (None, 10)                0         
=================================================================
Total params: 2,122,186
Trainable params: 2,122,186
Non-trainable params: 0
_________________________________________________________________
Epoch 1/40
2019-03-29 14:32:54.452991: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\platform\cpu_feature_guard.cc:141] Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX AVX2
2019-03-29 14:32:55.125500: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:1392] Found device 0 with properties: 
name: GeForce MX150 major: 6 minor: 1 memoryClockRate(GHz): 1.5315
pciBusID: 0000:01:00.0
totalMemory: 2.00GiB freeMemory: 1.62GiB
2019-03-29 14:32:55.125860: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:1471] Adding visible gpu devices: 0
2019-03-29 14:32:55.923603: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:952] Device interconnect StreamExecutor with strength 1 edge matrix:
2019-03-29 14:32:55.923860: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:958]      0 
2019-03-29 14:32:55.924038: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:971] 0:   N 
2019-03-29 14:32:55.924334: I C:\users\nwani\_bazel_nwani\ujdkfsks\execroot\org_tensorflow\tensorflow\core\common_runtime\gpu\gpu_device.cc:1084] Created TensorFlow device (/job:localhost/replica:0/task:0/device:GPU:0 with 1377 MB memory) -> physical GPU (device: 0, name: GeForce MX150, pci bus id: 0000:01:00.0, compute capability: 6.1)
   32/50000 [..............................] - ETA: 1:21:01 - loss: 2.3407 - acc: 0.0312
  160/50000 [..............................] - ETA: 16:34 - loss: 2.9253 - acc: 0.0688  
  288/50000 [..............................] - ETA: 9:21 - loss: 2.6647 - acc: 0.0868 
  384/50000 [..............................] - ETA: 7:08 - loss: 2.5766 - acc: 0.1016
  512/50000 [..............................] - ETA: 5:26 - loss: 2.5025 - acc: 0.1113
  608/50000 [..............................] - ETA: 4:39 - loss: 2.4698 - acc: 0.1184
  736/50000 [..............................] - ETA: 3:54 - loss: 2.4335 - acc: 0.1318
  832/50000 [..............................] - ETA: 3:30 - loss: 2.4112 - acc: 0.1322
  960/50000 [..............................] - ETA: 3:05 - loss: 2.3853 - acc: 0.1427
 1056/50000 [..............................] - ETA: 2:51 - loss: 2.3720 - acc: 0.1402
 1184/50000 [..............................] - ETA: 2:34 - loss: 2.3524 - acc: 0.1402
 1312/50000 [..............................] - ETA: 2:22 - loss: 2.3316 - acc: 0.1418
 1440/50000 [..............................] - ETA: 2:11 - loss: 2.3255 - acc: 0.1431
 1536/50000 [..............................] - ETA: 2:04 - loss: 2.3141 - acc: 0.1432
 1664/50000 [..............................] - ETA: 1:56 - loss: 2.3062 - acc: 0.1436
 1792/50000 [>.............................] - ETA: 1:50 - loss: 2.2897 - acc: 0.1557
 1920/50000 [>.............................] - ETA: 1:44 - loss: 2.2841 - acc: 0.1568
 2016/50000 [>.............................] - ETA: 1:40 - loss: 2.2727 - acc: 0.1602
 2144/50000 [>.............................] - ETA: 1:35 - loss: 2.2627 - acc: 0.1651
 ...
46656/50000 [==========================>...] - ETA: 2s - loss: 1.5545 - acc: 0.4408
46784/50000 [===========================>..] - ETA: 2s - loss: 1.5538 - acc: 0.4410
46880/50000 [===========================>..] - ETA: 1s - loss: 1.5532 - acc: 0.4413
47008/50000 [===========================>..] - ETA: 1s - loss: 1.5527 - acc: 0.4415
47104/50000 [===========================>..] - ETA: 1s - loss: 1.5520 - acc: 0.4416
47232/50000 [===========================>..] - ETA: 1s - loss: 1.5516 - acc: 0.4418
47360/50000 [===========================>..] - ETA: 1s - loss: 1.5509 - acc: 0.4420
47488/50000 [===========================>..] - ETA: 1s - loss: 1.5504 - acc: 0.4422
47616/50000 [===========================>..] - ETA: 1s - loss: 1.5502 - acc: 0.4423
47744/50000 [===========================>..] - ETA: 1s - loss: 1.5499 - acc: 0.4425
47840/50000 [===========================>..] - ETA: 1s - loss: 1.5490 - acc: 0.4428
47968/50000 [===========================>..] - ETA: 1s - loss: 1.5484 - acc: 0.4429
48096/50000 [===========================>..] - ETA: 1s - loss: 1.5480 - acc: 0.4432
48224/50000 [===========================>..] - ETA: 1s - loss: 1.5470 - acc: 0.4436
48352/50000 [============================>.] - ETA: 1s - loss: 1.5462 - acc: 0.4439
48480/50000 [============================>.] - ETA: 0s - loss: 1.5461 - acc: 0.4440
48608/50000 [============================>.] - ETA: 0s - loss: 1.5458 - acc: 0.4442
48736/50000 [============================>.] - ETA: 0s - loss: 1.5443 - acc: 0.4446
48832/50000 [============================>.] - ETA: 0s - loss: 1.5440 - acc: 0.4447
48960/50000 [============================>.] - ETA: 0s - loss: 1.5429 - acc: 0.4451
49088/50000 [============================>.] - ETA: 0s - loss: 1.5424 - acc: 0.4453
49216/50000 [============================>.] - ETA: 0s - loss: 1.5414 - acc: 0.4458
49312/50000 [============================>.] - ETA: 0s - loss: 1.5409 - acc: 0.4460
49440/50000 [============================>.] - ETA: 0s - loss: 1.5399 - acc: 0.4464
49568/50000 [============================>.] - ETA: 0s - loss: 1.5394 - acc: 0.4466
49696/50000 [============================>.] - ETA: 0s - loss: 1.5391 - acc: 0.4469
49824/50000 [============================>.] - ETA: 0s - loss: 1.5383 - acc: 0.4472
49952/50000 [============================>.] - ETA: 0s - loss: 1.5377 - acc: 0.4474
50000/50000 [==============================] - 32s 632us/step - loss: 1.5375 - acc: 0.4476`

