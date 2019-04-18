# Anaconda环境搭建 - wsp_1138886114的博客 - CSDN博客





2018年05月24日 21:29:23[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：406








# Anaconda环境搭建

清华镜像站：[https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/](https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/)

清华镜像站–Anaconda各版本下载：[https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)

清华镜像站–Pytorch镜像下载：[https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/win-64/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/win-64/)
### 一、创建环境

> 
##### 创建命令：`conda create -n env_name python=3.x`

`env_name ： 环境名`
`python=2.x/3.x ：指定python版本`
**示例：**
`conda create -n py36 python=3.6`
`创建名py36的环境，python版本为3.6`
##### 列出base环境所有的包：`conda info -e`

### 二、管理环境

> 
##### 进入环境：

`source activate my_env (OSX/LINUX系统)`
`activate my_env (Windows系统)	#示例：activate py36`

##### 环境中添加所需的包：

`conda install package_name	#示例：conda install numpy/scipy`

##### 环境中删除所需的包：

`conda remove package_name`

##### 删除环境：

`conda remove -n my_env --all`


### 三 Anaconda瘦身

> 
##### 方法1
- Anaconda就像一个相对独立的生态，默认情况下，conda 创建的新环境 以及过往安装的模块缓存都存储在用户目录下，这一点不会在 conda （user-specific）配置文件 $HOME/.condarc 中体现出来，除非用户修改了默认设置。我们可以使用`conda info`查看

```python
C:\Users\XXXX>conda info

    active environment : None
           shell level : 0
      user config file : C:\Users\XXXX\.condarc
populated config files :
         conda version : 4.5.11
   conda-build version : 3.10.5
        python version : 3.6.5.final.0
      base environment : C:\Anaconda3  (writable)
          channel URLs : https://repo.anaconda.com/pkgs/main/win-64
                         https://repo.anaconda.com/pkgs/main/noarch
                         https://repo.anaconda.com/pkgs/free/win-64
                         https://repo.anaconda.com/pkgs/free/noarch
                         https://repo.anaconda.com/pkgs/r/win-64
                         https://repo.anaconda.com/pkgs/r/noarch
                         https://repo.anaconda.com/pkgs/pro/win-64
                         https://repo.anaconda.com/pkgs/pro/noarch
                         https://repo.anaconda.com/pkgs/msys2/win-64
                         https://repo.anaconda.com/pkgs/msys2/noarch
         package cache : C:\Anaconda3\pkgs
                         C:\Users\XXXX\AppData\Local\conda\conda\pkgs
      envs directories : C:\Anaconda3\envs
                         C:\Users\XXXX\AppData\Local\conda\conda\envs
                         C:\Users\XXXX\.conda\envs
              platform : win-64
            user-agent : conda/4.5.11 requests/2.18.4 CPython/3.6.5 Windows/10 Windows/10.0.17134
         administrator : False
            netrc file : None
          offline mode : False
```
- 修改conda环境和缓存的默认路径（envs directories 和 package cache）。

路径的设置可以通过编辑 $HOME/.condarc 来实现。添加或修改  $HOME/.condarc 中的 env_dirs 和 pkgs_dirs 配置项分别设置环境和缓存路径，按顺序第一个路径作为默认存储路径，搜索环境和缓存时按先后顺序在各目录中查找。

```python
channels:
	  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/
	  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
	  - defaults
	show_channel_urls: true
	envs_dirs:
	  - D:\data\xxxx\Miniconda\envs    # 此处为修改后的目录
	  - C:\Users\xxxx\AppData\Local\conda\conda\envs
	  - C:\Users\xxxx\.conda\envs
	  - C:\ProgramData\Anaconda\Miniconda3\envs                          
	pkgs_dirs:
	  - D:\data\xxxx\Miniconda\pkgs   # 此处为修改后的目录
	  - C:\Users\xxxx\AppData\Local\conda\conda\pkgs
	  - C:\Users\xxx\.conda
	  - C:\ProgramData\Anaconda\Miniconda3\pkgs
```

##### 方法2

使用`conda clean`命令

第一步：通过`conda clean -p`来删除一些没用的包，这个命令会检查哪些包没有在包缓存中被应用依赖到其他地方，并删除它们。

第二步：通过`conda clean -t`删除conda保存下来的tar包（删除后，以后安装要重新下载的）。

或者直接只用`conda clean -y -all` 删除所有的安装包及cache
### 四、更换国内源

> 
##### Anaconda修改为清华镜像源

###### 1. 使用win+r 打开cmd

**执行命令**：`conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/`

###### 2. 在cmd下再次执行

**执行命令**：`conda config --set show_channel_urls yes`

> 
2.1 目录 C:\Users<用户名> 下生成配置文件.condarc，文件内容如下(notepad++打开)：
![这里写图片描述](https://img-blog.csdn.net/20180524210500536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.2 删除上述配置文件 .condarc 中的第三行，然后保存，效果如下：
![这里写图片描述](https://img-blog.csdn.net/20180524211244912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
###### 3. 在Anaconda Prompt下再次执行

执行命令：`conda info`

查看当前配置信息，内容如下，即修改成功，关注 channel URLs 字段内容
![这里写图片描述](https://img-blog.csdn.net/20180524211837930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
###### 附加：`jupyter notebook`[使用方法](https://blog.csdn.net/wsp_1138886114/article/details/80444621)












