# 【小猪原创】手把手教你玩转SOCKET模型之 重叠I/O 篇 - ljx0305的专栏 - CSDN博客
2008年05月15日 20:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：508标签：[socket																[buffer																[windows																[null																[io																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
建议大家还是到我的blog上看彩版的吧，清晰醒目，我帖在这里只是为了引起大家注意..-_-b   
  在这里看这么多字简直是折磨自己的双眼啊......   
  “身为一个初学者，时常能体味到初学者入门的艰辛，所以总是想抽空作点什么来尽我所能的帮助那些需要帮助的人。我也希望大家能把自己的所学和他人一起分享，不要去鄙视别人索取时的贪婪，因为最应该被鄙视的是不肯付出时的吝啬。”   
                                                                                                                        -----   题记     By   PiggyXP(小猪)   
                                                                前       言     
      其实我首先应该道歉，因为7月份的时候曾信誓旦旦的说要写一套关于SOCKET所有模型的入门文章以及配套代码，不过没想到后天竟然被美女所迷出去度假了，刚刚回来不久。。。。。。-_-b其实那些模型的配套代码我已经基本写完了，只是没写配套文字，不过我想还是先从稍微难一点的模型写起吧，因为其他模型的入门毕竟要简单一些。   
  不过由于也是初学者，疏漏之处还望不吝指正。   
  本文凝聚着笔者心血，如要转载，请指明原作者及出处，谢谢！^_^   
  OK,   Let’s   go   !     Have   fun！!   q^_^p   
  本文配套的示例源码下载地址   
  (VC.net   2003编写的多客户端MFC代码，配有详尽注释，只是简单的显示一下客户端发来的字符，稍加改进就是个聊天室了)：   
  http://www.haha567.com/PiggyXP/OverlappedModel.rar   
  (unix系统，千万注意链接大小写)   
  非常感谢网络版的limin兄弟为我无偿提供的空间，以及在我学习过程中给我的大力帮助与支持，真的非常感谢他，感激涕零啊~~~~~T_T   
  也欢迎大家光临他的Blog一起讨论网络技术   
  http://blog.haha567.com/   
  (本文假设你已经具备用SOCKET简单模型编程的能力，如果对SOCKET一无所知请关注本系列其他文章)   
  目录：   
  1． 重叠模型的优点   
  2． 重叠模型的基本原理   
  3． 关于重叠模型的基础知识   
  4． 重叠模型的实现步骤   
  5． 多客户端情况的注意事项   
  一． 重叠模型的优点   
  1. 可以运行在支持Winsock2的所有Windows平台   ,而不像完成端口只是支持NT系统。   
  2. 比起阻塞、select、WSAAsyncSelect以及WSAEventSelect等模型，重叠I/O(Overlapped   I/O)模型使应用程序能达到更佳的系统性能。   
                    因为它和这4种模型不同的是，使用重叠模型的应用程序通知缓冲区收发系统直接使用数据，也就是说，如果应用程序投递了一个10KB大小的缓冲区来接收数据，且数据已经到达套接字，则该数据将直接被拷贝到投递的缓冲区。   
            而这4种模型种，数据到达并拷贝到单套接字接收缓冲区中，此时应用程序会被告知可以读入的容量。当应用程序调用接收函数之后，数据才从单套接字缓冲区拷贝到应用程序的缓冲区，差别就体现出来了。   
  3. 从《windows网络编程》中提供的试验结果中可以看到，在使用了P4   1.7G   Xero处理器(CPU很强啊)以及768MB的回应服务器中，最大可以处理4万多个SOCKET连接，在处理1万2千个连接的时候CPU占用率才40%   左右   ――   非常好的性能，已经直逼完成端口了^_^   
  二． 重叠模型的基本原理   
              说了这么多的好处，你一定也跃跃欲试了吧，不过我们还是要先提一下重叠模型的基本原理。   
              概括一点说，重叠模型是让应用程序使用重叠数据结构(WSAOVERLAPPED)，一次投递一个或多个Winsock   I/O请求。针对这些提交的请求，在它们完成之后，应用程序会收到通知，于是就可以通过自己另外的代码来处理这些数据了。   
              需要注意的是，有两个方法可以用来管理重叠IO请求的完成情况（就是说接到重叠操作完成的通知）：   
  1. 事件对象通知(event   object   notification)     
  2. 完成例程(completion   routines)   ,注意，这里并不是完成端口   
          而本文只是讲述如何来使用事件通知的的方法实现重叠IO模型，完成例程的方法准备放到下一篇讲   ：）   (内容太多了，一篇写不完啊)   ，如没有特殊说明，本文的重叠模型默认就是指的基于事件通知的重叠模型。   
          既然是基于事件通知，就要求将Windows事件对象与WSAOVERLAPPED结构关联在一起（WSAOVERLAPPED结构中专门有对应的参数），通俗一点讲，就是。。。。对了，忘了说了，既然要使用重叠结构，我们常用的send,   sendto,   recv,   recvfrom也都要被WSASend,   WSASendto,   WSARecv,   WSARecvFrom替换掉了，   它们的用法我后面会讲到，这里只需要注意一点，它们的参数中都有一个Overlapped参数，我们可以假设是把我们的WSARecv这样的操作操作“绑定”到这个重叠结构上，提交一个请求，其他的事情就交给重叠结构去操心，而其中重叠结构又要与Windows的事件对象“绑定”在一起，这样我们调用完WSARecv以后就可以“坐享其成”，等到重叠操作完成以后，自然会有与之对应的事件来通知我们操作完成，然后我们就可以来根据重叠操作的结果取得我们想要德数据了。     
              也许说了半天你还是不大明白，那就继续往后面看吧。。。。。。。-_-b，语言表达能力有限啊~~~   
  三． 关于重叠模型的基础知识   
              下面来介绍并举例说明一下编写重叠模型的程序中将会使用到的几个关键函数。   
  1. WSAOVERLAPPED结构   
  这个结构自然是重叠模型里的核心，它是这么定义的   
  typedef   struct   _WSAOVERLAPPED   {   
      DWORD   Internal;   
      DWORD   InternalHigh;   
      DWORD   Offset;   
      DWORD   OffsetHigh;   
      WSAEVENT   hEvent;             //   唯一需要关注的参数，用来关联WSAEvent对象   
  }   WSAOVERLAPPED,   *LPWSAOVERLAPPED;   
          我们需要把WSARecv等操作投递到一个重叠结构上，而我们又需要一个与重叠结构“绑定”在一起的事件对象来通知我们操作的完成，看到了和hEvent参数，不用我说你们也该知道如何来来把事件对象绑定到重叠结构上吧？大致如下：   
  WSAEVENT   event;                                       //   定义事件   
  WSAOVERLAPPED   AcceptOverlapped   ;   //   定义重叠结构   
  event   =   WSACreateEvent();                   //   建立一个事件对象句柄   
  ZeroMemory(&AcceptOverlapped,   sizeof(WSAOVERLAPPED));   //   初始化重叠结构   
  AcceptOverlapped.hEvent   =   event;         //   Done   !！   
  2. WSARecv系列函数   
  在重叠模型中，接收数据就要靠它了，它的参数也比recv要多，因为要用刀重叠结构嘛，它是这样定义的：   
          int   WSARecv(   
                          SOCKET   s,                                             //   当然是投递这个操作的套接字   
                          LPWSABUF   lpBuffers,                     //   接收缓冲区，与Recv函数不同   
                                                                                //   这里需要一个由WSABUF结构构成的数组   
                        DWORD   dwBufferCount,                 //   数组中WSABUF结构的数量   
                        LPDWORD   lpNumberOfBytesRecvd,     //   如果接收操作立即完成，这里会返回函数调用   
                                                                                    //   所接收到的字节数   
                        LPDWORD   lpFlags,                           //   说来话长了，我们这里设置为0   即可   
                        LPWSAOVERLAPPED   lpOverlapped,     //   “绑定”的重叠结构   
                        LPWSAOVERLAPPED_COMPLETION_ROUTINE   lpCompletionRoutine   
                                                                //   完成例程中将会用到的参数，我们这里设置为   NULL   
  );   
  返回值：   
  WSA_IO_PENDING   ：   最常见的返回值，这是说明我们的WSARecv操作成功了，但是   
                                          I/O操作还没有完成，所以我们就需要绑定一个事件来通知我们操作何时完成   
  举个例子：(变量的定义顺序和上面的说明的顺序是对应的，下同)   
  SOCKET   s;   
  WSABUF   DataBuf;                       //   定义WSABUF结构的缓冲区   
  //   初始化一下DataBuf   
  #define   DATA_BUFSIZE   5096   
  char   buffer[DATA_BUFSIZE];   
  ZeroMemory(buffer,   DATA_BUFSIZE);   
  DataBuf.len   =   DATA_BUFSIZE;   
  DataBuf.buf   =   buffer;   
  DWORD   dwBufferCount   =   1,   dwRecvBytes   =   0,   Flags   =   0;   
  //   建立需要的重叠结构   
  WSAOVERLAPPED   AcceptOverlapped   ;//   如果要处理多个操作，这里当然需要一个   
  //   WSAOVERLAPPED数组   
  WSAEVENT   event;           //   如果要多个事件，这里当然也需要一个WSAEVENT数组   
                                                        //   需要注意的是可能一个SOCKET同时会有一个以上的重叠请求，   
  //     也就会对应一个以上的WSAEVENT   
  Event   =   WSACreateEvent();   
  ZeroMemory(&AcceptOverlapped,   sizeof(WSAOVERLAPPED));   
  AcceptOverlapped.hEvent   =   event;           //   关键的一步，把事件句柄“绑定”到重叠结构上   
  //   作了这么多工作，终于可以使用WSARecv来把我们的请求投递到重叠结构上了，呼。。。。   
  WSARecv(s,   &DataBuf,   dwBufferCount,   &dwRecvBytes,     
  &Flags,   &AcceptOverlapped,   NULL);   
  其他的函数我这里就不一一介绍了，因为我们毕竟还有MSDN这么个好帮手，而且在讲后面的完成例程和完成端口的时候我还会讲到一些   ^_^ 
3. WSAWaitForMultipleEvents函数   
          熟悉WSAEventSelect模型的朋友对这个函数肯定不会陌生，不对，其实大家都不应该陌生，这个函数与线程中常用的WaitForMultipleObjects函数有些地方还是比较像的，因为都是在等待某个事件的触发嘛。   
  因为我们需要事件来通知我们重叠操作的完成，所以自然需要这个等待事件的函数与之配套。   
  DWORD   WSAWaitForMultipleEvents(   
      DWORD   cEvents,                                                 //   等候事件的总数量   
      const   WSAEVENT*   lphEvents,                       //   事件数组的指针   
      BOOL   fWaitAll,                     //   这个要多说两句：   
                                                  //   如果设置为   TRUE，则事件数组中所有事件被传信的时候函数才会返回   
                                                      //   FALSE则任何一个事件被传信函数都要返回   
                                                      //   我们这里肯定是要设置为FALSE的   
      DWORD   dwTimeout,                   //   超时时间，如果超时，函数会返回   WSA_WAIT_TIMEOUT   
                                                        //   如果设置为0，函数会立即返回   
                                                        //   如果设置为   WSA_INFINITE只有在某一个事件被传信后才会返回   
                                                    //   在这里不建议设置为WSA_INFINITE，因为。。。后面再讲吧..-_-b   
      BOOL   fAlertable               //   在完成例程中会用到这个参数，这里我们先设置为FALSE   
  );   
  返回值：   
  WSA_WAIT_TIMEOUT   ：最常见的返回值，我们需要做的就是继续Wait   
  WSA_WAIT_FAILED   ：   出现了错误，请检查cEvents和lphEvents两个参数是否有效   
          如果事件数组中有某一个事件被传信了，函数会返回这个事件的索引值，但是这个索引值需要减去预定义值   WSA_WAIT_EVENT_0才是这个事件在事件数组中的位置。   
  具体的例子就先不在这里举了，后面还会讲到   
  注意：WSAWaitForMultipleEvents函数只能支持由WSA_MAXIMUM_WAIT_EVENTS对象定义的一个最大值，是   64，就是说WSAWaitForMultipleEvents只能等待64个事件，如果想同时等待多于64个事件，就要   创建额外的工作者线程，就不得不去管理一个线程池，这一点就不如下一篇要讲到的完成例程模型了。   
  4. WSAGetOverlappedResult函数   
  既然我们可以通过WSAWaitForMultipleEvents函数来得到重叠操作完成的通知，那么我们自然也需要一个函数来查询一下重叠操作的结果，定义如下   
  BOOL   WSAGetOverlappedResult(   
      SOCKET   s,                                       //   SOCKET，不用说了   
      LPWSAOVERLAPPED   lpOverlapped,     //   这里是我们想要查询结果的那个重叠结构的指针   
      LPDWORD   lpcbTransfer,                 //   本次重叠操作的实际接收(或发送)的字节数   
      BOOL   fWait,                                       //   设置为TRUE，除非重叠操作完成，否则函数不会返回   
                                                              //   设置FALSE，而且操作仍处于挂起状态，那么函数就会返回FALSE   
                                                              //   错误为WSA_IO_INCOMPLETE   
                                                    //   不过因为我们是等待事件传信来通知我们操作完成，所以我们这里设   
                                                //   置成什么都没有作用…..-_-b     别仍鸡蛋啊，我也想说得清楚一些…   
      LPDWORD   lpdwFlags               //   指向DWORD的指针，负责接收结果标志   
  );   
  这个函数没什么难的，这里我们也不需要去关注它的返回值，直接把参数填好调用就可以了，这里就先不举例了   
  唯一需要注意一下的就是如果WSAGetOverlappedResult完成以后，第三个参数返回是   0   ，则说明通信对方已经关闭连接，我们这边的SOCKET,   Event之类的也就可以关闭了。   
  四． 实现重叠模型的步骤   
        作了这么多的准备工作，费了这么多的笔墨，我们终于可以开始着手编码了。其实慢慢的你就会明白，要想透析重叠结构的内部原理也许是要费点功夫，但是只是学会如何来使用它，却是真的不难，唯一需要理清思路的地方就是和大量的客户端交互的情况下，我们得到事件通知以后，如何得知是哪一个重叠操作完成了，继而知道究竟该对哪一个套接字进行处理，应该去哪个缓冲区中的取得数据，everything   will   be   OK^_^。   
  下面我们配合代码，来一步步的讲解如何亲手完成一个重叠模型。   
  【第一步】定义变量…………   
  #define   DATA_BUFSIZE           4096                         //   接收缓冲区大小   
  SOCKET                   ListenSocket,                           //   监听套接字   
  AcceptSocket;                                                     //   与客户端通信的套接字   
  WSAOVERLAPPED     AcceptOverlapped;               //   重叠结构一个   
  WSAEVENT     EventArray[WSA_MAXIMUM_WAIT_EVENTS];       
                                                                                //   用来通知重叠操作完成的事件句柄数组   
  WSABUF           DataBuf[DATA_BUFSIZE]   ;               
  DWORD           dwEventTotal   =   0,                         //   程序中事件的总数   
                            dwRecvBytes   =   0,                         //   接收到的字符长度   
      Flags   =   0;                                         //   WSARecv的参数   
  【第二步】创建一个套接字，开始在指定的端口上监听连接请求   
  和其他的SOCKET初始化全无二致，直接照搬即可，在此也不多费唇舌了，需要注意的是为了一目了然，我去掉了错误处理，平常可不要这样啊，尽管这里出错的几率比较小。   
  WSADATA   wsaData;   
  WSAStartup(MAKEWORD(2,2),&wsaData);   
  ListenSocket   =   socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);     //创建TCP套接字   
  SOCKADDR_IN   ServerAddr;                                                               //分配端口及协议族并绑定   
  ServerAddr.sin_family=AF_INET;                                                                   
  ServerAddr.sin_addr.S_un.S_addr     =htonl(INADDR_ANY);                       
  ServerAddr.sin_port=htons(11111);   
  bind(ListenSocket,(LPSOCKADDR)&ServerAddr,   sizeof(ServerAddr));   //   绑定套接字   
  listen(ListenSocket,   5);                                                                               //开始监听   
  【第三步】接受一个入站的连接请求   
      一个accept就完了，都是一样一样一样一样的啊~~~~~~~~~~   
    至于AcceptEx的使用，在完成端口中我会讲到，这里就先不一次灌输这么多了，不消化啊^_^   
    AcceptSocket   =   accept   (ListenSocket,   NULL,NULL)   ;     
  当然，这里是我偷懒，如果想要获得连入客户端的信息（记得论坛上也常有人问到），accept的后两个参数就不要用NULL，而是这样   
  SOCKADDR_IN   ClientAddr;                                       //   定义一个客户端得地址结构作为参数   
  int   addr_length=sizeof(ClientAddr);   
  AcceptSocket   =   accept(ListenSocket,(SOCKADDR*)&ClientAddr,   &addr_length);   
  //   于是乎，我们就可以轻松得知连入客户端的信息了   
  LPCTSTR   lpIP   =     inet_ntoa(ClientAddr.sin_addr);             //   IP   
  UINT   nPort   =   ClientAddr.sin_port;                                             //   Port   
  【第四步】建立并初始化重叠结构   
  为连入的这个套接字新建立一个WSAOVERLAPPED重叠结构，并且象前面讲到的那样，为这个重叠结构从事件句柄数组里挑出一个空闲的对象句柄“绑定”上去。   
  //   创建一个事件   
  //   dwEventTotal可以暂时先作为Event数组的索引   
  EventArray[dwEventTotal]   =   WSACreateEvent();               
  ZeroMemory(&AcceptOverlapped,   sizeof(WSAOVERLAPPED));                       //   置零   
  AcceptOverlapped.hEvent   =   EventArray[dwEventTotal];                         //   关联事件   
  char   buffer[DATA_BUFSIZE];   
  ZeroMemory(buffer,   DATA_BUFSIZE);   
  DataBuf.len   =   DATA_BUFSIZE;   
  DataBuf.buf   =   buffer;                                                     //   初始化一个WSABUF结构   
  dwEventTotal   ++;                                                               //   总数加一   
  【第五步】以WSAOVERLAPPED结构为参数，在套接字上投递WSARecv请求   
  各个变量都已经初始化OK以后，我们就可以开始Socket操作了，然后让WSAOVERLAPPED结构来替我们管理I/O   请求，我们只用等待事件的触发就OK了。   
  if(WSARecv(AcceptSocket   ,&DataBuf,1,&dwRecvBytes,&Flags,   
  &   AcceptOverlapped,   NULL)   ==   SOCKET_ERROR)   
  {     
        //   返回WSA_IO_PENDING是正常情况，表示IO操作正在进行，不能立即完成   
        //   如果不是WSA_IO_PENDING错误，就大事不好了~~~~~~！！！   
  if(WSAGetLastError()   !=   WSA_IO_PENDING)           
  {   
                                    //   那就只能关闭大吉了   
  closesocket(AcceptSocket);   
  WSACloseEvent(EventArray[dwEventTotal]);   
  }   
  }   
  【第六步】   用WSAWaitForMultipleEvents函数等待重叠操作返回的结果   
      我们前面已经给WSARecv关联的重叠结构赋了一个事件对象句柄，所以我们这里要等待事件对象的触发与之配合，而且需要根据WSAWaitForMultipleEvents函数的返回值来确定究竟事件数组中的哪一个事件被触发了，这个函数的用法及返回值请参考前面的基础知识部分。   
  DWORD   dwIndex;   
  //   等候重叠I/O调用结束   
  //   因为我们把事件和Overlapped绑定在一起，重叠操作完成后我们会接到事件通知   
  dwIndex   =   WSAWaitForMultipleEvents(dwEventTotal,     
  EventArray   ,FALSE   ,WSA_INFINITE,FALSE);   
  //   注意这里返回的Index并非是事件在数组里的Index，而是需要减去WSA_WAIT_EVENT_0   
  dwIndex   =   dwIndex   –   WSA_WAIT_EVENT_0;
【第七步】使用WSAResetEvent函数重设当前这个用完的事件对象   
  事件已经被触发了之后，它对于我们来说已经没有利用价值了，所以要将它重置一下留待下一次使用，很简单，就一步，连返回值都不用考虑   
  WSAResetEvent(EventArray[dwIndex]);   
  【第八步】使用WSAGetOverlappedResult函数取得重叠调用的返回状态   
      这是我们最关心的事情，费了那么大劲投递的这个重叠操作究竟是个什么结果呢？其实对于本模型来说，唯一需要检查一下的就是对方的Socket连接是否已经关闭了   
  DWORD   dwBytesTransferred;   
  WSAGetOverlappedResult(   AcceptSocket,   AcceptOverlapped   ,   
  &dwBytesTransferred,   FALSE,   &Flags);   
  //   先检查通信对方是否已经关闭连接   
  //   如果==0则表示连接已经，则关闭套接字   
  if(dwBytesTransferred   ==   0)   
  {   
  closesocket(AcceptSocket);   
              WSACloseEvent(EventArray[dwIndex]);         //   关闭事件   
  return;   
  }   
  【第九步】“享受”接收到的数据   
  如果程序执行到了这里，那么就说明一切正常，WSABUF结构里面就存有我们WSARecv来的数据了，终于到了尽情享用成果的时候了！喝杯茶，休息一下吧~~~^_^   
  DataBuf.buf就是一个char*字符串指针，听凭你的处理吧，我就不多说了   
  【第十步】同第五步一样，在套接字上继续投递WSARecv请求，重复步骤   6   ~   9   
    这样一路作下来，我们终于可以从客户端接收到数据了，但是回想起来，呀~~~~~，这样岂不是只能收到一次数据，然后程序不就Over了？…….-_-b     所以我们接下来不得不重复一遍第四步和第五步的工作，再次在这个套接字上投递另一个WSARecv请求，并且使整个过程循环起来，are   u   clear？？   
            大家可以参考我的代码，在这里就先不写了，因为各位都一定比我smart，领悟了关键所在以后，稍作思考就可以灵活变通了。   
  五． 多客户端情况的注意事项   
            完成了上面的循环以后，重叠模型就已经基本上搭建好了80%了，为什么不是100%呢？因为仔细一回想起来，呀~~~~~~~，这样岂不是只能连接一个客户端？？是的，如果只处理一个客户端，那重叠模型就半点优势也没有了，我们正是要使用重叠模型来处理多个客户端。   
              所以我们不得不再对结构作一些改动。   
  1．首先，肯定是需要一个SOCKET数组   ,分别用来和每一个SOCKET通信   
      其次，因为重叠模型中每一个SOCKET操作都是要“绑定”一个重叠结构的，所以需要为每一个SOCKET操作搭配一个WSAOVERLAPPED结构，但是这样说并不严格，因为如果每一个SOCKET同时只有一个操作，比如WSARecv，那么一个SOCKET就可以对应一个WSAOVERLAPPED结构，但是如果一个SOCKET上会有WSARecv   和WSASend两个操作，那么一个SOCKET肯定就要对应两个WSAOVERLAPPED结构，所以有多少个SOCKET操作就会有多少个WSAOVERLAPPED结构。   
  然后，同样是为每一个WSAOVERLAPPED结构都要搭配一个WSAEVENT事件，所以说有多少个SOCKET操作就应该有多少个WSAOVERLAPPED结构，有多少个WSAOVERLAPPED结构就应该有多少个WSAEVENT事件，最好把SOCKET   –   WSAOVERLAPPED   –   WSAEVENT三者的关联起来，到了关键时刻才会临危不乱：）   
  2． 不得不分作两个线程：   
  一个用来循环监听端口，接收请求的连接，然后给在这个套接字上配合一个WSAOVERLAPPED结构投递第一个WSARecv请求，然后进入第二个线程中等待操作完成。   
  第二个线程用来不停的对WSAEVENT数组WSAWaitForMultipleEvents，等待任何一个重叠操作的完成，然后根据返回的索引值进行处理，处理完毕以后再继续投递另一个WSARecv请求。   
          这里需要注意一点的是，前面我是把WSAWaitForMultipleEvents函数的参数设置为WSA_   
  INFINITE的，但是在多客户端的时候这样就不OK了，需要设定一个超时时间，如果等待超时了再重新WSAWaitForMultipleEvents，因为WSAWaitForMultipleEvents函数在没有触发的时候是阻塞在那里的，我们可以设想一下，这时如果监听线程忠接入了新的连接，自然也会为这个连接增加一个Event，但是WSAWaitForMultipleEvents还是阻塞在那里就不会处理这个新连接的Event了。也不知道说明白了没有。。。。。。-_-b   可能在这里你也体会不到，真正编码的时候就会明白了。     
  其他还有不明白的地方可以参考我的代码，代码里也有比较详尽的注释,     Enjoy~~~   
  不过可惜是为了照顾大多数人，使用的是MFC的代码，显得代码有些杂乱。   
  六．         已知问题   
          这个已知问题是说我的代码中的已知问题，可不是重叠结构的已知问题：）   
  这个示例代码已经写好了很久了，这两天做最后测试的时候才发现竟然有两个Bug，而且还不是每次都会出现，5555，我最近是实在没有精力去改了，如果有心的朋友能修改掉这两个Bug，那真是造福大家了，这篇文章都险些流产，我更没有经历去修改都快要淡忘了的代码的Bug了，我写在这里提醒一下大家了，反正这个代码也仅仅是抛砖引玉而已，而且我觉得比起代码来还是文字比较珍贵^_^，因为重叠模型的代码网上也还是有不少的。两个Bug是这样的：   
  1． 多个客户端在连续退出的时候，有时会出现异常；   
  2． 有时多个客户端的接收缓冲区竟然会重叠到一起，就是说A客户端发送的数据后面会根有B客户端上次发来的数据。。。。。-_-b   
  改进算法：     其实代码中的算法还有很多可以改进的地方，limin朋友就向我提及过几个非常好的改进算法，比如如何在socket数组中寻找空闲的socket用来通信，但是我并没有加到这份代码里面来，因为本来重叠模型的代码就比较杂，再加上这些东西恐怕反而会给初学者带来困难。但是非常欢迎各位和我讨论重叠模型的改进算法以及我代码中存在问题！^_^   
  就说这么多吧，但愿你能通过这篇文章熟练的玩转重叠IO模型，就没有枉费我这番功夫了。^_^   
  敬请期待本系列下一篇拙作《手把手教你玩转SOCKET模型之完成例程篇》
引用:http://blog.csdn.net/wang_54439463/archive/2008/05/15/2447731.aspx
