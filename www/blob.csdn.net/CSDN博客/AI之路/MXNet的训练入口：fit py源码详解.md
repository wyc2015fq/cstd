# MXNet的训练入口：fit.py源码详解 - AI之路 - CSDN博客





2017年06月29日 22:27:17[AI之路](https://me.csdn.net/u014380165)阅读数：3426
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**fit.py是MXNet的fine-tune.py（参看博文：[MXNet的fine-tune.py源码详解](http://blog.csdn.net/u014380165/article/details/73864457)）中启动训练的入口，非常值得读一读源码。这个脚本是作者包装好的训练入口，最核心的还是Module类的fit方法（model.fit()就是Module类的对象在条用fit方法）。总的来讲，这个fit.py脚本包含训练的一些配置，导入模型，训练模型和保存模型这几步，接下来详细阐述。建议从最后的主函数fit()开始看起。**

```python
import mxnet as mx
import logging
import os
import time


# 这个函数主要是和学习率相关，我们在启动训练的时候一般会添加这个参数：--lr，就是学习率，
# 如果不设置的话就会采用fine-tune.py中的默认lr。lr_factor表示当你要改变lr的时候是以多大比率改变，
# 举个例子，你原来lr是0.1，设置的lr_step_epochs是2，那么当你训练到epoch==2的时候，
# 你的lr就会变成lr*lr_factor，这个lr_factor在fit.py脚本中默认设置为0.1。
# 另外你的lr_step_epochs可以有多个值，比如（2,4,6），表示当epoch达到这些值的时候都要乘以lr_factor。
def _get_lr_scheduler(args, kv):
    if 'lr_factor' not in args or args.lr_factor >= 1:
        return (args.lr, None)
    epoch_size = args.num_examples / args.batch_size
    if 'dist' in args.kv_store:
        epoch_size /= kv.num_workers
    begin_epoch = args.load_epoch if args.load_epoch else 0
    step_epochs = [int(l) for l in args.lr_step_epochs.split(',')]
    lr = args.lr
    for s in step_epochs:
        if begin_epoch >= s:
            lr *= args.lr_factor
    if lr != args.lr:
        logging.info('Adjust learning rate to %e for epoch %d' %(lr, begin_epoch))

    steps = [epoch_size * (x-begin_epoch) for x in step_epochs if x-begin_epoch > 0]
    return (lr, mx.lr_scheduler.MultiFactorScheduler(step=steps, factor=args.lr_factor))


# 导入模型，其实本质还是和fine-tune.py中的导入模型一样采用model.py脚本中的load_checkpoint函数。
# 首先判断你的load_epoch参数有没有设置，没设置的话运行时候会直接报错。
def _load_model(args, rank=0):
    if 'load_epoch' not in args or args.load_epoch is None:
        return (None, None, None)
    assert args.model_prefix is not None
    model_prefix = args.model_prefix
    if rank > 0 and os.path.exists("%s-%d-symbol.json" % (model_prefix, rank)):
        model_prefix += "-%d" % (rank)
    sym, arg_params, aux_params = mx.model.load_checkpoint(
        model_prefix, args.load_epoch)
    logging.info('Loaded model %s_%04d.params', model_prefix, args.load_epoch)
    return (sym, arg_params, aux_params)


#保存模型，需要你设置model_prefix这个参数。
def _save_model(args, rank=0):
    if args.model_prefix is None:
        return None
    dst_dir = os.path.dirname(args.model_prefix)
    if not os.path.isdir(dst_dir):
        os.mkdir(dst_dir)
    return mx.callback.do_checkpoint(args.model_prefix if rank == 0 else "%s-%d" % (
        args.model_prefix, rank))


#这部分主要是配置和训练相关的一些参数，里面的help字段是对该参数的解释，比较清晰了。
def add_fit_args(parser):
    """
    parser : argparse.ArgumentParser
    return a parser added with args required by fit
    """
    train = parser.add_argument_group('Training', 'model training')
    train.add_argument('--network', type=str,
                       help='the neural network to use')
    train.add_argument('--num-layers', type=int,
                       help='number of layers in the neural network, required by some networks such as resnet')
    train.add_argument('--gpus', type=str,
                       help='list of gpus to run, e.g. 0 or 0,2,5. empty means using cpu')
    train.add_argument('--kv-store', type=str, default='device',
                       help='key-value store type')
    train.add_argument('--num-epochs', type=int, default=100,
                       help='max num of epochs')
    train.add_argument('--lr', type=float, default=0.1,
                       help='initial learning rate')
    train.add_argument('--lr-factor', type=float, default=0.1,
                       help='the ratio to reduce lr on each step')
    train.add_argument('--lr-step-epochs', type=str,
                       help='the epochs to reduce the lr, e.g. 30,60')
    train.add_argument('--optimizer', type=str, default='sgd',
                       help='the optimizer type')
    train.add_argument('--mom', type=float, default=0.9,
                       help='momentum for sgd')
    train.add_argument('--wd', type=float, default=0.0001,
                       help='weight decay for sgd')
    train.add_argument('--batch-size', type=int, default=128,
                       help='the batch size')
    train.add_argument('--disp-batches', type=int, default=20,
                       help='show progress for every n batches')
    train.add_argument('--model-prefix', type=str,
                       help='model prefix')
    parser.add_argument('--monitor', dest='monitor', type=int, default=0,
                        help='log network parameters every N iters if larger than 0')
    train.add_argument('--load-epoch', type=int,
                       help='load the model on an epoch using the model-load-prefix')
    train.add_argument('--top-k', type=int, default=0,
                       help='report the top-k accuracy. 0 means no report.')
    train.add_argument('--test-io', type=int, default=0,
                       help='1 means test reading speed without training')
    return train

def fit(args, network, data_loader, **kwargs):
    """
    train a model
    args : argparse returns
    network : the symbol definition of the nerual network
    data_loader : function that returns the train and val data iterators
    """
    # kvstore
# kvstore主要是解决你的梯度更新是在cpu进行还是gpu进行，这里主要调用kvstore.py脚本的create函数，
# 路径是~/mxnet/python/mxnet/kvstore.py。这里的kv_store默认是‘device’，表示在GPU上计算梯度和更新权重，
# 如果是要分布式训练，可以修改成’dist_device_sync‘。如果是在cpu上更新，那么kv_store就要设置为‘local’，
# 不过一般不会这么做，详细可以参考kvstore.py这个脚本。
    kv = mx.kvstore.create(args.kv_store)

    # logging
#这个主要是日志细节信息，可以参考另一篇博文：[MXNet的fine-tune.py源码详解]
    head = '%(asctime)-15s Node[' + str(kv.rank) + '] %(message)s'
    logging.basicConfig(level=logging.DEBUG, format=head)
    logging.info('start with arguments %s', args)

    # data iterators
# 数据导入主要在fine-tune.py脚本中调用data.py的get_rec_iter函数时候已经做好了，
# 所以这一部分做的就很少，而且test_io默认是0，那个if语句也就进不去了。
    (train, val) = data_loader(args, kv)
    if args.test_io:
        tic = time.time()
        for i, batch in enumerate(train):
            for j in batch.data:
                j.wait_to_read()
            if (i+1) % args.disp_batches == 0:
                logging.info('Batch [%d]\tSpeed: %.2f samples/sec' % (
                    i, args.disp_batches*args.batch_size/(time.time()-tic)))
                tic = time.time()

        return

    # load model
#导入模型参数已经在fine-tune.py中做好了，这里就是简单地一个赋值。当然如果你之前导入模型没做好，这个还有个else可以导入。
    if 'arg_params' in kwargs and 'aux_params' in kwargs:
        arg_params = kwargs['arg_params']
        aux_params = kwargs['aux_params']
    else:
        sym, arg_params, aux_params = _load_model(args, kv.rank)
        if sym is not None:
            assert sym.tojson() == network.tojson()

    # save model
# 保存模型就是你训练结束后生成的.param文件，跳到前面的_save_model函数
    checkpoint = _save_model(args, kv.rank)

    # devices for training
#设置在cpu还是gpu上训练，因为默认的gpus参数没有值，所以你如果要在gpu上训练，需要在启动训练的时候加上类似--gpus 0这样的参数。
    devs = mx.cpu() if args.gpus is None or args.gpus is '' else [
        mx.gpu(int(i)) for i in args.gpus.split(',')]

    # learning rate
#关于学习率的改变
    lr, lr_scheduler = _get_lr_scheduler(args, kv)

    # create model
# 这一步比较重要，通过mx.mod.Module函数生成model对象，注意这里的symbol就是网络结构，用fit函数的输入之一network赋值。
# 如果你需要在训练的时候固定一些层的参数不更新，只更新部分层的参数，那么可以在生成这个model对象的时候加上
# 类似fixed_param_names = [‘layer_name1’，‘layer_name2’]这样的参数，表示这两个参数不参与更新。
    model = mx.mod.Module(
        context       = devs,
        symbol        = network
    )

    lr_scheduler  = lr_scheduler
    optimizer_params = {
            'learning_rate': lr,
            'momentum' : args.mom,
            'wd' : args.wd,
            'lr_scheduler': lr_scheduler,
            'multi_precision': True}


# monitor参数可以用于设置迭代多少次就显示下网络的参数情况
    monitor = mx.mon.Monitor(args.monitor, pattern=".*") if args.monitor > 0 else None


# 这一部分主要是参数的初始化，可以看到随机初始化主要采用gaussion，不过现在一般都采用fine-tune，
# 即用别人的模型的参数来初始化你的模型，随机初始化用的并不多了。
    if args.network == 'alexnet':
        # AlexNet will not converge using Xavier
        initializer = mx.init.Normal()
    else:
        initializer = mx.init.Xavier(
            rnd_type='gaussian', factor_type="in", magnitude=2)
    # initializer   = mx.init.Xavier(factor_type="in", magnitude=2.34),


# 评价标准，默认采用准确率，top_k是表示预测的类别概率最高的前k个包含真实概率即算预测正确，常见ImageNet比赛的top_5。
    # evaluation metrices
    eval_metrics = ['accuracy']
    if args.top_k > 0:
        eval_metrics.append(mx.metric.create('top_k_accuracy', top_k=args.top_k))

    # callbacks that run after each batch
# 这个batch_end_callbacks简单讲就是显示训练到第几个epoch了，第几个batch了以及训练速度等信息。
# 这里的dis_batches参数就是你在训练界面看到的多少batch后显示结果，默认是20。
    batch_end_callbacks = [mx.callback.Speedometer(args.batch_size, args.disp_batches)]
    if 'batch_end_callback' in kwargs:
        cbs = kwargs['batch_end_callback']
        batch_end_callbacks += cbs if isinstance(cbs, list) else [cbs]

    # run
# 最后是最重要的训练启动入口，这个model.fit()表示调用model对象的fit函数，
# 这个model对象是前面create model时候通过mx.mod.Module生成的，详细可以查看~/mxnet/python/mxnet/model.py脚本，
# 里面包含最重要的fit()函数，下一篇博客细讲这个函数。
    model.fit(train,
        begin_epoch        = args.load_epoch if args.load_epoch else 0,
        num_epoch          = args.num_epochs,
        eval_data          = val,
        eval_metric        = eval_metrics,
        kvstore            = kv,
        optimizer          = args.optimizer,
        optimizer_params   = optimizer_params,
        initializer        = initializer,
        arg_params         = arg_params,
        aux_params         = aux_params,
        batch_end_callback = batch_end_callbacks,
        epoch_end_callback = checkpoint,
        allow_missing      = True,
        monitor            = monitor)
```



