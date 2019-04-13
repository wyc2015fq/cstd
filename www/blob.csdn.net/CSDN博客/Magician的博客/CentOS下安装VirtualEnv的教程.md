
# CentOS下安装VirtualEnv的教程 - Magician的博客 - CSDN博客


2018年08月30日 10:43:43[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：338



### 文章目录
[前言：](#_2)
[1、下载安装virutalenv](#1virutalenv_8)
[2、安装新的Python版本（可以直接安装anaconda：安装过程可自行查资料）](#2Pythonanaconda_78)
[3、 VirtualEnv的设置](#3_VirtualEnv_97)
[4、Python虚拟环境的作用](#4Python_136)
[总结：](#_142)

## 前言：
在目前的Linux系统中，默认使用的都是Python 2.7.x的版本环境，如何想要在其上基于Python3的环境来进行开发，则需要基于VirutalEnv来构建独立的开发环境。
## 1、下载安装virutalenv
执行pip命令
`>>  pip install virtualenv``[root@xxxZ]# pip install virtualenv
Collecting virtualenv
  Downloading http://mirrors.aliyun.com/pypi/packages/6f/86/3dc328ee7b1a6419ebfac7896d882fba83c48e3561d22ddddf38294d3e83/virtualenv-15.1.0-py2.py3-none-any.whl (1.8MB)
    100% |████████████████████████████████| 1.8MB 34.9MB/s 
Installing collected packages: virtualenv
Successfully installed virtualenv-15.1.0`测试是否安装成功：
`>> virtualenv -h``[root@xxx]# virtualenv -h
Usage: virtualenv [OPTIONS] DEST_DIR
 
Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -v, --verbose         Increase verbosity.
  -q, --quiet           Decrease verbosity.
  -p PYTHON_EXE, --python=PYTHON_EXE
                        The Python interpreter to use, e.g.,
                        --python=python2.5 will use the python2.5 interpreter
                        to create the new environment.  The default is the
                        interpreter that virtualenv was installed with
                        (/usr/bin/python2)
  --clear               Clear out the non-root install and start from scratch.
  --no-site-packages    DEPRECATED. Retained only for backward compatibility.
                        Not having access to global site-packages is now the
                        default behavior.
  --system-site-packages
                        Give the virtual environment access to the global
                        site-packages.
  --always-copy         Always copy files rather than symlinking.
  --unzip-setuptools    Unzip Setuptools when installing it.
  --relocatable         Make an EXISTING virtualenv environment relocatable.
                        This fixes up scripts and makes all .pth files
                        relative.
  --no-setuptools       Do not install setuptools in the new virtualenv.
  --no-pip              Do not install pip in the new virtualenv.
  --no-wheel            Do not install wheel in the new virtualenv.
  --extra-search-dir=DIR
                        Directory to look for setuptools/pip distributions in.
                        This option can be used multiple times.
  --download            Download preinstalled packages from PyPI.
  --no-download, --never-download
                        Do not download preinstalled packages from PyPI.
  --prompt=PROMPT       Provides an alternative prompt prefix for this
                        environment.
  --setuptools          DEPRECATED. Retained only for backward compatibility.
                        This option has no effect.
  --distribute          DEPRECATED. Retained only for backward compatibility.
                        This option has no effect.`virutalenv安装成功了
## 2、安装新的Python版本（可以直接安装anaconda：安装过程可自行查资料）
可以从python的官方站点，下载源代码，编译安装，这里Python安装包是3.6.1的版本。具体的安装过程可以参考网路上的文档。
`*  download the source code package from python.org
    *  tar zxvf python.xx.tgz  ,   tar xvf Python.tar.xz
    *  cd source_code_folder
    * ./configure  --prefix=/opt/python-install-path
    * make & make install`默认会安装到/usr/local
## 3、 VirtualEnv的设置
**3.1、对于安装Python的情况：**
`>> virtualenv -p  python36-install-path/bin/python.exe python36env
    这里设置Python版本为3.6， 新的环境目录为python36env
  >> source python36env/bin/activate    # 激活虚拟环境
  >>  deactivate   # 退出当前的虚拟环境`**3.2、对于安装anaconda的情况：**
大部分情况下，我们希望直接用anaconda中的python环境以及anaconda自带的许多第三方库，这时又不想和别的python程序共用环境。这时需要自定义一个虚拟环境。过程如下：
1、为Anaconda创建一个python3.5（或者3.6都行，随便）的环境，环境名称为test ，输入下面命令：
`conda create -n test`**2、启动test环境**
`source activate test
#或者 conda activate test`关闭test环境，命令为：
`source deactivate test
#或者 conda deactivate test`**3、安装所需的各种第三方库**
`(tensorflow)$ pip install xxx`**4、测试安装的第三方库**
## 4、Python虚拟环境的作用
Python的虚拟环境可以使一个Python程序拥有独立的库library和解释器interpreter，而不用与其他Python程序共享统一个library和interpreter。虚拟环境的好处是避免了不同Python程序间的互相影响（共同使用global library 和 interpreter），例如程序A需要某个库的1.0版本，而程序B需要同样这个库的2.0版本，如果程序B执行则A就不能执行了。
## 总结：
这个隔离的环境非常方便，进行程序的开发和环境的切换。
参考：[https://blog.csdn.net/blueheart20/article/details/70598031](https://blog.csdn.net/blueheart20/article/details/70598031)
参考：[https://blog.csdn.net/u012052268/article/details/77487604](https://blog.csdn.net/u012052268/article/details/77487604)

