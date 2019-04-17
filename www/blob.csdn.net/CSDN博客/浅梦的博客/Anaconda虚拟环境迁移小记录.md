# Anaconda虚拟环境迁移小记录 - 浅梦的博客 - CSDN博客





2017年01月15日 23:20:55[浅梦s](https://me.csdn.net/u012151283)阅读数：7024








今天尝试了把本机中的虚拟环境envs中的python3环境直接以复制粘贴的形式拷贝到服务器中anaconda envs目录下。

拷贝完成之后使用`activate python3`是可以激活进入python3虚拟环境的，但是使用`conda list`命令却提示找不到命令，解决方案是将服务器中的`Anaconda2\envs\python3\Scripts`目录下的`conda.bat`中的执行路径替换为`C:\Anaconda2\Scripts\conda`也就是服务器中的本来的conda命令所在的路径。 

之后可以把同目录下的`activate.bat`和`deactivate.bat`执行路径都做相应的替换。

还有一个需要注意的地方是`Anaconda2\envs\python3\Library\share\pkgconfig`目录下的`zlib.pc`文件，该文件的中路径也要进行相应的替换，改为服务器中对应的路径即可。



