
# 这可能是最详细的Python文件操作！ - 喜欢打酱油的老鸟 - CSDN博客


2019年01月12日 19:32:01[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：56


[https://www.toutiao.com/a6642595087190589956/](https://www.toutiao.com/a6642595087190589956/)
2019-01-04 19:01:29
![这可能是最详细的Python文件操作！](http://p3.pstatp.com/large/pgc-image/1546599635941e41244c591)
**删除**
```python
# ==================删除==================
# 只能删除文件，若为目录则报错
# 若文件正在使用，Windows下会直接报错，Linux下会在目录表中删除记录，但文件还在
# 与os.unlink()一样的作用
os.remove(r"C:UsersMicrosoftDesktopI.exe")
# 删除目录，目录必须为空，否则报错OSError
os.rmdir(r"C:UsersMicrosoftDesktopaaa")
# 递归删除空目录
os.removedirs(r"C:UsersMicrosoftDesktop")
# 删除整个目录（不管此目录下有多少目录和文件）
shutil.rmtree(r"C:UsersMicrosoftDesktopaaa")
私信小编“01”即可获取Python入门学习资料！安装教程等等！
# ======================================
```
![这可能是最详细的Python文件操作！](http://p1.pstatp.com/large/pgc-image/b5653b841a1b4c1cb4cb92f0912a5e9e)
**新建**
```python
# ==================新建==================
# 新建文件夹，若倒数第二层文件夹不存在则会报错
os.mkdir(r"/home/python")
# 递归新建目录，若文件夹存在则报错
os.makedirs(r"C:UsersMicrosoftDesktopaaa66?7")
# ======================================
```
**复制**
```python
# ==================复制==================
# 复制文件，不能复制文件夹
shutil.copyfile(r"D:pythonpy1.txt", r"D:pythonpy2.txt")
# 复制文件，与copyfile不同的是，第二个可以参数可以使用文件夹路径
shutil.copy(r"D:pythonpy1.txt", r"D:pythonpy2")
# shutil.copy只复制文件内容，copy2还复制了创建时间、修改时间等信息，
shutil.copy2(r"D:pythonpy1.txt", r"D:pythonpy2")
# 复制整个文件夹，若目标目录已存在，则会报错
shutil.copytree(r"D:pythonpy1", r"D:pythonpy3py1")
# ======================================
```
**移动**
```python
# ==================移动====================
# 移动文件或文件夹，可使用目录树形式
shutil.move(r"D:pythonpy1", r"D:pythonpy3py2")
# ========================================
```
**重命名**
```python
# ==================重命名==================
# 重命名文件或目录，但不能这样：rename("/aaa/a.txt","/bbb/a.txt")，也不能同时重命名目录和文件
os.rename(r"C:UsersMicrosoftDesktopaaa",r"C:UsersMicrosoftDesktopbb")
# 重命名目录或文件
# 若同时重命名路径和文件，则会新建路径并将文件重命名后移动到新目录
# 如：os.renames("/aaa/b.txt","/bbb/c.txt")，则会在aaa同级目录新建bbb目录，并将b.txt重命名为c.txt后移动到bbb目录
os.renames(r"C:UsersMicrosoftDesktopaaa",r"C:UsersMicrosoftDesktopaaaS")
# =======================================
```
**路径相关**
```python
# ==================路径相关==================
# 获取文件的完整路径
os.path.abspath(r"script.py")
# 获取完整路径中的文件名，以下返回script.py，等价于os.path.split(path)[1]
os.path.basename(r"/home/python/script.py")
# 获取文件的绝对路径，参数也必须是全路径，另：即使文件不存在也会正常返回，等价于os.path.split(path)[0]
os.path.dirname(r"/home/python/script.py")
# 获取文件的文件名和扩展名，以下返回：('script', '.py')
os.path.splitext(r"script.py")
# 获取脚本运行目录
os.getcwd()
# 获取目录下文件列表，参数为空则为脚本运行目录，返回格式：['1.py','2.py']
os.listdir(r"/home/python")
# 判断文件或路径是否存在
os.path.exists(r"/home/python")
# 判断目标是文件还是路径
os.path.isdir(r"/home/python")
os.path.isfile(r"/home/python")
# =========================================
```
**遍历目录**
两种方式：
os.walk()
os.listdir() 递归
**os.walk**
os.walk(top[, topdown=True[, onerror=None[, followlinks=False]]])
参数1：要遍历的目录
参数2：默认True为广度遍历，False为深度遍历
参数3：可空，callable 对象，当 walk 异常时，会调用
参数4：可空，如果为 True，则会遍历目录下的快捷方式(linux 下是 symbolic link)实际所指的目录(默认关闭)
```python
import os 
list_dirs = os.walk(rootDir) 
for root, dirs, files in list_dirs:
 # root 所指的是当前正在遍历的这个文件夹的本身的地址
 # dirs 是一个list，内容是该文件夹中所有的目录的名字(不包括子目录)
 # files 同样是list , 内容是该文件夹中所有的文件(不包括子目录)
 print("======")
 for d in dirs:
 # 只输出遍历文件夹名
 print(os.path.join(root, d))
 for f in files:
 # 只输出遍历的文件名
 print(os.path.join(root, f))
```
**os.listdir递归**
os.listdir(path)
参数1：目录
返回path目录下的文件夹名和文件名，并按字母顺序排列
```python
def Recursion(rootDir):
 for lists in os.listdir(rootDir):
 path = os.path.join(rootDir, lists)
 # 输出文件名或文件夹名
 print(path)
 # 判断是否为文件夹，如果是，则递归遍历
 # 如果不是则为文件，可执行具体逻辑
 if os.path.isdir(path):
 Recursion(path)
 else:
 pass
```

