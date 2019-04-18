# Qt Creator编译出错：‘cl’不是内部或外部命令 - L_Andy的专栏 - CSDN博客

2017年02月23日 20:13:36[卡哥](https://me.csdn.net/L_Andy)阅读数：1873



在安装有vs2010的Win7系统下安装qt-opensource-windows-x86-msvc2010-5.5.1.exe，完成后打开Qt Creator，利用其向导新建一个Qt Application，构建时提示错误：

‘cl’不是内部或外部命令

查看Qt Creator的工具->选项->构建和运行， 
构建套件(Kit)
![这里写图片描述](https://img-blog.csdn.net/20161019141429726)
Qt Version
![这里写图片描述](https://img-blog.csdn.net/20161019141523617)
编译器
![这里写图片描述](https://img-blog.csdn.net/20161019141635961)

配置都正确。详细查看编译器选项时发现，编译器有个初始化对vs所有环境变量的设置 
![这里写图片描述](https://img-blog.csdn.net/20161019141952275)

通过开始菜单栏，选择“visual studio 命令提示（2010）”时发现提示错误： 
![这里写图片描述](https://img-blog.csdn.net/20161019142412339)

error: cannot determine the location of the vs common tools folder

通过开始运行gpedit.msc发现注册表并未被禁用，网上也有修改vcvarsall.bat文件的，有兴趣的可以测试一下。 

最简单的办法将路径C:\Windows\System32添加到环境变量的PATH中去，注销重新登录一下，再次运行“visual studio 命令提示（2010）”时，发现报错信息已经没有了。

同时去构建Qt Application时，也可以构建成功。

