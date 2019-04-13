
# python环境变量和import模块导入方法 - jiahaowanhao的博客 - CSDN博客


2018年02月21日 20:21:06[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：566


[python环境变量和import模块导入方法](http://cda.pinggu.org/view/24799.html)
1、定义
模块：本质就是.py结尾的文件（逻辑上组织python代码）模块的本质就是实现一个功能 文件名就是模块名称
包： 一个有__init__.py的文件夹；用来存放模块文件
2、导入模块
import 模块名
form 模块名 import *
from 模块名 import 模块名 as 新名称
3、 导入模块本质
import 模块名 ===》 将模块中所有的数据赋值给模块名，调用时需要模块名.方法名（）
from 模块名 import 方法名 ==》将该方法单独放到当前文件运行一遍，调用时只需要方法名（）即可运行
导入一个包 就是执行包下的__init__.py文件
路径搜索
import sys,os
os.path.abspath(__file__) \#获取当前文件的全名
os.path.dirname() \#获取当前对象的父级目录
sys.path.insert()\#将当前对象的路径添加到首位
sys.path.append() \# 将当前环境变量添加到环境变量的末尾
以上这篇python 环境变量和import模块导入方法(详解)就是小编分享给大家的全部内容了

