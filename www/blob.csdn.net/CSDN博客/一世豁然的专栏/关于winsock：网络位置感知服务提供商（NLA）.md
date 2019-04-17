# 关于winsock：网络位置感知服务提供商（NLA） - 一世豁然的专栏 - CSDN博客





2018年10月15日 12:47:28[一世豁然](https://me.csdn.net/Explorer_day)阅读数：113








运行Microsoft Windows的个人计算机通常具有多个网络连接，例如连接到不同网络的多个网络接口卡（NIC），或物理网络连接和拨号连接。 Windows套接字能够在一段时间内枚举可用的网络接口，但以前无法获得有关网络连接的某些关键信息。 这包括诸如连接Windows计算机的逻辑网络或多个接口是否连接到同一网络的信息。

网络位置感知服务提供程序（通常称为NLA）使Windows套接字2应用程序能够识别Windows计算机所连接的逻辑网络。 此外，NLA使Windows套接字应用程序能够识别给定应用程序保存特定信息的物理网络接口。 NLA实现为通用Windows套接字2名称解析服务提供程序。





一、NLA的作用

网络位置感知（NLA）服务提供商对于可能在不同网络之间移动的计算机或设备以及在有多个网络可用时选择最佳配置至关重要。 例如，在物理网络之间漫游的无线计算机可以使用NLA基于关于其可用网络连接的信息来确定适当的配置。 当多宿主计算机与一个网络建立物理连接，同时通过拨号连接或隧道连接到另一个网络时，NLA也证明是有价值的。

过去，开发人员必须获取有关逻辑网络接口的信息，因此需要根据大量不同的网络信息做出有关网络连接的决策。 在这种情况下，开发人员必须根据IP地址，接口子网，与接口关联的域名系统（DNS）名称，NIC的MAC地址，无线网络名称或选择适当的网络接口。 其他网络信息。 NLA通过提供标准接口来缓解此问题，该接口用于枚举逻辑网络附件信息，将其与物理网络接口信息相关联，然后在先前返回的信息失效时提供通知。

NLA提供以下网络位置信息：

逻辑网络身份

NLA首先尝试通过其DNS域名来识别逻辑网络。 如果逻辑网络没有域名，NLA会从存储在注册表中的自定义静态信息中识别网络，最后从其子网地址中识别。

逻辑网络接口

对于连接计算机的每个网络，NLA提供唯一标识物理接口（如NIC）或逻辑接口（如RAS连接）的AdapterName。 然后，AdapterName可以与IP Helper API中提供的功能一起使用，以获得更多接口特性。

NLA将逻辑网络实现为服务类，具有关联的类GUID和属性。 NLA返回信息的每个逻辑网络都是该服务类的实例。





二、查询NLA

要获取无效逻辑网络的通知，请使用WSANSPIoctl函数注册网络位置更改事件。 可以使用两种方法来确定先前有效的网络位置是否已变为无效：轮询方法或使用重叠I / O或Windows消息传递的通知。

使用WSALookupServiceBegin，WSALookupServiceNext和WSALookupServiceEnd函数形成查询以枚举所有可用的逻辑网络。 在本节的其余部分中，将从WSALookupServiceBegin函数开始单独解释这些函数中每个函数的使用。

> 
注意

NLA需要Mswsock.h头文件，默认情况下不包含在Winsock2.h文件中。




一）、第1步：启动查询

为了快速参考，WSALookupServiceBegin函数具有以下语法：

```
INT WSALookupServiceBegin(
  LPWSAQUERYSET lpqsRestrictions,
  DWORD dwControlFlags,
  LPHANDLE lphLookup
);
```

NLA支持以下dwControlFlags查找标志：

LUP \ _RETURN \ _NAME LUP \ _RETURN \ _COMMENT LUP \ _RETURN \ _BLOB LUP \ _RETURN \ _ALL LUP \ _DEEP

这些标志将后续调用中返回的结果集限制为WSALookupServiceNext，函数包含指定类型字段的网络。 例如，在WSALookupServiceBegin函数调用的dwControlFlags参数中指定LUP_RETURN_BLOB会将后续调用WSALookupServiceNext的结果集限制为包含BLOB信息的网络。 使用LUP_RETURN_ALL标志等同于指定LUP_RETURN_NAME，LUP_RETURN_COMMENT和LUP_RETURN_BLOB，但不指定LUP_DEEP。

有关这些查找标志的说明，请参阅WSALookupServiceBegin函数参考页面。

NLA在lphLookup参数中返回的查找句柄对NLA是私有的，不应修改。 由于返回的句柄对NLA是私有的，因此WSAGetOverlappedResult等函数不可用。

成功完成后，NLA返回零，如WSALookupServiceBegin函数参考页面中所定义。 否则，NLA支持以下错误代码。
|Error|Meaning|
|----|----|
|WSANOTINITIALISED|未成功调用WSAStartup函数来初始化NLA。|
|WSAEINVAL|一个或多个参数无效，或者函数调用中指定的参数适用于IP以外的协议。|
|WSASERVICE_NOT_FOUND|在lpqsRestrictions参数中传递的WSAQUERYSET结构的lpServiceClassId参数包含无效的GUID。|
|WSANO_DATA|LUP_CONTAINERS标志在dwControlFlags参数中指定。|
|WSAEFAULT|尝试访问用户提供的参数时发生访问冲突。|
|WSASYSNOTREADY|NLA服务无法处理请求。|
|WSA_NOT_ENOUGH_MEMORY|NLA或NLA服务无法分配足够的内存来处理此请求。|



二）、第2步：执行查询

查询NLA的下一步需要使用WSALookupServiceNext函数。 为了快速参考，WSALookupServiceNext函数具有以下语法：

```
INT WSALookupServiceNext(
  HANDLE hLookup,
  DWORD dwControlFlags,
  LPDWORD lpdwBufferLength,
  LPWSAQUERYSET lpqsResults
);
```

lLookup参数是从上一次调用WSALookupServiceBegin函数返回的查找句柄。

dwControlFlags参数支持以下标志：

LUP \ _RETURN \ _NAME LUP \ _RETURN \ _COMMENT LUP \ _RETURN \ _BLOB LUP \ _RETURN \ _ALL LUP \ _FLUSHPREVIOUS

这些标志独立于WSALookupServiceBegin函数调用中支持的标志。 请注意，在上一次调用WSALookupServiceBegin函数中指定的任何约束都会限制查找; 使用WSALookupServiceNext函数添加标志以尝试扩展超出WSALookupServiceBegin调用中指定的约束的查询将被静默忽略。 但是，允许指定比WSALookupServiceBegin调用中指定的更严格的标志集。

如果lpqsResults中详述的网络是活动网络，则会在lpqsResults中返回的WSAQUERYSET结构的lpBlob成员中指定附加一系列NLA_BLOB结构。 这些NLA_BLOB结构可以链接，并且可以通过遍历列表来枚举，而NLA_BLOB.header.nextOffset非零。 要获取所有网络位置信息的结果，请继续调用WSALookupServiceNext函数，直到返回WSA_E_NO_MORE错误，如WSALookupServiceNext参考页中所述。

WSALookupServiceNext函数还与WSANSPIoctl函数结合使用，以接收网络更改的通知。 有关详细信息，请参阅NLA的通知。

成功完成后，NLA返回零。 NLA的客户端应继续调用WSALookupServiceNext函数，直到返回WSA_E_NO_MORE，表示已返回有关可用网络的所有信息。

否则，调用NAL的WSALookupServiceNext函数支持以下错误代码。
|Error|Meaning|
|----|----|
|WSANOTINITIALISED|未成功调用初始化NLA的WSAStartup函数。|
|WSA_INVALID_HANDLE|hLookup参数中提供的查找句柄不是有效的NLA SP句柄。 客户端必须首先调用WSALookupServiceBeginfunction并接收有效的NLA SP句柄以获取查询结果。|
|WSAESYSNOTREADY|NLA服务无法处理此请求。|
|WSAEFAULT|lpdwBufferLength参数中指定的缓冲区大小不足以保存lpqsResults指向的结果。 所需的缓冲区在lpdwBufferLength中指定; 如果客户端无法提供足够大的缓冲区，则客户端可以调用WSALookupServiceNextfunction并将dwControlFlags设置为LUP_FLUSHPREVIOUS以跳过该条目。|
|WSA_NOT_ENOUGH_MEMORY|由于呼叫过程中的内存不足，NLA无法从NLA系统服务获取网络信息。|
|WSA_E_NO_MORE|没有其他网络可以枚举查询。|



三）、第3步：终止查询

当对NLA的所有查询都完成且应用程序不再需要使用NLA时，应该调用WSALookupServiceEnd函数。 如果应用程序将根据提交的查询收到更改通知，请不要调用WSALookupServiceEnd。 有关接收通知的详细信息，请参阅NLA的通知。 与大多数Windows套接字服务提供商一样，NLA维护其客户端的引用计数。 在完成对NLA的查询时调用WSALookupServiceEnd函数可以释放NLA不再需要的系统资源。

NLA支持WSALookupServiceEnd函数调用的以下错误代码。
|Error|Meaning|
|----|----|
|WSANOTINITIALISED|未成功调用WSAStartup函数来初始化NLA。|
|WSA_INVALID_HANDLE|hLookup参数中提供的句柄不是有效的NLA SP句柄。|





三、来自NLA的通知

NLA能够向其客户提供网络位置变化的通知。 用于请求更改事件通知的机制是WSALookupServiceBegin，WSANSPIoctl和WSALookupServiceNext函数的组合。

为了从NLA接收更改通知，客户端必须首先调用WSALookupServiceBegin以获取有效的NLA SP查找句柄。 接下来，客户端可以按任何顺序调用WSALookupServiceNext或WSANSPIoctl; 要注册通知，请使用dwControlCode参数中设置的SIO_NSP_NOTIFY_CHANGE控制代码调用WSANSPIoctl函数。

WSALookupServiceNext函数返回WSA_E_NO_MORE作为集合定界符。 当客户端使用WSANSPIoctl函数注册通知并且WSALookupServiceNext返回WSA_E_NO_MORE时，再次调用WSALookupServiceNext会显示是否发生了更改：
- 如果自上一个WSA_E_NO_MORE以来未发生任何更改，则返回WSA_E_NO_MORE。
- 如果发生了更改，或者发生了更改并进行了轮询调用，则WSALookupServiceNext函数调用将网络作为WSAQUERYSET结构返回，并在其dwOutputFlags成员中设置以下标志之一：

RESULT\_IS\_ADDED RESULT\_IS\_CHANGED RESULT\_IS\_DELETED

自从使用WSALookupServiceBegin函数调用获取NLA查找句柄以来，或者自上次枚举导致WSA_E_NO_MORE错误以来，为更改的任何字段提供了更改通知。 提供所有更改的网络位置信息后，将返回WSA_E_NO_MORE。 客户端可以随时在同一查询句柄上重新发出WSANSPIoctl函数调用，一次一个，并设置SIO_NSP_NOTIFY_CHANGE标志。 此功能使客户端能够持续回收查询句柄，从而减轻客户端自身维护更改状态信息的负担。 一旦客户端不再需要更改通知，它应该使用WSALookupServiceEnd函数关闭查询句柄。




四、使用NLA注册服务实例

NLA客户端可以在系统范围内记录网络配置信息，使得将来的查询可以返回指定的配置信息，无论网络是否处于活动状态。 此功能允许NLA客户端在多个应用程序中影响一致的网络信息用户体验。



一）、参数

要使用Network Location Awareness服务提供程序注册服务实例，请使用WSASetService函数。 为了正确注册服务实例，必须使用适当的信息设置WSASetService函数的某些参数，如本节所述。 为了快速参考，WSASetService函数具有以下语法：

```
INT WSASetService(
  LPWSAQUERYSET lpqsRegInfo,
  WSAESETSERVICEOP essOperation,
  DWORD dwControlFlags
);
```

对于lpqsRegInfo参数，根据NLA SP查询结果提供WSAQUERYSET结构，或者根据查询NLA中指定的NLA SP查询要求构建WSAQUERYSET结构。

essOperation参数支持的操作如下：

RNRSERVICE_REGISTER

lpqsRegInfo中提供的WSAQUERYSET结构中定义的网络通过将网络实例存储在当前用户的注册表配置单元中而允许活动用户持久化，这允许模拟。

RNRSERVICE_DELETE

如果lpqsRegInfo中提供的WSAQUERYSET结构中定义的网络是持久的，则它将被删除。

可以通过以下选项修改essOperation参数中指定的操作，可以使用二进制OR逻辑指定：

NLA_FRIENDLY_NAME

与RNRSERVICE_REGISTER一起使用时，将检查lpqsRegInfo中定义的网络的lpszComment字段的有效性并持久存储。 与RNRSERVICE_DELETE一起使用且定义的网络具有友好名称时，将删除友好名称，但网络条目保持不变。

NLA_ALLUSERS_NETWORK

与RNRSERVICE_REGISTER一起使用时，该条目将永久存储在HKEY_LOCAL_MACHINE下，使其在查询期间可用于本地计算机上的所有用户。 与RNRSERVICE_DELETE一起使用时，将从HKEY_LOCAL_MACHINE中删除指定的网络。 如果指定的网络不存在，则返回错误。 要从当前用户的注册表配置单元中删除网络，不得指定此标志。 此标志仅在本地系统管理员的安全上下文中有效。

NLA支持WSASetService函数调用的以下错误代码：
|Error|Meaning|
|----|----|
|WSANOTINITIALISED|未成功调用WSAStartup函数来初始化NLA。|
|WSAEACCESS|NLA_ALLUSERS_NETWORK在dwControlFlags中指定，而不在本地系统管理员的安全上下文中。|
|WSAEALREADY|指定的网络已经以请求的方式持久存储，并且在dwControlFlags中未指定任何标志以指示对现有条目的更新。|
|WSAEAFNOSUPPORT|指定了一个没有支持的协议族。 NLA仅支持IP协议系列。|
|WSAEPFNOSUPPORT|指定了一个没有支持的协议。 NLA仅支持IP协议。|



