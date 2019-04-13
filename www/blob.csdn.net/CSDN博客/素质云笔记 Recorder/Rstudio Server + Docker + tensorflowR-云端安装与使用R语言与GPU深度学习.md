
# Rstudio Server + Docker + tensorflowR - 云端安装与使用R语言与GPU深度学习 - 素质云笔记-Recorder... - CSDN博客

2018年09月28日 15:49:36[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：541


笔者本来想在阿里云上部署nvidia - docker + Rstudio Server，然后使用R语言来进行深度学习。本篇是在该过程中产生的经验之谈。

---文章目录

---[1 准备阶段](#1__11)

---[2 Rstudio Server nvidia-docker 环境](#2_Rstudio_Server_nvidiadocker__33)

---[2.1 docker初始化的问题](#21_docker_36)

---[2.2 docker中文环境布置问题](#22_docker_77)

---[3 Rstudio Server的部署](#3_Rstudio_Server_109)

---[3.1 启动](#31__110)

---[3.2 添加用户组](#32__126)

---[4 R语言与tensorflow + keras](#4_Rtensorflow__keras_157)

---[4.1 安装](#41__158)

---[4.2 占用指定显存容量](#42__212)

---[4.3 模型](#43__222)


---1 准备阶段

---来看看要准备一些什么：

---cuda/cudnn不提了…

---docker/nvidia-docker安装如下，一些问题可参考：

---[docker︱在nvidia-docker中使用tensorflow-gpu/jupyter](https://blog.csdn.net/sinat_26917383/article/details/78728215)

---，大致如下：

---安装docker

---curl -sSL https://get.docker.com/ | sh1

---安装Nvidia Docker

---wget -P /tmp https://github.com/NVIDIA/nvidia-docker/releases/download/v1.0.1/nvidia-docker_1.0.1-1_amd64.deb
sudo dpkg -i /tmp/nvidia-docker*.deb12

---通过以下命令来检验是否成功：

---nvidia-docker run --rm nvidia/cuda nvidia-smi

---2 Rstudio Server nvidia-docker 环境

---在项目

---[pbhogale/gpu-keras-rstudio](https://github.com/pbhogale/gpu-keras-rstudio)

---中有非常棒的环境已经写好，大致的环境为：

---nvidia/cuda:9.0-cudnn7-runtime

---2.1 docker初始化的问题

---但是由于在阿里云上对接github上很多资源下载会出现time out；

---也会出现因为版本问题而无法下载的，在笔者调试了很多次下该

---Dockerfile

---是可以把环境部署好的（

---[下载链接](https://download.csdn.net/download/sinat_26917383/10693102)

---），但是需要自己在docker里面自己装keras:tensorflow。

---把笔者提供的

---Dockerfile

---替换一下

---gpu-keras-tidyverse

---文件夹下的文件。

---其中需要注意的有，

---注意点一：

---RUN set -e \
    && useradd -m -d /home/rstudio rstudio \
    && echo rstudio:rstudioTheLegendOfZelda \
    | chpasswd \
    && apt-get -y autoremove \
    && apt-get clean

---这个代表了设置rstudio的初始密码，好吧… 这个docker默认设置有点调皮，密码好长…

---账号为：rstudio

---密码为：rstudioTheLegendOfZelda

---注意点二：

---RUN set -e \
    && grep '^DISTRIB_CODENAME' /etc/lsb-release \
    | cut -d = -f 2 \
    | xargs -I {} echo "deb \${CRAN_URL}bin/linux/ubuntu {}/" \
    | tee -a /etc/apt/sources.list \
    && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E084DAB9 \
	&& apt-get update \
    && apt-get upgrade -y -q \
    && pip install --upgrade pip==9.0.3 \
    && pip install virtualenv \
    && pip install pelican \
    && rm -rf .cache \
    && echo 'options(repos = c(CRAN = "https://mirrors.tuna.tsinghua.edu.cn/CRAN/"))' >> /etc/R/Rprofile.site \
    && /usr/lib/R/site-library/littler/examples/install.r tensorflow keras \
	&& r -e "install.packages(c('devtools', 'xgboost', 'tidyverse', 'rmarkdown', 'greta', 'usethis', 'docopt'))" \
    && apt-get -y autoremove \
    && apt-get clean

---options(repos = c(CRAN = "https://mirrors.tuna.tsinghua.edu.cn/CRAN/"))

---这个如果是阿里云需要改成清华的镜像，不然基本下载不了。

---2.2 docker中文环境布置问题

---启动docker + r server会发现中文读入不了，笔者加载了

---Sys.setlocale("LC_ALL", 'en_US.UTF-8')

---或

---Sys.setlocale()

---有以下几个问题：原因是系统里面没有utf-8的locale。

---Warning message:
In Sys.setlocale("LC_ALL", "Chinese") :
  OS reports request to set locale to "Chinese" cannot be honored

---以及

---Warning message:
Character set is not UTF-8; please change your locale

---需要在docker环境下安装locale，可参考：

---locale-gen en_US.UTF-8
apt-get install language-pack-zh-hant-base language-pack-zh-hans-base

---参考：

---[Ubuntu 安装和启用中文](https://www.edlad.com/2016/09/12/ubuntu-chinese/)

---[修改locale](http://wiki.ubuntu.org.cn/%E4%BF%AE%E6%94%B9locale)

---比较来说，在

---/etc/default/locale

---文件最好使用以下：

---LANG="en_US.UTF-8"
LANGUAGE="en_US:en"

---3 Rstudio Server的部署

---3.1 启动

---\#查看是否安装正确 
    sudo rstudio-server verify-installation 
    \#\# 启动 
    sudo rstudio-server start 
    \#\# 查看状态 
    sudo rstudio-server status 
    \#\# 停止 
    sudo rstudio-server stop 
    \#\# 重启
    sudo rstudio-server restart
    \#\# 查看服务端ip地址 
    ifconfig

---3.2 添加用户组

---\#\#添加用户组
    sudo groupadd rstudio-group
    \#\#添加用户
    sudo useradd anlan -g rstudio-group
    \#\#设置用户密码
    sudo passwd xxxxxx
    \#\#创建用户目录
    sudo mkdir /home/anlan
    \#\#赋予目录权限
    sudo chown -R anlan:rstudio-group /home/anlan

---但是如果是在docker之中使用，笔者一直发现一个问题，就是添加不了用户，一直报错：

---RSTUDIO INITIALIZATION ERROR: UNABLE TO CONNECT TO SERVICE

---解决办法是：

---参考：

---[解決登入 Docker 的 RStudio 遇到錯誤 RStudio Initialization Error: Unable to connect to service](https://errerrors.blogspot.com/2018/05/resolve-docker-rstudio-initialization-error.html)

---只登陆一个账号，一般为

---rstudio:rstudio

---，此处如果是默认的docker的话，即为：

---rstudio:rstudioTheLegendOfZelda

---第二种是在docker启动的时候，就为设置账号和密码（笔者实验好像无效）：

---docker run -d -p 8787:8787 -e USER=<username> -e PASSWORD=<password> rocker/rstudio

---4 R语言与tensorflow + keras

---4.1 安装

---有了以上的环境，那么来看一下R之中使用keras/tensorflow,如果要使用简单的keras，需要先加载Keras，然后使用keras下载tensorflow。

---install.packages("devtools")
devtools::install_github("rstudio/keras")
library(keras)
install_tensorflow(gpu=TRUE)

---其中

---[install_tensorflow()](https://github.com/rstudio/tensorflow/blob/10cefc322786afd562675184517da2bb73d5deeb/R/install.R)

---与

---[install_keras()](https://github.com/rstudio/keras/blob/61d3fd349eaae112e6b24a20ab51571da3589d53/R/install.R)

---有几个参数：

---\# 函数一：install_keras
install_keras <- function(method = c("auto", "virtualenv", "conda"), 
                          conda = "auto",
                          tensorflow = "default",
extra_packages = c("tensorflow-hub")) 
\# 函数二：install_tensorflow 
install_tensorflow <- function(method = c("auto", "virtualenv", "conda", "system"),
                               conda = "auto",
                               version = "default",
                               envname = "r-tensorflow",
                               extra_packages = c("keras", "tensorflow-hub"),
restart_session = TRUE)

---Method

---Description
|
|---|---|
---auto

---Automatically choose an appropriate default for the current platform.
|
---virtualenv

---Install into a Python virtual environment at ~/.virtualenvs/r-tensorflow
|
---conda

---Install into an Anaconda Python environment named r-tensorflow
|
---system

---Install into the system Python environment
|
---一些控制版本：

---\# tensorflow
install_tensorflow(version = "1.0.0")
install_tensorflow(method = "conda")
install_tensorflow(version = "nightly-gpu")  \# gpu version
install_tensorflow(version = "gpu")  \# gpu version
\# keras
library(keras)
install_keras(tensorflow = "1.2.1")
install_keras(tensorflow = "1.2.1-gpu")
install_keras(method = "conda")

---那么大体来说，py的版本控制，好像没有特别的参数与控制，一般默认是py2的tensorflow，其中在

---[install_tensorflow()](https://github.com/rstudio/tensorflow/blob/10cefc322786afd562675184517da2bb73d5deeb/R/install.R)

---：

---python_versions <- py_versions_windows()

---4.2 占用指定显存容量

---这边如果在使用tf的时候，会出现一下子把显存全部占满的情况，则需要控制一下，笔者只是照搬了py的稍微改一下，没想到能用，分享给大家。

---library(tensorflow)
config = tf\$ConfigProto()
config\$gpu_options\$per_process_gpu_memory_fraction = 0.3
tf\$Session(config=config)

---4.3 模型

---这边就不举例了，在很多地方都有：

---Keras for R：

---[https://blog.rstudio.com/2017/09/05/keras-for-r/](https://blog.rstudio.com/2017/09/05/keras-for-r/)

---可参考：

---[Installing TensorFlow](https://tensorflow.rstudio.com/tensorflow/articles/installation.html)

---[https://github.com/rstudio/keras](https://github.com/rstudio/keras)

---[https://github.com/rstudio/tensorflow](https://github.com/rstudio/tensorflow)


