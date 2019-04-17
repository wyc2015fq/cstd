# win7 /10 下 python anaconda 安装 kivy,pygame, dlib, pytorch - 墨流觞的博客 - CSDN博客





2018年10月13日 16:26:16[墨氲](https://me.csdn.net/dss_dssssd)阅读数：298








windows环境下各种库的网站地址:[https://www.lfd.uci.edu/~gohlke/pythonlibs/](https://www.lfd.uci.edu/~gohlke/pythonlibs/)
- anaconda

在[https://www.anaconda.com/download/](https://www.anaconda.com/download/) ,这个网站是[https://repo.anaconda.com/archive/](https://repo.anaconda.com/archive/)之前的版本，下载后安装很简单，**建议在安装过程中选择add to path …**

##### .whl文件的安装：

`python -m pip install [whl文件所在的绝对路径]`

##### 删除pip缓存

[https://stackoverflow.com/questions/9510474/removing-pips-cache](https://stackoverflow.com/questions/9510474/removing-pips-cache)

##### 1.   cv2

[https://www.lfd.uci.edu/~gohlke/pythonlibs/#opencv](https://www.lfd.uci.edu/~gohlke/pythonlibs/#opencv)

不幸的是这个地方安装出错了。`DLL not found`.

* python3.dll 有吗？

* Visual C++ Redistributable 装了吗？

检查后我两个库都有

直接用conda安装：

1. 删除已安装的版本：
`python -m pip uninstall opencv-python`

2. conda命令安装
`conda install -c menpo opencv`

最后发现安装正确，
###### 2.  kivy

[https://www.lfd.uci.edu/~gohlke/pythonlibs/#kivy](https://www.lfd.uci.edu/~gohlke/pythonlibs/#kivy) .

##### 3. pygame  还需要安装依赖库 `videocapture`

[https://www.lfd.uci.edu/~gohlke/pythonlibs/#pygame](https://www.lfd.uci.edu/~gohlke/pythonlibs/#pygame)
[https://www.lfd.uci.edu/~gohlke/pythonlibs/#videocapture](https://www.lfd.uci.edu/~gohlke/pythonlibs/#videocapture)

###### 4.  pytorch

[https://pytorch.org/](https://pytorch.org/)

使用conda安装：

```
conda install pytorch -c pytorch
pip install torchvision
```

##### 5. dlib  使用conda安装

直接`pip install dlib`

注意：
- 建议安装visual studio 2017（里边有cmake tools和boost tools），2015应该也可以
- 安装cmake，注册environment path。

我安装以上两项才直接用`pip install dlib`安装成功的

##### 6. facerecognition

先参考5安装dlib

之后使用：`python -m pip install face_recognition`直接安装即可




