# 【python】python的抽象类 - zkq_1986的博客 - CSDN博客





2018年01月02日 09:48:45[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：159







由于python 没有抽象类、接口的概念，所以要实现这种功能得abc.py 这个类库,具体方式如下#-----------------抽象类----------------------import abc
class File_personal(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def read(self):
        passclass Txt(File_personal):
    def read(self):
        print('reading')
    def ls(self):
        print('lalala')

a=Txt()
a.read()
a.ls()


