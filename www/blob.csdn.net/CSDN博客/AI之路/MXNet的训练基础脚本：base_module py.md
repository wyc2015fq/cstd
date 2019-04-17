# MXNet的训练基础脚本：base_module.py - AI之路 - CSDN博客





2017年07月15日 09:17:40[AI之路](https://me.csdn.net/u014380165)阅读数：1527
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**写在前面：在MXNet中有一个很重要的脚本：base_module.py，这个脚本中的BaseModule类定义了和模型实现相关的框架。另外还有一个脚本module.py会在另外一篇博客中讲，这个类继承BaseModule类，并进行了具体的实现，比如forward方法，update_metric方法等。**

**在创建模型的时候用的是mx.mod.Module这个类，比如：model = mx.mod.Module(symbol = sym)，具体这个Module类的内容是什么？其实mx.mod.Module准确地写应该是mxnet.module.Module()**，这个类的路径是~/mxnet/python/mxnet/module/module.py，但是在实际调用的时候是调用python库中的module.py，路径是：/usr/local/lib/python2.7/dist-packages/mxnet-0.10.1-py2.7.egg/mxnet/module/module.py，这个mxnet-0.10.1-py2.7.egg就是在编译python，即运行setup.py时候生成的。module.py这个脚本中定义了Module类，而这个类继承另一个类BaseModule，BaseModule这个类是在base_module.py脚本中定义的，该脚本和module.py脚本在同一目录下。**因为fit()方法继承自BaseModule类并且在BaseModule类中实现，Module类中并没有做修改，所以在BaseModule类中主要看fit()方法**。注意到在BaseModule类中其他方法最后都有一句raise NotImplementedError()，这句话是引发未实现的异常的，也就是说如果你继承了BaseModule类却没有实现一些方法（带raise这句话），那么在调用这个方法的时候就会引发异常。

base_module.py代码的git地址： 
[https://github.com/dmlc/mxnet/blob/master/python/mxnet/module/base_module.py](https://github.com/dmlc/mxnet/blob/master/python/mxnet/module/base_module.py)

**base_module.py中最重要的就是BaseModule类**，接下来截取这个类中重要的几个函数来介绍，大部分函数只是在这里定义，具体实现一般由别的类通过继承该类进行实现：

```
class BaseModule(object):
"""The base class of a module.

    A module represents a computation component. One can think of module as a computation machine.
    A module can execute forward and backward passes and update parameters in a model.
    We aim to make the APIs easy to use, especially in the case when we need to use the imperative
    API to work with multiple modules (e.g. stochastic depth network).

A module has several states:

    - Initial state: Memory is not allocated yet, so the module is not ready for computation yet.
    - Binded: Shapes for inputs, outputs, and parameters are all known, memory has been allocated,
      and the module is ready for computation.
    - Parameters are initialized: For modules with parameters, doing computation before
      initializing the parameters might result in undefined outputs.
    - Optimizer is installed: An optimizer can be installed to a module. After this, the parameters
      of the module can be updated according to the optimizer after gradients are computed
      (forward-backward).

In order for a module to interact with others, it must be able to report the
    following information in its initial state (before binding):

    - `data_names`: list of type string indicating the names of the required input data.
    - `output_names`: list of type string indicating the names of the required outputs.

    After binding, a module should be able to report the following richer information:

    - state information
        - `binded`: `bool`, indicates whether the memory buffers needed for computation
          have been allocated.
        - `for_training`: whether the module is bound for training.
        - `params_initialized`: `bool`, indicates whether the parameters of this module
          have been initialized.
        - `optimizer_initialized`: `bool`, indicates whether an optimizer is defined
          and initialized.
        - `inputs_need_grad`: `bool`, indicates whether gradients with respect to the
          input data are needed. Might be useful when implementing composition of modules.

    - input/output information
        - `data_shapes`: a list of `(name, shape)`. In theory, since the memory is allocated,
          we could directly provide the data arrays. But in the case of data parallelism,
          the data arrays might not be of the same shape as viewed from the external world.
        - `label_shapes`: a list of `(name, shape)`. This might be `[]` if the module does
          not need labels (e.g. it does not contains a loss function at the top), or a module
          is not bound for training.
        - `output_shapes`: a list of `(name, shape)` for outputs of the module.

    - parameters (for modules with parameters)
        - `get_params()`: return a tuple `(arg_params, aux_params)`. Each of those
          is a dictionary of name to ``NDArray`` mapping. Those `NDArray` always lives on
          CPU. The actual parameters used for computing might live on other devices (GPUs),
          this function will retrieve (a copy of) the latest parameters. Therefore, modifying
        - ``set_params(arg_params, aux_params)``: assign parameters to the devices
          doing the computation.
        - ``init_params(...)``: a more flexible interface to assign or initialize the parameters.

    - setup
        - `bind()`: prepare environment for computation.
        - `init_optimizer()`: install optimizer for parameter updating.

    - computation
        - `forward(data_batch)`: forward operation.
        - `backward(out_grads=None)`: backward operation.
        - `update()`: update parameters according to installed optimizer.
        - `get_outputs()`: get outputs of the previous forward operation.
        - `get_input_grads()`: get the gradients with respect to the inputs computed
          in the previous backward operation.
        - `update_metric(metric, labels)`: update performance metric for the previous forward
          computed results.

    - other properties (mostly for backward compatibility)
        - `symbol`: the underlying symbolic graph for this module (if any)
          This property is not necessarily constant. For example, for `BucketingModule`,
          this property is simply the *current* symbol being used. For other modules,
          this value might not be well defined.

    When those intermediate-level API are implemented properly, the following
    high-level API will be automatically available for a module:

    - `fit`: train the module parameters on a data set.
    - `predict`: run prediction on a data set and collect outputs.
    - `score`: run prediction on a data set and evaluate performance.
”“”
# 以上这些注释是base_module.py中定义BaseModule类时候的注释，一开始对于bind的理解还不够，看了这些注释后会更清晰一点。
# 这里的源码主要还是intermediate-level API和high-level API，可以看看作者是怎么做封装的。上面这几种状态在后面都会提到，比较重要。

# 初始化，当你写一个类继承BaseModule这个类的时候，一般会重写初始化函数，比如module.py中的Module类
def __init__(self, logger=logging):
        self.logger = logger
        self.binded = False
        self.for_training = False
        self.inputs_need_grad = False
        self.params_initialized = False
        self.optimizer_initialized = False
        self._symbol = None
        self._total_exec_bytes = 0

def score(self, eval_data, eval_metric, num_batch=None, batch_end_callback=None,
              score_end_callback=None,
              reset=True, epoch=0):
        """Runs prediction on ``eval_data`` and evaluates the performance according to
        the given ``eval_metric``.

        Checkout `Module Tutorial <http://mxnet.io/tutorials/basic/module.html>`_ to see
        a end-to-end use-case.

        Parameters
        ----------
        eval_data : DataIter
            Evaluation data to run prediction on.
        eval_metric : EvalMetric or list of EvalMetrics
            Evaluation metric to use.
        num_batch : int
            Number of batches to run. Defaults to ``None``, indicating run until the `DataIter`
            finishes.
        batch_end_callback : function
            Could also be a list of functions.
        reset : bool
            Defaults to ``True``. Indicates whether we should reset `eval_data` before starting
            evaluating.
        epoch : int
            Defaults to 0. For compatibility, this will be passed to callbacks (if any).
            During training, this will correspond to the training epoch number.

        Examples
        --------
        >>> # An example of using score for prediction.
        >>> # Evaluate accuracy on val_dataiter
        >>> metric = mx.metric.Accuracy()
        >>> mod.score(val_dataiter, metric)
        >>> mod.score(val_dataiter, ['mse', 'acc'])
        """
        assert self.binded and self.params_initialized

        if reset:
            eval_data.reset()

        if not isinstance(eval_metric, metric.EvalMetric):
            eval_metric = metric.create(eval_metric)

        eval_metric.reset()
        actual_num_batch = 0

        for nbatch, eval_batch in enumerate(eval_data):
            if num_batch is not None and nbatch == num_batch:
                break


            # 测试时候计算准确率只需要前向走一遍网络，然后计算准确率即可，因此可以看到这部先后调用了forward和update_metric函数
            self.forward(eval_batch, is_train=False)
            self.update_metric(eval_metric, eval_batch.label)


           # batch_end_callback表示如果你需要在每个mini-batch中计算准确率
            if batch_end_callback is not None:
                batch_end_params = BatchEndParam(epoch=epoch,
                                                 nbatch=nbatch,
                                                 eval_metric=eval_metric,
                                                 locals=locals())
                for callback in _as_list(batch_end_callback):
                    callback(batch_end_params)
            actual_num_batch += 1

        # 一般我们习惯计算一整个epoch的准确率
        if score_end_callback:
            params = BatchEndParam(epoch=epoch,
                                   nbatch=actual_num_batch,
                                   eval_metric=eval_metric,
                                   locals=locals())
            for callback in _as_list(score_end_callback):
                callback(params)


       # 这个get_name_value方法前面说过，返回的是元组的列表，元组包含评价指标和值。
        return eval_metric.get_name_value()

def fit(self, train_data, eval_data=None, eval_metric='acc',
            epoch_end_callback=None, batch_end_callback=None, kvstore='local',
            optimizer='sgd', optimizer_params=(('learning_rate', 0.01),),
            eval_end_callback=None,
            eval_batch_end_callback=None, initializer=Uniform(0.01),
            arg_params=None, aux_params=None, allow_missing=False,
            force_rebind=False, force_init=False, begin_epoch=0, num_epoch=None,
            validation_metric=None, monitor=None):
        """Trains the module parameters.

        Checkout `Module Tutorial <http://mxnet.io/tutorials/basic/module.html>`_ to see
        a end-to-end use-case.

        Parameters
        ----------
        train_data : DataIter
            Train DataIter.
        eval_data : DataIter
            If not ``None``, will be used as validation set and the performance
            after each epoch will be evaluated.
        eval_metric : str or EvalMetric
            Defaults to 'accuracy'. The performance measure used to display during training.
            Other possible predefined metrics are:
            'ce' (CrossEntropy), 'f1', 'mae', 'mse', 'rmse', 'top_k_accuracy'.
        epoch_end_callback : function or list of functions
            Each callback will be called with the current `epoch`, `symbol`, `arg_params`
            and `aux_params`.
        batch_end_callback : function or list of function
            Each callback will be called with a `BatchEndParam`.
        kvstore : str or KVStore
            Defaults to 'local'.
        optimizer : str or Optimizer
            Defaults to 'sgd'.
        optimizer_params : dict
            Defaults to ``(('learning_rate', 0.01),)``. The parameters for
            the optimizer constructor.
            The default value is not a dict, just to avoid pylint warning on dangerous
            default values.
        eval_end_callback : function or list of function
            These will be called at the end of each full evaluation, with the metrics over
            the entire evaluation set.
        eval_batch_end_callback : function or list of function
            These will be called at the end of each mini-batch during evaluation.
        initializer : Initializer
            The initializer is called to initialize the module parameters when they are
            not already initialized.
        arg_params : dict
            Defaults to ``None``, if not ``None``, should be existing parameters from a trained
            model or loaded from a checkpoint (previously saved model). In this case,
            the value here will be used to initialize the module parameters, unless they
            are already initialized by the user via a call to `init_params` or `fit`.
            `arg_params` has a higher priority than `initializer`.
        aux_params : dict
            Defaults to ``None``. Similar to `arg_params`, except for auxiliary states.
        allow_missing : bool
            Defaults to ``False``. Indicates whether to allow missing parameters when `arg_params`
            and `aux_params` are not ``None``. If this is ``True``, then the missing parameters
            will be initialized via the `initializer`.
        force_rebind : bool
            Defaults to ``False``. Whether to force rebinding the executors if already bound.
        force_init : bool
            Defaults to ``False``. Indicates whether to force initialization even if the
            parameters are already initialized.
        begin_epoch : int
            Defaults to 0. Indicates the starting epoch. Usually, if resumed from a
            checkpoint saved at a previous training phase at epoch N, then this value should be
            N+1.
        num_epoch : int
            Number of epochs for training.

        Examples
        --------
        >>> # An example of using fit for training.
        >>> # Assume training dataIter and validation dataIter are ready
        >>> # Assume loading a previously checkpointed model
        >>> sym, arg_params, aux_params = mx.model.load_checkpoint(model_prefix, 3)
        >>> mod.fit(train_data=train_dataiter, eval_data=val_dataiter, optimizer='sgd',
        ...     optimizer_params={'learning_rate':0.01, 'momentum': 0.9},
        ...     arg_params=arg_params, aux_params=aux_params,
        ...     eval_metric='acc', num_epoch=10, begin_epoch=3)
        """
        assert num_epoch is not None, 'please specify number of epochs'


# bind函数的官方解释是：Binds the symbols to construct executors. 
# This is necessary before one can perform computation with the module。
# bind函数的具体实现请看module.py脚本。
        self.bind(data_shapes=train_data.provide_data, label_shapes=train_data.provide_label,
                  for_training=True, force_rebind=force_rebind)

# monitor参数主要是用来跟踪训练过程的，一般正常训练时候默认是None
        if monitor is not None:
            self.install_monitor(monitor)

# 这个init_params函数在该脚本中只定义，并未实现，实现可以参看module.py脚本。
# 这几个参数介绍下：initializer是用来初始化参数的，arg_params和aux_params是你导入的模型相关的参数和状态；
# allow_missing默认是False，如果为True，表示如果有参数没有值，会用这个initializer来初始化；
# force_init默认是False，如果为True，表示强制初始化一些已经有值的参数。
        self.init_params(initializer=initializer, arg_params=arg_params, aux_params=aux_params,
                         allow_missing=allow_missing, force_init=force_init)

# 这个init_optimizer函数也是在该脚本中只定义，并未实现，实现可以参看module.py脚本。
# kvstore默认是local，表示在CPU上更新梯度，我们一般用device，表示在GPU上更新。
# optimizer就是优化方式，比如默认的SGD表示随机梯度下降。
# optimizer_params表示优化参数，这里主要是学习率。
        self.init_optimizer(kvstore=kvstore, optimizer=optimizer,
                            optimizer_params=optimizer_params)


# 如果没有指定验证指标的话，那么验证指标和训练指标一致，训练指标默认是准确率。
        if validation_metric is None:
            validation_metric = eval_metric

# isinstance函数表示如果eval_metric的类型是否属于metric.EvalMetric，后者可能是一个类型，也可能是很多类型组成的列表
        if not isinstance(eval_metric, metric.EvalMetric):
            eval_metric = metric.create(eval_metric)

        ################################################################################
        # training loop
        ################################################################################
        for epoch in range(begin_epoch, num_epoch):
            # time.time()是用来记录当前时间的
            tic = time.time()

            # reset是eval_metric对象的一个方法，表示Resets the internal evaluation result to initial state
            eval_metric.reset()
            nbatch = 0
            data_iter = iter(train_data)
            end_of_batch = False
            next_data_batch = next(data_iter)

           # 当执行完所有的batch，则一个epoch结束
            while not end_of_batch:
                data_batch = next_data_batch
                if monitor is not None:
                    monitor.tic()

                # forward_backward函数很重要，这个脚本只是定义了这个函数，具体实现在module.py脚本中，
                # 该函数包含forward和backward两部分，分别表示前向传递和方向传递梯度。update函数表示更新参数。
                self.forward_backward(data_batch)
                self.update()

                # try和except表示异常机制，如果try语句运行出错，则引发except语句，即end_of_batch = True，那么就会在下次循环中退出while循环。
                try:
                    # pre fetch next batch
                    next_data_batch = next(data_iter)
                    self.prepare(next_data_batch)
                except StopIteration:
                    end_of_batch = True


                # update_metric函数表示每计算一个batch，就更新metric结果，在这里就是准确率。具体实现请看module.py脚本。
                self.update_metric(eval_metric, data_batch.label)

                if monitor is not None:
                    monitor.toc_print()

                if batch_end_callback is not None:
                    batch_end_params = BatchEndParam(epoch=epoch, nbatch=nbatch,
                                                     eval_metric=eval_metric,
                                                     locals=locals())
                    for callback in _as_list(batch_end_callback):
                        callback(batch_end_params)
                nbatch += 1

            # one epoch of training is finished
            # get_name_value方法返回的是一个包含元组的列表，元组：(name,value)
            for name, val in eval_metric.get_name_value():
                self.logger.info('Epoch[%d] Train-%s=%f', epoch, name, val)

            # 前面有个tic是开始时间，这里的toc是当前时间，相减就是整个epoch的运行时间 
            toc = time.time()
            self.logger.info('Epoch[%d] Time cost=%.3f', epoch, (toc-tic))

            # sync aux params across devices
            arg_params, aux_params = self.get_params()
            self.set_params(arg_params, aux_params)

            if epoch_end_callback is not None:
                for callback in _as_list(epoch_end_callback):
                    callback(epoch, self.symbol, arg_params, aux_params)

            #----------------------------------------
            # evaluation on validation set
            # 训练数据跑完一个epoch，就需要在测试数据集上测试下这个时候模型的效果，这里主要是调用了score函数，这个函数在前面已经介绍。
            if eval_data:
                res = self.score(eval_data, validation_metric,
                                 score_end_callback=eval_end_callback,
                                 batch_end_callback=eval_batch_end_callback, epoch=epoch)
                #TODO: pull this into default
                for name, val in res:
                    self.logger.info('Epoch[%d] Validation-%s=%f', epoch, name, val)

            # end of 1 epoch, reset the data-iter for another epoch
            train_data.reset()
```



