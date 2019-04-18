# 关于Unity的网络框架 - DesignYourDream - 博客园
# [关于Unity的网络框架](https://www.cnblogs.com/designyourdream/p/4374029.html)
　　注：Unity 5.1里引入了新的网络框架，未来目标应该是WOW那样的，现在还只是个P2P的架子。
　　网络的框架，无非是如何管理网络数据的收发，通信双方如何约定协议。之前做的框架与GameObject无关，网络是看不到GameObject这一层的，看到的是事务，比如例如进入场景等。在Unity里，GameObject自己可以有网络数据的处理能力，网络行为基于GameObject搭建。最大的不同是客户端服务器运行的是同一套机制，服务器上有客户端有的所有东西。uLink相比Unity自己那套东西来说，实现了更多的功能。这里打算对这类的网络进行下分析。
　　Unity规定，网络行为只能有RPC和状态同步两种机制，非此即彼。Unity的networkview组件实现了这种机制，从Networkview的编辑界面上看，会有疑惑，因为看起来networkview监听的是一个component，那是不是networkview监听的是脚本里的RPC和状态同步数据呢，答案是非也。If you are not directly sending data, just using RPC calls, then you can turn off synchronization (no data directly sent) and nothing needs to be set as the Observed property.这句话的意思就是说，networkview里设置observed component，只是为状态同步的。那么RPC机制是如何做的？RPC calls just need a single network view present so you don’t need to add a view specifically for RPC if a view is already present.意思就是说RPC调用不需要专门给gameobject加一个netview，只要有一个就行了，也许是某个用来做状态同步的Netview。
If you want to use Remote Procedure Calls in your script all you need is a NetworkView component present in the same GameObject the script is attached to. The NetworkView can be used for something else, or in case it’s only used for sending RPCs it can have no script observed and state synchronization turned off.
由于Unity是C/S同构，同一个GameObject存在于C和S上，C可以直接调用
```
workView.RPC ("PlayerFire", RPCMode.All);
```
同时，必须要提供一个带[RPC]表示的函数
```
[RPC]
    void PlayerFire () {
    }
```
这样，收到RPC调用的端，被告知说有一个GameObject上id为xxx的netview，其component里有一个名称是PlayerFire的RPC，你去找到它，并执行。
对于viewID,Unity这种可以通过控制实例化GameObject，来确保gameobject在网络上的同时存在，从而可以确保同一个gameobject，他的viewID相同。uLink里的实现是The viewID is used internally in uLink to identify the sender and target game object.说明viewID和gameobject是一一对应的。关于ID的分配方式一部分静态，双方约定；一部分是动态，由服务器分配。uLink是这么解释手动分配ID，即静态的ID：The other option is to assign manual viewIDs to game objects in the scene.Make sure you assign the same viewID to the game object in the client scene and the corresponding game object in the server scene. This is commonly used for static objects like chat windows, doors, elevators and trigger zones.
关于RPC到底是监听一个脚本还是一个GameObject？uLink里有这么一句话：The RPC calling code will use the attached `uLinkNetworkView` component to invoke all `PrintText()` functions in any scripts attached to this same `GameObject` on all other clients (and the server) if this code is executed on one of the clients.所以至少uLink是会扫描整个GameObject的。
uLink里不推荐在一个gameobject上使用多个netview，目的有两个，一是为了降低出bug的风险，另外是简化组件结构，便于维护。要监听多个component，它提供了一个observerList。
uLink里怎么做状态同步呢？其增加了一个observerList的组件，可以往里面放置一组component，然后会为每个component生成一个处理器。当收到一个Netview id的同步请求后，ulink找到这个netview id，调用对应的netview的observerList里的托管函数，把bitstream传进去，注意这个bitstream，他是顺序放置数据的，例如有三条数据，那么这个流里面是顺序摆放，然后逐个解析。
　　为了搞明白一些细节，这里分析两个demo，一个是LerpzAuthServer，另一个是snowbox。
　　LerpzAuthServer这个例子是为了演示权威服务器，实现了角色之间的同步，聊天。此工程下有两个场景，可以理解为导出两个不同的exe程序。一个玩家登录后，会存在三个概念，ower,proxy,creator，分别代表自己，自己在其他玩家机器上的镜像，自己在服务器上的镜像。他们的区别在于，通过对应脚本的加减实现了不同的功能组合，ower支持操作控制，可以播放动画；creator不支持操作控制，也不播放动画；proxy是支持播放动画，但不支持操作控制。共同点在于，都有networkview脚本，
从场景结构上看，客户端与服务器的区别是两个GameObject节点，SpawnPoint和StartServer，前一个节点上挂了AuthSpawnPrefab脚本，此脚本重写了uLink_OnPlayerConnected方法，当一个玩家连接成功后，执行这个方法，这里执行的就是实例化玩家。这个过程在权威服务器上只能由服务器来做。StartServer节点上挂载了一个uLinkSimpleServer脚本，启动时会执行uLink.Network.InitializeServer方法初始化服务器，配置的参数是监听的端口号和最大的连接数。在socket初始化结束后，将调用uLink_OnServerInitialized方法。玩家断开连接后，将执行uLink_OnPlayerDisconnected方法，这里将执行对断开连接玩家的清理工作
 uLink.Network.DestroyPlayerObjects(player);
			uLink.Network.RemoveRPCs(player);
关于状态同步，即角色位置的同步，有两种情况，一种是玩家自身和服务器的位置同步，另一种是和玩家镜像的同步。
实例化出来的镜像ower,proxy,creator都会有一个脚本uLinkSmoothCharacter，其中调用了InvokeRepeating方法实现定期调用SendToServer这个方法，此方法将调用move的RPC方法，服务器执行这个move方法实现了服务器上角色位置的更新（权威服务器），实现了自己和服务器的状态同步。
 该脚本里面重写了uLink_OnSerializeNetworkView方法实现了状态同步，主要同步了位置，速度，朝向三个变量。服务器将定时执行这个方法发送移动了的玩家位置等信息，代理对象收到这个信息后，移动代理的位置，从而实现了对所有代理对象（proxy）的资料更新。
　　snowbox工程，实现有客户端预测，插值，权威服务器，大厅服务器，玩家切换服务器等。
　　在角色的位移同步上，ower,proxy,creator上都挂了一个uLinkStrictPlatformer脚本，为了实现平滑的移动，此脚本里会对镜像的行为做修正和模拟，镜像会存储最近一组的玩家的位置，朝向等信息，通过这组信息进行插值计算当前镜像的位置。如果长时间没受到数据更新，镜像会根据最近一次镜像的状态数据模拟镜像的下一个位置和朝向。
　　服务器将模拟所有的物理行为包括重力，并检查角色是否有作弊行为。角色移动时将发送一个rpc，如果检测到玩家作弊，服务器会给客户端一个修正命令。如果玩家行为正常，就告知客户端，玩家行为正常。服务器收到角色位置，速度，旋转值，计算出一个位移值，并把玩家移动的位置存储起来，在lateupdate里，对玩家实际的目标点尝试进行修正。
　　关于仍雪球。当owner有鼠标点击行为后，将调用服务器的throw 这个RPC方法，告知服务器雪球的方向，服务器收到这个行为后，将实例化雪球并模拟仍雪球的行为，并广播给其他玩家。关于雪球实体，玩家触发雪球后，前后端都会实例化雪球，都挂载有Snowball脚本，利用胶囊体碰撞实现被击效果，区别是客户端（owner和Proxy）会挂一个TimedTrailRenderer这样的渲染相关的脚本来实现雪球拖尾的效果。这个意思就是，服务器上的gameobject可以不加载哪些不需要进行判定的行为。
　　关于切换服务器（非切场景）。支持creator切换到另外一个进程，由另外一个进程的服务器管理。uLink的P2P更多的是在服务器端的架构，不需要服务器端再维护一个中央服务器来协调各个服务器之间的资源（C/S模型）。uLink文档上说更多的是适用于服务器端的局域网直连。没看到说客户端在不同局域网之间的连接，所以默认它不具备打洞的功能。
　　建立P2P，给serverA 的一个gameobject上挂载一个uLinkNetworkP2P脚本，表示此server可以接受别的server的连接，给serverB上的一个GameObject上挂载一个uLinkNetworkP2PConnector脚本，同时将自动加上uLinkNetworkP2P脚本，他的逻辑是说，我要连接别人，那我也一定要能接受别人连我。服务器上执行的creator，在场景之间的切换，只需要调用一个HandoverPlayerObjects方法就可以。
 　 uLink里有个uLobby的概念，游戏大厅，从功能上看处理用户的连接，然后找一台合适的服务器把玩家扔进去，支持P2P方式的连接，即玩家也可以开房间，让其他玩家加入，这样看起来暗黑3里也有这样的，比如开一个公共游戏，然后邀请别人加入进来，从一定意义上看就是网关。uLobby是运行在一个单独的U3D工程下，可以让其他的client连接。
 　 uLink里的uZone，如下图：
![](https://images0.cnblogs.com/blog2015/698097/201506/022002118361195.png)
这张图里有几个关键的概念
uZone Master是uZone network系统里的核心，当node连接到uZone network上之后，master将负责跟踪这些node信息，master也会管理uZone Client的连接。
上图里的node是个应用程序，保存node到master的session， 负责管理Instance，所谓Instance都可以认为是进程。node和node之间互相独立，node同时只能连接一个Master。一般情况，每台机器上只会有一个Node，一个uZone里会有多个Node。这里的Client很不一样，只有使用了uZone client api的才能称之为uZone client，这种client一般都是和uLobby合作的某种模式的服务器，当然也有别的，像负载均衡，分布式计算，分布式测试或者监控。
client请求master启动一个功能，master将会通知相应的node启动一个instance，也就是process（进程）。正常情况下一个instance就是一个进程。

