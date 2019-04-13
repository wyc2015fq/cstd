
# 远程调用技术代码追踪(webservice) - .NET博文收藏 - CSDN博客


2008年06月10日 12:52:00[hejishan](https://me.csdn.net/hejishan)阅读数：2526


最近阅读了SocketConn的源码和WebService 的源码，把追踪的过程写了下来，方便大家学习。毕竟这需要精力，时间和毅力。感谢煮茶待英雄博志区和三层数据库讨论区兄弟们的支持，特别是julian兄弟，不是他，我可能没耐心继续下去。如果有时间，大家可以继续完善。从socket和Websevice的底层实现细节，我们发现BORLAND的工程师们的构思和实现的过程。我觉得这对我们的学习应该是非常重要的。学会思考。学会读源码，学会分析。
希望和我交往的朋友可通过QQ或Email联系我。Wu_yanan2003@yahoo.com.cn
另见：《远程调用技术代码追踪(socket) 》
关注我的：《远程调用技术代码追踪(第三方控件) 》

远程调用技术内幕
有关WebService的相关的知识，我就不说了，我直接分析源码。有问题的地方请参考李维的书。
initialization
InvRegistry.RegisterInterface(TypeInfo(IMyFirstWS), 'urn:MyFirstWSIntf-IMyFirstWS', 'utf-8');
看过李维的分布式架构的应该都知道，WEB服务端对类和接口进行了注册，客户端这里也进行了注册。然后客户端把数据通过HTTP传输到服务器端，服务器端通过拆包，去到注册管理的类中寻找相应的接口，并创建一个相应的对象，把客户端的数据压进去，调用后，把数据再传回来。
在调用这句的时候，TinvokableClassRegistry类已经创建了，由于inquire_v1也引用了InvRegistry注册，所以在哪里被引用的时候已经被创建了。
function InvRegistry: TInvokableClassRegistry;
begin
if not Assigned(InvRegistryV) then
InitIR;
Result := InvRegistryV;
end;
初次引用会调用InitIR方法。
procedure InitIR;
begin
InvRegistryV := TInvokableClassRegistry.Create;
RemTypeRegistryV := TRemotableClassRegistry.Create;
RemClassRegistryV:= RemTypeRegistry;
InitBuiltIns; //定们到这一句：
InitXSTypes;
InitMoreBuiltIns;
end;
先看InvRegistryV := TInvokableClassRegistry.Create;，这个类是用来注册，相应的接口及类，
并能够根据soap封包内容找到相应的接口及方法。
TRemotableClassRegistry= TRemotableTypeRegistry;
所对应的是TremotableTypeRegistry, 这个类主要是对数据类型进行注册。
大致来了解一下这个类。
TInvokableClassRegistry = class(TInterfacedObject)
private
FLock: TRTLCriticalSection;
FRegClasses: array of InvRegClassEntry;
FRegIntfs: array of InvRegIntfEntry;
这里可以看到，声明了两个动态数组。分别用来放接口注册，及类注册信息。
TCreateInstanceProc = procedure(out obj: TObject);
InvRegClassEntry = record
ClassType: TClass;
Proc: TCreateInstanceProc;
URI: string;
end;
它包含了webservice实现类的指针，以建立实现类的factory函数指针。
InvRegIntfEntry = record
Name: string;                             { Native name of interface    }
ExtName: Widestring;                      { PortTypeName                }
UnitName: string;                         { Filename of interface       }
GUID: TGUID;                              { GUID of interface           }
Info: PTypeInfo;                          { Typeinfo of interface       }
DefImpl: TClass;                          { Metaclass of implementation }
Namespace: Widestring;                    { XML Namespace of type       }
WSDLEncoding: WideString;                 { Encoding                    }
Documentation: string;                    { Description of interface    }
SOAPAction: string;                       { SOAPAction of interface     }
ReturnParamNames: string;                 { Return Parameter names      }
InvokeOptions: TIntfInvokeOptions;        { Invoke Options              }
MethNameMap: array of ExtNameMapItem;             { Renamed methods     }
MethParamNameMap: array of MethParamNameMapItem; { Renamed parameters }
IntfHeaders: array of IntfHeaderItem;      { Headers                    }
IntfExceptions: array of IntfExceptionItem;{ Exceptions                 }
UDDIOperator: String;                      { UDDI Registry of this porttype }
UDDIBindingKey: String;                    { UDDI Binding key           }
end;
看到它里面有很多东西，接口名称，单元名，GUID等信息。
procedure InitBuiltIns;
begin
{ DO NOT LOCALIZE }
RemClassRegistry.RegisterXSInfo(TypeInfo(System.Boolean), XMLSchemaNameSpace, 'boolean');
对于处理结构型数据，需要进行SOAP封包类型的转换
开发人员在使用这种自定义数据类型前必须对其进行注册，分别是RegisterXSClass和RegisterXSInfo。前一个方法是注册从Tremotable继承下来的类，后一个不需要是从TremotablXS继承下来的类。
InitBuiltIns;
InitXSTypes;
InitMoreBuiltIns;
这三个函数类似，都是注册一些基本类型等。
看看到底怎么处理的,(这里注册一个BOOLEAN类型)
RemClassRegistry.RegisterXSInfo(TypeInfo(System.Boolean), XMLSchemaNameSpace, 'boolean');
procedure TRemotableTypeRegistry.RegisterXSInfo(Info: PTypeInfo; const URI: WideString = '';
const Name: WideString = '';
const ExtName: WideString = '');
…
Index := GetEntry(Info, Found, Name);
if Found then
Exit;
if AppNameSpacePrefix <> '' then
AppURI := AppNameSpacePrefix + '-';
if URI = '' then
begin
if Info.Kind = tkDynArray then
begin
UnitName := GetTypeData(Info).DynUnitName;
URIMap[Index].URI := 'urn:' + AppURI + UnitName;
end
else if Info.Kind = tkEnumeration then
begin
UnitName := GetEnumUnitName(Info);
URIMap[Index].URI := 'urn:' + AppURI + UnitName;
end
else if Info.Kind = tkClass then
URIMap[Index].URI := 'urn:' + AppURI + GetTypeData(Info).UnitName
else
URIMap[Index].URI := 'urn:' + AppURI;
end
else
URIMap[Index].URI := URI;
if Name <> '' then
URIMap[Index].Name := Name
else
begin
URIMap[Index].Name := Info.Name;
end;
URIMap[Index].ExtName := ExtName;
URIMap[Index].Info := Info;
if Info.Kind = tkClass then
URIMap[Index].ClassType := GetTypeData(Info).ClassType;
finally
UnLock;
end;
end;
看研究一下GetEntry函数，这里以后多次用到，发现这个函数是TremotableClassRegistry类的，说明实际的注册还是在TremotableClassRegistry这个类完成的。
function TRemotableClassRegistry.GetEntry(Info: PTypeInfo; var Found: Boolean; const Name: WideString): Integer;
begin
Result := FindEntry(Info, Found, Name);
if not Found then
SetLength(URIMap, Result + 1);
end;
这个函数功能是搜索类型是否已注册，否则，动态数组加1，分配空间进行注册。
看看FindEntry (这里传进来的info是TypeInfo(System.Boolean), name: Boolean)
function TRemotableClassRegistry.FindEntry(Info: PTypeInfo; var Found: Boolean; const Name: WideString): Integer;
begin
Result := 0;
Found := False;
while Result < Length(URIMap) do
begin
if (Info <> nil) and (URIMap[Result].Info = Info) then
begin
if (Name = '') or (URIMap[Result].Name = Name) then
begin
Found := True;
Exit;
end;
end;
Inc(Result);
end;
end;
这个函数的功能是遍历整个动态数组TremRegEntry，利用TypeInfo信息和名字进行搜索，查看是否已进行注册。
看看URIMAP的定义：
URIMAP:array of TRemRegEntry;
TObjMultiOptions = (ocDefault, ocMultiRef, ocNoMultiRef);
TRemRegEntry = record
ClassType: TClass; //类信息
Info: PtypeInfo;    // typeInfo信息（RTTL）
URI: WideString;   //
Name: WideString; //
ExtName: WideString; //
IsScalar: Boolean;    //
MultiRefOpt: TObjMultiOptions; //
SerializationOpt: TSerializationOptions;
PropNameMap: array of ExtNameMapItem;             { Renamed properties }
end;
继续RegisterXSInfo函数：
这是对动态数组的uri赋值：
if AppNameSpacePrefix <> '' then
AppURI := AppNameSpacePrefix + '-';
if URI = '' then
begin
if Info.Kind = tkDynArray then
begin
UnitName := GetTypeData(Info).DynUnitName;
URIMap[Index].URI := 'urn:' + AppURI + UnitName;
end
else if Info.Kind = tkEnumeration then
begin
UnitName := GetEnumUnitName(Info);
URIMap[Index].URI := 'urn:' + AppURI + UnitName;
end
else if Info.Kind = tkClass then
URIMap[Index].URI := 'urn:' + AppURI + GetTypeData(Info).UnitName
else
URIMap[Index].URI := 'urn:' + AppURI;
end
else
URIMap[Index].URI := URI;
if Name <> '' then
URIMap[Index].Name := Name
else
begin
URIMap[Index].Name := Info.Name;
end;
这句比较关键：
URIMap[Index].Info := Info;
把RTTL信息保存在URL动态数组中。
总结一下：一些基本类型，都是通过这种方式，把URI，及INFO信息保存在动态数组中的。
为什么要进行登记，因为WEBSERVICE中的数据类型要转换成DELPHI的PAS类型，用URI标记的XML文件，传输之后，根据这张对照表，就可以分配相应的空间。另外这些类型的注册信息是放在：TremRegEntry动态数组中的。和我们自己定义的接口及类是不同的。
FRegClasses: array of InvRegClassEntry;
FRegIntfs: array of InvRegIntfEntry; 这是注册自己定义接口及类的动态数组。
再来分析：
InitBuiltIns函数中的：
RemClassRegistry.RegisterXSClass(TSOAPAttachment, XMLSchemaNamespace, 'base64Binary', '', False, ocNoMultiRef);
大致和基本类型差不多。
procedure TRemotableTypeRegistry.RegisterXSClass(AClass: TClass; const URI: WideString = '';
const Name: WideString = '';
const ExtName: WideString = '';
IsScalar: Boolean = False;
MultiRefOpt: TObjMultiOptions = ocDefault);
var
Index: Integer;
Found: Boolean;
AppURI: WideString;
begin
Lock;
try
Index := GetEntry(AClass.ClassInfo, Found, Name);
if not Found then
begin
if AppNameSpacePrefix <> '' then
AppURI := AppNameSpacePrefix + '-';
if URI = '' then
URIMap[Index].URI := 'urn:' + AppURI + GetTypeData(AClass.ClassInfo).UnitName { do not localize }
else
URIMap[Index].URI := URI;
if Name <> '' then
URIMap[Index].Name := Name
else
begin
URIMap[Index].Name := AClass.ClassName;
end;
URIMap[Index].ExtName := ExtName;
URIMap[Index].ClassType := AClass;
URIMap[Index].Info := AClass.ClassInfo;
URIMap[Index].IsScalar := IsScalar;
URIMap[Index].MultiRefOpt := MultiRefOpt;
end;
finally
UnLock;
end;
end;

前面都是说系统类型的注册。下面看看我们自己定义的接口，是如何注册的：
procedure TInvokableClassRegistry.RegisterInterface(Info: PTypeInfo; const Namespace: InvString;
const WSDLEncoding: InvString; const Doc: string; const ExtName: InvString);
for I := 0 to Length(FRegIntfs) - 1 do
if FRegIntfs[I].Info = Info then
Exit;
Index := Length(FRegIntfs);
SetLength(FRegIntfs, Index + 1);
GetIntfMetaData(Info, IntfMD, True);
FRegIntfs[Index].GUID := IntfMD.IID;
FRegIntfs[Index].Info := Info;
FRegIntfs[Index].Name := IntfMD.Name;
FRegIntfs[Index].UnitName := IntfMD.UnitName;
FRegIntfs[Index].Documentation := Doc;
FRegIntfs[Index].ExtName := ExtName;
FRegIntfs[Index].WSDLEncoding := WSDLEncoding;
if AppNameSpacePrefix <> '' then
URIApp := AppNameSpacePrefix + '-';
{ Auto-generate a namespace from the filename in which the interface was declared and
the AppNameSpacePrefix }
if Namespace = '' then
FRegIntfs[Index].Namespace := 'urn:' + URIApp + IntfMD.UnitName + '-' + IntfMD.Name
else
begin
FRegIntfs[Index].Namespace := Namespace;
FRegIntfs[Index].InvokeOptions := FRegIntfs[Index].InvokeOptions + [ioHasNamespace];
end;
if FRegIntfs[Index].DefImpl = nil then
begin
{ NOTE: First class that implements this interface wins!! }
for I := 0 to Length(FRegClasses) - 1 do
begin
Table := FRegClasses[I].ClassType.GetInterfaceTable;
if (Table = nil) then
begin
Table := FRegClasses[I].ClassType.ClassParent.GetInterfaceTable;
end;
for J := 0 to Table.EntryCount - 1 do
begin
if IsEqualGUID(IntfMD.IID, Table.Entries[J].IID) then
begin
FRegIntfs[Index].DefImpl := FRegClasses[I].ClassType;
Exit;
end;
end;
end;
end;
finally
Unlock;
end;
end;
功能：
for I := 0 to Length(FRegIntfs) - 1 do
if FRegIntfs[I].Info = Info then
Exit;
遍历FRegIntfs: array of InvRegIntfEntry;数组，根据TypeInfo信息判断该接口是否已注册。
Index := Length(FRegIntfs);
SetLength(FRegIntfs, Index + 1);
新增一个数组元素。
GetIntfMetaData(Info, IntfMD, True);
//得到接口的RTTL信息，然后动态增加到注册的动态数组中。
FRegIntfs[Index].GUID := IntfMD.IID;
FRegIntfs[Index].Info := Info;
FRegIntfs[Index].Name := IntfMD.Name;
FRegIntfs[Index].UnitName := IntfMD.UnitName;
FRegIntfs[Index].Documentation := Doc;
FRegIntfs[Index].ExtName := ExtName;
FRegIntfs[Index].WSDLEncoding := WSDLEncoding;
DefImpl里存放的是classType信息：
if FRegIntfs[Index].DefImpl = nil then
begin
for I := 0 to Length(FRegClasses) - 1 do
begin
Table := FRegClasses[I].ClassType.GetInterfaceTable;
if (Table = nil) then
begin
Table := FRegClasses[I].ClassType.ClassParent.GetInterfaceTable;
end;
for J := 0 to Table.EntryCount - 1 do
begin
if IsEqualGUID(IntfMD.IID, Table.Entries[J].IID) then
begin
FRegIntfs[Index].DefImpl := FRegClasses[I].ClassType;
Exit;
end;
end;
end;
end;
注意这里：
FRegClasses: array of InvRegClassEntry;
到注册类的动态数组中去搜寻接口的实现类是否注册，如果注册，便把实现类的指针拷贝到DefImpl数据字段。
顺便看一下类是怎么注册的：
procedure TInvokableClassRegistry.RegisterInvokableClass(AClass: TClass; CreateProc: TCreateInstanceProc);
var
Index, I, J: Integer;
Table: PInterfaceTable;
begin
Lock;
try
Table := AClass.GetInterfaceTable;
。。。。。。
Index := Length(FRegClasses);
SetLength(FRegClasses, Index + 1);
FRegClasses[Index].ClassType := AClass;
FRegClasses[Index].Proc := CreateProc;
for I := 0 to Table.EntryCount - 1 do
begin
for J := 0 to Length(FRegIntfs) - 1 do
if IsEqualGUID(FRegIntfs[J].GUID, Table.Entries[I].IID) then
if FRegIntfs[J].DefImpl = nil then
FRegIntfs[J].DefImpl := AClass;
end;
finally
UnLock;
end;
end;
可以看到和注册接口非常相似。在调用上面方法时，会传入实现类的指针及factory函数指针，调用GetInterfaceTable判断是否实现接口。否则为NIL， 然后在FregClasses增加一元素，把值写入。最后再到FregIntfs是搜寻此实现类的接口是否已经注册。是的话，就把指针储存在FRegIntfs[J].DefImpl中。
继续：
InvRegistry.RegisterDefaultSOAPAction(TypeInfo(IMyFirstWS), 'urn:MyFirstWSIntf-IMyFirstWS\#%operationName%');
procedure TInvokableClassRegistry.RegisterDefaultSOAPAction(Info: PTypeInfo; const DefSOAPAction: InvString);
var
I: Integer;
begin
I := GetIntfIndex(Info);
if I >= 0 then
begin
FRegIntfs[I].SOAPAction := DefSOAPAction;
//值为：urn:MyFirstWSIntf-IMyFirstWS\#%operationName
FRegIntfs[I].InvokeOptions := FRegIntfs[I].InvokeOptions + [ioHasDefaultSOAPAction];
Exit;
end;
end;
设置接口的SOAPAction, 及InvokeOptions属性。
上面讲了用户接口及自定义类注册的实现。
看看这几句为何如此神奇，竟然可以实现对象的远程调用？
MyHTTPRIO := THTTPRIO.Create(nil);
MyHTTPRIO.URL :='http://localhost/soap/MyCGI.exe/soap/IMyFirstWS';
ShowMessage(( MyHTTPRIO As IMyFirstWS ).GetObj);
研究一下客户端代码：
constructor THTTPRIO.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
{ Converter }
FDomConverter := GetDefaultConverter;
FConverter := FDomConverter as IOPConvert;
{ WebNode }
FHTTPWebNode := GetDefaultWebNode;
FWebNode := FHTTPWebNode as IWebNode;
end;
继续到父类中TRIO查看相应代码：
constructor TRIO.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FInterfaceBound := False;
FContext := TInvContext.Create;
FSOAPHeaders := TSOAPHeaders.Create(Self);
FHeadersInbound := THeaderList.Create;
FHeadersOutBound:= THeaderList.Create;
FHeadersOutbound.OwnsObjects := False;
(FSOAPHeaders as IHeadersSetter).SetHeadersInOut(FHeadersInbound, FHeadersOutBound);
end;
创建了TinvContext，这个对象是用来创建一个和服务器端一样的调用环境。
客户端的参数信息一个个的填入这个环境中。
创建一个TSOAPHeaders头对象。
回到
constructor THTTPRIO.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
{ Converter }
FDomConverter := GetDefaultConverter;
FConverter := FDomConverter as IOPConvert;
{ WebNode }
FHTTPWebNode := GetDefaultWebNode;
FWebNode := FHTTPWebNode as IWebNode;
end;
function THTTPRIO.GetDefaultConverter: TOPToSoapDomConvert;
begin
if (FDefaultConverter = nil) then
begin
FDefaultConverter := TOPToSoapDomConvert.Create(Self);
FDefaultConverter.Name := 'Converter1';                 { do not localize }
FDefaultConverter.SetSubComponent(True);
end;
Result := FDefaultConverter;
end;
而TOPToSoapDomConvert可以把Object Pascal的呼叫和參數自動轉換為SOAP封裝的格式資訊，再藉由THTTPReqResp傳送HTTP封包。
function THTTPRIO.GetDefaultWebNode: THTTPReqResp;
begin
if (FDefaultWebNode = nil) then
begin
FDefaultWebNode := THTTPReqResp.Create(Self);
FDefaultWebNode.Name := 'HTTPWebNode1';                { do not localize }
FDefaultWebNode.SetSubComponent(True);
end;
Result := FDefaultWebNode;
end;
//用来传送HTTP的封包。

function THTTPRIO.GetDefaultConverter: TOPToSoapDomConvert;
begin
if (FDefaultConverter = nil) then
begin
FDefaultConverter := TOPToSoapDomConvert.Create(Self);
FDefaultConverter.Name := 'Converter1';                 { do not localize }
FDefaultConverter.SetSubComponent(True);
end;
Result := FDefaultConverter;
end;

FHTTPWebNode := GetDefaultWebNode;
function THTTPRIO.GetDefaultWebNode: THTTPReqResp;
begin
if (FDefaultWebNode = nil) then
begin
FDefaultWebNode := THTTPReqResp.Create(Self);
FDefaultWebNode.Name := 'HTTPWebNode1';                { do not localize }
FDefaultWebNode.SetSubComponent(True);
end;
Result := FDefaultWebNode;
end;
创建了一个THTTPReqResp，用于HTTP通信。
MyHTTPRIO.URL :='http://localhost/soap/MyCGI.exe/soap/IMyFirstWS';
procedure THTTPRIO.SetURL(Value: string);
begin
if Assigned(FHTTPWebNode) then
begin
FHTTPWebNode.URL := Value;
if Value <> '' then
begin
WSDLLocation := '';
ClearDependentWSDLView;
end;
end;
end;
procedure THTTPReqResp.SetURL(const Value: string);
begin
if Value <> '' then
FUserSetURL := True
else
FUserSetURL := False;
InitURL(Value);
Connect(False);
end;
procedure THTTPReqResp.InitURL(const Value: string);
InternetCrackUrl(P, 0, 0, URLComp);
FURLScheme := URLComp.nScheme;
FURLPort := URLComp.nPort;
FURLHost := Copy(Value, URLComp.lpszHostName - P + 1, URLComp.dwHostNameLength);
FURL := Value;
end;
设置THTTPReqResp的属性。和HTTP服务器通信。
procedure THTTPReqResp.Connect(Value: Boolean);
if Assigned(FInetConnect) then
InternetCloseHandle(FInetConnect);
FInetConnect := nil;
if Assigned(FInetRoot) then
InternetCloseHandle(FInetRoot);
FInetRoot := nil;
FConnected := False;
Value 为FLASE。

ShowMessage(( MyHTTPRIO As IMyFirstWS ).GetObj);
利用AS转换成webservice的接口。用转换后的接口到客户端的InvRegInftEntry表格中搜寻WEBSERVICE服务接口，根据RTTL生成SOAP封包。
procedure _IntfCast(var Dest: IInterface; const Source: IInterface; const IID: TGUID);
先看这一句：CALLDWORD PTR [EAX] + VMTOFFSET IInterface.QueryInterface
function THTTPRIO.QueryInterface(const IID: TGUID; out Obj): HResult;
var
UDDIOperator, UDDIBindingKey: string;
begin
Result := inherited QueryInterface(IID, Obj);
if Result = 0 then
begin
if IsEqualGUID(IID, FIID) then
begin
FHTTPWebNode.SoapAction := InvRegistry.GetActionURIOfIID(IID);
if InvRegistry.GetUDDIInfo(IID, UDDIOperator, UDDIBindingKey) then
begin
FHTTPWebNode.UDDIOperator := UDDIOperator;
FHTTPWebNode.UDDIBindingKey := UDDIBindingKey;
end;
end;
end;
end;
Result := inherited QueryInterface(IID, Obj);//跟踪一下这一句：
这句比较重要，要重点分析。
这里创建了虚拟表格。
function TRIO.QueryInterface(const IID: TGUID; out Obj): HResult;
begin
Result := E_NOINTERFACE;
{ IInterface, IRIOAccess } //判断接口是不是IRIOAccess类型
if IsEqualGUID(IID, IInterface) or IsEqualGUID(IID, IRIOAccess) then
{ ISOAPHeaders }//判断接口是不是ISOAPHeaders类型
if IsEqualGUID(IID, ISOAPHeaders) then
…
if GenVTable(IID) then
begin
Result := 0;
FInterfaceBound := True;
Pointer(Obj) := IntfTableP;
InterlockedIncrement(FRefCount);
end;
看看GenVTable函数：
function TRIO.GenVTable(const IID: TGUID): Boolean;
Info := InvRegistry.GetInterfaceTypeInfo(IID);
这个函数是去到TinvokableClassRegistry中搜寻该接口是否注册，注册过的接口则返回typeinfo信息赋给指针。
function TInvokableClassRegistry.GetInterfaceTypeInfo(const AGUID: TGUID): Pointer;
var
I: Integer;
begin
Result := nil;
Lock;
try
for I := 0 to Length(FRegIntfs) - 1 do
begin
if IsEqualGUID(AGUID, FRegIntfs[I].GUID) then
begin
Result := FRegIntfs[I].Info;
Exit;
end;
end;
finally
UnLock;
end;
end;
继续：通过infotype得到RTTL信息。
try
GetIntfMetaData(Info, IntfMD, True);
except
HasRTTI := False;
Exit;
end;
{
TProc = procedure of object;
TObjFunc = function: Integer of Object; stdcall;
TQIFunc = function(const IID: TGUID; out Obj): HResult of object; stdcall;
PProc = ^TProc;
TCracker = record
case integer of
0: (Fn: TProc);
1: (Ptr: Pointer);
2: (ObjFn: TObjFunc);
3: (QIFn: TQIFunc);
end;}
Crack.Fn := GenericStub;
StubAddr := Crack.Ptr;
地址指向函数TRIO.GenericStub函数。
Crack.Fn结构的指针指向
这段代码的意思是用C/stdcall等方式调用函数。
从左到右，从右到左压入堆栈。调整TRIO.IntfTable的指针,最后调用TRIO.Generic
procedure TRIO.GenericStub;
asm
POP     EAX { Return address in runtime generated stub }
POP     EDX { Is there a pointer to return structure on stack and which CC is used? }
CMP     EDX, 2
JZ      @@RETONSTACKRL
CMP     EDX, 1
JZ      @@RETONSTACKLR
POP     EDX           { Method \# pushed by stub }
PUSH    EAX           { Push back return address }
LEA     ECX, [ESP+12] { Calc stack pointer to start of params }
MOV     EAX, [ESP+8] { Calc interface instance ptr }
JMP     @@CONT
@@RETONSTACKLR:
POP     EDX           { Method \# pushed by stub   }
PUSH    EAX           { Push back return address }
LEA     ECX, [ESP+12] { Calc stack pointer to start of params }
MOV     EAX, [ESP+8] { Calc interface instance ptr }
JMP     @@CONT
@@RETONSTACKRL:
POP     EDX           { Method \# pushed by stub }
PUSH    EAX           { Push back return address }
LEA     ECX, [ESP+8] { Calc stack pointer to start of params }
MOV     EAX, [ESP+12] { calc interface instance ptr }
@@CONT:
SUB     EAX, OFFSET TRIO.IntfTable; { Adjust intf pointer to object pointer }
JMP     TRIO.Generic
end;

Crack.Fn := ErrorEntry;
ErrorStubAddr := Crack.Ptr;
//首先分配vtable空间，接口数加3， 因为有Iunknown接口。
GetMem(IntfTable, (Length(IntfMD.MDA) + NumEntriesInIInterface) * 4);
IntfTableP := @IntfTable;
然后把地址赋给IntfTableP变量
GetMem(IntfStubs, (Length( IntfMD.MDA) + NumEntriesInIInterface) * StubSize );
分配存根接口空间。
这是解释
IntfTable: Pointer;{ Generated vtable for the object   }
IntfTableP: Pointer;            { Pointer to the generated vtable   }
IntfStubs: Pointer;             { Pointer to generated vtable thunks}
//Load the IUnknown vtable 分配指针，加入三个接口Iunknown
VTable := PPointer(IntfTable);
Crack.QIFn := _QIFromIntf;
QI查询指针赋值给 Crack结构体
VTable^ := Crack.Ptr; 赋给VT指针
IncPtr(VTable, 4);增加一个指针。
Crack.ObjFn := _AddRefFromIntf;
VTable^ := Crack.Ptr;
IncPtr(VTable, 4);
Crack.ObjFn := _ReleaseFromIntf;
VTable^ := Crack.Ptr;
IncPtr(VTable, 4);

VTable := AddPtr(IntfTable, NumEntriesInIInterface * 4);
//增加IunKnown指针的三个方法。压入IntfTable中。
Thunk := AddPtr(IntfStubs, NumEntriesInIInterface * StubSize);
//调整Thunk，加入IunKnown接口方法。
//遍历所有方法：产生机器相应的汇编机器代码。
for I := NumEntriesInIInterface to Length(IntfMD.MDA) - 1 do
begin
CallStubIdx := 0;
if not IntfMD.MDA[I].HasRTTI then
begin
GenByte($FF); { FF15xxxxxxxx Call [mem]    }
GenByte($15);
Crack.Fn := ErrorEntry;
GenDWORD(LongWord(@ErrorStubAddr));
end else
begin
{ PUSH the method ID }
GenPushI(I);
//定位这里：看看函数做了什么：
CallStub: array[0..StubSize-1] of Byte;
I=3。CallStubIdx=2
procedure TRIO.GenPushI(I: Integer);
begin
if I < 128 then
begin
CallStub[CallStubIdx] := $6A;
CallStub[CallStubIdx + 1] := I;
Inc(CallStubIdx, 2);
end
else
begin
CallStub[CallStubIdx] := $68;
PInteger(@CallStub[CallStubIdx + 1])^ := I;
Inc(CallStubIdx, 5);
end;
end;
登记函数调用信息, 数组增加一元素。
遍历接口信息，函数ID号压入堆栈中。
{ PUSH the info about return value location }
if RetOnStack(IntfMD.MDA[I].ResultInfo) then
begin
if IntfMD.MDA[I].CC in [ccStdcall, ccCdecl] then
GenPushI(2)
else
GenPushI(1);
end
else
GenPushI(0);
把返回值压入堆栈中。//把返回参数压入堆栈。
接着把GenericStub压入堆栈中。
{ Generate the CALL [mem] to the generic stub }
GenByte($FF); { FF15xxxxxxxx Call [mem] }
GenByte($15);
GenDWORD(LongWord(@StubAddr));
这几句是生成汇编的代码。可以产生这样的调用：
ff15xxxxxx:地址： caa [mem]编号: //这里调用的。
//看看里面的内容是什么：
{ Generate the return sequence }
if IntfMD.MDA[I].CC in [ccCdecl] then
begin
{ For cdecl calling convention, the caller will do the cleanup, so }
{ we convert to a regular ret. }
GenRet;
end
else
begin
BytesPushed := 0;
for J := 0 to IntfMD.MDA[I].ParamCount - 1 do
begin
if IsParamByRef(IntfMD.MDA[I].Params[J].Flags, IntfMD.MDA[I].Params[J].Info, IntfMD.MDA[I].CC) then
Inc(BytesPushed, 4)
else
Inc(BytesPushed, GetStackTypeSize(IntfMD.MDA[I].Params[J].Info, IntfMD.MDA[I].CC ));
//每个参数分配空间。
end;

Inc(BytesPushed, GetStackTypeSize(IntfMD.MDA[I].SelfInfo, IntfMD.MDA[I].CC ));
//压入函数本身信息：
{ TODO: Investigate why not always 4 ?? }
if RetOnStack(IntfMD.MDA[I].ResultInfo) or (IntfMD.MDA[I].CC = ccSafeCall) then
Inc(BytesPushed, 4);
if BytesPushed > 252 then
raise Exception.CreateFmt(STooManyParameters, [IntfMD.MDA[I].Name]);
GenRET(BytesPushed);
end;
end;
//GenRET(BytesPushed); 分配函数参数空间。
{ Copy as much of the stub that we initialized over to the }
{ block of memory we allocated. }
P := PByte(Thunk);
for J := 0 to CallStubIdx - 1 do
begin
P^ := CallStub[J];
IncPtr(P);
end;
Thunk的指针，指向汇编代码相应的调用信息：
{ And then fill the remainder with INT 3 instructions for             }
{ cleanliness and safety. If we do the allocated more smartly, we    }
{ can remove all the wasted space, except for maybe alignment.        }
for J := CallStubIdx to StubSize - 1 do
begin
P^ := $CC;
IncPtr(P);
end;
增加Thunk指向存根相应调用信息：
{ Finally, put the new thunk entry into the vtable slot. }
VTable^ := Thunk;
IncPtr(VTable, 4);
把thunk指针赋给vtable之后，压入堆栈。
IncPtr(Thunk, StubSize);
把存根相应调用信息压入堆栈。
然后继续下一个函数的相应操作。
end;
end;
procedure IncPtr(var P; I: Integer = 1);
asm
ADD     [EAX], EDX
end;
总结一下GenVTable函数，这个函数，根据注册的接口，生成了内存表格。
首先遍历整个动态数组，然后，得到接口的RTTL信息，随后把Tcracker结构内存入相应的调用信息。然后再分配两块内存，一块放接口信息，一块放存根调用信息，再把接口内存的指针赋给TRIO的IntfTableP变量。IntfStubs存放存根指针IntfTable指接口信息后，又加入了Iunknown的指针空间。最近遍历接口函数，把函数信息写入CallStub数组之后（生成机器代码），再填入堆栈之中。
继续：
THTTPRIO.QueryInterface
TInvokableClassRegistry.GetActionURIOfInfo
if InvRegistry.GetUDDIInfo(IID, UDDIOperator, UDDIBindingKey) then
调用之后：
function TInvokableClassRegistry.GetUDDIInfo(const IntfInfo: PTypeInfo; var Operator, BindingKey: string): Boolean;
返回
procedure _IntfCast(var Dest: IInterface; const Source: IInterface; const IID: TGUID);
这里，继续：
procedure TRIO.GenericStub;
JMPTRIO.Generic

//这里是最重要的地方：这个函数完成了。打包，传递，并返回服务器端结果。我们仔细研究一下。
function TRIO.Generic(CallID: Integer; Params: Pointer): Int64;
。。。。
MethMD := IntfMD.MDA[CallID]; //得到方法相应的属性。
FContext.SetMethodInfo(MethMD); // FContext 产生虚拟的表函数表格。
procedure TInvContext.SetMethodInfo(const MD: TIntfMethEntry);
begin
SetLength(DataP, MD.ParamCount + 1);
SetLength(Data, (MD.ParamCount + 1) * MAXINLINESIZE);
end;
if MethMd.CC <> ccSafeCall then
begin
if RetOnStack(MethMD.ResultInfo) then
begin
RetP := Pointer(PInteger(P)^);
if MethMD.ResultInfo.Kind = tkVariant then
IncPtr(P, sizeof(Pointer))
else
IncPtr(P, GetStackTypeSize(MethMD.ResultInfo, MethMD.CC));
if MethMD.CC in [ccCdecl, ccStdCall] then
begin
IncPtr(P, sizeof(Pointer));   { Step over self }
end;
end else
RetP := @Result;
FContext.SetResultPointer(RetP);
end;
//把相应的返回信息压入Fcontext中。
for J := 0 to MethMD.ParamCount - 1 do
begin
FContext.SetParamPointer(ParamIdx, P);
with MethMD.Params[J] do
begin
if (Info.Kind = tkVariant) and
(MethMD.CC in [ccCdecl, ccStdCall, ccSafeCall]) and
not (pfVar in Flags) and
not (pfOut in Flags) then
begin
IncPtr(P, sizeof(TVarData)); { NOTE: better would be to dword-align!! }
end
else if IsParamByRef(Flags, Info, MethMD.CC) then
IncPtr(P, 4)
else
IncPtr(P, GetStackTypeSize(Info, MethMD.CC));
end;
Inc(ParamIdx, LeftRightOrder);
end;
//把相应的参数压入Fcontext中。
//转换成XML封包，并写入流中，这里就是具体打包的地方：
大家看清楚了：
Req := FConverter.InvContextToMsg(IntfMD, MethNum, FContext, FHeadersOutBound);
现在来好好研究一下它是怎么转换成XML封包的。
function TOPToSoapDomConvert.InvContextToMsg(const IntfMD: TIntfMetaData; MethNum: Integer;
Con: TInvContext; Headers: THeaderList): TStream;
MethMD := IntfMD.MDA[MethNum];
首先得到方法的动态信息。
XMLDoc := NewXMLDocument; 看看这句：
function TOPToSoapDomConvert.NewXMLDocument: IXMLDocument;
begin
Result := XMLDoc.NewXMLDocument;
Result.Options := Result.Options + [doNodeAutoIndent];
Result.ParseOptions := Result.ParseOptions + [poPreserveWhiteSpace];
end;
function NewXMLDocument(Version: DOMString = '1.0'): IXMLDocument;
begin
Result := TXMLDocument.Create(nil);
Result.Active := True;
if Version <> '' then
Result.Version := Version;
end;
创建了一个TXMLDocument对象用于读写XML。
procedure TXMLDocument.SetActive(const Value: Boolean);
begin
。。。。
CheckDOM;
FDOMDocument := DOMImplementation.createDocument('', '', nil);
try
LoadData;
except
ReleaseDoc(False);
raise;
end;
DoAfterOpen;
end
else
begin
DoBeforeClose;
ReleaseDoc;
DoAfterClose;
end;
end;
end;
procedure TXMLDocument.CheckDOM;
begin
if not Assigned(FDOMImplementation) then
if Assigned(FDOMVendor) then
FDOMImplementation := FDOMVendor.DOMImplementation
else
FDOMImplementation := GetDOM(DefaultDOMVendor);
end;
在TXMLDocument内部使用了Abstract Factory模式
Abstract Factory希望不用指定具体的类，但为了找到它们，在TXMLDocument是通过指定一个字符串，也就是我们点击DOMVendor时出现的哪几个字符串．
GetDOM函数如下：
Result := GetDOMVendor(VendorDesc).DOMImplementation;
//根据传递进去的名字，创建相应在的实例：
function GetDOMVendor(VendorDesc: string): TDOMVendor;
begin
if VendorDesc = '' then
VendorDesc := DefaultDOMVendor;
if (VendorDesc = '') and (DOMVendorList.Count > 0) then
Result := DOMVendorList[0]
else
Result := DOMVendorList.Find(VendorDesc);
if not Assigned(Result) then
raise Exception.CreateFmt(SNoMatchingDOMVendor, [VendorDesc]);
end;
最后取得一个IDOMImplementation，它有一个createDocument（….）:IDOMDocument;函数，这个函数将返回一个IDOMDocument;接口让IXMLDoucment使用。
//由此可见，默认状态下是创建DOM，微软的XML解析器。
function DOMVendorList: TDOMVendorList;
begin
if not Assigned(DOMVendors) then
DOMVendors := TDOMVendorList.Create;
Result := DOMVendors;
end;
function TDOMVendorList.GetVendors(Index: Integer): TDOMVendor;
begin
Result := FVendors[Index];
end;
如果为空，就返回默认的。
function TMSDOMImplementationFactory.DOMImplementation: IDOMImplementation;
begin
Result := TMSDOMImplementation.Create(nil);
end;
再返回到函数：
procedure TXMLDocument.SetActive(const Value: Boolean);
FDOMDocument := DOMImplementation.createDocument('', '', nil);
继续：
function TMSDOMImplementation.createDocument(const namespaceURI,
qualifiedName: DOMString; doctype: IDOMDocumentType): IDOMDocument;
begin
Result := TMSDOMDocument.Create(MSXMLDOMDocumentCreate);
end;
在如果使用MSXML,接口对应的是TMSDOMDocument，TMSDOMDocument是实际上是调用MSXML技术，下面是调用MS COM的代码
function CreateDOMDocument: IXMLDOMDocument;
begin
Result := TryObjectCreate([CLASS_DOMDocument40, CLASS_DOMDocument30,
CLASS_DOMDocument26, msxml.CLASS_DOMDocument]) as IXMLDOMDocument;
if not Assigned(Result) then
raise DOMException.Create(SMSDOMNotInstalled);
end;
再返回到函数：
procedure TXMLDocument.SetActive(const Value: Boolean);
..
LoadData
//因为是新建的TXMLDocument，所以装内空数据,立即返回。
procedure TXMLDocument.LoadData;
const
UnicodeEncodings: array[0..2] of string = ('UTF-16', 'UCS-2', 'UNICODE');
var
Status: Boolean;
ParseError: IDOMParseError;
StringStream: TStringStream;
Msg: string;
begin
…
Status := True; { No load, just create empty doc. }
创建空的文档：
if not Status then
begin
DocSource := xdsNone;
ParseError := DOMDocument as IDOMParseError;
with ParseError do
Msg := Format('%s%s%s: %d%s%s', [Reason, SLineBreak, SLine,
Line, SLineBreak, Copy(SrcText, 1, 40)]);
raise EDOMParseError.Create(ParseError, Msg);
end;
SetModified(False);
end;
设置不能修改。因为空文档。
继续返回到
function NewXMLDocument(Version: DOMString = '1.0'): IXMLDocument;
begin
if Version <> '' then
Result.Version := Version;
end;
procedure TXMLDocument.SetVersion(const Value: DOMString);
begin
SetPrologValue(Value, xpVersion);
end;
procedure TXMLDocument.SetPrologValue(const Value: Variant;
….
PrologNode := GetPrologNode;
PrologAttrs := InternalSetPrologValue(PrologNode, Value, PrologItem);
NewPrologNode := CreateNode('xml', ntProcessingInstr, PrologAttrs);
if Assigned(PrologNode) then
Node.ChildNodes.ReplaceNode(PrologNode, NewPrologNode)
else
ChildNodes.Insert(0, NewPrologNode);
end;

NewPrologNode := CreateNode('xml', ntProcessingInstr, PrologAttrs);
这句调用了：
function TXMLDocument.CreateNode(const NameOrData: DOMString;
NodeType: TNodeType = ntElement; const AddlData: DOMString = ''): IXMLNode;
begin
Result := TXMLNode.Create(CreateDOMNode(FDOMDocument, NameOrData,
NodeType, AddlData), nil, Self);
end;

在返回到这个函数中：
function TOPToSoapDomConvert.InvContextToMsg(const IntfMD: TIntfMetaData; MethNum: Integer;
Con: TInvContext; Headers: THeaderList): TStream;
BodyNode := Envelope.MakeBody(EnvNode);
if not (soLiteralParams in Options) then
begin
SoapMethNS := GetSoapNS(IntfMD);
ExtMethName := InvRegistry.GetMethExternalName(IntfMD.Info, MethMD.Name)；
;;;;;
//创建一个SOAP的body:
function TSoapEnvelope.MakeBody(ParentNode: IXMLNode): IXMLNode;
begin
Result := ParentNode.AddChild(SSoapNameSpacePre + ':' + SSoapBody, SSoapNameSpace);
end;

SoapMethNS := GetSoapNS(IntfMD); 返回：'urn:MyFirstWSIntf-IMyFirstWS'
ExtMethName := InvRegistry.GetMethExternalName(IntfMD.Info, MethMD.Name);
得到调用方法名。剩下的部分就是把参数打包。生成SOAP的源文件。然后写到内存流中。

再回到函数中：InvContextToMsg
Result := TMemoryStream.Create();
DOMToStream(XMLDoc, Result);
把内存块的数据，转化成XML。
具体的函数如下：
procedure TOPToSoapDomConvert.DOMToStream(const XMLDoc: IXMLDocument; Stream: TStream);
var
XMLWString: WideString;
StrStr: TStringStream;
begin
if (FEncoding = '') or (soUTF8EncodeXML in Options) then
begin
XMLDoc.SaveToXML(XMLWString);
StrStr := TStringStream.Create(UTF8Encode(XMLWString));
try
Stream.CopyFrom(StrStr, 0);
finally
StrStr.Free;
end;
end else
XMLDoc.SaveToStream(Stream);
end;
我们跟踪之后StrStr的结果如下：
'<?xml version="1.0"?>'\#$D\#$A''\#$D\#$A' '\#$D\#$A''\#$D\#$A'3'\#$D\#$A'**4**'\#$D\#$A'    '\#$D\#$A''\#$D\#$A''\#$D\#$A

转化后继续调用Generic函数：
。。。。
FWebNode.BeforeExecute(IntfMD, MethMD, MethNum-3, nil);
if (BindingType = btMIME) then
begin
。。。
FWebNode.BeforeExecute(IntfMD, MethMD, MethNum-3, nil);
THTTPReqResp.BeforeExecute
。。。。。
MethName := InvRegistry.GetMethExternalName(IntfMD.Info, MethMD.Name);
FSoapAction := InvRegistry.GetActionURIOfInfo(IntfMD.Info, MethName, MethodIndex);
得到方法名和FsoapAction
FBindingType := btSOAP
DoBeforeExecute // TRIO.
if Assigned(FOnBeforeExecute) then
退出：
继续：
Resp := GetResponseStream(RespBindingType);

继续返回到TRIO.Generic函数中执行：
try
FWebNode.Execute(Req, Resp);
比较重要的部分：
这个函数就是THTTPReqResp向IIS发出请求。并返回信息：
procedure THTTPReqResp.Execute(const Request: TStream; Response: TStream);
begin
…
Context := Send(Request);
try
try
Receive(Context, Response);
Exit;
except
on Ex: ESOAPHTTPException do
begin
Connect(False);
if not CanRetry or not IsErrorStatusCode(Ex.StatusCode) then
raise;
{ Trigger UDDI Lookup }
LookUpUDDI := True;
PrevError := Ex.Message;
end;
else
begin
Connect(False);
raise;
end;
end;
finally
if Context <> 0 then
InternetCloseHandle(Pointer(Context));
end;
end;
{$ENDIF}
end;
现在看看Send函数，看看到底如何发送数据给WEB服务器的。
function THTTPReqResp.Send(const ASrc: TStream): Integer;
var
Request: HINTERNET;
RetVal, Flags: DWord;
P: Pointer;
ActionHeader: string;
ContentHeader: string;
BuffSize, Len: Integer;
INBuffer: INTERNET_BUFFERS;
Buffer: TMemoryStream;
StrStr: TStringStream;
begin
{ Connect }
Connect(True);
Flags := INTERNET_FLAG_KEEP_CONNECTION or INTERNET_FLAG_NO_CACHE_WRITE;
if FURLScheme = INTERNET_SCHEME_HTTPS then
begin
Flags := Flags or INTERNET_FLAG_SECURE;
if (soIgnoreInvalidCerts in InvokeOptions) then
Flags := Flags or (INTERNET_FLAG_IGNORE_CERT_CN_INVALID or
INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
end;
Request := nil;
try
Request := HttpOpenRequest(FInetConnect, 'POST', PChar(FURLSite), nil,
nil, nil, Flags, 0{Integer(Self)});
Check(not Assigned(Request));
{ Timeouts }
if FConnectTimeout > 0 then
Check(InternetSetOption(Request, INTERNET_OPTION_CONNECT_TIMEOUT, Pointer(@FConnectTimeout), SizeOf(FConnectTimeout)));
if FSendTimeout > 0 then
Check(InternetSetOption(Request, INTERNET_OPTION_SEND_TIMEOUT, Pointer(@FSendTimeout), SizeOf(FSendTimeout)));
if FReceiveTimeout > 0 then
Check(InternetSetOption(Request, INTERNET_OPTION_RECEIVE_TIMEOUT, Pointer(@FReceiveTimeout), SizeOf(FReceiveTimeout)));
{ Setup packet based on Content-Type/Binding }
if FBindingType = btMIME then
begin
ContentHeader := Format(ContentHeaderMIME, [FMimeBoundary]);
ContentHeader := Format(ContentTypeTemplate, [ContentHeader]);
HttpAddRequestHeaders(Request, PChar(MIMEVersion), Length(MIMEVersion), HTTP_ADDREQ_FLAG_ADD);
{ SOAPAction header }
{ NOTE: It's not really clear whether this should be sent in the case
of MIME Binding. Investigate interoperability ?? }
if not (soNoSOAPActionHeader in FInvokeOptions) then
begin
ActionHeader:= GetSOAPActionHeader;
HttpAddRequestHeaders(Request, PChar(ActionHeader), Length(ActionHeader), HTTP_ADDREQ_FLAG_ADD);
end;
end else { Assume btSOAP }
begin
{ SOAPAction header }
if not (soNoSOAPActionHeader in FInvokeOptions) then
begin
ActionHeader:= GetSOAPActionHeader;
HttpAddRequestHeaders(Request, PChar(ActionHeader), Length(ActionHeader), HTTP_ADDREQ_FLAG_ADD);
end;
if UseUTF8InHeader then
ContentHeader := Format(ContentTypeTemplate, [ContentTypeUTF8])
else
ContentHeader := Format(ContentTypeTemplate, [ContentTypeNoUTF8]);
end;
{ Content-Type }
HttpAddRequestHeaders(Request, PChar(ContentHeader), Length(ContentHeader), HTTP_ADDREQ_FLAG_ADD);
{ Before we pump data, see if user wants to handle something - like set Basic-Auth data?? }
if Assigned(FOnBeforePost) then
FOnBeforePost(Self, Request);
ASrc.Position := 0;
BuffSize := ASrc.Size;
if BuffSize > FMaxSinglePostSize then
begin
Buffer := TMemoryStream.Create;
try
Buffer.SetSize(FMaxSinglePostSize);
{ Init Input Buffer }
INBuffer.dwStructSize := SizeOf(INBuffer);
INBuffer.Next := nil;
INBuffer.lpcszHeader := nil;
INBuffer.dwHeadersLength := 0;
INBuffer.dwHeadersTotal := 0;
INBuffer.lpvBuffer := nil;
INBuffer.dwBufferLength := 0;
INBuffer.dwBufferTotal := BuffSize;
INBuffer.dwOffsetLow := 0;
INBuffer.dwOffsetHigh := 0;
{ Start POST }
Check(not HttpSendRequestEx(Request, @INBuffer, nil,
HSR_INITIATE or HSR_SYNC, 0));
try
while True do
begin
{ Calc length of data to send }
Len := BuffSize - ASrc.Position;
if Len > FMaxSinglePostSize then
Len := FMaxSinglePostSize;
{ Bail out if zip.. }
if Len = 0 then
break;
{ Read data in buffer and write out}
Len := ASrc.Read(Buffer.Memory^, Len);
if Len = 0 then
raise ESOAPHTTPException.Create(SInvalidHTTPRequest);
Check(not InternetWriteFile(Request, @Buffer.Memory^, Len, RetVal));
RetVal := InternetErrorDlg(GetDesktopWindow(), Request, GetLastError,
FLAGS_ERROR_UI_FILTER_FOR_ERRORS or FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS or
FLAGS_ERROR_UI_FLAGS_GENERATE_DATA, P);
case RetVal of
ERROR_SUCCESS: ;
ERROR_CANCELLED: SysUtils.Abort;
ERROR_INTERNET_FORCE_RETRY: {Retry the operation};
end;
{ Posting Data Event }
if Assigned(FOnPostingData) then
FOnPostingData(ASrc.Position, BuffSize);
end;
finally
Check(not HttpEndRequest(Request, nil, 0, 0));
end;
finally
Buffer.Free;
end;
end else
begin
StrStr := TStringStream.Create('');
try
StrStr.CopyFrom(ASrc, 0);
while True do
begin
Check(not HttpSendRequest(Request, nil, 0, @StrStr.DataString[1], Length(StrStr.DataString)));
RetVal := InternetErrorDlg(GetDesktopWindow(), Request, GetLastError,
FLAGS_ERROR_UI_FILTER_FOR_ERRORS or FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS or
FLAGS_ERROR_UI_FLAGS_GENERATE_DATA, P);
case RetVal of
ERROR_SUCCESS: break;
ERROR_CANCELLED: SysUtils.Abort;
ERROR_INTERNET_FORCE_RETRY: {Retry the operation};
end;
end;
finally
StrStr.Free;
end;
end;
except
if (Request <> nil) then
InternetCloseHandle(Request);
Connect(False);
raise;
end;
Result := Integer(Request);
end;

function THTTPReqResp.Send(const ASrc: TStream): Integer;
先调用了：
procedure THTTPReqResp.Connect(Value: Boolean);
……
if InternetAttemptConnect(0) <> ERROR_SUCCESS then
SysUtils.Abort;
这个函数可以说非常简单，只是尝试计算机连接到网络。
FInetRoot := InternetOpen(PChar(FAgent), AccessType, PChar(FProxy), PChar(FProxyByPass), 0);
创建HINTERNET句柄，并初始化WinInet的API函数：
Check(not Assigned(FInetRoot));
try
FInetConnect := InternetConnect(FInetRoot, PChar(FURLHost), FURLPort, PChar(FUserName),
PChar(FPassword), INTERNET_SERVICE_HTTP, 0, Cardinal(Self));
//创建一个特定的会话：
Check(not Assigned(FInetConnect));
FConnected := True;
except
InternetCloseHandle(FInetRoot);
FInetRoot := nil;
raise;
end;
这里已经创建了一个会话：
继续返回function THTTPReqResp.Send(const ASrc: TStream): Integer;函数之中：
。。。。
Request := HttpOpenRequest(FInetConnect, 'POST', PChar(FURLSite), nil,
nil, nil, Flags, 0{Integer(Self)});
Check(not Assigned(Request));。
打开一个HTTP的请求。向WEB服务器提出请求：
。。
if not (soNoSOAPActionHeader in FInvokeOptions) then
begin
ActionHeader:= GetSOAPActionHeader;
HttpAddRequestHeaders(Request, PChar(ActionHeader), Length(ActionHeader), HTTP_ADDREQ_FLAG_ADD);
end;
。。。
为请求添加一个或多个标头。可以看到标点的信息为：
'SOAPAction: "urn:MyFirstWSIntf-IMyFirstWS\#GetObj"'

HttpAddRequestHeaders(Request, PChar(ContentHeader), Length(ContentHeader), HTTP_ADDREQ_FLAG_ADD);
继续加入标头'Content-Type: text/xml'信息：
StrStr := TStringStream.Create('');
try
StrStr.CopyFrom(ASrc, 0);
while True do
begin
Check(not HttpSendRequest(Request, nil, 0, @StrStr.DataString[1], Length(StrStr.DataString)));
建立到internet 的连接，并将请求发送到指定的站点。
这句执行完后的图如下（用工具跟踪的结果）：
看看前面的soap生成的字符 StrStr的结果如下，发现后半部分是一样的。
继续
function THTTPReqResp.Execute(const Request: TStream): TStream;
Receive(Context, Response);

procedure THTTPReqResp.Receive(Context: Integer; Resp: TStream; IsGet: Boolean);
var
Size, Downloaded, Status, Len, Index: DWord;
S: string;
begin
..
//获取请求信息：
HttpQueryInfo(Pointer(Context), HTTP_QUERY_CONTENT_TYPE, @FContentType[1], Size, Index);
repeat
Check(not InternetQueryDataAvailable(Pointer(Context), Size, 0, 0));
if Size > 0 then
begin
SetLength(S, Size);
Check(not InternetReadFile(Pointer(Context), @S[1], Size, Downloaded));
//下载数据：
Resp.Write(S[1], Size);
{ Receiving Data event }
if Assigned(FOnReceivingData) then
FOnReceivingData(Size, Downloaded)
end;
until Size = 0;
S的结果如下和刚才跟踪器里的是一模一样的：
'<?xml version="1.0"?>'\#$D\#$A''\#$D\#$A' '\#$D\#$A''\#$D\#$A'      12'\#$D\#$A'    '\#$D\#$A''\#$D\#$A''\#$D\#$A
最后关闭HTTP会话句柄：
InternetCloseHandle(Pointer(Context));
在返回function TRIO.Generic(CallID: Integer; Params: Pointer): Int64;函数中继续查看：
RespXML := Resp;
返回信息的内存流
FConverter.ProcessResponse(RespXML, IntfMD, MethMD, FContext, FHeadersInbound);
再次把SOAP封包转换成PASCEL调用：
procedure TOPToSoapDomConvert.ProcessResponse(const Resp: TStream;
const IntfMD: TIntfMetaData;
const MD: TIntfMethEntry;
Context: TInvContext;
Headers: THeaderList);
var
XMLDoc: IXMLDocument;
begin
XMLDoc := NewXMLDocument;
XMLDoc.Encoding := FEncoding;
Resp.Position := 0;
XMLDoc.LoadFromStream(Resp);
ProcessResponse(XMLDoc, IntfMD, MD, Context, Headers);
end;
procedure TOPToSoapDomConvert.ProcessResponse(const XMLDoc: IXMLDocument;
const IntfMD: TIntfMetaData;
const MD: TIntfMethEntry;
Context: TInvContext;
Headers: THeaderList);
var
ProcessSuccess(RespNode, IntfMD, MD, Context);
ProcessSuccess函数如下：
….
for I := 0 to RespNode.childNodes.Count - 1 do
begin
Node := RespNode.childNodes[I];
{ Skip non-valid nodes }
if Node.NodeType <> ntElement then
continue;
// 处理返回值：
if I = RetIndex then
begin
InvData := InvContext.GetResultPointer;
ByRef := IsParamByRef([pfOut], MD.ResultInfo, MD.CC);
ConvertSoapToNativeData(InvData, MD.ResultInfo, InvContext, RespNode, Node, True, ByRef, 1);

ConvertSoapToNativeData(InvData, MD.ResultInfo, InvContext, RespNode, Node, True, ByRef, 1);
把SOAP的结果，写入返回区地址空间。

procedure TSOAPDomConv.ConvertSoapToNativeData(DataP: Pointer; TypeInfo: PTypeInfo;
Context: TDataContext; RootNode, Node: IXMLNode; Translate, ByRef: Boolean;
NumIndirect: Integer);
var
TypeUri, TypeName: InvString;
IsNull: Boolean;
Obj: TObject;
P: Pointer;
I: Integer;
ID: InvString;
begin
Node := GetDataNode(RootNode, Node, ID);
IsNull := NodeIsNull(Node);
if TypeInfo.Kind = tkVariant then
begin
if NumIndirect > 1 then
DataP := Pointer(PInteger(DataP)^);
if IsNull then
begin
Variant(PVarData(DataP)^) := NULL;
end else
ConvertSoapToVariant(Node, DataP);
end else
if TypeInfo.Kind = tkDynArray then
begin
P := DataP;
for I := 0 to NumIndirect - 2 do
P := Pointer(PInteger(P)^);
P := ConvertSoapToNativeArray(P, TypeInfo, RootNode, Node);
if NumIndirect = 1 then
PInteger(DataP)^ := Integer(P)
else if NumIndirect = 2 then
begin
DataP := Pointer(PInteger(DataP)^);
PInteger(DataP)^ := Integer(P);
end;
end else
if TypeInfo.Kind = tkClass then
begin
Obj := ConvertSOAPToObject(RootNode, Node, GetTypeData(TypeInfo).ClassType, TypeURI, TypeName, DataP, NumIndirect);
if NumIndirect = 1 then
PTObject(DataP)^ := Obj
else if NumIndirect = 2 then
begin
DataP := Pointer(PInteger(DataP)^);
PTObject(DataP)^ := Obj;
end;
end else
begin
if Translate then
begin
if NumIndirect > 1 then
DataP := Pointer(PInteger(DataP)^);
if not TypeTranslator.CastSoapToNative(TypeInfo, GetNodeAsText(Node), DataP, IsNull) then
raise ESOAPDomConvertError.CreateFmt(STypeMismatchInParam, [node.nodeName]);
end;
end;
end;
作为整型数据，处理方式为：
if not TypeTranslator.CastSoapToNative(TypeInfo, GetNodeAsText(Node), DataP, IsNull) then
function TTypeTranslator.CastSoapToNative(Info: PTypeInfo; const SoapData: WideString; NatData: Pointer; IsNull: Boolean): Boolean;
var
ParamTypeData: PTypeData;
begin
DecimalSeparator := '.';
Result := True;
if IsNull and (Info.Kind = tkVariant) then
begin
Variant(PVarData(NatData)^) := NULL;
Exit;
end;
ParamTypeData := GetTypeData(Info);
case Info^.Kind of
tkInteger:
case ParamTypeData^.OrdType of
otSByte,
otUByte:
PByte(NatData)^ := StrToInt(Trim(SoapData));
otSWord,
otUWord:
PSmallInt(NatData)^ := StrToInt(Trim(SoapData));
otSLong,
otULong:
PInteger(NatData)^ := StrToInt(Trim(SoapData));
end;
tkFloat:
case ParamTypeData^.FloatType of
ftSingle:
PSingle(NatData)^ := StrToFloatEx(Trim(SoapData));
ftDouble:
begin
if Info = TypeInfo(TDateTime) then
PDateTime(NatData)^ := XMLTimeToDateTime(Trim(SoapData))
else
PDouble(NatData)^ := StrToFloatEx(Trim(SoapData));
end;
ftComp:
PComp(NatData)^ := StrToFloatEx(Trim(SoapData));
ftCurr:
PCurrency(NatData)^ := StrToFloatEx(Trim(SoapData));
ftExtended:
PExtended(NatData)^ := StrToFloatEx(Trim(SoapData));
end;
tkWString:
PWideString(NatData)^ := SoapData;
tkString:
PShortString(NatData)^ := SoapData;
tkLString:
PString(NatData)^ := SoapData;
tkChar:
if SoapData <> '' then
PChar(NatData)^ := Char(SoapData[1]);
tkWChar:
if SoapData <> '' then
PWideChar(NatData)^ := WideChar(SoapData[1]);
tkInt64:
PInt64(NatData)^ := StrToInt64(Trim(SoapData));
tkEnumeration:
{ NOTE: Here we assume enums to be byte-size; make sure (specially for C++)
that enums have generated with the proper size }
PByte(NatData)^ := GetEnumValueEx(Info, Trim(SoapData));
tkClass:
;
tkSet,
tkMethod,
tkArray,
tkRecord,
tkInterface,
tkDynArray:
raise ETypeTransException.CreateFmt(SUnexpectedDataType, [ KindNameArray[Info.Kind]] );
tkVariant:
CastSoapToVariant(Info, SoapData, NatData);
end;
end;
PWideString(NatData)^ := SoapData;
通过把值赋给了相应的指针地址：
另外在看一下传对象时的情况：
Obj := ConvertSOAPToObject(RootNode, Node, GetTypeData(TypeInfo).ClassType, TypeURI, TypeName, DataP, NumIndirect);

if Assigned(Obj) and LegalRef then
begin
if (NodeClass <> nil) and (NodeClass <> Obj.ClassType) then
Obj := NodeClass.Create;
end else
begin
if (NodeClass <> nil) and NodeClass.InheritsFrom(AClass) then
Obj := TRemotableClass(NodeClass).Create
else
Obj := TRemotableClass(AClass).Create;
end;
Result := Obj;
可以理解，经过双边注册过之后，才可以传递对象。
现在研究一下服务器端的代码：
先大概简单介绍一下WEB服务器应用程序的工作模式：
这里的WEB服务器就是IIS。
也就是说WEB服务器会把客户的HTTP请求消息，传递给CGI程序。然后由CGI进行处理：
CGIApp单元中的：
procedure InitApplication;
begin
Application := TCGIApplication.Create(nil);
end;
//创建一个CGI的应用程序
constructor TWebApplication.Create(AOwner: TComponent);
begin
WebReq.WebRequestHandlerProc := WebRequestHandler;
inherited Create(AOwner);
Classes.ApplicationHandleException := HandleException;
if IsLibrary then
begin
IsMultiThread := True;
OldDllProc := DLLProc;
DLLProc := DLLExitProc;
end
else
AddExitProc(DoneVCLApplication);
end;
constructor TWebRequestHandler.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
FCriticalSection := TCriticalSection.Create;
FActiveWebModules := TList.Create;
FInactiveWebModules := TList.Create;
FWebModuleFactories := TWebModuleFactoryList.Create;
FMaxConnections := 32;
FCacheConnections := True;
end;
procedure TCGIApplication.Run;
var
HTTPRequest: TCGIRequest;
HTTPResponse: TCGIResponse;
begin
inherited Run;
if IsConsole then
begin
Rewrite(Output);
Reset(Input);
end;
try
HTTPRequest := NewRequest;
try
HTTPResponse := NewResponse(HTTPRequest);
try
HandleRequest(HTTPRequest, HTTPResponse);
finally
HTTPResponse.Free;
end;
finally
HTTPRequest.Free;
end;
except
HandleServerException(ExceptObject, FOutputFileName);
end;
end;
HTTPResponse := NewResponse(HTTPRequest);
调用：
function TCGIApplication.GetFactory: TCGIFactory;
begin
if not Assigned(FFactory) then
FFactory := TCGIFactory.Create;
Result := FFactory;
end;

function TCGIFactory.NewRequest: TCGIRequest;
Result := TCGIRequest.Create
。。。
end;
//创建TCGIRequest
HTTPResponse := NewResponse(HTTPRequest);
Result := TCGIResponse.Create(CGIRequest)
HandleRequest(HTTPRequest, HTTPResponse);调用
现在看看是怎么响应客户端的：
function TWebRequestHandler.HandleRequest(Request: TWebRequest;
Response: TWebResponse): Boolean;
var
I: Integer;
WebModules: TWebModuleList;
WebModule: TComponent;
WebAppServices: IWebAppServices;
GetWebAppServices: IGetWebAppServices;
begin
Result := False;
WebModules := ActivateWebModules;
继续：
function TWebRequestHandler.ActivateWebModules: TWebModuleList;
begin
………………
FWebModuleFactories.AddFactory(TDefaultWebModuleFactory.Create(WebModuleClass));
把TWebModule1加入工厂中,并创建TwebModuleList对象。
if FWebModuleFactories.ItemCount > 0 then
begin
Result := TWebModuleList.Create(FWebModuleFactories);
………………..
继续：
if Assigned(WebModules) then
try
WebModules.AutoCreateModules;
procedure TWebModuleList.AutoCreateModules
….... AddModule(Factory.GetModule);
调用：TWebModule1.create并加入TwebModuleList中。
function TDefaultWebModuleFactory.GetModule: TComponent;
begin
Result := FComponentClass.Create(nil);
end;
constructor TWebModule.Create(AOwner: TComponent);调用
constructor TCustomWebDispatcher.Create(AOwner: TComponent);
之后又创建了THTTPSoapDispatcher，创建是在Treader类中创建的，有兴趣的朋友就追踪一下吧，这里实在是太麻烦。我也追了很久才发现。就懒得贴上来了。内容太多。
继续创建了TWSDLHTMLPublish
在回到TWebRequestHandler.HandleRequest函数中：
。。。
Result := WebAppServices.HandleRequest;
最后调用了：
function TCustomWebDispatcher.HandleRequest(
Request: TWebRequest; Response: TWebResponse): Boolean;
begin
FRequest := Request;
FResponse := Response;
Result := DispatchAction(Request, Response);
end;
注意HandleRequest函数，这里是关键部分：
function TCustomWebDispatcher.DispatchAction(Request: TWebRequest;
Response: TWebResponse): Boolean;
…………………
while not Result and (I < FDispatchList.Count) do
begin
if Supports(IInterface(FDispatchList.Items[I]), IWebDispatch, Dispatch) then
begin
Result := DispatchHandler(Self, Dispatch,
Request, Response, False);
end;
Inc(I);
end;
继续：
function DispatchHandler(Sender: TObject; Dispatch: IWebDispatch; Request: TWebRequest; Response: TWebResponse;
DoDefault: Boolean): Boolean;
begin
Result := False;
if (Dispatch.Enabled and ((Dispatch.MethodType = mtAny) or
(Dispatch.MethodType = Dispatch.MethodType)) and
Dispatch.Mask.Matches(Request.InternalPathInfo)) then
begin
Result := Dispatch.DispatchRequest(Sender, Request, Response);
end;
end;

http调用在到达服务器后，WebModule父类TCustomWebDispatcher
会对其进行分析，抽取参数等信息。然后在TCustomWebDispatcher.HandleRequest
方法中调用TCustomWebDispatcher.DispatchAction方法，将调用
根据其path info重定向到相应的处理方法去。而DispatchAction方法将
Action重定向到FDispatchList字段中所有的实现了IWebDispatch接口的组件。
而THTTPSoapDispatcher正是实现了IWebDispatch，其将在
TCustomWebDispatcher.InitModule方法中被自动检测到并加入FDispatchList字段
具体如下：
procedure TCustomWebDispatcher.InitModule(AModule: TComponent);
var
I: Integer;
Component: TComponent;
DispatchIntf: IWebDispatch;
begin
if AModule <> nil then
for I := 0 to AModule.ComponentCount - 1 do
begin
Component := AModule.Components[I];
if Supports(IInterface(Component), IWebDispatch, DispatchIntf) then
FDispatchList.Add(Component);
end;
end;
...
THTTPSoapDispatcher = class(THTTPSoapDispatchNode, IWebDispatch)
因此Web Service程序的http请求处理实际上是由THTTPSoapDispatcher进行的。

我们接着看看THTTPSoapDispatcher.DispatchRequest方法中对SOAP
协议的处理，关键代码如下
function THTTPSoapDispatcher.DispatchRequest(Sender: TObject;
Request: TWebRequest; Response: TWebResponse): Boolean;
var
…..
http信息被封装在TwebRequest里：我们来看是怎么进行分析的：
SoapAction := Request.GetFieldByName(SHTTPSoapAction);
首先得到SOAPAction信息, 这个SOAPAction大家应该比较熟悉了，前面讲过，这里主要是根据相应信息调用方法：() 具体的内容例如：urn:MyFirstWSIntf-IMyFirstWS
….
if SoapAction = '' then
SoapAction := Request.GetFieldByName('HTTP_' + UpperCase(SHTTPSoapAction)); { do not localize }
CGI或者Apache的处理方式。如果不是SOAP请求，就默认HTTP请求。
记录请求的路径。
Path := Request.PathInfo;
XMLStream := TMemoryStream.Create; //把客户端的请求流化。
ReqStream := TWebRequestStream.Create(Request);
创建一个响应的流信息,以例把结果返回客户端
RStream := TMemoryStream.Create; 创建返回信息的流。
try
FSoapDispatcher.DispatchSOAP(Path, SoapAction, XMLStream, RStream, BindingTypeIn);
这句是最重要的：
它把ＨＴＴＰ的调用方法，委托给THTTPSoapPascalInvoker.DispatchSOAP来处理。
FSoapDispatcher.DispatchSOAP(Path, SoapAction, XMLStream, RStream, BindingTypeIn);
IHTTPSoapDispatch = interface
['{9E733EDC-7639-4DAF-96FF-BCF141F7D8F2}']
procedure DispatchSOAP(const Path, SoapAction: WideString; const Request: TStream;
Response: TStream; var BindingType: TWebServiceBindingType);
end;
父类实现的接口：
THTTPSoapDispatchNode = class(TComponent)
private
procedure SetSoapDispatcher(const Value: IHTTPSoapDispatch);
protected
FSoapDispatcher: IHTTPSoapDispatch;
procedure Notification(AComponent: TComponent; Operation: TOperation); override;
public
procedure DispatchSOAP(const Path, SoapAction: WideString; const Request: TStream;
Response: TStream); virtual;
published
property Dispatcher: IHTTPSoapDispatch read FSoapDispatcher write SetSoapDispatcher;
end;
也被THTTPSoapPascalInvoker实现。所以THTTPSoapDispatcher中的Dispatcher接口的实例其实是：THTTPSoapPascalInvoker
THTTPSoapPascalInvoker = class(TSoapPascalInvoker, IHTTPSoapDispatch)
public
procedure DispatchSOAP(const Path, SoapAction: WideString; const Request: TStream;
Response: TStream; var BindingType: TWebServiceBindingType); virtual;
end;
FSoapDispatcher.DispatchSOAP(Path, SoapAction, XMLStream, RStream, BindingTypeIn);
相应于调用了：
procedure THTTPSoapPascalInvoker.DispatchSOAP(const Path, SoapAction: WideString; const Request: TStream;
Response: TStream; var BindingType: TWebServiceBindingType);
var
IntfInfo: PTypeInfo;
PascalBind: IHTTPSOAPToPasBind;
InvClassType: TClass;
ActionMeth: String;
MD: TIntfMetaData;
if not PascalBind.BindToPascalByPath(Path, InvClassType, IntfInfo, ActionMeth) or (InvClassType = nil) then
调用:
function THTTPSOAPToPasBind.BindToPascalByPath(Path: String;
var AClass: TClass; var IntfInfo: PTypeInfo; var AMeth: String): Boolean;
begin
Result := InvRegistry.GetInfoForURI(Path, '', AClass, IntfInfo, AMeth);
end;
由InvRegistry的注册信息，返回相应的类名，接口信息等信息。
这了这些准备信息，下步才是真正的调用。
Invoke(InvClassType, IntfInfo, ActionMeth, Request, Response, BindingType);
函数最后一句：调用了父类：这里是真正工作的地方：
这里了仔细认真研究一下：
procedure TSoapPascalInvoker.Invoke(AClass: TClass; IntfInfo: PTypeInfo; MethName: string; const Request: TStream;
Response: TStream; var BindingType: TWebServiceBindingType);
var
Inv: TInterfaceInvoker;
Obj: TObject;
InvContext: TInvContext;
IntfMD: TIntfMetaData;
MethNum: Integer;
SOAPHeaders: ISOAPHeaders;
Handled: Boolean;
begin
try
GetIntfMetaData(IntfInfo, IntfMD, True); 得到接口ＲＴＴＬ信息；
InvContext := TInvContext.Create;　　　　构造调用堆栈。
{ Convert XML to Invoke Context }
FConverter.MsgToInvContext(Request, IntfMD, MethNum, InvContext, FHeadersIn);
这个函数请见前面的参考InvContextToMsg, 把TinvContext内容转化成XML封包。
这个函数是逆操作，把XML内容转化成Context。

try
Obj := InvRegistry.GetInvokableObjectFromClass(AClass);
搜寻注册信息，创建实例：
if Obj = nil then
raise Exception.CreateFmt(SNoClassRegistered, [IntfMD.Name]);
……………..
Inv := TInterfaceInvoker.Create;
Inv.Invoke(Obj, IntfMD, MethNum, InvContext);
真正调用的地方：

源代码为：
这段代码，就是根据对象，接口信息等，把ＣＯＮtext的信息压入相应参数，应调用。
有时间再仔细研究。
procedure TInterfaceInvoker.Invoke(const Obj: TObject;
IntfMD: TIntfMetaData; const MethNum: Integer;
const Context: TInvContext);
var
MethPos: Integer;
Unk: IUnknown;
IntfEntry: PInterfaceEntry;
IntfVTable: Pointer;
RetIsOnStack, RetIsInFPU, RetInAXDX: Boolean;
I: Integer;
RetP : Pointer;
MD : TIntfMethEntry;
DataP: Pointer;
Temp, Temp1: Integer;
RetEAX: Integer;
RetEDX: Integer;
TotalParamBytes: Integer;
ParamBytes: Integer;
begin
{$IFDEF LINUX}
try
{$ENDIF}
TotalParamBytes := 0;
MD := IntfMD.MDA[MethNUm]; //得到方法的动态数组信息：
if not Obj.GetInterface(IntfMD.IID, Unk) then
raise Exception.CreateFmt(SNoInterfaceGUID,
[Obj.ClassName, GUIDToString(IntfMD.IID)]);
IntfEntry := Obj.GetInterfaceEntry(IntfMD.IID); //得到接口的动态数组信息
IntfVTable := IntfEntry.VTable; //指向VTB表的指针
MethPos := MD.Pos * 4; { Pos is absolute to whole VMT } //定位
if MD.ResultInfo <> nil then
begin
RetIsInFPU := RetInFPU(MD.ResultInfo);
RetIsOnStack := RetOnStack(MD.ResultInfo);
RetInAXDX := IsRetInAXDX(MD.ResultInfo);
RetP := Context.GetResultPointer;     //根据context  得到返回参数的地址。
end else
begin
RetIsOnStack := False;
RetIsInFPU := False;
RetInAXDX := False;
end;
if MD.CC in [ccCDecl, ccStdCall, ccSafeCall] then
begin
if (MD.ResultInfo <> nil) and (MD.CC = ccSafeCall) then
asm PUSH DWORD PTR [RetP] end;    //把函数返回参数压入堆栈中。
for I := MD.ParamCount - 1 downto 0 do   //遍历参数。
begin
DataP := Context.GetParamPointer(I);    //指向一个参数地址：
if IsParamByRef(MD.Params[I].Flags,MD.Params[I].Info, MD.CC) then {基本类型}
asm
PUSH DWORD PTR [DataP]       //压入堆栈。
end
else
begin
ParamBytes := GetStackTypeSize(MD.Params[I].Info, MD.CC);    {特殊类型}
PushStackParm(DataP, ParamBytes);
Inc(TotalParamBytes, ParamBytes);
end;
end;
asm PUSH DWORD PTR [Unk] end;         //压入Iunknown指针
if RetIsOnStack and (MD.CC <> ccSafeCall) then
asm PUSH DWORD PTR [RetP] end;
end
else if MD.CC = ccPascal then
begin
for I := 0 to MD.ParamCount - 1 do
begin
DataP := Context.GetParamPointer(I);
if IsParamByRef(MD.Params[I].Flags,MD.Params[I].Info, MD.CC) then
asm
PUSH DWORD PTR [DataP]
end
else
begin
//PushStackParm(DataP, GetStackTypeSize(MD.Params[I].Info, MD.CC));
ParamBytes := GetStackTypeSize(MD.Params[I].Info, MD.CC);
PushStackParm(DataP, ParamBytes);
Inc(TotalParamBytes, ParamBytes);
end;
end;
if RetIsOnStack then
asm PUSH DWORD PTR [RetP] end;
asm PUSH DWORD PTR [Unk] end;
end else
raise Exception.CreateFmt(SUnsupportedCC, [CallingConventionName[MD.CC]]);
if MD.CC <> ccSafeCall then
begin
asm
MOV DWORD PTR [Temp], EAX   //把ＥＡＸ保存到临时变量中
MOV DWORD PTR [Temp1], ECX //把ＥCＸ保存到临时变量中
MOV EAX, MethPos     //函数定位的地方
MOV ECX, [IntfVtable]   //虚拟表的入口
MOV ECX, [ECX + EAX]   //真正调用的地址
CALL ECX
MOV DWORD PTR [RetEAX], EAX //把结果返回的信息保存在变量RetEAX（低位）
MOV DWORD PTR [RetEDX], EDX //把结果返回的信息保存在变量RetEDX（高位）
MOV EAX, DWORD PTR [Temp]    //恢复寄存器ＥＡＸ
MOV ECX, DWORD PTR [Temp1]//恢复寄存器ＥＣＸ
end;
end else
begin
asm
MOV DWORD PTR [Temp], EAX
MOV DWORD PTR [Temp1], ECX
MOV EAX, MethPos
MOV ECX, [IntfVtable]
MOV ECX, [ECX + EAX]
CALL ECX
CALL System.@CheckAutoResult
MOV DWORD PTR [RetEAX], EAX
MOV DWORD PTR [RetEDX], EDX
MOV EAX, DWORD PTR [Temp]
MOV ECX, DWORD PTR [Temp1]
end;
end;
if MD.CC = ccCDecl then /如果是ＣＣＤＥＣＬ方式，必须自己清除使用的堆栈。
asm
MOV EAX, DWORD PTR [TotalParamBytes]
ADD ESP, EAX
end;
//调用后，返回参数的处理：
if MD.ResultInfo <> nil then
begin
if MD.CC <> ccSafeCall then //返回类型不为ccSafeCall时，必须进行处理。
begin
if RetIsInFPU then //tkFloat类型：
begin
GetFloatReturn(RetP, GetTypeData(MD.ResultInfo).FloatType);
end else if not RetIsOnStack then
begin
if RetInAXDX then //tkInt64整型６４位类型处理：
asm
PUSH EAX
PUSH ECX
MOV EAX, DWORD PTR [RetP]
MOV ECX, DWORD PTR [RetEAX]
MOV [EAX], ECX
MOV ECX, DWORD PTR [RetEDX]
MOV [EAX + 4], ECX
POP ECX
POP EAX
end
else
asm                     //堆栈类型：
PUSH EAX                      //EAX入栈
PUSH ECX                      //ECX入栈
MOV EAX, DWORD PTR [RetP]    //返回地址ＭＯＶ到ＥＡＸ
MOV ECX, DWORD PTR [RetEAX] // RetEAX中是调用后得到的值
MOV [EAX], ECX//把调用后的结果写入返回的地址中
POP ECX                        //ECX出栈
POP EAX                        //EAX出栈 (先入后出)
end;
end;
end;
end;
{$IFDEF LINUX}
except
// This little bit of code is required to reset the stack back to a more
// resonable state since the exception unwinder is completely unaware of
// the stack pointer adjustments made in this function.
asm
MOV EAX, DWORD PTR [TotalParamBytes]
ADD ESP, EAX
end;
raise;
end;
{$ENDIF}
end;












FSoapDispatcher.DispatchSOAP(Path, SoapAction, XMLStream, RStream, BindingTypeIn);
返回调用后的内存块为。
Response.ContentStream := RStream;
然后再发送给客户端。
到这里，基本上客户端和服务器端都进行了分析。





