
# keras获得某一层或者某层权重的输出 - leofionn的博客 - CSDN博客


2018年12月11日 12:01:18[leofionn](https://me.csdn.net/qq_36142114)阅读数：597个人分类：[python																](https://blog.csdn.net/qq_36142114/article/category/7385154)


print("Loading vgg19 weights...")
vgg_model = VGG19(include_top=False, weights='imagenet')
from_vgg = dict()   \# 因为模型定义中的layer的名字与原始vgg名字不同，所以需要调整
from_vgg['conv1_1'] = 'block1_conv1'
from_vgg['conv1_2'] = 'block1_conv2'
from_vgg['conv2_1'] = 'block2_conv1'
from_vgg['conv2_2'] = 'block2_conv2'
from_vgg['conv3_1'] = 'block3_conv1'
from_vgg['conv3_2'] = 'block3_conv2'
from_vgg['conv3_3'] = 'block3_conv3'
from_vgg['conv3_4'] = 'block3_conv4'
from_vgg['conv4_1'] = 'block4_conv1'
from_vgg['conv4_2'] = 'block4_conv2'
for layer in model.layers:
if layer.name in from_vgg:
vgg_layer_name = from_vgg[layer.name]
layer.set_weights(vgg_model.get_layer(vgg_layer_name).get_weights())
print("Loaded VGG19 layer: " + vgg_layer_name)
densenet.load_weights('model/densenet_weight/densenet_bottom.h5')
\# densenet.save_weights('densenet_bottom.h5')
\# print(densenet.weights)\# 获得模型所有权值
t=densenet.get_layer('densenet_conv1/bn')
print(t)
print(densenet.get_weights()[2])
---------------------
原文：https://blog.csdn.net/xiaojiajia007/article/details/80580919


