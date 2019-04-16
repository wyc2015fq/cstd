# 基于AWS使用深度学习的10个Linux命令 - 我相信...... - CSDN博客





2017年09月04日 21:56:53[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1431
所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)









在AWS上执行大规模的深度学习处理是一个廉价而且有效的学习和开发方式。花少量的钱就可以使用数十GB的内存，数十个CPU，多个GPU，这是值得推荐的。

如果你是使用EC2或者Linux 命令的新人，在云端执行深度学习脚本的时候， 这些命令是非常有效的。 

本文的主要内容包括： 

1）在本机和EC2实例间复制数据 

2）使脚本按天，周，月安全地运行 

3）监测进程，系统和GPU 的性能
注意：所有命令在类linux环境中执行（Linux，OS x 或者 cygwin）

### 0、环境约定

假设AWS EC2 已经正常运行，方便起见，对环境做如下的设定： 

1）EC2 服务器的IP地址为 54.218.86.47 

2）用户名为ec2-user 

3）SSH 密钥位于 ~/.ssh/ 中，文件名为aws-keypair.pem; 

4）使用python 脚本工作
关于如何搭建一个 基于GPU的EC2实例来 执行深度学习，可以参见： 
[https://machinelearningmastery.com/develop-evaluate-large-deep-learning-models-keras-amazon-web-services/](https://machinelearningmastery.com/develop-evaluate-large-deep-learning-models-keras-amazon-web-services/)

### 1、 登陆到服务器

在做任何操作之前，首先要登陆到目标服务器。简单地，使用SSH命令。将SSH 密钥存储在 ~/.ssh/ 中，使用有意义的文件名，例如aws-keypair.pem。使用如下命令登陆EC2主机，注意地址和用户名：

> 
ssh -i ~/.ssh/aws-keypair.pem ec2-user@54.218.86.47


### 2、 拷贝文件到服务器

使用SCP命令拷贝本地文件到服务器，例如将script.py 文件拷贝到EC2 服务器的命令如下：

> 
scp -i ~/.ssh/aws-keypair.pem script.py ec2-user@54.218.86.47:~/


### 3、使脚本在服务器的后台运行

在服务的后台执行脚本，可以忽略其他进行的信号量，忽略标准的输入输出，将所有的输出和错误信息重定向到一个日志文件中。对于需要长时间运行的深度学习模型而言， 这是非常必要的。
`> nohup python /home/ec2-user/script.py >/home/ec2-user/script.py.log </dev/null 2>&1 &`
该命令中script.py 和 script.py.log 都位于 ／home/ec2-user/ 目录下。关于 nohup 和重定向 参考其他的详细介绍（例如wikipedia中的介绍）。

### 4、在服务器的指定 GPU 上执行脚本

如果EC2 支持的话，推荐在同时运行多个脚本。例如，EC2有4个GPU的话， 可以在每个GPU上单独运行一个脚本，示例代码如下：
`CUDA_VISIBLE_DEVICES=0  nohup python /home/ec2-user/script.py >/home/ec2-user/script.py.log </dev/null 2>&1 &`
如果有4个GPU的话，可以指定CUDA_VISIBLE_DEVICES从0到3。这在TF做后台的Keras上是可行的，在Theano 没有测试过。

关于CUDA_VISIBLE_DEVICES 的更多信息可以参见[https://devblogs.nvidia.com/parallelforall/cuda-pro-tip-control-gpu-visibility-cuda_visible_devices/](https://devblogs.nvidia.com/parallelforall/cuda-pro-tip-control-gpu-visibility-cuda_visible_devices/)

### 5、监测脚本的输出

如果输出结果中有项目评分或者一个算法的运行结果，实时监控脚本的输出是很有意义的。示例如下：
`tail -f script.py.log`
遗憾的是，当屏幕上在一段时间没有输出的时候，AWS 会关闭这个终端，所以最好使用：

```bash
watch "tail script.py.log"
```

有的时候看不到python的标准输出，不知道是python的问题还是EC2的问题。

### 6、监测系统和进程的性能

监测EC2系统的性能是有意义的，尤其是已经使用了或还剩下多少内存。例如：
`top -M`
或者指定进程标识PID：
`top -p PID -M`
### 7、监测GPU 性能

如果在GPU上同时执行多个脚本，并行执行的话，查看每个GPU 的性能和使用率是不错的主意。例如：

```bash
watch "nvidia-smi"
```

### 8、检查脚本是否还在服务器上运行

一般地，会保持终端一直是开着的。

```bash
watch "ps -ef | grep python"
```

### 9、在服务器上编辑文件

一般不建议在服务器直接修改，当然你熟知vi除外：
`vi ~/script.py`
vi 的用法就不在这里赘述了。

### 10、从服务器上下载文件

与上传文件相对，这是一个下个png文件的例子：
`scp -i ~/.ssh/aws-keypair.pem ec2-user@54.218.86.47:~/*.png .`
### 需要注意的几点
- 如果希望同时运行多个脚本，最好选用拥有多个GPU 的EC2
- 最好在本地编写脚本
- 将执行结果输出到文件，下载到本地进行分析
- 使用watch 命令保持终端处于运行中
- 在本地执行远程命令

原文参见 [https://machinelearningmastery.com/command-line-recipes-deep-learning-amazon-web-services/](https://machinelearningmastery.com/command-line-recipes-deep-learning-amazon-web-services/)





