
# ubuntu16.04下安装CUDA，cuDNN及tensorflow-gpu版本过程 - Multiangle's Notepad - CSDN博客


2016年12月19日 00:52:46[multiangle](https://me.csdn.net/u014595019)阅读数：41578标签：[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[cuda																](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)[nvidia																](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)[tensorflow																](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[cudnn																](https://so.csdn.net/so/search/s.do?q=cudnn&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)个人分类：[linux																](https://blog.csdn.net/u014595019/article/category/6352832)[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[
							](https://blog.csdn.net/u014595019/article/category/6352832)
[
				](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)

---
这篇文章主要依据两篇文章：
[深度学习主机环境配置: Ubuntu16.04+Nvidia GTX 1080+CUDA8.0](http://www.52nlp.cn/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E4%B8%BB%E6%9C%BA%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE-ubuntu-16-04-nvidia-gtx-1080-cuda-8)
[深度学习主机环境配置: Ubuntu16.04+GeForce GTX 1080+TensorFlow](http://www.52nlp.cn/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E4%B8%BB%E6%9C%BA%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE-ubuntu16-04-geforce-gtx1080-tensorflow)
不过在实际运行的过程中，有一定的不同之处，随着时间的推移，一些组件已经可以更方便的安装，不再需要自己编译了。一些流程也有所更改。因此我在这里把自己在ubuntu16.04下安装cuda,cudnn和gpu版tensorflow的流程写下来，供人参考。
---

# 安装显卡驱动
首先安装显卡驱动。首先看自己显卡
```python
lspci
```
```python
| grep -i vga
```
```python
lspci
```
```python
| grep -i nvidia
```
然后看显卡驱动
```python
lsmod
```
```python
| grep -i nvidia
```
在ubuntu16.04中，更换驱动非常方便，去
系统设置->软件更新->附加驱动->切换到最新的NVIDIA驱动即可。应用更改->重启
![选区_052.png-132.8kB](http://static.zybuluo.com/multiangle/5i4vcxw2kaq61t32nyb5oyto/%E9%80%89%E5%8C%BA_052.png)
再运行`nvidia-smi`来看看
![选区_053.png-55.8kB](http://static.zybuluo.com/multiangle/9i2f2favpomdlken8h3f0snk/%E9%80%89%E5%8C%BA_053.png)
---

# 安装CUDA
去[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads)，根据自己的系统下载对应安装包,下载的是runfile文件，而不是deb文件。下载好以后，运行
```python
sudo ./cuda_8
```
```python
.0
```
```python
.44
```
```python
_linux.
```
```python
run
```
```python
--tmpdir=/tmp
```
开始安装。后面参数 tmpdir好像是为了放日志，具体是什么日志忘了=。= 安装开始以后，首先是一个协议，一直按回车到底以后，输入accept。其他的操作如下所示
```python
Do
```
```python
you accept the previously
```
```python
read
```
```python
EULA?
accept/decline/quit: accept
Install NVIDIA Accelerated Graphics Driver
```
```python
for
```
```python
Linux-x86_64
```
```python
367.48
```
```python
?
(y)es/(n)o/(q)uit: n
Install the CUDA
```
```python
8.0
```
```python
Toolkit?
(y)es/(n)o/(q)uit: y
Enter Toolkit Location
[
```
```python
default
```
```python
is
```
```python
/usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
]:
```
```python
Do
```
```python
you want
```
```python
to
```
```python
install a symbolic link
```
```python
at
```
```python
/usr/
```
```python
local
```
```python
/cuda?
(y)es/(n)o/(q)uit: y
Install the CUDA
```
```python
8.0
```
```python
Samples?
(y)es/(n)o/(q)uit: y
```
下面是安装时的输出信息：
```python
Installing
```
```python
the
```
```python
CUDA Toolkit
```
```python
in
```
```python
/usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
…
Installing
```
```python
the
```
```python
CUDA Samples
```
```python
in
```
```python
/home/textminer …
Copying samples
```
```python
to
```
```python
/home/textminer/NVIDIA_CUDA-
```
```python
8.0
```
```python
_Samples now…
Finished copying samples.
===========
= Summary =
===========
Driver: Not Selected
Toolkit: Installed
```
```python
in
```
```python
/usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
Samples: Installed
```
```python
in
```
```python
/home/textminer
Please make sure
```
```python
that
```
```python
– PATH includes /usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
/bin
– LD_LIBRARY_PATH includes /usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
/lib64,
```
```python
or
```
```python
, add /usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
/lib64
```
```python
to
```
```python
/etc/ld.so.conf
```
```python
and
```
```python
run
```
```python
ldconfig
```
```python
as
```
```python
root
To uninstall
```
```python
the
```
```python
CUDA Toolkit,
```
```python
run
```
```python
the
```
```python
uninstall
```
```python
script
```
```python
in
```
```python
/usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
/bin
Please see CUDA_Installation_Guide_Linux.pdf
```
```python
in
```
```python
/usr/
```
```python
local
```
```python
/cuda-
```
```python
8.0
```
```python
/doc/pdf
```
```python
for
```
```python
detailed information
```
```python
on
```
```python
setting up CUDA.
***WARNING: Incomplete installation! This installation did
```
```python
not
```
```python
install
```
```python
the
```
```python
CUDA Driver. A driver
```
```python
of
```
```python
version
```
```python
at
```
```python
least
```
```python
361.00
```
```python
is
```
```python
required
```
```python
for
```
```python
CUDA
```
```python
8.0
```
```python
functionality
```
```python
to
```
```python
work.
To install
```
```python
the
```
```python
driver using this installer,
```
```python
run
```
```python
the
```
```python
following command, replacing
```
```python
with
```
```python
the
```
```python
name
```
```python
of
```
```python
this
```
```python
run
```
```python
file
```
```python
:
sudo .
```
```python
run
```
```python
-silent -driver
Logfile
```
```python
is
```
```python
/tmp/cuda_install_6583.
```
```python
log
```
安装完毕后，再声明一下环境变量，并将其写入到 ~/.bashrc 的尾部:
```python
export
```
```python
CUDA_HOME=/usr/local/cuda
```
```python
export
```
```python
PATH=
```
```python
$PATH
```
```python
:
```
```python
$CUDA_HOME
```
```python
/bin
```
```python
export
```
```python
LD_LIBRARY_PATH=/usr/local/cuda-
```
```python
8.0
```
```python
/lib64
```
```python
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}
```
```python
}
```
保存退出，运行`source ~/.bashrc`
测试是否安装成功
```python
// 如果怕把samples搞坏了那就先搞一个备份，在备份里搞
```
```python
cd
```
```python
/usr/local/cuda/samples/
```
```python
1
```
```python
_Utilities/deviceQuery
```
```python
sudo
```
```python
make
./deviceQuery
```
结果如下
![选区_054.png-169.8kB](http://static.zybuluo.com/multiangle/sz6g8svpy9y1gohg8u3e4lak/%E9%80%89%E5%8C%BA_054.png)
之前有看到说要把gcc改成4.9的。不过我现在用5.4的也可以编译，就是有几个警告，不知道会不会有什么大的影响。
---

# cuDNN安装
如果要使用gpu来对tensorflow进行加速，除了安装CUDA以外，cuDNN也是必须要安装的。跟cuda一样，去nvidia的官网下载cuDNN的安装包。不过这次没法直接下载，需要先注册，然后还要做个调查问卷什么的，稍微有点麻烦。我下的是`cuDNN v5.1 Library for Linux`这个版本。不要下`cuDNN v5.1 Developer Library for Ubuntu16.04 Power8 (Deb)`这个版本,因为是给powe8处理器用的，不是amd64.
下载下来以后，发现是一个tgz的压缩包，使用tar进行解压
```python
tar
```
```python
-xvf
```
```python
cudnn
```
```python
-
```
```python
8.0
```
```python
-linux
```
```python
-x64
```
```python
-v5
```
```python
.1
```
```python
.
```
```python
tgz
```
安装cuDNN比较简单，解压后把相应的文件拷贝到对应的CUDA目录下即可
```python
sudo cp cuda/
```
```python
include
```
```python
/cudnn.h /usr/
```
```python
local
```
```python
/cuda/
```
```python
include
```
```python
/
sudo cp cuda/lib64/libcudnn* /usr/
```
```python
local
```
```python
/cuda/lib64/
sudo chmod
```
```python
a
```
```python
+r /usr/
```
```python
local
```
```python
/cuda/
```
```python
include
```
```python
/cudnn.h
sudo chmod
```
```python
a
```
```python
+r /usr/
```
```python
local
```
```python
/cuda/lib64/libcudnn*
```
---

# tensorflow安装
我之前已经安装了cpu-only版的tensorflow，所以现在要先把原先的tf卸载
```python
sudo
```
```python
pip uninstall tensorflow
```
之后从[https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc1-cp35-cp35m-linux_x86_64.whl](https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc1-cp35-cp35m-linux_x86_64.whl)下载gpu版的tf
```python
sudo pip install tensorflow_gpu
```
```python
-
```
```python
0.12
```
```python
.0
```
```python
rc1
```
```python
-cp35
```
```python
-cp35m
```
```python
-linux_x86_64
```
```python
.
```
```python
whl
```
来测试一下
```python
$ipython
```
```python
import tensorflow
```
```python
as
```
```python
tf
```
结果如下
![选区_055.png-81.5kB](http://static.zybuluo.com/multiangle/xprt5y85bfulkgatts58tva8/%E9%80%89%E5%8C%BA_055.png)
没有报错，说明已经安装成功了。
---

# 在idea中调用tensorflow
之前在命令行中已经调用tensorflow成功了，没想到在idea中写程序时调用tensorflow又出现了错误：
```python
ImportError
```
```python
:
```
```python
libcudart.so.8.0: cannot open shared object file: No such file or directory
```
```python
Error
```
```python
importing tensorflow.  Unless you are
```
```python
using
```
```python
bazel,
you should
```
```python
not
```
```python
try
```
```python
to
```
```python
import tensorflow
```
```python
from
```
```python
its source directory;
please
```
```python
exit
```
```python
the tensorflow source tree,
```
```python
and
```
```python
relaunch your python interpreter
```
```python
from
```
```python
there.
```
![选区_056.png-10kB](http://static.zybuluo.com/multiangle/skdl9o9rwdm8fybd9trmrwfs/%E9%80%89%E5%8C%BA_056.png)
但是我并没有在tf的根目录下运行呀？而且同一个文件用命令行是可以运行的呀？为什么换idea来就不行了捏？
后来经过尝试发现，问题还是出在环境变量LD_LIBRARY_PATH中。光把环境变量写在~/.bashrc中是不行的，还需要写在/etc/profile下。因此，可以在/etc/profile的结尾处加上
```python
## cuda
```
```python
export
```
```python
CUDA_HOME=/usr/local/cuda
```
```python
export
```
```python
PATH=
```
```python
$PATH
```
```python
:
```
```python
$CUDA_HOME
```
```python
/bin
```
```python
export
```
```python
LD_LIBRARY_PATH=/usr/local/cuda-
```
```python
8.0
```
```python
/lib64
```
```python
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}
```
```python
}
```
登出后重新登陆，再打开idea，就可以在idea中调用tf啦
![选区_057.png-25.9kB](http://static.zybuluo.com/multiangle/4wf5s8zd5aspnb5opdc96m0o/%E9%80%89%E5%8C%BA_057.png)

