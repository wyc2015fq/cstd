# windows下安装Python-Whl文件 - wishchinYang的专栏 - CSDN博客
2015年04月13日 08:48:05[wishchin](https://me.csdn.net/wishchin)阅读数：8619
原文链接：[http://blog.sciencenet.cn/home.php?mod=space&uid=1181151&do=blog&id=865515](http://blog.sciencenet.cn/home.php?mod=space&uid=1181151&do=blog&id=865515)
       Note1. 整个安装过程主要参考下述网页：[http://www.pymolwiki.org/index.php/Windows_Install](http://www.pymolwiki.org/index.php/Windows_Install)
Note2. 需要首先安装Python2.7和vcpython2.7(即Microsoft VC++ Compiler for Python, 浏览器中打开http://aka.ms/vcpython27下载安装), 然后在Python2.7的环境下安装Pip, numpy, setuptools等模块，最后从[http://www.lfd.uci.edu/~gohlke/pythonlibs/#pymol](http://www.lfd.uci.edu/~gohlke/pythonlibs/#pymol)页面下载PyMOL的安装文件whl，利用pip进行安装。
详细步骤如下（Suppose vcpython27已安装）：
1. 安装Python2.7 官方下载地址： [https://www.python.org/download/releases/2.7.5/](https://www.python.org/download/releases/2.7.5/) 我安装在C盘根目录下，生成安装目录为C:\Python27
2. 从 [http://www.lfd.uci.edu/~gohlke/pythonlibs/#pip](http://www.lfd.uci.edu/~gohlke/pythonlibs/#pip) 下载pip安装文件pip‑6.0.7‑py2.py3‑none‑any.whl，放在Python根目录中。打开cmd窗口，执行
```python
python.exe pip-6.0.7-py2.py3-none‑any.whl/pip install pip-6.0.7-py2.py3-none‑any.whl
```
安装pip到C:\Python27\Scripts
3. 命令行一次执行
```python
#安装SetupTools
pip search setuptools
pip install setuptools
#安装Matplotlib
pip search matplotlib
pip install matplotlib
```
       或者在下载的文件夹里面：
      执行 pip install  matplotlib-1.4.3-cp27-none-win_amd64.whl
      安装预先下载的whl包
对于出现的 error: Microsoft Visual C++ 9.0 is required (Unable to find vcvarsall.bat). Get it from http://aka.ms/vcpython27
参考：windows下使用pip安装包的时候需要机器装有vs2008，VS2012还不行，如果不想装VS2008的话，可以安装一个[Micorsoft Visual C++ Compiler for Python 2.7](http://www.microsoft.com/en-us/download/details.aspx?id=44266)的包
        4.运行....................
3. 命令行进入C:\Python27\Scripts目录，依次执行
pip install pmw
pip install numpy
pip install setuptools
安装pmw，numpy和setuptools模块。
4. 从[http://www.lfd.uci.edu/~gohlke/pythonlibs/#pymol](http://www.lfd.uci.edu/~gohlke/pythonlibs/#pymol)下载相应的Pymol安装文件，放入C:\Python27\Scripts目录。因为是64位windows和python27，所以下载的是pymol-1.7.2.1-cp27-none-win_amd64.whl和pymol_launcher-1.0-cp27-none-win_amd64.whl。
5. cmd窗口执行
pip install pymol-1.7.2.1-cp27-none-win_amd64.whl
安装pymol，安装时间比较长，安装完之后在scripts文件夹下生成pymol.cmd文件，双击运行即可打开pymol运行窗口。
执行
pip install pymol_launcher-1.0-cp27-none-win_amd64.whl
安装pymol launcher，安装时间很短，安装完后在Python27目录下生成PyMOL.exe文件，双击打开PyMOL的运行主窗口。
6. 双击scripts文件夹下的pymol.cmd即可打开pymol运行窗口。
另外，锦上添花的步骤（在运行PyMOL时安装/加载plugins）：
7. 从[https://github.com/Pymol-Scripts/Pymol-script-repo/zipball/master](https://github.com/Pymol-Scripts/Pymol-script-repo/zipball/master)下载Pymol-script-repo（zip file）
8. 解压到:**C:\Python27\Lib\site-packages\pymol\pymol_path\Pymol-script-repo**确认文件名和路径完全正确。
9. 打开记事本程序把下面的代码粘贴进去：
# Add paths to sys.path so PyMOL can find modules and scripts
import sys, os
pymol_git = os.path.abspath(os.path.join(os.environ['PYMOL_PATH'], 'Pymol-script-repo'))
os.environ['PYMOL_GIT_MOD'] = os.path.join(pymol_git,'modules')
sys.path.append(pymol_git)
sys.path.append(os.environ['PYMOL_GIT_MOD'])
# Make setting changes to Plugin Manager
import pymol.plugins
pymol.plugins.preferences = {'instantsave': False, 'verbose': False}
pymol.plugins.autoload = {'apbs_tools': False}
pymol.plugins.set_startup_path([os.path.join(pymol_git, 'plugins'), os.path.join(sys.prefix, 'Lib', 'site-packages', 'pmg_tk', 'startup')])
pymol.plugins.preferences = {'instantsave': True, 'verbose': False}
然后将文件存为：**File->Save as->All files-> C:\Python27\Lib\site-packages\pymol\pymol_path\run_on_startup.py**
10. 再次打开PyMOL，从主菜单的Plugins选项就可以安装和运行plugins了。
DIY的哦，不收费～
