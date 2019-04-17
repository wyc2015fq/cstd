# python2和python3导入包不同的坑 - westbrook1998的博客 - CSDN博客





2018年11月18日 22:38:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：101








绝对路径导入格式为`import a.b`或者`from a import b`，相对路径导入格式为`from . import b`或者`from ..a import b`。

python2默认为相对路径导入，python3默认为绝对路径导入。

为使python2，3通用，建议采用相对路径导入`from .a import b`



