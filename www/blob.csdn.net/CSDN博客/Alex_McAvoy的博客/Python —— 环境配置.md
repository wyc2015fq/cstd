# Python —— 环境配置 - Alex_McAvoy的博客 - CSDN博客





2018年08月14日 16:25:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：526
所属专栏：[Python 学习笔记](https://blog.csdn.net/column/details/35090.html)









# 【下载】

根据需要选择版本，地址：[点击这里](https://www.python.org/downloads/)

选择相应版本后，有如下三种安装方式：
- embeddable zip：是将 python 打包成 zip 压缩包，下载即用
- executable：可执行文件，将要安装的 python 下载好，然后本机安装
- web-based：通过网络安装，执行安装后，通过网络下载相应的包，再进行安装

![](https://img-blog.csdnimg.cn/20190311200258491.png)

# 【安装】

以 Python 3.7.2 executable installe 为例

## 1）选择安装方式

Install Now 会默认安装到 C 盘，此处选择 Customize installation，此外，在选择 Customize installation 安装之前，记得勾选最下方的 Add Python 3.7 to PATH 来自动添加环境变量
![](https://img-blog.csdnimg.cn/20190311200939736.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 2）选定特性

选择 Customize installation 后，进入选择特性页面，根据实际需要进行勾选
- Documentation：Python 的英文文档
- pip：Python 包管理工具，提供对 Python 包的查找、下载、安装、卸载功能
- tcl/tk and IDLE：Python 自带的集成开发环境，可以它方便地创建、运行、测试和调试 Python 程序
- Python test suite：Python 测试套件
- py launcher：可通过全局命令 py 来启动 Python，安装好后，可通过注册表与文件管理器中找到 py.exe 的位置
- for all user：为所有用户按照 py launcher 

![](https://img-blog.csdnimg.cn/20190311201009324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

### 3）高级选项

选择 Next 后，进入 Advanced Options 页面，根据实际需要进行勾选
- Install for all users：是否允许 Python 令系统的所有用户使用
- Associate files with Python：是否将 Python 相关文件与 Python 关联
- Create shortcuts for installed applications：是否创建桌面快捷方式
- Add Python to environment variables：是否添加环境变量
- Precompile standard library：是否预编译公共库
- Download debugging symbols：是否下载调试符号，调试符号是为定位调试出错的代码行数，若 Python 是用作开发环境的话，推荐勾选，若用作运行环境的话，可以不勾选
- Download debug binaries：是否下载用于VS的调试符号，如果不使用VS作为开发工具，则无需勾选


![](https://img-blog.csdnimg.cn/20190311200952758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 4）安装成功

在高级选项中选择好路径并点击 Install 后，等待按照完成即可

![](https://img-blog.csdnimg.cn/20190311201021368.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)



