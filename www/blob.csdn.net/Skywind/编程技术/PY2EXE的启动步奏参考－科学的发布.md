# Skywind Inside » PY2EXE的启动步奏参考－科学的发布
## PY2EXE的启动步奏参考－科学的发布
October 13th, 2006[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
周末读了一下Py2Exe的代码，偶然发现Py2Exe的实现，是一套很成熟的客户端发布方式，不限于只有
Python项目可以参考，Py2exe启动步奏如下：
Py2exe启动步奏方式参考：
1.       主程序启动，尝试在本EXE的资源中找到zip过后的PythonXX.dll的数据
2.       如果找到 PythonXX.dll数据则unzip后以memory dll方式加载并导出接口
3.       初始化 memoryimporter，给python提供在内存中import一个pyc或者dll的功能
4.       利用memoryimporter加载_ctypes.pyd模块
5.       利用py2exe_util.c中的方法，加载程序所需要的资源：ICON等
6.       在资源中初始化 zlib.pyd等必要模块
7.       在资源中找到初始化，以”__main__”的命名开始转入Python脚本
