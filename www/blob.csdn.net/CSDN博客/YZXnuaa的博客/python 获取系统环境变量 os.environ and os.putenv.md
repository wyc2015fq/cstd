# python 获取系统环境变量 os.environ and os.putenv - YZXnuaa的博客 - CSDN博客
2018年06月13日 15:34:05[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1703
# " if "BATCH_CONFIG_INI" in os.environ:” 判断环境变量的值有没有定义
如果定义的话就去环境变量的值，否则就取当前目录下的config.ini文件。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1         if "BATCH_CONFIG_INI" in os.environ:
2             print "Using custom ini file!"
3             self.inifile = os.environ["BATCH_CONFIG_INI"]
4         else:
5             self.inifile = self.cur_file_dir() + "/config.ini"
6         self.db_print ("inifile = (%s)" %(self.inifile))
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
用Python Shell设置或获取环境变量的方法：
一、设置系统环境变量
1、os.environ['环境变量名称']='环境变量值' #其中key和value均为string类型
2、os.putenv('环境变量名称', '环境变量值')
二、获取系统环境变量
1、os.environ['环境变量名称']
2、os.getenv('环境变量名称')
