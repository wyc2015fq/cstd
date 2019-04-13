
# 远程调用技术代码追踪(socket) - .NET博文收藏 - CSDN博客


2008年06月10日 12:52:00[hejishan](https://me.csdn.net/hejishan)阅读数：1927


远程调用技术内幕
聊聊远程调用的相应技术。
微软的模型是rpc, DCOM的封包就是OleVariant型的。经常听到有人说OleVariant的效率不高，其实MIDAS就是采用这种封包的。
有人问方法的调用，秘密都在IAppServer这个接口里。 客户端要想调用服务器端的方法是需要代理DLL，和存根DLL的。具体的可以参考COM原理。进程内的就不说了，因为同一个地址空间，它可以通过指针去访问的。主要说说进程外的实现，这也是我们比较感兴趣的。为什么需要代理DLL和存根DLL，这是由于，不同物理空间的两台机器。对它们来讲，地址是没有用的。
从A机到B机通过COM传一个字符串，整型，都比较容易。那么对象怎么办呢。 大家知道，传递对象，其实就是传递指针而已。即然地址对两台机器而言，已经失去了意义。那么需要有一种机制把A机的数据包，发到B机，B机能解释，返回一模一样的内存空间出来。
比如： A要调B机器类的一个方法，怎么做？A机器要模访B机器的内存空间。
A就傻BB的把参数压进去。就调用了，对A机而言。是完全透明的。 在DELPHI中，秘密就在tlb中, import type library这一项 , 例：TRDMCardServer = class(TOleServer)
通过这种方式，不仅可以访问DELPHI写的中间层，而且可以访问其实的中间层。 在SAP应用中，以前同事，就用这种办法引出代理DLL，来访问SAP里面的业务逻辑对象。这个TOleServer对象是干嘛的？这是大家比较关心的问题。其实A机和B机双方使用了COM工厂进行沟通的。
class function CoRDMCardServer.CreateRemote(const MachineName: string): IRDMCardServer;
begin
Result := CreateRemoteComObject(MachineName, CLASS_RDMCardServer) as IRDMCardServer;
end;
（注册表中）COM组件的localserver32下面正是COM组件的位置。研究一下CreateRemoteComObject这个函数。
CoCreateInstanceEx看到这里，有人可能已经理解了。这个函数是用COM库中的远程调用的API。
if Ole32 <> 0 then
begin
@CoCreateInstanceEx := Windows.GetProcAddress(Ole32, 'CoCreateInstanceEx');
@CoInitializeEx := Windows.GetProcAddress(Ole32, 'CoInitializeEx');
@CoAddRefServerProcess := Windows.GetProcAddress(Ole32, 'CoAddRefServerProcess');
@CoReleaseServerProcess := Windows.GetProcAddress(Ole32, 'CoReleaseServerProcess');
@CoResumeClassObjects := Windows.GetProcAddress(Ole32, 'CoResumeClassObjects');
@CoSuspendClassObjects := Windows.GetProcAddress(Ole32, 'CoSuspendClassObjects');
end;
WINDOW中 ole32.dll中引出的API。
现在已经明白是怎么激活了,这种方式是用DCOM连接的。

现在来看看borland socket server怎么来接收数据，并创建远程数据模块的。
先看scktsvr.dpr中
SocketForm.Initialize(False);
procedure TSocketForm.Initialize(FromService: Boolean);
… ReadSettings;…..
看看ReadSettings函数：
if CompareText(Sections[i], csSettings) <> 0 then
CreateItem(StrToInt(Sections[i])); //定位这一句

procedure CreateItem(ID: Integer);
var
SH: TSocketDispatcher;
begin
SH := TSocketDispatcher.Create(nil); //定位这一句：
SH.ReadSettings(ID, Reg);
PortList.Items.AddObject(IntToStr(SH.Port), SH);
try
SH.Open;                 //定位这一句，注意：
except
on E: Exception do
raise Exception.CreateResFmt(@SOpenError, [SH.Port, E.Message]);
end;
end;
TSocketDispatcher = class(TServerSocket)
由此创建serversocket服务器端
constructor TSocketDispatcher.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
ServerType := stThreadBlocking;
OnGetThread := GetThread;
end;
服务器端用线程阻塞模式。
继续往下看：
constructor TServerSocket.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FServerSocket := TServerWinSocket.Create(INVALID_SOCKET);
InitSocket(FServerSocket);
FServerSocket.ThreadCacheSize := 10;
end;
创建了一个TserverWinSocket（服务器端）。
接下来调用InitSocket(FServerSocket);这是ServerSocket的祖先类TAbstractSocket的一个方法,传入参数是成员FserverSocket完成的功能是将ServerSocket的事件指针指向TServerWinSocket的事件，使其能处理Socket触发的事件。
最后，设置FServerSocket允许连接的线程数。
procedure TAbstractSocket.InitSocket(Socket: TCustomWinSocket);
begin
Socket.OnSocketEvent := DoEvent;
Socket.OnErrorEvent := DoError;
end;
procedure TAbstractSocket.DoEvent(Sender: TObject; Socket: TCustomWinSocket;
SocketEvent: TSocketEvent);
begin
Event(Socket, SocketEvent);
end;
里面是调用Event，再看看Event的声明：
procedure Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);virtual; abstract;
这是一个抽象函数，由此可以知道，TAbstractSocket是一个抽象的类，它只是封装了一般的操作，具体地都留到了它的子类中去了，所以它的子类一定覆盖了这个Event方法，而DoEvent中调用的Event实际上就是调用它子类的Event方法，这个就是典型的模板模式。
好，看看它的子类有没有覆盖这个方法，果然在TCustomSocket中看到了这个方法，并实现了它，看看它的源代码：
procedure TCustomSocket.Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
begin
case SocketEvent of
seLookup: if Assigned(FOnLookup) then FOnLookup(Self, Socket);
seConnecting: if Assigned(FOnConnecting) then FOnConnecting(Self, Socket);
seConnect:
begin
FActive := True;
if Assigned(FOnConnect) then FOnConnect(Self, Socket);
end;
seListen:
begin
FActive := True;
if Assigned(FOnListen) then FOnListen(Self, Socket);
end;
seDisconnect:
begin
FActive := False;
if Assigned(FOnDisconnect) then FOnDisconnect(Self, Socket);
end;
seAccept: if Assigned(FOnAccept) then FOnAccept(Self, Socket);
seRead: if Assigned(FOnRead) then FOnRead(Self, Socket);
seWrite: if Assigned(FOnWrite) then FOnWrite(Self, Socket);
end;
end;
其中FonAccept等这些都是TCusotmSocket的成员，都是TSocketNotifyEvent类型的，TCustomSocket还通过下面这些属性，使该类拥有了这些事件处理的能力：
property OnLookup: TSocketNotifyEvent read FOnLookup write FOnLookup;
property OnConnecting: TSocketNotifyEvent read FOnConnecting write FOnConnecting;
property OnConnect: TSocketNotifyEvent read FOnConnect write FOnConnect;
property OnDisconnect: TSocketNotifyEvent read FOnDisconnect write FOnDisconnect;
property OnListen: TSocketNotifyEvent read FOnListen write FOnListen;
property OnAccept: TSocketNotifyEvent read FOnAccept write FOnAccept;
property OnRead: TSocketNotifyEvent read FOnRead write FOnRead;
property OnWrite: TSocketNotifyEvent read FOnWrite write FOnWrite;
而上面的Event实现，我们可以看到它是根据SocketEvent的类型来调用相应的事件指针的，而它的子类ServerSocket和ClientSocket也拥有了这些事件的处理能力，到这里我们终于明白了，它们的事件是怎么样出来的，但如果想得知这些事件是怎么触发的，还要看这一句：Socket.OnSocketEvent，OnSocketEvent到后面再说，这里先略。
constructor TServerWinSocket.Create(ASocket: TSocket);
begin
FConnections := TList.Create;
FActiveThreads := TList.Create;
FListLock := TCriticalSection.Create;
inherited Create(ASocket); //定位这里：
FAsyncStyles := [asAccept];
end;
在这里，创建了用来管理客户端连接的Fconnections, 代表各个处理客户连接的Socket,
通过这个属性对各个客户连接进行操作. 线程池FactiveThreads, 管理由Connections 数组确定的的客户端连接线程返回当前正在使用的TServerClientThread对象的个数 。TcriticalSection ,Critical Section使得多线程程序中的一个线程能够暂时阻塞所有其他线程尝试使用同一个Critical Section, Critical Section可以用来确保一次只有一个线程正在执行那一块代码。因此，受到Critical Section保护的那块代码应当尽可能的小因为如果使用不当的话它们可能严重影响性能。所以，每块代码都应当使用它们自己的TCriticalSection，而不是重用全程序共享的TCriticalSection。
继续：
constructor TCustomWinSocket.Create(ASocket: TSocket);
begin
inherited Create;
Startup;
FSocketLock := TCriticalSection.Create;
FASyncStyles := [asRead, asWrite, asConnect, asClose];
FSocket := ASocket;
FAddr.sin_family := PF_INET;
FAddr.sin_addr.s_addr := INADDR_ANY;
FAddr.sin_port := 0;
FConnected := FSocket <> INVALID_SOCKET;
end;
这里看到TcustomWinSocket内部也使用了TcriticalSection.
Asocket 的值为：INVALID_SOCKET；

procedure Startup;
var
ErrorCode: Integer;
begin
ErrorCode := WSAStartup($0101, WSAData);
if ErrorCode <> 0 then
raise ESocketError.CreateResFmt(@sWindowsSocketError,
[SysErrorMessage(ErrorCode), ErrorCode, 'WSAStartup']);
end;
这个函数是加戴WINSOCKET库（1.0版本）,并初始化，地址，端口等信息。

返回 SH.Open;处：
procedure TAbstractSocket.SetActive(Value: Boolean);
begin
if Value <> FActive then
begin
if (csDesigning in ComponentState) or (csLoading in ComponentState) then
FActive := Value;
if not (csLoading in ComponentState) then
DoActivate(Value);
end;
end;
procedure TCustomServerSocket.DoActivate(Value: Boolean);
begin
if (Value <> FServerSocket.Connected) and not (csDesigning in ComponentState) then
begin
if FServerSocket.Connected then
FServerSocket.Disconnect(FServerSocket.SocketHandle)
else FServerSocket.Listen(FHost, FAddress, FService, FPort, SOMAXCONN);
end;
end;
当Value不等它的成员FServerSocket.Connected时（我们可先认为这个表示当前的监听状态），而如果此时FServerSocket.Connected为True，则表示Value为False,，那么这时要断开连接，调用FServerSocket.Disconnect(FServerSocket.SocketHandle)
很多时候ServerSocket都是调用它的成员FServerSocket来完成操作的，这一点我已经看到了，下面还有更多这样的情况。否则，则Value为True，调用
FServerSocket.Listen(FHost, FAddress, FService, FPort, SOMAXCONN);来开始进行监视。好的，现在我们就要打开FServerSocket的源码，看看它是怎么样完成断开连接和开始连接的了
procedure TServerWinSocket.Listen(var Name, Address, Service: string; Port: Word;
QueueSize: Integer);
begin
inherited Listen(Name, Address, Service, Port, QueueSize, ServerType = stThreadBlocking);
if FConnected and (ServerType = stThreadBlocking) then
FServerAcceptThread := TServerAcceptThread.Create(False, Self);
end;
这里调用其父类的Listen方法，如果它是阻塞方式的，则还要生成一个线程类。
而BORLAND这里用了阻塞方式。
看看到底做了什么：
procedure TCustomWinSocket.Listen(const Name, Address, Service: string; Port: Word;
QueueSize: Integer; Block: Boolean);
begin
if FConnected then raise ESocketError.CreateRes(@sCannotListenOnOpen);
FSocket := socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
if FSocket = INVALID_SOCKET then raise ESocketError.CreateRes(@sCannotCreateSocket);
try
Event(Self, seLookUp);
if Block then
begin
FAddr := InitSocket(Name, Address, Service, Port, False);
DoListen(QueueSize);
end else
AsyncInitSocket(Name, Address, Service, Port, QueueSize, False);
except
Disconnect(FSocket);
raise;
end;
end;
原来，创建了一个SOCKET套接字。
FSocket := socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
记得上面Fsockeet曾被赋过值吗，是INVALID_SOCKET;，而这里终于被替回了一个可用的套接字了。
Event(Self, seLookUp);
不出所料，调用事件指针了，这时ServerSocket的事件就会触发了，回过头去看看，procedure TCustomWinSocket.Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
begin
if Assigned(FOnSocketEvent) then FOnSocketEvent(Self, Socket, SocketEvent);
end;
seLookup: if Assigned(FOnLookup) then FOnLookup(Self, Socket);
这里CustomSocket的OnLookup事件发生，但ServerSocket并没有显化它，ClientSocket就有这个事件。
继续：
FAddr := InitSocket(Name, Address, Service, Port, False);
function TCustomWinSocket.InitSocket(const Name, Address, Service: string; Port: Word;
Client: Boolean): TSockAddrIn;
begin
Result.sin_family := PF_INET;
if Name <> '' then
Result.sin_addr := LookupName(name)
else if Address <> '' then
Result.sin_addr.s_addr := inet_addr(PChar(Address))
else if not Client then
Result.sin_addr.s_addr := INADDR_ANY
else raise ESocketError.CreateRes(@sNoAddress);
if Service <> '' then
Result.sin_port := htons(LookupService(Service))
else
Result.sin_port := htons(Port);
end;
说明：htons（将16位主机字符顺序转换成网络字符顺序）
procedure TCustomWinSocket.DoListen(QueueSize: Integer);
begin
CheckSocketResult(bind(FSocket, FAddr, SizeOf(FAddr)), 'bind');
DoSetASyncStyles;
if QueueSize > SOMAXCONN then QueueSize := SOMAXCONN;
Event(Self, seListen);
CheckSocketResult(Winsock.listen(FSocket, QueueSize), 'listen');
FLookupState := lsIdle;
FConnected := True;
end;
listen()用来等待参数s 的socket连线。参数backlog指定同时能处理的最大连接要求，如果连接数目达此上限则client端将收到ECONNREFUSED的错误。Listen()并未开始接收连线，只是设置socket为listen模式，真正接收client端连线的是accept()。通常listen()会在socket()，bind()之后调用，接着才调用accept()。
狐狸尾巴终于出来了，这里调用了SOCKET的API 绑定一个地址，并开始侦听。
继续上面的Listen事件
FServerAcceptThread := TServerAcceptThread.Create(False, Self);
winsocket侦听，并创建了一个AcceptThread线程。
多线程TserverAcceptThread来响应客户端的accept.
具体的线程执行代码如下：
procedure TServerWinSocket.Accept(Socket: TSocket);
var
ClientSocket: TServerClientWinSocket;
ClientWinSocket: TSocket;
Addr: TSockAddrIn;
Len: Integer;
OldOpenType, NewOpenType: Integer;
begin
Len := SizeOf(OldOpenType);
if getsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, PChar(@OldOpenType),
Len) = 0 then
try
if FServerType = stThreadBlocking then
begin
NewOpenType := SO_SYNCHRONOUS_NONALERT;
setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, PChar(@NewOpenType), Len);
end;
Len := SizeOf(Addr);
ClientWinSocket := WinSock.accept(Socket, @Addr, @Len);
if ClientWinSocket <> INVALID_SOCKET then
begin
ClientSocket := GetClientSocket(ClientWinSocket);
if Assigned(FOnSocketEvent) then
FOnSocketEvent(Self, ClientSocket, seAccept);
if FServerType = stThreadBlocking then
begin
ClientSocket.ASyncStyles := [];
GetServerThread(ClientSocket);
end;
end;
finally
Len := SizeOf(OldOpenType);
setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, PChar(@OldOpenType), Len);
end;
end;
getsockopt()会将参数s所指定的socket状态返回。
ClientWinSocket := WinSock.accept(Socket, @Addr, @Len);
accept()用来接受参数s的socket连线。参数s的socket必需先经bind()、listen()函数处理过，当有连线进来时accept()会返回一个新的socket处理代码，往后的数据传送与读取就是经由新的socket处理，而原来参数s的socket能继续使用accept()来接受新的连线要求。连线成功时，参数addr所指的结构会被系统填入远程主机的地址数据，参数addrlen为scokaddr的结构长度。
设置与指定套接口相关的属性选项。
setsockopt()用来设置参数s所指定的socket状态。
服务器端侦听客户连接。
然后服务器就就accept等待客户端连接


现在我们来追踪一下TsocketConnection源码分析一下。
大家知道TsocketConnection在open后，就会启动一个TremoteDataModule。为什么呢？
TsocketConnection.Open 看底层是怎么实现的。
procedure TCustomConnection.Open;
begin
SetConnected(True);
end;
procedure TCustomConnection.SetConnected(Value: Boolean);
….
if Value = GetConnected then Exit;
if Value then
begin
if Assigned(BeforeConnect) then BeforeConnect(Self);
DoConnect;
SendConnectEvent(True);
if Assigned(AfterConnect) then AfterConnect(Self);
end
..
BeforeConnect，和AfterConnect可让由用户在连接前和连接后做些什么事情。
看一下DoConnect;
procedure TSocketConnection.DoConnect; ->>>>>
procedure TStreamedConnection.DoConnect;
…
InternalOpen
…
procedure TStreamedConnection.InternalOpen;
begin
if FSupportCallbacks then
begin
FTransport := TTransportThread.Create(Handle, CreateTransport);
FTransport.OnTerminate := TransportTerminated;
WaitForSingleObject(FTransport.Semaphore, INFINITE);
end else
begin
FTransIntf := CreateTransport;
FTransIntf.SetConnected(True);
end;
end;
是否支持回调
function TSocketConnection.CreateTransport: ITransport;
var
SocketTransport: TSocketTransport;
begin
if SupportCallbacks then
if not LoadWinSock2 then raise Exception.CreateRes(@SNoWinSock2);
if (FAddress = '') and (FHost = '') then
raise ESocketConnectionError.CreateRes(@SNoAddress);
SocketTransport := TSocketTransport.Create;
SocketTransport.Host := FHost;
SocketTransport.Address := FAddress;
SocketTransport.Port := FPort;
SocketTransport.InterceptGUID := InterceptGUID;
Result := SocketTransport as ITransport;
end;
到这里，发现，原来真正干活的是TsocketTransport这个类。继续往下看。
SocketTransport := TSocketTransport.Create;
里面只是计数器加1。
继续执行的操作, FTransIntf.SetConnected(True);前面只是把一些类的信息初始化，比如IP地址，端口号等。
下面才是最重要的：

procedure TSocketTransport.SetConnected(Value: Boolean);
begin
if GetConnected = Value then Exit;
if Value then
begin
if (FAddress = '') and (FHost = '') then
raise ESocketConnectionError.CreateRes(@SNoAddress);
FClientSocket := TClientSocket.Create(nil);
FClientSocket.ClientType := ctBlocking;
FSocket := FClientSocket.Socket;
FClientSocket.Port := FPort;
if FAddress <> '' then
FClientSocket.Address := FAddress else
FClientSocket.Host := FHost;
FClientSocket.Open;
end else
begin
if FSocket <> nil then FSocket.Close;
FSocket := nil;
FreeAndNil(FClientSocket);
if FEvent <> 0 then WSACloseEvent(FEvent);
FEvent := 0;
end;
end;
分析一下，竟然在内部创建了一个TclientSocket，说明它的底层原原本本用SOCKET通讯来完成的，这里还没有远程创建数据模块。
继续回到前面的
procedure TStreamedConnection.DoConnect;
var
TempStr: string;
begin
try
if ServerGUID <> '' then
TempStr := ServerGUID else
TempStr := ServerName;
if TempStr = '' then
raise Exception.CreateResFmt(@SServerNameBlank, [Name]);
InternalOpen;
SetAppServer(Interpreter.CallCreateObject(TempStr));   ////
except
InternalClose;
raise;
end;
end;

InternalOpen; 由于和前面的代码相似，所以不去理会，一直跟踪到
procedure TCustomWinSocket.DoOpen;
begin
DoSetASyncStyles;
Event(Self, seConnecting);
CheckSocketResult(WinSock.connect(FSocket, FAddr, SizeOf(FAddr)), 'connect');
FLookupState := lsIdle;
if not (asConnect in FAsyncStyles) then
begin
FConnected := FSocket <> INVALID_SOCKET;
Event(Self, seConnect);
end;
end;
CheckSocketResult(WinSock.connect(FSocket, FAddr, SizeOf(FAddr)), 'connect');
API调用连接服务器。先气撇下SetAppServer，到服务器端跟踪一下。
服务器端得到Tsocket套接字之后
在accept函数里。
…
ClientWinSocket := WinSock.accept(Socket, @Addr, @Len);
if ClientWinSocket <> INVALID_SOCKET then
begin
ClientSocket := GetClientSocket(ClientWinSocket);
if Assigned(FOnSocketEvent) then
FOnSocketEvent(Self, ClientSocket, seAccept);
if FServerType = stThreadBlocking then
begin
ClientSocket.ASyncStyles := [];
GetServerThread(ClientSocket);
end;
end;
………………
..
ClientSocket := GetClientSocket(ClientWinSocket);
function TServerWinSocket.GetClientSocket(Socket: TSocket): TServerClientWinSocket;
begin
Result := nil;
if Assigned(FOnGetSocket) then FOnGetSocket(Self, Socket, Result);
if Result = nil then
Result := TServerClientWinSocket.Create(Socket, Self);
end;

创建了一个TserverClientWinSocket对象。用来管理客户端连接。
constructor TServerClientWinSocket.Create(Socket: TSocket; ServerWinSocket: TServerWinSocket);
begin
FServerWinSocket := ServerWinSocket;
if Assigned(FServerWinSocket) then
begin
FServerWinSocket.AddClient(Self);
if FServerWinSocket.AsyncStyles <> [] then
begin
OnSocketEvent := FServerWinSocket.ClientEvent;
OnErrorEvent := FServerWinSocket.ClientError;
end;
end;
inherited Create(Socket);
if FServerWinSocket.ASyncStyles <> [] then DoSetAsyncStyles;
if FConnected then Event(Self, seConnect);
end;
FServerWinSocket.AddClient(Self);增加一个客户端。
if FConnected then Event(Self, seConnect);
继续追踪到：
function TServerWinSocket.GetServerThread(ClientSocket: TServerClientWinSocket): TServerClientThread;
var
I: Integer;
begin
Result := nil;
FListLock.Enter;
try
for I := 0 to FActiveThreads.Count - 1 do
if TServerClientThread(FActiveThreads[I]).ClientSocket = nil then
begin
Result := FActiveThreads[I];
Result.ReActivate(ClientSocket);
Break;
end;
finally
FListLock.Leave;
end;
if Result = nil then
begin
if Assigned(FOnGetThread) then FOnGetThread(Self, ClientSocket, Result);
if Result = nil then Result := DoCreateThread(ClientSocket);
end;
end;
先到线程池中去检查，是否有活动的线程。否则，就返回一个可用的线程。
这里记得以前曾经设置过最大的线程为10。
TServerWinSocket.GetServerThread
if Assigned(FOnGetThread) then FOnGetThread(Self, ClientSocket, Result);
procedure TSocketDispatcher.GetThread(Sender: TObject;
ClientSocket: TServerClientWinSocket;
var SocketThread: TServerClientThread);
begin
SocketThread := TSocketDispatcherThread.Create(False, ClientSocket,
InterceptGUID, Timeout, SocketForm.RegisteredAction.Checked, SocketForm.AllowXML.Checked);
end;
创建了一个线程，专门处理客户端的信息。
具体的多线程的执行代码如下：
procedure TSocketDispatcherThread.ClientExecute;
var
Data: IDataBlock;
msg: TMsg;
Obj: ISendDataBlock;
Event: THandle;
WaitTime: DWord;
begin
CoInitialize(nil);
try
Synchronize(AddClient);
FTransport := CreateServerTransport;
try
Event := FTransport.GetWaitEvent;
PeekMessage(msg, 0, WM_USER, WM_USER, PM_NOREMOVE);
GetInterface(ISendDataBlock, Obj);
if FRegisteredOnly then
FInterpreter := TDataBlockInterpreter.Create(Obj, SSockets) else
FInterpreter := TDataBlockInterpreter.Create(Obj, '');
try
Obj := nil;
if FTimeout = 0 then
WaitTime := INFINITE else
WaitTime := 60000;
while not Terminated and FTransport.Connected do
try
case MsgWaitForMultipleObjects(1, Event, False, WaitTime, QS_ALLEVENTS) of
WAIT_OBJECT_0:
begin
if IsSocketClose(ClientSocket.SocketHandle, Event) then
break; //这里我自己加的。
WSAResetEvent(Event);
Data := FTransport.Receive(False, 0);
if Assigned(Data) then
begin
FLastActivity := Now;
FInterpreter.InterpretData(Data);
Data := nil;
FLastActivity := Now;
end;
end;
WAIT_OBJECT_0 + 1:
while PeekMessage(msg, 0, 0, 0, PM_REMOVE) do
DispatchMessage(msg);
WAIT_TIMEOUT:
if (FTimeout > 0) and ((Now - FLastActivity) > FTimeout) then
FTransport.Connected := False;
end;
except
FTransport.Connected := False;
end;
finally
FInterpreter.Free;
FInterpreter := nil;
end;
finally
FTransport := nil;
end;
finally
CoUninitialize;
Synchronize(RemoveClient);
end;
end;
创建了TdataBlockInterpreter类。
TDataBlockInterpreter(对发送过来的数据进行解析InterpretData(Data: IdataBlock)
实际处理的类型是
TDataBlock = class(TInterfacedObject, IDataBlock)
客户端和服务器传递的数据块就是TdataBlock.
创建完TsocketDispatcherThread之后，便继续又进入了accept中（前面建立的线程中），等待下一个客户的连接。
TsocketDispatcherThread也执行到MsgWaitForMultipleObjects
为了解决在主线程中Wait的问题，微软专门设计了一个函数MsgWaitForMultipleObjects，这个函数即可以等待信号（thread,event,mutex等等），也可以等待消息（MSG）。即不论有信号被激发或者有消息到来，此函数都可以返回。
再看客户端：
SetAppServer(Interpreter.CallCreateObject(TempStr));这句才是真正的启动远程的数据模块。跟踪一下TempStr,发现他就是COM组件的GUID。
一个想法，如果传递一个guid给另一台机器，再由另一台机器通过COM工厂查找相应的EXE，然后由COM工厂来启动这个COM组件不就完全这个过程了吗？
继续看它后面是怎么做的。
function TStreamedConnection.GetInterpreter: TCustomDataBlockInterpreter;
begin
if not Assigned(FInterpreter) then
FInterpreter := TDataBlockInterpreter.Create(Self, SSockets);
Result := FInterpreter;
end;
先创建一个TdataBlockInterpreter类.（数据块的解释）
可以看到，服务器端和客户端都是利用TdataBlockInterpreter来解释数据的。
看到这里，大家应该睁大眼睛了，前面说的代理DLL和存根DLL的列集和散集，就是由这个类来完成的。
function TDataBlockInterpreter.CallCreateObject(Name: string): OleVariant;
var
Flags: TVarFlags;
Data: IDataBlock;
begin
Data := TDataBlock.Create as IDataBlock;
WriteVariant(Name, Data);
Data.Signature := CallSig or asCreateObject;
Data := FSendDataBlock.Send(Data, True);
Result := ReadVariant(Flags, Data);
end;
首先来创建一个数据块.
constructor TDataBlock.Create;
begin
inherited Create;
FIgnoreStream := False;
FStream := TMemoryStream.Create;
Clear;
end;
发现它创建了一个TmemoryStream对象。
WriteVariant(Name, Data); 把Name写到变量中，Name就是要调用的GUID
看看怎么写进去的。
procedure _OleVarFromLStr(var V: TVarData {OleVariant}; const Value: string);
begin
_VarFromWStr(V, WideString(Value));
end;
procedure TDataBlockInterpreter.WriteVariant(const Value: OleVariant;
const Data: IDataBlock);
var
I, VType: Integer;
W: WideString;
begin
。。。
case (VType and varTypeMask) of
。。
varOleStr:
begin
W := WideString(Value);           //转换成WideString;
I := Length(W);                  //计算长度
Data.Write(VType, SizeOf(Integer));   //写入相应的类型信息。
在8个字节之后写入类型信息。长度4
Data.Write(I,SizeOf(Integer));        //写入长底信息。 长度4
Data.Write(W[1], I * 2);             //写入字符串信息， 长度*2（宽字节）
end;
。。。。
end;
这里要详细说明一下：
我们知道，TdataBlockInterpreter是负责解释数据的，而TdataBlock采是真正的数据块。
根据Signature的不同，代表，不同的数据包。对于不同的数据包。TdataBlockInterpreter要负责解释，到底要做什么动作。另外要注意的是，在向TdataBlock中写数据的时候，相应的参数都被转化成了OleVariant类型。
Data.Signature := CallSig or asCreateObject; 设置数据块的标记。这个数据块到底是干什么的。
等下可能会发现很多不同种类的标记。
看到这里，终于明白了，这个数据块是想创建一个对象。（远程对象）
再研究一下TdataBlock里到底是什么样的结构。

接着看看send都做了什么。
Data := FSendDataBlock.Send(Data, True);
if not Assigned(FTransIntf) then Exit;
Context := FTransIntf.Send(Data);
Result := FTransIntf.Receive(WaitForResult, Context);
end;
if Assigned(Result) and ((Result.Signature and asMask) = asError) then
Interpreter.InterpretData(Result);

function TSocketTransport.Send(const Data: IDataBlock): Integer;
var
P: Pointer;
begin
Result := 0;
InterceptOutgoing(Data);
P := Data.Memory;
FSocket.SendBuf(P^, Data.Size + Data.BytesReserved);
end;
FSocket: TCustomWinSocket;
Socket把数据发送给了 Borland socket Server, 因为已经启动了。正在侦听数据。
大家看到send发送之后，服务器就启动了, 。
Result := FTransIntf.Receive(WaitForResult, Context);
是服务器端传回来的FappServer, 保存在
TCustomRemoteServer = class(TCustomConnection)
private
FAppServer: Variant;
中。
然后客户端一直等着服务器端返回的数据。
Result := FTransIntf.Receive(WaitForResult, Context);
下面看服务器端代码：
当客户端连接时，便启动一个相应的线程。查看一下它的父类。procedure TServerClientThread.Execute;
begin
。。。。
while True do
begin
if StartConnect then ClientExecute;
if EndConnect then Break;
end;
。。。。。。。
现在看服务器端的procedure TSocketDispatcherThread.ClientExecute;
case MsgWaitForMultipleObjects(1, Event, False, WaitTime, QS_ALLEVENTS) of
WAIT_OBJECT_0:
begin
if IsSocketClose(ClientSocket.SocketHandle,  Event) then
break;
WSAResetEvent(Event);
Data := FTransport.Receive(False, 0);
if Assigned(Data) then
begin
FLastActivity := Now;
FInterpreter.InterpretData(Data);
Data := nil;
FLastActivity := Now;
end;
end;
再看FInterpreter.InterpretData(Data);
FInterpreter: TDataBlockInterpreter;
又由它来解释数据：
procedure TDataBlockInterpreter.InterpretData(const Data: IDataBlock);
var
Action: Integer;
begin
Action := Data.Signature;
if (Action and asMask) = asError then DoException(Data);
try
case (Action and asMask) of
asInvoke: DoInvoke(Data);
asGetID: DoGetIDsOfNames(Data);
asCreateObject: DoCreateObject(Data);
asFreeObject: DoFreeObject(Data);
asGetServers: DoGetServerList(Data);
asGetAppServers: DoGetAppServerList(Data);
else
if not DoCustomAction(Action and asMask, Data) then
raise EInterpreterError.CreateResFmt(@SInvalidAction, [Action and asMask]);
end;
except
on E: Exception do
begin
Data.Clear;
WriteVariant(E.Message, Data);
Data.Signature := ResultSig or asError;
FSendDataBlock.Send(Data, False);
end;
end;
end;

可以看到Action := Data.Signature;数据标志如下（客户端请求做什么）：
asInvoke: DoInvoke(Data);//调用方法
asGetID: DoGetIDsOfNames(Data); //得到一个ID
asCreateObject: DoCreateObject(Data); //创建一个对象(com)
asFreeObject: DoFreeObject(Data);   //释放一个对象(com)
asGetServers: DoGetServerList(Data); //得到列表，与MIDAS有关
asGetAppServers: DoGetAppServerList(Data); //得到列表，
这里是要求创建一个对象。
procedure TDataBlockInterpreter.DoCreateObject(const Data: IDataBlock);
var
V: OleVariant;
VarFlags: TVarFlags;
I: Integer;
begin
V := CreateObject(ReadVariant(VarFlags, Data));
Data.Clear;//数据块清空。
I := TVarData(V).VType;//得到数据类型；
if (I and varTypeMask) = varInteger then
begin
I := varDispatch;
Data.Write(I, SizeOf(Integer));
I := V;
Data.Write(I, SizeOf(Integer));
end else
WriteVariant(V, Data);   //直接把中间层创建的COM组件的指针传递给客户端。
Data.Signature := ResultSig or asCreateObject;
FSendDataBlock.Send(Data, False);
end;
这里要注意：
现在返回客户端的数据包，我们研究一下，看它是怎么打包的。
那么，客户端怎么能通过这个指针来调用相应的方法呢。原因在于，这个指针已经被保存在了FDispList: OleVariant; TdataBlockInterpreter的私有变量中。
真正创建组件的地方：
function TDataBlockInterpreter.InternalCreateObject(const ClassID: TGUID): OleVariant;
var
Unk: IUnknown;
begin
OleCheck(CoCreateInstance(ClassID, nil, CLSCTX_INPROC_SERVER or
CLSCTX_LOCAL_SERVER or CLSCTX_REMOTE_SERVER, IUnknown, Unk));
Result := Unk as IDispatch;
end;
V := CreateObject(ReadVariant(VarFlags, Data));
这里指针可以保存在OleVariant变量里，请注意这种用法。可以把它传递给客户端。

WriteVariant(V, Data); //这一句很重要。多次被我忽略：
procedure TDataBlockInterpreter.WriteVariant
。。。
varDispatch:
begin
if VType and varByRef = varByRef then
raise EInterpreterError.CreateResFmt(@SBadVariantType,[IntToHex(VType,4)]);
I := StoreObject(Value);
Data.Write(VType, SizeOf(Integer));
Data.Write(I, SizeOf(Integer));
end;
。。。。
I := StoreObject(Value);
真是辛苦。终于找到你了。
FDispList: OleVariant;
FDispList := VarArrayCreate([0,10], varVariant);
默认创建了十个数组。
把创建的COM对象的指针全部都存在了一个LIST中。
为什么要传给客户端，前面不是说了一个机器的指针传到另一个机器里是没有意义的。
我猜这里是为了定位。继续往下看。
继续仔细看看它的包的结构是什么样子的：
I := StoreObject(Value);//返回数组下标
Data.Write(VType, SizeOf(Integer));//写入数据类型
Data.Write(I, SizeOf(Integer));//及数组下标
看到这里很失望吧，原来客户端的GetApp是什么东西。原来只是中间层返回的一个数组下标而已。

Data.Signature := ResultSig or asCreateObject;
FSendDataBlock.Send(Data, False);
把数据发送给了客户端。
现在跟踪客户端代码：
前面分析到了客户端
function TStreamedConnection.Send之后一直都在
Result := FTransIntf.Receive(WaitForResult, Context);
TSocketTransport.Receive
。。。
Result := TDataBlock.Create as IDataBlock;
先创建数据块后，把数据拆分出来。
if Assigned(Result) and ((Result.Signature and asMask) = asError) then
Interpreter.InterpretData(Result);
再交给TdataBlockInterpreter来解释数据：
又回到客户端的
function TDataBlockInterpreter.CallCreateObject(Name: string): OleVariant;
var
Flags: TVarFlags;
Data: IDataBlock;
begin
Data := TDataBlock.Create as IDataBlock;
WriteVariant(Name, Data);
Data.Signature := CallSig or asCreateObject;
Data := FSendDataBlock.Send(Data, True);
Result := ReadVariant(Flags, Data);
end;

Data := FSendDataBlock.Send(Data, True);
终于得到了数据。
Result := ReadVariant(Flags, Data);
这一句
TDataBlockInterpreter.ReadVariant
所以ReadVariant, 和writeVariant其实就是解释数据。
后来放出现ReadVariant函数；
varDispatch:
begin
Data.Read(I, SizeOf(Integer));
Result := TDataDispatch.Create(Self, I) as IDispatch;
end;
在创建TdataDispatch这个类的时候，会把相应的指针传进来。登记在FdispatchIndex中。
TDataDispatch = class(TInterfacedObject, IDispatch)
private
FDispatchIndex: Integer;
constructor TDataDispatch.Create(Interpreter: TCustomDataBlockInterpreter; DispatchIndex: Integer);
begin
inherited Create;
FDispatchIndex := DispatchIndex;
FInterpreter := Interpreter;
Interpreter.AddDispatch(Self);
end;
TdataDispatch 的 FInterpreter: TCustomDataBlockInterpreter;
又引用了TdataBlockInterpreter这个类。
procedure TDataBlockInterpreter.AddDispatch(Value: TDataDispatch);
begin
if FDispatchList.IndexOf(Value) = -1 then
FDispatchList.Add(Value);
end;
再回到客户端的
procedure TStreamedConnection.DoConnect;
。。。。
SetAppServer(Interpreter.CallCreateObject(TempStr));   ////
到这里终于明白。原来
TSocketConnection.GetServer里面藏的竟然是一个TdataDispatch
终于明白了吧？
这里太重要了。
TStreamedConnection是TsocketConnection的父类。
TdispatchConnection是TStreamedConnection的父类。
function TDispatchConnection.GetServer: IAppServer;
var
QIResult: HResult;
begin
Connected := True;
QIResult := IDispatch(AppServer).QueryInterface(IAppServer, Result);
if QIResult <> S_OK then
Result := TDispatchAppServer.Create(IAppServerDisp(IDispatch(AppServer)));
end;

原来创建了一个TdispatchAppServer类，并把AppServer传进去，
AppServer就是FAppServer: Variant; 这里是TdataDispatch，转换成Idispatch指针
IDispatch = interface(IUnknown)
['{00020400-0000-0000-C000-000000000046}']
function GetTypeInfoCount(out Count: Integer): HResult; stdcall;
function GetTypeInfo(Index, LocaleID: Integer; out TypeInfo): HResult; stdcall;
function GetIDsOfNames(const IID: TGUID; Names: Pointer;
NameCount, LocaleID: Integer; DispIDs: Pointer): HResult; stdcall;
function Invoke(DispID: Integer; const IID: TGUID; LocaleID: Integer;
Flags: Word; var Params; VarResult, ExcepInfo, ArgErr: Pointer): HResult; stdcall;
end;
的具体定义。

IRDMCardServerDisp(IDispatch(pSocketConn.GetServer))；
接口赋值后：
pIRDMDataPerDisp.SelectData(sSqlStr, sPrvName);
这个是我在TLB定义的一个方法：
pIRDMDataPerDisp.SelectData(sSqlStr, sPrvName);
继续：sSqlStr 为一条SQL语句：
下面看看参数是如何打包的。如何把参数信息压入数据块中的：
_IntfDispCall函数中的：
JMPDispCallByIDProc
DispCallByID这个函数中：
function TDataDispatch.Invoke(DispID: Integer; const IID: TGUID; LocaleID: Integer;
Flags: Word; var Params; VarResult, ExcepInfo, ArgErr: Pointer): HResult; stdcall;
begin
Result := FInterpreter.CallInvoke(FDispatchIndex, DispID, IID, LocaleID, Flags,
Params, VarResult, ExcepInfo, ArgErr);
end;

function TDispatchAppServer.Invoke(DispID: Integer; const IID: TGUID; LocaleID: Integer;
Flags: Word; var Params; VarResult, ExcepInfo, ArgErr: Pointer): HResult;
begin
Result := IDispatch(FAppServer).Invoke(DispID, IID, LocaleID, Flags, Params,
VarResult, ExcepInfo, ArgErr);
end;
我们在前面看到了FappServer实际上就是TDataDispatch
function TDataDispatch.Invoke(DispID: Integer; const IID: TGUID; LocaleID: Integer;
Flags: Word; var Params; VarResult, ExcepInfo, ArgErr: Pointer): HResult; stdcall;
begin
Result := FInterpreter.CallInvoke(FDispatchIndex, DispID, IID, LocaleID, Flags,
Params, VarResult, ExcepInfo, ArgErr);
end;
可以看到调用后来转换成了TdataBlock数据打包，
把参数写入数据块中。
Data := FSendDataBlock.Send(Data, True);及数据发送（调用远程对象的方法）
这里的DispID就是在TLB定义的数值。
function TDataBlockInterpreter.CallInvoke(DispatchIndex, DispID: Integer; const IID: TGUID; LocaleID: Integer;
Flags: Word; var Params; VarResult, ExcepInfo, ArgErr: Pointer): HResult; stdcall;
var
VarFlags: TVarFlags;
PDest: PVarData;
i: Integer;
Data: IDataBlock;
begin
Data := TDataBlock.Create as IDataBlock;
WriteVariant(DispatchIndex, Data);
WriteVariant(DispID, Data);
WriteVariant(Flags, Data); // VarFlags 标志：
WriteVariant(VarResult <> nil, Data);
WriteVariant(PDispParams(@Params).cArgs, Data);
WriteVariant(PDispParams(@Params).cNamedArgs, Data);
for i := 0 to PDispParams(@Params).cNamedArgs - 1 do
WriteVariant(PDispParams(@Params).rgdispidNamedArgs[i], Data);
for i := 0 to PDispParams(@Params).cArgs - 1 do
WriteVariant(OleVariant(PDispParams(@Params).rgvarg^[i]), Data);
Data.Signature := CallSig or asInvoke;
Data := FSendDataBlock.Send(Data, True);
Result := ReadVariant(VarFlags, Data);
if (Result = DISP_E_EXCEPTION) then
begin
PExcepInfo(ExcepInfo).scode := ReadVariant(VarFlags, Data);
PExcepInfo(ExcepInfo).bstrDescription := ReadVariant(VarFlags, Data);
end;
for i := 0 to PDispParams(@Params).cArgs - 1 do
with PDispParams(@Params)^ do
if rgvarg^[i].vt and varByRef = varByRef then
begin
if rgvarg^[i].vt = (varByRef or varVariant) then
PDest := @TVarData(TVarData(rgvarg^[i]).VPointer^)
else
PDest := @TVarData(rgvarg^[i]);
CopyDataByRef(TVarData(ReadVariant(VarFlags, Data)), PDest^);
end;
if VarResult <> nil then
PVariant(VarResult)^ := ReadVariant(VarFlags, Data);
end;
Data := FSendDataBlock.Send(Data, True);
的具体函数中：
TStreamedConnection.Send
并开始接收服务器返回的数据。
ClientExecute
服务器端：
FInterpreter.InterpretData(Data);
asInvoke: DoInvoke(Data);
发现是一个函数调用：
ObjID := ReadVariant(VarFlags, Data);
Disp := LockObject(ObjID);
LockObject调用：
Result := FDispList[ID];
返回TdataBlockInterpreter维护的远程数据模块的ID。
DispID := ReadVariant(VarFlags, Data);
Flags := ReadVariant(VarFlags, Data);
ExpectResult := ReadVariant(VarFlags, Data);
DispParams.cArgs := ReadVariant(VarFlags, Data);
DispParams.cNamedArgs := ReadVariant(VarFlags, Data);
解析数据：
最终RetVal := Disp.Invoke(DispID, GUID_NULL, 0, Flags, DispParams, @V, @ExcepInfo, nil);
可以看到Disp就是COM的指针，调用了COM的invoke的方法。并返回了数据
对于这个函数。BORLAND并没有开放。大家可以参考WEBSERVICE的 invoke道理应该是一样的。
就是动态的找到相应的函数，并把参数压入堆栈中，动态调用。
可能当时BORLAND觉得这个技术挺牛的，没有开放，后来WEBSERVICE又开放了出来。
这里运用了比较高深的技术，比较动态代理，动态调用等。
1: TDataBlockInterpreter(对发送过来的数据进行解析InterpretData(Data: IDataBlock))
解析数据（水平有限，对它真是还是一知半解，有错请指出)
接口类IDataBlock,由TDataBlock通过TMemoryStream的读写来实现，其中Signature是其主要标识，说明这个IDataBlock的数据类型，TDataBlockInterpreter根据Signature来对应进行相应的调用，如：
Client端连接后，在Server要运行应用服务器(Application Server),
Client端需要得到ServerName列表,
Client端得到Server的DataBroker的列表，
Client端断开连接后，Server要Close应用服务器(Application Server),
Client和Server的数据交换，也是由它来解析。
所以这个IDataBlock的数据很重要，而我们的压缩和解压就是针对于它，但是TDataBlockInterpreter是得到Data才对它解析，因而我们要在Send和Recv之前对它解压和压缩。这个任务在TSocketTransport身上。
2: TSocketTransport;(数据进行发送和接收,实现ITransport接口)
Server端:
在Server端,TSocketTransport其实就是一个用来管理对ClientSocket实例，它将ClientSocket.Handle生成一个对象后，ClientSocket发送和接收过来的Data，在发送Data之前,它将调用InterceptOutgoing(Data: IDataBlock)函数，这个函数的功能是:
如果InterceptGUID <> ''，那么它将根据这个GUID生成一个COM(Obj)对象，Obj.DataOut(Data: IDataBlock),也就是我们注册的那个压缩的DLL中的那个压缩函数，将压缩过后的Data再发送出去。这就完成compress and send Data.(我试过那个压缩功能，压缩比大概是1/9,像zip压缩比差不多).
由客户端传过来的数据调用InterceptIncoming(Data: IDataBlock)函数，这就不多说了，Data :=解压后的Data.压缩和解压过后的Data交由TDataBlockInterpreter去解析，完成一次数据交换。
Client端：
说完Server端，客户端的道理也是差不多的。唯一不同的是Server端中不调用ITransport.SetConnected()方法，因为它是根据ClientSocket.Handle生成的对象，也就是它是已经连接的对象,而Client端的TSocktConnection调用Connected := True时，其实就是调用ITransport.SetConnect将一个ClientSocket连接到Server端中的TServerSocket中，然后TServerSocket根据这个ClientSocket.Handle生成了一个TServerClientThread对象保存在本地中，开始对这个ClientSocket的监控(FD_Read, FD_Close消息事件).
注：
IDataBlock由TDataBlock实现，主要是管理TMemoryStream来存放数据
ITransport由TSocketTransport实现，主要是用TClientSocket来连接TServerSocket,并和它进行交换数据。
ISendDataBlock在Scktsrvr.exe中由TServerClientThread实现，通过TSocketTransport来发送数据.
TDataDispatch使CONN的 GETAPP转向TDataBlockInterpreter由它解释数据。并用TDataBlock 打包数据。
TDataBlockInterpreter用
FSendDataBlock: ISendDataBlock;
接收，发送数据。
TDataBlockInterpreter创建的时候传入TStreamedConnection。
然后又把事件绑定在TDataBlockInterpreter上。
而TDataBlockInterpreter的事件，实际是由TSocketTransport完成的,绑定在TSocketTransport事件中。
TStreamedConnection.InternalOpen;
FTransIntf := CreateTransport;
TStreamedConnection.Send
Context := FTransIntf.Send(Data);





