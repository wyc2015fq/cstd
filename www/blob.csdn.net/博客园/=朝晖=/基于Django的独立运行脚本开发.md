# 基于Django的独立运行脚本开发 - =朝晖= - 博客园
# [基于Django的独立运行脚本开发](https://www.cnblogs.com/dhcn/p/7121437.html)
     1、在Django框架下工作时间长了，会对Django的技术设施产生依赖，比如其方便的ORM，如果写基于Django的独立运行脚本，主要在脚本前面加上以下代码：
- import sys,os,django  
- 
- sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) #把manage.py所在目录添加到系统目录  
- os.environ['DJANGO_SETTINGS_MODULE'] = 'jcsbms.settings' #设置setting文件  
- django.setup()#初始化Django环境  
     2、如果用这种方法写的服务脚本，在数据库关闭等可能导致数据库连接关闭的情况下，得重启这个服务脚本，否则ORM DB Connection会一直在失败状态。当然你也可以用异常处理来重建连接。
参考：http://blog.csdn.net/wang1144/article/details/41362283
- 顶
0

