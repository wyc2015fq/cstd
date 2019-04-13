
# 推送技术-icomet学习和分析 - 3-Number - CSDN博客


置顶2015年09月09日 13:11:09[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：1460


## 1 iComet
## 在
## Web
## 系统和移动应用系统中的角色
![](https://img-blog.csdn.net/20150909130602966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 2 iComet
## 的工作流程
![](https://img-blog.csdn.net/20150909130629221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3
## 实现原理
### 3.1
### 关键技术
使用libevent构建一个web服务器，然后定时响应各种请求。libevent库使得高并发响应HTTP Server的编写变得很容易。整个过程包括如下几部：初始化，创建HTTP Server, 指定callback, 进入事件循环。另外在回调函数中，可以获取客户端请求(request的HTTP Header和参数等)，进行响应的处理，再将结果发送给客户端(response的HTTP Header和内容，如html代码)。libevent除了设置generic的callback，还可以对特定的请求路径设置对应callback(回调/处理函数)。
### 3.2
### 关键数据结构
参考：http://www.cnblogs.com/keepsimple/archive/2013/05/06/3063251.html
## 3.3
## 推送设计原理
### 3.3.1
### 事件的设计
针对没一类操作事件，设置对应的事件响应回调函数。
/sub订阅：
Example:http://127.0.0.1:8100/sub?cname=12
响应此事件，新建一个cname的通道和对应的订阅者类。
/push推送：
Example:http://127.0.0.1:8000/push?cname=12&content=hi
响应此事件，将内容发送给cname指定的通道。程序会为每一个订阅者分配一个通道来发送数据。
还有其他功能性事件，先简单分析以上两种，后期实践应用时再进行补充。
### 3.3.2
### 长连接技术的应用
用户一上线，立马根据建立通道，到程序关闭才结束通道，然后保持长连接，保持心跳。推送一次后就会连续的推送，不用再建立长连接，节省了开销。
**如果有类似经验或者有类似需求，愿意一同分享和讨论。**



