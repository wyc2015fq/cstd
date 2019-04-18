# Python sys.argv [ ] - YZXnuaa的博客 - CSDN博客
2018年01月30日 15:06:48[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：116
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
                
因为是看书自学的python,开始后不久就遇到了这个引入的模块函数，且一直在IDLE上编辑了后运行，试图从结果发现它的用途，然而结果一直都是没结果，也在网上查了许多，但发现这个问题的比较详细的解释只有一个版本，大部分都是转裁和复制的。给的都是简明python教程上那个一长串代码的例子，说看了就明白了，可我看得晕头转向的还是没真正明白，只知道“sys.argv[0]表示代码本身文件路径”这点，其实还是不明其意。后来经过大量努力，多方求教才真正明悟了，谨以记录和分享，希望能从另一个角度给同在求索过程中的同学一点启发。
　　sys.argv[]说白了就是一个从程序外部获取参数的桥梁，这个“外部”很关键，所以那些试图从代码来说明它作用的解释一直没看明白。因为我们从外部取得的参数可以是多个，所以获得的是一个列表（list)，也就是说sys.argv其实可以看作是一个列表，所以才能用[]提取其中的元素。其第一个元素是程序本身，随后才依次是外部给予的参数。
下面我们通过一个极简单的test.py程序的运行结果来说明它的用法。
```
1 #test.py
2 
3 import sys
4 a=sys.argv[0]
5 print(a)
```
将test.py保存在c盘的根目录下。
在程序中找到 ‘运行’->点击->输入"cmd"->回车键   进入控制台命令窗口（如下图），先输入cd c:\   (作用是将命令路径改到c盘根目录)，然后输入test.py运行我们刚刚写的程序：
![](https://images2015.cnblogs.com/blog/1122011/201703/1122011-20170324211214361-638728496.png)
得到的结果是C:\test.py，这就是0指代码（即此.py程序）本身的意思。
然后我们将代码中0改为1 :
a=sys.argv[1]
保存后，再从控制台窗口运行，这次我们加上一个参数，输入：test.py what
![](https://images2015.cnblogs.com/blog/1122011/201703/1122011-20170324211227690-1096721685.png)
 得到的结果就是我们输入的参数what，看到这里你是不是开始明白了呢。
那我们再把代码修改一下：
a=sys.argv[2：]
保存后，再从控制台窗台运行程序，这次多加几个参数，以空格隔开:
test.py a b c d e f
![](https://images2015.cnblogs.com/blog/1122011/201703/1122011-20170324211300346-1171068647.png)
得到的结果为[‘b’, ’c’, ’d’, ’e’, ’f’]
sys.argv[]是用来获取命令行参数的，sys.argv[0]表示代码本身文件路径，所以参数从1开始，以下两个例子说明:
1、使用sys.argv[]的一简单实例:
以下是sample1.py文件：
**[python]**[view plain](http://blog.csdn.net/sxingming/article/details/52074311#)[copy](http://blog.csdn.net/sxingming/article/details/52074311#)
- import sys,os   
- print sys.argv  
- os.system(sys.argv[1])  
这个例子os.system接收命令行参数，运行参数指令，cmd命令行带参数运行python sample1.py notepad，将打开记事本程序。
2、这个例子是简明python教程上的，明白它之后你就明白sys.argv[]了。
以下是sample.py文件：
**[python]**[view plain](http://blog.csdn.net/sxingming/article/details/52074311#)[copy](http://blog.csdn.net/sxingming/article/details/52074311#)
- #!/usr/bin/env python  
- #_*_ coding:utf-8 _*_  
- import sys      
- def readfile(filename):  #定义readfile函数，从文件中读出文件内容    
- '''''''''Print a file to the standard output.'''
-     f = file(filename)      
- whileTrue:      
-         line = f.readline()      
- if len(line) == 0:      
- break
- print line, # notice comma  分别输出每行内容    
-     f.close()      
- # Script starts from here  
- print sys.argv    
- if len(sys.argv) < 2:      
- print'No action specified.'
-     sys.exit()      
- if sys.argv[1].startswith('--'):      
-     option = sys.argv[1][2:]      
- # fetch sys.argv[1] but without the first two characters    
- if option == 'version':  #当命令行参数为-- version，显示版本号    
- print'Version 1.2'
- elif option == 'help':  #当命令行参数为--help时，显示相关帮助内容    
- print'''  
- This program prints files to the standard output.    
- Any number of files can be specified.    
- Options include:    
-   --version : Prints the version number    
-   --help    : Display this help'''      
- else:      
- print'Unknown option.'
-     sys.exit()      
- else:      
- for filename in sys.argv[1:]: #当参数为文件名时，传入readfile，读出其内容    
-         readfile(filename)   
