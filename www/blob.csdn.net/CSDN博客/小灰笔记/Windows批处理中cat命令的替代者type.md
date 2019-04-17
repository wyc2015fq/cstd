# Windows批处理中cat命令的替代者type - 小灰笔记 - CSDN博客





2017年04月09日 20:32:12[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：10075








       在使用bash的时候，cat是一个很常用的命令。通常查看文本内容的时候都会使用cat，它能够展示文本的信息同时还仅仅是对文件的读取操作，并不会改变文件的内容。鉴于有时候有些情况下没有bash使用，比如说有些公司禁止员工自由使用开发工具并且只能够使用Windows，那么Windows下还是有替代者的。唯一需要额外考虑的就是批处理的一点点学习成本。

       查看批处理中type的使用帮助如下：

C:\Users\ThinkPad\Desktop>helptype

显示文本文件的内容。



TYPE[drive:][path]filename

       说明十分简洁，使用也十分简单。

使用示范如下：

C:\Users\ThinkPad\Desktop>typeE:\WorkSpace\code_count.py

#!/usr/bin/python



import os



pwd = os.getcwd()



# extension namesfor files, according to the need to add.

ext_names =['.c','.h','.java','.py','.m','.pl','.bat']



line_num = 0



forroot,dirs,files in os.walk('.'):

        os.chdir(root)

        for each_file in files:

                for ext_name in ext_names:

                        ifeach_file.lower().endswith(ext_name.lower()):

                                line_num +=len(open(each_file,'r').readlines())

                                break

        os.chdir(pwd)

print("codeline number is:" + str(line_num))

       从上面的结果可以看出，目前虽然中断的启动位置位于驱动盘的C盘，但是依然可以用于展示E盘上的文件信息。

       说起来，在使用Windows系统的时候这个功能我其实不常用。Windows中极少在命令行终端中进行频繁的切换或者信息查看，至少现在不常用。不过，后期看看PowerShell的功能是否便捷，如果便捷并且强大的话，类似的功能或许不会少用。



