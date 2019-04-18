# resNet_model—定义残差网络模型 - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:42:16[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：348











### 文章目录
- [resnet_model.py](#resnet_modelpy_1)
- [resnet_main.py](#resnet_mainpy_366)
- [cifar_input.py](#cifar_inputpy_602)




### resnet_model.py

```python
```python
"""ResNet model.

Related papers:
https://arxiv.org/pdf/1603.05027v2.pdf
https://arxiv.org/pdf/1512.03385v1.pdf
https://arxiv.org/pdf/1605.07146v1.pdf
"""


import numpy as np
import tensorflow as tf
import six
from collections import namedtuple
from tensorflow.python.training import moving_averages


HParams = namedtuple('HParams',
                     'batch_size, num_classes, min_lrn_rate, lrn_rate, '
                     'num_residual_units, use_bottleneck, weight_decay_rate, '
                     'relu_leakiness, optimizer')

"""
    HParams = namedtuple('HParams',
                         '一个batch内的图片个数', 
                         '分类任务数目', 
                         '最小的学习率', 
                         '学习率', 
                         '一个残差组内残差单元数量', 
                         '是否使用bottleneck',  
                         'relu泄漏',
                         '优化策略')
    定义resNet网络模型：
        |
        |----- __init__        （初始化）
        |----- build_graph     （构建模型图） 
        |----- _build_model    （构建模型）
        |         |-----（残差网络：bottleneck模块/标准模块 ）
        |         |----- 第一组
        |         |----- 第二组
        |         |----- 第三组
        |         |----- 全局池化
        |         |----- 全连接层 + Softmax
        |         |----- 构建损失函数（交叉熵）
        |
        |----- _build_train_op（构建训练操作）学习率/步长-->计算训练参数的梯度-->设置优化方法
        |                                     梯度优化操作-->合并BN更新操作-->建立优化操作组
        |         
        |----- _stride_arr（步长调整） 
        |----- _residual（残差单元模块）
        |          |-------是否前置激活(先 残差直连、还是先 BN和ReLU）
        |          |-------第一子层 
        |          |-------第二子层 
        |          |-------合并残差层
        |
        |----- bottleneck残差单元模块
        |          |-------是否前置激活(先 残差直连、还是先 BN和ReLU）
        |          |-------第一子层 
        |          |-------第二子层 
        |          |-------第三子层 
        |          |-------合并残差层
        |
        |----- _batch_norm 批量归一化：((x-mean)/var)*gamma+beta
        |----- _decay      权重衰减 L2正则loss
        |----- _conv 2D    卷积
        |----- _relu :     leaky ReLU激活函数，泄漏参数leakiness为0就是标准ReLU
        |----- _fully_connected  全连接层，网络最后一层
        |----- _global_avg_pool  全局均值池化

"""
class ResNet(object):
"""ResNet model."""

    def __init__(self, hps, images, labels, mode):
        """ResNet constructor.
        Args:
          hps: Hyperparameters.
          images: Batches of images 图片. [batch_size, image_size, image_size, 3]
          labels: Batches of labels 类别标签. [batch_size, num_classes]
          mode: One of 'train' and 'eval'.
        """
        self.hps = hps
        self._images = images
        self.labels = labels
        self.mode = mode
        self._extra_train_ops = []

    
    def build_graph(self):                       # 构建模型图
        # 新建全局step
        self.global_step = tf.contrib.framework.get_or_create_global_step()
        self._build_model()                      # 构建ResNet网络模型
        if self.mode == 'train':                 # 构建优化训练操作
            self._build_train_op()
        self.summaries = tf.summary.merge_all()  # 合并所有总结

    def _build_model(self):                     # 构建模型
        with tf.variable_scope('init'):
            x = self._images
            """第一层卷积（3,3x3/1,16）"""
            x = self._conv('init_conv', x, 3, 3, 16, self._stride_arr(1))

        strides = [1, 2, 2]                                  # 残差网络参数
        activate_before_residual = [True, False, False]     # 激活前置
        if self.hps.use_bottleneck:
            res_func = self._bottleneck_residual            # bottleneck残差单元模块
            filters = [16, 64, 128, 256]                    # 通道数量
        else:
            res_func = self._residual                       # 标准残差单元模块
            filters = [16, 16, 32, 64]                      # 通道数量

        # 第一组
        with tf.variable_scope('unit_1_0'):
            x = res_func(x, filters[0], filters[1], 
                       self._stride_arr(strides[0]),
                       activate_before_residual[0])
        for i in six.moves.range(1, self.hps.num_residual_units):
            with tf.variable_scope('unit_1_%d' % i):
            x = res_func(x, filters[1], filters[1], self._stride_arr(1), False)

        # 第二组
        with tf.variable_scope('unit_2_0'):
            x = res_func(x, filters[1], filters[2], 
                       self._stride_arr(strides[1]),
                       activate_before_residual[1])
        for i in six.moves.range(1, self.hps.num_residual_units):
            with tf.variable_scope('unit_2_%d' % i):
                x = res_func(x, filters[2], filters[2], self._stride_arr(1), False)
        
        # 第三组
        with tf.variable_scope('unit_3_0'):
            x = res_func(x, filters[2], filters[3], self._stride_arr(strides[2]),
                       activate_before_residual[2])
        for i in six.moves.range(1, self.hps.num_residual_units):
            with tf.variable_scope('unit_3_%d' % i):
                x = res_func(x, filters[3], filters[3], self._stride_arr(1), False)

        # 全局池化层
        with tf.variable_scope('unit_last'):
            x = self._batch_norm('final_bn', x)
            x = self._relu(x, self.hps.relu_leakiness)
            x = self._global_avg_pool(x)

        # 全连接层 + Softmax
        with tf.variable_scope('logit'):
            logits = self._fully_connected(x, self.hps.num_classes)
            self.predictions = tf.nn.softmax(logits)

        # 构建损失函数
        with tf.variable_scope('costs'):
            # 交叉熵
            xent = tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=self.labels)
            self.cost = tf.reduce_mean(xent, name='xent')    # 加和
            self.cost += self._decay()                       # L2正则，权重衰减
            tf.summary.scalar('cost', self.cost)             # 添加cost总结，用于Tensorborad显示

    # 构建训练操作
    def _build_train_op(self):
        """
        （ 学习率/步长-->计算训练参数的梯度-->设置优化方法）
        """
        self.lrn_rate = tf.constant(self.hps.lrn_rate, tf.float32)
        tf.summary.scalar('learning_rate', self.lrn_rate)

        trainable_variables = tf.trainable_variables()
        grads = tf.gradients(self.cost, trainable_variables)
        
        if self.hps.optimizer == 'sgd':
            optimizer = tf.train.GradientDescentOptimizer(self.lrn_rate)
        elif self.hps.optimizer == 'mom':
            optimizer = tf.train.MomentumOptimizer(self.lrn_rate, 0.9)

        # 梯度优化操作-->合并BN更新操作-->建立优化操作组
        apply_op = optimizer.apply_gradients(
                            zip(grads, trainable_variables),
                            global_step=self.global_step, 
                            name='train_step')
        train_ops = [apply_op] + self._extra_train_ops
        self.train_op = tf.group(*train_ops)


    # 把步长值转换成tf.nn.conv2d需要的步长数组
    def _stride_arr(self, stride):    
        return [1, stride, stride, 1]

    # 残差单元模块
    def _residual(self, x, in_filter, out_filter, stride, activate_before_residual=False):
        if activate_before_residual:                      # 是否前置激活(取残差直连之前进行BN和ReLU）
            with tf.variable_scope('shared_activation'):  # 先做BN和ReLU激活
                x = self._batch_norm('init_bn', x)
                x = self._relu(x, self.hps.relu_leakiness)
                orig_x = x                                # 获取残差直连
        else:
            with tf.variable_scope('residual_only_activation'):
                orig_x = x                               # 获取残差直连
                x = self._batch_norm('init_bn', x)       # 后做BN和ReLU激活
                x = self._relu(x, self.hps.relu_leakiness)

        with tf.variable_scope('sub1'):                  # 第1子层
            # 3x3卷积，通道数(in_filter -> out_filter,使用输入步长，)
            x = self._conv('conv1', x, 3, in_filter, out_filter, stride)

        with tf.variable_scope('sub2'):                  # 第2子层
            x = self._batch_norm('bn2', x)               # BN和ReLU激活
            x = self._relu(x, self.hps.relu_leakiness)
            # 3x3卷积，步长为1，通道数不变(out_filter)
            x = self._conv('conv2', x, 3, out_filter, out_filter, [1, 1, 1, 1])
    
        # 合并残差层
        with tf.variable_scope('sub_add'):
            if in_filter != out_filter:                    # 当通道数有变化时
                orig_x = tf.nn.avg_pool(orig_x, stride, 
                                        stride, 'VALID')   # 均值池化，无补零
                orig_x = tf.pad(orig_x,                    # 通道补零(第4维前后对称补零)
                                [[0, 0], 
                                 [0, 0], 
                                 [0, 0],
                                 [(out_filter-in_filter)//2, (out_filter-in_filter)//2]
                                ])
            x += orig_x                                  # 合并残差
        tf.logging.debug('image after unit %s', x.get_shape())
        return x

    # bottleneck残差单元模块
    def _bottleneck_residual(self, x, in_filter, out_filter, stride,
                           activate_before_residual=False):
        
        if activate_before_residual:                  # 是否前置激活(取残差直连之前进行BN和ReLU）
            with tf.variable_scope('common_bn_relu'):
                x = self._batch_norm('init_bn', x)   # 先做BN和ReLU激活
                x = self._relu(x, self.hps.relu_leakiness)
                orig_x = x                            # 获取残差直连
        else: 
            with tf.variable_scope('residual_bn_relu'):
                orig_x = x                           # 获取残差直连
                x = self._batch_norm('init_bn', x)   # 后做BN和ReLU激活
                x = self._relu(x, self.hps.relu_leakiness)

        # 第1子层
        with tf.variable_scope('sub1'):
            # 1x1卷积，使用输入步长，通道数(in_filter -> out_filter/4)
            x = self._conv('conv1', x, 1, in_filter, out_filter/4, stride)

        # 第2子层
        with tf.variable_scope('sub2'):
            x = self._batch_norm('bn2', x)           # BN和ReLU激活
            x = self._relu(x, self.hps.relu_leakiness)
            # 3x3卷积，步长为1，通道数不变(out_filter/4)
            x = self._conv('conv2', x, 3, out_filter/4, out_filter/4, [1, 1, 1, 1])

        # 第3子层
        with tf.variable_scope('sub3'):
            x = self._batch_norm('bn3', x)          # BN和ReLU激活
            x = self._relu(x, self.hps.relu_leakiness)
            # 1x1卷积，步长为1，通道数不变(out_filter/4 -> out_filter)
            x = self._conv('conv3', x, 1, out_filter/4, out_filter, [1, 1, 1, 1])

        # 合并残差层
        with tf.variable_scope('sub_add'):
            if in_filter != out_filter:          # 当通道数有变化时
                # 1x1卷积，使用输入步长，通道数(in_filter -> out_filter)
                orig_x = self._conv('project', orig_x, 1, in_filter, out_filter, stride)

            x += orig_x                            # 合并残差
        tf.logging.info('image after unit %s', x.get_shape())
        return x


  # Batch Normalization批归一化    ((x-mean)/var)*gamma+beta
    def _batch_norm(self, name, x):
        with tf.variable_scope(name):
            params_shape = [x.get_shape()[-1]]     # 输入通道维数
            beta = tf.get_variable('beta',         # offset
                                 params_shape, 
                                 tf.float32,
                                 initializer=tf.constant_initializer(0.0, tf.float32))
            gamma = tf.get_variable('gamma',      # scale
                                  params_shape, 
                                  tf.float32,
                                  initializer=tf.constant_initializer(1.0, tf.float32))

            if self.mode == 'train':                                # 为每个通道计算均值、标准差
                mean, variance = tf.nn.moments(x, [0, 1, 2], name='moments')
                moving_mean = tf.get_variable('moving_mean',        # 新建或建立测试阶段使用的batch均值、标准差
                                              params_shape, tf.float32,
                                              initializer=tf.constant_initializer(0.0, tf.float32),
                                              trainable=False)
                moving_variance = tf.get_variable('moving_variance', 
                                                  params_shape, tf.float32,
                                                  initializer=tf.constant_initializer(1.0, tf.float32),
                                                  trainable=False)
                """
                     添加batch均值和标准差的更新操作(滑动平均)
                     moving_mean = moving_mean * decay + mean * (1 - decay)
                     moving_variance = moving_variance * decay + variance * (1 - decay)
                """
                self._extra_train_ops.append(moving_averages.assign_moving_average(
                                                                moving_mean, mean, 0.9))
                self._extra_train_ops.append(moving_averages.assign_moving_average(
                                                    moving_variance, variance, 0.9))
            else:
                mean = tf.get_variable('moving_mean',       # 获取训练中积累的batch均值、标准差
                                       params_shape, tf.float32,
                                       initializer=tf.constant_initializer(0.0, tf.float32),
                                       trainable=False)
                variance = tf.get_variable('moving_variance', 
                                           params_shape, tf.float32,
                                           initializer=tf.constant_initializer(1.0, tf.float32),
                                           trainable=False)
                tf.summary.histogram(mean.op.name, mean)   # 添加到直方图总结
                tf.summary.histogram(variance.op.name, variance)

            # BN层：((x-mean)/var)*gamma+beta
            y = tf.nn.batch_normalization(x, mean, variance, beta, gamma, 0.001)
            y.set_shape(x.get_shape())
            return y



    def _decay(self):                        # 权重衰减，L2正则loss
        costs = []
        for var in tf.trainable_variables(): # 遍历所有可训练变量
            if var.op.name.find(r'DW') > 0:  #只计算标有“DW”的变量
                costs.append(tf.nn.l2_loss(var))
        # 加和，并乘以衰减因子
        return tf.multiply(self.hps.weight_decay_rate, tf.add_n(costs))

    # 2D卷积
    def _conv(self, name, x, filter_size, in_filters, out_filters, strides):
        with tf.variable_scope(name):
            n = filter_size * filter_size * out_filters
            # 获取或新建卷积核，正态随机初始化
            kernel = tf.get_variable(
                  'DW', 
                  [filter_size, filter_size, in_filters, out_filters],
                  tf.float32, 
                  initializer=tf.random_normal_initializer(stddev=np.sqrt(2.0/n)))
        # 计算卷积
        return tf.nn.conv2d(x, kernel, strides, padding='SAME')

    # leaky ReLU激活函数，泄漏参数leakiness为0就是标准ReLU
    def _relu(self, x, leakiness=0.0):
        return tf.where(tf.less(x, 0.0), leakiness * x, x, name='leaky_relu')

    # 全连接层，网络最后一层
    def _fully_connected(self, x, out_dim):
        
        x = tf.reshape(x, [self.hps.batch_size, -1])    # 输入转换成2D tensor，尺寸为[N,-1]
        w = tf.get_variable('DW',                       # 参数w，平均随机初始化，[-sqrt(3/dim), sqrt(3/dim)]*factor
                            [x.get_shape()[1], out_dim],
                            initializer=tf.uniform_unit_scaling_initializer(factor=1.0))
        
        b = tf.get_variable('biases',                  # 参数b，0值初始化
                            [out_dim], 
                            initializer=tf.constant_initializer())

        return tf.nn.xw_plus_b(x, w, b)              # 计算x*w+b

    # 全局均值池化
    def _global_avg_pool(self, x):
        assert x.get_shape().ndims == 4
        # 在第2&3维度上计算均值，尺寸由WxH收缩为1x1
        return tf.reduce_mean(x, [1, 2])
```
```

### resnet_main.py

```python
```python
"""
    ResNet Train/Eval module.
"""
import time
import six
import sys

import cifar_input
import numpy as np
import resnet_model
import tensorflow as tf


# FLAGS参数设置
FLAGS = tf.app.flags.FLAGS
# 数据集类型
tf.app.flags.DEFINE_string('dataset', 
                           'cifar10', 
                           'cifar10 or cifar100.')
# 模式：训练、测试
tf.app.flags.DEFINE_string('mode', 
                           'train', 
                           'train or eval.')
# 训练数据路径
tf.app.flags.DEFINE_string('train_data_path', 
                           'data/cifar-10-batches-bin/data_batch*',
                           'Filepattern for training data.')
# 测试数据路劲
tf.app.flags.DEFINE_string('eval_data_path', 
                           'data/cifar-10-batches-bin/test_batch.bin',
                           'Filepattern for eval data')
# 图片尺寸
tf.app.flags.DEFINE_integer('image_size', 
                            32, 
                            'Image side length.')
# 训练过程数据的存放路劲
tf.app.flags.DEFINE_string('train_dir', 
                           'temp/train',
                           'Directory to keep training outputs.')
# 测试过程数据的存放路劲
tf.app.flags.DEFINE_string('eval_dir', 
                           'temp/eval',
                           'Directory to keep eval outputs.')
# 测试数据的Batch数量
tf.app.flags.DEFINE_integer('eval_batch_count', 
                            50,
                            'Number of batches to eval.')
# 一次性测试
tf.app.flags.DEFINE_bool('eval_once', 
                         False,
                         'Whether evaluate the model only once.')
# 模型存储路劲
tf.app.flags.DEFINE_string('log_root', 
                           'temp',
                           'Directory to keep the checkpoints. Should be a '
                           'parent directory of FLAGS.train_dir/eval_dir.')
# GPU设备数量（0代表CPU）
tf.app.flags.DEFINE_integer('num_gpus', 
                            1,
                            'Number of gpus used for training. (0 or 1)')


def train(hps):
    # 构建输入数据(读取队列执行器）
    images, labels = cifar_input.build_input(
    FLAGS.dataset, FLAGS.train_data_path, hps.batch_size, FLAGS.mode)
    # 构建残差网络模型
    model = resnet_model.ResNet(hps, images, labels, FLAGS.mode)
    model.build_graph()

    # 计算预测准确率
    truth = tf.argmax(model.labels, axis=1)
    predictions = tf.argmax(model.predictions, axis=1)
    precision = tf.reduce_mean(tf.to_float(tf.equal(predictions, truth)))

    # 建立总结存储器，每100步存储一次
    summary_hook = tf.train.SummarySaverHook(
          save_steps=100,
          output_dir=FLAGS.train_dir,
          summary_op=tf.summary.merge(
                          [model.summaries,
                           tf.summary.scalar('Precision', precision)]))
    # 建立日志打印器，每100步打印一次
    logging_hook = tf.train.LoggingTensorHook(
    tensors={'step': model.global_step,
           'loss': model.cost,
           'precision': precision},
    every_n_iter=100)

    # 学习率更新器，基于全局Step
    class _LearningRateSetterHook(tf.train.SessionRunHook):
        def begin(self):
            self._lrn_rate = 0.1     #初始学习率

        def before_run(self, run_context):
            return tf.train.SessionRunArgs(
                          model.global_step,  # 获取全局Step 设置学习率
                          feed_dict={model.lrn_rate: self._lrn_rate})  

        def after_run(self, run_context, run_values):
            train_step = run_values.results  # 动态更新学习率
            if train_step < 40000:
                self._lrn_rate = 0.1
            elif train_step < 60000:
                self._lrn_rate = 0.01
            elif train_step < 80000:
                self._lrn_rate = 0.001
            else:
                self._lrn_rate = 0.0001

    # 建立监控Session
    with tf.train.MonitoredTrainingSession(checkpoint_dir=FLAGS.log_root,
                                           hooks=[logging_hook, _LearningRateSetterHook()],
                                           chief_only_hooks=[summary_hook],
                                           save_summaries_steps=0,   # 禁用默认的SummarySaverHook，save_summaries_steps设置为0
                                           config=tf.ConfigProto(allow_soft_placement=True)) as mon_sess:
        while not mon_sess.should_stop():
            mon_sess.run(model.train_op)   # 执行优化训练操作


    def evaluate(hps):
        # 构建输入数据(读取队列执行器）
        images, labels = cifar_input.build_input(
        FLAGS.dataset, FLAGS.eval_data_path, hps.batch_size, FLAGS.mode)
        # 构建残差网络模型
        model = resnet_model.ResNet(hps, images, labels, FLAGS.mode)
        model.build_graph()
        saver = tf.train.Saver()    # 模型变量存储器
        summary_writer = tf.summary.FileWriter(FLAGS.eval_dir)   # 总结文件 生成器

        # 执行Session
        sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True))

        tf.train.start_queue_runners(sess)   # 启动所有队列执行器

        best_precision = 0.0
        while True:
            try:            # 检查checkpoint文件
                ckpt_state = tf.train.get_checkpoint_state(FLAGS.log_root)
            except tf.errors.OutOfRangeError as e:
                tf.logging.error('Cannot restore checkpoint: %s', e)
                continue
            if not (ckpt_state and ckpt_state.model_checkpoint_path):
                tf.logging.info('No model to eval yet at %s', FLAGS.log_root)
                continue
  
        # 读取模型数据(训练期间生成)
        tf.logging.info('Loading checkpoint %s', ckpt_state.model_checkpoint_path)
        saver.restore(sess, ckpt_state.model_checkpoint_path)

        # 逐Batch执行测试
        total_prediction, correct_prediction = 0, 0
        for _ in six.moves.range(FLAGS.eval_batch_count):
            (loss, predictions, truth, train_step) = sess.run(
              [model.cost, model.predictions,
               model.labels, model.global_step])
            # 计算预测结果
            truth = np.argmax(truth, axis=1)
            predictions = np.argmax(predictions, axis=1)
            correct_prediction += np.sum(truth == predictions)
            total_prediction += predictions.shape[0]

        # 计算准确率
        precision = 1.0 * correct_prediction / total_prediction
        best_precision = max(precision, best_precision)

        # 添加准确率总结
        precision_summ = tf.Summary()
        precision_summ.value.add(
            tag='Precision', simple_value=precision)
        summary_writer.add_summary(precision_summ, train_step)

        # 添加最佳准确总结
        best_precision_summ = tf.Summary()
        best_precision_summ.value.add(
            tag='Best Precision', simple_value=best_precision)
        summary_writer.add_summary(best_precision_summ, train_step)

        # 添加测试总结
        #summary_writer.add_summary(summaries, train_step)

        # 打印日志
        tf.logging.info('loss: %.3f, precision: %.3f, best precision: %.3f' %
                        (loss, precision, best_precision))

        # 执行写文件
        summary_writer.flush()

        if FLAGS.eval_once:
            break

        time.sleep(60)


def main(_):
    if FLAGS.num_gpus == 0:        # 设备选择
        dev = '/cpu:0'
    elif FLAGS.num_gpus == 1:
        dev = '/gpu:0'
    else:
        raise ValueError('Only support 0 or 1 gpu.')

    if FLAGS.mode == 'train':       # 执行模式
        batch_size = 128
    elif FLAGS.mode == 'eval':
        batch_size = 100

    if FLAGS.dataset == 'cifar10':      # 数据集类别数量
        num_classes = 10
    elif FLAGS.dataset == 'cifar100':
        num_classes = 100

    # 残差网络模型参数
    hps = resnet_model.HParams(batch_size=batch_size,
                             num_classes=num_classes,
                             min_lrn_rate=0.0001,
                             lrn_rate=0.1,
                             num_residual_units=5,
                             use_bottleneck=False,
                             weight_decay_rate=0.0002,
                             relu_leakiness=0.1,
                             optimizer='mom')
    # 执行训练或测试
    with tf.device(dev):
        if FLAGS.mode == 'train':
            train(hps)
        elif FLAGS.mode == 'eval':
            evaluate(hps)


if __name__ == '__main__':
    tf.logging.set_verbosity(tf.logging.INFO)
    tf.app.run()
```
```

### cifar_input.py

```python
```python
"""CIFAR dataset input module.
"""

import tensorflow as tf

def build_input(dataset, data_path, batch_size, mode):
    """Build CIFAR image and labels.

    Args:
    dataset(数据集): Either 'cifar10' or 'cifar100'.
    data_path(数据集路径): Filename for data.
    batch_size: Input batch size.
    mode(模式）: Either 'train' or 'eval'.
    Returns:
    images(图片): Batches of images. [batch_size, image_size, image_size, 3]
    labels(类别标签): Batches of labels. [batch_size, num_classes]
    Raises:
    ValueError: when the specified dataset is not supported.
    """

    # 数据集参数
    image_size = 32
    if dataset == 'cifar10':
        label_bytes = 1
        label_offset = 0
        num_classes = 10
    elif dataset == 'cifar100':
        label_bytes = 1
        label_offset = 1
        num_classes = 100
    else:
        raise ValueError('Not supported dataset %s', dataset)

    # 数据读取参数
    depth = 3
    image_bytes = image_size * image_size * depth
    record_bytes = label_bytes + label_offset + image_bytes

    # 获取文件名列表
    data_files = tf.gfile.Glob(data_path)
    # 文件名列表生成器
    file_queue = tf.train.string_input_producer(data_files, shuffle=True)
    # 文件名列表里读取原始二进制数据
    reader = tf.FixedLengthRecordReader(record_bytes=record_bytes)
    _, value = reader.read(file_queue)

    # 将原始二进制数据转换成图片数据及类别标签
    record = tf.reshape(tf.decode_raw(value, tf.uint8), [record_bytes])
    label = tf.cast(tf.slice(record, [label_offset], [label_bytes]), tf.int32)
    # 将数据串 [depth * height * width] 转换成矩阵 [depth, height, width].
    depth_major = tf.reshape(tf.slice(record, [label_bytes], [image_bytes]),
                           [depth, image_size, image_size])
    # 转换维数：[depth, height, width]转成[height, width, depth].
    image = tf.cast(tf.transpose(depth_major, [1, 2, 0]), tf.float32)

    if mode == 'train':       #图像增强
        image = tf.image.resize_image_with_crop_or_pad(
                            image, image_size+4, image_size+4)
        image = tf.random_crop(image, [image_size, image_size, 3])
        image = tf.image.random_flip_left_right(image)
        image = tf.image.per_image_standardization(image)

        # 建立输入数据队列(随机洗牌)
        example_queue = tf.RandomShuffleQueue(
            capacity=16 * batch_size,              # 队列容量
            min_after_dequeue=8 * batch_size,       # 队列数据的最小容许量
            dtypes=[tf.float32, tf.int32],
            shapes=[[image_size, image_size, depth], [1]])  # 图片数据尺寸，标签尺寸
        # 读线程的数量
        num_threads = 16
    else:
        # 获取测试图片，并做像素值中心化
        image = tf.image.resize_image_with_crop_or_pad(
                            image, image_size, image_size)
        image = tf.image.per_image_standardization(image)

        # 建立输入数据队列(先入先出队列）
        example_queue = tf.FIFOQueue(
            3 * batch_size,
            dtypes=[tf.float32, tf.int32],
            shapes=[[image_size, image_size, depth], [1]])
        num_threads = 1           # 读线程的数量

    # 数据入队操作
    example_enqueue_op = example_queue.enqueue([image, label])
    
    # 队列执行器
    tf.train.add_queue_runner(tf.train.queue_runner.QueueRunner(
      example_queue, [example_enqueue_op] * num_threads))

    # 数据出队操作，从队列读取Batch数据
    images, labels = example_queue.dequeue_many(batch_size)
    # 将标签数据由稀疏格式转换成稠密格式
    # [ 2,       [[0,1,0,0,0]
    #   4,        [0,0,0,1,0]  
    #   3,   -->  [0,0,1,0,0]    
    #   5,        [0,0,0,0,1]
    #   1 ]       [1,0,0,0,0]]
    labels = tf.reshape(labels, [batch_size, 1])
    indices = tf.reshape(tf.range(0, batch_size, 1), [batch_size, 1])
    labels = tf.sparse_to_dense(
                  tf.concat(values=[indices, labels], axis=1),
                  [batch_size, num_classes], 1.0, 0.0)

    #检测数据维度
    assert len(images.get_shape()) == 4
    assert images.get_shape()[0] == batch_size
    assert images.get_shape()[-1] == 3
    assert len(labels.get_shape()) == 2
    assert labels.get_shape()[0] == batch_size
    assert labels.get_shape()[1] == num_classes

    # 添加图片总结
    tf.summary.image('images', images)
    return images, labels
```
```



