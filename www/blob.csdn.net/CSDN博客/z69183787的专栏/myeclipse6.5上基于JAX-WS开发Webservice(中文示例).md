# myeclipse6.5上基于JAX-WS开发Webservice(中文示例) - z69183787的专栏 - CSDN博客
2012年12月27日 15:46:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1003
个人分类：[IDE-Eclipse																[WebService																[WebService-JaxWs](https://blog.csdn.net/z69183787/article/category/2176831)](https://blog.csdn.net/z69183787/article/category/2175701)](https://blog.csdn.net/z69183787/article/category/2176833)
1. Introduction 
This document will outline the process of developing a JAX-WS web service and deploying it using MyEclipse 6.5 to the internal MyEclipse Tomcat server.
 The web service used in this tutorial will be a very simple calculator service that provides add, subtract, multiply and divide operations to the caller. 
MyEclipse also supports developing web services using the existing XFire framework from previous MyEclipse releases. For folks needing to develop and
 deploy WebSphere JAX-RPC or WebSphere JAX-WS web services, please take a look at our MyEclipse Blue Edition of the MyEclipse IDE. 
Additional resources covering web service creation using JAX-RPC, JAX-WS or XFire are included in the Resources section of this document. 
2. System Requirements 
This tutorial was created with MyEclipse 6.5. If you are using another version of MyEclipse (possibly newer), most of these screens and instructions
 should still be very similar. 
If you are using a newer version of MyEclipse and notice portions of this tutorial looking different than the screens you are seeing, please let us
 know and we will make sure to resolve any inconsistencies. 
3.新建一个工程 
开始我们新建一个Web Service Project工程File->New->Web Service Project(Optional Maven Support) 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0u4kTPsI/AAAAAAAAACc/XbGxXY1BgGM/s400/NewJAX-WS.jpg)
Note:A JAX-WS web service can
 also be generated in any existing Java EE 5 web project. 
我们给这个工程取名为WebServiceProject.注意JAX-WS支持只在javaEE5或更高版本的工程中是可行的。如果你需要使用低版本的工程类型(java1.4或者1.3)，那么只能使用XFire Web Service代替JAX-WS。 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0uP56yUI/AAAAAAAAACM/bUWdpIxNSFw/s400/Xfire.jpg)
这里我们使用上面的JAX—WS。 
4.创建服务类 
服务类就是一个普通的java类，负责提供我们想要发布的执行方法。这里我们写一个简单的计算器类，实现几个典型的计算器应用方法，如加减乘除。 
首先我们先建一个包，WebServiceProject->src->new->package,取名com.myeclipseide.ws 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0uV9yxpI/AAAAAAAAACU/YC273NEuoWc/package.jpg)
让后我们在这个包下建一个类,Calculator.java. 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0umepsSI/AAAAAAAAACY/g9xvRWO-5FQ/s400/Calculator.jpg)
根据上面提到的，这个计算器类实现计算器的加减乘除算法，简单实现： 
Java代码  ![收藏代码](http://horizonhyg.iteye.com/images/icon_star.png)
- package com.myeclipseide.ws;   
- 
- publicclass Calculator {   
- publicint add(int a, int b) {   
- return (a + b);   
-     }   
- 
- publicint subtract(int a, int b) {   
- return (a - b);   
-     }   
- 
- publicint multiply(int a, int b) {   
- return (a * b);   
-     }   
- 
- publicint divide(int a, int b) {   
- return (a / b);   
- 
    }   
**//支持返回自定义类型 **
- public User list(String name){
User u = new User();
u.setName(name);
return u;
}
public List<User> listAll(String name){
List<User> list = new ArrayList<User>();
for(int i=0;i<10;i++){
User u = new User();
u.setName(name);
list.add(u);
}
return list;
}
- }   
可以看出，这个类中的方法是非常简单的，没有用到特殊的注释还有接口，父类之类的东西。 
5.创建一个Web Service 
在上面的工具条中点击新建Web Service 
![](http://lh3.ggpht.com/_3nT0GM_9BIA/SgU0vODx0GI/AAAAAAAAACg/pF3bF-1e51c/newWebService.JPG)
Note：如果没有的话可以File->New->others->Myeclipse->WebService->webService
点击之后出现的屏幕，在Strategy中选择Bottom-up scenario，因为我们已经建立好了Calculator类而且想根据它建立JAX-WS服务。 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0vWWmoNI/AAAAAAAAACk/JafM-8XcSc4/s400/newWebService2.JPG)
下面是创建的最后一个屏幕，你需要选择提供webService方法的javaBean，在我们这个例子中就是我们已经建立好的Calculator类。 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0vllVM_I/AAAAAAAAACo/GwXaKJDTP7k/s400/lastScreen.jpg)
填好之后，Myeclipse会自动帮我们填满其他的项，Select
 Generate WSDL in project and hit Finish. 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0v3abAHI/AAAAAAAAACs/vDZCdBYeaik/s400/generateWSDL.jpg)
点击完成之后，Myeclipse会自动生成CalculatorDelegate代理类，还有一些必须的JAX-WS描述符，而且会自动在服务器目录下的web.xml中配置WebService的一些mappings，方便将webService部署到服务器中。 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0wI0Ob4I/AAAAAAAAACw/NLRbyalI1Zs/webSitem.jpg)
到此web service已经建立好了，我们开始部署它然后进行测试。 
6.部署和测试webService。 
这里我们不使用用Myeclipse自带的tomcat服务器，使用自己应经在电脑中部署好的tomcat5.5。 
在server面板中右击，选择configure 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0wThSzwI/AAAAAAAAAC0/5nccAVwjpiQ/s400/tomcat.jpg)
部署自己的tomcat注意选择jdk要跟项目中的相同。 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0xGDqDvI/AAAAAAAAAC4/TxnUVeeviQw/s400/jdk2.jpg)
现在要向工程中导入JAX-WS的jar包 
在项目名称上右击->properties->Add Library->Myeclipse Libraries->最后面的两个。 
![](http://lh6.ggpht.com/_3nT0GM_9BIA/SgU0xsDSw9I/AAAAAAAAADA/7SYnPVbDPCE/s400/upjar.jpg)
![](http://lh6.ggpht.com/_3nT0GM_9BIA/SgU0xfMDtdI/AAAAAAAAAC8/JEcgZx70AAc/s400/jar.jpg)
点击完成，导入成功。 
Note：Myeclipse自带的tomcat中有自带的这两个jar包，可以不用导入。 
6.1部署 
在部署好的tomcat服务器上右击选择Add Deployment 
![](http://lh6.ggpht.com/_3nT0GM_9BIA/SgU0xzSlXeI/AAAAAAAAADE/ZXzYJEyU7p4/deployweb1.JPG)
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0yDQTuVI/AAAAAAAAADM/Xtg8OMf3vlM/s400/deployweb2.JPG)
点击完成。 
6.2测试 
运行tomcat服务器，在工具栏中点击launch WebService Explorer 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0x-eAFUI/AAAAAAAAADI/6MouBOllS90/launch.JPG)
打开后，点击右上角的WSDL视图，可以看到下面的屏幕 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0zDEzvHI/AAAAAAAAADg/NUt3fExSQS4/s512/w1.jpg)
在WSDL URL中填写路径：http://localhost:8888/WebServiceProject/CalculatorPort?WSDL 
解释下路径组成： 
http://localhost:8888/是服务器的路径，我的端口号是8888，可以根据自己的更改，一般都是8080。 
/WebServiceProject = We know by
 default the Web Context-root that is used to deploy(部署) web projects matches the name of the projects. (因为我们没有为这个工程自定义我们的Web Context-root，所以他就是这个工程的名字) 
/CalculatorPort = As we saw from the last screenshot in Section #5, when our JAX-WS web service was generated, it was bound using a servlet-mapping
 in the web.xml file to the /CalculatorPort path. 
Xml代码  ![收藏代码](http://horizonhyg.iteye.com/images/icon_star.png)
- <servlet>
- <description>JAX-WS endpoint - CalculatorService</description>
- <display-name>CalculatorService</display-name>
- <servlet-name>CalculatorService</servlet-name>
- <servlet-class>
-         com.sun.xml.ws.transport.http.servlet.WSServlet  
- </servlet-class>
- <load-on-startup>1</load-on-startup>
- </servlet>
- <servlet-mapping>
- <servlet-name>CalculatorService</servlet-name>
- <url-pattern>/CalculatorPort</url-pattern>
- </servlet-mapping>
?WSDL = This is a universal query
 string argument that can be added to the end of any web service which will tell the web service to return it's full WSDL to the caller. In this case, the WSDL is returned to our Web Services Explorer tool which loads it up, and displays the web services exposed
 operations to us. 
弄清楚之后，我们开始测试，比如我们选择add方法： 
填写args，点击go，在status中就会显示结果。 
![](http://lh6.ggpht.com/_3nT0GM_9BIA/SgU0zT8HwXI/AAAAAAAAADk/9FuwjR5j-bc/s400/w2.jpg)
结果是正确的。 
7.创建Webservice Client 
现在我们已经部署好Webservice，而且应经测试过了，那我们新建一个Webservice client，来调用Webservice提供的方法。 
7.1新建一个java project，给他取个名字。比如我们叫它ClientofWebService 
![](http://lh3.ggpht.com/_3nT0GM_9BIA/SgU0yUSoxxI/AAAAAAAAADQ/pnEE-GeVOo0/s400/clent.jpg)
在工具条中点击new Web Service Client 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0yl11utI/AAAAAAAAADU/Zj1B2FnupyA/client1.JPG)
然后按照以下步骤操作： 
![](http://lh4.ggpht.com/_3nT0GM_9BIA/SgU0y3p_zvI/AAAAAAAAADY/YNA8Zdd2YvA/s400/client2.jpg)
The last step of the web service client creation is to specify either a WSDL File or a WSDL URL for the wizard to retrieve the web service WSDL from.
 In our case we are using the URL and generate the client into the new package com.myeclipseide.ws.client: 
http://localhost:8888/WebServiceProject/CalculatorPort?WSDL 
![](http://lh5.ggpht.com/_3nT0GM_9BIA/SgU0y-LoaPI/AAAAAAAAADc/USoMxIQNs5g/s400/client3.jpg)
点击Next知道完成。 
可以看到在新建的java project ClientofWebService中，src文件夹下产生了许多的文件，根据名称我们大体可以了解其意思，可以打开看一下源代码，其实不难理解。比如add文件，就是Calculator类中add方法的两个参数的get和set方法。其他类似。 
我们在文件夹下见一个类test.java写一个main函数测试 
Java代码  ![收藏代码](http://horizonhyg.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {   
- /* Create the service instance */
-     CalculatorService service = new CalculatorService();   
-     CalculatorDelegate delegate = service.getCalculatorPort();   
- 
- /* Using the web service, perform the 4 calculations */
-     System.out.println("1. 3+7=" + delegate.add(3, 7));   
-     System.out.println("2. 12-2=" + delegate.subtract(12, 2));   
-     System.out.println("3. 9*9=" + delegate.multiply(9, 9));   
- 
    System.out.println("4. 40/2=" + delegate.divide(40, 2));   
**User u = delegate.list("dasdsa");   java.util.List<User> list  = delegate.listAll("11111111111fdsf");   System.out.println(u.getName());**
- }   
运行得到如下结果： 
1. 3+7=10 
2. 12-2=10 
3. 9*9=81 
4. 40/2=20 
测试完成。 
8. Resources 
In this section we want to provide you with additional links to resources that supplement the topics covered in this tutorial. While this is not an
 exhaustive list, we do make an effort to point to the more popular links that should provide you with diverse, high-quality information. 
Example Web Service Project & Web Service Client Project 
NOTE: The example projects provided are configured to run against WebSphere 6.1. You may need to adjust the Target Server and/or the runtime JRE libraries
 used to build the projects to more closely match your particular build and deployment environment. 
有些地方说的不是很好，但是大体应该能够弄明白，限于能力问题，请大家体谅。
