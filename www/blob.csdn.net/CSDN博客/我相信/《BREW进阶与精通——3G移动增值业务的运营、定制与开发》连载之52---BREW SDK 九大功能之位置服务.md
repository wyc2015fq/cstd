# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之52---BREW SDK 九大功能之位置服务 - 我相信...... - CSDN博客





2010年04月07日 09:07:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2477标签：[brew																[手机																[服务器																[网络																[优化																[network](https://so.csdn.net/so/search/s.do?q=network&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)





LBS（基于位置的服务）是相对复杂的服务，但BREW实现起来却很简单，只提供了一个IPosDet接口就能够取得与位置相关的多种信息。PosDet对LBS的支持主要是由手机中的GPSone模块提供的。手机中的GPSone软件模块结构如图11-9所示。



**图****11-9****：****手机中的GPSone****软件模块结构**

手机定位的模式有3种：基于手机的定位模式（MS-Based），手机辅助的定位模式（MS-Assisted）和独立手机的定位模式（standalone），其间的区别如表11-2所示。
|**MS-Assisted**|**MS-Based**|**Standalone**|
|----|----|----|
|每次定位都要发送辅助数据到手机上|只有需要的时候，才会周期性地发送辅助数据到手机上|根本不需要辅助数据|
|手机接收GPS的区域信息,然后发送到网络服务器（PDE），由服务器完成最终的位置计算|手机既能接收GPS的区域信息，也能够完成位置的计算|完全由手机完成所有操作不需要网络服务器（PDE）的配合|
|–高度的GPS敏感度–支持多种定位方法例如AFLT，因而损益最大–每一次定位都需要建立数据呼叫连接|–只使用GPS卫星定位–跟踪和导航时连续定位的反应时间短–只需要很少量的数据呼叫连接|–增加了定位时间和耗电量–减小了损益和精度–不需要建立数据呼叫连接|
|在网络上完成定位|定位基本上在手机上完成|手机完成定位|
|–适用于定位频率较低的应用–速度较慢–满足多层次的定位需求|–适用于高频率定位的应用，速度快–满足反复导航和快速跟踪的定位需求|最适合于手机不在运营商提供的无线网络服务区内|

**表****11-2****：**** 3****种定位模式的比较**

BREW 从1.X就开始提供了对定位服务的接口，随着版本的升级，支持的手机定位功能也越来越丰富，BREW对定位服务提供的功能如表11-3所示。
|BREW 版本|GPSone 模式|说明|
|----|----|----|
|BREW 1.X|Mobile-Assisted|支持有限的手机辅助GPSone定位数据（主要是经纬度）|
|BREW 2.0.X|Mobile-Assisted|支持全部的手机辅助定位功能，基于GPSone的定位数据也丰富了很多，除了经纬度，还有海拔，水平速率，垂直速率，不确定值等等，增加的操作模式有single shot，Track_Local，track_network，DLOAD_First|
|BREW 2.1.0BREW 2.1.1|Mobile-Assisted|增加了对方向性的支持（手机集成了罗盘的功能）和查询手机上支持的操作模式|
|BREW 2.1.2|Mobile-AssistedMobile-Based|开始支持MS-Based定位模式和智能操作模式，允许应用在使用GPSone时能够优化速度或者精度|
|BREW 3.0|Mobile-AssistedMobile-Based|增加了新的智能操作模式能够优化手机与定位服务器之间的数据交互，引入了新的操作模式Track_Optimal和新的优化方法Optimal_Payload, DLOAD_First增强了定位的频率|
|BREW 3.1.2|Mobile-AssistedMobile-BasedStandalone|增加了几个新的数据元素使定位信息更加具体化，同时开始支持standalone定位模式|
|BREW 3.1.4|Mobile-AssistedMobile-BasedStandalone|增加了读取E911所需的只读定位标识（通过IShell_GetDeviceInfoEx获得）|
|BREW 3.1.5|Mobile-AssistedMobile-BasedStandalone|更新了相关文档和使用用例|

**表****11-3****：****BREW****对定位服务提供的功能**

针对不同的应用目的，在定位过程中的操作方法也是不同的，这样可以提高定位模式的使用效率。下面逐一给予说明，见表11-4。
|**操作模式的值**|**说明**|
|----|----|
|One_Shot|执行一次定位是默认操作方式，“Optim” 参数来确定使用 MS-Assisted 还是MS-Based|
|Track_Network|执行一系列的MS-Assisted 定位|
|Track_Local|执行一系列的MS-Based 定位|
|Track_Optimal|gpsOne引擎选择MS-Based 或者 MS-Assisted依赖于优化的准则，可以使用户不必关心所有的定位数据细节|
|Track_Standalone|执行一系列stand-alone 定位|
|DLoad_First|只请求辅助信息而不发生定位操作，在用户不在网络服务区时或者在下载辅助数据来准备MS-Based 操作时非常有用|

**表****11-4****：****定位过程中的不同操作方法**

以中国联通运营基于BREW的LBS业务作为示例，定位工作由四部分组成: 移动终端( 手机) 、GPS 卫星、基战、业务服务器, 地图服务器。其中基于MS-Assisted的定位操作模式工作流程如图11-10。



**图****11-10****：****基于MS-Assistedd****的定位操作模式工作流程**

对于基于MS-Assisted的定位操作模式工作流程，这里给予简要的描述：(1)Gpsone 手机用户向所属的基战发出“自我定位”请求.(2)手机获得当前位置的经纬度.(3)手机用户向业务服务器发送经纬度.(4)业务服务器根据用户提交的经纬度向地图服务器发出位置请求.(5)地图服务器根据经纬度算出用户当前位置地图、将结果返回给业务服务器.(6)业务服务器将结果发送给定位用户.](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




