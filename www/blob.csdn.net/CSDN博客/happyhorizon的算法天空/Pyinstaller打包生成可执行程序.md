# Pyinstaller打包生成可执行程序 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:30[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：832

## 安装:
`sudo pip install pyinstaller`
也可以下载安装:
[https://sourceforge.net/projects/pyinstaller/](https://sourceforge.net/projects/pyinstaller/)
Pyinstaller命令的基本形式为:
```
pyinstaller [options] script [script ...] | specfile
```
## 使用
`pyinstaller myscript.py`
Pyinstaller将分析myscript.py文件.然后将myscript.spec文件写入与py文件同名的文件夹(myscript文件夹.)在该文件夹中创建build文件夹.在build文件夹中写入log文件和工作文件.之后在myscript中创建dist文件夹,将myscript可执行文件写入到dist文件夹.
一般情况下pyinstaller后面只指定一个py文件.如果指定了多个py文件,所有的文件都会被分析,并生成输出文件.但是所有的输出文件都会保存到第一个py文件名所命名的输出文件夹中.
`Pyinstaller myscript.py myscript1.py myscript2.py`
运行后,只生成一个myscript的文件夹中,里面包含了对这三个文件分析后生成的输出文件.
此外,还可以编辑myscript.spec文件,编辑后就可以直接编译spec文件(而不是py文件):
`pyinstaller myscript.spec`
如果py文件不在当前路径下,可以用下面命令编译:
`pyinstaller options.. ~/path/to/script/myscript.py
**Pyinstaller命令运行参数包括：**
-F, --onefile Py代码只有一个文件
-D, --onedir Py代码放在一个目录中（默认是这个）
-K, --tk 包含TCL/TK
-d, --debug 生成debug模式的exe文件
-w, --windowed, --noconsole 窗体exe文件(WindowsOnly)
-c, --nowindowed, --console 控制台exe文件(WindowsOnly)
-X, --upx 使用upx压缩exe文件
-o DIR, --out=DIR 设置spec文件输出的目录，默认在PyInstaller同目录
-v FILE, --version=FILE 加入版本信息文件
特别要注意的是,当前的pyinstaller还不支持python3.6版本.虽然支持3.6的还没有发布,但是可以手动安装[https://github.com/pyinstalle](https://github.com/pyinstalle)... 下载zip后把PyInstaller提出来。删掉python/Lib/site-packages/PyInstaller 文件夹，把解压得到的PyInstaller放进去应该就OK了。#_#
(值得试一试!)
拿matplotlib官网的例子animate_decay成功生成了linux下的可执行文件.
Pyinstaller -F animate_decay.py
在dist文件下,可以找到animate_decay可执行文件.运行:
./animate_decay
![image.png](https://upload-images.jianshu.io/upload_images/4685306-b7e070caf5fb72e1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
参考:
[http://www.jianshu.com/p/3b10c6664706](http://www.jianshu.com/p/3b10c6664706)
[http://kevinhao.blog.51cto.com/5204735/1704411](http://kevinhao.blog.51cto.com/5204735/1704411)
[https://segmentfault.com/q/1010000009242914](https://segmentfault.com/q/1010000009242914)
