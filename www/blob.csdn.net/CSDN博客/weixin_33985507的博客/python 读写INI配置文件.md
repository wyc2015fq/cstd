# python 读写INI配置文件 - weixin_33985507的博客 - CSDN博客
2016年11月24日 16:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# -*- coding: utf-8 -*-
import ConfigParser
import os
'''读写配置文件的类
[section]
logpath = D:\log\
imageminsize = 200
'''
class ConfigFile:
'''构造函数：初始化'''
    def __init__(self,fileName):
        fileName = unicode(fileName,'utf8')
        self.flag = False
        if os.path.isfile(fileName):
            self.fileName = fileName
            self.cf = ConfigParser.ConfigParser()
            self.cf.read(self.fileName)
            self.flag = True
'''获取节为section,键值为Key的值'''
    def GetValue(self,Section, Key):
        if self.flag:
            try:
                result = self.cf.get(Section, Key)
                return result
            except Exception,e:
                print e
                return ""
        else:
            return ""
    def SetValue(self,Section, Key,Value):
        if self.flag:
            try:
                self.cf.set(Section, Key, Value)
                self.cf.write(open(self.fileName, "w"))
            except Exception,e:
                print e
                return ""
#测试代码
configfile = os.path.join(os.getcwd(),'config.conf')cf = ConfigFile(configfile)
*print cf.GetValue("section","logpath")***cf.SetValue("section","imageminsize","200")**
