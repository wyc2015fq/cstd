# Win10-64位系统安装xgboost（使用MinGW编译） - 浅梦的博客 - CSDN博客





2017年01月01日 17:46:27[浅梦s](https://me.csdn.net/u012151283)阅读数：765








# 准备工作：
- Pyton科学计算环境，推荐直接安装[Anaconda](https://www.continuum.io/downloads)选择Py2或者Py3版本都可以（注意选对版本，不要安装成32位的否则会出现“**WindowsError:[Error 193] %1 不是有效的 Win32**”错误） 

如果之前已经安装过了，请确定下你的版本：打开cmd,输入`python`，确定是64位的 
![验证版本](https://img-blog.csdn.net/20170103180538833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- [MinGW](https://sourceforge.net/projects/mingw-w64/files/latest/download)
![MinGW安装选项](https://img-blog.csdn.net/20170101171130945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

之后选择安装路径即可进行安装。- [Git Bash](https://git-for-windows.github.io/)

关于Git的安装基本选择默认选项即可

# 环境配置
- Python的相关环境变量Anaconda已经帮助我配置好
- 配置MinGW相关的环境变量： 

将`C:\Program Files\mingw-w64\x86_64-6.2.0-win32-seh-rt_v5-rev1-seh-rt_v5-rev1\mingw64\bin`和`C:\Program Files\mingw-w64\x86_64-6.2.0-win32-seh-rt_v5-rev1-seh-rt_v5-rev1\mingw64\x86_64-w64-mingw32\include`添加到系统变量Path中。添加完毕后打开`cmd`,输入`g++`验证。下面的图片说明正确安装并配置。![g++测试](https://img-blog.csdn.net/20170101172006850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 下载xgboost

在所要安装的目录下打开Git Bash，依次输入如下命令

```php
git clone --recursive https://github.com/dmlc/xgboost
cd xgboost
git submodule init
git submodule update
```

# 编译xgboost

## Windows

在xgboost目录内打开Git Bash窗口总依次输入：

```
cp make/mingw64.mk config.mk;
mingw32-make -j4；
```

![编译成功](https://img-blog.csdn.net/20170103011249108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后运行结果如上，说明编译成功。  
## Ubuntu/Debian

```bash
git clone --recursive https://github.com/dmlc/xgboost
cd xgboost; make -j4
```

## Linux GPU支持

```bash
$ mkdir build
$ cd build
$ cmake .. -DUSE_CUDA=ON
$ make -j
```

# 安装Python包

进入`xgboost\python-package`目录下，打开`cmd`运行 `python setup.py install`
![Python版本安装](https://img-blog.csdn.net/20170103011316374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果在Python3版本下安装，可能会遇见如下错误 
![这里写图片描述](https://img-blog.csdn.net/20170318172543835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决方案是将`setup.py`文件中的`setup`函数中大概在38行左右的`include_package_data=False,`改成`True`，然后重新执行安装命令。 

安装好的提示 
![安装好的提示](https://img-blog.csdn.net/20170103011412828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 验证安装成功

打开python终端：输入 `import xgboost as xgb` 成功导入说明安装成功 
![验证安装](https://img-blog.csdn.net/20170103180607821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








