
# WebService-（一）了解并使用webService - wangzhiguo9261的博客 - CSDN博客


2018年02月25日 13:55:29[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：51


# [WebService 学习之路（一）：了解并使用webService](http://www.cnblogs.com/xiaochangwei/p/4969448.html)
webService主要用于向其他系统提供接口以便调用，系统间可能开发语言等完全不同，根据约定的接口规范，调用者传递相关参数进行接口调用，服务方根据传入的条件进行业务处理并进行结果返回。
webSercive现在有很多框架支持，并且可以和主流框架如Spring进行集成，更易于操作。推荐使用Apache CXF. 不过今天学习暂时不设计CXF，后期将会以具体实例展示SpringMVC和CXF、restful的集成及使用。[Springmvc集成CXF请看教程二 http://www.cnblogs.com/xiaochangwei/p/5399507.html](http://www.cnblogs.com/xiaochangwei/p/5399507.html)
现在采用最传统的方式来进行简单的webService创建及演示。
① 打开eclipse创建一个java工程并新建class，代码如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
packagewebService001;importjavax.jws.WebService;importjavax.xml.ws.Endpoint;
@WebServicepublicclassWebService001 {publicvoidtestWebService(String name) {
        System.out.println("Test sucessfully, you input name is :" +name);
    }publicstaticvoidmain(String[] args) {
        Endpoint.publish("http://www.xiaochangwei.com:8888/webService/test",newWebService001());
        System.out.println("webService pulish completed!");
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
需要注意的是class上有@Webservice注解.   右键 run as javaApplication，待控制台打出public complete，请保持此程序一直运行，不要关闭
打开浏览器，输入publish中的地址，并在末尾加上?wsdl，如:http://www.xiaochangwei.com:8888/webService/test?wsdl   打开后如下即表示发布成功
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160416151944848-607818919.png)
② 写客户端代码，来进行调用
打开eclise，新建项目，选择web service client，在新打开的窗口中service defination中输入 http://www.xiaochangwei.com:8888/webService/test?wsdl，finish后会看见项目中有5个java文件，
XXXXX.java
XXXXXPortBindingStub.java
XXXXXProxy.java
XXXXXService.java
XXXXXServiceLocator.java
这些都是根据wsdl定义文件给我们生成的本地java文件，这样我们可以像操作本地的java类一样进行远程服务的调用
新建ServiceTest.java类进行测试，代码如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
packagewebService001;publicclassServiceTest {publicstaticvoidmain(String[] args)throwsException {
        WebService001 test=newWebService001ServiceLocator().getWebService001Port();
        test.testWebService("xiaochangwei");
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
执行后，由于我们是直接从控制台打出消息，所以需要我们到发布service的eclipse控制台查看打印的信息，当然可以修改为返回String等任意对象，在客户端显示
![](https://images2015.cnblogs.com/blog/658609/201604/658609-20160416153409645-1497986947.png)
上述就是webservice的入门实例，有兴趣的朋友请持续关注，近期将发布SpringMVC中集成CXF发布webservice及相关配置、使用等
即下面的配置方式，不需要再生成如此多的文件
<jaxws:endpointid="helloWorld"implementor="\#hello"address="/HelloWorld"/>
[Springmvc集成CXF请看教程二 http://www.cnblogs.com/xiaochangwei/p/5399507.html](http://www.cnblogs.com/xiaochangwei/p/5399507.html)


