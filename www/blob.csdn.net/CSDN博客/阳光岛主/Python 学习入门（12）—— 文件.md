
# Python 学习入门（12）—— 文件 - 阳光岛主 - CSDN博客

2013年11月28日 23:50:23[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4826个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



python进行文件读写的函数是open或file：f = open(filename, mode）
|模式
|描述
|
|r
|以读方式打开文件，可读取文件信息。
|
|w
|以写方式打开文件，可向文件写入信息。如文件存在，则清空该文件，再写入新内容
|
|a
|以追加模式打开文件（即一打开文件，文件指针自动移到文件末尾），如果文件不存在则创建|
|r+
|以读写方式打开文件，可对文件进行读和写操作。
|
|w+
|消除文件内容，然后以读写方式打开文件。
|
|a+
|以读写方式打开文件，并把文件指针移到文件尾。
|
|b
|以二进制模式打开文件，而不是以文本模式。该模式只对Windows或Dos有效，类Unix的文件是用二进制模式进行操作的。
|

**Table 文件对象方法**
|方法
|描述
|
|f.open(file, mode)
|打开文件，file是文件名，mode是打开模式（r/r+，w/w+， a/a+， b）
|
|f.close()
|关闭文件，记住用open()打开文件后一定要记得关闭它，否则会占用系统的可打开文件句柄数。
|
|f.fileno()
|获得文件描述符，是一个数字
|
|f.flush()
|刷新输出缓存
|
|f.isatty()
|如果文件是一个交互终端，则返回True，否则返回False。
|
|f.read([count])
|读出文件，如果有count，则读出count个字节。
|
|f.readline()
|读出一行信息。
|
|f.readlines()
|读出所有行，也就是读出整个文件的信息。
|
|f.seek(offset[,where])
|把文件指针移动到相对于where的offset位置。where为0表示文件开始处，这是默认值 ；1表示当前位置；2表示文件结尾。
|
|f.tell()
|获得文件指针位置。
|
|f.truncate([size])
|截取文件，使文件的大小为size。
|
|f.write(string)
|把string字符串写入文件。
|
|f.writelines(list)
|把list中的字符串一行一行地写入文件，是连续写入文件，没有换行。
|

读写文件

```python
#/usr/bin/python
#
# ithomer in 2013

filename = "test_file.txt"
def write_file():
    f = open(filename, 'w')
    f.write('hello ithomer')
    f.write('\n')
    f.write('my blog: http://blog.ithomer.net')
    f.write('\n')
    f.write('this is the end')
    f.write('\n')
    f.close()
def read_file():
    f = open(filename, 'r')
    line = f.readline()
    while line:
        print line
        line = f.readline()
    f.close()
if __name__ == "__main__":
    write_file()
    read_file()
```
运行结果：
hello ithomer
my blog: http://blog.ithomer.net
this is the end


**文件操作示例：**
```python
#/usr/bin/python
# 
# ithomer in 2013

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

f = open('test_file.txt','r')           #以读方式打开文件，rb为二进制方式(如图片或可执行文件等)  
  
print 'read()'                          #读取整个文件  
print f.read()  
  
print 'readline() f.seek(0)'            #返回文件头，读取一行  
f.seek(0)  
print f.readline()  
  
print 'readline() f.seek(1)'            #返回文件头，读取一行  
print f.tell()                          #显示当前位置  
f.seek(1)  
print f.tell()                          #显示当前位置  
print f.readline()  
  
print 'readline() f.seek(10)'          #返回文件头，读取一行  
print f.tell()                
f.seek(10)  
print f.tell()                
print f.readline()  
print f.tell()                
print f.readline()  
print 'readline() f.seek(15)'          #返回文件头，读取一行  
print f.tell()                
f.seek(15)  
print f.tell()                
print f.readline()  
print f.tell()                
print f.readline()  
  
  
print 'readlines()'                     #返回文件头，返回所有行的列表  
f.seek(0)  
print f.readlines()  
print
  
print 'list all lines'                  #返回文件头，显示所有行  
print 'readlines()'                     #返回文件头，返回所有行的列表  
f.seek(0)  
print f.readlines()  
print
  
print 'list all lines'                  #返回文件头，显示所有行  
f.seek(0)  
textlist = f.readlines()  
for line in textlist:  
    print line,  
print   
  
print 'seek(15) function'               #移位到第15个字符，从16个字符开始显示余下内容  
f.seek(15)  
print 'tell() function'  
print f.tell()                          #显示当前位置  
print f.read()  
  
f.close()                               #关闭文件句柄  

''' 
======== output result  =========
read()
hello world
my blog is ithomer.net
this is end.
readline() f.seek(0)
hello world
readline() f.seek(1)
12
1
ello world
readline() f.seek(10)
12
10
d
12
my blog is ithomer.net
readline() f.seek(15)
35
15
blog is ithomer.net
35
this is end.
readlines()
['hello world\n', 'my blog is ithomer.net\n', 'this is end.\n']
list all lines
readlines()
['hello world\n', 'my blog is ithomer.net\n', 'this is end.\n']
list all lines
hello world
my blog is ithomer.net
this is end.
seek(15) function
tell() function
15
blog is ithomer.net
this is end.
'''
```

python中对文件、文件夹（文件操作函数）的操作需要涉及到os模块和shutil模块

**常用操作**
得到当前工作目录，即当前Python脚本工作的目录路径:os.getcwd()
返回指定目录下的所有文件和目录名:os.listdir()
函数用来删除一个文件:os.remove()
删除多个目录：os.removedirs（r“c：\python”）
检验给出的路径是否是一个文件：os.path.isfile()
检验给出的路径是否是一个目录：os.path.isdir()
判断是否是绝对路径：os.path.isabs()
检验给出的路径是否真地存:os.path.exists()
返回一个路径的目录名和文件名:os.path.split()eg os.path.split('/home/swaroop/byte/code/poem.txt') 结果：('/home/swaroop/byte/code', 'poem.txt')
分离扩展名：os.path.splitext()
获取路径名：os.path.dirname()
获取文件名：os.path.basename()
运行shell命令:os.system()
读取和设置环境变量:os.getenv() 与os.putenv()
给出当前平台使用的行终止符:os.linesepWindows使用'\r\n'，Linux使用'\n'而Mac使用'\r'
指示你正在使用的平台：os.name对于Windows，它是'nt'，而对于Linux/Unix用户，它是'posix'
重命名：os.rename（old， new）
创建多级目录：os.makedirs（r“c：\python\test”）
创建单个目录：os.mkdir（“test”）
获取文件属性：os.stat（file）
修改文件权限与时间戳：os.chmod（file）
终止当前进程：os.exit（）
获取文件大小：os.path.getsize（filename）
**文件操作**
os.mknod("test.txt")创建空文件
fp = open("test.txt",w)直接打开一个文件，如果文件不存在则创建文件
关于open 模式：
w     以写方式打开，
a     以追加模式打开 (从 EOF 开始, 必要时创建新文件)
r+     以读写模式打开
w+     以读写模式打开 (参见 w )
a+     以读写模式打开 (参见 a )
rb     以二进制读模式打开
wb     以二进制写模式打开 (参见 w )
ab     以二进制追加模式打开 (参见 a )
rb+    以二进制读写模式打开 (参见 r+ )
wb+    以二进制读写模式打开 (参见 w+ )
ab+    以二进制读写模式打开 (参见 a+ )
fp.read([size])\#size为读取的长度，以byte为单位
fp.readline([size])\#读一行，如果定义了size，有可能返回的只是一行的一部分
fp.readlines([size])\#把文件每一行作为一个list的一个成员，并返回这个list。其实它的内部是通过循环调用readline()来实现的。如果提供size参数，size是表示读取内容的总长，也就是说可能只读到文件的一部分。
fp.write(str)\#把str写到文件中，write()并不会在str后加上一个换行符
fp.writelines(seq)\#把seq的内容全部写到文件中(多行一次性写入)。这个函数也只是忠实地写入，不会在每行后面加上任何东西。
fp.close()\#关闭文件。python会在一个文件不用后自动关闭文件，不过这一功能没有保证，最好还是养成自己关闭的习惯。  如果一个文件在关闭后还对其进行操作会产生ValueError
fp.flush()\#把缓冲区的内容写入硬盘
fp.fileno()\#返回一个长整型的”文件标签“
fp.isatty()\#文件是否是一个终端设备文件（unix系统中的）
fp.tell()\#返回文件操作标记的当前位置，以文件的开头为原点
fp.next()\#返回下一行，并将文件操作标记位移到下一行。把一个file用于for … in file这样的语句时，就是调用next()函数来实现遍历的。
fp.seek(offset[,whence])\#将文件打操作标记移到offset的位置。这个offset一般是相对于文件的开头来计算的，一般为正数。但如果提供了whence参数就不一定了，whence可以为0表示从头开始计算，1表示以当前位置为原点计算。2表示以文件末尾为原点进行计算。需要注意，如果文件以a或a+的模式打开，每次进行写操作时，文件操作标记会自动返回到文件末尾。
fp.truncate([size])\#把文件裁成规定的大小，默认的是裁到当前文件操作标记的位置。如果size比文件的大小还要大，依据系统的不同可能是不改变文件，也可能是用0把文件补到相应的大小，也可能是以一些随机的内容加上去。

**目录操作**
os.mkdir("file")创建目录
复制文件：
shutil.copyfile("oldfile","newfile")oldfile和newfile都只能是文件
shutil.copy("oldfile","newfile")oldfile只能是文件夹，newfile可以是文件，也可以是目标目录
复制文件夹：
shutil.copytree("olddir","newdir")olddir和newdir都只能是目录，且newdir必须不存在
重命名文件（目录）
os.rename("oldname","newname")文件或目录都是使用这条命令
移动文件（目录）
shutil.move("oldpos","newpos")
删除文件
os.remove("file")
删除目录
os.rmdir("dir")只能删除空目录
shutil.rmtree("dir")空目录、有内容的目录都可以删
转换目录
os.chdir("path")换路径


# [ 文件目录操作](http://www.cnblogs.com/bluescorpio/archive/2013/06/02/3113495.html)
如果想切换到和当前目录的其他目录，可以使用
os.path.split(os.getcwd())[0]得到当前目录的父目录，然后使用os.path.join方法去得到目标文件的地址
filepath = os.path.join(os.path.split(os.getcwd())[0], "template\test.xml")
os和os.path模块
os.listdir(dirname)：列出dirname下的目录和文件
os.getcwd()：获得当前工作目录
os.curdir:返回但前目录（'.')
os.chdir(dirname):改变工作目录到dirname
os.path.isdir(name):判断name是不是一个目录，name不是目录就返回false
os.path.isfile(name):判断name是不是一个文件，不存在name也返回false
os.path.exists(name):判断是否存在文件或目录name
os.path.getsize(name):获得文件大小，如果name是目录返回0L
os.path.abspath(name):获得绝对路径
os.path.normpath(path):规范path字符串形式
os.path.split(name):分割文件名与目录（事实上，如果你完全使用目录，它也会将最后一个目录作为文件名而分离，同时它不会判断文件或目录是否存在）
os.path.splitext():分离文件名与扩展名
os.path.join(path,name):连接目录与文件名或目录
os.path.basename(path):返回文件名
os.path.dirname(path):返回文件路径

**操作实例**
将文件夹下所有图片名称加上'_fc'，python代码：

```python
# -*- coding:utf-8 -*-
import re
import os
import time
#str.split(string)分割字符串
#'连接符'.join(list) 将列表组成字符串
def change_name(path):
    global i
    if not os.path.isdir(path) and not os.path.isfile(path):
        return False
    if os.path.isfile(path):
        file_path = os.path.split(path) #分割出目录与文件
        print(file_path)
        lists = file_path[1].split('.') #分割出文件与文件扩展名
        file_ext = lists[-1]            #取出后缀名(列表切片操作)
        img_ext = ['bmp','jpeg','gif','psd','png','jpg']
        if file_ext in img_ext:
            path2 = file_path[0]+'/'+lists[0]+'_fc.'+file_ext
            os.rename(path, path2)
            print("old = %s" % path)
            print("new = %s" % path)
            i+=1 #注意这里的i是一个陷阱
        #或者
        #img_ext = 'bmp|jpeg|gif|psd|png|jpg'
        #if file_ext in img_ext:
        #    print('ok---'+file_ext)
    elif os.path.isdir(path):
        for x in os.listdir(path):
            change_name(os.path.join(path, x)) #os.path.join()在路径处理上很有用
img_dir = '\\home\\homer\\images'
print img_dir
img_dir = img_dir.replace('\\','/')
print img_dir
start = time.time()
i = 0
change_name(img_dir)
c = time.time() - start
print('程序运行耗时:%12.10f'%(c))
print('总共处理了 %s 张图片'%(i))
```

**运行结果：**
\home\homer\images
/home/homer/images
('/home/homer/images', '4.png')
old = /home/homer/images/4.png
new = /home/homer/images/4.png
('/home/homer/images', '5.jpg')
old = /home/homer/images/5.jpg
new = /home/homer/images/5.jpg
('/home/homer/images', '3.jpg')
old = /home/homer/images/3.jpg
new = /home/homer/images/3.jpg
('/home/homer/images', '6.png')
old = /home/homer/images/6.png
new = /home/homer/images/6.png
('/home/homer/images', '1.jpg')
old = /home/homer/images/1.jpg
new = /home/homer/images/1.jpg
('/home/homer/images', '2.jpg')
old = /home/homer/images/2.jpg
new = /home/homer/images/2.jpg
程序运行耗时:0.0006089211
总共处理了 6 张图片

**遍历文件夹示例**

```python
import os
cur_path = os.getcwd()
IMG_PATH = cur_path + "/1/"
IMG_PATH = "/home/homer/images/"
CDN_URL = "http://localhost:8080/homer/poster?type=%s&url=%s"
def scan_img_path():
        
        for file in os.listdir(IMG_PATH):
            print file
            imgurl = IMG_PATH + file
            imgtype = file.split(".")[1]
            imgname = file.split(".")[0]
            
            cnd_upload_url = CDN_URL % (imgtype, imgurl)
            print cnd_upload_url
            
#             cmd = 'curl  "%s"' % cnd_upload_url
#             ret = os.system(cmd)
#             print("%s  \n" % ret)
def main():
    scan_img_path()
    
if __name__ == '__main__':
    main()
    print("end")
```
**文件夹图片：**
homer@ubuntu:~$ls  -l  /home/homer/images/
-rw-rw-r-- 1 homer homer 4072610  3月 13 16:30 001_B.jpg
-rw-rw-r-- 1 homer homer  171994  3月 13 18:55 001_F.png
-rw-rw-r-- 1 homer homer 1393710  3月 13 18:46 001_S.jpg
-rw-rw-r-- 1 homer homer 1418663  3月 13 16:40 002_B.jpg
-rw-rw-r-- 1 homer homer  166635  3月 13 18:54 002_F.png
-rw-rw-r-- 1 homer homer  681094  3月 13 18:22 002_S.jpg
-rw-rw-r-- 1 homer homer 3047430  3月 13 17:20 003_B.jpg
-rw-rw-r-- 1 homer homer  172479  3月 13 18:55 003_F.png
-rw-rw-r-- 1 homer homer  882476  3月 13 18:51 003_S.jpg
-rw-rw-r-- 1 homer homer 2453056  3月 13 17:21 004_B.jpg
-rw-rw-r-- 1 homer homer  107173  3月 13 18:54 004_F.png
-rw-rw-r-- 1 homer homer  827485  3月 13 18:53 004_S.jpg
**运行结果：**003_B.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/003_B.jpg
004_S.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/004_S.jpg
001_B.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/001_B.jpg
001_S.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/001_S.jpg
003_S.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/003_S.jpg
001_F.png
http://localhost:8080/homer/poster?type=png&url=/home/homer/images/001_F.png
004_B.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/004_B.jpg
003_F.png
http://localhost:8080/homer/poster?type=png&url=/home/homer/images/003_F.png
004_F.png
http://localhost:8080/homer/poster?type=db&url=/home/homer/images/Thumbs.db
002_B.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/002_B.jpg
002_S.jpg
http://localhost:8080/homer/poster?type=jpg&url=/home/homer/images/002_S.jpg
002_F.png
http://localhost:8080/homer/poster?type=png&url=/home/homer/images/002_F.png
end


**参考推荐：**
[python对文件进行读写操作](http://sucre.iteye.com/blog/704077)
[python文件操作](http://www.cnblogs.com/rollenholt/archive/2012/04/23/2466179.html)



