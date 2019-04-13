
# 远程调用技术代码追踪(Remobjects第三方控件) - .NET博文收藏 - CSDN博客


2008年06月10日 12:52:00[hejishan](https://me.csdn.net/hejishan)阅读数：1940



远程调用技术内幕
在前面我已经分析了socket和webservice的代码追踪。现在总结一下：三层架构的运作模型：
1.BizSnap与.NET Remoting的Server端运作模式
当Client将Request送达Server端后，会经过一个Message Dispatcher机制，这个机制大多是几个重要的组件合作完成，主要在于解出Request中对于所要求对象的描述，以及欲呼叫的方法等信息，有了这些信息后Dispatcher就可以找到对应的对象与方法，接着就开始了呼叫动作，由于Request是SOAP讯息格式，并不能直接用来呼叫对象的方法，因此得先将SOAP讯息转化为Stack(堆栈)，完成这个转换动作后就到了这种处理模式中的核心概念了，也就是建立起目的对象并呼叫对应的方法，这个动作非常依赖前面的Message To Stack程序，因为这个程序会将SOAP讯息转化为Stack，有了Stack之后Push Stack and Call Method动作才能正确的执行，那么如何呼叫目的方法呢?很简单，只要利用该语言所提供的RTTI信息(.NET中则是MetaData)，就可取得该方法的内存地址，接着只须以低阶的ASM或IL所提供的CALL指令即可呼叫该方法，由于已将SOAP讯息转为Stack，因此传入参数就不是问题了。在呼叫结束后，Stack中已经有了传回的参数，接着只须将Stack转回SOAP讯息传回给Client端就可以了。
**BizSnap****、****.NET Remoting****的****Client****端运作模式**
不管是BizSnap或是.NET Remoting，当Client端欲呼叫Web Services时都会经过一个Proxy Object，于BizSnap中这个对象就是THTTPRIO，.NET Remoting中这个对象就是RealProxy，由于这个对象属于静态的，因此在使用之前必需将其转型回目的对象的型别，当Client端下达转型动作后整个魔法就开始运行了，首先Proxy Object会利用RTTI或是MetaData信息取得欲转型的类别信息，并依照这些信息建立起一个兼容于该类别的对象(Transparent Proxy Object)，接着将这个对象中的所有方法地址替换为Stub Method，Stub Method做的事情很单纯，只是将Stack转为SOAP Message后送出，当Server端响应后再将SOAP Message转换为Stack后返回，这样整个Client端呼叫动作就完成了，下次再呼叫时只需由Cache中取出这个已建立好的Transparent Proxy Object，就可以直接进行呼叫，这可以避免因反复以RTTI或是MetaData建立Transparent Proxy Object而失去效率。
BizSnap、.NET Remoting的处理模式属于较低阶的方法，这种方法的坏处大于好处，
好处是设计者可以完全不了解其内部运作，以传统方式来撰写程序，坏处是过度依赖编译器及平台，增加日后移植到其它语言或平台上的难度，另外使用动态产生对象类别的低阶技术很容易引发效率及内存管理的问题。

2.**NET Web Services****与****Java**
NET Web Services与Java的处理模式与.NET Remoting、BizSnap大同小异，其间最大的不同之处在于这种模式利用了其语言的特性，采动态呼叫的方式来执行呼叫的动作，而非如先前所提的模式在Stack与Message之间进行转换，这种模式简单的在Client端与Server端之间插入一个预先编译好的Proxy Object，这个Object是由WSDL所产生的，其中定义了所有目标对象的方法，在这些方法中简单的将传入的参数转换为SOAP Message，将传回的讯息转回参数，其间的运作完全属于高阶型态:
Client端的呼叫

服务器端：


由上面两个图上可看出，这种模式讲求简单，Client端的Stub Method由原本的一个变成每个方法一个，Server端则由原本的低阶CALL命令转为语言本身所提供的动态呼叫功能。这样的简化带来了效率，由于Client端不再经过动态转型与建立中介对象的动作，因此在效率上有显着的提升，也因为少了这些低阶的动作，内存管理上显得容易多了。但这种方式却有着另外的几个缺点，由于Proxy Object的程序代码增加，相对的程序所占用的内存也随之变大，另外Server采用动态呼叫的方式来唤起方法，这种方式通常效率不高。
**RemObjects SDK**
前面所提的两种模式皆有其优缺点，RO在这方面则提出了另一个崭新的处理模式，
下图是RO的Server端处理模式:


上图中大约与前面所提及的模式相同，其中不同之处在于Invoke Object，这是一个预先编译好的对象，其作用与.NET Web Services的Proxy Object相同，这个对象中所有方法都是Stub Method，将SOAP讯息转为参数后呼叫Real Object(Implement Object)的方法，完成后将参数转回讯息后返回Client端。那么这种模式有何独到之处呢??答案是效率，整个动作之中看不到低阶的Stack或是动态呼叫，没有这些动作的加入，当然速度上也就加快不少。





RO的Client端处理方式与Server端大同小异，因此结论是! RO没有用到任何的中介技术，也没有用到任何语言独有的功能，这也是RO .NET为何能在短短的几个月内就能完成的主要原因。

下面对RO自带的代码CalcService进行分析：
ROWinMessageServer1, TROIndyHTTPServer, TROIndyTCPServer
服务器支持windowMsg, HTTP, TCP等连接方式：
ROMessage: TROSOAPMessage; ROBINMessage1: TROBINMessage;
支持SOAP,和bin两种数据格式：
RO支持多种连接方式和两种数据格式，具体请参考相应资料。
现在先分析服务器端和ROTCPClient客户端及相应的TROBINMessage。
打开CalcService服务器端之后，开始跟踪代码：
unit uROTypes;
initialization
type TRODataType = (rtInteger, rtDateTime, rtDouble, rtCurrency, rtWidestring, rtString,
rtInt64, rtBoolean, rtVariant, rtBinary, rtUserDefined);
定义了RO的相应的数据类型
unit uROClient;
_MessageClasses := TClassList.Create;
_ExceptionClasses := TClassList.Create;
用来注册Message和exception类。
procedure RegisterStringFormats(const StringFormats: array of TROStringFormatClass);
unit uROProxy;
initialization
_ProxyClasses := TStringList.Create;
//创建ProxyClasses
unit uROServer;
initialization
AddTerminateProc(FinalizeClasses);
_ServerClasses := TClassList.Create;
_ClassFactoryList := nil;
//创建成ServerClasses
unit uROIndyTCPServer;
initialization
RegisterServerClass(TROIndyTCPServer);
// TROIndyTCPServer注册到ServerClasses列表中。

unit uROIndyHTTPServer;
initialization
RegisterServerClass(TROIndyHTTPServer);
// TROIndyHTTPServer注册到ServerClasses列表中。
unit uROBINMessage;
initialization
RegisterMessageClass(TROBINMessage);
// TROBINMessage注册到MessageClass列表中。
unit uROWinMessageServer;
initialization
RegisterServerClass(TROWinMessageServer);
// TROWinMessageServer注册到MessageClass列表中。
然后到
unit CalcLibrary_Intf;单元
initialization
RegisterProxyClass(CalcService_IID, TCalcService_Proxy);
可以看到，注到到前面创建的_ProxyClass中。
_ProxyClasses := TStringList.Create;
procedure RegisterProxyClass(const anInterfaceID : TGUID; aProxyClass : TROProxyClass);
begin
_ProxyClasses.AddObject(GUIDToString(anInterfaceID), TObject(aProxyClass))
end;
unit CalcService_Impl;
initialization
TROClassFactory.Create('CalcService', Create_CalcService, TCalcService_Invoker);
这里是比较重要的地方：
我们要把一些信息到TROClassFactory中注册。
依次是接口，过程(唤起业务对象)，invoker类（完成数据打包，发送等）
procedure Create_CalcService(out anInstance : IUnknown);
begin
anInstance := TCalcService.Create(NIL);
end;
另外unit CalcLibrary_Invk;
TCalcService_Invoker = class(TROInvoker)
private
protected
published
procedure Invoke_Sum(const __Instance:IInterface; const __Message:IROMessage; const __Transport:IROTransport);
procedure Invoke_GetServerTime(const __Instance:IInterface; const __Message:IROMessage; const __Transport:IROTransport);
end;
这个类则是完成了低层的调用。
看看这段代码：
procedure TCalcService_Invoker.Invoke_Sum(const __Instance:IInterface; const __Message:IROMessage; const __Transport:IROTransport);

__Message.Read('A', TypeInfo(Integer), A, []);
__Message.Read('B', TypeInfo(Integer), B, []);
Result := (__Instance as CalcService).Sum(A, B);
这里调用了TcalcService的具体方法：

__Message.Initialize(__Transport, 'CalcLibrary', 'CalcService', 'SumResponse');
__Message.Write('Result', TypeInfo(Integer), Result, []);
__Message.Finalize;
由此可见，真正工作的是TCalcService_Invoker这个类
它把低层的通信隐藏了起来。取出客户端调用的参数后，调用TcalcService做实际的业务处理，然后把相应的信息写入Message中返回给客户端。稍后请看详细的分析。
看看实际注册的过程：
TRORemotableCreatorFunc = procedure(out anInstance : IInterface);
constructor TROClassFactory.Create(const anInterfaceName: string;
aCreatorFunc: TRORemotableCreatorFunc;
anInvokerClass : TROInvokerClass);
begin
inherited Create;
fInvoker := NIL;
fCreatorFunc := aCreatorFunc;
fInterfaceName := anInterfaceName;
fInvokerClass := anInvokerClass;
RegisterClassFactory(Self);
end;
TROClassFactory = class(TInterfacedObject, IROClassFactory)
private
fCreatorFunc : TRORemotableCreatorFunc;
把实际调用业务对象的地址赋给fCreatorFunc，在适当的时候创建该业务对象。
继续：
procedure RegisterClassFactory(const aClassFactory : IROClassFactory);
begin
ClassFactoryList.Add(aClassFactory);
end;
把每个工厂增加到ClassFactoryList中，统一管理。
TROClassFactoryList = class(TInterfaceList)
继续：
Application.CreateForm(TfmMain, fmMain);
constructor TROMessage.Create(aOwner : TComponent);
begin
inherited Create(aOwner);
InitObject;
end;
继续：
procedure TROSOAPMessage.InitObject;
begin
inherited;
fXMLMessage := NewROXmlDocument;
fXMLMessage.New(tag_Envelope);
end;
function NewROXmlDocument : IXMLDocument;
result := TROMSXMLDocument.Create;
//这里就不细分析了，在《远程调用WEBSERVICE》中已经具体分析过。用来解析XML。
总结一下：CalcLibrary_Intf,，CalcService_Impl和CalcLibrary_Invk单元。这三个单元。
CalcLibrary_Intf
主要是声明了业务接口。TCalcService_Proxy一个代理的类（这里用的是静态代理），这里留到以后（RO代码生成技术，生成三个单元，我是不喜欢这种用法的，如果能改成用动态代理，估计会节省大量的代码，整个维护更加简单。从EJB到SPRING，大家可以看到这种代码生成技术的不足，否则JAVA中的SPRING也不会这么流行，有兴趣的兄弟，还是可以把它改成动态代理来实现。我也有一个想法，Indy控件的效率及代码生成还有数据持久层及业务对象处理上(做一款类似spring的框架)，RO支持的还是不充分的，有时间，准备把它改写一遍，有兴趣的可以和我一起交流）。
继续代码追踪：
CoCalcService = class
class function Create(const aMessage : IROMessage; aTransportChannel : IROTransportChannel) : CalcService;
end;
这个是一个服务类。就是创建一个TCalcService_Proxy代理。
CalcService_Impl这个单元，则是真正的业务对象方法实现的单元。
procedure Create_CalcService(out anInstance : IUnknown);
begin
anInstance := TCalcService.Create(NIL);
end;
这里RO用了一种常用的事件绑定的手法。这样RO的工厂指针赋值后就可唤起业务对象，实现业务对象的激活。
TROClassFactory.Create('CalcService', Create_CalcService, TCalcService_Invoker);
我们看还注册了TCalcService_Invoker这个类。
回到CalcLibrary_Invk单元中发现
procedure TCalcService_Invoker.Invoke_Sum(const __Instance:IInterface; const __Message:IROMessage; const __Transport:IROTransport);
{ function Sum(const A: Integer; const B: Integer): Integer; }
var
A: Integer;
B: Integer;
Result: Integer;
begin
try
__Message.Read('A', TypeInfo(Integer), A, []);
**//****把客户端发送的****Message****消息包拆包，赋给已定义的变量。**
__Message.Read('B', TypeInfo(Integer), B, []);
**Result := (__Instance as CalcService).Sum(A, B); //****调用**
__Message.Initialize(__Transport, 'CalcLibrary', 'CalcService', 'SumResponse');
__Message.Write('Result', TypeInfo(Integer), Result, []);
//把服务器端的结果打包。标记'Result',写入值。发送客户端。
__Message.Finalize;
finally
end;
end;
它调用了真正的业务对象，现在大概清楚它的工作原理了吧，让我们继续追踪它的源码。
首先创建TROBINMessage类。
constructor TROBINMessage.Create(aOwner: TComponent);
begin
inherited;
UseCompression := TRUE;
end;
前面说过，RO支持SOAP,和bin两种数据格式：它们的父类都是TROMessage
RO的消息处理的技术是非常值得学习的，它的模型比较先进。我会仔细分析的。
先到父类里看看：
constructor TROMessage.Create(aOwner : TComponent);
begin
inherited Create(aOwner);
InitObject;
end;
看看InitObject
procedure TROMessage.InitObject;
begin
fSerializer := CreateSerializer;
fClientID := NewGuid();
//CreateGUID(fClientID);
end;
先停下来仔细研究ROMessage,这个类非常重要。
TROMessage = class(TComponent, IUnknown, IROMessage, IROMessageCloneable, IROModuleInfo)
private
fSerializer : TROSerializer;
fMessageName,
fInterfaceName : string;
fOnReadFromStream: TStreamOperation;
fOnWriteToStream: TStreamOperation;
fOnServerException: TExceptionEvent;
……
fClientID : TGUID;
这里比较重要的TROSerializer, fMessageName, fInterfaceName, fClientID
TStreamOperation = procedure(aStream : TStream) of object;处理TStream
TExceptionEvent = procedure(anException : Exception) of object;处理异常
另外用了GUID做标识。
function TROMessage.Clone: IROMessage;克隆一个RoMessage
function TROMessage.Clone: IROMessage;
begin
result := TROMessageClass(ClassType).CreateRefCountedClone(self) as IROMessage;
end;
constructor TROMessage.CreateRefCountedClone(iMessage: TROMessage);
begin
Create(); //调用构造函数
Assign(iMessage);   //赋值
fReferenceCounted := true;
end;
现在看看TROSerializer，这是TROMessage最重要的部分：
里面主要定义了一些虚方法。这个一个序列化的类。主要功能是序列化。COM中也叫散集和列集。说穿了就是数据怎么序列化到一个流中，或者逆操作。我们来仔细研究研究。个人觉得这里面的东西很值得研究。搞清楚序列化，对于我们灵活的使用RO将有非常大的帮助。
procedure TROSerializer.BeginReadObject(const aName: string;
aClass : TClass; var anObject: TObject; var LevelRef : IUnknown; var IsValidType : boolean; ArrayElementId : integer = -1);
begin
IsValidType := aClass.InheritsFrom(TROComplexType)
end;
//判断是不是RO定义的TROComplexType（合成类型的类）派生出来的子类。
//序列化(读)
procedure TROSerializer.Read(const aName: string; aTypeInfo: PTypeInfo;
var Ptr; ArrayElementId : integer = -1);
begin
case aTypeInfo^.Kind of
tkClass    : ReadObject(aName, GetTypeData(aTypeInfo).ClassType, Ptr, ArrayElementId);
else RaiseError(err_TypeNotSupported, [GetEnumName(TypeInfo(TTypeKind), Ord(aTypeInfo^.Kind))]);
end;
end;
其它类型就不研究了，大家估计都用过。我自己开发的一款类型的O/Pmaping中也用到了相应的技术，可以通过设置VO之间的映射，把两个VO加入明细VO类中，然后通过RTTL信息解析出SQL语句(select, insert,update,delete)（分析）。，有兴趣的兄弟可以跟我联系。我的例子如下：
/------------------------------------------------------
例如：
Tuser = (TdataTransferObject)
A: TA;
B: TB;
End;
用法：
rdmDS.select(TUser, ds)返回一个数据集
/-------------------------------------------------------
继续看看ReadObject怎么处理的：
procedure TROSerializer.ReadObject(const aName: string; aClass : TClass; var Ref; ArrayElementId : integer = -1);
var obj : TObject absolute Ref;
props : PPropList;
cnt, i : integer;
LevelRef : IUnknown;
validtype : boolean;
// Temporary variables
int64val : int64;
intval : integer;
enuval : byte;
dblval : double;
//extval : extended;
strval : string;
{$IFNDEF DELPHI5}wstrval : widestring;{$ENDIF}
objval : TObject;
begin
obj := nil; { no matter what's passed in, we wanna start fresh }
BeginReadObject(aName, aClass, obj, levelref, validtype, ArrayElementId);
procedure TROSerializer.BeginReadObject(const aName: string;
aClass : TClass; var anObject: TObject; var LevelRef : IUnknown; var IsValidType : boolean; ArrayElementId : integer = -1);
begin
IsValidType := aClass.InheritsFrom(TROComplexType)
end;
在序列化（读）一个对象时，首先判断是不是从TROComplexType派生的。

if Assigned(obj) and (not validtype) then RaiseError(err_TypeNotSupported, [obj.ClassName]);   //**如果不是从****TROComplexType****派生，则抛出一个异常！**
**这里大家应该清楚了，如果想序列化一个对象，必须从****TROComplexType****派生才可以。**
**清楚了这点，在三层中传递对象的问题，大家应该知道怎么解决了吧。**
**TROComplexType = class(TPersistent)**
**private**
**fFieldCount : integer;**
**fFieldList : PPropList;**
**序列化从****Tpersistent****派生出来****({m+}{m-}****这里是关键****)****，这里****RO****还加上了****RTTL****信息。**
if Assigned(obj) and (obj.ClassInfo<> nil) then begin
cnt := GetTypeData(obj.ClassInfo).PropCount;
**//GetTypeData****函数获得类的属性数量。**
if (cnt>0) then begin
GetMem(props, cnt*SizeOf(PPropInfo));**//****分配存放属性信息的空间**
try
cnt := GetPropList(PTypeInfo(obj.ClassInfo), tkProperties, props);
**//****GetPropList****传入类的****TTypeInfo****指针和****TPropList****的指针，它为****PropList****分配一块内存后把该内存填充为指向****TPropInfo****的指针数组，最后返回属性的数量。**
for i := 0 to (cnt-1) do begin
with props^[i]^ do begin
。。。。。。。。。。。。。。
tkInteger : begin
ReadInteger(Name, GetTypeData(PropType^).OrdType, intval);**//****虚函数：调用子类方法****(****从客户端发送的一个流中****)****例：**
**procedure TROStreamSerializer.ReadInteger(const aName: string;**
**anOrdType: TOrdType; var Ref; ArrayElementId : integer = -1);**
**var sze : byte;**
**src : pointer;**
**begin**
**src := @Ref;**
**。。。。。**
**fStream.Read(src^, sze);**
**end;**
SetOrdProp(obj, Name, intval);**//****把属性信息写入对象中。**
end;
。。。。。。。。。。。
tkClass : begin
ReadObject(Name, GetTypeData(PropType^).ClassType, objval);**//****递归**
SetObjectProp(obj, Name, objval);**//****把属性（对象）写入对象中。**
end;
…………..
CustomReadObject(aName, aClass, obj, ArrayElementId);
EndReadObject(aName, aClass, obj, levelref);
end;
**//****把****TROArray****类型的数据（序列化）**
procedure TROSerializer.CustomWriteObject(const aName: string; aClass : TClass; const Ref; ArrayElementId : integer = -1);
var obj : TObject absolute Ref;
i : integer;
itemref : pointer;
begin
if Assigned(obj) then begin
if (obj is TROArray) then with TROArray(obj) do begin
if (GetItemClass<>NIL) then begin
for i := 0 to (Count-1) do begin
itemref := GetItemRef(i);
Write(ArrayItemName, GetItemType, itemref, i);
end;
end
else begin
for i := 0 to (Count-1) do begin
itemref := GetItemRef(i);
Write(ArrayItemName, GetItemType, itemref^, i);
end;
end;
end;
end;
end;
**//****再回到****CreateSerializer**
procedure TROMessage.InitObject;
begin
fSerializer := CreateSerializer;
fClientID := NewGuid();
//CreateGUID(fClientID);
end;
function TROBINMessage.CreateSerializer : TROSerializer;
begin
result := TROStreamSerializer.Create(NIL);
end;
**//****真正工作的类是****TROStreamSerializer****，具体的数据（序列化）稍后再分析。**
constructor TROIndyTCPServer.Create(aComponent: TComponent);
begin
inherited;
fIndyServer := CreateIndyServer;
fIndyServer.Name := 'InternalIndyServer';
{$IFDEF DELPHI6UP}
fIndyServer.SetSubComponent(True);
{$ENDIF}
end;
跟踪一下TROIndyTCPServer用法：
constructor TROServer.Create(aOwner: TComponent);
begin
inherited;
fDispatchers := GetDispatchersClass.Create(Self);
end;
function TROServer.GetDispatchersClass : TROMessageDispatchersClass;
begin
result := TROMessageDispatchers; // Default
end;
constructor TROMessageDispatchers.Create(aServer : TROServer);
begin
inherited Create(GetDispatcherClass);
fServer := aServer;
end;
function TROIndyTCPServer.CreateIndyServer: TIdTCPServer;
begin
result := TROTIdTCPServer.Create(Self);
result.OnExecute := InternalOnExecute;
result.DefaultPort := 8090;
end;
调用：
constructor TIdTCPServer.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FBindings := TIdSocketHandles.Create(Self);
// Before Command handlers
FReplyTexts := TIdRFCReplies.Create(Self);
FCommandHandlers := TIdCommandHandlers.Create(Self);
FCommandHandlersEnabled := IdCommandHandlersEnabledDefault;
FGreeting := TIdRFCReply.Create(nil);
FMaxConnectionReply := TIdRFCReply.Create(nil);
FThreads := TThreadList.Create;
FThreadClass := TIdPeerThread;
FReplyUnknownCommand := TIdRFCReply.Create(nil);
//
FTerminateWaitTime := 5000;
FListenQueue := IdListenQueueDefault;
//TODO: When reestablished, use a sleeping thread instead
// fSessionTimer := TTimer.Create(self);
end;
//继续：
constructor TIdComponent.Create(axOwner: TComponent);
begin
inherited Create(axOwner);
GStackCriticalSection.Acquire; try
Inc(GInstanceCount);
if GInstanceCount = 1 then begin
GStack := TIdStack.CreateStack;
end;
finally GStackCriticalSection.Release; end;
end;
class function TIdStack.CreateStack: TIdStack;
begin
Result := GStackClass.Create;
end;
constructor TIdStackWindows.Create;
var
sData: TWSAData;
begin
inherited Create;
if not GStarted then
begin
if WSAStartup($202, sData) = SOCKET_ERROR then begin
raise EIdStackInitializationFailed.Create(RSWinsockInitializationError);
end;
GStarted := True;
end;
end;
//可以看出真正进行SOCKET通过的部分应该是TidSocketHandle,而TidSocketHandle又调用了TidStack的派生类TidStackWindows来完成真正的底层通信工作的。
TidStack里面的方法都是抽象的，说明具体的通信细节是由派生类实现的。这样的好处是什么，底层通信的无关性。我觉得这是非常重要的部分，仔细阅读RO的源码中，发现了类似非常多的应用，正是这种良好的设计，使RO的通信部分与业务逻辑部分彻底分离。同时支持多种通信协议。
constructor TROIndyTCPServer.Create(aComponent: TComponent);
begin
inherited;
fIndyServer := CreateIndyServer;
fIndyServer.Name := 'InternalIndyServer';
fIndyServer.SetSubComponent(True);
end;

classs单元：
function InitInheritedComponent(Instance: TComponent; RootAncestor: TClass): Boolean;
function InitComponent(ClassType: TClass): Boolean;
begin
Result := False;
if (ClassType = TComponent) or (ClassType = RootAncestor) then Exit;
Result := InitComponent(ClassType.ClassParent);
Result := InternalReadComponentRes(ClassType.ClassName, FindResourceHInstance(
FindClassHInstance(ClassType)), Instance) or Result;
end;
var
LocalizeLoading: Boolean;
begin
GlobalNameSpace.BeginWrite; // hold lock across all ancestor loads (performance)
try
LocalizeLoading := (Instance.ComponentState * [csInline, csLoading]) = [];
if LocalizeLoading then BeginGlobalLoading; // push new loadlist onto stack
try
Result := InitComponent(Instance.ClassType);
if LocalizeLoading then NotifyGlobalLoading; // call Loaded
finally
if LocalizeLoading then EndGlobalLoading; // pop loadlist off stack
end;
finally
GlobalNameSpace.EndWrite;
end;
end;
procedure NotifyGlobalLoading;
var
I: Integer;
G: TList;
begin
G := GlobalLoaded; // performance: eliminate repeated trips through TLS lookup
for I := 0 to G.Count - 1 do
TComponent(G[I]).Loaded;
end;
procedure TIdTCPServer.Loaded;
begin
inherited Loaded;
// Active = True must not be performed before all other props are loaded
if Active then begin
FActive := False;
Active := True;
end;
end;
procedure TROServer.Loaded;
begin
inherited;
IntSetActive(FALSE);
Active := fLoadActive;
fDoneAfterLoad := TRUE;
end;

procedure TIdTCPServer.SetActive(AValue: Boolean);
var
i: Integer;
LListenerThread: TIdListenerThread;
begin
if (not (csDesigning in ComponentState)) and (FActive<> AValue)
and (not (csLoading in ComponentState)) then begin
if AValue then begin
// InitializeCommandHandlers must be called only at runtime, and only after streaming
// has occured. This used to be in .Loaded and that worked for forms. It failed
// for dynamically created instances and also for descendant classes.
if not FCommandHandlersInitialized then begin
FCommandHandlersInitialized := True;
InitializeCommandHandlers;
end;
// Set up bindings
if Bindings.Count = 0 then begin
Bindings.Add;
end;
**function TIdSocketHandles.Add: TIdSocketHandle;**
**begin**
**Result := Inherited Add as TIdSocketHandle;**
**Result.Port := DefaultPort;**
**end;**
**constructor TIdSocketHandle.Create(ACollection: TCollection);**
**begin**
**inherited Create(ACollection);**
**Reset;**
**FClientPortMin := 0;**
**FClientPortMax := 0;**
**if assigned(ACollection) then begin**
**Port := TIdSocketHandles(ACollection).DefaultPort;**
**end;**
**end;**

// Set up ThreadMgr
ThreadMgr.ThreadClass := ThreadClass;其不意//TIdPeerThread
function TIdTCPServer.GetThreadMgr: TIdThreadMgr;
begin
if (not (csDesigning in ComponentState)) and (not Assigned(FThreadMgr)) then
begin
// Set up ThreadMgr
ThreadMgr := TIdThreadMgrDefault.Create(Self); //指定TIdTCPServer
FImplicitThreadMgr := true;
end;
Result := FThreadMgr;
end;
constructor TIdThreadMgr.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FActiveThreads := TThreadList.Create;
FThreadPriority := tpNormal;
end;
// Setup IOHandler
if not Assigned(FIOHandler) then begin
IOHandler := TIdServerIOHandlerSocket.Create(self);
constructor TIdComponent.Create(axOwner: TComponent);
begin
inherited Create(axOwner);
GStackCriticalSection.Acquire; try
Inc(GInstanceCount);
if GInstanceCount = 1 then begin
GStack := TIdStack.CreateStack;
end;
finally GStackCriticalSection.Release; end;
end;
FImplicitIOHandler := true;
end;
// Set up listener threads
FListenerThreads := TThreadList.Create;
IOHandler.Init;
for i := 0 to Bindings.Count - 1 do begin
with Bindings[i] do begin
AllocateSocket;

procedure TIdSocketHandle.AllocateSocket(const ASocketType: Integer = Id_SOCK_STREAM;
const AProtocol: Integer = Id_IPPROTO_IP);
begin
// If we are reallocating a socket - close and destroy the old socket handle
CloseSocket;
if HandleAllocated then begin
Reset;
end;
FHandle := GStack.CreateSocketHandle(ASocketType, AProtocol);
FHandleAllocated := True;
end;
TIdStackSocketHandle = TSocket; Integer;
//可以看出真正进行SOCKET通过的部分应该是TidSocketHandle,而TidSocketHandle又调用了Gstack
function TIdStack.CreateSocketHandle(const ASocketType: Integer;
const AProtocol: Integer = Id_IPPROTO_IP): TIdStackSocketHandle;
begin
result := WSSocket(Id_PF_INET, ASocketType, AProtocol);
if result = Id_INVALID_SOCKET then begin
raise EIdInvalidSocket.Create(RSCannotAllocateSocket);
end;
end;
//创建一个SOCKET套接字
function TIdStackWindows.WSSocket(AFamily, AStruct, AProtocol: Integer): TIdStackSocketHandle;
begin
result := Socket(AFamily, AStruct, AProtocol);
end;
if (FReuseSocket = rsTrue) or ((FReuseSocket = rsOSDependent) and (GOSType = otLinux))
then begin
SetSockOpt(Id_SOL_SOCKET, Id_SO_REUSEADDR, PChar(@Id_SO_True), SizeOf(Id_SO_True));
end;
Bind;
Bind的调用如下：
function TIdSocketHandle.TryBind: Boolean;
begin
Result := not GStack.CheckForSocketError(GStack.WSBind(Handle, Id_PF_INET, IP, Port)
, [Id_WSAEADDRINUSE]);
if Result then begin
UpdateBindingLocal;
end;
end;
function TIdStackWindows.WSBind(ASocket: TIdStackSocketHandle;
const AFamily: Integer; const AIP: string;
const APort: Integer): Integer;
var
Addr: TSockAddrIn;
begin
Addr.sin_family := AFamily;
if length(AIP) = 0 then begin
Addr.sin_addr.s_addr := INADDR_ANY;
end else begin
Addr.sin_addr := TInAddr(StringToTInAddr(AIP));
end;
Addr.sin_port := HToNS(APort);
result := Bind(ASocket, @addr, SizeOf(Addr));
end;
侦听：
Listen(FListenQueue);
procedure TIdSocketHandle.Listen(const anQueueCount: integer);
begin
GStack.CheckForSocketError(GStack.WSListen(Handle, anQueueCount));
end;
//这里创建了一个侦听线程,用于accept客户连接。
LListenerThread := TIdListenerThread.Create(Self, Bindings[i]);
constructor TIdListenerThread.Create(AServer: TIdTCPServer; ABinding: TIdSocketHandle);
begin
inherited Create;
FBinding := ABinding;
FServer := AServer;
end;
//加入线程池中。
FListenerThreads.Add(LListenerThread);
procedure TThreadList.Add(Item: Pointer);
begin
LockList;
try
if (Duplicates = dupAccept) or
(FList.IndexOf(Item) = -1) then
FList.Add(Item)
else if Duplicates = dupError then
FList.Error(@SDuplicateItem, Integer(Item));
finally
UnlockList;
end;
end;
//启动线程。
LListenerThread.Start;
end;
end;
end else begin
TerminateListenerThreads;
// Tear down ThreadMgr
try
TerminateAllThreads;
finally
if ImplicitThreadMgr and TIdThreadSafeList(Threads).IsCountLessThan(1) then begin // DONE -oAPR: BUG! Threads still live, Mgr dead ;-(
FreeAndNil(FThreadMgr);
FImplicitThreadMgr := False;
end;
end;//tryf
end;
end;
FActive := AValue;
end;
procedure TIdListenerThread.Run;
var
LIOHandler: TIdIOHandler;
LPeer: TIdTCPServerConnection;
LThread: TIdPeerThread;
begin
try
if Assigned(Server) then begin // This is temporary code just to test one exception
while True do begin
LThread := nil;
LPeer := TIdTCPServerConnection.Create(Server);
**constructor TIdTCPConnection.Create(AOwner: TComponent);**
**begin**
**inherited Create(AOwner);**
**FGreeting := TIdRFCReply.Create(nil);**
**FLastCmdResult := TIdRFCReply.Create(nil);**
**FRecvBuffer := TIdSimpleBuffer.Create;**
**RecvBufferSize := GRecvBufferSizeDefault;**
**FSendBufferSize := GSendBufferSizeDefault;**
**FInputBuffer := TIdManagedBuffer.Create(BufferRemoveNotify);**
**FMaxLineLength := IdMaxLineLengthDefault;**
**end;**

LIOHandler := Server.IOHandler.Accept(Binding.Handle, SELF);
**//****等候客户端连接：**
function TIdSocketHandle.Select(ASocket: TIdStackSocketHandle;
ATimeOut: Integer): boolean;
var
ReadList: TList;
begin
ReadList := TList.Create; try
ReadList.Add(Pointer(ASocket));
Result := GStack.WSSelect(ReadList, nil, nil, ATimeOut) = 1;
TIdAntiFreezeBase.DoProcess(result = false);
finally ReadList.free; end;
end;
//一旦有客户连接跳出循环，处理客户消息。否则一直循环。
if LIOHandler = nil then begin
FreeAndNil(LPeer);
Stop;
Exit;
end
else begin
LThread := TIdPeerThread(Server.ThreadMgr.GetThread);
LThread.FConnection := LPeer;
LThread.FConnection.IOHandler := LIOHandler;
LThread.FConnection.FFreeIOHandlerOnDisconnect := true;
end;
// LastRcvTimeStamp := Now; // Added for session timeout support
// ProcessingTimeout := False;
if (Server.MaxConnections > 0) and // Check MaxConnections
NOT TIdThreadSafeList(Server.Threads).IsCountLessThan(Server.MaxConnections)
then begin
Server.ThreadMgr.ActiveThreads.Remove(LThread);
LPeer.WriteRFCReply(Server.MaxConnectionReply);
LPeer.Disconnect;
FreeAndNil(LThread); // This will free both Thread and Peer.
end else begin
Server.Threads.Add(LThread); //APR
// Start Peer Thread
LThread.Start;
Break;
end;
end;
end;
except
on E: Exception do begin
if Assigned(LThread) then
FreeAndNil(LThread);
Server.DoListenException(Self, E);
end;
end;
End;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//客户端分析
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
现在来分析分析客户端代码：ROTCPClient先看这个工程：
constructor TROBINMessage.Create(aOwner: TComponent);
begin
inherited;
UseCompression := TRUE;
end;
服务器端分析过，这里就不分析了。
constructor TROIndyTCPChannel.Create(aOwner: TComponent);
begin
inherited;
fIndyClient := CreateIndyClient;
fIndyClient.Name := 'InternalIndyClient';
{$IFDEF DELPHI6UP}
fIndyClient.SetSubComponent(TRUE);
{$ENDIF}
end;

constructor TROBaseConnection.Create(aOwner: TComponent);
{ Creates an object of type TROBaseConnection, and initializes properties. }
begin
inherited Create(aOwner);
{$IFDEF RemObjects_UseEncryption}
fEncryption := TROEncryption.Create();//(nil);
{$ENDIF}
//fEncryption.SetSubComponent(true);
//fInternalEncryption := true;
{ ToDo -cCDK: Add your initialization code here. }
end;
function TROIndyTCPChannel.CreateIndyClient: TIdTCPClient;
begin
result := TIdTCPClient.Create(Self);
result.Port := 8090;
result.Host := '127.0.0.1';
end;
constructor TIdTCPClient.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FBoundPort := BoundPortDefault;
end;
constructor TIdComponent.Create(axOwner: TComponent);
begin
inherited Create(axOwner);
GStackCriticalSection.Acquire; try
Inc(GInstanceCount);
if GInstanceCount = 1 then begin
GStack := TIdStack.CreateStack;
end;
finally GStackCriticalSection.Release; end;
end;
接下来看看客户端调用情况：
可以看出来这个时候，还没有连接到服务器上。
procedure TfmMain.btnCalcClick(Sender: TObject);
var
vService:CalcService;
begin
vService:=CoCalcService.Create(ROBINMessage1,ROIndyTCPChannel1);
try
lblResult.Caption:=IntToStr(vService.Sum(StrToInt(edtValue1.Text),StrToInt(edtValue2.Text)));
finally
vService:=Nil;
end;
end;
接下来会详细分析，客户端怎么调用服务器方法。数据如何打包。传输的。
vService:=CoCalcService.Create(ROBINMessage1,ROIndyTCPChannel1);
我们看到创建了一个实例：
CalcService_Intf这个单元，是RO自已生成的：
class function CoCalcService.Create(const aMessage : IROMessage; aTransportChannel : IROTransportChannel) : CalcService;
begin
result := TCalcService_Proxy.Create(aMessage, aTransportChannel);
end;
大家看到，这里调用了一个代理类：也就是“桩”。
RO用的是静态代理的技术。我个人觉得这里应该采用“动态代理”技术。好处我在前面已经说过了。就不多说了，继续。+
TCalcService_Proxy = class(TROProxy, CalcService)
private
protected
function Sum(const A: Integer; const B: Integer): Integer;
function GetServerTime: DateTime;
end;
继承于TROProxy类。
创建的时候用了接口技术。个人认为，这是RO的核心中的核心。正是因为使用了这个接口，好处就是消息处理格式，消息底层处理完完全全分离了。因此，静态代理也变得更加单纯。不用关心怎么去操作不同格式的消息，及消息的底层调用，与底层通信协议无关了。所以这里用HTTP，TCP，UDP不同的组件，就可以实现在业务层根据不动的情况下。多种网络通信的可能性。
中间层服务器可以服务通过这种组件和不同协议的客户端通信，被不同协议的客户端所调用。
constructor TROProxy.Create(const aMessage: IROMessage;
const aTransportChannel: IROTransportChannel);
begin
inherited Create;
fMessage := pointer(aMessage);
fTransportChannel := pointer(aTransportChannel);
end;
IROTransport = interface
['{56FA09B9-FFC8-4432-80E3-BF78E5E7DF33}']
function GetTransportObject : TObject;
end;
IROTransportChannel = interface(IROTransport)
['{EDFA0CF3-3265-46C9-AC5C-14C3CACF2721}']
procedure Dispatch(aRequest, aResponse : TStream);
end;
//我们看到IROTransportChannel很简单，就是分发消息，GetTransportObject这个函数是由接口得到实例。
TROTransportChannel = class(TComponent, IROTransportChannel, IROMetadataReader)
function GetTransportObject : TObject; virtual; abstract;
这个类是TROIndyTCPChannel的父类，可以看到上面的方法是虚方法。
看看真正实现的内容。
function TROIndyTCPChannel.GetTransportObject: TObject;
begin
result := Self;
end;
这里很简单，但必须自己实现这个方法，因为Delphi本身不支持直接从接口转化成实例类。再看看procedure Dispatch(aRequest, aResponse : TStream);
研究一下RO底层消息到底是如何分发的。
procedure Dispatch(aRequest, aResponse : TStream); reintroduce;
procedure IntDispatch(aRequest, aResponse : TStream); virtual; abstract;
procedure TROTransportChannel.Dispatch(aRequest, aResponse : TStream);
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
{$IFDEF REMOBJECTS_UseEncryption}
if Encryption.EncryptionMethod<> tetNone then begin
EncRequest:= TMemoryStream.Create;
EncResponse := TMemoryStream.Create;
try
DoEncryption(aRequest,EncRequest);   //进行加密
IntDispatch(encRequest, encResponse);
DoDecryption(EncResponse,aResponse); //进行解密
finally
EncRequest.Free;
EncResponse.free;
end;
end
else
{$ENDIF}
begin
IntDispatch(aRequest, aResponse);
end;
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
end;
procedure TROIndyTCPChannel.IntDispatch(aRequest, aResponse: TStream);
begin
try
if not IndyClient.Connected
then IndyClient.Connect;
IndyClient.WriteStream(aRequest, TRUE, TRUE);
IndyClient.ReadStream(aResponse);
finally
if not KeepAlive
then IndyClient.Disconnect;
end;
end;
//是不是有点失望，它的消息分发太简单了。这就是抽象编程的优点。分发只管分发消息。打包由其它的类实现。这里远程调用被抽象成了：‘一个aRequest，一个aResponse’。是不是有点象B/S架构中的HTTP请求。
再看看前面的代码：
TROProxy = class(TInterfacedObject, IUnknown)
property __Message : IROMessage read GetMessage;
具体实现
function TCalcService_Proxy.Sum(const A: Integer; const B: Integer): Integer;
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
__Message.Write('A', TypeInfo(Integer), A, []);
__Message.Write('B', TypeInfo(Integer), B, []);
__Message.Finalize;
__Message.WriteToStream(__request);
__TransportChannel.Dispatch(__request, __response);
__Message.ReadFromStream(__response);
__Message.Read('Result', TypeInfo(Integer), result, []);
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
这里总结一下：
TROTransportChannel.Dispatch用来处理底层的数据包是否加密解密。
派生的子类TROIndyTCPChannel，借助具体的通信实现类TIdTCPClient完成数据的收发。数据的打包借助TROBINMessage, SOAPMessage类，来完成具体的打包细节，因为用了接口，所以数据的打包变得很灵活可以是SOAP消息，或者BIN消息。
具体的通信细节则隐藏在了TROIndyTCPChannel控件中，不同的控件，就实现了不同的通信细节，比如TROIndyUDPChannel，具体的返回服务器参数的问题则由静态代理配合Message类来完成：如TCalcService_Proxy和TROBINMessage类。
因此，中间层通过TROIndyTCPServer ,TROIndyUDPServer, TROIndyHTTPServer等控件。就可以分别和不同的客户TROIndyTCPChannel, TROIndyUDPChannel进行通信了。
可能有人又不理解为什么，再看看
__Message.WriteToStream(__request);
__TransportChannel.Dispatch(__request, __response);
__Message.ReadFromStream(__response);
是不是豁然开朗了呢？
业务对象–>静态代理
静态代理借助Message接口，TransportChannel接口巧妙地把具体的消息打包的消息收发隐藏在代理之后。
再看看代理类：
class function CoCalcService.Create(const aMessage : IROMessage; aTransportChannel : IROTransportChannel) : CalcService;
begin
result := TCalcService_Proxy.Create(aMessage, aTransportChannel);
end;
由此可见，传进去的message，aTransportChannel接口不同，由此消息打包的底层的通信就彻底发生了变成。而丝毫不影响业务对象本身，而中间层只需要支持多种Message及通信类就行，这是设计模式中的桥式结构。接口的妙用，体现的沐漓尽致
------------------------------------------------------------------------------------------------------------
|                                             |
TROServer                                  TROTransportChannel
|                                             |
TCP，UDP，HTTP                              TCP，UDP，HTTP
|                                              |
--------------------------------------------------------------------------------------------------
bin , soap                                      bin , soap
|                                             |
TROMessage                                   TROMessage
|                                               |
------------------------------------------------------------------------------------------------------
业务对象
//具体的加密解密在后面详细分析。现在总结一下静态代理底层到底做了什么：
TROProxy = class(TInterfacedObject, IUnknown)
private
fMessage,//通过接口解析各种不同格式消息
fTransportChannel : pointer;//通过接口处理不同的协议通信
fInterfaceName : string;//处理的接口名称
….
end;
所以静态代理的工作原理就是，利用fMessage解析，打包数据利用fTransportChannel传输，接收服务器端数据。

继续追踪源代码：
看看具体怎么调用的：
lblResult.Caption:=IntToStr(vService.Sum(StrToInt(edtValue1.Text),StrToInt(edtValue2.Text)));
这里是关键，我们好好看看RO是怎么打包数据，传输数据。然后接收服务器端数据，解析出数据的，代码如下：
function TCalcService_Proxy.Sum(const A: Integer; const B: Integer): Integer;
var __request, __response : TMemoryStream;
__http : IROHTTPTransport;
begin
__request := TMemoryStream.Create;//请求
__response := TMemoryStream.Create;//响应
try
__Message.Initialize(__TransportChannel, 'NewLibrary', CalcService_DefaultURN, 'Sum');//初始化数据：
if Supports(__TransportChannel, IROHTTPTransport, __http) then begin
__http.Headers['SOAPAction'] := '"urn:CalcLibrary-CalcService\#Sum"';
if (__http.TargetURL='') then __http.TargetURL := CalcService_EndPointURI;
//处理HTTP协议特有的数据包头，判断是否支持HTTP接口。这里不支持，因为这里是TCP协议。
end;
__Message.Write('A', TypeInfo(Integer), A, []);//打包数据（参数1）
__Message.Write('B', TypeInfo(Integer), B, []);//打包数据（参数2）
__Message.Finalize;
__Message.WriteToStream(__request);//把数据打包入流中。
__TransportChannel.Dispatch(__request, __response);//向服务器端发送数据包，并返回服务器端数据包。
__Message.ReadFromStream(__response);//解析服务器端数据包
__Message.Read('Result', TypeInfo(Integer), result, []);//读出数据包中数据，写入返回值中。
finally
__request.Free;
__response.Free;
end
end;
现在看这段代码，估计不难了吧。我们不得不惊叹RO设计的完美，简单。
这里的抽象编程运用的沐漓尽致。

下面来仔细分析一下具体工作流程：
//数据包初始化过程：
procedure TROBINMessage.Initialize(const aTransport : IROTransport; const anInterfaceName, aMessageName : string);
begin
inherited;
procedure TROMessage.Initialize(const aTransport: IROTransport; const aLibraryName, anInterfaceName, aMessageName: string);
begin
Initialize(aTransport, anInterfaceName, aMessageName);
end;
把TROIndyTCPChannel，接口，及调用函数传下去。
procedure TROMessage.Initialize(const aTransport : IROTransport; const anInterfaceName, aMessageName: string);
begin
fInterfaceName := anInterfaceName;
fMessageName := aMessageName;
end;
SetHTTPInfo(aTransport, dfBinary);
//是否使用HTTP协议传输数据：如果是则把相应信息设置。
procedure SetHTTPInfo(const aTransport : IROTransport; aDataFormat : TDataFormat);
var http : IROHTTPTransport;
begin
{$IFDEF DOTNET}
{$ELSE}
if Supports(aTransport, IROHTTPTransport, http) then begin
http.ContentType := DataFormatStrs[aDataFormat];
http.UserAgent := str_ProductName;
end;
{$ENDIF DOTNET}
end;
if fDestroyStream then FreeAndNIL(fStream);
fStream := TMemoryStream.Create;
fDestroyStream := TRUE;
//创建一个TmemoryStream对象。准备打包数据。
TROMessage = class(TComponent, IUnknown, IROMessage, IROMessageCloneable, IROModuleInfo)
private
fSerializer : TROSerializer;
我们看到ROMessage通过序列化对象，对数据进行序列化操作，写入流中。
TROStreamSerializer派生与TROSerializer,在前面已经对序列化进行了详细的介绍，请参考前面内容。
(Serializer as TROStreamSerializer).SetStorageRef(fStream); // Very important!
//注意：Bin数据时
procedure TROStreamSerializer.SetStorageRef(aStorageRef: TStream);
begin
//result := TObject(aStorageRef) is TStream;
//if result then fStream := TStream(aStorageRef);
fStream := aStorageRef;
end;
//SOAP数据时：
TROSOAPMessage
(Serializer as TROXMLSerializer).SetStorageRef(pointer(fMessageNode));
procedure TROXMLSerializer.SetStorageRef(aStorageRef : pointer);
begin
fNode := nil;
if Supports(IUnknown(aStorageRef), IXMLNode) then begin
//RaiseError('TROXMLSerializer: Not a valid IXMLNode reference',[]);
fBodyNode := NIL;
fRespNode := NIL;
fMaxRef := 0;
fNode := IXMLNode(aStorageRef);
end;
{fNode := aStorageRef;}
end;
MessageName := aMessageName;
InterfaceName := anInterfaceName;
Stream_WriteStringWithLength(fStream,InterfaceName);
procedure Stream_WriteStringWithLength(iStream:TStream; const iString: string);
var lLength:integer;
{$IFDEF DOTNET}
lBuffer:array of byte;
i:integer;
{$ENDIF DOTNET}
begin
lLength := Length(iString);
iStream.Write(lLength, SizeOf(lLength)); //往流中写入长度信息：
if (lLength > 0) then begin
{$IFDEF DOTNET}
SetLength(lBuffer, lLength);
for i := 0 to lLength-1 do lBuffer[i] := ord(iString[i+1]);
iStream.Write(lBuffer, lLength);
{$ELSE}
iStream.Write(iString[1], lLength);
{$ENDIF}
end;
//ToDo: find a save and FAST way to do this in .NET
end;
Stream_WriteStringWithLength(fStream,MessageName);
end;
从上看出，binMessage首先把接口名称和消息名称写出了流中。
__Message.Write('A', TypeInfo(Integer), A, []);
把参数A写入流中：
procedure TROMessage.Write(const aName: string; aTypeInfo: PTypeInfo;
const Ptr; Attributes: TParamAttributes);
begin
Serializer.Write(aName, aTypeInfo, Ptr);
end;
继续：
procedure TROSerializer.Write(const aName: string; aTypeInfo: PTypeInfo;
const Ref; ArrayElementId : integer = -1);
begin
case aTypeInfo^.Kind of
tkEnumeration : WriteEnumerated(aName, aTypeInfo, Ref);
tkInteger     : WriteInteger(aName, GetTypeData(aTypeInfo)^.OrdType, Ref);
tkFloat       : if (aTypeInfo=TypeInfo(TDateTime))
then WriteDateTime(aName, Ref)
else WriteFloat(aName, GetTypeData(aTypeInfo)^.FloatType, Ref);
tkWString     : WriteWideString(aName, Ref);
tkLString,
tkString      : WriteString(aName, Ref);
tkInt64       : WriteInt64(aName, Ref);
tkClass       : WriteObject(aName, GetTypeData(aTypeInfo).ClassType, Ref, ArrayElementId);
else RaiseError(err_TypeNotSupported, [GetEnumName(TypeInfo(TTypeKind), Ord(aTypeInfo^.Kind))]);
end;
end;
整型数据：
procedure TROStreamSerializer.WriteInteger(const aName: string;
anOrdType: TOrdType; const Ref; ArrayElementId : integer = -1);
var sze : byte;
src : pointer;
begin
src := @Ref;
sze := 0;
case anOrdType of
otSByte,
otUByte : sze := SizeOf(byte);
otSWord,
otUWord : sze := SizeOf(word);
otSLong,
otULong : sze := SizeOf(integer);
end;
fStream.Write(src^, sze);   //fStream由TROBINMessage初始化时创建的。
end;

继续：
__Message.Finalize;
procedure TROMessage.Finalize;
begin
//FreeAndNIL(fSerializer);
end;
继续回来业务方法中：
__Message.WriteToStream(__request);
注意：这里是对整个客户端的请求消息进行打包后的数据流。
__TransportChannel.Dispatch(__request, __response);
__Message.ReadFromStream(__response);
__Message.Read('Result', TypeInfo(Integer), result, []);
procedure TROBINMessage.WriteToStream(aStream: TStream);
begin
WriteStream(typMessage, fStream, aStream);
注意：aStream：是request数据流.    fStream：TROStreamSerializer处理参数后的数据流。
inherited;
end;
//这里要注意：
fStream是内部的一个Tsteam包括了接口信息，消息信息，及参数数据。
而aStream是要得到的binMessage的整个数据流（包括头信息，客户ID等）
请仔细看下面的分析：这里说明一下，为什么要这样设计，其实道理比较简单。
Fstream仅仅写了了相当的调用信息，而__request流则附加了标志，头等信息，这样分离设计是有好处的。便于系统维护。灵活改变数据结构，而减少整个程序的代码修改。
真正的参数打包部分交给了TROStreamSerializer来处理。TROBINMessage.WriteStream则把头信息，ID等加入整个数据流中。
procedure TROBINMessage.WriteStream(aMessageType: TMessageType; Source, Destination : TStream);
var start : cardinal;
lHeader : TBINHeader;
begin
//创建一个头信息
lHeader := TBINHeader.Create();
constructor TBINHeader.Create;
begin
inherited Create();
fHeader[0] := Ord(BINSignature[0]);
fHeader[1] := Ord(BINSignature[1]);
fHeader[2] := Ord(BINSignature[2]);
fHeader[3] := Ord(BINSignature[3]);
fHeader[4] := Ord(BINSignature[4]);
end;
头首部标志为‘RO107’
try
lHeader.Compressed := UseCompression;
procedure TBINHeader.SetCompressed(const Value: boolean);
begin
if Value then
fHeader[OFFSET_MESSAGE_FLAGS] := fHeader[OFFSET_MESSAGE_FLAGS] or BINMESSAGE_FLAG_COMPRESSED
else
fHeader[OFFSET_MESSAGE_FLAGS] := fHeader[OFFSET_MESSAGE_FLAGS] and not BINMESSAGE_FLAG_COMPRESSED;
end;
OFFSET_MESSAGE_FLAGS值为5，
lHeader.MessageType := aMessageType;
procedure TBINHeader.SetMessageType(const Value: TMessageType);
begin
fHeader[OFFSET_MESSAGE_TYPE] := byte(Value);
end;
OFFSET_MESSAGE_FLAGS值为6，
lHeader.ClientID := GetClientID;
procedure TBINHeader.SetClientID(const Value: TGUID);
begin
Move(Value, fHeader[OFFSET_CLIENTID], SizeOf(TGUID));
end;OFFSET_CLIENTID=12
lHeader.WriteToStream(Destination);
//写入BinHeader头信息：
procedure TBINHeader.WriteToStream(iStream: TStream);
begin
iStream.Write(fHeader, SizeOf(fHeader));
end;

//写入__request的数据信息
Source.Position := 0;
if UseCompression then begin
start := GetTickCount;
CompressStream(Source, Destination);
附加的数据流(合并数据)，把头信息，和参数打包信息合并。
procedure CompressStream(anInputStream, aCompressedStream : TStream);
var zstream : TCompressionStream;
begin
try
zstream := TCompressionStream.Create(clMax, aCompressedStream);
zstream.CopyFrom(anInputStream, 0);
finally
FreeAndNIL(zstream);
end;
end;

继续：
procedure TROMessage.WriteToStream(aStream: TStream);
begin
if Assigned(fOnWriteToStream) then begin
aStream.Position := 0; // Just in case
fOnWriteToStream(aStream);
end;
aStream.Position := 0; // Just in case
end;
继续回到SUM方法中：

__TransportChannel.Dispatch(__request, __response);
__Message.ReadFromStream(__response);
继续：
procedure TROTransportChannel.Dispatch(aRequest, aResponse : TStream);
。。。。。。。。。
{$IFDEF REMOBJECTS_UseEncryption}
if Encryption.EncryptionMethod<> tetNone then begin
EncRequest:= TMemoryStream.Create;
EncResponse := TMemoryStream.Create;
try
DoEncryption(aRequest,EncRequest);
IntDispatch(encRequest, encResponse);
DoDecryption(EncResponse,aResponse);
finally
EncRequest.Free;
EncResponse.free;
end;
end
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
对数据流是否首先进行加密，解密。
procedure TROIndyTCPChannel.IntDispatch(aRequest, aResponse: TStream);
begin
try
if not IndyClient.Connected
then IndyClient.Connect;
IndyClient.WriteStream(aRequest, TRUE, TRUE);
IndyClient.ReadStream(aResponse);
finally
if not KeepAlive
then IndyClient.Disconnect;
end;
end;
这里三句，分别是连接服务器端，写入流操作（发送消息给中间层），读取中间层返回的结果。先看看网络通信相关代码。
procedure TIdTCPClient.Connect(const ATimeout: Integer = IdTimeoutDefault);
begin
…………………….
IOHandler := TIdIOHandlerSocket.Create(Self);
…………………..
IOHandler.Open;
procedure TIdIOHandlerSocket.Open;
begin
inherited Open;
if not Assigned(FBinding) then begin
FBinding := TIdSocketHandle.Create(nil);
end
else
FBinding.Reset(true);
end;
创建了一个TidSocketHandle类。前面已经介绍过这个类了。这里就不说了。
ResetConnection;
IOHandler.ConnectClient(Host, Port, BoundIP, BoundPort, BoundPortMin, BoundPortMax, ATimeout);
//-------------------------------------------------ConnectClient---------------------------------------------
procedure TIdIOHandlerSocket.ConnectClient(const AHost: string;
const APort: Integer; const ABoundIP: string; const ABoundPort,
ABoundPortMin, ABoundPortMax: Integer; const ATimeout: Integer = IdTimeoutDefault);
。。。。。。。。。。。。。。。。。。
with Binding do begin
AllocateSocket;
procedure TIdSocketHandle.AllocateSocket(const ASocketType: Integer = Id_SOCK_STREAM;
const AProtocol: Integer = Id_IPPROTO_IP);
。。。。。。
FHandle := GStack.CreateSocketHandle(ASocketType, AProtocol);
。。。。。。。。
end;
获取一个套接字：
function TIdStack.CreateSocketHandle(const ASocketType: Integer;
const AProtocol: Integer = Id_IPPROTO_IP): TIdStackSocketHandle;
begin
result := WSSocket(Id_PF_INET, ASocketType, AProtocol);
if result = Id_INVALID_SOCKET then begin
raise EIdInvalidSocket.Create(RSCannotAllocateSocket);
end;
IP := ABoundIP;
Port := ABoundPort;
ClientPortMin := ABoundPortMin;
ClientPortMax := ABoundPortMax;
Bind;
end;
if not GStack.IsIP(LHost) then begin
DoStatus(hsResolving, [LHost]);
end;
//设置IP及端口地址：
Binding.SetPeer(GStack.ResolveHost(LHost), LPort);
……………..
DoStatus(hsConnecting, [Binding.PeerIP]);
…………………………
GStack.CheckForSocketError(Binding.Connect);
//连接服务器
function TIdSocketHandle.Connect(const AFamily: Integer = Id_PF_INET): Integer;
begin
Result := GStack.WSConnect(Handle, AFamily, PeerIP, PeerPort);
UpdateBindingLocal; //更新IP地址，主机名等信息
UpdateBindingPeer; //更新端口号信息等
……………….
function TIdStackWindows.WSConnect(const ASocket: TIdStackSocketHandle;
const AFamily: Integer; const AIP: string;
const APort: Integer): Integer;
var
Addr: TSockAddrIn;
begin
Addr.sin_family := AFamily;
Addr.sin_addr := TInAddr(StringToTInAddr(AIP));
Addr.sin_port := HToNS(APort);
result := Connect(ASocket, @Addr, SizeOf(Addr));
end;
SocksInfo.MakeSocksConnection(AHost, APort);
//-------------------------------------------ConnectClien结束---------------------------------------------
继续回到
procedure TIdTCPClient.Connect(const ATimeout: Integer = IdTimeoutDefault);
………………..
if Assigned(Intercept) then begin
Intercept.Connect(Self);
end;
DoStatus(hsConnected, [Host]);
DoOnConnected;
except
DisconnectSocket;
raise;
end;
end;
退出该函数。
继续。
procedure TROIndyTCPChannel.IntDispatch(aRequest, aResponse: TStream);
begin
………………………………..
// aRequest为TROTransportChannel传进来的EncRequest:= TMemoryStream.Create;
IndyClient.WriteStream(aRequest, TRUE, TRUE);
IndyClient.ReadStream(aResponse);
//第一句是借助把流发送到服务端。（根据包长度拆分成多个小包）。
//第二句读服务器端流数据。
procedure TIdTCPConnection.WriteStream(AStream: TStream; const AAll: boolean = true;
const AWriteByteCount: Boolean = False; const ASize: Integer = 0);
var
if ASize = 0 then begin
LStreamEnd := AStream.Size;
end else begin
LStreamEnd := ASize + AStream.Position;
end;
LSize := LStreamEnd - AStream.Position;
if AWriteByteCount then begin
WriteInteger(LSize);//写入数据包长度。
end;
BeginWork(wmWrite, LSize); try
LBuffer := TMemoryStream.Create; try//生成临时发生流。
LBuffer.SetSize(FSendBufferSize);
while True do begin
LSize := Min(LStreamEnd - AStream.Position, FSendBufferSize);
if LSize = 0 then begin
Break;
end;
//分批读出数据包。
LSize := AStream.Read(LBuffer.Memory^, LSize);
if LSize = 0 then begin
raise EIdNoDataToRead.Create(RSIdNoDataToRead);
end;
WriteBuffer(LBuffer.Memory^, LSize);
end;
finally FreeAndNil(LBuffer); end;
finally EndWork(wmWrite); end;
end;
WriteBuffer(LBuffer.Memory^, LSize);
procedure TIdTCPConnection.WriteBuffer(const ABuffer; AByteCount: Integer;
const AWriteNow: boolean = false);
var
LBuffer: TIdSimpleBuffer;
nPos, nByteCount: Integer;
begin
//判断是否与服务器端连接，否则调用TIdTCPConnection.Connected
if connected then begin
if (FWriteBuffer = nil) or AWriteNow then begin
LBuffer := TIdSimpleBuffer.Create; try
LBuffer.WriteBuffer(ABuffer, AByteCount);
if Assigned(Intercept) then begin
LBuffer.Position := 0;
Intercept.Send(LBuffer);//发送数据
AByteCount := LBuffer.Size;
end;
nPos := 1;
repeat
nByteCount := IOHandler.Send(PChar(LBuffer.Memory)[nPos - 1], LBuffer.Size - nPos + 1);
function TIdIOHandlerSocket.Send(var ABuf; ALen: integer): integer;
begin
if Connected then
begin
Result := Binding.Send(ABuf, ALen, 0);
end
else begin
raise EIdClosedSocket.Create(RSStatusDisconnected);
end;
end;
function TIdSocketHandle.Send(var Buf; len, flags: Integer): Integer;
begin
result := GStack.WSSend(Handle, Buf, len, flags);
end;
…………………………….
nPos := nPos + nByteCount;
until nPos > AByteCount;
finally FreeAndNil(LBuffer); end;
end else begin
FWriteBuffer.WriteBuffer(ABuffer, AByteCount);
if (FWriteBuffer.Size >= FWriteBufferThreshhold) and (FWriteBufferThreshhold > 0) then begin
FlushWriteBuffer(FWriteBufferThreshhold);
end;
end;
end
else
begin
Raise EIdNotConnected.Create(RSNotConnected);
end;
end;
end;
继续：
读服务器端返回的数据：
procedure TIdTCPConnection.ReadStream(AStream: TStream; AByteCount: Integer = -1;
const AReadUntilDisconnect: Boolean = False);
。。。。。。。。。。。。。。。。。。。。。。。。。。。。
if (AByteCount = -1) and (AReadUntilDisconnect = False) then begin
AByteCount := ReadInteger; //读数据包长度
end;
function TIdTCPConnection.ReadInteger(const AConvert: boolean = true): Integer;
begin
ReadBuffer(Result, SizeOf(Result));
……
end;
继续：
procedure TIdTCPConnection.ReadBuffer(var ABuffer; const AByteCount: Integer);
…………………..
while (InputBuffer.Size<AByteCount) do begin
ReadFromStack;
function TIdTCPConnection.ReadFromStack(const ARaiseExceptionIfDisconnected: Boolean = True;
ATimeout: Integer = IdTimeoutDefault; const ARaiseExceptionOnTimeout: Boolean = True):
…………………………………….
if Connected then begin
LByteCount := 0;
repeat
if IOHandler.Readable(ATimeout) then begin
function TIdSocketHandle.Readable(AMSec: Integer = IdTimeoutDefault): boolean;
………………………………
try
ReadList.Add(Pointer(Handle));
Result := GStack.WSSelect(ReadList, nil, nil, AMSec) = 1;
TIdAntiFreezeBase.DoProcess(result = false);
finally ReadList.free; end;
end;
调用：
TIdStackWindows.WSSelect
function TIdStackWindows.WSSelect(ARead, AWrite, AErrors: TList; ATimeout: Integer):
………………………….
if ATimeout = IdTimeoutInfinite then begin
Result := Select(0, @FDRead, @FDWrite, @FDError, nil);
end else begin
tmTo.tv_sec := ATimeout div 1000;
tmTo.tv_usec := (ATimeout mod 1000) * 1000;
Result := Select(0, @FDRead, @FDWrite, @FDError, @tmTO);
end;
GetFDSet(ARead, FDRead);
GetFDSet(AWrite, FDWrite);
GetFDSet(AErrors, FDError);
end;
/-------------------------------------------服务器端---------------------------------------------------
此时，客户端处于阻塞状态，直到服务器端返回的数据包为止。
现在在看看服务器端如何响应的：
procedure TIdListenerThread.Run;
。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
LThread := TIdPeerThread(Server.ThreadMgr.GetThread);
function TIdThreadMgrDefault.GetThread: TIdThread;
begin
Result := CreateNewThread;
ActiveThreads.Add(result);
end;
这里创建了一个线程TIdPeerThread，加入到线程池中。
LThread.FConnection := LPeer;
LThread.FConnection.IOHandler := LIOHandler;
LThread.FConnection.FFreeIOHandlerOnDisconnect := true;
end;
if (Server.MaxConnections > 0) and // Check MaxConnections
……………………….
Server.Threads.Add(LThread); //APR
LThread.Start;//启动新的线程TidPeerThread，TidListenerThread线程继续listen.
…………………….
End;
下面看看新创建的线程TidPeerThreadj是怎么工作的。
procedure TIdPeerThread.Run;
begin
try
try
if not Connection.Server.DoExecute(Self) then begin
except
。。。。。。。。。。。。。。。。。。。。。。。。
end;
function TIdTCPServer.DoExecute(AThread: TIdPeerThread): boolean;
。。。。。。。。。。。。。。。
if Result then begin
OnExecute(AThread);
end;
end;
end;
继续：
procedure TROIndyTCPServer.InternalOnExecute(AThread: TIdPeerThread);
var req, resp : TStringStream;
tcptransport : IROTCPTransport;
begin
req := TStringStream.Create('');
resp := TStringStream.Create('');
tcptransport := TIndyTCPConnectionTransport.Create(aThread);
try
with AThread do begin
Connection.ReadStream(req);
DispatchMessage(tcptransport, req, resp);
Connection.WriteStream(resp, TRUE, TRUE);
if not KeepAlive
then Connection.Disconnect;
end;
finally
req.Free;
resp.Free;
end;
end;
继续：
procedure TIdTCPConnection.ReadStream(AStream: TStream; AByteCount: Integer = -1;
const AReadUntilDisconnect: Boolean = False);
。。。。。。。。。。。。。。。。。。。。。。。。。。。。
if (AByteCount = -1) and (AReadUntilDisconnect = False) then begin
AByteCount := ReadInteger; //读数据包长度
end;
function TIdTCPConnection.ReadInteger(const AConvert: boolean = true): Integer;
begin
ReadBuffer(Result, SizeOf(Result));
……
end;
继续：
procedure TIdTCPConnection.ReadBuffer(var ABuffer; const AByteCount: Integer);
…………………..
while (InputBuffer.Size<AByteCount) do begin
ReadFromStack;
function TIdTCPConnection.ReadFromStack(const ARaiseExceptionIfDisconnected: Boolean = True;
ATimeout: Integer = IdTimeoutDefault; const ARaiseExceptionOnTimeout: Boolean = True):
…………………………………….
if Connected then begin
LByteCount := 0;
repeat
if IOHandler.Readable(ATimeout) then begin
function TIdSocketHandle.Readable(AMSec: Integer = IdTimeoutDefault): boolean;
………………………………
try
ReadList.Add(Pointer(Handle));
Result := GStack.WSSelect(ReadList, nil, nil, AMSec) = 1;
TIdAntiFreezeBase.DoProcess(result = false);
finally ReadList.free; end;
end;
调用：
TIdStackWindows.WSSelect
function TIdStackWindows.WSSelect(ARead, AWrite, AErrors: TList; ATimeout: Integer):
………………………….
if ATimeout = IdTimeoutInfinite then begin
Result := Select(0, @FDRead, @FDWrite, @FDError, nil);
end else begin
tmTo.tv_sec := ATimeout div 1000;
tmTo.tv_usec := (ATimeout mod 1000) * 1000;
Result := Select(0, @FDRead, @FDWrite, @FDError, @tmTO);
end;
GetFDSet(ARead, FDRead);
GetFDSet(AWrite, FDWrite);
GetFDSet(AErrors, FDError);
end;
继续回到：function TIdTCPConnection.ReadFromStack
LByteCount := IOHandler.Recv(FRecvBuffer.Memory^, FRecvBuffer.Size);
function TIdIOHandlerSocket.Recv(var ABuf; ALen: integer): integer;
begin
if Connected then
begin
Result := Binding.Recv(ABuf, ALen, 0);
end
else begin
raise EIdClosedSocket.Create(RSStatusDisconnected);
end;
end;接收数据。
。。。。。。。。
if LByteCount > 0 then begin
FRecvBuffer.Size := LByteCount;
if Assigned(Intercept) then begin
FRecvBuffer.Position := 0;
Intercept.Receive(FRecvBuffer);
LByteCount := FRecvBuffer.Size;
end;
if ASCIIFilter then begin
for i := 1 to FRecvBuffer.Size do begin
PChar(FRecvBuffer.Memory)[i] := Chr(Ord(PChar(FRecvBuffer.Memory)[i]) and $7F);
end;
end;
FInputBuffer.Seek(0, soFromEnd);
FInputBuffer.WriteBuffer(FRecvBuffer.Memory^, FRecvBuffer.Size);
end;
继续TIdTCPConnection.ReadStream：
。。。。。。。。。。。。。。。。。。。。。。。。。
if InputBuffer.Size > 0 then begin
i := Min(InputBuffer.Size, LWorkCount);
InputBuffer.Position := 0;
AStream.CopyFrom(InputBuffer, i); //拷贝数据
InputBuffer.Remove(i);
Dec(LWorkCount, i);
end;
继续：
procedure TROIndyTCPServer.InternalOnExecute(AThread: TIdPeerThread);
。。。。。。。。。。。。。。。
DispatchMessage(tcptransport, req, resp);
Connection.WriteStream(resp, TRUE, TRUE);
我们看到rep中的数据为：'RO107'\#1\#0\#0\#0\#0\#0\#0'(繩腦w/E?O喿'\#$17'檩x阢f``pN蘄'\#$E'N-*薒Ne'\#6'r僈s晙'\#$14'?'\#0'gO'\#5\#$AD
下面看一个消息分发的机制：
function TROServer.DispatchMessage(const aTransport: IROTransport; aRequeststream, aResponsestream: TStream) : boolean;
………………….
dispatcher := Dispatchers.FindDispatcher(aTransport, aRequeststream);
function TROMessageDispatchers.FindDispatcher(const aTransport : IROTransport;
aRequestStream : TStream): TROMessageDispatcher;
var i : integer;
begin
result := NIL;
for i := 0 to (Count-1) do
if Dispatchers[i].CanHandleMessage(aTransport, aRequestStream) then begin
result := Dispatchers[i];
Break;
end;
end;
………………….
TriggerReadFromStream(aRequeststream);
result := dispatcher.ProcessMessage(aTransport, aRequeststream, aResponsestream);function TROMessageDispatcher.ProcessMessage(const aTransport: IROTransport; aRequeststream, aResponsestream: TStream) : boolean;
var lMessage:IROMessage;
begin
lMessage := (MessageIntf as IROMessageCloneable).Clone();
result := MainProcessMessage(lMessage, aTransport, aRequeststream, aResponsestream);
end;
这里是比较重要的实现：
function MainProcessMessage(const aMessage : IROMessage; const aTransport : IROTransport; aRequestStream, aResponseStream : TStream) : boolean;
…………………
aMessage.ReadFromStream(aRequestStream);
。。。。
factory := FindClassFactory(aMessage.InterfaceName);
//查找相应的工厂注册信息，准备调用。
result := factory.Invoker.HandleMessage(factory, aMessage, aTransport);
function TROClassFactory.GetInvoker: IROInvoker;
begin
if (fInvoker=NIL)
then fInvoker := fInvokerClass.Create; // Only creates it when really needed
result := fInvoker;
end;

function TROInvoker.CustomHandleMessage(const aFactory : IROClassFactory;
const aMessage: IROMessage;
const aTransport : IROTransport) : boolean;
var mtd : TMessageInvokeMethod;
instance : IInterface;
begin
。。。。
@mtd := MethodAddress('Invoke_'+aMessage.MessageName);
if (@mtd<>NIL) then try
try
aFactory.CreateInstance(aMessage.ClientID, instance);
//这里CALL了静态代理类：
procedure Create_CalcService(out anInstance : IUnknown);
begin
anInstance := TCalcService.Create(NIL);
end;

BeforeInvoke(mtd, instance, aFactory, aMessage, aTransport);
mtd(instance, aMessage, aTransport);
//实际的调用：
procedure TCalcService_Invoker.Invoke_Sum(const __Instance:IInterface; const __Message:IROMessage; const __Transport:IROTransport);
{ function Sum(const A: Integer; const B: Integer): Integer; }
var
A: Integer;
B: Integer;
Result: Integer;
begin
try
__Message.Read('A', TypeInfo(Integer), A, []);
__Message.Read('B', TypeInfo(Integer), B, []);
Result := (__Instance as CalcService).Sum(A, B);
__Message.Initialize(__Transport, 'CalcLibrary', 'CalcService', 'SumResponse');
__Message.Write('Result', TypeInfo(Integer), Result, []);
__Message.Finalize;
finally
end;
end;
AfterInvoke(mtd, instance, aFactory, aMessage, aTransport, NIL);
…………………
再看看如何把数据打包返回给客户端的：
aMessage.WriteToStream(aResponseStream);
procedure TROBINMessage.WriteToStream(aStream: TStream);
begin
WriteStream(typMessage, fStream, aStream);
inherited;
end;
…………………………
继续：
procedure TIdTCPConnection.WriteStream(AStream: TStream; const AAll: boolean = true;
const AWriteByteCount: Boolean = False; const ASize: Integer = 0);




客户端：
function TCalcService_Proxy.Sum(const A: Integer; const B: Integer): Integer;
。。。。
__TransportChannel.Dispatch(__request, __response);
__Message.ReadFromStream(__response);
__Message.Read('Result', TypeInfo(Integer), result, []);
。。。。
procedure TROStreamSerializer.ReadInteger(const aName: string;
anOrdType: TOrdType; var Ref; ArrayElementId : integer = -1);
var sze : byte;
src : pointer;
begin
src := @Ref;
sze := 0;
case anOrdType of
otSByte,
otUByte : sze := SizeOf(byte);
otSWord,
otUWord : sze := SizeOf(word);
otSLong,
otULong : sze := SizeOf(integer);
end;
fStream.Read(src^, sze);
end;




