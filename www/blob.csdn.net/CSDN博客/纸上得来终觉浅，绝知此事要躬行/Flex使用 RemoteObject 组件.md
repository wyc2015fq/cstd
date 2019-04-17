# Flex使用 RemoteObject 组件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年05月29日 16:15:50[boonya](https://me.csdn.net/boonya)阅读数：1891标签：[flex																[ejb																[javabeans																[access																[actionscript](https://so.csdn.net/so/search/s.do?q=actionscript&t=blog)
个人分类：[Flex/Flash](https://blog.csdn.net/boonya/article/category/1768619)





# [示例 RemoteObject 应用程序](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fde)




- 
[在
 ActionScript 中调用 RemoteObject 组件](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fdb)

- 
[访问源路径中的
 Java 对象](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fda)

- 
[访问
 JNDI 中的 EJB 和其它对象](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fd9)

- 
[保留的方法名称](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fd8)

- 
[ActionScript
 和 Java 之间的序列化](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fd7)
- 
[使用自定义序列化](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html#WS2db454920e96a9e51e63e3d11c0bf66651-7fd2)









可以使用 Flex RemoteObject 组件来针对 ColdFusion 组件或 Java 类调用方法。


还可以将包含 PHP 和 .NET 对象的 RemoteObject 组件与第三方软件（如开放源代码项目 AMFPHP 和 SabreAMF 以及 Midnight Coders WebORB）结合使用。有关详细信息，请访问以下网站：
- 

Zend Framework [http://framework.zend.com/](http://framework.zend.com/)

- 

AMFPHP [http://amfphp.sourceforge.net/](http://amfphp.sourceforge.net/)

- 

SabreAMF [http://www.osflash.org/sabreamf](http://www.osflash.org/sabreamf)

- 

Midnight Coders WebORB [http://www.themidnightcoders.com/](http://www.themidnightcoders.com/)



RemoteObject 组件使用 AMF 协议发送和接收数据，而 WebService 和 HTTPService 组件使用 HTTP 协议。AMF 显著快于 HTTP，但服务器端编码和配置通常更复杂。


与 HTTPService 和 WebService 组件一样，您可以使用 RemoteObject 组件在应用程序中显示数据库查询结果。也可以使用该组件在数据库中插入、更新和删除数据。在将查询结果返回到应用程序之后，可以将其显示在一个或多个用户界面控件中。


有关 RemoteObject 组件的 API 参考信息，请参阅 [mx.rpc.remoting.mxml.RemoteObject](http://help.adobe.com/zh_CN/AS3LCR/Flex_4.0/mx/rpc/remoting/mxml/RemoteObject.html)。



## 示例 RemoteObject 应用程序



#### MXML 代码


以下示例中的应用程序使用 RemoteObject 组件调用 ColdFusion 组件。ColdFusion 组件查询一个名为 users 的 MySQL 数据库表。它将查询结果返回到应用程序，在该应用程序中，查询结果将绑定到 DataGrid 控件的 dataProvider 属性并显示在 DataGrid 控件中。应用程序还将新用户的用户名和电子邮件地址发送到
 ColdFusion 组件，ColdFusion 组件会在用户数据库表中执行插入操作。
<?xml version="1.0" encoding="utf-8"?> 
<s:Application xmlns:fx="http://ns.adobe.com/mxml/2009" 
        xmlns:s="library://ns.adobe.com/flex/spark" 
        xmlns:mx="library://ns.adobe.com/flex/mx" minWidth="955" minHeight="600"> 
    <fx:Declarations> 
        <mx:RemoteObject 
            id="userRequest" 
            destination="ColdFusion" 
            source="flexapp.returnusers"> 
             
            <mx:method name="returnRecords" result="returnHandler(event)" 
                       fault="mx.controls.Alert.show(event.fault.faultString)"/> 
            <mx:method name="insertRecord" result="insertHandler()" 
                       fault="mx.controls.Alert.show(event.fault.faultString)"/> 
        </mx:RemoteObject> 
    </fx:Declarations> 
     
    <fx:Script> 
        <![CDATA[ 
            import mx.rpc.events.ResultEvent; 
             
            private function returnHandler(e:ResultEvent):void  
            { 
                dgUserRequest.dataProvider = e.result; 
            } 
            private function insertHandler():void  
            { 
                userRequest.returnRecords(); 
            } 
            private function clickHandler():void  
            { 
                userRequest.insertRecord(username.text, emailaddress.text); 
            }  
        ]]> 
    </fx:Script> 
     
    <mx:Form x="22" y="10" width="300"> 
        <mx:FormItem> 
            <s:Label text="Username" />     
            <s:TextInput id="username"/> 
        </mx:FormItem> 
        <mx:FormItem> 
            <s:Label text="Email Address" />     
            <s:TextInput id="emailaddress"/> 
        </mx:FormItem> 
        <s:Button label="Submit" click="clickHandler()"/> 
    </mx:Form> 
 
    <mx:DataGrid id="dgUserRequest" x="22" y="200"> 
        <mx:columns> 
            <mx:DataGridColumn headerText="User ID" dataField="userid"/> 
            <mx:DataGridColumn headerText="User Name" dataField="username"/> 
        </mx:columns> 
    </mx:DataGrid> 
</s:Application>

在此应用程序中，RemoteObject 组件的 destination 属性设置为 Coldfusion，source 属性设置为
 ColdFusion 组件的完全限定名称。


与之相反，在使用 LiveCycle Data Services ES 或 BlazeDS 时，需要在配置文件（默认情况下为 remoting-config.xml 文件）内为远程服务目标的 source 属性指定完全限定的类名，并在 RemoteObject 组件的destination 属性中指定目标的名称。目标类还必须具有不带任何参数的构造函数。在使用
 ColdFusion 时，您可以选择按这种方式配置目标，而不是使用 RemoteObject 组件的 source 属性。



#### ColdFusion 组件


应用程序调用下面的 ColdFusion 组件。此 ColdFusion 代码执行 SQL 数据库插入和查询，并将查询结果返回到应用程序。ColdFusion 页使用 cfquery 标签在数据库中插入数据并查询数据库，使用 cfreturn 标签将查询结果的格式设置为
 ColdFusion 查询对象。
<cfcomponent name="returnusers">  
    <cffunction name="returnRecords" access="remote" returnType="query"> 
 
        <cfquery name="alluserinfo" datasource="flexcf"> 
                SELECT userid, username, emailaddress FROM users 
        </cfquery> 
        <cfreturn alluserinfo> 
    </cffunction> 
    <cffunction name="insertRecord" access="remote" returnType="void"> 
 
        <cfargument name="username" required="true" type="string"> 
        <cfargument name="emailaddress" required="true" type="string"> 
        <cfquery name="addempinfo" datasource="flexcf"> 
            INSERT INTO users (username, emailaddress) VALUES ( 
            <cfqueryparam value="#arguments.username#" cfsqltype="CF_SQL_VARCHAR" maxlength="255">, 
            <cfqueryparam value="#arguments.emailaddress#" cfsqltype="CF_SQL_VARCHAR" maxlength="255"> ) 
        </cfquery>     
        <cfreturn> 
    </cffunction>     
</cfcomponent>




## 在 ActionScript 中调用 RemoteObject 组件



在下面的 ActionScript 示例中，调用 useRemoteObject() 方法即可以声明服务、设置目标、设置 result 和 fault 事件侦听器并调用服务的 getList() 方法。
<?xml version="1.0"?> 
<!-- fds\rpc\ROInAS.mxml --> 
<s:Application xmlns:fx="http://ns.adobe.com/mxml/2009" 
        xmlns:s="library://ns.adobe.com/flex/spark" 
        xmlns:mx="library://ns.adobe.com/flex/mx" minWidth="955" minHeight="600"> 
    <fx:Script> 
        <![CDATA[ 
            import mx.controls.Alert; 
            import mx.rpc.remoting.RemoteObject; 
            import mx.rpc.events.ResultEvent; 
            import mx.rpc.events.FaultEvent; 
 
            [Bindable] 
            public var empList:Object;          
            public var employeeRO:RemoteObject; 
 
            public function useRemoteObject(intArg:int, strArg:String):void { 
                employeeRO = new RemoteObject(); 
                employeeRO.destination = "SalaryManager"; 
                employeeRO.getList.addEventListener("result", getListResultHandler); 
                employeeRO.addEventListener("fault", faultHandler); 
                employeeRO.getList(deptComboBox.selectedItem.data); 
            } 
 
            public function getListResultHandler(event:ResultEvent):void { 
                 // Do something 
                empList=event.result; 
            } 
 
            public function faultHandler (event:FaultEvent):void { 
             // Deal with event.fault.faultString, etc. 
                Alert.show(event.fault.faultString, 'Error'); 
            } 
        ]]> 
    </fx:Script> 
    <s:ComboBox id="deptComboBox"/> 
</mx:Application>



## 访问源路径中的 Java 对象



使用 RemoteObject 组件可以访问位于 LiveCycle Data Services ES、BlazeDS 或 ColdFusion Web 应用程序的源路径中的无状态和有状态的 Java 对象。可以将独立类文件放在 Web 应用程序的 WEB‑INF/classes 目录中，以便将它们添加到源路径中。可以将 Java Archive (JAR) 文件中所包含的类放在 Web 应用程序的 WEB‑INF/lib 目录中，以便将它们添加到源路径中。在 LiveCycle Data Services
 ES、BlazeDS 或 ColdFusion 的 services-config.xml 文件，或者通过引用而包括的文件（如 remoting-config.xml 文件）中，在远程服务目标的 source 属性中指定完全限定的类名。该类还必须具有不带任何参数的构造函数。对于 ColdFusion，可以选择将 RemoteObject
 组件的 destination 属性设置为 Coldfusion，将 source 属性设置为
 ColdFusion 组件或 Java 类的完全限定名称。


在配置远程服务目标以访问无状态的对象（请求作用域）时，Flex 会为每个方法调用创建一个新对象，而不是在同一个对象上调用多个方法。您可以将对象的作用域设置为请求作用域（默认值）、应用程序作用域或会话作用域。应用程序作用域中的对象可供包含该对象的 Web 应用程序使用。会话作用域中的对象可用于整个客户端会话。


在配置远程对象目标以访问有状态的对象时，Flex 会在服务器上创建一次该对象，并在方法调用之间保持该对象的状态。如果将对象存储在应用程序作用域或会话作用域中会导致内存问题，则应该使用请求作用域。




## 访问 JNDI 中的 EJB 和其它对象



可以访问 Java 命名和目录接口 (JNDI) 中存储的 Enterprise JavaBeans (EJB) 和其它对象，方法是对目标调用方法，该方法是在 JNDI 中查找对象并调用其方法的服务 facade 类。


可以使用无状态对象或有状态对象来调用 Enterprise JavaBeans 和其它使用 JNDI 的对象的方法。对于 EJB，可以调用服务 facade 类，该类返回 JNDI 中的 EJB 对象并针对该 EJB 调用某个方法。


在 Java 类中，可以使用标准的 Java 编码模式，在该模式下，可以创建初始上下文并执行 JNDI 查找。对于 EJB，也可以使用标准编码模式，在该模式下，您所创建的类中可以包含调用 EJB 主对象的 create() 方法和所得到 EJB 的业务方法。


下面的示例在 facade 类定义中使用一个名为 getHelloData() 的方法：
<mx:RemoteObject id="Hello" destination="roDest"> 
    <mx:method name="getHelloData"/> 
</mx:RemoteObject>

在 Java 端，getHelloData() 方法可以封装针对 EJB 调用业务方法所必需的全部内容。以下示例中的 Java 方法会执行如下操作：
- 

新建一个用来调用 EJB 的初始上下文

- 

执行一个用来获取 EJB 主对象的 JNDI 查找

- 

调用 EJB 主对象的 create() 方法

- 

调用 EJB 的 sayHello() 方法
... 
public void getHelloData() { 
    try{ 
        InitialContext ctx = new InitialContext(); 
        Object obj = ctx.lookup("/Hello"); 
        HelloHome ejbHome = (HelloHome) 
        PortableRemoteObject.narrow(obj, HelloHome.class); 
        HelloObject ejbObject = ejbHome.create(); 
        String message = ejbObject.sayHello(); 
        } 
    catch (Exception e); 
    } 
...




## 保留的方法名称



Flex 远程库使用下面的方法名称；请不要将这些名称用于您自己的方法：
addHeader() 
addProperty() 
deleteHeader() 
hasOwnProperty() 
isPropertyEnumerable() 
isPrototypeOf() 
registerClass() 
toLocaleString() 
toString() 
unwatch() 
valueOf() 
watch()

而且，不应该以下划线 (_) 字符作为方法名称的开头。


通常，只需按服务变量来命名 RemoteObject 方法（操作）即可使它们可访问。但是，如果操作名称恰巧与针对服务定义的方法一样，就会发生命名冲突。可以针对 RemoteObject 组件使用以下 ActionScript 方法来返回给定名称的操作：
public function getOperation(name:String):Operation



## ActionScript 和 Java 之间的序列化



LiveCycle Data Services ES 和 BlazeDS 序列化 ActionScript (AMF 3) 和 Java 之间的数据以及两个方向中的 ColdFusion 数据类型。有关 ColdFusion 数据类型的信息，请参阅 ColdFusion 文档集。


#### 将数据从 ActionScript 转换为 Java


当方法参数将数据从应用程序发送到 Java 对象时，数据会自动从 ActionScript 数据类型转换为 Java 数据类型。当 LiveCycle Data Services ES 或 BlazeDS 搜索 Java 对象的合适的方法时，它会进一步使用更宽松的转换来查找匹配项。


客户端上的简单数据类型（如 Boolean 和 String 值）通常与远程 API 完全匹配。但是，Flex 在搜索 Java 对象的合适的方法时，会尝试一些简单的转换。


ActionScript Array 可以通过两种方法来编制条目索引。严格 Array 中的所有索引均为 Number 类型。关联 Array 中至少有一个索引是基于 String 类型的。一定要了解向服务器发送的 Array 的类型，因为 Array 类型会更改用来针对 Java 对象调用方法的参数的数据类型。密集 Array 中所有的数值索引都是连续的（即没有间隔），从 0（零）开始。稀疏 Array 的数值索引之间有间隔；这种
 Array 作为对象来处理，数值索引会变为属性，这些属性会序列化为 java.util.Map 对象，以免发送许多空条目。


下表列出了对于简单数据类型所支持的 ActionScript (AMF 3) 到 Java 转换。

|ActionScript 类型 (AMF 3)|反序列化为 Java|支持的 Java 类型绑定|
|----|----|----|
|Array（密集）|java.util.List|java.util.Collection, Object[ ] (本机数组)如果类型是一个接口，则会映射到下面的接口实现：- List 变为 ArrayList- SortedSet 变为 TreeSet- Set 变为 HashSet- Collection 变为 ArrayList自定义 Collection 实现的新实例会绑定到该类型。|
|Array（稀疏）|java.util.Map|java.util.Map|
|Boolean字符串"true"或"false"|java.lang.Boolean|Boolean、boolean 和 String|
|flash.utils.ByteArray|byte []||
|flash.utils.IExternalizable|java.io.Externalizable||
|Date|java.util.Date（已设置为协调世界时 (UTC) 格式）|java.util.Date、java.util.Calendar、java.sql.Timestamp、java.sql.Time 和 java.sql.Date|
|int/uint|java.lang.Integer|java.lang.Double、java.lang.Long、java.lang.Float、java.lang.Integer、java.lang.Short、java.lang.Byte、java.math.BigDecimal、java.math.BigInteger、String，以及基元类型 double、long、float、int、short 和 byte|
|null|null|基元|
|Number|java.lang.Double|java.lang.Double、java.lang.Long、java.lang.Float、java.lang.Integer、java.lang.Short、java.lang.Byte、java.math.BigDecimal、java.math.BigInteger、String、0（零）如果发送了 null，则为基元类型 double、long、float、int、short 和 byte|
|Object（泛型）|java.util.Map|如果指定了 Map 接口，则为 java.util.Map 创建一个新的 java.util.HashMap，为 java.util.SortedMap 创建一个新的 java.util.TreeMap。|
|String|java.lang.String|java.lang.String、java.lang.Boolean、java.lang.Number、java.math.BigInteger、java.math.BigDecimal、char[]、以及任何基元数字类型|
|有类型对象|有类型对象在使用 [RemoteClass] 元数据标签指定远程类名称时。Bean 类型必须具有公共的无参数构造函数。|有类型对象|
|undefined|null|null（对于对象）和默认值（对于基元）|
|XML|org.w3c.dom.Document|org.w3c.dom.Document|
|XMLDocument（旧 XML 类型）|org.w3c.dom.Document|org.w3c.dom.Document可以针对在 services-config.xml 文件中定义的任何通道启用对于 XMLDocument 类型的旧 XML 支持。此设置仅在将数据从服务器发回到客户端时很重要，它控制 org.w3c.dom.Document 实例如何发送到 ActionScript。有关更多信息，请参阅[为通道配置 AMF 序列化](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7b7c.html)。|



在 Java 中，基元值不能设置为 null。在将 Boolean 和 Number 值从客户端传递到 Java 对象时，Flex 将 null 值解释为基元类型的默认值，例如，0（对于
 double、float、long、int、short 和 byte）、\u0000（对于 char）和 false（对于 Boolean）。只有基元 Java 类型才能获得默认值。


LiveCycle Data Services ES 和 BlazeDS 会像处理任何其它已设置类型的对象那样处理 java.lang.Throwable 对象。处理 java.lang.Throwable 对象时遵循的规则是查找公共字段和 bean 属性，并将有类型对象返回到客户端。这些规则与普通的 bean 规则相似，区别在于这些规则会检查 getter 是否具有只读属性。这允许您从 Java 异常获取更多信息。如果需要 Throwable 对象的旧行为，则可以将通道的 legacy-throwable 属性设置为 true；有关更多信息，请参阅[为通道配置
 AMF 序列化](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7b7c.html)。


可以将严格 Array 以参数形式传递给要求 java.util.Collection 实现或本机 Java Array API 的方法。


Java Collection 可以包含任意数量的 Object 类型，而 Java Array 要求各个条目具有相同的类型（例如，java.lang.Object[ ] 和 int[ ])）。


LiveCycle Data Services ES 和 BlazeDS 也将 ActionScript 严格 Array 转换为常见 Collection API 接口的适当实现。例如，如果将 ActionScript 严格 Array 发送到 Java 对象方法 public void addProducts(java.util.Set
 products)，则 LiveCycle Data Services ES 和 BlazeDS 会先将其转换为 java.util.HashSet 实例然后再以参数形式传递它，这是由于 HashSet 是 java.util.Set 接口的适当实现。同样，LiveCycle Data Services ES 和 BlazeDS 将 java.util.TreeSet 实例传递到用 java.util.SortedSet 接口设置类型的参数。


LiveCycle Data Services ES 和 BlazeDS 会将 java.util.ArrayList 实例传递到使用 java.util.List 接口以及任何其它扩展了 java.util.Collection 的接口来设置类型的参数。这些类型随后将作为 mx.collections.ArrayCollection 实例发回到客户端。如果需要将标准 ActionScript Array 发送回客户端，则必须在通道定义的属性的 serialization 部分中将 legacy-collection 元素设置为 true。有关更多信息，请参阅[为通道配置
 AMF 序列化](http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7b7c.html)。



#### 显式映射 ActionScript 和 Java 对象


对于 LiveCycle Data Services ES 和 BlazeDS 不隐式处理的 Java 对象，在公共 bean 属性中找到的具有 get/set 方法和公共变量的值将作为 0bject 的属性发送到客户端。私有属性、常量、静态属性和只读属性等将不会进行序列化。对于 ActionScript 对象，用 get/set 访问器和公共变量定义的公共属性将发送到服务器。


LiveCycle Data Services ES 和 BlazeDS 使用标准的 Java 类 (java.beans.Introspector) 获取 JavaBean 类的属性描述符，还使用反射功能收集类的公共字段，并且优先使用 bean 属性（而不是字段）。Java 属性名与 ActionScript 属性名应该匹配。本机 Flash Player 代码用来确定 ActionScript 类如何在客户端上进行内部检查。


在 ActionScript 类中，可以使用 [RemoteClass(alias=" ")] 元数据标签来创建直接映射到 Java 对象的 ActionScript 对象。数据所转换到的 ActionScript 类必须在 MXML 文件中使用或引用，才能链接到 SWF 文件并在运行时可用。实现上述操作的一个好办法是对结果对象进行强制转换，如下面的示例所示：
var result:MyClass = MyClass(event.result);

类本身应该使用强类型引用，以便它的依赖项也可以被链接。


下面的示例显示了一个使用 [RemoteClass(alias=" ")] 元数据标签的 ActionScript 类的源代码：
package samples.contact { 
    [Bindable] 
    [RemoteClass(alias="samples.contact.Contact")] 
    public class Contact { 
        public var contactId:int; 
 
        public var firstName:String; 
 
        public var lastName:String; 
 
        public var address:String; 
 
        public var city:String; 
 
        public var state:String; 
 
        public var zip:String; 
    } 
}

如果您不映射到服务器上的 Java 对象，但需要将对象类型从服务器发回，则可以使用没有别名的 [RemoteClass] 元数据标签。在将 ActionScript 对象发送到服务器时，它会序列化为一个特殊的 Map 对象，但是从服务器返回到客户端的对象还是最初的 ActionScript 类型。


为了禁止将特定属性从 ActionScript 类发送到服务器，需要在 ActionScript 类中该属性声明的上方使用 [Transient] 元数据标签。



#### 将数据从 Java 转换为 ActionScript


从 Java 方法返回的对象将由 Java 转换为 ActionScript。LiveCycle Data Services ES 和 BlazeDS 也处理在对象中找到的对象。LiveCycle Data Services ES 隐式处理下表中的 Java 数据类型。

|Java 类型|ActionScript 类型 (AMF 3)|
|----|----|
|java.lang.String|String|
|java.lang.Boolean, boolean|Boolean|
|java.lang.Integer, int|int如果值小于 0xF0000000 且大于 0x0FFFFFFF，则会按照 AMF 编码要求将值提升为 Number。|
|java.lang.Short, short|int如果 i 小于 0xF0000000 且大于 0x0FFFFFFF，则会将值提升为 Number。|
|java.lang.Byte, byte[]|int如果 i 小于 0xF0000000 且大于 0x0FFFFFFF，则会将值提升为 Number。|
|java.lang.Byte[]|flash.utils.ByteArray|
|java.lang.Double, double|Number|
|java.lang.Long, long|Number|
|java.lang.Float, float|Number|
|java.lang.Character, char|String|
|java.lang.Character[], char[]|String|
|java. math.BigInteger|String|
|java.math.BigDecimal|String|
|java.util.Calendar|Date日期按照协调世界时 (UTC) 时区的时间进行发送。客户端和服务器必须根据时区相应地调整时间。|
|java.util.Date|Date日期按照 UTC 时区的时间进行发送。客户端和服务器必须根据时区相应地调整时间。|
|java.util.Collection（例如，java.util.ArrayList）|mx.collections.ArrayCollection|
|java.lang.Object[]|Array|
|java.util.Map|Object（无类型）。例如，将 java.util.Map[] 转换为对象的 Array。|
|java.util.Dictionary|Object（无类型）|
|org.w3c.dom.Document|XML 对象|
|null|null|
|java.lang.Object（以前列出的类型除外）|有类型 Object通过使用 JavaBean 内部检查规则将对象进行序列化，并且对象包括公共字段。不包括静态字段、瞬态字段、非公共字段，以及非公共 bean 属性或静态 bean 属性。|




#### 为通道配置 AMF 序列化


您可以支持早期版本的 Flex 中所使用的旧 AMF 类型序列化，并在 services-config.xml 文件的通道定义中配置其它序列化属性。


下表描述了可以在通道定义的 <serialization> 元素中设置的属性：

|属性|说明|
|----|----|
|<ignore-property-errors>    true</ignore-property-errors>|默认值为 true。确定在传入的客户端对象具有无法在服务器对象上设置的意外属性时，端点是否应该引发错误。|
|<log-property-errors>    false</log-property-errors>|默认值为 false。如果为 true，则会记录意外的属性错误。|
|<legacy-collection>false</legacy-collection>|默认值为 false。如果为 true，则 java.util.Collection 实例将以 ActionScript Array 形式返回。如果为 false，则 java.util.Collection 的实例将以 mx.collections.ArrayCollection 形式返回。|
|<legacy-map>false</legacy-map>|默认值为 false。如果为 true，则 java.util.Map 实例将序列化为 ECMA Array 或关联 Array，而不是匿名 Object。|
|<legacy-xml>false</legacy-xml>|默认值为 false。如果为 true，则 org.w3c.dom.Document 实例将序列化为 flash.xml.XMLDocument 实例，而不是内部 XML（支持 E4X）实例。|
|<legacy-throwable>false</legacy-throwable>|默认值为 false。如果为 true，则 java.lang.Throwable 实例将序列化为 AMF 状态信息对象（而不是普通的 bean 序列化，包括只读属性）。|
|<type-marshaller>    className</type-marshaller>|指定 flex.messaging.io.TypeMarshaller 的一个实现，该实现将对象转换为所需类的实例。在调用 Java 方法或者填充 Java 实例，如果反序列化中的输入对象的类型（例如，ActionScript 匿名 Object 始终反序列化为 java.util.HashMap）与目标 API（例如，java.util.SortedMap）不匹配，使用该属性。因此，该类型可以封送成所需的类型。|
|<restore-references>    false</restore-references>|默认值为 false。一个高级开关，在必须进行类型转换时，让反序列化程序跟踪对象引用；例如，在为 java.util.SortedMap 类型的属性发送匿名 Object 时，该 Object 将首先像平常那样反序列化为 java.util.Map，然后转换为合适的 SortedMap 实现（如 java.util.TreeMap）。如果其它对象指向对象图中的同一个匿名 Object，则该设置将复原这些引用，而不是到处创建 SortedMap 实现。请注意，如果将此属性设置为 true，在存在大量数据时，将会大大降低性能。|
|<instantiate-types>    true</instantiate-types>|默认值为 true。一个高级开关，如果设置为 false，会禁止反序列化程序创建强类型对象的实例，而是保留类型信息并反序列化 Map 实现（即 flex.messaging.io.ASObject）中的原始属性。请注意 flex.* 包下面的任何类总是会进行实例化。|





### 使用自定义序列化



如果用来在客户端上的 ActionScript 和服务器上的 Java 之间序列化和反序列化数据的标准机制无法满足您的需要，您可以编写自己的序列化方案。您可以在客户端实现基于 ActionScript 的[flash.utils.IExternalizable](http://help.adobe.com/zh_CN/AS3LCR/Flex_4.0/flash/utils/IExternalizable.html) 接口，在服务器上实现基于
 Java 的相应 java.io.Externalizable 接口。


使用自定义序列化的一个典型原因是，避免在整个网络层传递某个对象的客户端或服务器端表示的所有属性。在实现自定义序列化时，可以对类进行编码，从而使仅在客户端或仅在服务器端具有的特定属性不通过网络传递。在使用标准的序列化方案时，所有的公共属性都在客户端和服务器之间来回传递。


在客户端，用来实现 flash.utils.IExternalizable 接口的类的标识是在序列化流中编写的。该类序列化和重新构造其实例的状态。该类实现 IExternalizable 接口的 writeExternal() 和 readExternal() 方法，以控制对象及其父类型的序列化流的内容和格式，而不是类名或类型。这些方法取代了本机的
 AMF 序列化行为。这些方法必须与其远程对应部分对称，才能保存类的状态。


在服务器端，用来实现 java.io.Externalizable 接口的 Java 类与用来实现 flash.utils.IExternalizable 接口的 ActionScript 类执行相似的功能。

注： 如果需要精确按引用序列化，则不应使用通过 HTTPChannel 实现 IExternalizable 接口的类型。如果这样做，重复出现的对象之间的引用将丢失而且似乎在端点处克隆。


下面的示例显示一个客户端 (ActionScript) 版本的 Product 类的完整源代码，该类映射到服务器端的基于 Java 的 Product 类。客户端 Product 实现 IExternalizable 接口，而服务器端 Product 实现 Externalizable 接口。
// Product.as 
package samples.externalizable { 
 
import flash.utils.IExternalizable; 
import flash.utils.IDataInput; 
import flash.utils.IDataOutput; 
 
[RemoteClass(alias="samples.externalizable.Product")] 
public class Product implements IExternalizable { 
    public function Product(name:String=null) { 
        this.name = name; 
    } 
 
    public var id:int; 
    public var name:String; 
    public var properties:Object; 
    public var price:Number; 
 
    public function readExternal(input:IDataInput):void { 
        name = input.readObject() as String; 
        properties = input.readObject(); 
        price = input.readFloat(); 
    } 
 
    public function writeExternal(output:IDataOutput):void { 
        output.writeObject(name); 
        output.writeObject(properties); 
        output.writeFloat(price); 
    } 
} 
}

客户端 Product 使用两种序列化：与 java.io.Externalizable 接口兼容的标准序列化；AMF 3 序列化。下面的示例显示客户端 Product 的 writeExternal() 方法。该方法同时使用这两种类型的序列化：
public function writeExternal(output:IDataOutput):void { 
    output.writeObject(name); 
    output.writeObject(properties); 
    output.writeFloat(price); 
}

如下面的示例所示，服务器端 Product 的 writeExternal() 方法与该方法的客户端版本几乎完全相同：
public void writeExternal(ObjectOutput out) throws IOException { 
    out.writeObject(name); 
    out.writeObject(properties); 
    out.writeFloat(price); 
}

在客户端 Product 的 writeExternal() 方法中，flash.utils.IDataOutput.writeFloat() 方法是标准序列化方法的一个示例，能够满足用于处理基元类型的
 Java java.io.DataInput.readFloat() 方法的规范。此方法向服务器端 Product 发送 price 属性（是一个
 Float 值）。


在客户端 Product 的 writeExternal() 方法中，使用 AMF 3 序列化的示例是对 flash.utils.IDataOutput.writeObject() 方法的调用，该方法映射到服务器端
 Product 的 readExternal() 方法中的java.io.ObjectInput.readObject() 方法调用。flash.utils.IDataOutput.writeObject() 方法向服务器端
 Product 发送 properties 属性（该属性是一个 Object）和 name 属性（该属性是一个 String）。之所以能够实现这一点，是因为 AMFChannel 端点中实现了一个 java.io.ObjectInput 接口，该接口获得 writeObject() 方法发送的数据以将其设置为
 AMF 3 格式。


反过来，当在服务器端 Product 的 readExternal() 方法中调用 readObject() 方法时，将使用 AMF
 3 反序列化；这正是将 ActionScript 版本的 properties 值假定为 Map 类型，而将 name 的类型假定为
 String 的原因。


下面的示例显示了服务器端 Product 类的完整源代码：
// Product.java 
package samples.externalizable; 
 
import java.io.Externalizable; 
import java.io.IOException; 
import java.io.ObjectInput; 
import java.io.ObjectOutput; 
import java.util.Map; 
 
/** 
* This Externalizable class requires that clients sending and  
* receiving instances of this type adhere to the data format 
* required for serialization. 
*/ 
public class Product implements Externalizable { 
    private String inventoryId; 
    public String name; 
    public Map properties; 
    public float price; 
 
    public Product() 
    { 
    } 
 
        /** 
        * Local identity used to track third-party inventory. This property is 
        * not sent to the client because it is server specific. 
        * The identity must start with an 'X'. 
        */ 
        public String getInventoryId() { 
            return inventoryId; 
        } 
 
        public void setInventoryId(String inventoryId) { 
            if (inventoryId != null && inventoryId.startsWith("X")) 
            { 
                this.inventoryId = inventoryId; 
            } 
            else 
            { 
                throw new IllegalArgumentException("3rd party product 
                inventory identities must start with 'X'"); 
            } 
        } 
 
        /** 
         * Deserializes the client state of an instance of ThirdPartyProxy 
         * by reading in String for the name, a Map of properties 
         * for the description, and  
         * a floating point integer (single precision) for the price.  
         */ 
        public void readExternal(ObjectInput in) throws IOException, 
            ClassNotFoundException { 
            // Read in the server properties from the client representation. 
            name = (String)in.readObject(); 
            properties = (Map)in.readObject(); 
            price = in.readFloat(); 
            setInventoryId(lookupInventoryId(name, price)); 
        } 
 
        /** 
         * Serializes the server state of an instance of ThirdPartyProxy 
         * by sending a String for the name, a Map of properties 
         * String for the description, and a floating point 
         * integer (single precision) for the price. Notice that the inventory  
         * identifier is not sent to external clients. 
         */ 
        public void writeExternal(ObjectOutput out) throws IOException { 
            // Write out the client properties from the server representation. 
            out.writeObject(name); 
            out.writeObject(properties); 
            out.writeFloat(price); 
        } 
         
        private static String lookupInventoryId(String name, float price) { 
            String inventoryId = "X" + name + Math.rint(price); 
            return inventoryId; 
        } 
}

下面的示例显示了服务器端 Product 的 readExternal() 方法：
public void readExternal(ObjectInput in) throws IOException, 
    ClassNotFoundException { 
    // Read in the server properties from the client representation. 
    name = (String)in.readObject(); 
    properties = (Map)in.readObject(); 
    price = in.readFloat(); 
    setInventoryId(lookupInventoryId(name, price)); 
    }

在序列化期间，客户端 Product 的 writeExternal() 方法不向服务器发送 id 属性，因为该属性对于服务器版本的
 Product 对象没有用处。同样，服务器端 Product 的 writeExternal() 方法不向客户端发送inventoryId 属性，因为该属性是一个特定于服务器的属性。


请注意，在序列化过程中，在任何一个方向都不发送 Product 的属性名。由于类的状态是固定而且可管理的，因此属性将按照具有完善定义的顺序发送而且发送时不带名称，readExternal() 方法会按照相应的顺序读取它们。


from:  [http://help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html](http://http//help.adobe.com/zh_CN/Flex/4.0/AccessingData/WS2db454920e96a9e51e63e3d11c0bf69084-7fda.html)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=javabeans&t=blog)](https://so.csdn.net/so/search/s.do?q=ejb&t=blog)](https://so.csdn.net/so/search/s.do?q=flex&t=blog)




