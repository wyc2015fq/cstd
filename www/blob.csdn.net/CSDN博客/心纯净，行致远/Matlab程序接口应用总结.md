# Matlab程序接口应用总结 - 心纯净，行致远 - CSDN博客





2018年03月23日 15:51:20[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1320








## **matlab 与外部程序的编程接口两大类：**

               一是 如何在matlab里调用其他语言写的代码。 （见例子：使用C-MEX技术，ActiveX技术）

               二是 如何在其他语言里调用matlab。 （见 使用matlab引擎, MAT数据交换, matlab发布com组件, DeployTool）




## **matlab接口技术包含以下几个方面：**

1. 数据导入导出，主要是MAT文件数据的导入导出。

2.普通的动态链接库dll文件的交互，Matlab6.5起，直接matlab环境中调用dll文件导出的函数。

3.matlab环境中调用c/c++语言代码的接口，通过MEX（MATLAB EXecutable）技术实现。C/C++代码通过实现特殊的入口函数，编译成MEX文件（实际dll），可以像一般的m文件被调用，调用级别比M函数高。

4.matlab环境中调用java，6.0版本起，matlab增加java支持，其工作界面GUI也是java编写的，且包含了Java虚拟机。

5.对COM与DDE的支持，5.1后DDE没再增加新内容，6.5后推荐COM接口。matlab的com编译器能将matlab函数转换成COM对象供多种语言使用。

6.matlab使用网络服务或者和串口通讯。




转载自：[https://blog.csdn.net/fonjames/article/details/51554385](https://blog.csdn.net/fonjames/article/details/51554385)




