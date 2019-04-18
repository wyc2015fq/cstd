# RefineDet算法源码 （一）训练脚本 - AI之路 - CSDN博客





2018年06月04日 08:16:44[AI之路](https://me.csdn.net/u014380165)阅读数：5534








论文：Single-Shot Refinement Neural Network for Object Detection 

论文链接：[https://arxiv.org/abs/1711.06897](https://arxiv.org/abs/1711.06897)

代码链接：[https://github.com/sfzhang15/RefineDet](https://github.com/sfzhang15/RefineDet)
关于RefineDet算法内容可以先看看博客：[RefineDet论文笔记](https://blog.csdn.net/u014380165/article/details/79502308)。

RefineDet算法是SSD算法的升级版本，所以大部分的代码也是基于SSD的开源代码来修改的。SSD开源代码参考链接：[https://github.com/weiliu89/caffe/tree/ssd](https://github.com/weiliu89/caffe/tree/ssd)。**RefineDet主要包含anchor refinement module (ARM) 、object detection module (ODM)、transfer connection block (TCB)3个部分，ARM部分可以直接用SSD代码，只不过将分类支路的类别数由object数量+1修改成2，类似RPN网络，目的是得到更好的初始bbox。ODM部分也可以基于SSD代码做修改，主要是原本采用的default box用ARM生成的bbox代替，剩下的分类和回归支路与SSD一样。TCB部分则通过一些卷积层和反卷积层即可实现。**

**这篇博客介绍RefineDet算法的训练和宏观上的网络结构构造，以主网络为ResNet101，数据集为COCO的训练脚本为例。**代码所在脚本：~RefineDet/examples/refinedet/ResNet101_COCO_320.py。

```python
from __future__ import print_function
import sys
sys.path.append("./python")
import caffe
from caffe.model_libs import *
from google.protobuf import text_format

import math
import os
import shutil
import stat
import subprocess

# Add extra layers on top of a "base" network (e.g. VGGNet or ResNet).
# AddExtraLayers函数是网络结构构造中比较重要的函数，主要实现的就是论文中的
# transfer connection block (TCB)部分，也就是类似FPN算法的特征融合操作。
def AddExtraLayers(net, arm_source_layers=[], use_batchnorm=True):
    use_relu = True

    # Add additional convolutional layers.
    # 320/32: 10 x 10
# 传进来的net变量就是ResNet101（最后一层是'res5c_relu'）
    last_layer = net.keys()[-1]
    from_layer = last_layer

    # 320/64: 5 x 5
# 在ResNet101后面再添加一个residual block，为的是获得stride=64的feature map。
    ResBody(net, from_layer, '6', out2a=128, out2b=128, out2c=512, stride=2, use_branch1=True)


# arm_source_layers传进来是['res3b3_relu', 'res4b22_relu', 'res5c_relu', 
# 'res6_relu']，reverse的目的是从最后一层开始执行transfer connection block操作。
    arm_source_layers.reverse()
    num_p = 6
    for index, layer in enumerate(arm_source_layers):
        from_layer = layer
        out_layer = "TL{}_{}".format(num_p, 1)
        ConvBNLayer(net, from_layer, out_layer, use_batchnorm, use_relu, 256, 3, 1, 1)

# num_p等于6是最后一层的处理，比较特殊，因为没有更上层的反卷积输入，最后得到的输出用P6表示。
        if num_p == 6:
            from_layer = out_layer
            out_layer = "TL{}_{}".format(num_p, 2)
            ConvBNLayer(net, from_layer, out_layer, use_batchnorm, use_relu, 256, 3, 1, 1)

            from_layer = out_layer
            out_layer = "P{}".format(num_p)
            ConvBNLayer(net, from_layer, out_layer, use_batchnorm, use_relu, 256, 3, 1, 1)
# 其他情况下执行的就是文章中的transfer connection block操作，
# 包含两个卷积层、一个反卷积操作和一个求和操作，最后得到的输出用P5、P4、P3表示。
        else:
            from_layer = out_layer
            out_layer = "TL{}_{}".format(num_p, 2)
            ConvBNLayer(net, from_layer, out_layer, use_batchnorm, False, 256, 3, 1, 1)

            from_layer = "P{}".format(num_p+1)
            out_layer = "P{}-up".format(num_p+1)
            DeconvBNLayer(net, from_layer, out_layer, use_batchnorm, False, 256, 2, 0, 2)

            from_layer = ["TL{}_{}".format(num_p, 2), "P{}-up".format(num_p+1)]
            out_layer = "Elt{}".format(num_p)
            EltwiseLayer(net, from_layer, out_layer)
            relu_name = '{}_relu'.format(out_layer)
            net[relu_name] = L.ReLU(net[out_layer], in_place=True)
            out_layer = relu_name

            from_layer = out_layer
            out_layer = "P{}".format(num_p)
            ConvBNLayer(net, from_layer, out_layer, use_batchnorm, use_relu, 256, 3, 1, 1)
        num_p = num_p - 1

    return net

### Modify the following parameters accordingly ###
# The directory which contains the caffe code.
# We assume you are running the script at the CAFFE_ROOT.
caffe_root = os.getcwd()

# Set true if you want to start training right after generating all files.
run_soon = True
# Set true if you want to load from most recently saved snapshot.
# Otherwise, we will load from the pretrain_model defined below.
resume_training = True
# If true, Remove old model files.
remove_old_models = False

# The database file for training data. Created by data/coco/create_data.sh
train_data = "examples/coco/coco_train_lmdb"
# The database file for testing data. Created by data/coco/create_data.sh
test_data = "examples/coco/coco_minival_lmdb"
# Specify the batch sampler.
# 这两个值就是模型输入的图像大小
resize_width = 320
resize_height = 320
resize = "{}x{}".format(resize_width, resize_height)
# batch_sampler列表用在数据读取和处理中
batch_sampler = [
        {
                'sampler': {
                        },
                'max_trials': 1,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'min_jaccard_overlap': 0.1,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'min_jaccard_overlap': 0.3,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'min_jaccard_overlap': 0.5,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'min_jaccard_overlap': 0.7,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'min_jaccard_overlap': 0.9,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        {
                'sampler': {
                        'min_scale': 0.3,
                        'max_scale': 1.0,
                        'min_aspect_ratio': 0.5,
                        'max_aspect_ratio': 2.0,
                        },
                'sample_constraint': {
                        'max_jaccard_overlap': 1.0,
                        },
                'max_trials': 50,
                'max_sample': 1,
        },
        ]
# train_transform_param字典是对训练数据的预处理操作
train_transform_param = {
        'mirror': True,
        'mean_value': [104, 117, 123],
        'force_color': True,
        'resize_param': {
                'prob': 1,
                'resize_mode': P.Resize.WARP,
                'height': resize_height,
                'width': resize_width,
                'interp_mode': [
                        P.Resize.LINEAR,
                        P.Resize.AREA,
                        P.Resize.NEAREST,
                        P.Resize.CUBIC,
                        P.Resize.LANCZOS4,
                        ],
                },
        'distort_param': {
                'brightness_prob': 0.5,
                'brightness_delta': 32,
                'contrast_prob': 0.5,
                'contrast_lower': 0.5,
                'contrast_upper': 1.5,
                'hue_prob': 0.5,
                'hue_delta': 18,
                'saturation_prob': 0.5,
                'saturation_lower': 0.5,
                'saturation_upper': 1.5,
                'random_order_prob': 0.0,
                },
        'expand_param': {
                'prob': 0.5,
                'max_expand_ratio': 4.0,
                },
        'emit_constraint': {
            'emit_type': caffe_pb2.EmitConstraint.CENTER,
            }
        }
# test_transfor_param字典是对测试数据的预处理操作
test_transform_param = {
        'mean_value': [104, 117, 123],
        'force_color': True,
        'resize_param': {
                'prob': 1,
                'resize_mode': P.Resize.WARP,
                'height': resize_height,
                'width': resize_width,
                'interp_mode': [P.Resize.LINEAR],
                },
        }

# A learning rate for batch_size = 1, num_gpus = 1.
# 设定初始学习率
base_lr = 0.00004 #0.00004

# Modify the job name if you want.
job_name = "refinedet_resnet101_{}".format(resize)
# The name of the model. Modify it if you want.
model_name = "coco_{}".format(job_name)


# Directory which stores the model .prototxt file.
save_dir = "models/ResNet/coco/{}".format(job_name)
# Directory which stores the snapshot of models.
snapshot_dir = "models/ResNet/coco/{}".format(job_name)
# Directory which stores the job script and log file.
job_dir = "jobs/ResNet/coco/{}".format(job_name)
# Directory which stores the detection results.
output_result_dir = "{}/data/RefineDet/coco/results/{}".format(os.environ['HOME'], job_name)

# model definition files.
# 这部分是定义.prototxt文件的路径，后面会将网络结构写到.prototxt文件中
train_net_file = "{}/train.prototxt".format(save_dir)
test_net_file = "{}/test.prototxt".format(save_dir)
deploy_net_file = "{}/deploy.prototxt".format(save_dir)
solver_file = "{}/solver.prototxt".format(save_dir)
# snapshot prefix.
snapshot_prefix = "{}/{}".format(snapshot_dir, model_name)
# job script path.
job_file = "{}/{}.sh".format(job_dir, model_name)

# Stores the test image names and sizes. Created by data/coco/create_list.sh
name_size_file = "data/coco/minival2014_name_size.txt"
# The pretrained ResNet101 model from https://github.com/KaimingHe/deep-residual-networks.
pretrain_model = "models/ResNet/ResNet-101-model.caffemodel"
# Stores LabelMapItem.
label_map_file = "data/coco/labelmap_coco.prototxt"

# MultiBoxLoss parameters.
# num_classes = object的类别数+背景
num_classes = 81
share_location = True
background_label_id = 0
train_on_diff_gt = False
normalization_mode = P.Loss.VALID
code_type = P.PriorBox.CENTER_SIZE
ignore_cross_boundary_bbox = False
mining_type = P.MultiBoxLoss.MAX_NEGATIVE
neg_pos_ratio = 3.
loc_weight = (neg_pos_ratio + 1.) / 4.

# multibox_loss_param字典是损失函数的参数配置信息，比如 'loc_loss_type'是
# 坐标回归的损失函数， 'conf_loss_type'是分类的损失函数
multibox_loss_param = {
    'loc_loss_type': P.MultiBoxLoss.SMOOTH_L1,
    'conf_loss_type': P.MultiBoxLoss.SOFTMAX,
    'loc_weight': loc_weight,
    'num_classes': num_classes,
    'share_location': share_location,
    'match_type': P.MultiBoxLoss.PER_PREDICTION,
    'overlap_threshold': 0.5,
    'use_prior_for_matching': True,
    'background_label_id': background_label_id,
    'use_difficult_gt': train_on_diff_gt,
    'mining_type': mining_type,
    'neg_pos_ratio': neg_pos_ratio,
    'neg_overlap': 0.5,
    'code_type': code_type,
    'ignore_cross_boundary_bbox': ignore_cross_boundary_bbox,
    'objectness_score': 0.01,
    }
loss_param = {
    'normalization': normalization_mode,
    }

# parameters for generating priors.
# minimum dimension of input image
min_dim = 320
# res3b3_relu ==> 40 x 40
# res4b22_relu ==> 20 x 20
# res5c_relu ==> 10 x 10
# res5c_relu/conv1_2_relu ==> 5 x 5
# arm_source_layers是ARM部分的待融合特征层，odm_source_layers是ODM部分的待融合特种层
arm_source_layers = ['res3b3_relu', 'res4b22_relu', 'res5c_relu', 'res6_relu']
odm_source_layers = ['P3', 'P4', 'P5', 'P6']
min_sizes = [32, 64, 128, 256]
max_sizes = [[], [], [], []]
steps = [8, 16, 32, 64]
aspect_ratios = [[2], [2], [2], [2]]
# variance used to encode/decode prior bboxes.
if code_type == P.PriorBox.CENTER_SIZE:
  prior_variance = [0.1, 0.1, 0.2, 0.2]
else:
  prior_variance = [0.1]
flip = True
clip = False

# Solver parameters.
# Defining which GPUs to use.
gpus = "0,1,2,3"
gpulist = gpus.split(",")
num_gpus = len(gpulist)

# Divide the mini-batch to different GPUs.
batch_size = 32
accum_batch_size = 32
iter_size = accum_batch_size / batch_size
solver_mode = P.Solver.CPU
device_id = 0
batch_size_per_device = batch_size
if num_gpus > 0:
  batch_size_per_device = int(math.ceil(float(batch_size) / num_gpus))
  iter_size = int(math.ceil(float(accum_batch_size) / (batch_size_per_device * num_gpus)))
  solver_mode = P.Solver.GPU
  device_id = int(gpulist[0])

if normalization_mode == P.Loss.NONE:
  base_lr /= batch_size_per_device
elif normalization_mode == P.Loss.VALID:
  base_lr *= 25. / loc_weight
elif normalization_mode == P.Loss.FULL:
  # Roughly there are 2000 prior bboxes per image.
  # TODO(weiliu89): Estimate the exact # of priors.
  base_lr *= 2000.

# Evaluate on whole test set.
num_test_image = 5000
test_batch_size = 1
test_iter = num_test_image / test_batch_size

# solver_param字典是训练的一些超参数设置，比如stepvalue表示当迭代次数到达多少时修改学习率，
# max_iter是迭代的最大次数，snapshot是每隔多少个迭代次数保存模型等。
solver_param = {
    # Train parameters
    'base_lr': base_lr,
    'weight_decay': 0.0005,
    'lr_policy': "multistep",
    'stepvalue': [280000, 360000, 400000],
    'gamma': 0.1,
    'momentum': 0.9,
    'iter_size': iter_size,
    'max_iter': 400000,
    'snapshot': 10000,
    'display': 10,
    'average_loss': 10,
    'type': "SGD",
    'solver_mode': solver_mode,
    'device_id': device_id,
    'debug_info': False,
    'snapshot_after_train': True,
    # Test parameters
    # 'test_iter': [test_iter],
    # 'test_interval': 10000,
    # 'eval_type': "detection",
    # 'ap_version': "11point",
    # 'test_initialization': False,
    }

# parameters for generating detection output.
det_out_param = {
    'num_classes': num_classes,
    'share_location': share_location,
    'background_label_id': background_label_id,
    'nms_param': {'nms_threshold': 0.45, 'top_k': 1000},
    'keep_top_k': 500,
    'confidence_threshold': 0.01,
    'code_type': code_type,
    'objectness_score': 0.01,
    }

# parameters for evaluating detection results.
det_eval_param = {
    'num_classes': num_classes,
    'background_label_id': background_label_id,
    'overlap_threshold': 0.5,
    'evaluate_difficult_gt': False,
    'name_size_file': name_size_file,
    }

### Hopefully you don't need to change the following ###
# Check file.
check_if_exist(train_data)
check_if_exist(test_data)
check_if_exist(label_map_file)
check_if_exist(pretrain_model)
make_if_not_exist(save_dir)
make_if_not_exist(job_dir)
make_if_not_exist(snapshot_dir)

# Create train net.
# 调用caffe.NetSpec()初始化得到一个网络
net = caffe.NetSpec()
# CreateAnnotatedDataLayer函数是用来读取数据的，函数所在脚本：
# ~RefineDet/python/caffe/model_libs.py，这部分和SSD代码是一样的。
net.data, net.label = CreateAnnotatedDataLayer(train_data, batch_size=batch_size_per_device,
        train=True, output_label=True, label_map_file=label_map_file,
        transform_param=train_transform_param, batch_sampler=batch_sampler)

# ResNet101Body函数是读取ResNet101网络结构，函数所在脚本和上述相同：
# ~RefineDet/python/caffe/model_libs.py，且这部分代码和SSD代码也是一样的。
ResNet101Body(net, from_layer='data', use_pool5=False, use_dilation_conv5=False)

# AddExtraLayers函数是基于前面的得到ResNet101网络结构增加一个residual block，
# 然后对4个层执行论文中Figure1的transfer connection block操作。
# 首先取ResNet101的res3b3_relu、res4b22_relu、res5c_relu、
# res6_relu层（这一层就是新增加的一个residual block）输出，假设输入图像大小是320*320，
# 那么这4层的输出feature map大小分别是40*40,20*20,10*10,5*5。
# 这4层就对应Figure1中Anchor Refinement Module部分的4个灰色矩形块，
# 接着从这4个矩形块引出Transfer Connection Block得到P6,P5,P4,P3，
# 也就是Figure1中Object Detection Module部分的4个蓝色矩形块。
# 这就是AddExtraLayers函数实现的内容。
AddExtraLayers(net, arm_source_layers, use_batchnorm=True)

# 因为前面AddExtraLayers函数中对arm_source_layers执行了reverse操作，所以这里相当于再反转回来。
arm_source_layers.reverse()

# CreateRefineDetHead函数用来生成分类层、回归层等，是比较重要的一个函数，
# 经过该函数后返回的mbox_layers就是完整的网络结构输出，该函数所在脚本：
# ~RefineDet/python/caffe/model_libs.py，这部分代码也是在原来SSD的
# CreateMultiBoxHead函数基础上修改得到的，另一篇博客会详细介绍该函数细节。
# 该函数有两个重要输入：from_layers=arm_source_layers和
# from_layers2=odm_source_layers，前者是Figuer1中4个灰色矩形块的集合（arm是
# Anchor Refinement Module的缩写）；后者是Figure1中4个蓝色矩形块的集合（odm是
# Object Detection Module的缩写），初始化为['P3', 'P4', 'P5', 'P6']，
# 这也是本文和SSD算法比较大的不同点。最后大概介绍下mbox_layers的内容：
# mbox_layers[0]是"arm_loc"，表示bbox的回归输出；mbox_layers[1]是"arm_conf"，
# 表示bbox的分类输出（是否是object的二分类）；mbox_layers[2]是"arm_priorbox"，
# 表示priorbox（anchor）的信息；mbox_layers[3]是”odm_conf“，表示bbox的回归输出；
# mbox_layers[4]是”odm_loc“，表示bbox的分类输出（类别数是所有object的类别数+背景）。
mbox_layers = CreateRefineDetHead(net, data_layer='data', from_layers=arm_source_layers,
        use_batchnorm=False, min_sizes=min_sizes, max_sizes=max_sizes,
        aspect_ratios=aspect_ratios, steps=steps, normalizations=[],
        num_classes=num_classes, share_location=share_location, flip=flip, clip=clip,
        prior_variance=prior_variance, kernel_size=3, pad=1, lr_mult=1, from_layers2=odm_source_layers,
        inter_layer_depth = [1, 1, 1, 1])

# 定义好网络结构后，就要定义损失函数了。
# 先定义”arm_loss“，通过L.MultiBoxLoss接口来计算损失函数。mbox_layers_arm列表
# 就是保存了bbox的回归输出（”arm_loc“）、分类输出（”arm_conf“）、anchor（或者叫priorbox）
# 信息（”arm_priorbox“）、ground truth信息（net.label），multibox_loss_param_arm中
# 除了分类类别数（”num_classes=2“）与原来的配置不同外，其他都是沿用原来的配置。
# 回传损失只回传前面两个变量。这部分损失基本上和RPN网络类似，分类部分的损失根据
# mbox_layers[1]（”arm_conf“）和net.label来得到，回归部分的损失根据
# mbox_layers[0]（”arm_loc“）和mbox_layers[2]（”arm_priorbox“）来得到。
# MultiBoxLoss是自定义层，最早见于SSD算法中，这里稍作修改，源码参考
# https://github.com/sfzhang15/RefineDet/blob/master/src/caffe/layers/multibox_loss_layer.cpp。
# 不过在这里调用时候和在SSD中调用没有区别（输入列表都是4个变量），只不过分类的类别数变化了而已。
name = "arm_loss"
mbox_layers_arm = []
mbox_layers_arm.append(mbox_layers[0])
mbox_layers_arm.append(mbox_layers[1])
mbox_layers_arm.append(mbox_layers[2])
mbox_layers_arm.append(net.label)
multibox_loss_param_arm = multibox_loss_param.copy()
multibox_loss_param_arm['num_classes'] = 2
net[name] = L.MultiBoxLoss(*mbox_layers_arm, ultibox_loss_param=multibox_loss_param_arm, loss_param=loss_param, include=dict(phase=caffe_pb2.Phase.Value('TRAIN')),
        propagate_down=[True, True, False, False])

# 这一部分代码主要是将net["arm_conf"]作为softmax函数的输入，并得到分类概率输出net[flatten_name]，
# 或者写成net["arm_conf_flatten"]。net["arm_conf"]是前面arm部分的二分类输出结果，
# 因此这部分操作和Faster RCNN中得到proposal的过程几乎是一样的。
# Create the MultiBoxLossLayer.
conf_name = "arm_conf"
reshape_name = "{}_reshape".format(conf_name)
net[reshape_name] = L.Reshape(net[conf_name], shape=dict(dim=[0, -1, 2]))
softmax_name = "{}_softmax".format(conf_name)
net[softmax_name] = L.Softmax(net[reshape_name], axis=2)
flatten_name = "{}_flatten".format(conf_name)
net[flatten_name] = L.Flatten(net[softmax_name], axis=1)

# 定义”odm_loss“，也是通过L.MultiBoxLoss接口来计算损失函数。
# mbox_layers_odm列表保存了bbox的回归输出（”odm_loc“）、分类输出（”odm_conf“）、
# anchor（或者叫priorbox）信息（”arm_priorbox“）、gound truth信息（net.label）、
# 分类的概率输出（net["arm_conf_flatten"]）、ARM部分的bbox的回归输出（net[”arm_loc“]）。
# 除了前面两个变量外，后面4个变量都是为了做bbox的过滤和正负样本的平衡，因此回传损失只回传前面两个变量。
# 这部分的损失函数计算和SSD算法类似，分类支路的损失根据mbox_layers[4]（”odm_conf“）和
# net.label得到，回归支路的损失根据mbox_layers[3]（”odm_loc“）和
# mbox_layers[2]（”arm_priorbox“）得到。这里可以看到输入列表变成6个变量，
# 这里就是RefineNet中对MultiBoxLossLayer的修改：增加了两个输入变量。
# 这部分非常重要，也是RefineDet的一个亮点的体现。这两个变量中的net["arm_conf_flatten"]
# 主要参与到hard negative mining过程，是对负样本排序和sample的（文中说的是负样本
# 的confidence（也就是判为负样本的概率）大于阈值0.99，则该样本不参与到ODM部分的训练）。
# 另一个变量net[”arm_loc“]提供了bbox的初始坐标，有利于检测网络得到更准确的结果。
name = "odm_loss"
mbox_layers_odm = []
mbox_layers_odm.append(mbox_layers[3])
mbox_layers_odm.append(mbox_layers[4])
mbox_layers_odm.append(mbox_layers[2])
mbox_layers_odm.append(net.label)
mbox_layers_odm.append(net[flatten_name])
mbox_layers_odm.append(mbox_layers[0])
net[name] = L.MultiBoxLoss(*mbox_layers_odm, multibox_loss_param=multibox_loss_param,
        loss_param=loss_param, include=dict(phase=caffe_pb2.Phase.Value('TRAIN')),
        propagate_down=[True, True, False, False, False, False])
```




