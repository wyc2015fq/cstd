# （GPS移植三部曲）Linux下移植GPS应用程序之常见问题的分析与解决方法之一 - 2019跟着小虎玩着去软考 - CSDN博客
2012年05月22日 00:00:22[littletigerat](https://me.csdn.net/littletigerat)阅读数：2451标签：[linux																[工具																[测试																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[GPS																[Linux																[手机开发																[C/C++](https://blog.csdn.net/littletigerat/article/category/666612)](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/613914)](https://blog.csdn.net/littletigerat/article/category/658856)
# Linux下移植GPS应用程序之常见问题的分析与解决方法之一
# 问题一：GPS应用程序启动不起来
## 原因：
1．GPS数据的路径不对；
2．动态链接库是否正确，包括大小与版本；
## 解决办法（步骤和方法）：
1．手机中的GPS的库以及数据的文件大小是否与Release目录下的文件大小一致；
2．在navione.ini文件中，请Check GPS数据的存放路径是否正确；
3．上一个Release版本是否正常；
# 问题二：接受不到GPS信号
## 原因：
1．GPS参数与开发板不匹配；
2．GPS天线是否可以接受信号；
## 解决办法（步骤和方法）：
1．确认当前的开发板是Cayman还是FiJI还是Bali
2．确认当前配置文件atgps.ini参数配置是否正确
3．gpstool工具运行是否正常
4．请到GPS天线放到窗口外边或者到室外测试
# 问题三：接受不到GPS信号
首先，确认atgps.ini文件是否可以正常打开
其次，atgps.ini文件中的参数配置是否正确
1.GPIO端口号是否正确
2.GPIOReset重置端口号是否正确
3.波特率
4.GPIO设备的路径
5.串口的路径
6.pumtool电源管理工具的路径
            
