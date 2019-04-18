# 关于signal(SIGPIPE,SIG_IGN); - nosmatch的专栏 - CSDN博客
2012年02月14日 09:31:24[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：656标签：[signal																[服务器																[tcp](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)
                
当服务器close一个连接时，若client端接着发数据。根据TCP协议的规定，会收到一个RST响应，client再往这个服务器发送数据 时，系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不要再写了。根据信号的默认处理规则SIGPIPE信号的默认执行动作是terminate(终止、退出), 所以client会退出。
若不想客户端退出可以把 SIGPIPE设为SIG_IGN
如:    signal(SIGPIPE,SIG_IGN);
这时SIGPIPE交给了系统处理。
服务器采用了fork的话，要收集垃圾进程，防止僵死进程的产生，可以这样处理：
signal(SIGCHLD,SIG_IGN);　交给系统init去回收。
这里子进程就不会产生僵死进程了
            
