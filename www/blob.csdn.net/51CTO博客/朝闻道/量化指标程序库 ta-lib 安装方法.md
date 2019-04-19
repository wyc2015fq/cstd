# 量化指标程序库 ta-lib 安装方法-朝闻道-51CTO博客
TA-lib是一个技术分析库，里面包含了大部分主流的技术指标，让使用者不用再重复造轮子。
这个库在国外很常用，各种大型的开源量化框架，都会内置这个库。
计算速度快
TA-lib的一个特点是计算速度快，因为原始版本是用C语言写的，然后通过Cython转化，使得Python也能调用。
然后我在安装这个库的时候 pip install ta-lib发现了错误
具体解决的操作过程如下
先安装一个VS2013 或者其他版本
然后打开
![量化指标程序库 ta-lib 安装方法](https://s1.51cto.com/images/blog/201810/09/bfcac8c5274671c0001e11bdd7046987.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
然后下载tc lib
[http://ta-lib.org/hdr_dw.html](http://ta-lib.org/hdr_dw.html)
![量化指标程序库 ta-lib 安装方法](https://s1.51cto.com/images/blog/201810/09/fdc0f4e7c17eae4b1c1fdbfabdaf8d98.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
下载之后解压到C盘 
然后在上面这个命令行中 cd到 C:\ta-lib\c\make\cdr\win32\msvc 目录下，执行nmake
![量化指标程序库 ta-lib 安装方法](https://s1.51cto.com/images/blog/201810/09/884001770976355812b79e69d99ac6b6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
然后就可以 pip install ta-lib
如果这个方法还报错 就到这里http://mrjbq7.github.io/ta-lib/func_groups/overlap_studies.html下载
然后用 python setup.py build
python setup.py install
