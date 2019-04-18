# 非阻塞socket编程问题小结 - tifentan的专栏 - CSDN博客

2014年11月26日 10:17:48[露蛇](https://me.csdn.net/tifentan)阅读数：527
个人分类：[网络](https://blog.csdn.net/tifentan/article/category/2739913)



项目需要写一个主动连接且定时发送数据的客户端程序，并保证传输数据的可靠性和稳定性。

注意的问题有：

1.connect返回值判定

之前的程序

if(connect(tcp_client_sock,(structsockaddr*)&server, server_length) <0)

//向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接

     {

       printf("Can Not Connect To %s!\n",SERVE_IP);

       close(tcp_client_sock);

    return-1;

       //exit(1);

     }

     else

     {

     TCP_CONNET_FLAG = 1;//建立连接

...

}

但是在网上查询发现：当我们以非阻塞的方式来进行连接的时候，返回的结果如果是-1,这并不代表这次连接发生了错误，如果它的返回结果是EINPROGRESS，那么就代表连接还在进行中。后面可以通过poll或者select来判断socket是否可写，如果可以写，说明连接完成了。

更改如下：

//先定位为非阻塞模式,立即返回状态；如有错误存为SO_ERROR值     

if((flags =fcntl(tcp_client_sock,F_GETFL, 0 )) < 0)

        {

       perror("fcntl");

       return -1;

        }

        flags |= O_NONBLOCK;

        if(fcntl(tcp_client_sock, F_SETFL, flags) < 0)//设置socket为非阻塞模式

       {

         perror("fcntl");

         return -1;

       }

 if(connect(tcp_client_sock,(structsockaddr*)&server, server_length) <0) //向服务器发起连接

       {   

        if(errno != EINPROGRESS)//非等待状态

           {

             perror("connect error");

          close(tcp_client_sock);//下一步重连

         //return -1;

           }

           else//EINPROGRESS:正常处理连接

           {

           perror("connect");//查询ERROR值

              printf("check delay connected\n");

              goto done;//进一步检查是否握手完成

           }

       }

else

       {

    done:

        ...//下一步的select判定connect是否完成及tcp_client_sock可写性

    }

这里,perror("connect")语句查询ERROR值为“Operation now inprogress”，表明非阻塞connect立即返回的状态为正在建立三次握手；如果不想出现这种情况，可以将以上关于非阻塞设置socket的语句放到connect之后，就会在阻塞方式下等待connect完成，但仍需要作进一步检查。对于非阻塞方式，下一步就可以通过select自定义超时时间(通常比阻塞方式下connect超时时间短)，并进一步检查是否连接错误和规定时间内套接口可读写性。

2.select超时设置问题

如果设置connect为非阻塞函数后，进行select时只关注writefds，忽略readfds,exceptfds，可能出现一个问题：本来不想由于connect阻塞等太久，结果用select后反而傻等了。

如果在connect后开始select，只关注writefds，设置的超时是10秒，在connect发出[SYN]后：假定目标IP的主机不存在或者是目标端口给防火墙过滤了，那么你等再久也不会有任何回复，这时候如果是阻塞connect可能要15秒才返回，那么你10秒就返回了，这种情况就赚了5秒。

然而假定connect的目标IP主机是存在的也没防火墙，只是端口是没打开的，在connect发出[SYN]后的1秒系统已经收到目标主机回复的[RST,ACK]，也就是说系统此时已经知道这个端口是连接不上的了，但是应用程序只关注writefds，后面的9秒钟select就会傻傻的等待下去……原本以为用select来减少不必要的等待时间，如果不设置参数exceptfds，这时候反而浪费时间。 

3.send/recv 返回值

由于send、recv函数用于已连接的数据报或流式套接口s进行数据的接收。所以在非阻塞socket的客户端程序中recv、send函数成功返回并不代表对端一定收到了发送的消息。tcp协议本身是可靠的,并不等于应用程序用tcp发送数据就一定是可靠的。不管是否阻塞,send发送的大小,并不代表对端recv到多少的数据.

关于recv返回值，百度的解释：

     1).若无错误发生，recv()返回读入的字节数。

     2).如果连接已中止，返回0。

     3).否则的话，返回SOCKET_ERROR错误。

　 如果套接口为SOCK_STREAM类型，并且远端“优雅”地中止了连接，那么recv()一个数据也不读取，立即返回。如果立即被强制中止，那么recv()将以WSAECONNRESET错误失败返回。

   接收数据时perror时常遇到"Resource temporarilyunavailable"的提示，errno代码为11(EAGAIN)。这表明你在非阻塞模式下调用了阻塞操作，在该操作没有完成就返回这个错误，这个错误不会破坏socket的同步，不用管它，下次循环接着recv就可以。对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。其实这算不上错误，只是一种异常而已。

　　另外，如果出现EINTR即errno为4，错误描述Interrupted systemcall，即由于信号中断导致操作失败，也应该继续。

      val= recv(client_sock,info,length,MSG_NOSIGNAL);

      if(val< 0)//判断是否网络无数据或接收缓冲区是否准备好

      { 

            if(val==EAGAIN||EWOULDBLOCK||EINTR)

            {

                 printf("Recvdata Timeout.Waiting...\n");

                 return1;

             }

             else//网络异常断开或阻塞；需重新连接

             {

                 perror("recv");

                 return-1;

             }

      }

      elseif(val == 0)//server端正常关闭，需重新连接

      {    

        TCP_CONNET_FLAG=0;//关闭TCP连接flag

         printf("socketclose nomally\n");

         return0;

       }

      else//返回接收字节大于零，正常接收数据

       { 

           ...//对接收缓冲区info的每个字节作读写操作处理

          return 1；

       }

关于send函数在阻塞模式和非阻塞模式下的区别：

      在阻塞模式下,send函数的过程是将应用程序请求发送的数据拷贝到发送缓存中发送并得到确认后再返回.但由于发送缓存的存在,表现为:如果发送缓存大小比请求发送的大小要大,那么send函数立即返回,同时向网络中发送数据;否则,send向网络发送缓存中不能容纳的那部分数据,并等待对端确认后再返回(接收端只要将数据收到接收缓存中,就会确认,并不一定要等待应用程序调用recv);

      在非阻塞模式下,send函数的过程仅仅是将数据拷贝到协议栈的缓存区而已,如果缓存区可用空间不够,则尽能力的拷贝,返回成功拷贝的大小;如缓存区可用空间为0,则返回-1,同时设置errno为EAGAIN.

当客户通过Socket提供的send函数发送大的数据包时，就可能返回一个EGGAIN的错误。该错误产生的原因是由于send函数中的size变量大小超过了tcp_sendspace的值。tcp_sendspace定义了应用在调用send之前能够在kernel中缓存的数据量。当应用程序在socket中设置了O_NDELAY或者O_NONBLOCK属性后，如果发送缓存被占满，send就会返回EAGAIN的错误。

为了消除该错误，有三种方法可以选择：
1).调大tcp_sendspace，使之大于send中的size参数
---no -p -otcp_sendspace=65536
2).在调用send前，在setsockopt函数中为SNDBUF设置更大的值

  intopt=SO_REUSEADDR;

     setsockopt(tcp_client_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

          opt = 256*1024;//512k

          int optlen = sizeof(int);

          setsockopt(tcp_client_sock,SOL_SOCKET,SO_SNDBUF,&opt,sizeof(int));

          getsockopt(tcp_client_sock,SOL_SOCKET,SO_SNDBUF,&opt,&optlen); 
3).使用write替代send，因为write没有设置O_NDELAY或者O_NONBLOCK

 在CSDN中看到有这样一种情况：

假如发送端流量大于接收端的流量(意思是epoll所在的程序读比转发的socket要快),由于是非阻塞的socket,那么send()函数虽然返回,但实际缓冲区的数据并未真正发给接收端,这样不断的读和发，当缓冲区满后会产生EAGAIN错误,同时,不理会这次请求发送的数据.

所以,需要根据send()函数返回值及errno值作进一步处理。遇到该情况，函数要求尽量将数据写完再返回，或通过更改发送缓冲区大小。当写缓冲已满(send()返回-1,且errno为EAGAIN),那么会等待后再重试send().这种方式并不很完美,在理论上可能会长时间的阻塞在socket的send()中,但暂没有更好的办法.

        SendFlag&=TCP_CONNET_FLAG;//测试tcp_client_sock描述符可写且TCP连接存在

           if(SendFlag==1)//尝试发送

           {

              res=send_client_info(tcp_client_sock,buf,45);            

               if(res<0)

               {

                 if(errno == EINTR)//当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,返回后判断网络状态再重试.

                   return -1;

                 if(errno == EAGAIN)//发送缓冲区剩余0字节,延时等待发送;

                 {

                      usleep(10000);

                      res=send_client_info(tcp_client_sock,buf,45);

                 }

               }

          }

          else

