# python 文件操作 - Keith - CSDN博客





2016年12月22日 19:58:18[ke1th](https://me.csdn.net/u012436149)阅读数：444
个人分类：[python](https://blog.csdn.net/u012436149/article/category/6462204)









# python 文件操作

## 判断文件/文件夹是否存在

```python
import os
print os.path.exits(file_name) # 存在：返回True，否则，返回False
print os.path.exits(dir_name)  # 存在：返回True，否则，返回False
print os.path.isfile(file_name) #是file返回 True，否则返回False
print os.path.isdir(name)
```

## 创建文件夹

```python
import os
os.makedirs(dir_name)
```

## 列出文件夹下的所有文件夹和文件的名字

```python
import os
os.listdir(root_dir) #注意，返回的名字是无序的
```

## 删除文件（夹）

```python
import os
imoprt shutil
os.remove("file") #删除文件
os.rmdir("dir") #只能删除空目录
shutil.rmtree("dir")  #空目录、有内容的目录都可以删
```

## 复制文件（夹）

```python
shutil.copyfile("oldfile","newfile") #oldfile和newfile都只能是文件
shutil.copy("oldfile","newfile") #oldfile只能是文件夹，newfile可以是文件，也可以是目标目录

shutil.copytree("olddir","newdir")#olddir和newdir都只能是目录，且newdir必须不存在
```

**参考资料**
[http://www.cnblogs.com/hushaojun/p/4533241.html](http://www.cnblogs.com/hushaojun/p/4533241.html)
[http://www.cnblogs.com/nwf5d/archive/2011/03/30/2000245.html](http://www.cnblogs.com/nwf5d/archive/2011/03/30/2000245.html)
[https://www.jb51.net/article/48001.htm](https://www.jb51.net/article/48001.htm)



