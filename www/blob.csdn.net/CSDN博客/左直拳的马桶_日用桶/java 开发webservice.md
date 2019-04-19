# java 开发webservice - 左直拳的马桶_日用桶 - CSDN博客
2015年06月09日 18:57:52[左直拳](https://me.csdn.net/leftfist)阅读数：2104标签：[java																[webservice](https://so.csdn.net/so/search/s.do?q=webservice&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
这几天用java开发一个webservice，搞死了。java果然很难侍候！
传说java搞webservice，有好几种途径（为什么不是一种？要搞这么多种，让人一听，头都大了。当然啦，生物多样性总是一种好事。正如我们不能只有普通话，不能只有汉字一样，多种方案，多样技术，会启迪人的思维）
1、XFire 
eclipse默认提供的，我觉得最方便了。也用来做了一个HelloWorld，并且客户端调用测试通过。但当我添加引用了第三方的包之后，就不行了。可能是有冲突。 
本来就是要对这个第三方的包进行封装和二次开发，不用怎么能行，只好放弃这个XFire。
2、Axis2 
搞死了。 
1）安装 
首先要下载安装包和插件包。 
所谓的安装包，只要作用是提供一个类似JAVA_HOME一样的AXIS2_HOME环境变量，好方便一些时候用得上，比如生成客户端代码。 
插件包，用在eclipse上，方便创建和发布webservice。 
![这里写图片描述](https://img-blog.csdn.net/20150609181433434)
其中这个插件包，版本要与eclipse对应。我的eclipse版本比较低，结果搞个1.6的放到eclipse的plugins，死活看不到效果。后来换成1.4可以了
那两个插件的安装方法，是ZIP文件解压，将里面的文件夹拷贝到eclipse的plugins下。如图： 
![这里写图片描述](https://img-blog.csdn.net/20150826191229971)
有关安装的参考文章（这篇文章好像有误导，别尽信；网上其他一些教程，什么改名，添加link文件，应该是for linux的吧）： 
[http://www.blogjava.net/tianchijiaozi/archive/2013/03/15/396452.html](http://www.blogjava.net/tianchijiaozi/archive/2013/03/15/396452.html)
2）发布 
这里所谓的webservice，其实是建一个web project，就是一个网站了。然后再在其上建一个axis2 Service Archiver。具体方法： 
右击该项目 - 新建 - 其他 - Axis2 Service Archiver，然后一路默认next，选好发布aar包的路径，最后得到一个aar包。
注意我们写的webservice必须正确无误，才可以一路next。我今天就遇到其中有个服务返回值为自定义类型，导致点击next无反应。自定义类型应该也可以的吧，可能要做一些什么配置，不过我现在还不知道。
另外，有关指定编译好的class文件所在文件夹，通常web service的话，应该在web-inf/…，如图 
![这里写图片描述](https://img-blog.csdn.net/20150810181159416)
![这里写图片描述](https://img-blog.csdn.net/20150827093426528)
将这个aar包拷贝到tomcat下的 “\webapps\axis2\WEB-INF\services”，然后通过浏览器，输入类似 [http://localhost:8080/axis2/services/HelloWorldService?wsdl](http://localhost:8080/axis2/services/HelloWorldService?wsdl)这样的地址 测试是否部署成功
3）调用 
仅仅用浏览器进行测试部署没有意义，webservice要调用成功才算。 
我想用jsp来调用，但还没有成功。不过用java application成功了。 
方法： 
（1）生成客户端脚本 
去AXIS2的解压目录下bin(%AXIS2_HOME%\bin)下执行下面语句 
wsdl2java -uri [http://localhost:8080/axis2/services/HelloWorldService?wsdl](http://localhost:8080/axis2/services/HelloWorldService?wsdl) -p ws -s -o c:\temp 
-p参数指定了生成的Java类的包名 
-o参数指定了生成的一系列文件保存的根目录 
在c:\temp\src\ws自动生成HelloWorldServiceStub.java
据说也可以通过插件方式: 
IDE中选择New->other->Axis2 Code Generator来生成，但我总是报错。
（2）新建 java工程 Axis2Client 
新建\Axis2Client\lib文件夹 
将%AXIS2_HOME%\lib\ 下的jar包复制到\Axis2Client\lib，并加入工程引用中 
将通过脚本生成的单一HelloWorldServiceStub.java文件 加入到src\ws下：建立一个“源文件夹”:src，然后在src下新建一个包ws，将HelloWorldServiceStub.java复制到这个包里。
（3）新建test.TestWs.java 主要代码如下：
```java
package com.leftfist.test;
import java.rmi.RemoteException;
import ws.HelloWorldServiceStub;
public class TestWs {
    public static void main(String args[]) throws RemoteException  
     {  
//  初始化Sub类
    HelloWorldServiceStub stub = new HelloWorldServiceStub();
//  传递AxisServiceStub.ShowName对象，相关参数在这边赋值。
    HelloWorldServiceStub.SayHello command = new HelloWorldServiceStub.SayHello();
    command.setName("leftfist!!!");
//  取得返回值
    String name = stub.sayHello(command).get_return();
    System.out.println(name);
     }
}
```
附：HelloWorldService代码：
```java
public class HelloWorldService {
    public String sayHello(String name) {
        return "Hello," + name;
    }
}
```
里面的代码初初一看，觉得很奇怪，我们哪有写ShowName这样的方法？但这就是java webservice的调用方式。真怪异。
然后鼠标右击这个test.TestWs.java，运行方式 - java 应用程序，可看到结果矣！
参考文章： 
[http://www.cnblogs.com/cx-zyq/p/3334236.html](http://www.cnblogs.com/cx-zyq/p/3334236.html)
4）aar依赖的包 
利用插件发布aar的时候，如果一直狂按next，那么aar里面并没有我们要引用的一些包。这里估计有2种解决办法： 
（1）利用插件发布的时候，界面好像有提示要引用哪些包，把要用的包选进去，应该可以，但我没有试
（2）看TOMCAT下的axis2，其实就是一个网站，跟我们做的其他站点一起并肩放在webapps/下。把我们webservice需要用到的包、classes拷贝到 axis2/web-inf下，也可以的。我就是用了这种方法
搞死了，靠。
