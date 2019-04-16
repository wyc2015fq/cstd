# BREW中的调试信息 - 我相信...... - CSDN博客





2011年03月19日 11:45:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1231标签：[brew																[手机																[c																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)




作为原始的调试方式，开发者可以使用帮助函数中的DBGPRINTF()在应用中的任何位置将调试信息打印到模拟器的输出窗口中。模拟器的输出窗口还会显示大量的AEE层运行时产生的信息，这些信息的含义如表16-1所示。

标识

 含义


#*gBI

 BREW 进行初始化


#*gEX

 退出BREW


#*gST=<ClassID>

 发送EVT_APP_START启动ClassID为标识的应用


#*gSU=<ClassID>

 发送EVT_APP_SUSPEND挂起ClassID为标识的应用


#*gRE=<ClassID>

 发送EVT_APP_RESUME恢复ClassID为标识的应用


#*gCL=<ClassID>

 发送EVT_APP_STOP停止ClassID为标识的应用


#*gXX

 关闭所有的BREW应用


#*g**=<ErrorNum>

 错误代码为ErrorNum的BREW异常


#*g*C=<ClassID>:<ErrorNum>

 在创建ClassID应用或接口时产生的错误代码是ErrorNum


#*p:ECode:<Event>,Key:<Code>

 按键事件


表 16-1：模拟器输出窗口调试信息的含义

下面再举一些例子，并解释一下：

#*gST=16809984                               启动了ClassID为16809984的一个BREW应用

#*p:ECode:101, Key:e035                   用户按下了“select”键，相应产生了AVK_SELECT 的EVT_KEY_PRESS事件

#*p:ECode:100, Key:e035       用户按下了“select”键，相应产生了AVK_SELECT 的EVT_KEY事件

#*p:ECode:102, Key:e035                   用户按下了“select”键，相应产生了AVK_SELECT 的EVT_KEY_RELEASE事件

#*gSU=16809984                               手机或模拟器挂起了ClassID16809984的一个BREW应用

#*gCL=16809984                               由于ClassID为16809984的应用没有处理EVT_APP_SUSPEND 事件，手机或模拟器关闭了该应用

#*g*C=101402c:3                   在创建ClassID为0x101402c 的应用或接口时返回了ECLASSNOTSUPPORT 错误

开发者如果能够清楚地知道这些信息的含义，对应用的调试将有很大的帮助。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/11/30/6045553.aspx](http://blog.csdn.net/wireless_com/archive/2010/11/30/6045553.aspx)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




