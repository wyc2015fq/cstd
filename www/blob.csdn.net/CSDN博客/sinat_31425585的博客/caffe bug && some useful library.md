# caffe bug && some useful library - sinat_31425585的博客 - CSDN博客
2018年11月07日 22:55:35[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：106
个人分类：[Caffe](https://blog.csdn.net/sinat_31425585/article/category/6764176)
1、**relocation R_X86_64_32S against `.rodata' can not be used when making a shared object; recompile with -fPIC #2171**
解决方案：
[https://github.com/BVLC/caffe/issues/2171](https://github.com/BVLC/caffe/issues/2171)
2、**libstdc++.so.6: version `GLIBCXX_3.4.21' not found**
解决方案：安装一个3.4.21版本的gcc
```python
conda install libgcc
```
参考资料：
[https://blog.csdn.net/a8039974/article/details/78544209](https://blog.csdn.net/a8039974/article/details/78544209)
3、**protoc: error while loading shared libraries: libprotoc.so.14: cannot open shared object file: No such file or directory**
解决方案：将protoc安装的库路径加入到系统变量
```cpp
sudo vim ld.so.conf
```
然后将/usr/local/lib粘贴到文件中
最后执行：
```cpp
sudo ldconfig
```
让环境变量生效。
参考资料：
[https://blog.csdn.net/weixin_37251044/article/details/79139906](https://blog.csdn.net/weixin_37251044/article/details/79139906)
4、**系统无法识别protobuf**
可以直接用protoc生成cpp文件：
```cpp
protoc.exe  --cpp_out=./  *.proto
```
参考资料：
[https://blog.csdn.net/shimazhuge/article/details/78091113](https://blog.csdn.net/shimazhuge/article/details/78091113)
5、**将anaconda添加到系统环境变量中**
先打开.bashrc文件
```python
vim ~/.bashrc
```
在末尾添加：
```python
export PATH=/home/grant/anaconda2/bin:$PATH
```
然后source一下，让更改后的环境变量生效
```python
source ~/.bashrc
```
参考资料：
[https://www.linuxidc.com/Linux/2016-08/134259.htm](https://www.linuxidc.com/Linux/2016-08/134259.htm)
6、**cmake安装**
下载cmake的source文件，并解压，进入压缩文件，运行：
```cpp
./bootstrap
make -j8
sudo make install
```
参考资料：
[https://blog.csdn.net/lj402159806/article/details/76408597/](https://blog.csdn.net/lj402159806/article/details/76408597/)
7、**源码编译opencv后，python中：**
```python
import cv2
```
出现一个错误，找不到cv2
解决方案：
将build->install->lib路径下的cv2.so文件拷贝到anaconda2->lib->python2.7->site-packages路径下即可
```cpp
pip install opencv-python==3.4.4.19
```
8、**ImportError: No module named google.protobuf.internal**
解决方案：
```python
pip install argparse --user
pip install PyHamcrest --user
pip install protobuf --user
```
参考资料：
[https://www.cnblogs.com/yyxf1413/p/7927985.html](https://www.cnblogs.com/yyxf1413/p/7927985.html)
9、**神TM的Permission denied**
解决方案：
```cpp
chmod -R 777 directory
```
参考资料：
[https://blog.csdn.net/qq_16525279/article/details/80245350](https://blog.csdn.net/qq_16525279/article/details/80245350)
10、**妈蛋，手贱把annaconda里面的lib文件夹删除了，然后不管输什么命令，都报下面这个错误**
ERROR: File or directory already exists: '/home/ubuntu/anaconda3'
If you want to update an existing installation, use the -u option.
ubuntu@ai-server:~/share$ python
Could not find platform independent libraries <prefix>
Could not find platform dependent libraries <exec_prefix>
Consider setting $PYTHONHOME to <prefix>[:<exec_prefix>]
Fatal Python error: initfsencoding: Unable to get the locale encoding
ModuleNotFoundError: No module named 'encodings'
Current thread 0x00007f0f748ef700 (most recent call first):
已放弃 (核心已转储)
解决方法：
重新安装一个anaconda，将对应文件夹替换掉你误删除的文件夹。
**11、安装cudnn版本过高**
```
./include/caffe/util/cudnn.hpp:23:10: warning: enumeration value ‘CUDNN_STATUS_RUNTIME_PREREQUISITE_MISSING’ not handled in switch [-Wswitch]
   switch (status) {
          ^
```
解决方案：将最新的caffe下下来，把caffe-master\include\caffe\util文件夹下面的cudnn.hpp文件复制到对应位置，重新编译就行了。
**12、caffe在ubuntu上安装最大的坑就是protobuf**
解决方法：先源码安装一次，再用conda安装一个相同版本的。
参考资料：
[https://www.jianshu.com/p/9da384f8d7b2](https://www.jianshu.com/p/9da384f8d7b2)
[https://github.com/BVLC/caffe/issues/5793](https://github.com/BVLC/caffe/issues/5793)
**13、对‘TIFFIsTiled@LIBTIFF_4.0’未定义的引用**
解决方法：编译opencv时，取消对libtiff库的编译选项。
~~~未完待续~~~
