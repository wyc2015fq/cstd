# tensorflow学习笔记（六）：tensorflow abject detection api - sinat_31425585的博客 - CSDN博客
2018年07月17日 10:48:56[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：103
## 1、安装cuda9.0及cudnn7.0
     安装cuda9.0很简单，直接下载cuda_9.0.176_windows.exe及cudnn-9.0-windows7-x64-v7.zip，这里cudnn下载需要注册。cudnn安装，其实就是讲cudnn解压后，文件夹中的bin、include、lib三个子文件夹复制到路径：C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v9.0下面去即可。
## 2、Anaconda安装：
      找对应于python3.5的版本，也是直接一波流，傻瓜式安装。
## 3、tensorflow1.5安装
     如果你的网够好，还有梯子，可以直接尝试：  
```python
pip install tensorflow-gpu==1.5.1
```
    还有一种方式，就是直接下载tensorflow_gpu-1.5.1-cp35-cp35m-win_amd64.whl文件，这里给一个链接：[tensorflow1.5.1二进制安装包](https://download.csdn.net/download/sinat_31425585/10545555)，下载完以后，在该二进制文件所在目录运行： 
```python
pip install tensorflow_gpu-1.5.1-cp35-cp35m-win_amd64.whl
```
    等待安装完成即可。
## 4、下载tensorflow的models模块，并解压：
[https://github.com/tensorflow/models](https://github.com/tensorflow/models)
## 5、编译protobuf：
     首先，下载protoc-3.4.0-win32.zip，解压，并将bin目录添加进系统环境变量中，重启电脑使得系统环境变量生效，然后，在路径models/research/下，运行：
```python
protoc object_detection/protos/*.proto --python_out=.
```
      运行完了以后，在models\research\object_detection\protos路径下会生成一些编译完成的.proto文件。
## 6、添加tensorflow路径：
      在路径Anaconda3\Lib\site-packages下新建tensorflow_model.pth文件，文件内容为：
```python
F:\models\research
F:\models\research\slim
```
## 7、测试安装是否成功：
      在models的research文件夹下，打开cmd窗口，运行：
```python
python object_detection/builders/model_builder_test.py
```
![测试效果](https://img-blog.csdn.net/20180717102402188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 8、验证api的检测效果：
     在models\research\object_detection路径下运行：
```python
jupyter notebook
```
     打开object_detection_tutorial.ipynb文件，然后一路ctrl+回车，运行所以的cell：
![object detect api的运行效果](https://img-blog.csdn.net/20180717103252106?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 9、参考资料：
[https://blog.csdn.net/zlase/article/details/78734138](https://blog.csdn.net/zlase/article/details/78734138)
[https://blog.csdn.net/mr_jor/article/details/79071963](https://blog.csdn.net/mr_jor/article/details/79071963)
[https://zhuanlan.zhihu.com/p/35854575](https://zhuanlan.zhihu.com/p/35854575)
[https://www.cnblogs.com/qcloud1001/p/7677661.html](https://www.cnblogs.com/qcloud1001/p/7677661.html)
