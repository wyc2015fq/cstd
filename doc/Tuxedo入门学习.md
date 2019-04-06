# Tuxedo入门学习



## **中间件介绍：**

介于客户机和server之间的夹层，突破了传统的c/s架构，为构建大规模，高性能，分布式c/s应用程序提供了通信，事物，安全，容错等基础服务，屏蔽了底层应用细节，应用程序不必从底层开发，以自身的复杂性换取应用程序开发的简单。

 

## **Tuxedo是什么？**

**Tuxedo**:Transaction for Unix has been Extended for Distributed Operation分布式操作扩展之后的Unix事务系统。

交易中间件位于client和server之间

Tuxedo是一个事务处理(TP)监督器（transaction  processing monitor），它管理联机事务处理(OLTP)系统操作的事务。客户通过结构化查询语言(SQL)调用，或其他类型的请求，产生对server的请求。这个事务处理监督器确信，正确地进行了改动，以保证数据的完整性。这在一个事务能够改变多个位置的数据库的分布式数据库环境是很重要的。这个事务处理监督器使用双阶段提交，以保证全部的数据库都已经接收和认可了这些数据的正确性。否则，这个数据库返回它的事务前状态

 

**WTC**:WebLogic  Tuxedo Connector

**OLTP**: On-Line Transaction Processing 联机事务处理

**OLAP**: On-Line Analytical Processing 联机分析处理

**ATMI**:application-to-Transaction  Monitor Interface 应用程序到事务监视器接口

**DTP**:Distributed  Transaction Processing分布式事务处理

**MSSQ**：Mutile  Server,Singal Queue 

 

## **TUXEDO 採用三层结构的组件软件模型 ：**

²  Client 为第一逻辑层。实现用户交互和数据表示，向第二层的Server发请求，调用业务逻辑处理服务。

²  Server组件中间层，这些组件由TUXEDO管理，实现业务逻辑服务,接收服务请求，并返回服务结果。

²  第三层为资源管理器，比方像关系数据库。负责管理应用系统的数据资源    

 

## Tuxedo的核心子系统：

事务管理器 TM(Transaction Manager)

工作站 WorkStation

域 Domain

队列 Queue

 



 

 

![img](http://hi.csdn.net/attachment/201106/14/0_1308016587ao94.gif)

 

**队列：**

s![img](http://hi.csdn.net/attachment/201106/14/0_1308016679SpZ4.gif)

 

 

## **X/Open DTP 模型：**

 

 ![img](http://hi.csdn.net/attachment/201106/14/0_13080167986266.gif)

 

 

## **Tuxedo 与 WebLogic 通过WTC互联：**

通经常使用TUXEDO实现系统的核心业务，用WEBLOGIC做为系统扩展到web的平台，实现电子商务。由WEBLOGIC 调用 TUXEDO上的服务，须要在Tuxedo和Weblogic之间建立连接。

WTC不仅能让WEBLOGIC调用TUXEDO中的SERVICE，并且能让TUXEDO调用WEBLOGIC中的EJB。但WTC仅能实现这两个平台之间的互联。

 

Tuxedo与WebLogic 之间通过Domain实现互联调用，Tuxedo与Weblogic分别代表两个TDOMAIN。

使用WTC时，Tuxedo方面要配置对应的Domain配置文件(dmconfig)，指明本身以及weblogic所在Domain的IP和Port。

使用WTC时Weblogic方面要做的改动是：

​     —在ClassPath 中，增加jamti.jar所在的路径。

​     —在weblogic的配置文件，bdmconfig.xml 中，增加描写叙述两个TDOMAIN的部分

在Tuxedo和weblogic 启动对应服务后，weblogic通过client端掉用对应ejb，再有该ejb调用tuxedo  service。 

 

 

## **Tuxedo应用开发：**

开发TUXEDO C/S系统的必要步骤：

Ø  环境变量设置，通常写在setenv.sh

TUXDIR：Tuxedo应用的安装路径。

TUXCONFIG:便以后的Tuxedo配置文件路径。

VIEWDIR/VIEWFILES：view文件的路径和文件名称。

LD_LIBRARY_PATH：Tuxedo应用时，须要寻找的库文件的路径。

PATH: Tuxedo特用的一些可运行文件的路径

假设涉及到Domain，还有对应的环境变量。

Ø  编码，并编译client/服务端程序。

Ø  编写/编译Tuxedo配置文件。

Tuxedo配置文件ubbconfig 描写叙述了应用配置信息。Ubbconfig文件是二进制文件，是由文本文件通过tmloadcf 命令编译而成。

Ubb 文件的内容包含例如以下的Section：

​                            Resources:包括整个应用范围的信息。必须在配置在文件第一节，必不可少。

​                   Machines:节包括应用有关的每一个处理器的信息。本节必须在*RESOURCES节后列出。

​                   Groups:节包括服务组的定义。一台机器至少要定义一个服务组,每一个组仅仅要定义组名，映射组名的组号和逻辑机器名

​                   SERVERS：包括了服务进程的信息。一个入口代表一个应用启动时载入的服务。这些信息包括服务名，命令行參数，服务环境，重新启动动等等。

​                   SERVICES：提供了应用的特殊交易的信息，包含负载平衡（LOAD）和数据缓冲类型检查（BUFTYPE）。假设所有都是缺省值则本节能够省略。

​         上述每个Section中，包括众多的Option选项，详细应用时，察看Tuxedo相关文档，进行配置。  

 

l  在执行时，这些配置信息被装入一段共享内存，称为（Bulletin-Board）。

l  TUXEDO提供一个管理进程，称为BBL(Bulletin Board Liaison),包括了一个公告牌的本地拷贝和本地server上应用的状态。 

l  TUXEDO提供的还有一个管理进程DBBL(Distinguished Bulletin Board Liaison)，用于多server配置时。DBBL与BBL协同，保证全部部分的公告牌内容的一致性。

Ø  启动服务。

Ø  測试（功能測试、压力測试）。 

 

## **client/Client开发**

### client的任务：

获取採集运行操作应得的数据。

发起向服务端的请求并等待服务端回应。

将结果依照一定格式返回给用户

client的程序设计和实现应该分成两个部分：

用户处理过程。

Tuxedo功能部分。

 

开发Client涉及的API:

进程管理的API：

**int tpinit(TPINIT \*tpinfo)**

负责将Client端连接到BB，使Client端能够进一步调用ATMI函数。

TPINIT參数是一个Tuxedo定义的结构，用以存放一些安全相关的数据（必须在tuxedo的配置文件里打开security选项）。否则，能够使用NULL。

**tpinit，不能在server端中出现，否则tuxedo会产生TPEPROTO（协议错）这样一个错误。**

 

**int tpterm( )**

client调用tpterm( )切断与应用的连接，结束了client的TUXEDO进程  .

 

编写完毕的Client代码，用buildclient 命令进行编译。

buildclient –f  *filename*  -O *output file* 

假设client端是一个workstation（本地没有Tuxedo  server），还要加上-w 选向。

 

**Clieng与Server 之间的通讯接口：**

Client通过ATMI提供的API，与Server之间进行通讯，调用Server提供的服务。

通讯主要分为两种方式：

​         同步方式：採用同步通讯时，Client端在向Server端发出请求后就被堵塞，等待Server端的返回。

​         同步通讯方式的API：

int tpcall(  char  *svc,   char  *idata,   long   ilen, char   **odata,  long   *olen,   long   flags) 

Svc ： 调用的服务（service）名称

idata： 指向输入数据缓冲区指针。

ilen：   输入数据的缓冲区大小。

odata：指向输出数据缓冲区指针的指针。

olen：  输出数据缓冲区的大小的指针。

flags： 通讯控制标志。 

​         异步方式：採用异步方式时，Client端在发出请求后，能够继续其它的任务，须要结果时，使用API去获取response队列中的结果。

​         异步通信方式的API

​                   — int tpacall(char **svc*, char **data*, long *len*, long *flags*)

​                   — tpacall 调用成功后返回一个整数，称为descriptor，client使用这个整数在以后的某个时间来获取结果。

ATMI提供tpgetrply( ) 来获取异步调用的结果

​                   — int tpgetrply(int **cd*, char ***data*, long **len*, long *flags*)

​                   — 參数*cd* , 就是存放tpacall返回descriptor的指针。

**不管是tpcall、tpacall以及tpgetrply，在client端和server端都能够使用。**

 

### **Server端开发：**

l  Server是系统资源的联系点。

l  Server必须公布系统内能够訪问的交易，保证client能够知道把请求发往何处

l  Server启动后，进入一个循环：接收请求/处理请求/返回节后，周而复始。

l  系统管理员通过命令tmshutdown，显式地关掉tuxedo的服务。

 

### **Server端开发涉及的API：**

l  tpreturn( ): 在Tuxedo中，tpreturn用来取代常规的return函数，运行tpreturn后，server将回应的数据缓冲区返回请求的发起点，交出程序的控制权。

void tpreturn(int *rval*, long *rcode*, char **data*, long *len*, long   *flags*)

*rval* :是返回的结果，一般是TPSUCCESS,  TPFAIL,  TPEXIT。

​    *rcode：是用户自己定义的返回值，用以进一步区分返回结果。*

​    *data   ：是返回结果缓冲区。len:是缓冲区长度。*

*flags :  结果返回的标志，通常都是0。*

 

l  tpsvrinit()和tpsvrdone()分别用来启动和关闭服务。

假设在server的代码中，不提供这两个函数，Tuxedo将使用缺省函数。

tpsvrinit()用tpopen()缺省打开RM连接。

tpsvrdone()用tpclose()关闭RM连接。

tpsvrinit()仅仅在服务boot起来时运行一次。对应的  tpsvrdone()也仅仅在服务shutdown时运行一次。

 

## **设计服务时的几点考虑：**

l  最好不要使用收到的数据缓冲向其它服务请求，由于该缓冲可能被改变引起错误。

l  所有在服务中分配的数据缓冲，在程序结束时必须所有释放；唯一例外是用在tpreturn()中的返回数据缓冲。 

l  服务中的交易不应调用本服务中的交易，由于easy产生死锁。

l  一个MSSQ集中的服务须要返回时，应有自己的返回队列；否则会与本集中其它服务冲突。（RQADDR  = XXXX ,同一时候REPLYQ = Y）。  

 

## **缓冲区数据类型：**

l  Client与Server之间，Server与Server之间，都要通过数据缓冲区来传递数据。

 

l  ![img](http://hi.csdn.net/attachment/201106/14/0_1308016934Rxfx.gif)sTuxedo支持下图所看到的的缓冲区类型：

 

l  Buffer type包括：STRING,CARRY,VIEW,FML等。

STRING：是以空值结尾的单字字符串

CARRY：有长度定义的二进制数据。

VIEW：  类似于C的structure。

FML：  固定结构的自己定义缓冲。

 

## **Tuxedo中的事务处理**

l  不管是Client还是Server都能够主动发起一个全局事务。

l  Tuxedo会对一个transaction产生一个全局交易ID（GTRID)，这个ID号在全部的交易參与这样的共享，并唯一标示这个transaction。

l  Tuxedo通过TLOG，来跟踪一个全局交易。

l  提供通知(notify)RM的方法，使得RM知道自己參加到一个交易中，并lock住对应的记录。

l  Tuxedo作为TM，能够管理两阶段提交（two-phase  commit）。使得全部交易參与者一起提交，保持结果的一致性。

 

TMS和RM之间，使用XA接口来协调工作。

Tuxedo提供tpbegin, tpcommit, tpabort 等API来管理交易

 

## **Tuxedo中的事务处理：**

**![img](http://hi.csdn.net/attachment/201106/14/0_1308017005674a.gif)**

 

**事务管理器：**