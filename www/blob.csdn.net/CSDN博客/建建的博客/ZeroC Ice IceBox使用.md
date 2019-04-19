# ZeroC Ice IceBox使用 - 建建的博客 - CSDN博客
2017年05月02日 17:47:21[纪建](https://me.csdn.net/u013898698)阅读数：619
# IceBox介绍
        IceBox就像一个Tomcat，我们只要写N个Ice服务代码，用一个装配文件定义需要加载的服务列表、服务器的启动参数、启动次序等必要信息，然后启动IceBox，我们的应用系统就能够正常运行了。IceBox采用的是UNIX上通用的方式-属性文件的方式，其可能的原因的可以省去复杂的XML解析和相关的lib库，并能快速加载和启动程序。
# IceBox使用
        要将一个Ice服务纳入到IceBox中，我们需要引入IceBox.jar这个库，另外只需要让这个服务实现类继承IceBox定义的Service接口即可。
|1234567891011121314151617181920212223242526|`public``class``MyServiceImpl ``extends``_MyServiceDisp ``implements``Service {``    ``private``static``final``long``serialVersionUID = 7114601588161119171L;``    ``private``Ice.ObjectAdapter _adapter;``    ``@Override``    ``public``String hello(Current __current) {``        ``return``"Hello Jerome"``;``    ``}``    ``@Override``    ``public``void``start(String name, Communicator communicator, String[] args) {``        ``_adapter = communicator.createObjectAdapter(name);``        ``_adapter.add(``this``, communicator.stringToIdentity(name)); ``// 创建servant,``        ``_adapter.activate();``        ``System.out.println(``"log:"``+ name + ``"is started!"``);``    ``}``    ``@Override``    ``public``void``stop() {``        ``System.out.println(``"log:"``+ ``this``._adapter.getName() + ``"is stoped!"``);``        ``// 销毁adapter``        ``_adapter.destroy();``    ``}``    ``}`|
在src下新建IceBox配置文件config.properties
|12345678910111213141516171819202122232425262728293031323334353637383940414243|`#service properties``IceBox.InstanceName=MyAppIceBox 1``IceBox.InheritProperties=1``#所有服务初始化完成之后打印 xxx ready``IceBox.PrintServicesReady=MyAppIceBox 1``#IceBox.serviceManager.Endpoints=tcp -p 9999 -h localhost``#performance properties``IceBox.ThreadPool.Server.Size=4``IceBox.ThreadPool.Server.SizeMax=100``IceBox.ThreadPool.Server.SizeWarn=40``IceBox.ThreadPool.Client.Size=4``IceBox.ThreadPool.Client.SizeMax=100``IceBox.ThreadPool.Client.SizeWarn=40``#for system stronger``Ice.ACM.Client=300``Ice.ACM.Server=300``#log and trace``#Ice.LogFile=iceserver.log``Ice.PrintStackTraces=1``Ice.Trace.Retry=2``Ice.Trace.Network=2``Ice.Trace.ThreadPool=1``Ice.Warn.Connections=1``Ice.Warn.Dispatch=1``Ice.Warn.Endpoints=1``#service define begin``#IceBox.Service.name=entry_point [--key=value] [args]``#name定义service的名字，作为start方法的name的参数，必须是唯一的``#entry_point是上面MyServie的完整类名``#[--key=value]：被作为property属性,用于构建该服务的communicator,``#用来更加精确的控制每个Ice服务的性能调优,这里也可以使用--Ice.Config=xxx.cfg的方式从具体的配置文件中加载参数。``#另外，也可以用IceBox.InheriProperties=1的属性让所以Ice服务实例都使用IceBox的配置属性``#[args]作为传入start方法的参数，作为服务启动初始化参数``IceBox.Service.MyService=service.MyServiceImpl prop1=1 prop2=2 prop3=3``#IceBox.Service.SMSService=com.SMSServiceI2``MyService.Endpoints=tcp -p 10001 -h localhost``#OnlineBook.Endpoints=tcp -p 10000 -h localhost``#service end``#service load order``#配置多个服务的先后顺序``IceBox.LoadOrder=MyService``IceBox.UseSharedCommunicator.MyService=1``IceBox.UseSharedCommunicator.SMSService=1`|
启动：在Eclipse右击项目Run As→Run Configuration，选择Project和Main class，和配置Arguments：--Ice.Config=config.properties
![](https://img-blog.csdn.net/20160316193527031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316193543984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考
《ZeroC Ice 权威指南》3.2
资料
        参考：D:\ZeroC\Ice-3.5.1-demos\demoj\IceBox
        源码：链接：[http://pan.baidu.com/s/1btCaIi](http://pan.baidu.com/s/1btCaIi) 密码：edv0
参考
《ZeroC Ice 权威指南》3.2
资料
        参考：D:\ZeroC\Ice-3.5.1-demos\demoj\IceBox
        源码：链接：[http://pan.baidu.com/s/1btCaIi](http://pan.baidu.com/s/1btCaIi) 密码：edv0

