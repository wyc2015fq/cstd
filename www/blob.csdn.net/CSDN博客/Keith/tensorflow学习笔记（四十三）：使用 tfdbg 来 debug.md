# tensorflow学习笔记（四十三）：使用 tfdbg 来 debug - Keith - CSDN博客





2017年08月22日 14:14:13[ke1th](https://me.csdn.net/u012436149)阅读数：6693
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









由于 `tensorflow` 在训练的时候是在后台运行的，所以使用 python 的 debug 工具来 debug tensorflow 的执行过程是不可行的，为此，官方提供了一款debug 工具，名为 **tfdbg**

有很多人觉得，为了 debug tensorflow 的计算过程又要学习一个新的工具，很烦。

但其实不然，**tfdbg** 用起来是十分简单的。以至于简单到我们**只需要增加两行代码**，就可以将之前的模型改成可 `debug` 的。

在 **debug** 界面，也只需熟悉几个常用的命令就可以了。

```python
# 第一行： 引包
from tensorflow.python import debug as tf_debug

sess = tf.Session()
# 初始化的 sess 没必要加上 debug wrapper
sess.run(tf.global_variables_initializer())

# 第二行，给 session 加个 wrapper
debug_sess = tf_debug.LocalCLIDebugWrapperSession(sess=sess)
debug_sess.run(train_op) # 用 加了 wrapper 的 session，来代替之前的 session 做训练操作
```

好了，这样就可以了，然后命令行执行：

```
python demo_debug.py
# 或者
python -m demo_debug
```

不一会，下面界面就会出现，就可以开心的 `debug` 了

![这里写图片描述](https://img-blog.csdn.net/20170822141325084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
**Tips** : debug 界面中 带 下划线的 东西 都是可以用鼠标点一下，就会触发相应操作的


关于debug 界面的命令，官方文档有详细的说明 [https://www.tensorflow.org/programmers_guide/debugger#debugging_model_training_with_tfdbg](https://www.tensorflow.org/programmers_guide/debugger#debugging_model_training_with_tfdbg)

核心的几个命令是：
- run ：执行一次 debug_session.run() , 这次执行产生中间 tensor 的值都可以通过 debug 界面查看
- exit ： 退出 debug

## 注意事项
- debug 的 wrapper 要加在 执行 `train_op` 的 session 上，因为要 debug 的是 `train` 过程。但是如果是想 debug  input-pipeline 的话，感觉是可以将 wrapper 加在 执行 input-pipeline 的 session 上的（没有测试过）。
- 如果代码中使用了 input-pipeline 的话， debug 非常慢（不知道原因是啥） 

## 参考资料

[https://www.tensorflow.org/programmers_guide/debugger](https://www.tensorflow.org/programmers_guide/debugger)



