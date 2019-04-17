# Webservice soap wsdl区别之个人见解 - DoubleLi - 博客园





- Web Service实现业务诉求：Web Service是真正“办事”的那个，提供一种办事接口的统称。
- WSDL提供“能办的事的文档说明”：对要提供的服务的一种描述格式。我想帮你的忙，但是我要告诉你我都能干什么，以及干这些事情需要的参数类型。
- SOAP提供“请求”的规范：向服务接口传递请求的格式，包括方法和参数等。你想让人家办事，总得告诉人家你想干什么吧，SOAP就是定义这个“请求”的格式的，按照SOAP定义的“请求”格式“书写”请求就可以保证Web Service能够正确的解读你想让它干什么以及你为它提供了什么参数。在这个请求中，你需要描述的主要问题有：向哪个Web Service发送请求，请求的参数类型、参数值、返回值类型。这些都“填写”完毕，也就完成了符合SOAP规范的SOAP消息。



wsdl和soap虽然是web service的两大标准，但是两者并没有必然的联系，都可以独立使用。


  wsdl提供了一个统一的接口，目前已经成为一个国际上公认的标准，通过wsdl提供的接口可以访问不同类型的资源（如java、c#、C、C、C++等），因为wsdl是基于xml，与语言平台无关的。另外wsdl提供了binding和service元素，用以绑定接口到具体的服务，实现了接口与实现的分离。

soap（简单对象访问协议）是一种基于http的传输协议，用来访问远程服务


  wsdl与soap的关系在于：wsdl绑定服务的时候可以设定使用的协议，协议可以是soap、http、smtp、ftp等任何一种传输协议，除此以外wsdl还可以绑定jms、ejb及local java等等，不过都是需要对binding和service元素做扩展的，而且需要扩展服务器的功能以支持这种扩展



soap协议是一种请求和应答协议规范，而http是web传输协议，soap的传输是可以基于http的，但也可以基于其他的传输协议，如ftp、smtp等。



简单对象访问协议（SOAP）是W3C组织的一个Note, 它描述了一种在分散的或分布式的环境中如何交换信息的轻量级协议。SOAP是一个基于XML的协议，它包括三个部分：SOAP封装(Envelop)，封装定义了一个描述消息中的内容是什么，是谁发送的，谁应当接受并处理它以及如何处理它们的框架；SOAP编码规则（Encoding Rules），用于表示应用程序需要使用的数据类型的实例；SOAP RPC表示(RPC Representation)，表示远程过程调用和应答的协定；SOAP可以和多种传输协议绑定（Binding），使用底层协议交换信息。在这个文档中，目前只定义了SOAP如何和HTTP以及HTTP扩展进行绑定的框架。

SOAP是个通信协议， SOAP在HTTP协议的基础上，把编写成XML的REQUEST参数, 放在HTTP BODY上提交个WEB SERVICE服务器(SERVLET，ASP什么的) 处理完成后，结果也写成XML作为RESPONSE送回用户端， 为了使用户端和WEB SERVICE可以相互对应，可以使用WSDL作为这种通信方式的描述文件，利用WSDL工具可以自动生成WS和用户端的框架文件，SOAP具备把复杂对象序列化捆绑到XML里去的能力。

SOAP的前身是RPC, 就是远程呼叫处理的协议，这个协议安全性不是很好，多数防火墙都会阻挡RPC的通信包，而SOAP则使用HTTP协议作为基本的协议，使用端口80使得SOAP可以透过防火墙，完成RPC的功能。

SOAP协议和HTTP协议一样，都是底层的通信协议，只是请求包的格式不同而已，SOAP包是XML格式的，现在我们编写WEB SERVICE不需要深入理解SOAP也没关系。如果SERVICE和CLIENT在同样的环境下使用SOAP，由于一般情况下都有自动生成SOAP程序框架的工具，因此不知道细节也没关系. 可是, 如果CLIENT和SERVICE的环境不同，比如说JAVA的Client和.NET的SERVICE进行通信，或者是VB CLIENT和TOMCAT下的JAVA SERVICE通信，还是要知道一点细节为好. 特别是, WSDL或者UDDI都不是标准，如果不让用就只好手工配制SOAP MESSAGE啦。










