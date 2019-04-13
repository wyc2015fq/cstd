
# docker︱在nvidia-docker中使用tensorflow-gpu-jupyter - 素质云笔记-Recorder... - CSDN博客

2017年12月06日 11:27:14[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13374标签：[gpu																](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)[cuda																](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)[nvidia																](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=nvidia&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)个人分类：[docker/linux/API部署																](https://blog.csdn.net/sinat_26917383/article/category/7328428)
[
																								](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)

docker小白…
搭建基础：宿主机已经有CUDA8.0
**进出快捷键：**
ctrl+d 退出容器且关闭, docker ps 查看无
ctrl+p+q 退出容器但不关闭, docker ps 查看有
使用docker restart命令重启容器
使用docker attach命令进入容器

---文章目录

---[一、安装](#_19)

---[二、nvidia-smi的使用](#nvidiasmi_88)

---[1、拉取镜像、开启容器](#1_91)

---[2.上传容器与创建镜像](#2_130)

---[3.容器与镜像删减](#3_146)

---[.
4.容器改名](#
4_185)

---[5.容器的保存](#5_195)

---[.
6.在tensorflow容器中打开Jupyter notebook](#
6tensorflowJupyter_notebook_213)

---[主题换色（参考：https://github.com/dunovank/jupyter-themes）：](#httpsgithubcomdunovankjupyterthemes_248)

---[延伸一：如果修改Jupyter notebook密码](#Jupyter_notebook_274)

---[延伸二：报错：OSError: [Errno 99] Cannot assign requested address](#OSError_Errno_99_Cannot_assign_requested_address_293)

---[.
7.在容器中打开nvidia/digits](#
7nvidiadigits_301)

---[三、tensorflow安装](#tensorflow_314)

---[延伸：](#_359)

---[延伸二：](#_366)

---[延伸三：上传至阿里云](#_375)

---[延伸三：docker 之中screen 与主机的环境不一样，py版本不一样](#docker_screen_py_403)

---[参考：](#_424)


---一、安装

---参考：

---[Docker Compose + GPU + TensorFlow = ❤️](https://hackernoon.com/docker-compose-gpu-tensorflow-%EF%B8%8F-a0e2011d36)

---安装docker

---curl -sSL https://get.docker.com/ | sh

---安装Nvidia Docker

---wget -P /tmp https://github.com/NVIDIA/nvidia-docker/releases/download/v1.0.1/nvidia-docker_1.0.1-1_amd64.deb
sudo dpkg -i /tmp/nvidia-docker*.deb

---通过以下命令来检验是否成功：

---nvidia-docker run --rm nvidia/cuda nvidia-smi

---执行的时候可能会报错：

---Error: unsupported CUDA version: driver 8.0 < image 9.0.176

---所以需要指定一下版本：

---nvidia-docker run --rm nvidia/cuda:8.0-devel nvidia-smi

---可能报错二：

---docker: Error response from daemon: create nvidia_driver_367.48: create nvidia_driver_367.48: Error looking up volume plugin nvidia-docker: legacy plugin: plugin not found.
See 'docker run --help'.

---使用下面命令查看nvidia-docker 是否启动

---systemctl status nvidia-docker

---可能报错三：

---docker: Error response from daemon: create nvidia_driver_390.30: create nvidia_driver_390.30: Error looking up volume plugin nvidia-docker: legacy plugin: plugin not found.

---或者：

---docker: Error response from daemon: create nvidia_driver_390.30: found reference to volume 'nvidia_driver_390.30' in driver 'nvidia-docker', but got an error while checking the driver: error while checking if volume "nvidia_driver_390.30" exists in driver "nvidia-docker": Post http://%2Fvar%2Flib%2Fnvidia-docker%2Fnvidia-docker.sock/VolumeDriver.Get: dial unix /var/lib/nvidia-docker/nvidia-docker.sock: connect: connection refused: volume name must be unique.
See 'docker run --help'.

---需要重启nvidia-docker服务：

---systemctl start nvidia-docker 
systemctl status nvidia-docker

---网上说的：

---start nvidia-docker

---是错误的！

---.

---二、nvidia-smi的使用

---1、拉取镜像、开启容器

---当一台机器有很多个GPU可以通过NV_GPU来指定，

---通过-v将宿主机下的/data1/matt/docker文件夹与docker之中的/mnt

---共享文件

---。

---matt/docker是容器名称

---NV_GPU=1 nvidia-docker run -v /matt/docker:/mnt $container --rm -ti matt/docker bash

---tensorflow官网有个镜像拉取方式：

---sudo docker run -it --rm $DEVICES -v /usr/lib64/nvidia/:/usr/local/nvidia/lib64 tensorflow/tensorflow:latest-gpu bash

---有一个nvidia关于CUDA8的镜像：

---sudo nvidia-docker run --rm -ti nvidia/cuda:8.0 bash

---从docker hub上拉取镜像：

---docker pull mattzheng/docker_gpu

---最简单的启动：

---nvidia-docker run --rm -ti <镜像名字>
docker attach <容器名字>  \# 已打开的容器

---.

---2.上传容器与创建镜像

---创建镜像，容器名字叫

---device-query

---nvidia-docker build -t device-query

---上传容器到docker hub之上，

---（1）先得在网站注册：

---[https://hub.docker.com/](https://hub.docker.com/)

---（2）在terminal，中键入

---docker login

---输入账号与密码，就与Hub联通了

---sudo docker push mattzheng/device-query

---.

---3.容器与镜像删减

---镜像操作

---docker images \#查看有哪些镜像
docker rmi image_id \#删除镜像ID或者名称都可以

---容器操作

---docker ps -a  \# 容器参考
docker container ls \# 参考容器
docker rm container_id \#删除容器

---容器停止：

---docker ps -a | grep <container-id>
docker stop <container-id>

---有时候很难删除镜像：

---\# 第一种
Error response from daemon: conflict: unable to delete e4b9e4f71238 (must be forced) - image is being used by stopped container 1e359ad4363d
\# 第二种
Error response from daemon: conflict: unable to delete 1dc4f730b414 (cannot be forced) - image has dependent child images

---第一种代表有依赖的容器，需要根据容器名称，

---docker rm

---进行删除。

---第二种，因为有child，需要删除依赖的image，可以根据tag来进行删除，

---docker rm REPOSITORY:TAG    \# 根据TAG删除容器

---.

---4.容器改名

---sudo nvidia-docker tag tensorflow/tensorflow:latest-gpu matt/nvidia-docker

---把这个容器

---tensorflow/tensorflow:latest-gpu

---，改成名字

---matt/nvidia-docker

---.

---5.容器的保存

---如何对容器进行保存防止退出后全部修改消失

---（1）预先获取docker ps -l的容器ID

---（2）然后从新复制一个新的容器：

---docker commit 1610c46c28bd matt/test

---docker commit -m="has update" -a="matt" fc4bd61a4af2 matt/docker:5.2

---冒号之后是tag名称，可以继续更新，需要注意，关闭之前，需要apt-get update

---.

---6.在tensorflow容器中打开Jupyter notebook

---官方：

---[https://hub.docker.com/r/tensorflow/tensorflow/](https://hub.docker.com/r/tensorflow/tensorflow/)

---nvidia-docker run -it -p 8888:8888 tensorflow/tensorflow:latest-gpu

---然后就可以用http://localhost:8888/ 打开该jupyter。

---但是如果是，宿主机是远程Linux服务器，你想在远程调用服务器中docker的Jupyter notebook。因为会跟宿主机的IP起冲突，所以需要指定以下IP，在重启的时候：

---sudo nvidia-docker run -it -p 7777:8888 mattzheng/docker_gpu_1

---也就是把docker中的8888端口，赋值到宿主机的7777端口，这样不会与其他冲突。

---其中，第一次打开输入的密码，是toke之后的内容，本图中的为：

---0375ddd82c0417e55dddf4d3bf7f9dcba9530e89391a6163

![这里写图片描述](https://img-blog.csdn.net/20171206190445671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---打开容器之后就可以启动了：

---jupyter notebook --allow-root

---或者要在Jupyter中同时使用Py2 py3的话，可以

---[参考](http://lee-w-blog.logdown.com/posts/306983-used-in-the-jupyter-python2-python3)

---：

---pip2 install ipython notebook
pip3 install ipython notebook
ipython2 kernelspec install-self
ipython3 kernelspec install-self

---就可以使用了，重启后，会出现pip3/pip2

---主题换色（参考：

---[https://github.com/dunovank/jupyter-themes）：](https://github.com/dunovank/jupyter-themes%EF%BC%89%EF%BC%9A)

---安装主题包：

---!pip install --upgrade jupyterthemes

---\# 用 solarized-light 主题，代码字体是 inputmono，字号 12 点，界面字体 sourcesans，输出字体 sourcesans，开启工具栏，开启标题栏
!jt -t solarized-light -f inputmono -fs 12 -nf sourcesans -tf sourcesans -T -N

---执行后重新打开即可。参考模板型号：

---Available Themes: 
   chesterish
   grade3
   gruvboxd
   gruvboxl
   monokai
   oceans16
   onedork
   solarizedd
   solarizedl

---延伸一：如果修改Jupyter notebook密码

---可以参考 ：Running a notebook server在服务器上执行：jupyter notebook --generate-config

---记录下生成的配置文件位置，例如：/home/.jupyter/jupyter_notebook_config.py

---打开jupyter，新建一个notebook，生成密码的sha1秘钥，代码如下：

---from notebook.auth import passwd
passwd()

---在文本框中输入并确认一次密码后记录sha1秘钥值，如

---'sha1:XXXXXXX'

---将这段值按如下格式粘贴到配置文件jupyter_notebook_config.py末尾

---c.NotebookApp.password = u'sha1:XXXXXXX'

---重启jupyter，重新打开网页即可

---延伸二：报错：OSError: [Errno 99] Cannot assign requested address

---启动的时候需要：

---jupyter notebook --ip=0.0.0.0 --no-browser --allow-root

---.

---7.在容器中打开nvidia/digits

---nvidia-docker run --name digits -p 5000:5000 nvidia/digits

---.

---三、tensorflow安装

---在

---tensorflow/tensorflow:latest-gpu

---版本中，没有pip3

---需要安装一下：

---apt-get update
apt-get install python3-pip

---然后利用pip3安装tensorflow以及keras（

---[官网链接](https://www.tensorflow.org/install/install_linux)

---）：

---apt-get install libcupti-dev
apt-get install python3-pip python3-dev python-virtualenv
virtualenv --system-site-packages -p python3 targetDirectory
pip3 install tensorflow-gpu
pip3 install keras

---之后想安装opencv，但是有一些依赖很容易导致报错：

---apt-get install libsm6 libxrender1 libfontconfig1
apt-get install -y python-qt4
pip3 install opencv-python

---不然有可能会报错：

---ImportError: libSM.so.6: cannot open shared object file: No such file or directory
ImportError: libXext.so.6: cannot open shared object file: No such file or directory

---如果也同时需要安装Keras,除了

---pip install keras

---，不然会报错：

---python ImportError:

---load_weights

---requires h5py.

---还需要加载：

---pip3 install h5py

---.

---延伸：

---最长的开docker命令：

---NV_GPU=1 nvidia-docker run -it -p 7777:8888 -v /data/matt/docker:/mnt $container --rm -ti matt/docker bash

---延伸二：

---出现

---Unable to locate package

---问题，则需要：

---apt-get update

---在使用会出现的

---UnicodeEncodeError: 'ascii' codec can't encode character '\uff08' in position 0: ordinal not in range(128)

---错误：

---系统出现不能识别中文，需要加载中文字体：zh_CN.UTF-8（参考博客：

---[docker 学习笔记——解决Ubuntu中文乱码问题](http://www.nikochan.cc/2017/10/30/DockerSolution/)

---）

---延伸三：上传至阿里云

---参考：

---[https://blog.csdn.net/qq_16605855/article/details/79961933](https://blog.csdn.net/qq_16605855/article/details/79961933)

---4 管理Docker Hub镜像站点：配置Docker加速器

---链接：

---[https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4\#/accelerator](https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4#/accelerator)

---5 创建镜像仓库的命名空间

---例如：msj

---链接：

---[https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4\#/namespace/index](https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4#/namespace/index)

---6 创建镜像仓库

---例如：image-test

---链接：

---[https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4\#/imageList](https://cr.console.aliyun.com/?spm=5176.1971733.0.2.duOGn4#/imageList)

---然后通过这段code就可以上传自己的docker到自己的阿里云:

---$ sudo docker login --username= registry.cn-hangzhou.aliyuncs.com
  $ sudo docker tag [ImageId] registry.cn-hangzhou.aliyuncs.com/matt_docker/nvidia-docker:[镜像版本号]
  $ sudo docker push registry.cn-hangzhou.aliyuncs.com/matt_docker/nvidia-docker:[镜像版本号]

---延伸三：docker 之中screen 与主机的环境不一样，py版本不一样

---这样的情况需要在screen之中设置环境变量：

---export CONDA_DEFAULT_ENV=py36
    export CONDA_PREFIX=/miniconda/envs/$CONDA_DEFAULT_ENV
    export PATH=$CONDA_PREFIX/bin:$PATH
    export CONDA_AUTO_UPDATE_CONDA=false

---这个是在某个conda环境下，进行的设置范例。

---**公众号“素质云笔记”定期更新博客内容：**

---![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

---参考：

---[CentOS Linux 安裝與使用 NVIDIA Docker GPU 計算環境教學](https://blog.gtwang.org/virtualization/centos-linux-install-nvidia-docker-gpu-container-tutorial/)

---[Docker 中玩转 GPU](http://blog.opskumu.com/docker-gpu.html)

---[Using TensorFlow via Docker](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/tools/docker/README.md)

---[Docker Compose + GPU + TensorFlow = ❤️](https://hackernoon.com/docker-compose-gpu-tensorflow-%EF%B8%8F-a0e2011d36)

---[Docker基礎教程](http://tw.gitbook.net/docker/docer-save-changes.html)

---我的博客即将同步至腾讯云+社区，邀请大家一同入驻。


