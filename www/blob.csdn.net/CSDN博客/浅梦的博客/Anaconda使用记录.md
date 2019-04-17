# Anaconda使用记录 - 浅梦的博客 - CSDN博客





2017年01月07日 00:16:08[浅梦s](https://me.csdn.net/u012151283)阅读数：5472








# Anaconda

[Anaconda](https://anaconda.org/) 是一个包含数据科学常用包的发行版本。它基于 conda ——一个包和环境管理器——衍生而来。  

Anaconda 实际上是一个软件发行版，它附带了 `conda`、Python 和 150 多个科学包及其依赖项。应用程序 `conda` 是包和环境管理器。Anaconda 的下载文件比较大（约 500 MB），因为它附带了 Python 中最常用的数据科学包。如果只需要某些包，或者需要节省带宽或存储空间，也可以使用 **Miniconda** 这个较小的发行版（仅包含 conda 和 Python）。  

## 安装Anaconda

Anaconda 可用于 Windows、Mac OS X 和 Linux。可以在 [https://www.continuum.io/downloads](https://www.continuum.io/downloads) 上找到安装程序和安装说明。  

如果计算机上已经安装了 Python，这不会有任何影响。实际上，脚本和程序使用的默认 Python 是 Anaconda 附带的 Python。 
**这里需要注意的是选对自己操作系统对应的版本。**

安装完后打开`cmd`,输入`conda list`可以查看当前 

安装的内容。 

输入`conda upgrade --all`，可以更新默认环境下的所有包。并在提示是否更新的时候输入y（Yes）以便让更新继续。初次安装下的软件包版本一般都比较老旧，因此提前更新可以避免未来不必要的问题。
# 管理包

`conda` 是一种只能通过命令行来使用的程序。类似于pip那样可以对Python库的包进行管理。 

安装命令：`conda install package_name`。如安装numpy,输入`conda install numpy`

同时安装多个包：`conda install numpy scipy pandas`

安装指定版本的包：`conda install numpy=1.11`
**conda会自动安装依赖库。**

卸载包：`conda remove package_name`

更新包：`conda update package_name`

更新环境中的所有包：`conda update --all`

列出已安装的包：`conda list`
**为conda添加清华镜像源**
```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```

如果不知道要找的包的确切名称，可以使用 `conda search search_term` 进行搜索。

# 管理环境

除了管理包之外，conda 还是虚拟环境管理器。它类似于另外两个很流行的环境管理器，即 [virtualenv](https://virtualenv.pypa.io/en/stable/) 和 [pyenv](https://github.com/yyuu/pyenv)。 

环境可以分隔不同项目的包。因为我们使用的时候会依赖于某个库的不同版本的代码或在py2和py3之间进行切换。  

也可以将环境中的包的列表导出为文件，然后将该文件与代码包括在一起。这能让其他人轻松加载代码的所有依赖项。pip 提供了类似的功能，即 `pip freeze > requirements.txt`

## 创建环境：

`conda create -n env_name list of packages`，`-n env_name` 设置环境的名称（`-n` 是指名称），而 `list of packages` 是要安装在环境中的包的列表。 

创建环境时，可以指定要安装在环境中的 Python 版本。 `conda create -n py3 python=3` 或 `conda create -n py2 python=2` 。要安装特定版本（例如 Python 3.3），请使用 `conda create -n py python=3.3`。

## 进入环境

创建了环境后，在 OSX/Linux 上使用 `source activate my_env` 进入环境。在 Windows 上，请使用 `activate my_env`。 

要离开环境，请键入 `source deactivate`（在 OSX/Linux 上）。在 Windows 上，请使用 `deactivate`。

## 保存和加载环境

共享环境这项功能确实很有用，它能让其他人安装你的代码中使用的所有包，并确保这些包的版本正确。可以使用 `conda env export > environment.yaml` 将包保存为 [YAML](http://www.yaml.org/)。第一部分 `conda env export` 写出环境中的所有包（包括 Python 版本）。 

第二部分 `> environment.yaml` 将导出的文本写入到 YAML 文件 `environment.yaml` 中。现在可以共享此文件，而且其他人能够创建和你用于项目相同的环境。 

要通过环境文件创建环境，请使用 `conda env create -f environment.yaml`。这会创建一个新环境，而且它具有在 `environment.yaml` 中列出的同一库。
## 列出环境

如果忘记了环境的名称，可以使用 `conda env list` 或`conda info -e`列出你创建的所有环境。你会看到环境的列表，而且你当前所在环境的旁边会有一个星号。默认的环境（即当你不在环境中时使用的环境）名为 `root`。

## 删除环境

不再使用某些环境，可以使用 `conda env remove -n env_name` 删除指定的环境（在这里名为 `env_name`）。

# 共享环境

共享环境 

在 GitHub 上共享代码时，最好同样创建环境文件并将其包括在代码库中。这能让其他人更轻松地安装你的代码的所有依赖项。对于不使用 conda 的人，我通常还会使用 `pip freeze`[(在此处了解详情)](https://pip.pypa.io/en/stable/reference/pip_freeze/)将一个 `pip requirements.txt` 文件包括在内。

```
> pip freeze > requirements.txt
> pip install -r requirements.txt
```






