# ZeroC Ice Ice Registry实现负载均衡 - 建建的博客 - CSDN博客
2017年05月02日 17:47:52[纪建](https://me.csdn.net/u013898698)阅读数：625
个人分类：[ICE](https://blog.csdn.net/u013898698/article/category/6896358)

# **Registry介绍**
        对于多个IceBox集群该怎么负载均衡？以服务注册表Registry为依托的Service Locator组件，以及依赖其而诞生的强大的分分布式框架-IceGrid。
        IceBox没有地方存储的Endpoint信息。为了解决这个问题，诞生了服务注册表Registry组件，它是一个以二进制文件形式储存运行期Ice服务注册信息的独立进程，支持主从同步，从节点可以分担查询请求，类似MySQL读写分离的功能，并防止单点障。同时依托Registry的功能，Zero
 C设计实现了Service Locator服务组件，它是一个标准的Ice Objcet服务对象，我们可以在Ice程序中调用这个服务，从而解决服务地址的查询问题。另外，Service Locator服务组件与Ice客户端Runtime框架相互结合，实现了自动化的透明的服务调用负载均衡功能。
    原理图：
![](https://img-blog.csdn.net/20161001090249164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# Registry使用
在项目中新建Registry配置文件：registry.cfg
|123456789101112|`#registery config for icegrid``#客户端通过Endpoint 访问注册表``IceGrid.Registry.Client.Endpoints=tcp -p 4061``IceGrid.Registry.Server.Endpoints=tcp``IceGrid.Registry.Internal.Endpoints=tcp``IceGrid.Registry.AdminPermissionsVerifier=IceGrid``/NullPermissionsVerifier``#registry 持久化文件存放目录 需要手工创建目录``IceGrid.Registry.Data=.``/registry``#是否允许动态注册  生产关闭``IceGrid.Registry.DynamicRegistration=1``#Registry进程不依赖其他进程，相反，每个Node上的服务IceGrid进程及客户端都需要和Registry通讯，在配置文件中需要定义Registry的Service Locator访问地址``Ice.Default.Locator=IceGrid``/Locator``:tcp -h registryhost -p 4061`|
    在项目根目录下创建目录： registry
    启动：启动IceGrid的Registry进程，使用Ice安装后提供的命令icegridregistry，在bin目录下
        icegridregistry --Ice.Config=src/registry.cfg
    修改IceBox配置文件，新增：
|1234|`#整合Ice Registry配置``Ice.Default.Locator=IceGrid``/Locator``:tcp -h localhost -p 4061``MyService.AdapterId=MyServiceAdapter``SMSService.AdapterId=SMSServiceAdapter`|
    客户端代码做响应修改， 
|123456|`String[] initParams = ``new``String[]{``"--Ice.Default.Locator=IceGrid/Locator:tcp -h localhost -p 4061"``};``// 初始化通信容器``ic = Ice.Util.initialize(initParams);``// 传入远程服务单元的名称、网络协议、IP及端口，构造一个Proxy对象``// Ice.ObjectPrx base = ic.stringToProxy("MyService:default -p 10001");``Ice.ObjectPrx base = ic.stringToProxy(``"MyService@MyServiceAdapter"``);`|
    分别启动registry和iceBox，即可。
参考
        《ZeroC Ice 权威指南》3.3
资料
        源码：链接：http://pan.baidu.com/s/1bnZrhWb 密码：velf
