# tensorflow学习笔记（四）：跑一跑yolo tensorflow - sinat_31425585的博客 - CSDN博客
2018年07月03日 18:38:31[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：813
    参考博客：https://blog.csdn.net/thm225679/article/details/79407619
    首先，下载yolo tensorflow及预先训练好的模型：
`https://github.com/hizhangp/yolo_tensorflow`
   链接: [https://pan.baidu.com/s/1htt9YBE](http://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1htt9YBE)  密码: ehw2
    然后，在yolo_tensorflow-master文件夹下新建data文件夹，在data文件夹下新建weights子文件夹，并将训练好的模型放到weights文件夹下。
    最后，在yolo_tensorflow-master文件夹下运行cmd，输入：    
```python
python test.py
```
    由于安装tensorflow版本问题，可能会报错：
```python
C:\Users\Administrator\Desktop\yolo_tensorflow-master>python test.py
Traceback (most recent call last):
  File "test.py", line 211, in <module>
    main()
  File "test.py", line 197, in main
    yolo = YOLONet(False)
  File "C:\Users\Administrator\Desktop\yolo_tensorflow-master\yolo\yolo_net.py",
 line 41, in __init__
    is_training=is_training)
  File "C:\Users\Administrator\Desktop\yolo_tensorflow-master\yolo\yolo_net.py",
 line 69, in build_network
    net, 64, 7, 2, padding='VALID', scope='conv_2')
  File "d:\softW\Anaconda3\lib\site-packages\tensorflow\contrib\framework\python
\ops\arg_scope.py", line 181, in func_with_args
    return func(*args, **current_args)
  File "d:\softW\Anaconda3\lib\site-packages\tensorflow\contrib\layers\python\la
yers\layers.py", line 1039, in convolution
    outputs = activation_fn(outputs)
  File "C:\Users\Administrator\Desktop\yolo_tensorflow-master\yolo\yolo_net.py",
 line 244, in op
    return tf.nn.leaky_relu(inputs, alpha=alpha, name='leaky_relu')
AttributeError: module 'tensorflow.python.ops.nn' has no attribute 'leaky_relu'
```
  tensorflow.python.ops.nnl没有leaky_relu属性，解决方法：
修改yolo_net.py文件的第242至245行
```python
def leaky_relu(alpha):
    def op(inputs):
        return tf.nn.leaky_relu(inputs, alpha=alpha, name='leaky_relu')
    return op
```
修改为：
```python
def leaky_relu(alpha):
    def op(inputs):
        return tf.maximum(inputs, alpha * inputs)
    return op
```
输出结果为：
![](https://img-blog.csdn.net/20180703183712353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果想测试其他的图片，可以修改test.py文件的206行，修改为你要测试的图片路径即可。
