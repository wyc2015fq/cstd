# Socket编程模型流程 - 逍遥剑客 - CSDN博客
2007年03月20日 00:09:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2293标签：[socket																[编程																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
面向连接协议的Socket编程模型：
引用内容
服务器                                     客户机 
Socket()
  ↓
bind()
  ↓
listen()
  ↓
accept()                                   socket()
  ↓                                              ↓
阻塞，等待客户数据←建立连接---connect()
  ↓                                              ↓
recv()←----------请求数据-----send()
  ↓                                              ↓
send()-----------应答数据---→recv()
  ↓                                              ↓
close()                                      close()
面向无连接协议的Socket编程模型：
引用内容
服务器                                     客户机 
Socket()
  ↓
bind()                                   socket()
  ↓                                          ↓
recvfrom()                            bind()         
  ↓                                          ↓
阻塞，等待客户数据            ↓
  ↓←-----------建立连接---sendto()
  ↓                                          ↓
recv()←---------请求数据---send()
  ↓                                          ↓
处理服务请求                        ↓
  ↓                                          ↓
sendto()------服务应答-→recvfrom()
  ↓                                          ↓
close()                                 close()
