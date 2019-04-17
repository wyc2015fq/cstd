# Python调用Windows CMD命令删除文件异常分析与解决 - 小灰笔记 - CSDN博客





2017年02月24日 21:17:58[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：616








       Windows下偶尔需要用到使用Python调用系统中的命令处理某些东西，在处理文件的时候偶尔会遇到问题。问题基本上都会与Windows的CMD命令使用有关，而我对这个不是很熟悉。刚刚尝试进行文件的批量删除发现遇到点小问题，做个简单的小例子说明一下。

       以下是用于测试的目录以及文件的目录树结构：



卷序列号为 00000031 2013:5089

E:.

│ exp_01.py

│

├─q a

│     a（）【】1.txt

│

└─【】（）

       a c.txt



       写一个Python脚本如下：

#!/usr/bin/python



import os



pwd = os.getcwd()



forroot,dirs,files in os.walk('./'):

       os.chdir(root)

       for each_file in files:

              print("removing %s in%s" %(each_file,root))

              try:

                     os.system("del%s" % each_file)

              except:

                     print("remove %sfailed!" % each_file)

       os.chdir(pwd)

       运行结果如下;

E:\WorkSpace\01_编程语言\04_Python\exp_01>python exp_01.py

removing a（）【】1.txt in ./q a

removing a c.txtin ./【】（）

找不到E:\WorkSpace\01_编程语言\04_Python\exp_01\【】（）\a

       还不错，只有一个错误报出来了。在看一下现在的目录树信息：

文件夹 PATH 列表

卷序列号为 000000E32013:5089

E:.

│  exp_01.py

│

├─q a

└─【】（）

        a c.txt

       由此大致可以得出这样的结论：程序切换目录的时候用的是Python自带的功能，即使是调用了操作系统的接口在使用上也可以看做是Python固有的特性。而通过os模块调用系统功能的时候，调用的功能原则上是需要跟操作系统上功能使用方式完全一样。这样，出现的问题就是因为”a c.txt”的方式被识别成了两个被空格分开的参数。我做了一下尝试，其实这种情况下把不能够分开的文件名用引号引起来，便是正确的用法。于是，恢复被删除改动过的环境，再次修改代码如下;

#!/usr/bin/python



import os



pwd = os.getcwd()



forroot,dirs,files in os.walk('./'):

       os.chdir(root)

       for each_file in files:

              if each_file != 'exp_01.py':

                     print("removing %s in%s" %(each_file,root))

                     try:

                            os.system("del%s" % ('"'+each_file+'"'))

                     except:

                            print("remove%s failed!" % each_file)

       os.chdir(pwd)

程序运行结果：

E:\WorkSpace\01_编程语言\04_Python\exp_01>python exp_01.py

removing a（）【】1.txt in ./q a

removing a c.txtin ./【】（）

再看一下目录树;

文件夹 PATH 列表

卷序列号为 000000F92013:5089

E:.

│  exp_01.py

│

├─q a

└─【】（）

       这次运行中没有遇到异常，所有文件都被成功删除掉。需要注意的是，代码中重新“包装”Windows CMD命令行的时候，在不能分割的文件名两边加的是双引号，这个是必须如此的。不像Python中单引号双引号的使用没有什么区别，在Windows的CMD命令中这个倒是十分固定的。



