# 学习笔记——node.js - 左直拳的马桶_日用桶 - CSDN博客
2014年03月11日 17:06:17[左直拳](https://me.csdn.net/leftfist)阅读数：1560
node.js的作用在于，号称可以让服务器支持更多的连接。比如说，php + apche可以让服务器支持4000个并发连接，那么node.js + apche可以让服务器支持并发几万个。
为什么这么牛？
一方面，node.js创建的连接，只是发射一些事件，没有生成OS线程，因此不会占用什么资源，也没有资源锁这种问题。
另一方面，node.js内置了内存排队机制，也就是异步机制，因而可以对蜂拥而至的用户连接迅速做出反应。
node.js，就是在服务器端运行的javascript，采用的是谷歌chrome的V8 javascript引擎。javascript真是越来越火了，不是老树发新芽，而是生命之树长青！据说一个转行已久的程序员如果想在2014年重回编程岗位，那么他最应该学习的语言之一，就是javascript。
为什么javascript这么牛？一方面，它身处互联网时代，与互联网结合紧密；另一方面，它语法很灵活，是个四不像语言，更符合推崇自由不羁的程序员的天性。
参考文章：
[http://www.ibm.com/developerworks/cn/opensource/os-nodejs/](http://www.ibm.com/developerworks/cn/opensource/os-nodejs/)
