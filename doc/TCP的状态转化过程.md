# TCP的状态转化过程



## （11中状态）以及TIME_WAIT状态

置顶 2016年07月04日 20:53:10 [L未若](https://me.csdn.net/msdnwolaile) 阅读数：5329

 

TCP中的三次握手，四次挥手是我们所熟知的，可是，我们熟悉里面的各种状态吗？？？

（SYN_SENT,   ESTABLISHED,    CLOSE_WAIT.............）,试问一句，我们了解里面的状态转化吗？？？





1，大家先看一个简单的通信图（图片转载与：UNIX网络编程，page：36，图2-5）

​      ![img](https://img-blog.csdn.net/20160703002557770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​     可以很明显的看到，在通信双方，客户端，服务端的状态变化过程

​     有人可能会说：我们上面不是说，有11中状态吗？？为什么到啦这里变成了只有10中

​     （1,（主动打开：SYN_SENT）        2,ESTABLISHED        3,(主动关闭：FIN_WAIT_1)     4,FIN_WAIT_2

​       5,TIME_WAIT        6,SYN_RCVD     7,CLOSE_WAIT(被动关闭)     8，LAST_ACK    9,CLOSED

​       10,LISTEN)

​       为什么不是11个呢？？？

​       哈哈，其实还有一种状态叫做：CLOSING（这个状态产生的原因比较特殊，后面分析）

​       



​      接下来我们分析一下，这些状态的变化过程，，，

​           主动套接口：用来发起连接                            被动套接口：用来接受连接

​      1，对于服务器端来说:

​           当调用socket函数创建一个套接字时，状态是CLOSED，它被假设为一个主动套接字，也就是说，它是一个

​           将调用connect发起连接的客户套接字。listen函数把一个未连接的套接字转化成一个被动套接字，指示内核

​           应接受指向该套接字的连接请求。结合TCP的状态转化图：

​           调用listen函数导致套接字从：CLOSED状态转化为：LISTEN状态

​       2，对于客户端来说：

​             调用socket函数创建一个套接口时，状态也是CLOSED，同样的，它也被假设为一个主动套接字，紧接着，调

​             用connect主动打开套接口，并且一直阻塞着，等待三次握手的完成，我们把这个状态称之为：主动套接口。

​             当客户端发起了三次握手的第一次（SYN   J，MSS = 536）的时候，套接口的状态变成了：

​             SYN_SENT(主动打开)

​       3，对于服务器端而言，调用了listen之后，然后状态就变成了LISTEN状态，接着调用accept函数，使自身一直

​            保持阻塞的状态，直到三次握手的第一次来到（来自TCP协议栈的TCP的第一个分节），即接收到（SYN  J，

​            MSS = 536），此刻状态由：LISTEN转变为SYN_RCVD

​       4，对于客户端来说，刚才发送了TCP协议栈中TCP三次握手的第一个分节，此刻应该接受来自服务器发送过来的

​             TCP三次握手的第二个分节，这时服务器发送过来：（SYN K， ACK J+1， MSS = 1460），此刻，服务器

​             的状态不变，还是SYN_RCVD,然后，客户端接受服务器发送过来的TCP三次握手的第二次分节，此刻状态

​             由之前的：SYN_SENT转变为ESTABLISHED，（客户端已经建立完成），这时，connect函数返回

​       5，然后客户端保持ESTABLISHED状态，并且发出TCP协议栈中TCP三次握手的第三个分节（ACK  K+1）

​             服务端的状态由：SYN_RCVD转变为：ESTABLISHED，从未完成的队列中取出队首的第一个连接放在已完成

​             队列，这样accept函数就会返回。

​             此刻，两者都建立完成，这个时候可以完成通信了

​       6，那么接下来就是连接终止的四次握手，，，

​             当双方都变成ESTABLISHED状态之后，双方就可以通信了，在双方通信的过程中，由于状态都没有变化，

​             所以这里，我们暂且不讨论。在通信的时候呢，双方都可以主动发起关闭，那么：我们假定客户端发起一个

​             关闭请求（调用close函数）：会向服务端发送一个TCP分节（TCP协议栈中四次握手的的第一个分节：

​             FIN M），然后客户端的状态会变成：FIN_WAIT_1(主动关闭)，此刻，服务端接收到这个TCP分节后，

​            并且会对刚才发过来的连接进行确认（ACK M+1），，服务端的状态会变成 CLOSE_WAIT(被动关

​             闭)，当，客户端接收到这个确认之后（ACK M+1），客户端的状态转变 

​             为：FIN_WAIT_2   , 只有当服务端的read函数返回为0的时候，服务端才需要，也是才可以发起关闭请求（FIN 

​             N），发送完成之后，就变成了：

​             LAST_ACK,       当客户端接受到了这个关闭请求之后，状态会变成了：TIME_WAIT（会经过

​             2MSL（TCP报文端最大生存周期的两倍时间）之后，转变为：CLOSED），紧接着客户端会发送

​             最后一次确认：（ACK N+1）,等到服务端接收到这个确认后，服务端的状态会变成：CLOSED

​       



​             关于CLOSING：

​             该状态产生的原因是：对于客户端和服务端而言，两者同时关闭的情况（这种情况并不多见），如下图：

​             ![img](https://img-blog.csdn.net/20160704115943033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

、              两者同时关闭，后状态同时变成了FIN_WAIT_1,然后当另外一端接收到关闭分节后，状态同时变成CLOSING，然后都对刚才那个分节进行确认，当对端收到之后，两者又都变成了TIME_WAIT,

​                  所以说：在关闭的过程中，不一定可以必须要经过FIN_WAIT_2这个状态。。。。。。。。。。。。



​                  





关于TIME_WAIT:

​            1，我们可以从上面的状态分析中得知，对于TIME_WAIT状态而言，是执行主动关闭的那端经历了这个状态。

​                   该端点停留在这个状态的持续时间是最长分节生命期（MAXIMUM  SEGMENT  LIFETIME， msl）的两

​                    倍，有时候称之为：2MSL

​                

​                    任何TCP实现都必须为MSL选择一个值，RFC1122的建议值是2分钟，而源自Berkeley的实现传统上改用

​                     30秒这个值，又因为：信息的传送是需要一个来回，着也就说明，TIME_WAIT状态的持续时间是1分钟

​                     到4分钟之间。而MSL是任何IP数据报能够在因特网中存活的最长时间。我们也知道这个时间是有限的，

​                     因为每个数据报含有一个跳限（hop limit）的8位字段，它的最大值是255。尽管这是一个跳数限制而不是

​                     真正的时间限制，我们仍然假设：

​                     具有最大跳限（255）的分组在网络中存在的时间不可能超过MSL秒。。。。。

​                    

​                     分组在网络中“迷途”通常是路由异路的结果。某个路由器崩溃或某两个路由器之间的某个链路断开时，路由

​                     协议需要花数秒钟到数分钟的时间才能稳定并找出另一条通路。在这段时间内可能发生路由循环（

​                     路由器A把分组发送给路由器B，而B再把它们发送给A），我们关心的分组可能就此陷入这样的循环。

​                     假设迷途的分组是一个TCP分节，在它迷途期间，发送端TCP超时重传该分组，而重传的分组却通过某条

​                     候选路径到达最终目的。然而不久后（自迷途的分组开始其旅程起最多MSL秒以内）路由循环修复，早先

​                     迷失在这个循环中的分组最终也被送到目的地。TCP必须正确处理这些重复的分组。



​                     TIME_WAIT状态存在的两个理由：

​                     1，可靠的实现TCP全双工连接的终止（更好的完善TCP的可靠性）

​                     2，允许老的重复分节在网络中消逝



​                     关于第一点：假设最终的ACK丢失了来解释（并不能保证传输的可靠行）。服务器将重新发送它的最终的

​                     那个FIN， 因此客户必须维护状态信息，以允许它重新发送那个ACK。要是客户不维护状态信息，它将

​                     响应以一个RST（另外一种类型的TCP分节），该分节将被服务器解释成一个错误。如果TCP打算执行所

​                     有必要的工作以彻底终止某个连接上两个方向的数据流（即全双工关闭），那么它必须正确处理连接终止

​                     序列4个分节中任何一个分节丢失的情况。本例子也说明了为什么执行主动关闭的那一端是处于

​                     TIME_WAIT的那一端;因为可能不得不重传最终的那个ACK的就是那一端。



​                     关于第二点：我们假设在12.106.32.254的1500端口和206.168.112.219的21端口之间有一个TCP连接。我

​                     们关闭这个连接，过一段时间后在相同的IP地址和端口之间建立另一个连接。后一个连接称为前一个连接

​                     的化身，因为他们的IP地址和端口号相同。TCP必须防止来自某个连接的老的重复分组在该连接已终止后

​                    再现，从而被误解成属于同一个连接的某个新的化身。为做到这一点，TCP将不给处于TIME_WAIT状态

​                    的连接发起新的化身。既然TIME_WAIT状态的持续时间是MSL的2倍，这就足矣让某个方向上的分组最多

​                    存活MSL秒即被丢弃，另一个方向上的应答最多存活MSL秒也被丢弃。通过实施这个规则，我们就能保证

​                    每成功建立一个TCP连接时，来自该连接先前化身的老的重复分组都已在网络中消逝了。。。。

​                     

​             

​                    大家可以过来看看！！！

​                    当我们仅仅打开服务端之后（端口号为5188），我们来看看所处的状态。

​                    打开服务端：

​                    ![img](https://img-blog.csdn.net/20160704121229690?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                    调用命令查看所有的网络状态：netstat

​                     ![img](https://img-blog.csdn.net/20160704121453788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                     然后，我们通过命令：摘取有关tcp的状态：netstat  -an |grep tcp

​                     ![img](https://img-blog.csdn.net/20160704121615304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                     紧接着为了删减出有效的信息，我们只需要tcp协议，5188这个端口，我们可以这样做：

​                    

```cpp
 netstat -an|grep tcp|grep 5188
```

 





​                     嗯嗯，此刻，可以看到，我们这里的状态是处于LISTEN，调用的accept函数还是在阻塞着，等待着返回。





​                     这时，我们再次打开客户端，继续观察一下状态：

​                    ![img](https://img-blog.csdn.net/20160704134354482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​                     然后，我们继续调用之前的命令：



```cpp
netstat -an|grep tcp|grep 5188
```

 





​                     当客户端一打开，那么就完成了TCP的建立，这里，我们可以看到有两个是：ESTABLISHED

​                     其中第二行的42555表示的是客户端所打开的端口，5188是服务端所打开的端口，客户端连向了服务器端



​                     由于我们上面的测试是在同一台主机上的，所以会出现上面的三种信息



​                     而对于其他的状态而言，只是因为状态的转化时间非常短（三次握手，四次挥手完成的特别快），我们不

​                     去探究具体的状态，，，





  1，查找服务器进程：

​       

```cpp
ps  -ef | grep echoserv
```





分析其pid号，知道了我们此刻打开的是中间的这个服务端（21858，21849）

所以，此刻，我们杀死这个进程：



```cpp
kill -9  21858
```







到啦这里，我们再次查看一下状态：

![img](https://img-blog.csdn.net/20160704142957206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



至于为什么会产生一个FIN_WAIT2,   而不是TIME_WAIT状态呢，，，，这是因为：我们程序中是这样处理的，我们

的服务端关闭之后，然后客户端接收到啦这个分节，并向服务端发送了当前的分节确认，然后自己阻塞在了从键盘获

取字符的这个位置，并不能运行到函数read处去，也就是说，

read函数压根就不会返回0，所以客户端就不会重新向服务端重新发送关闭连接的分节，也就停留在此刻了，同样的，

服务端接受到啦确认分节，那么自己的状态就变成了FIN_WAIT_2,这样就解释的通了，哈哈哈

以下是：我们的客户端处理程序：



```cpp
void echo_cli(int sock)



{



                                                                                                                                                



        char sendbuf[1024] = {0};



        char recvbuf[1024] = {0};



 



        while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)



        {



                writen(sock, sendbuf, strlen(sendbuf));



 



                int ret = readline(sock, recvbuf, sizeof(recvbuf));



                



                if(ret == -1)



                        ERR_EXIT("READline");



                else if(ret == 0)



                {



                        printf("client close \n");      



                        break;



                }



                



                fputs(recvbuf, stdout);      //fgets接受到的数据，默认说明是存在换行符的



                memset(sendbuf, 0 , sizeof(sendbuf));



                memset(recvbuf, 0 , sizeof(recvbuf));



        }



 



        close(sock);



}
```



此刻，如果我们再重新输入字符，然后就会执行到read函数处，由于对方已经关闭，对端会接收到（四次挥手）的

第一个分节（FIN），然后read返回0，从上面函数可以看出，程序执行break，然后继续执行close（sock）





而对于客户端先关闭的情况，，，则是这个样子的，，，

同理，先打开服务端，再打开客户端，，，

![img](https://img-blog.csdn.net/20160704165101626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

进去之后，直接按：CTRL + C，使客户端退出，我们查看一下状态：

![img](https://img-blog.csdn.net/20160704165231877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



可以知道，出现了TIME_WAIT状态，，，

同样的，这里，我们也需要查看一下echoserv具体的实现：



```cpp
void echo_serv(int conn)



{



        char recvbuf[1024];



        while(1)



        {



                memset(recvbuf, 0, sizeof(recvbuf));



                int ret = readline(conn, recvbuf, 1024);



                if(ret == -1)



                        ERR_EXIT("READLine");



                



                if(ret == 0)



                {



                        printf("client close\n");       



                        break;



                }



                        



                fputs(recvbuf, stdout);



                writen(conn, recvbuf, strlen(recvbuf));  



        }



}
```

出现这个状态也是比较简单，因为：客户端结束了之后，服务端开始执行readline（里面封装了read），read 返回为0



不会阻塞，紧接着就执行close，会继续发送一个fin分节，，所以会出现后面的TIME_WAIT状态啦，，，



我们的服务器端会处于TIME_WAIT状态，这时如果我们继续打开服务器会出现：地址占用，

# [bind：address already in use](http://blog.csdn.net/msdnwolaile/article/details/50743254)

如果，我们不使用REUSEADDR的话，如果我们使用这个REUSEADDR，并且设置选项的话，setsockopt的话，那么



我们可以随时打开服务器，不用等待2MSL个时间











关于RST分节，

1，对于RST分节，其实是这个样子的，我们打开服务端，客户端，然后关闭服务端（会向客户端发送一个FIN 分节）

​      ，但是这个时候，我们的客户端是阻塞在fgets函数的，我们从键盘给一个字符串，让其满足fgets函数，执行到write

​      函数，将刚才的字符串输出给服务端，由于刚才的服务端已经终止了并且发送了一个FIN，说明不能在发送

​      新的段，并且也不能接受对端的数据，由于此时服务端已经终止，所以上面客户端发送给服务端的信息，也就找不

​      到归宿这个时候（对方进程不存在了），TCP协议栈就会发送一个RST的tcp分节过去。如果这个时候，我们在调用

​      write（） 函数去读取的话，那么就会产生SIGPIPE，

​       程序如下：

​       

```cpp
 while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)



        {



 



        //      writen(sock, sendbuf, strlen(sendbuf));



                write(sock , sendbuf, 1);      //分两次发送，先发送1个，然后在发送剩余的



                write(sock , sendbuf + 1, strlen(sendbuf) - 1);



 



                int ret = readline(sock, recvbuf, sizeof(recvbuf));



                



                if(ret == -1)



                        ERR_EXIT("READline");



                else if(ret == 0)



                {



                        printf("client close \n");      



                        break;



                }



                



                fputs(recvbuf, stdout);      //fgets接受到的数据，默认说明是存在换行符的



                memset(sendbuf, 0 , sizeof(sendbuf));



                memset(recvbuf, 0 , sizeof(recvbuf));



        }
```

可以看到，上面我们调用了两次的write函数，第一次write函数（发送字符的时候），对面的进程已经不在了，TCP



协议栈会发送一个RST分节，紧接着我们再次调用了write函数，此刻就产生了一个SIGPIPE的信号中断，直接终止当

前进程，倘使不退出程序的话，那么read会返回0（readline中封装着read），所以ret等于0，应该会打印client  close

，但是我们的程序并没有打印。。。。。![img](https://img-blog.csdn.net/20160704200005994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



（打开相应的客户端，服务端）



观察状态：

![img](https://img-blog.csdn.net/20160704200117818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

服务端关闭：

![img](https://img-blog.csdn.net/20160704200220902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



观察状态：

![img](https://img-blog.csdn.net/20160704200254584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



给客户端一个字符串，满足fgets函数

![img](https://img-blog.csdn.net/20160704200346741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

程序直接退出了，所以看得出来，并没有打印client  close



所以说，我们上面的分析是合理的。。。。。。



接下来我们修改一下程序：

```cpp
<span style="color:#000000;">void handle_sigpipe(int sig)



{



        printf("recv is a sig = %d\n", sig);    



}



 



 



int main()



{                                                                                                                                               



 



        signal(SIGPIPE, handle_sigpipe);



        int sock;



        if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)



                ERR_EXIT("socket");</span>
```

同样的道理，我们来运行一下程序：



![img](https://img-blog.csdn.net/20160704203457723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里还能输出：client  close，为什么呢？？？这是因为产生了sigpipe中断信号后，我们对中断信号进行了处理了，所以不会退出程序了

同样的，我们来查看一下这个：sig = 13

![img](https://img-blog.csdn.net/20160704203559808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

可以看到，这里的正是sigpipe信号





上面看啦这么多，我们貌似好像看到了用kill杀死一个进程和CTRL + C，我们来看看区别！！！

同理，打开客户端，服务端

![img](https://img-blog.csdn.net/20160704202659231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

查看状态：

![img](https://img-blog.csdn.net/20160704202758627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

调用CTRL + C，关闭服务器

![img](https://img-blog.csdn.net/20160704202832796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接着我们继续查看状态
![img](https://img-blog.csdn.net/20160704202932424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果我们：调用kill杀死相应的服务端进程的话!!!![img](https://img-blog.csdn.net/20160704203129519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

紧接着，我们再来看看状态：

![img](https://img-blog.csdn.net/20160704203208958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





CTRL+C：发送SIGINT信号给前台进程组中的所有进程。常用于终止正在运行的程序，强制中断程序的执行

CTRL+Z：发送SIGTSTP信号给前台进程组中的所有进程，常用于挂起一个进程，是将任务中断，但是此任务并没有结束，它仍然在进程中他只是维持挂起的状态，用户可以使用fg/bg操作继续前台或后台的任务，fg命令重新启动前台被中断的任务，bg命令把被中断的任务放在后台执行



可知，如果我们调用kill的话，那么我们还能观察到对等的状态，如果我们调用CTRL + C的话，那么我们的整个服务端

程序都被中断





总之：上面说了这么多的原因，就是说，一端A调用close退出的话，会发送FIN分节给

对端B，但是对于B接收到A的分节之后，并不能保证A端的进程是不是已经消失，，，

因为对方调用close，并不意味着对方的进程会消失，，，当然，上面我们是通过kill或

者CTRL + C来确保的，如果这时B端再调用write，发现A端不存在，那么TCP协议栈会

发送一个RST分节（连接重置的TCP端），对于当前的全双工管道而言，如果再次调

用write函数的话，那么就会

产生SIGPIPE信号中断。。。。。。。。。。