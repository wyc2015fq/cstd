
# RPC编程 - 高科的专栏 - CSDN博客

2012年09月20日 09:30:24[高科](https://me.csdn.net/pbymw8iwm)阅读数：879


图 3 说明在客户机和服务器之间完成 RPC 涉及的步骤。
图 3. 在客户机和服务器之间完成 RPC 涉及的步骤
![完成 RPC 涉及的步骤](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/figure3.gif)
服务器 RPC 应用程序初始化期间它会向 RPC 运行时库注册接口。需要注册接口是因为，客户机在向服务器发出远程过程调用时，要检查它是否与服务器兼容。服务器创建绑定信息并把信息存储在名称服务数据库中，客户机可以访问这个数据库并寻找到服务器的连接信息。服务器如果使用动态端点，那么它把端点信息放在服务器系统上的本地端点映射数据库中。本地端点映射数据库用于存储在此主机上运行的 RPC 服务器进程的所有端点。服务器启动，监听来自客户机的远程过程调用。
客户机发出远程过程调用，此时它会联系名称服务数据库，以寻找服务器系统的相关信息。RPC 运行时库使用这些信息联系服务器系统上的本地端点映射数据库，了解服务器进程在哪个端点上监听到达的 RPC。
客户机找到服务器之后，客户机 stub 把远程过程调用和参数转换为服务器 stub 可以理解的格式，然后交给客户机运行时，由客户机运行时通过网络传输这些信息。
服务器 RPC 运行时库接收到达的 RPC 调用，把它传递给服务器 stub，服务器 stub 把它转换为服务器可以理解的格式。
执行 RPC 调用之后，服务器 stub 和服务器运行时把结果发送回客户机。
客户机 RPC 运行时接收执行结果，传递给客户机 stub，客户机 stub 再把它传递给客户机进程。客户机应用程序从客户机 stub 接收结果并完成 RPC 调用。
本文的其余部分解释完成远程过程调用在客户端和服务器端上需要使用的例程。
[回页首](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/#ibm-pcon)
服务器端运行时例程类别
在服务器上管理接口
本节概述在服务器应用程序中向 RPC 运行时注册接口规范和取消注册的例程。`rpc_server_register_if`例程用于注册接口，`rpc_server_unregister_if`用于取消注册。
rpc_server_register_if如[图 3](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/#fig3)所示，RPC 服务器应用程序必须执行的第一步是向运行时注册它的接口。这个例程用于完成这个步骤，这需要提供 IDL 编译器生成的接口句柄、类型 UUID 和管理器例程的入口点向量。
原型和 I/O 参数如下：
void rpc_server_register_if(IN rpc_if_handle_t  if_spec, 
IN uuid_p_t mgr_type_uuid, IN rpc_mgr_epv_t  mgr_epv, OUT	
unsigned32 *status);|
这个例程首先检查传递的管理器入口点向量 mgr_epv 是否是 NULL。如果是 NULL，它就使用接口规范中提供的默认向量。接下来，它计算接口 uuid 的散列值，检查 uuid 的格式是否有效。然后，获得一个锁，以便在访问接口注册表之前保护它。它在接口注册表中执行散列查询，寻找请求的接口规范。如果查询失败，就创建一个新的接口条目并添加到注册表中。把 mgr_epv 和 type_uuid 放在一个结构中，把这个结构的引用插入请求的接口的链表。现在，这个接口已经注册了。
rpc_server_unregister_if例程用于从接口注册表中删除接口。
在服务器上管理协议序列
rpc_server_use_protseq通过调用这个例程，让 RPC 运行时使用指定的协议序列。它为所需的 Network Address Family (NAF) 创建描述符，并把它添加到正在监听的服务器描述符池中。它使用一个动态分配的名称作为 Network Address Family Service 的描述符。
原型和 I/O 参数如下：
void rpc_server_use_protseq (IN unsigned_char_p_tprotseq, IN unsigned32
    max_call_requests,OUT unsigned32 *status);|
这个例程中的 Max_calls 输入参数表示对于特定的协议序列所允许的最大并发调用数量。这个例程用 NULL 值表示端点来调用 rpc_server_protseq_ep 例程，这会搜索 RPC 协议 ID 序列表，返回给定的 RPC 协议序列字符串的协议 ID。如果在表中没有找到这个协议序列，这个例程返回错误。接下来，用协议序列 ID、端点和 NAF ID 创建套接字描述符，并把它添加到正在监听的描述符池中。
rpc_server_use_all_protseqs这个例程让 RPC 运行时在运行时和操作系统支持的所有 RPC 协议序列上监听 RPC。对于支持的每个协议序列，这个例程调用rpc_server_use_protseq创建套接字描述符，并把它添加到正在监听的描述符池中。
原型和 I/O 参数如下：
void rpc_server_use_all_protseqs(IN  unsigned32 max_call_requests, OUT unsigned32
    *status);|
这个例程首先通过调用rpc_network_inq_protseqs获得运行时支持的有效协议序列的向量。对于支持的每个协议序列，这个例程调用rpc_server_use_protseq创建服务器套接字描述符，并把它添加到正在监听的描述符池中。更多信息请参见 rpc_server_use_protseq。
下面是用于管理协议序列的其他重要例程：
rpc_server_use_all_protseq_if：这个例程让 RPC 运行时监听所有支持的协议序列。对于每个支持的协议序列，它检查指定的接口规范是否具有已知的端点。如果有，就通过调用 rpc_server_use_protseq_ep 根据这个端点创建套接字。
rpc_server_use_protseq_if：这个例程与 rpc_server_use_protseq 相似，但是它要查询给定接口规范的端点并据此创建套接字。
管理绑定句柄
rpc_server_inq_bindings通过调用这个例程获得服务器绑定句柄的向量，这些句柄引用这个服务器的所有绑定信息。一组服务器绑定句柄称为向量。可以对这些句柄发出 RPC 调用。注意，这些绑定不包含对象 UUID。
原型和 I/O 参数如下：
void rpc_server_inq_bindings(OUT rpc_binding_vector_t 
**binding_vector (OUT unsigned32 *status);|
这个例程获得一组服务器绑定句柄。绑定句柄是在服务器应用程序调用管理协议序列中提到的任何例程时创建的。返回的绑定向量可能包含具有已知端点或动态端点的句柄。返回的端点类型取决于服务器应用程序在管理协议序列时调用的例程。如果没有绑定句柄，这个例程返回rpc_s_no_bindings并在 binding_vector 参数中返回 NULL。
把服务器导出到名称服务
rpc_ns_binding_export一般情况下，服务器会在名称服务数据库中公开地列出它的接口和对象 UUID，让任何客户机应用程序都可以使用此信息联系它。使用rpc_ns_binding_export调用把此信息导出到数据库。服务器不一定非要把此信息导出到数据库；如果不导出，那么只有已经知道服务器绑定的客户机才能连接它。
原型和 I/O 参数如下：
void rpc_ns_binding_export(IN unsigned32 entry_name_syntax,IN unsigned_char_p_t
entry_name, IN rpc_if_handle_t if_spec, IN rpc_binding_vector_p_t binding_vector,IN
uuid_vector_p_t  object_uuid_vector, OUT unsigned32  *status);|
这个例程使用服务器的多个绑定句柄或对象 UUID 建立一个名称服务数据库条目。这个例程首先检查 if_spec 或 binding_vec 是否为 NULL，以及 object_uuid_vec 是否为 NULL。如果是，它返回 rpc_s_nothing_to_export。它还统计要从绑定向量导出的非 NULL 绑定和非 NULL UUID 的数量。然后，筛选掉绑定向量中所有重复的绑定。接下来，检查条目名称的语法是否正确，根据运行时表示创建这个条目名称的名称服务表示。然后，在名称空间中创建名称服务条目。检查是否指定了接口，把每个非
 NULL 绑定转换为塔表示，最后导出。还会导出所有非 NULL 对象 UUID。
rpc_ns_binding_inq_entry_name这个例程用于根据给定的绑定句柄从名称服务数据库获取名称服务条目。
原型和 I/O 参数如下：
void rpc_ns_binding_inq_entry_name(IN rpc_binding_handle_t binding,
IN	unsigned32 entry_name_syntax, OUT unsigned_char_p_t  *entry_name,
OUT unsigned32  *status);|
这个例程检查传递的绑定句柄在名称服务数据库中是否有相应的条目名称。如果条目存在，它应该是名称服务表示格式的。把它转换为运行时表示的条目名称，然后返回给调用者。如果条目不存在，这个例程返回状态码 rpc_s_no_entry_name。
在服务器上管理端点
rpc_ep_register这个例程用于向本地端点映射数据库注册服务器的端点。只有在通过调用 rpc_server_use_protseq 或 rpc_server_use_all_protseq 例程使用动态端点时，才调用它，因为在服务器每次重新启动时端点都可能会变。如果通过调用 rpc_server_use_protseq_ep 例程使用已知端点，服务器应用程序仍然需要注册端点，因为客户机需要知道这个已知端点。如果通过调用 rpc_server_use_protseq_if 或 rpc_server_use_all_protseqs_if
 例程使用已知端点，就不需要调用这个例程，因为客户机可以通过接口规范获得端点值。
原型和 I/O 参数如下：
void rpc_ep_register(IN rpc_if_handle_t if_spec, IN rpc_binding_vector_p_t
    binding_vec, IN uuid_vector_p_t  object_uuid_vec, IN unsigned_char_p_t 
    annotation, OUT
    unsigned32 *status);|
这个例程首先检查接口句柄是否为 NULL。如果为 NULL，它返回 rpc_s_no_interfaces 错误。如果绑定向量为 NULL，或者向量中没有绑定，那么它返回 rpc_s_no_bindings。否则，它循环遍历绑定向量数组，寻找非 NULL 的绑定句柄指针。如果所有句柄为 NULL，它返回 rpc_s_no_bindings 错误。它还检查绑定向量数组中是否有不包含端点的绑定句柄，如果找到这样的句柄，它返回 rpc_s_invalid_binding 错误。找到包含端点的有效绑定句柄之后，它用本地端点映射器在数据库中注册这个端点。
监听 RPC
rpc_server_listen这个例程用于让服务器监听到达的远程过程调用。
原型和 I/O 参数如下：
void rpc_server_listen(IN	unsigned32 max_calls_exec, OUT unsigned32
*status);|
这个例程首先检查是否已经有监听器，从而确保任何时候只有一个监听器。然后，检查是否有任何服务器套接字描述符并把它们添加到监听器中，让它可以开始选择它们。接下来，启动所有调用执行者线程来监听到达的 RPC。调用执行者线程处理来自 RPC 运行时的 RPC。调用线程的数量取决于输入参数 max_calls，这个参数指定服务器可以处理的最大并发 RPC 调用数量。如果服务器收到的 RPC 超过这个最大数量，它会把多余的 RPC 排队，直到有空闲的调用执行者线程可以执行 RPC。这个例程会一直监听，直到通知它停止监听。当客户机应用程序或服务器的管理器调用
 rpc_mgmt_stop_server_listening 例程时，停止监听。如果在描述符表中没有找到服务器套接字，这个例程返回 rpc_s_no_protseq_registered 错误。

[回页首](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/#ibm-pcon)
客户端运行时例程
管理绑定句柄
绑定句柄是大多数 RPC 运行时例程的基本输入参数。可以使用绑定句柄完成不同的任务，管理这些句柄是必需的。可以根据需要把它们转换和组合为字符串格式，解析它们以寻找特定的字段值，使用它们查询对象的相关信息，使用它们获得完整的绑定，复制和释放它们。
在这个类别中（对于客户端），有两个重要的例程：
|rpc_binding_to_string_binding  rpc_ep_resolve_binding|
rpc_binding_to_string_binding这个例程把绑定数据结构转换为可读的字符串表示。字符串表示的格式通常是：
<UUID><Protocol Sequence><Network Address><Endpoint><Network
Options>|
通常，应用程序调用这个例程来了解特定 RPC 服务所用的协议、IP 地址和端口号。
原型和 I/O 参数如下：
void rpc_binding_to_string_binding (IN rpc_binding_handle_t binding_h,
OUT    unsigned_char_p_t       *string_binding, OUT unsigned32 *status)|
这个例程的输入是绑定句柄，绑定句柄指向包含绑定表示的数据结构。可以通过特定的调用获得 UUID、RPC 地址、端点、网络地址和选项。
如果绑定表示不包含 RPC 地址，就需要使用绑定入口点并通过连接查询相关联的 RPC 地址。使用网络地址入口点和 RPC 地址的引用获取网络地址、端点和网络选项详细信息。
从协议序列表获取协议序列。这个表包含 NAF 和 RPC 支持的协议的标识符。
然后，把以上所有数据组合成一个字符串并返回给调用者。使用 rpc_string_binding_composeroutine 完成这一步。
还有一个作用相反的例程，它把绑定的字符串表示转换为绑定句柄。
rpc_ep_resolve_binding这个例程用于解析一个部分绑定的绑定句柄。如果一个绑定句柄只有网络地址部分，还没有获得端点，就称之为部分绑定的。RPC 守护进程维护一个端点映射，其中包含给定接口和对象 UUID 的端点信息。RPC 运行时与这个守护进程联系，获取端点信息并返回给调用者。
原型和 I/O 参数如下：
>void rpc_ep_resolve_binding(IN rpc_binding_handle_t  binding_h, 
IN   rpc_if_handle_t       if_spec_h, OUT unsigned32      *status);|
这个例程的核心部分判断绑定句柄表示是否没有端点。如果有端点，例程把 rpc_s_ok 状态返回给调用者。查询 RPC 守护进程，首先尝试从给定的接口获取一个端点。如果找到端点，就把端点附加到绑定和绑定向量入口点中，让协议服务知道绑定信息的变化，并把端点返回给调用者。
如果接口没有端点，那么发出 RPC 调用，联系接口中指定的地址上的端点数据库。在联系数据库之前，运行时确认通信超时值没有设置为无限等待。如果是无限等待，那么运行时把它设置为最大超时值。这是因为运行时不需要一直尝试访问端点数据库。另外，为了实例化端点数据库，它要确认对象 UUID 为 NULL。有了连接端点数据库所需的完全绑定句柄之后，检查接口的协议版本与服务器协议版本的兼容性。如果不兼容，RPC 守护进程不返回端点。
与平常一样，绑定向量入口点把绑定句柄的变化告知协议服务。现在，下一个任务是在端点数据库中查找给定接口的所有兼容的塔。为此，客户机以映射塔的形式提供数据，映射塔作为查询键。映射塔是根据给定的接口规范构造的，主要包含接口 UUID、版本、协议序列、协议版本等等。以映射塔作为键，搜索端点数据库，获得所有兼容的塔的列表。然后，把一个随机选择的兼容的塔转换为 RPC 地址，从中提取出端点。把这个端点附加到接口的绑定句柄中并返回给调用者。
通过名称服务寻找服务器
名称服务数据库是服务器位置信息的存储库，其中的信息表示为绑定信息。客户机应用程序通常使用名称服务 RPC 例程：
获取数据库的引用
获取兼容绑定的列表
查找绑定
选择绑定
把搜索和获取标记为已完成
rpc_ns_binding_import_begin通常，RPC 服务器应用程序会公布或导出它的位置，位置信息以绑定信息的形式存储在名称服务数据库中。客户机通过这个数据库了解服务器的位置。客户机应用程序使用 rpc_ns_binding_import_begin 例程获取数据库的引用，这个引用用于在名称服务数据库中搜索绑定信息。
原型和 I/O 参数如下：
void rpc_ns_binding_import_begin (IN unsigned32	entry_name_syntax
IN	unsigned_char_p_t	entry_name, IN rpc_if_handle_t  if_spec,
IN	uuid_p_t object_uuid, OUT	rpc_ns_handle_t  *import_context,
OUT unsigned32	*status);|
第一个参数entry_name_syntax是一个整数值，它指定输入entry_name的方式。entry_name是搜索字符串的名称。还可以指定接口的句柄和对象 UUID。
这个例程调用 rpc_ns_binding_lookup_begin，它执行所需的初始化。调用一个宏检查传递的条目名称语法和条目名称是否为 NULL。如果是，就使用默认值。
为名称服务条目表示结构分配内存，其中包含条目名称和语法。然后，把条目名称展开成完整的 DNS 名称并返回。然后初始化一个查找数据结构，这是一个链表。这个结构中的重要字段包括接口规范名称服务缓存过期时间，这用于确认是否必须刷新缓存。然后创建并初始化一个查找节点，它也有名称服务条目。把这个节点添加到查找列表的开头，作为导入上下文返回节点的引用。现在，客户机应用程序可以使用这个导入上下文从名称服务数据库获取绑定信息。
rpc_ns_binding_import_next以 import_context 的形式获得查找结构的引用之后，客户机应用程序可以使用这个例程获取给定接口的兼容绑定信息。还可以使用对象 UUID。
原型和 I/O 参数如下：
void rpc_ns_binding_import_next ( [in] rpc_ns_handle_t  import_context,    
[out] rpc_binding_handle_t *binding,  [out] unsigned32	*status);|
第一个任务是查找给定的名称空间条目的兼容绑定。为此，需要获得包含所有兼容绑定的绑定向量。这要调用 rpc_ns_binding_lookup_next 例程。在这个例程中，首先检查是否有非空的上下文句柄。上下文句柄是用于在名称服务数据库中执行查找的引用。然后为绑定向量分配内存并初始化。然后进入一个循环，获取第一个节点并尝试解析这个节点。解析是指获得兼容绑定的完整列表，或者在节点列表中添加有效的成员。解析的类型取决于名称空间条目的类型。在这里，类型可以是搜索兼容的绑定，或者搜索由必须添加的组概要条目组成的上下文。名称服务数据库中的绑定信息存储在称为塔的数据结构中。

图 4. 简单的塔表示
![图 4. 简单的塔表示](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/figure4.gif)
如果塔的协议序列、接口 id、传输语法和协议版本号与客户机的接口匹配，就找到了兼容的绑定。在 rpc_ns_binding_import_next 中，根据解析后返回的状态码处理不同的情况。如果绑定向量满了，或者已经搜索了所有节点，那么 rpc_ns_binding_lookup_next 例程返回。当绑定向量达到一个预定义的值时，就认为它已经满了。从 rpc_ns_binding_lookup_next 返回之后，执行选择调用，从向量中随机选择一个兼容的绑定并返回给调用者。
管理接口信息
客户机应用程序使用 RPC 运行时提供的以下例程查询接口的详细信息：
rpc_if_inq_id这个例程查询接口的标识符和接口 UUID 的主版本或次版本。需要这些数据的情况包括对比接口和塔标识符、从名称服务数据库中删除多个绑定等等。
原型和 I/O 参数如下：
void rpc_if_inq_id(IN rpc_if_handle_t  if_spec, OUT rpc_if_id_t
*if_id, OUT unsigned32 *status);|
过程很简单：它从接口表示结构 (if_spec) 获取接口的 UUID 和版本，然后把这些数据放进接口标识符结构中 (if_id)。这个结构的字段是接口的 UUID、主版本和次版本。从 if_spec 获得的版本号是一个 32 位的值，将之与 0xffff 进行 AND 计算可以得到主版本，右移 16 次可以得到次版本。把这些数据放进 if_id 中并把它的引用返回给调用者。
rpc_if_id_vector_free应用程序的退出处理函数或清理例程可以调用这个例程。
原型和 I/O 参数如下：
void rpc_if_id_vector_free (IN/OUT rpc_if_id_vector_p_t  *if_id_vector,
>OUT unsigned32  *status);|
这个简单的例程释放运行时分配给接口 ID 向量结构的内存。这个结构包含接口 ID 结构的指针和接口的数量。如果输入向量本身为空，那么返回 rpc_s_invalid_arg 状态。否则，循环遍历向量，释放每个接口的内存。最后，还要释放分配给向量的内存并把 NULL 向量返回给调用者。
查询协议序列
rpc_network_inq_protseqs了解给定主机上支持的所有协议序列有助于做出应用程序和运行时级的决策。例如，RPC 运行时应该只监听主机支持的有效协议序列。RPC 应用程序只需查询主机支持的协议序列。在这种情况下，需要调用这个例程。
原型和 I/O 参数如下：
void rpc_network_inq_protseqs(OUT rpc_protseq_vector_p_t *protseq_vector, 
OUT unsigned32	*status)|
首先检查协议序列的数量是否为零。如果为零，这个例程返回并指出没有协议序列。否则，为保存协议序列字符串分配所需的内存。然后遍历协议序列 ID 表，把协议字符串复制到输出参数 ‘protseq_vector’ 中。
rpc_network_is_protseq_valid在使用协议之前，RPC 应用程序可能希望确认主机上是否支持给定的协议序列。在这种情况下，以协议序列字符串作为输入调用 rpc_network_is_protseq_valid。经常在调用 rpc_server_use_protseq 之前调用这个例程。
原型和 I/O 参数如下：
boolean rpc_network_is_protseq_valid( IN unsigned_char_p_t	protseq, 
OUT unsigned32 *status)|
这个例程在协议序列 ID 表中搜索给定的输入字符串。如果没有找到，就返回 rpc_s_protseq_not_supported。协议序列 ID 表包含一个布尔字段，表示是否支持给定的协议序列。在运行时初始化期间填充这个字段。这个例程遍历这个表，获取网络地址系列、接口和协议。这三个参数用于在操作系统上打开套接字，如果成功，就意味着协议支持字段被设置为 TRUE。
把相同的值返回给 rpc_network_is_protseq_valid() 的调用者。如果返回值为 FALSE，那么 status 字段设置为 rpc_s_protseq_not_supported。如果没有找到与给定的协议序列字符串匹配的协议序列，那么 status 字段被设置为 rpc_s_invalid_rpc_protseq。

[回页首](http://www.ibm.com/developerworks/cn/aix/library/au-rpc_programming/#ibm-pcon)
共用的例程
管理端点映射
rpc_mgmt_ep_elt_inq_beginRPC 应用程序要查看端点的内容时，它首先必须获得访问端点所需的引用。在这种情况下，调用这个例程。
原型和 I/O 参数如下：
void rpc_mgmt_ep_elt_inq_begin (IN rpc_binding_handle_t input_binding,
IN	unsigned32 inquiry_type, IN rpc_if_id_p_t if_id, IN unsigned32 vers_option, 
IN	uuid_p_t object_uuid,OUT rpc_ep_inq_handle_t *inquiry_context,
OUT unsigned32	*status);|
这个例程创建一个查询上下文，以用来访问本地或远程端点。为了获得这个上下文，首先需要绑定由输入绑定参数指定的本地或远程主机上的端点。如果传递的参数是 NULL，那么运行时尝试绑定包含支持的协议序列之一的本地端点。在此之后，把字符串绑定转换为绑定句柄。如果输入绑定参数不是 NULL，就意味着端点在远程主机上。从这个输入绑定句柄获取 RPC 地址和绑定数据结构的其他字段值，比如协议版本、超时等，把它们复制到另一个句柄表示中。rpc_binding_copy 例程完成这个任务。这里的目标只是获取端点数据库的引用。因此，从
 rpc_binding_copy 获得新的句柄之后，清除与 RPC 地址相关联的端点。把获取的句柄复制到端点查询上下文结构的适当字段中。这个结构还有其他字段，比如查询类型、对象 UUID、接口 ID 等等。适当地填充所有结构字段并把这个结构的引用作为 inquiry_context 返回给调用者。
rpc_mgmt_ep_elt_inq_next调用这个例程查看端点数据库的内容。一般情况下，端点数据库的内容包括 UUID、接口 ID、注解表示和字符串绑定信息。
原型和 I/O 参数如下：
void rpc_mgmt_ep_elt_inq_next (IN    rpc_ep_inq_handle_t  inquiry_context,
OUT  rpc_if_id_t *if_id, OUT rpc_binding_handle_t *binding,
OUT  uuid_t *object_uuid, OUT unsigned_char_p_t  *annotation,
OUT  unsigned32 *status);|
使用从 rpc_mgmt_ep_elt_inq_begin 例程获得的查询上下文引用访问端点数据库。与名称服务数据库一样，信息在端点数据库中存储为塔的形式。把获取的每个塔表示转换为绑定表示。如果转换成功，就从查询上下文引用获取接口 ID、对象 UUID 和注解等其他数据。在一个循环中进行这个处理，直到端点数据库中没有元素为止。
rpc_mgmt_ep_unregister当服务器不再希望在端点中注册时，调用这个例程。然后，服务器的位置信息会从端点数据库中删除掉。
原型和 I/O 参数如下：
void rpc_mgmt_ep_unregister(IN  rpc_binding_handle_t ep_binding, 
IN rpc_if_id_p_t  if_id, IN rpc_binding_handle_t binding,
IN	uuid_p_t object_uuid, OUT unsigned32 *status);|
可以对指定的接口、绑定句柄列表（每个支持的协议一个句柄）和指定的对象 UUID 列表执行取消注册。检查所有输入参数之后，获取访问端点数据库的引用。对于输入列表中指定的每个绑定，获取塔的引用列表，用于访问塔表示。另外，对于输入列表中指定的每个对象 UUID，执行从端点中删除它们的命令。然后释放塔，再释放塔的引用。继续处理，直到删除输入列表中指定的所有绑定为止。
管理本地或远程应用程序
所有管理例程都用于了解服务器的监听状态、服务器的统计数据、注册的接口列表等等。根据绑定句柄在本地或远程执行这个调用。NULL 绑定句柄表示在本地执行调用。
rpc_mgmt_is_server_listening应用程序有时候希望确认某个服务器是否正在监听。把绑定句柄作为输入传递给这个例程，就可以查明监听状态。true 值表示服务器正在监听到达的请求。
原型和 I/O 参数如下：
boolean32 rpc_mgmt_is_server_listening(IN rpc_binding_handle_t binding_handle,  OUT 
unsigned32 	*status);|
正如刚才提到的，NULL 绑定句柄表示在本地主机上执行这个调用。否则，先检查绑定句柄是否可以远程使用，然后在远程主机上执行调用。每个正在进行监听的服务器维护一个标志，如果在注册的端口上有正在监听的套接字，这个标志就设置为 true。如果运行时无法激活监听套接字，这个标志就设置为 false。
执行查询时，把这个标志的值返回给进行调用的应用程序。应用程序可以通过调用 rpc_mgmt_stop_server_listening 要求服务器停止监听。完成所有未处理或正在处理的请求之后，服务器停止监听，不再接受任何新的请求。
rpc_mgmt_inq_stats有时候，需要检查接口，确认它工作正常。在这种情况下，获取统计数据有助于调试，比如接口发送或接收的数据包数量和发送或接收的 RPC 调用数量。
原型和 I/O 参数如下：
void rpc_mgmt_inq_stats(IN rpc_binding_handle_t binding_handle, OUT
rpc_stats_vector_p_t *statistics, OUT unsigned32 *status);|
这个例程获取前面提到的统计数据。使用管理入口点向量查询协议服务，获取系统上支持的每个协议的统计数据。
管理名称服务的元素
这包括管理名称服务条目、组和概要。
rpc_ns_mgmt_entry_create对名称服务数据库有适当访问权的应用程序可以创建名称并存储在数据库中。其他应用程序可以根据需要访问这个名称。
原型和 I/O 参数如下：
void rpc_ns_mgmt_entry_create(IN unsigned32  entry_name_syntax, IN 
unsigned_char_p_t entry_name,OUT unsigned32  *status);|
首先，必须从给定的条目名称获取名称服务条目表示的引用。这个表示是一个结构，包含名称和名称长度字段。通过展开给定的条目名称并获取完整的名称和长度，获得引用。这需要解析条目名称并把它转换为兼容的格式。使用这个引用在名称服务数据库中创建条目。
rpc_ns_mgmt_entry_create这个例程在名称服务数据库中创建服务器条目。服务器条目包含接口标识符、绑定信息和可选的对象 UUID。接口标识符包含接口 UUID 和它的版本号。客户机使用这个标识符查找合适的接口。
原型和 I/O 参数如下：
void rpc_ns_mgmt_entry_create(IN unsigned32 entry_name_syntax, IN 
unsigned_char_p_t entry_name, OUT unsigned32 *status);|
entry_name_syntax 是一个整数值，它指定条目名称的语法。如果传递 NULL，就使用默认的条目语法。这个例程解析条目使用的语法，然后把传递的条目转换为名称服务表示。最后，在名称服务数据库中创建相关的条目，例程返回。
rpc_ns_group_mbr_addRPC 运行时为元素、组、概要和塔提供查询例程，这通过查询或访问名称服务数据库来完成。
使用这个例程在组条目中添加成员。组条目是一种名称服务条目，其中包含提供相同接口的服务器条目（成员）。客户机搜索组中提供相同接口的所有成员，寻找合适的服务器。
原型和 I/O 参数如下：
oid rpc_ns_group_mbr_add(IN unsigned32 group_name_syntax, IN 
unsigned_char_p_t	group_name, IN unsigned32 member_name_syntax,
IN unsigned_char_p_t	member_name,OUT	unsigned32 *status);|
这个例程的输入参数包括 group_name_syntax 和 member_name_syntax，它们是指定组和成员语法的整数值。如果传递的值是 NULL，就使用默认语法。group_name 和 member_name 分别是采用人可读的格式的组名和成员名。
这个例程首先把组名语法转换为内部形式，然后把组名转换为名称服务表示。同样，把成员名语法转换为内部形式，把成员名转换为名称服务表示。它在名称服务数据库中搜索这个组名，如果没有找到，就创建组条目，然后在刚创建的组中添加成员条目。如果找到了组条目，这个例程就只需在指定的组中添加成员条目。
rpc_ns_group_mbr_inq_begin这个例程开始一个查询上下文，用于向调用者返回组成员。查询上下文只是一个结构的指针，在内部维护这个结构，用于查询组成员、概要、塔或 uuid 成员。
原型和 I/O 参数如下：
void rpc_ns_group_mbr_inq_begin(IN unsigned32 group_name_syntax, 
IN unsigned_char_p_t group_name, IN unsigned32 member_name_syntax,
OUT rpc_ns_handle_t *inquiry_context, OUT unsigned32 *status);|
这个例程首先把组名语法转换为内部形式，把人可读的组名转换为名称服务表示。接下来，把成员名语法转换为内部形式。最后，为组成员创建查询上下文，设置查询上下文结构中的成员名语法字段，把创建的查询上下文结构的指针返回给调用者。然后，可以把查询上下文指针/句柄传递给 rpc_ns_group_mbr_inq_next 例程，这个例程返回组中的成员。
rpc_ns_group_mbr_inq_next在调用 rpc_ns_group_mbr_inq_begin 之后，使用这个例程获取特定组的成员。
原型和 I/O 参数如下：
void rpc_ns_group_mbr_inq_next(IN rpc_ns_handle_t inquiry_context,
OUT unsigned_char_p_t	*member_name, OUT unsigned32 *status);|
这个例程以 rpc_ns_group_mbr_inq_begin 返回的查询上下文句柄作为输入。它首先检查传递的查询上下文是否非 NULL，以及是否是组成员的上下文。如果传递的查询上下文是 NULL，或者不是用于组成员查询的上下文，它返回 rpc_s_invalid_ns_handle。否则，读取组的成员，把成员名由名称服务格式转换为人可读的格式并返回给调用者。
rpc_ns_group_mbr_inq_done通过调用这个例程，结束以前调用 rpc_ns_group_mbr_inq_begin 所设置的查询上下文。
原型和 I/O 参数如下：
void rpc_ns_group_mbr_inq_done (IN, OUT rpc_ns_handle_t *inquiry_context,
OUT unsigned32 *status);|
这个例程首先检查传递的查询上下文是否非 NULL，以及是否是用于组成员查询的上下文。如果是 NULL 或者不是用于组成员查询的上下文，它返回 rpc_s_invalid_ns_handle。否则，它释放在 rpc_ns_group_mbr_inq_begin 中分配给查询上下文结构的内存。


