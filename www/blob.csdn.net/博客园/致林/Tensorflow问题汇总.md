
# Tensorflow问题汇总 - 致林 - 博客园






# [Tensorflow问题汇总](https://www.cnblogs.com/bincoding/p/8270894.html)
**问题：**
> Cannot assign a device for operation 'MatMul': Operation was explicitly assigned to /device:GPU:1 but available devices are [ /job:localhost/replica:0/task:0/device:CPU:0, /job:localhost/replica:0/task:0/device:GPU:0 ]. Make sure the device specification refers to a valid device.
解决方法：
方法一. 卸载tensorflow，安装tensorflow-gpu
pip uninstall tensorflow
pip install tensorflow-gpu
如果还是出问题请尝试：
方法二. 将乘法操作移到gpu以外执行
示例：
\#oldwith tf.Session() as sess:
    matrix1= tf.constant([[3., 3.]])print("matrix1:",matrix1)
    matrix2= tf.constant([[2.],[2.]])print("matrix2:",matrix2)    
    with tf.device("/gpu:1"):\#将乘法运算移到外面执行product =tf.matmul(matrix1, matrix2)
result=sess.run(product)print("result:",result)\#newwith tf.Session() as sess:
    matrix1= tf.constant([[3., 3.]])print("matrix1:",matrix1)
    matrix2= tf.constant([[2.],[2.]])print("matrix2:",matrix2)
    product=tf.matmul(matrix1, matrix2)
with tf.device("/gpu:1"):
        result=sess.run(product)print("result:",result)
正确结果：
C:\Users\bin>python d:/PycharmProjects/TFLearn/Unit1/03.py2018-01-1122:43:03.490996: I C:\tf_jenkins\home\workspace\rel-win\M\windows-gpu\PY\36\tensorflow\core\platform\cpu_feature_guard.cc:137] Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX2018-01-1122:43:04.077880: I C:\tf_jenkins\home\workspace\rel-win\M\windows-gpu\PY\36\tensorflow\core\common_runtime\gpu\gpu_device.cc:1030] Found device0with properties:
name: GeForce GT 740M major:3minor:5memoryClockRate(GHz):1.0325pciBusID:0000:01:00.0totalMemory:1.00GiB freeMemory:836.07MiB2018-01-1122:43:04.078060: I C:\tf_jenkins\home\workspace\rel-win\M\windows-gpu\PY\36\tensorflow\core\common_runtime\gpu\gpu_device.cc:1120] Creating TensorFlow device (/device:GPU:0) -> (device:0, name: GeForce GT 740M, pci busid:0000:01:00.0, compute capability:3.5)
matrix1: Tensor("Const:0", shape=(1,2), dtype=float32)
matrix2: Tensor("Const_1:0", shape=(2,1), dtype=float32)
result: [[12.]]
**问题：**
> Could not find 'cudnn64_6.dll'. TensorFlow requires that this DLL be installed in a directory that is named in your %PATH% environment variable. Note that installing cuDNN is a separate step from installing CUDA, and this DLL is often found in a different directory from the CUDA DLLs. You may install the necessary DLL by downloading cuDNN 6 from this URL: https://developer.nvidia.com/cudnn
解决方法：
1. 安装cudn:
https://developer.nvidia.com/rdp/cudnn-download
https://developer.nvidia.com/cuda-zone
nvidia官网经常更新，请下载对应的版本，cudn和cudnn dll 版本号不一样，比如tensorflow-gpu 1.4对应cndn8，同时需要下载cudnn64_6.dll for cudn8。
2. 下载cudnn64_6，解压到指定cudn目录 C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0
[pan.baidu.com/s/1o8mc4Y](https://link.zhihu.com/?target=http%3A//pan.baidu.com/s/1o8mc4YI)windows
[pan.baidu.com/s/1hs23Hr](https://link.zhihu.com/?target=http%3A//pan.baidu.com/s/1hs23HrA)linux





