
# Python中shutil模块的学习笔记教程 - jiahaowanhao的博客 - CSDN博客


2018年04月28日 21:21:50[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：52标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python中shutil模块的学习笔记教程](http://cda.pinggu.org/view/25397.html)
shutil 名字来源于 shell utilities，有学习或了解过Linux的人应该都对 shell 不陌生，可以借此来记忆模块的名称。该模块拥有许多文件（夹）操作的功能，包括复制、移动、重命名、删除等等
一、chutil.copy(source, destination)
shutil.copy() 函数实现文件复制功能，将 source 文件复制到 destination 文件夹中，两个参数都是字符串格式。如果 destination 是一个文件名称，那么它会被用来当作复制后的文件名称，即等于 复制 + 重命名。
举例如下：
>> import shutil
>> import os
>> os.chdir('C:\')
>> shutil.copy('C:\spam.txt', 'C:\delicious')
'C:\delicious\spam.txt'
>> shutil.copy('eggs.txt', 'C:\delicious\eggs2.txt')
'C:\delicious\eggs2.txt'
如代码所示，该函数的返回值是复制成功后的字符串格式的文件路径
二、shutil.copytree(source, destination)
shutil.copytree()函数复制整个文件夹，将 source 文件夹中的所有内容复制到 destination 中，包括 source 里面的文件、子文件夹都会被复制过去。两个参数都是字符串格式。
注意：如果 destination 文件夹已经存在，该操作并返回一个 FileExistsError 错误，提示文件已存在。即表示，如果执行了该函数，程序会自动创建一个新文件夹（destination参数）并将 source 文件夹中的内容复制过去
举例如下：
>> import shutil
>> import os
>> os.chdir('C:\')
>> shutil.copytree('C:\bacon', 'C:\bacon_backup')
\'C:\bacon_backup'
如以上代码所示，该函数的返回值是复制成功后的文件夹的绝对路径字符串
所以该函数可以当成是一个备份功能
三、shutil.move(source, destination)
shutil.move() 函数会将 source 文件或文件夹移动到 destination 中。返回值是移动后文件的绝对路径字符串。
如果 destination 指向一个文件夹，那么 source 文件将被移动到 destination 中，并且保持其原有名字。例如：
>> import shutil
>> shutil.move('C:\bacon.txt', 'C:\eggs')
'C:\eggs\bacon.txt'
上例中，如果 C:\eggs 文件夹中已经存在了同名文件 bacon.txt，那么该文件将被来自于 source 中的同名文件所重写。
如果 destination 指向一个文件，那么 source 文件将被移动并重命名，如下：
>> shutil.move('C:\bacon.txt', 'C:\eggs\new_bacon.txt')
'C:\eggs\new_bacon.txt'
等于是移动+重命名
<b>注意，如果 destination 是一个文件夹，即没有带后缀的路径名，那么 source 将被移动并重命名为 destination</b>，如下：
>> shutil.move('C:\bacon.txt', 'C:\eggs')
'C:\eggs'
即 bacon.txt 文件已经被重命名为 eggs，是一个没有文件后缀的文件
最后，destination 文件夹必须是已经存在的，否则会引发异常：
>> shutil.move('spam.txt', 'C:\does_not_exist\eggs\ham')
Traceback (most recent call last):
File "D:\Python36\lib\shutil.py", line 538, in move
os.rename(src, real_dst)
FileNotFoundError: [WinError 3] 系统找不到指定的路径。: 'test.txt' -> 'C:\does_not_exist\eggs\ham'
During handling of the above exception, another exception occurred:
Traceback (most recent call last):
File "
四、永久性删除文件和文件夹
这里有涉及到 os 模块中的相关函数
os.unlink(path) 会删除 path 路径文件
os.rmdir(path)　会删除 path 路径文件夹，但是这个文件夹必须是空的，不包含任何文件或子文件夹
shutil.rmtree(path) 会删除 path 路径文件夹，并且在这个文件夹里面的所有文件和子文件夹都会被删除
利用函数执行删除操作时，应该倍加谨慎，因为如果想要删除 txt 文件，而不小心写到了 rxt ，那么将会给自己带来麻烦
此时，我们可以利用字符串的 endswith 属性对文件格式进行检查与筛选
总结
以上就是这篇文章的全部内容了，希望本文的内容对大家的学习或者工作能带来一定的帮助.

