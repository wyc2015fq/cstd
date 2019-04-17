# 向DOS批处理文件传入任意个数的参数方法实现 - 小灰笔记 - CSDN博客





2017年05月19日 00:06:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：387








      关于Windows下的DOS批处理我虽然使用的不多，但是有限的几个熟悉的命令倒也使用的不少。在一些使用受到很多限制的Windows电脑上，我时常会通过bat文件实现其他脚本语言或者程序提供给用户的工具接口。

      简单的举个例子，由于时常需要创建多个代码文件，我喜欢使用touch命令。然而，平时能够用到的Windows电脑上名没有Windows版本的shell工具也不给我随便添加软件的机会。好的是电脑上有Python等脚本工具，这样我就可以用来模拟一点简单的动作。比如，我会实现Python脚本如下：

#!/usr/binpython



import sys



def Touch(file_name):

      fid =open(file_name,'w')

      fid.close()



create_flg = False

for each_file in sys.argv:

      if create_flg:

            Touch(each_file)

      else:

            create_flg= True

            其中，create_flg使用的目的是避开第一个传入参数。因为第一个传入参数是本身。

      脚本测试的过程：

E:\WorkSpace\01_编程语言\03_Python\demos>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\01_编程语言\03_Python\demos 的目录



2017/05/18 23:52    <DIR>          .

2017/05/18 23:52    <DIR>          ..

2017/05/08 19:15               518 add_dir.py

2017/03/29 21:00    <DIR>          python_exp01

2017/03/29 21:02    <DIR>          python_exp02

2017/04/03 09:23    <DIR>          python_exp03

2017/05/18  23:47    <DIR>          python_exp04

2017/04/12 22:16    <DIR>          python_exp05

2017/05/07 18:53    <DIR>          python_exp06

2017/05/08 19:17    <DIR>          python_exp07

2017/05/08 19:52    <DIR>          python_exp08

2017/05/18 23:52               214 touch.py

               2个文件            732 字节

              10个目录77,768,101,888 可用字节



E:\WorkSpace\01_编程语言\03_Python\demos>python touch.py 1.c2.c 3.c



E:\WorkSpace\01_编程语言\03_Python\demos>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\01_编程语言\03_Python\demos 的目录



2017/05/18 23:52    <DIR>          .

2017/05/18 23:52    <DIR>          ..

2017/05/18 23:52                 0 1.c

2017/05/18 23:52                 0 2.c

2017/05/18 23:52                 0 3.c

2017/05/08 19:15               518 add_dir.py

2017/03/29 21:00    <DIR>          python_exp01

2017/03/29 21:02    <DIR>          python_exp02

2017/04/03 09:23    <DIR>          python_exp03

2017/05/18 23:47    <DIR>          python_exp04

2017/04/12 22:16    <DIR>          python_exp05

2017/05/07 18:53    <DIR>          python_exp06

2017/05/08 19:17    <DIR>          python_exp07

2017/05/08 19:52    <DIR>          python_exp08

2017/05/18 23:52               214 touch.py

               5个文件            732 字节

              10个目录77,768,101,888 可用字节

      通过上面的结果可以看出，使用脚本可以实现新文本文件的批量创建。但是每次都使用这个命令才进行这种操作还是有些麻烦，因为每次我们都需要增加一个python字符的输入。为了能够实现随时调用，写一个批处理文件并将其放到Windows可以寻找到的Path目录中。

      先说一下初期我使用的批处理脚本，最初的使用批处理文件中只有一句：

            @python dir/touch.py %1

      其中，%1的含义是输入的第一个参数，而dir则是脚本所在的实际目录。这样，可以做如下测试：

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:01    <DIR>          .

2017/05/19 00:01    <DIR>          ..

               0个文件              0 字节

               2个目录78,496,571,392 可用字节



C:\Users\ThinkPad\Desktop\demo>touch 1.c

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:02    <DIR>          .

2017/05/19 00:02    <DIR>          ..

2017/05/19 00:02                 0 1.c

               1个文件              0 字节

               2个目录78,496,571,392 可用字节



C:\Users\ThinkPad\Desktop\demo>touch 1.c 2.c

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:02    <DIR>          .

2017/05/19 00:02    <DIR>          ..

2017/05/19 00:02                 0 1.c

               1个文件              0 字节

               2个目录78,496,571,392 可用字节



C:\Users\ThinkPad\Desktop\demo>touch 2.c 3.c

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:02    <DIR>          .

2017/05/19 00:02    <DIR>          ..

2017/05/19 00:02                 0 1.c

2017/05/19 00:02                 0 2.c

               2个文件              0 字节

               2个目录78,496,571,392 可用字节

      通过这样的方法实现一个参数的传入是十分简单的，但是弊端是对于多余一个参数的传入不成功。就是后面多加几个参数也不是什么好的解决方法。后来，经过自己尝试发现，使用通配符的方式可以实现多个参数的准确传入。修改批处理如下：

      @python C:\Python27\touch.py %*

      再次测试：

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:04    <DIR>          .

2017/05/19 00:04    <DIR>          ..

               0个文件              0 字节

               2个目录78,496,595,968 可用字节



C:\Users\ThinkPad\Desktop\demo>touch 1.c 2.c 3.c

C:\Users\ThinkPad\Desktop\demo>dir

 驱动器 C 中的卷没有标签。

 卷的序列号是 28BD-E720



 C:\Users\ThinkPad\Desktop\demo 的目录



2017/05/19 00:04    <DIR>          .

2017/05/19 00:04    <DIR>          ..

2017/05/19 00:04                 0 1.c

2017/05/19 00:04                 0 2.c

2017/05/19 00:04                 0 3.c

               3个文件              0 字节

               2个目录78,496,595,968 可用字节

      从上面的结果可以看出，这次的功能实现就要实用多了。



