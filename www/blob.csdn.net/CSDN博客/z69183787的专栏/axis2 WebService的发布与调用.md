# axis2 WebService的发布与调用 - z69183787的专栏 - CSDN博客
2014年08月05日 11:42:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：876
1：准备：
    JDK:http://www.oracle.com/technetwork/java/javase/downloads/jdk6downloads-1902814.html
    eclipse:http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/juno/SR2/eclipse-jee-juno-SR2-win32.zip
    tomcat:http://tomcat.apache.org/download-60.cgi
    axis2:http://axis.apache.org/axis2/java/core/download.cgi
    eclipse的官方提供的axis插件工具，可以打包aar文件和生成客户端调用代码:http://axis.apache.org/axis2/java/core/tools/index.html
        其中的Service Archive Wizard - Eclipse Plug-in和Code Generator Wizard - Eclipse Plug-in
下载完成的软件如图1.1
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/%E8%BD%AF%E4%BB%B6%E5%88%97%E8%A1%A8.png)图1.1
2：配置环境：
    2.1:配置java环境变量（不赘述）。
    2.1:eclipse中axis2环境配置：Window->Perferences->Web Services->Axis2 perferences->Axis2 Runtime->Axis2 Runtime location,Browse选择解压axis2-1.6.2-bin.zip得到的axis2-1.6.2文件目录。如图2.1。
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/axis2%E9%85%8D%E7%BD%AE.png)图2.1
    2.2：安装插件：解压axis2-eclipse-codegen-plugin-1.6.2.zip和axis2-eclipse-service-plugin-1.6.2.zip，把得到的两个jar包放入eclipse目录下的\plugins\中，重启eclipse。
    2.3：配置tomcat:解压apache-tomcat-6.0.36-windows-x64.zip（不赘述）。
    2.4：eclipse中tomcat配置：Window->Perferences->Server->Runtime Environments添加。
3：发布axis2：
    3.1：解压axis2-1.6.2-war.zip获得axis2.war并把它放到tomcat解压目录的webapps文件夹下，启动tomcat，浏览器中输入http://localhost:8080/axis2/，出现图3.1，说明配置成功。
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%842222.png)图3.1
    3.2：用eclipse的axis2插件发布web服务。
        3.2.1    在eclipse中new一个Dynamic Web Project，取名webserviceService。编写一个简单的webService服务器代码
1package org.web.service;
2
3publicclass HelloWorldService {
4public String sayHello(String name){
5return "Hello," + name;
6     }
7 }
8
        3.2.2    在eclipse的空白workspace处，右键new->Other,在弹出的对话框中，找到Axis2 Service Archiver,双击->选择HelloWorldService所在项目的class路径，如图3.2
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%84.png)图3.2
next->选中skip WSDL,Next->什么都不填NEXT->选中Generate the service xml automatically,NEXT->
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%8421.png)图3.3
如如图3.3填写HelloWorldService类的全路径，点击load，在下面的Method表中出现sayHello说明load成功，点击NEXT->
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%84211.png)图3.4
    填写Output file location，点击Finish，如图3.4。
        3.2.2    右键点击webServiceService项目，刷新。出现my_service.aar文件，如图3.5。
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%842111.png)图3.5
    把此aar文件放到%tomcat_home%\webapps\axis2\WEB-INF\services下。浏览器中输入http://localhost:8080/axis2/services/HelloWorldService?wsdl，出现图3.6，说明发布成功。
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/class%E8%B7%AF%E5%BE%8433.png)图3.6
4：用eclipse的Web Service Client生成客户端调用代码。
    4.1：在eclipse的空白workspace处右键new->Other->Web services->Web Service Client,选中，点击NEXT->出现图4.1![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/webserviceclient1.png)图4.1
service definition填发布好的wsdl路径http://localhost:8080/axis2/services/HelloWorldService?wsdl，Client type默认，下面的下滑快拉到最上面，点击Server runtime：Tomcat v6.0 Server出现图4.2：
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/webserviceclient2.png)图4.2
在Server runtime中选择默认，Web service runtime选择Apache Axis2，点击Ok，返回图4.1，点击Client project：webServiceClient，出现图4.3![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/webserviceclient3.png)图4.3
在Client project的下拉列表中选择客户端代码存放的项目，本例选择webServiceClient。点击OK，返回图4.1，点击NEXT，进入下一个环节，然后点击Finish。
,![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/webserviceclient4.png)图4.4
如图4.4，在src的source folder下出现org.web.service包，下面有HelloWorldServiceCallBackHandler.java和HelloWorldServiceStub.java文件，Web App Libraries也有更新，在WebContent目录下也出现axis2-web文件夹，以及WEB-INF等的更新。
    4.2：写webService调用代码。
            在webServiceClient项目中新建一个客户端测试文件如下：
 1package org.web.client;
 2
 3import java.rmi.RemoteException;
 4
 5import org.web.service.HelloWorldServiceStub;
 6import org.web.service.HelloWorldServiceStub.SayHelloResponse;
 7
 8publicclass HelloWorldClient {
 9
10/**
11     * @param args
12     * @throws RemoteException 
13*/
14publicstaticvoid main(String[] args) throws RemoteException {
15         String target = "http://localhost:8080/axis2/services/HelloWorldService";
16         HelloWorldServiceStub stub = new HelloWorldServiceStub(target);
17// sayHello 为webService提供参数
18        HelloWorldServiceStub.SayHello sayHello = new HelloWorldServiceStub.SayHello();
19         sayHello.setName("jackii");
20         SayHelloResponse eur = stub.sayHello(sayHello);
21         String returnVal = eur.get_return();
22         System.out.println(returnVal);
23     }
24
25 }
运行上面代码Run As->Java Application,输出：
Hello,jackii
说明调用成功。
5：参考文档http://wenku.baidu.com/view/12501ed7195f312b3169a54b.html
6：服务端接收的参数为javaBean，返回list样例：
    6.1：创建服务。新建User.java
 1package org.web.service;
 2
 3publicclass User {
 4private String id;
 5private String name;
 6public String getId() {
 7return id;
 8     }
 9publicvoid setId(String id) {
10this.id = id;
11     }
12public String getName() {
13return name;
14     }
15publicvoid setName(String name) {
16this.name = name;
17     }
18 }
19
ListService.java
 1package org.web.service;
 2
 3import java.util.ArrayList;
 4import java.util.List;
 5
 6publicclass ListService {
 7public List<User> getUserList(User user){
 8         List<User> returnList = new ArrayList<User>();
 9         returnList.add(user);
10for(int i=0;i<3;i++){
11             User user1 = new User();
12             user1.setId("00"+i);
13             user1.setName("jack00"+i);
14             returnList.add(user1);
15         }
16return returnList;
17     }
18 }
文件目录如图6.1：
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/6.1.png)图6.1
按照3.2说明重新发布服务（图3.4Output File Name重新起个名字）
6.2：创建客户端调用代码，步奏同4。得到图6.2所示两个文件ListServiceStub.java和ListServiceCallbackHandler.java
![](http://www.blogjava.net/images/blogjava_net/tianchijiaozi/6.2.png)图6.2
创建ListServiceClient.java
 1package org.web.client;
 2
 3import java.rmi.RemoteException;
 4
 5import org.web.service.ListServiceStub;
 6import org.web.service.ListServiceStub.GetUserListResponse;
 7import org.web.service.ListServiceStub.User;
 8
 9publicclass ListServiceClient {
10
11/**
12     * @param args
13     * @throws RemoteException 
14*/
15publicstaticvoid main(String[] args) throws RemoteException {
16         String target = "http://localhost:8080/axis2/services/ListService";
17         ListServiceStub stub = new ListServiceStub(target);
18         ListServiceStub.GetUserList getUserList0 = new ListServiceStub.GetUserList();
19         User user = new User();
20         user.setId("clientTest");
21         user.setName("ClientName");
22         getUserList0.setUser(user);
23         GetUserListResponse eur = stub.getUserList(getUserList0);
24         User[] userArray = eur.get_return();
25for(int i=0;i<userArray.length;i++){
26             System.out.println("id:"+userArray[i].getId()+"name:"+userArray[i].getName()+"\n");
27         }
28     }
29 }
以java application方式运行，输出：
1 id:clientTestname:ClientName
2
3 id:000name:jack000
4
5 id:001name:jack001
6
7 id:002name:jack002
说明调用成功。
