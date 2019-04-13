
# windows下如何安装pip以及如何查看pip是否已经安装成功 - anlcy - 博客园






# [windows下如何安装pip以及如何查看pip是否已经安装成功](https://www.cnblogs.com/camilla/p/8023923.html)
最近刚学习python，发现很多关于安装以及查看pip是否安装成的例子都比较老，不太适合于现在（python 3.6 ）因此，下一个入门级别的教程。
0：首先如何安装python我就不做介绍了。
1：如果安装的是python3.4版本以上的，就不需要重新安装。此处给出官方链接[https://pip.pypa.io/en/stable/installing/](https://pip.pypa.io/en/stable/installing/)
然后你只需要在你安装的python的python\scripts文件中查看是否有以下文件 比如pip.exe   pip3.6.exe  easy_installer.exe  等。
![](https://images2015.cnblogs.com/blog/943825/201706/943825-20170628192450743-1131481285.png)
如果有，则表明pip已经在安装python的时候已经自带。
同时如果需要直接通过DOC打开pip，则需要将环境变量添加进去。
2：具体添加环境变量   系统属性——环境变量——Path——scripts路径添加进去。
![](https://images2015.cnblogs.com/blog/943825/201706/943825-20170628192718649-2106599980.png)
![](https://images2015.cnblogs.com/blog/943825/201706/943825-20170628192808805-805040520.png)
![](https://images2015.cnblogs.com/blog/943825/201706/943825-20170628192849727-851362981.png)
现在就可以通过Pip list 命令查看，安装了多少第三方库文件。
此外，安装的话
同时，在windows下确实很多package不好装。可以下载python包到本地，命令行进入文件夹（有setup.py路径下），输入：
$ python setup.py install
则可以将第三方库安装到python中。





