
# 使用epoll＆socket的总结 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月22日 05:32:06[seven-soft](https://me.csdn.net/softn)阅读数：367


1. epoll 中使用et方式触发，只需EPOLL_CTL_ADD一次，把EPOLLIN  EPOLLOUT 事件全注册，每个socket只需创建add一次，其事件就会
一直在epoll中，当然，socketclose 后应DEL掉。
2. ET触发方式是指当fd到状态发生变化时通知，如read buffer从无到有，write buffer从满到不满才会通知。
3. 若要将accept的文件描述符添加到epoll，那么同样需要将socket设置为nonblock，调用accept时要循环读，直到EWOULDBLOCK发生。
4. socket 无论是read/write事件，均需要循环读，知道EWOULDBLOCK发生。
5. accept 触发too many open file 时，accept到buffer已满，若close掉一些socket，仍然不能接受新连接，参见第2条，所以需要调用EPOLL_CTL_MOD重新触发accept事件,还一个简单方案，监听用到fd注册epoll是使用LT触发。

