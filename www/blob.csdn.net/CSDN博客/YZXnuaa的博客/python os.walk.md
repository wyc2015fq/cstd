# python os.walk - YZXnuaa的博客 - CSDN博客
2018年01月15日 19:49:48[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：88
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)

是一个简单易用的文件、目录遍历器，可以帮助我们高效的处理文件、目录方面的事情。
1.载入
要使用os.walk,首先要载入该函数
可以使用以下两种方法
- import os
- from os import walk
2.使用
os.walk的函数声明为:
walk(top, topdown=True, onerror=None, followlinks=False)
参数
- top 是你所要便利的目录的地址
- topdown 为真，则优先遍历top目录，否则优先遍历top的子目录(默认为开启)
- onerror 需要一个 callable 对象，当walk需要异常时，会调用
- followlinks 如果为真，则会遍历目录下的快捷方式(linux 下是 symbolic link)实际所指的目录(默认关闭)
os.walk 的返回值是一个生成器(generator),也就是说我们需要不断的遍历它，来获得所有的内容。
每次遍历的对象都是返回的是一个三元组(root,dirs,files)
- root 所指的是当前正在遍历的这个文件夹的本身的地址
- dirs 是一个 list ，内容是该文件夹中所有的目录的名字(不包括子目录)
- files 同样是 list , 内容是该文件夹中所有的文件(不包括子目录)
如果topdown 参数为真，walk 会遍历top文件夹，与top文件夹中每一个子目录。
举个例子
如果我们有如下的文件结构:
```
```
a ->   b   ->   1.txt,  2.txt
             c   ->   3.txt
             d   ->   
           4.txt
           5.txt
```
```
```bash
for (root, dirs, files) in os.walk('a'):
    #第一次运行时，当前遍历目录为 a
    所以 root == 'a'
         dirs == [ 'b', 'c', 'd']
         files == [ '4.txt', '5.txt']
    
    。。。
    # 接着遍历 dirs 中的每一个目录
    b:  root  = 'a\\b'
        dirs  = []
        files = [ '1.txt', '2.txt']
    
    # dirs为空，返回
    # 遍历c
    c:  root  = 'a\\c'
        dirs  = []
        files = [ '3.txt' ]
    
    PS : 如果想获取文件的全路径，只需要 
    for f in files:
        path = os.path.join(root,f)
    
    # 遍历d
    d:  root  = 'a\\b'
        dirs  = []
        files = []
    遍历完毕，退出循环
```
3.简单的例子
保持目录 a 的目录结构，在 b 中创建对应的文件夹,并把a中所有的文件加上后缀 _bak
```python
```python
import os
Root = 'a'
Dest = 'b'
for (root, dirs, files) in os.walk(Root):
    new_root = root.replace(Root, Dest, 1)
    if not os.path.exists(new_root):
        os.mkdir(new_root)
    
    for d in dirs:
        d = os.path.join(new_root, d)
        if not os.path.exists(d):
            os.mkdir(d)
    
    for f in files:
        # 把文件名分解为 文件名.扩展名
        # 在这里可以添加一个 filter，过滤掉不想复制的文件类型，或者文件名
        (shotname, extension) = os.path.splitext(f)
        # 原文件的路径
        old_path = os.path.join(root, f)
        new_name = shotname + '_bak' + extension
        # 新文件的路径
        new_path = os.path.join(new_root, new_name)
        try:
            # 复制文件
            open(new_path, 'wb').write(open(old_path, 'rb').read())
        except IOError as e:
            print(e）
```
```
