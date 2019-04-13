
# python实现tar文件压缩解压的实例详解 - jiahaowanhao的博客 - CSDN博客


2018年04月06日 10:31:53[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：555


[python实现tar文件压缩解压的实例详解](http://cda.pinggu.org/view/25197.html)
这篇文章主要介绍了python 实现tar文件压缩解压的实例详解的相关资料,这里提供实现方法，帮助大家学习理解这部分内容，需要的朋友可以参考下
python 实现tar文件压缩解压的实例详解
压缩文件：
import tarfile
import os
def tar(fname):
t = tarfile.open(fname + ".tar.gz", "w:gz")
for root, dir, files in os.walk(fname):
print root, dir, files
for file in files:
fullpath = os.path.join(root, file)
t.add(fullpath)
t.close()
if __name__ == "__main__":
tar("del")
解压：
import tarfile
import os
def untar(fname, dirs):
t = tarfile.open(fname)
t.extractall(path = dirs)
if __name__ == "__main__":
untar("del.tar.gz", ".")
dirs参数为保存的目标文件的路径。
以上就是python 实现tar文件压缩解压的实例

