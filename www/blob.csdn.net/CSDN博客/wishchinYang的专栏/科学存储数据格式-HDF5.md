# 科学存储数据格式-HDF5 - wishchinYang的专栏 - CSDN博客
2013年09月19日 09:39:44[wishchin](https://me.csdn.net/wishchin)阅读数：5308
**HDF数据格式**
Hierarchical Data Format，可以存储不同类型的[图像](http://baike.baidu.com/view/42116.htm)和数码数据的文件格式，并且可以在不同类型的机器上传输，同时还有统一处理这种文件格式的函数库。大多数普通计算机都支持这种[文件格式](http://baike.baidu.com/view/1066926.htm)。
转自百度百科：
### 简介
HDF是用于存储和分发科学数据的一种自我描述、多对象文件格式。HDF是由美国国家超级计算应用中心（NCSA）创建的，以满足不同群体的科学家在不同工程项目领域之需要。HDF可以表示出科学数据存储和分布的许多必要条件。HDF被设计为：
自述性：对于一个HDF文件里的每一个[数据对象](http://baike.baidu.com/view/178571.htm)，有关于该数据的综合信息（元数据）。在没有任何外部信息的情况下，HDF允许应用程序解释HDF文件的结构和内容。
通用性：许多[数据类型](http://baike.baidu.com/view/675645.htm)都可以被嵌入在一个HDF文件里。例如，通过使用合适的HDF数据结构，符号、数字和图形数据可以同时存储在一个HDF文件里。
灵活性：HDF允许用户把相关的[数据对象](http://baike.baidu.com/view/178571.htm)组合在一起，放到一个[分层结构](http://baike.baidu.com/view/5712796.htm)中，向数据对象添加描述和标签。它还允许用户把科学数据放到多个HDF文件里。
扩展性：HDF极易容纳将来新增加的数据模式，容易与其他标准格式兼容。
跨平台性：HDF是一个与平台无关的文件格式。HDF文件无需任何转换就可以在不同平台上使用。
**CGNS**
**CGNS**是**CFD General Notation**的缩写，它是一个面向[CFD](http://zh.wikipedia.org/wiki/%E8%AE%A1%E7%AE%97%E6%B5%81%E4%BD%93%E5%8A%9B%E5%AD%A6)数据存储和检索，通用、轻便并且可扩展的标准。它包含一系列的约定，和实现这些约定的[免费开放](http://zh.wikipedia.org/wiki/%E5%BC%80%E6%BA%90%E8%BD%AF%E4%BB%B6)的软件。它具有自描述性，[跨平台](http://zh.wikipedia.org/wiki/%E8%B7%A8%E5%B9%B3%E5%8F%B0)并且和机器无关，由国际指导委员会编写文档并且管理。
CGNS是自描述性的，允许应用程序在没有其它外部信息的时候描述文件的结构和内容。CGNS可以利用两种不同的底层的数据格式
- 一个内部开发并支持的叫做先进数据格式（Andaved Data Format，简称 ADF）的方法，基于先前在McDonnell Douglas中使用的通用文件格式系统；
- [HDF5](http://zh.wikipedia.org/w/index.php?title=HDF5&action=edit&redlink=1)，一个广泛使用的分层数据格式。
HDF5 编程
对于HDF5.h自包含的问题，需要把HDF安装后的src和lib都包含进去，组后添加hdf5.lib，才可以进行底层调用...
