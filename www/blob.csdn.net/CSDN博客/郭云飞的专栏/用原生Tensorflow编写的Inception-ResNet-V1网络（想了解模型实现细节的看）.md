
# 用原生Tensorflow编写的Inception-ResNet-V1网络（想了解模型实现细节的看） - 郭云飞的专栏 - CSDN博客


2018年08月15日 18:40:18[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：459


```python
# prepare input data
IMAGE_HEI = 128
IMAGE_WID = 128
IMAGE_CHA = 3
CLASS_NUM = 2
X = tf.placeholder(tf.float32, [None, IMAGE_HEI, IMAGE_WID, IMAGE_CHA])
Y_= tf.placeholder(tf.float32, [None, CLASS_NUM])
keep_prob = tf.placeholder(tf.float32)
lr = tf.placeholder(tf.float32) # learning rate
```
下边的代码片段实现了Inception-ResNet-V1网络的：Stem，Inception-resnet-A，Reduction-A，Inception-resnet-B，Reduction-B，Inception-resnet-C五个模块。
`#---------- Stem Block ------------------------------------------#
#           Input layer                                         out: 128 * 128 *   3
#               |
#           3*3 conv 32                                         out: 128 * 128 *  32
#               |
#           3*3 conv 32, stride=2 (we use max_pool instead)     out:  64 *  64 *  32
#               |
#           3*3 conv 64                                         out:  64 *  64 *  64
#               |
#           3*3 conv 64, stride=2 (we use max_pool instead)     out:  32 *  32 *  64
#               |
#           3*3 conv 128                                        out:  32 *  32 * 128
#               |
#           3*3 conv 128,stride=2 (we use max_pool instead)     out:  16 *  16 * 128
#               |
#           relu (output 16*16*128)                             out:  16 *  16 * 128
stem_conv1_k = 32
stem_conv1_w = tf.Variable(tf.truncated_normal([3, 3,    IMAGE_CHA, stem_conv1_k], stddev = 0.1))
stem_conv1_b = tf.Variable(tf.ones([stem_conv1_k]) / 10)
stem_conv1_o = tf.nn.conv2d(           X, stem_conv1_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv1_o = stem_conv1_o + stem_conv1_b;
# out: 64*64*32
stem_conv2_k = 32
stem_conv2_w = tf.Variable(tf.truncated_normal([3, 3, stem_conv1_k, stem_conv2_k], stddev = 0.1))
stem_conv2_b = tf.Variable(tf.ones([stem_conv2_k]) / 10)
stem_conv2_o = tf.nn.conv2d(stem_conv1_o, stem_conv2_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv2_o = stem_conv2_o + stem_conv2_b;
stem_conv2_o = tf.nn.max_pool(stem_conv2_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# out: 64*64*64
stem_conv3_k = 64
stem_conv3_w = tf.Variable(tf.truncated_normal([3, 3, stem_conv2_k, stem_conv3_k], stddev = 0.1))
stem_conv3_b = tf.Variable(tf.ones([stem_conv3_k]) / 10)
stem_conv3_o = tf.nn.conv2d(stem_conv2_o, stem_conv3_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv3_o = stem_conv3_o + stem_conv3_b;
#  32*32*64
stem_conv4_k = 64
stem_conv4_w = tf.Variable(tf.truncated_normal([3, 3, stem_conv3_k, stem_conv4_k], stddev = 0.1))
stem_conv4_b = tf.Variable(tf.ones([stem_conv4_k]) / 10)
stem_conv4_o = tf.nn.conv2d(stem_conv3_o, stem_conv4_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv4_o = stem_conv4_o + stem_conv4_b;
stem_conv4_o = tf.nn.max_pool(stem_conv4_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# In 32*32*64, Out 32*32*128
stem_conv5_k = 128
stem_conv5_w = tf.Variable(tf.truncated_normal([3, 3, stem_conv4_k, stem_conv5_k], stddev = 0.1))
stem_conv5_b = tf.Variable(tf.ones([stem_conv5_k]) / 10)
stem_conv5_o = tf.nn.conv2d(stem_conv4_o, stem_conv5_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv5_o = stem_conv5_o + stem_conv5_b;
# In 32*32*128, Out 16*16*128
stem_conv6_k = 128
stem_conv6_w = tf.Variable(tf.truncated_normal([3, 3, stem_conv5_k, stem_conv6_k], stddev = 0.1))
stem_conv6_b = tf.Variable(tf.ones([stem_conv6_k]) / 10)
stem_conv6_o = tf.nn.conv2d(stem_conv5_o, stem_conv6_w, strides = [1, 1, 1, 1], padding = 'SAME')
stem_conv6_o = stem_conv6_o + stem_conv6_b;
stem_conv6_o = tf.nn.max_pool(stem_conv6_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
stem_conv6_o = tf.nn.relu(stem_conv6_o)
#---------- Inception-ResNet-A 1 --------------------------------#
# We only use 1 Inception-ResNet-A block. There are 5 Inception-ResNet-A 
# blocks in origin paper.
#
#           previous layer (from stem block, input 16*16*128) 
#                   |
#            _______|_____________________________________________________
#           /                         |                      |            \
#    1*1 conv 32                1*1 conv 32            1*1 conv 32        |
#           |                         |                      |            |
#           |                   3*3 conv 32            3*3 conv 32        |
#           |                         |                      |            |
#           |                         |                3*3 conv 32        |
#           \_________________________C______________________/            |
#                                     |                                   |
#                                     | three signals concat              |
#                                     |                                   |
#                                1*1 conv 128                             |
#                                     \_________________+_________________/
#                                                       |
#                                                       | two signal add (must have the same shape)
#                                                       |
#                                             relu(output 16*16*128)
# Through the graph above, we know the input and output data have the same shape.
# path1
ia1_conv11_k = 32
ia1_conv11_w = tf.Variable(tf.truncated_normal([1, 1, stem_conv6_k, ia1_conv11_k], stddev = 0.1))
ia1_conv11_b = tf.Variable(tf.ones([ia1_conv11_k]) / 10)
ia1_conv11_o = tf.nn.conv2d(stem_conv6_o, ia1_conv11_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv11_o = ia1_conv11_o + ia1_conv11_b;
#path 2
ia1_conv21_k = 32
ia1_conv21_w = tf.Variable(tf.truncated_normal([1, 1, stem_conv6_k, ia1_conv21_k], stddev = 0.1))
ia1_conv21_b = tf.Variable(tf.ones([ia1_conv21_k]) / 10)
ia1_conv21_o = tf.nn.conv2d(stem_conv6_o, ia1_conv21_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv21_o = ia1_conv21_o + ia1_conv21_b;
ia1_conv22_k = 32
ia1_conv22_w = tf.Variable(tf.truncated_normal([3, 3, ia1_conv21_k, ia1_conv22_k], stddev = 0.1))
ia1_conv22_b = tf.Variable(tf.ones([ia1_conv22_k]) / 10)
ia1_conv22_o = tf.nn.conv2d(ia1_conv21_o, ia1_conv22_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv22_o = ia1_conv22_o + ia1_conv22_b;
# path 3
ia1_conv31_k = 32
ia1_conv31_w = tf.Variable(tf.truncated_normal([1, 1, stem_conv6_k, ia1_conv31_k], stddev = 0.1))
ia1_conv31_b = tf.Variable(tf.ones([ia1_conv31_k]) / 10)
ia1_conv31_o = tf.nn.conv2d(stem_conv6_o, ia1_conv31_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv31_o = ia1_conv31_o + ia1_conv31_b;
ia1_conv32_k = 32
ia1_conv32_w = tf.Variable(tf.truncated_normal([3, 3, ia1_conv31_k, ia1_conv32_k], stddev = 0.1))
ia1_conv32_b = tf.Variable(tf.ones([ia1_conv32_k]) / 10)
ia1_conv32_o = tf.nn.conv2d(ia1_conv31_o, ia1_conv32_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv32_o = ia1_conv32_o + ia1_conv32_b;
ia1_conv33_k = 32
ia1_conv33_w = tf.Variable(tf.truncated_normal([3, 3, ia1_conv32_k, ia1_conv33_k], stddev = 0.1))
ia1_conv33_b = tf.Variable(tf.ones([ia1_conv33_k]) / 10)
ia1_conv33_o = tf.nn.conv2d(ia1_conv32_o, ia1_conv33_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv33_o = ia1_conv33_o + ia1_conv33_b;
# concat path1 - 3
ia1_concat_i = tf.concat(axis=3, values=[ia1_conv11_o, ia1_conv22_o, ia1_conv33_o])
ia1_conv41_i = ia1_conv11_k + ia1_conv22_k + ia1_conv33_k
# conv after concat
ia1_conv41_k = 128
ia1_conv41_w = tf.Variable(tf.truncated_normal([1, 1, ia1_conv41_i, ia1_conv41_k], stddev = 0.1))
ia1_conv41_b = tf.Variable(tf.ones([ia1_conv41_k]) / 10)
ia1_conv41_o = tf.nn.conv2d(ia1_concat_i, ia1_conv41_w, strides = [1, 1, 1, 1], padding = 'SAME')
ia1_conv41_o = ia1_conv41_o + ia1_conv41_b;
# add signals and relu
ia1_conv41_o = tf.nn.relu(stem_conv6_o + ia1_conv41_o)
#---------- Reduction-A -----------------------------------------#
#           previous layer (input 16*16*128) 
#                   |
#            _______|___________________________________
#           /                         |                 \
#   2*2 max pooling        3*3 conv 64, stride=2     1*1 conv 32
#           |                         |                 |
#           |                         |              3*3 conv 32
#           |                         |                 |
#           |                         |              3*3 conv 64, stride=2
#           |_________________________C_________________|
#                                     |
#                                     | threee signal concat
#                                     |
#                           relu (output 8*8*256)
# Through the graph above, we can take this whole block as a pooling layer.
# path 1
ra1_pool11_o = tf.nn.max_pool(ia1_conv41_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# path 2
ra1_conv11_k = 64
ra1_conv11_w = tf.Variable(tf.truncated_normal([3, 3, ia1_conv41_k, ra1_conv11_k], stddev = 0.1))
ra1_conv11_b = tf.Variable(tf.ones([ra1_conv11_k]) / 10)
ra1_conv11_o = tf.nn.conv2d(ia1_conv41_o, ra1_conv11_w, strides = [1, 1, 1, 1], padding = 'SAME')
ra1_conv11_o = ra1_conv11_o + ra1_conv11_b;
ra1_conv11_o = tf.nn.max_pool(ra1_conv11_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# path 3
ra1_conv21_k = 32
ra1_conv21_w = tf.Variable(tf.truncated_normal([1, 1, ia1_conv41_k, ra1_conv21_k], stddev = 0.1))
ra1_conv21_b = tf.Variable(tf.ones([ra1_conv21_k]) / 10)
ra1_conv21_o = tf.nn.conv2d(ia1_conv41_o, ra1_conv21_w, strides = [1, 1, 1, 1], padding = 'SAME')
ra1_conv21_o = ra1_conv21_o + ra1_conv21_b;
ra1_conv22_k = 32
ra1_conv22_w = tf.Variable(tf.truncated_normal([3, 3, ra1_conv21_k, ra1_conv22_k], stddev = 0.1))
ra1_conv22_b = tf.Variable(tf.ones([ra1_conv22_k]) / 10)
ra1_conv22_o = tf.nn.conv2d(ra1_conv21_o, ra1_conv22_w, strides = [1, 1, 1, 1], padding = 'SAME')
ra1_conv22_o = ra1_conv22_o + ra1_conv22_b;
ra1_conv23_k = 64
ra1_conv23_w = tf.Variable(tf.truncated_normal([3, 3, ra1_conv22_k, ra1_conv23_k], stddev = 0.1))
ra1_conv23_b = tf.Variable(tf.ones([ra1_conv23_k]) / 10)
ra1_conv23_o = tf.nn.conv2d(ra1_conv22_o, ra1_conv23_w, strides = [1, 1, 1, 1], padding = 'SAME')
ra1_conv23_o = ra1_conv23_o + ra1_conv23_b;
ra1_conv23_o = tf.nn.max_pool(ra1_conv23_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# Out 8*8*256
ra1_concat_o = tf.concat(axis=3, values=[ra1_pool11_o, ra1_conv11_o, ra1_conv23_o])
# relu
ra1_concat_o = tf.nn.relu(ra1_concat_o)
#---------- Inception-ResNet-B 1 --------------------------------#
# We only use 1 Inception-ResNet-A block. There are 10 Inception-ResNet-A block in origin paper.
#           previous layer (input 8*8*256) 
#                   |
#            ___2___|______1____________________3_______
#           /                         |                 \
#       1*1 conv 128            1*1 conv 128            |
#               |                     |                 |
#       1*5 conv 128                  |                 |
#               |                     |                 |
#       5*1 conv 128                  |                 |
#               |                     |                 |
#               |_______concat________|                 |
#                          |                            |
#                     1*1 conv 256                      |
#                          |                            |
#                          |______________+_____________|
#                                         |
#                                         | two signal add
#                                         |
#                               relu (output 8*8*256)
# path 1
ib1_conv11_i = 256
ib1_conv11_k = 128
ib1_conv11_w = tf.Variable(tf.truncated_normal([1, 1, ib1_conv11_i, ib1_conv11_k], stddev = 0.1))
ib1_conv11_b = tf.Variable(tf.ones([ib1_conv11_k]) / 10)
ib1_conv11_o = tf.nn.conv2d(ra1_concat_o, ib1_conv11_w, strides = [1, 1, 1, 1], padding = 'SAME')
ib1_conv11_o = ib1_conv11_o + ib1_conv11_b;
# path 2
ib1_conv21_k = 128
ib1_conv21_w = tf.Variable(tf.truncated_normal([1, 1, ib1_conv11_i, ib1_conv21_k], stddev = 0.1))
ib1_conv21_b = tf.Variable(tf.ones([ib1_conv21_k]) / 10)
ib1_conv21_o = tf.nn.conv2d(ra1_concat_o, ib1_conv21_w, strides = [1, 1, 1, 1], padding = 'SAME')
ib1_conv21_o = ib1_conv21_o + ib1_conv21_b;
ib1_conv22_k = 128
ib1_conv22_w = tf.Variable(tf.truncated_normal([1, 5, ib1_conv21_k, ib1_conv22_k], stddev = 0.1))
ib1_conv22_b = tf.Variable(tf.ones([ib1_conv22_k]) / 10)
ib1_conv22_o = tf.nn.conv2d(ib1_conv21_o, ib1_conv22_w, strides = [1, 1, 1, 1], padding = 'SAME')
ib1_conv22_o = ib1_conv22_o + ib1_conv22_b;
ib1_conv23_k = 128
ib1_conv23_w = tf.Variable(tf.truncated_normal([5, 1, ib1_conv22_k, ib1_conv23_k], stddev = 0.1))
ib1_conv23_b = tf.Variable(tf.ones([ib1_conv23_k]) / 10)
ib1_conv23_o = tf.nn.conv2d(ib1_conv22_o, ib1_conv23_w, strides = [1, 1, 1, 1], padding = 'SAME')
ib1_conv23_o = ib1_conv23_o + ib1_conv23_b;
# concat path 1 - 2
ib1_concat_i = tf.concat(axis=3, values=[ib1_conv11_o, ib1_conv23_o])
# the conv after concat
ib1_conv31_i = ib1_conv11_k + ib1_conv23_k
ib1_conv31_k = 256
ib1_conv31_w = tf.Variable(tf.truncated_normal([1, 1, ib1_conv31_i, ib1_conv31_k], stddev = 0.1))
ib1_conv31_b = tf.Variable(tf.ones([ib1_conv31_k]) / 10)
ib1_conv31_o = tf.nn.conv2d(ib1_concat_i, ib1_conv31_w, strides = [1, 1, 1, 1], padding = 'SAME')
ib1_conv31_o = ib1_conv31_o + ib1_conv31_b;
# signal add
ib1_conv31_o = tf.nn.relu(ra1_concat_o + ib1_conv31_o)
#---------- Reduction-B -----------------------------------------#
#           previous layer (input 8*8*256) 
#                   |
#            _______|________________________________________________________
#           /                         |                      |               \
#    2*2 max pooling            1*1 conv 64             1*1 conv 64      1*1 conv 64
#           |                         |                      |               |
#                         3*3 conv 64,stride=2    3*3 conv 64,stride=2   3*3 conv 64
#           |                         |                      |               |
#           |                         |                      |           3*3 conv 128,stride=2
#           |                         |                      |               |
#           \_________________________|______________________|_______________/
#                                     C
#                                     | four signals concat
#                                     |
#                           relu (output 4*4*512)
# path 1
rb1_pool11_o = tf.nn.max_pool(ib1_conv31_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
rb1_conv11_i = 256
rb1_conv11_k = 64
rb1_conv11_w = tf.Variable(tf.truncated_normal([1, 1, rb1_conv11_i, rb1_conv11_k], stddev = 0.1))
rb1_conv11_b = tf.Variable(tf.ones([rb1_conv11_k]) / 10)
rb1_conv11_o = tf.nn.conv2d(ib1_conv31_o, rb1_conv11_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv11_o = rb1_conv11_o + rb1_conv11_b;
rb1_conv12_k = 64
rb1_conv12_w = tf.Variable(tf.truncated_normal([3, 3, rb1_conv11_k, rb1_conv12_k], stddev = 0.1))
rb1_conv12_b = tf.Variable(tf.ones([rb1_conv12_k]) / 10)
rb1_conv12_o = tf.nn.conv2d(rb1_conv11_o, rb1_conv12_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv12_o = rb1_conv12_o + rb1_conv12_b;
rb1_conv12_o = tf.nn.max_pool(rb1_conv12_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# path 2
rb1_conv21_k = 64
rb1_conv21_w = tf.Variable(tf.truncated_normal([1, 1, rb1_conv11_i, rb1_conv21_k], stddev = 0.1))
rb1_conv21_b = tf.Variable(tf.ones([rb1_conv21_k]) / 10)
rb1_conv21_o = tf.nn.conv2d(ib1_conv31_o, rb1_conv21_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv21_o = rb1_conv21_o + rb1_conv21_b;
rb1_conv22_k = 64
rb1_conv22_w = tf.Variable(tf.truncated_normal([3, 3, rb1_conv21_k, rb1_conv22_k], stddev = 0.1))
rb1_conv22_b = tf.Variable(tf.ones([rb1_conv22_k]) / 10)
rb1_conv22_o = tf.nn.conv2d(rb1_conv21_o, rb1_conv22_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv22_o = rb1_conv22_o + rb1_conv22_b;
rb1_conv22_o = tf.nn.max_pool(rb1_conv22_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# path 3
rb1_conv31_k = 64
rb1_conv31_w = tf.Variable(tf.truncated_normal([1, 1, rb1_conv11_i, rb1_conv31_k], stddev = 0.1))
rb1_conv31_b = tf.Variable(tf.ones([rb1_conv31_k]) / 10)
rb1_conv31_o = tf.nn.conv2d(ib1_conv31_o, rb1_conv31_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv31_o = rb1_conv31_o + rb1_conv31_b;
rb1_conv32_k = 64
rb1_conv32_w = tf.Variable(tf.truncated_normal([3, 3, rb1_conv31_k, rb1_conv32_k], stddev = 0.1))
rb1_conv32_b = tf.Variable(tf.ones([rb1_conv32_k]) / 10)
rb1_conv32_o = tf.nn.conv2d(rb1_conv31_o, rb1_conv32_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv32_o = rb1_conv32_o + rb1_conv32_b;
rb1_conv33_k = 128
rb1_conv33_w = tf.Variable(tf.truncated_normal([3, 3, rb1_conv32_k, rb1_conv33_k], stddev = 0.1))
rb1_conv33_b = tf.Variable(tf.ones([rb1_conv33_k]) / 10)
rb1_conv33_o = tf.nn.conv2d(rb1_conv32_o, rb1_conv33_w, strides = [1, 1, 1, 1], padding = 'SAME')
rb1_conv33_o = rb1_conv33_o + rb1_conv33_b;
rb1_conv33_o = tf.nn.max_pool(rb1_conv33_o, ksize= [1,2,2,1], strides= [1,2,2,1], padding='SAME')
# concat
rb1_concat_o = tf.concat(axis=3, values=[rb1_pool11_o, rb1_conv12_o, rb1_conv22_o, rb1_conv33_o])
rb1_concat_o = tf.nn.relu(rb1_concat_o)
#---------- Inception-ResNet-C 1 --------------------------------#
# We only use 1 Inception-ResNet-A block. There are 5 Inception-ResNet-A block in origin paper.
#           previous layer (input 4*4*512) 
#                   |
#            ___2___|______1____________________3_______
#           /                         |                 \
#       1*1 conv 256            1*1 conv 256            |
#               |                     |                 |
#       1*3 conv 256                  |                 |
#               |                     |                 |
#       3*1 conv 256                  |                 |
#               |                     |                 |
#               |_______concat________|                 |
#                          |                            |
#                     1*1 conv 512                      |
#                          |                            |
#                          |______________+_____________|
#                                         |
#                                         | two signal add
#                                         |
#                               relu (output 4*4*512)
# path 1
ic1_conv11_i = 512
ic1_conv11_k = 256
ic1_conv11_w = tf.Variable(tf.truncated_normal([1, 1, ic1_conv11_i, ic1_conv11_k], stddev = 0.1))
ic1_conv11_b = tf.Variable(tf.ones([ic1_conv11_k]) / 10)
ic1_conv11_o = tf.nn.conv2d(rb1_concat_o, ic1_conv11_w, strides = [1, 1, 1, 1], padding = 'SAME')
ic1_conv11_o = ic1_conv11_o + ic1_conv11_b;
# path 2
ic1_conv21_k = 256
ic1_conv21_w = tf.Variable(tf.truncated_normal([1, 1, ic1_conv11_i, ic1_conv21_k], stddev = 0.1))
ic1_conv21_b = tf.Variable(tf.ones([ic1_conv21_k]) / 10)
ic1_conv21_o = tf.nn.conv2d(rb1_concat_o, ic1_conv21_w, strides = [1, 1, 1, 1], padding = 'SAME')
ic1_conv21_o = ic1_conv21_o + ic1_conv21_b;
ic1_conv22_k = 256
ic1_conv22_w = tf.Variable(tf.truncated_normal([1, 3, ic1_conv21_k, ic1_conv22_k], stddev = 0.1))
ic1_conv22_b = tf.Variable(tf.ones([ic1_conv22_k]) / 10)
ic1_conv22_o = tf.nn.conv2d(ic1_conv21_o, ic1_conv22_w, strides = [1, 1, 1, 1], padding = 'SAME')
ic1_conv22_o = ic1_conv22_o + ic1_conv22_b;
ic1_conv23_k = 256
ic1_conv23_w = tf.Variable(tf.truncated_normal([3, 1, ic1_conv22_k, ic1_conv23_k], stddev = 0.1))
ic1_conv23_b = tf.Variable(tf.ones([ic1_conv23_k]) / 10)
ic1_conv23_o = tf.nn.conv2d(ic1_conv22_o, ic1_conv23_w, strides = [1, 1, 1, 1], padding = 'SAME')
ic1_conv23_o = ic1_conv23_o + ic1_conv23_b;
# concat
ic1_concat_i = tf.concat(axis=3, values=[ic1_conv11_o, ic1_conv23_o])
ic1_conv31_i = ic1_conv11_k + ic1_conv23_k
# conv after concat
ic1_conv31_k = 512
ic1_conv31_w = tf.Variable(tf.truncated_normal([1, 1, ic1_conv31_i, ic1_conv31_k], stddev = 0.1))
ic1_conv31_b = tf.Variable(tf.ones([ic1_conv31_k]) / 10)
ic1_conv31_o = tf.nn.conv2d(ic1_concat_i, ic1_conv31_w, strides = [1, 1, 1, 1], padding = 'SAME')
ic1_conv31_o = ic1_conv31_o + ic1_conv31_b;
# signal add and relu
ic1_conv31_o = tf.nn.relu(rb1_concat_o + ic1_conv31_o)`对于Inception-ResNet-V1网络的Average Pooling，Dropout，Softmax，我用了三个full connection来替代。
```python
#---------- Average Pooling / Dropout / Softmax -----------------#
# here we use full connection layers instead the left 3 models in Incpetion-ResNet-V1
# define the number of neruals in full connection layers
Full1_Num = 1024
Full2_Num = 1024
Full3_Num = CLASS_NUM
# full connection 1 w and b
Full1W = tf.Variable(tf.truncated_normal([4*4*ic1_conv31_k, Full1_Num], stddev=0.1))
Full1B = tf.Variable(tf.ones([Full1_Num]) / 10)
# full connection 2 w and b
Full2W = tf.Variable(tf.truncated_normal([Full1_Num, Full2_Num], stddev=0.1))
Full2B = tf.Variable(tf.ones([Full2_Num]) / 10)
# full connection 3 w and b
Full3W = tf.Variable(tf.truncated_normal([Full2_Num, Full3_Num], stddev=0.1))
Full3B = tf.Variable(tf.ones([Full3_Num]) / 10)
# full connection
fc1 = tf.reshape(ic1_conv31_o, shape=[-1, 4*4*ic1_conv31_k])
re1 = tf.nn.relu(tf.matmul(fc1, Full1W) + Full1B)
dp1 = tf.nn.dropout(re1, keep_prob)
re2 = tf.nn.relu(tf.matmul(dp1, Full2W) + Full2B)
dp2 = tf.nn.dropout(re2, keep_prob)
# full connection
Ylogits = tf.matmul(dp2, Full3W) + Full3B
Y = tf.nn.softmax(Ylogits)
# cross entropy
cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=Ylogits, labels=Y_)
cross_entropy = tf.reduce_mean(cross_entropy) * 100
# accuracy of the trained model, between 0 (worst) and 1 (best)
cls_true = tf.argmax(Y_, 1)
cls_eavl = tf.argmax(Y, 1)
correct_prediction = tf.equal(cls_eavl, cls_true)
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
# training step, the learning rate is a placeholder
train_step = tf.train.AdamOptimizer(lr).minimize(cross_entropy)
# init
init = tf.global_variables_initializer()
# save model
Saver = tf.train.Saver(max_to_keep = 1)  # defaults to saving all variables
ModelSavePath = './storage/'
""" computer the learning rate through a number
"""
def calculate_lr(INDEX):
    max_learning_rate = 0.005
    min_learning_rate = 1e-4
    decay_speed = 400
    
    if (INDEX >= 0):
        learning_rate = min_learning_rate + \
                        (max_learning_rate - min_learning_rate) * \
                        math.exp(-INDEX / decay_speed)
        return learning_rate
    else:
        return min_learning_rate
"""一个辅助函数，用来计算一批样本中，分对的比例和分错的比例
"""
def compute_accu(eav, tru):
    N = len(eav)
    correct_num = 0
    false_positive_num = 0
    false_negitive_num = 0
    for i in range(N):
        if (eav[i] == tru[i]):
            correct_num = correct_num + 1
        else:
            if tru[i] == 1:
                false_negitive_num = false_negitive_num + 1
            else:
                false_positive_num = false_positive_num + 1
    return correct_num*1.0 / N, false_positive_num*1.0 / N, false_negitive_num*1.0 / N
```
训练：自己准备样本集
`def count_parameters():
    print ('[INFO ] total parameters: %d'%np.sum([np.prod(v.get_shape().as_list()) for v in tf.trainable_variables()]))
"""训练，用tfrecords格式的数据
"""
def train_use_tfrecords_data():
    with tf.Session() as sess:
        sess.run(init)
        ckpt = tf.train.get_checkpoint_state(ModelSavePath)
        if ckpt and ckpt.model_checkpoint_path:
            Saver.restore(sess, ckpt.model_checkpoint_path)
            print '[INFO ] restore model successfully'
        else:
            print '[INFO ] no model so far'
        count_parameters()
        # prepare train data
        batch_size = 100
        image_tra_cls0, label_tra_cls0 = samples_to_tfrecord.read_tfrecord( \
        ['/home/raintai/z_tf/train0_class0.tfrecords', \
         '/home/raintai/z_tf/train1_class0.tfrecords', \
         '/home/raintai/z_tf/train2_class0.tfrecords', \
         '/home/raintai/z_tf/train3_class0.tfrecords'],\
        batch_size = batch_size)
        image_tra_cls1, label_tra_cls1 = samples_to_tfrecord.read_tfrecord( \
        ['/home/raintai/z_tf/train0_class1.tfrecords', \
         '/home/raintai/z_tf/train1_class1.tfrecords', \
         '/home/raintai/z_tf/train2_class1.tfrecords'],\
        batch_size = batch_size)
        # prepare test data
        image_tes_cls0, label_tes_cls0 = samples_to_tfrecord.read_tfrecord( \
        ['/home/raintai/z_tf/test0_class0.tfrecords', \
         '/home/raintai/z_tf/test1_class0.tfrecords'],\
        batch_size = batch_size)
        image_tes_cls1, label_tes_cls1 = samples_to_tfrecord.read_tfrecord( \
        ['/home/raintai/z_tf/test0_class1.tfrecords', \
         '/home/raintai/z_tf/test1_class1.tfrecords'],\
        batch_size = batch_size)
        # start queue
        threads = tf.train.start_queue_runners(sess=sess)
        # one hot encode
        label_tra_cls0 = tf.one_hot(label_tra_cls0, CLASS_NUM, dtype = tf.int32)
        label_tra_cls1 = tf.one_hot(label_tra_cls1, CLASS_NUM, dtype = tf.int32)
        label_tes_cls0 = tf.one_hot(label_tes_cls0, CLASS_NUM, dtype = tf.int32)
        label_tes_cls1 = tf.one_hot(label_tes_cls1, CLASS_NUM, dtype = tf.int32)
        iterNum = 1000000000L
        for i in xrange(1, iterNum):
            batch_X_tra_cls0, batch_Y_tra_cls0 = sess.run([image_tra_cls0, label_tra_cls0])
            batch_X_tra_cls1, batch_Y_tra_cls1 = sess.run([image_tra_cls1, label_tra_cls1])
            batch_X = np.append(batch_X_tra_cls0, batch_X_tra_cls1, axis = 0)
            batch_Y = np.append(batch_Y_tra_cls0, batch_Y_tra_cls1, axis = 0)
            #print(batch_X.shape, batch_Y.shape)
            cur_lr = calculate_lr(INDEX = i)
            #cur_lr = 1e-4
            eav, tru, cor, a, c, s = sess.run( \
                [cls_eavl, cls_true, correct_prediction, accuracy, cross_entropy, train_step], \
                {X: batch_X, Y_: batch_Y, lr: cur_lr, keep_prob: 0.6})
            correct_num, false_positive_num, false_negitive_num = compute_accu(eav, tru)
            if (i + 1) % 10 == 0:
                saveFg = 0
                if (i + 1) % 100 == 0:
                    # save tensorflow model
                    Saver.save(sess, ModelSavePath + "model", global_step = (i + 1))
                    saveFg = 1
                # test 
                batch_X_tes_cls0, batch_Y_tes_cls0 = sess.run([image_tes_cls0, label_tes_cls0])
                batch_X_tes_cls1, batch_Y_tes_cls1 = sess.run([image_tes_cls1, label_tes_cls1])
                test_batch_x = np.append(batch_X_tes_cls0, batch_X_tes_cls1, axis = 0)
                test_batch_y = np.append(batch_Y_tes_cls0, batch_Y_tes_cls1, axis = 0)
                eav1, tru1, cor1, test_cross, test_accu = sess.run( \
                    [cls_eavl, cls_true, correct_prediction, cross_entropy, accuracy], \
                    {X: test_batch_x, Y_: test_batch_y, keep_prob: 1.0})
                correct_num1, false_positive_num1, false_negitive_num1 = compute_accu(eav1, tru1)
                if not saveFg:
                    print \
                        '%6d'%(i + 1), \
                        '%12.4f(cross) '%c, \
                        '%3.2f(accu) -%3.3f-%3.3f'%(a, false_positive_num, false_negitive_num), \
                        '%9.4f(cross) '%test_cross, \
                        '%3.2f(accu) -%3.3f-%3.3f'%(test_accu, false_positive_num1, false_negitive_num1), \
                        '%10.5f(lr) '%cur_lr
                else:
                    print \
                        '%6d'%(i + 1), \
                        '%12.4f(cross) '%c, \
                        '%3.2f(accu) -%3.3f-%3.3f'%(a, false_positive_num, false_negitive_num), \
                        '%9.4f(cross) '%test_cross, \
                        '%3.2f(accu) -%3.3f-%3.3f'%(test_accu, false_positive_num1, false_negitive_num1), \
                        '%10.5f(lr) '%cur_lr, \
                        'save'
if __name__ == "__main__":
    train_use_tfrecords_data()`


