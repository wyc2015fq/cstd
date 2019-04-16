# Python 文件处理的几点注意事项 - 我相信...... - CSDN博客





2015年07月26日 15:18:03[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1379








文件处理在编程中是常见的操作，文件的打开，关闭，重命名，删除，追加，复制，随机读写非常容易理解和使用。需要注意的是文件的安全关闭，采用with语句轻松便捷： 

with open(pathname,”r”) as myfile: 

        do_some_with(myfile)- 
CSV 的文件处理 

 csv 模块可以很好地处理csv文件，而Pandas 模块则可以较好的处理大型的csv文件，还可以处理HTML等，并提供分块处理。

- 
XML 的文件处理 

 对于较小的xml文件，最好使用cElementTree,至少要用ElementTree，对于大型文件，最好用lxml处理。

- 
文件内容的序列化和范序列化 

用pickle 实现序列化和反序列化非常简单dump（）和load（）即可，但要注意，pickle不能实现原子化操作，数据源敏感，存在安全隐患。另一种序列化的形式是json。同样是dumps（），load（），扩展性好，可以指定decoder，性能比pickle差一些。

- 
日志文件处理 

在记录日志文件使用logging 模块是要注意logging是线程安全的，避免多个进程同时写入同一个日志。

- 
图像文件处理 

对于通常的文件的图像处理，PIL 模块足以应付了。在Linux环境下要注意相关图像库的安装，一般ImageMagick 库是首选。如果涉及图像识别等深层次图像处理，就要寻求opencv的帮助了





