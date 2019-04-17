# Python编程中遇到NameError: name 'reload' is not defined 问题以及Python编码问题 - zhusongziye的博客 - CSDN博客





2017年08月28日 09:09:35[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2102








### **一、对于Python 2.X（建议使用Python2.7.X最新版本）：**



import sys

reload(sys)

sys.setdefaultencoding("utf-8")


### **二、对于 <= Python 3.3：**

import imp

imp.reload(sys)




**注意：**

1. [**Python**](http://lib.csdn.net/base/python)** 3**与[**python**](http://lib.csdn.net/base/python)** 2**有**很大的区别**，其中**Python 3**系统默认使用的就是utf-8编码。

2. 所以，对于使用的是**Python 3**的情况，就不需要sys.setdefaultencoding("utf-8")这段代码。

3. **最重要的是**，**Python 3**的**sys**库里面已经**没有** setdefaultencoding() 函数了。

### 三、对**于 >= Python 3.4：（建议新手不要使用python3.X.X最新版本，降2个版本使用，最新版本资料少，不利于学习）**

import importlib

importlib.reload(sys)




** 四、总结一下Python程序中涉及到的编码：**

    在开发Python 程序的过程中，会涉及到三个方面的编码：

    1、Python 程序文件的编码。

我们在编写的程序本身也存在编码问题，一般的解决方式是在程序的开头加上“#coding=utf-8”或

“#coding=gbk”来使程序统一为UTF-8 或GBK 编码。

    2、Python 程序运行时环境（IDE）的编码。

不是管是Sublime Text 或是PyCharm 也它，使用的IDE 工具也存在编码问题。如果你不确定是否是IDE

的编码引起程序出错的，根据我的经验，建议你切换回Python IDLE 去执行程序。

    3、Python 程序读取外部文件、网页的编码。

当然，最容易出现编码问题应该是在读取外部数据或文件的时候。首先要确定读取的数据或文件的编码，

然后通过decode()和encode()方法来进行编码转换。

decode 的作用是将其他编码的字符串转换成Unicode 编码。

encode 的作用是将Unicode 编码转换成其他编码的字符串。

当我们在遇到Python 的编码问题时，从以上三个方法分析就会很容易找到解决编码问题的办法。




更多编码问题，请参考Python官方文档！

﻿﻿



