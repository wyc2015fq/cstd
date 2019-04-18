# IntelliJ IDEA 开发 WebService - z69183787的专栏 - CSDN博客
2016年12月07日 14:54:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7316
转自：http://www.biliyu.com/article/986.html
## 1、环境
- IntelliJ IDEA 14.0.2
- JAVA 1.6
- Tomcat 7
## 2、新建项目
进入`File > New Project…`菜单打开新建项目窗口，依次选择`Java`、`WebServices`，`Version`项选择 Apache
 Axis，`Libraries`项选择 Download。然后点击`Next`按钮进入下一页。
![](http://www.biliyu.com/wp-content/uploads/2015/07/52313ed725039c4ef1e2daa3cf938723.png)
在下一页中输入项目名称，然后点击`Finish`按钮开始下载依赖包。依赖包下载完成后进入新建的项目。
![](http://www.biliyu.com/wp-content/uploads/2015/07/7aa6e1115901a9c0d7c7f2a5aecc747a.png)
## 3、项目初始结构
![](http://www.biliyu.com/wp-content/uploads/2015/07/c28961e48f969fe5b83e2236f6680e41.png)
`src/example`目录下自动创建了一个 HelloWold
 类：
```
package example;/**
 * Created by Administrator on 2015/7/30.
 */
public class HelloWorld {
  public String sayHelloWorldFrom(String from) {
    String result = "Hello, world, from " + from;
    System.out.println(result);
    return result;
  }
}
```
`web/WEB-INF`目录下创建一个`server-config.wsdd`服务配置文件。
## 4、生成 WSDL 文件
在`HelloWorld.java`文件上点击右键，选
```
WebServices
 > Generate Wsdl From Java Code
```
 。
![](http://www.biliyu.com/wp-content/uploads/2015/07/55b47972124aa1a4fc7e49690c7d25ed.png)
然后在出现的窗口里点击`确定`按钮。
![](http://www.biliyu.com/wp-content/uploads/2015/07/257dfae12841797c0b5003cc2337058d.png)
在`HelloWorld.java`所在目录会生成一个`HelloWorld.wsdl`文件。
![](http://www.biliyu.com/wp-content/uploads/2015/07/2284732dd413a32368b374d946384687.png)
## 5、启动服务
启动服务这里稍微有点复杂，因为要配置 Tomcat 服务器。先喝口茶再继续往下看。
选择`Run > Edit Configurations…`菜单，打开
```
Run/Debug
 Configurations
```
窗口。
![](http://www.biliyu.com/wp-content/uploads/2015/07/61458ca12dda3d97de089619ce479887.png)
在`Run/Debug Configurations`窗口里添加`+`按钮，在出现的菜单里选择
```
Tomcat
 Server > Local
```
。如果看不到`Tocmat Server`菜单项，则点击最下面的
```
29
 items more (irelevant)…
```
菜单项查找。
![](http://www.biliyu.com/wp-content/uploads/2015/07/e40f32f76cf642f38912e6adc5776652.png)
然后在添加的`Tomcat Server`配置页面中添加配置的`Name`、
```
Application
 server
```
和`HTTP Port`。如果
```
Application
 server
```
为空，将本地的 Tomcat 安装地址添加进来。如果本地没有，从网上下载 Tomcat 7：
[https://tomcat.apache.org/download-70.cgi](https://tomcat.apache.org/download-70.cgi)
这时配置页面底部可能会显示有
```
Warning:No Artifacts marked
 for deployment
```
（警告：没有标记要部署的产物）。
点击警告信息右侧的`Fix…`按钮切换到Deployment标签页。
![](http://www.biliyu.com/wp-content/uploads/2015/07/7498244354a09e3443efe15f362bda25.png)
在Deployment标签页点击
```
Deploy
 at the server startup
```
区的`+`按钮，选择`Artifacts`
![](http://www.biliyu.com/wp-content/uploads/2015/07/c51587c315552c4b67a5653ec26df917.png)
添加 Artifact 后的界面如下，已经没有警告了。点击`OK`完成运行参数的配置。
![](http://www.biliyu.com/wp-content/uploads/2015/07/95cf09282f70ad6fdf774f6b20f8024e.png)
到这个时候，然后可以运行起服务了，但是访问服务页面时还是会报错。因为还要给部署的 Artifact 添加 Apache Axis 包的依赖。
选择`File> Project Structure…`菜单。
![](http://www.biliyu.com/wp-content/uploads/2015/07/7fbbccf0731e3a87c4e9e2a4564b3279.png)
在出现的窗口中选择`Project Settings > Artifacts`，会到在新的警告信息。点击警告信息右侧的`Fix…`按钮。
![](http://www.biliyu.com/wp-content/uploads/2015/07/c5ff73bac025194d63ae562346448fbb.png)
在弹出菜单里选择
```
Add 'JAX-WS-Apache Axis' to the
 artifact
```
。
![](http://www.biliyu.com/wp-content/uploads/2015/07/d98b4bc9767baf488339dcb931a57c63.png)
修复完成后警告消失。
![](http://www.biliyu.com/wp-content/uploads/2015/07/625a19244e18e2b1904593b13447a34d.png)
点击工具栏上的运行或调试按钮运行服务。
![](http://www.biliyu.com/wp-content/uploads/2015/07/9d996061151d91fc5c46105e418a5030.png)
然后在浏览器里输入：
[http://localhost:8080/services](http://localhost:8080/services)
访问结果如下：
![](http://www.biliyu.com/wp-content/uploads/2015/07/aceaf1c6c65256a11e4b03ec5fba8582.png)
## 6、从 WSDL 文件生成服务端代码
WSDL 文件可以用来生成客户端代码，也可以用来生成服务端代码。客户端代码用于访问远程服务，服务端代码用于提供远程服务。在 IntelliJ IDEA 里这两个操作是类似的。这里主要讲述服务端代码的生成。
一个 WSDL 文件对应一个 JAVA 类，比如前面的`HelloWorld.wsdl`文件对应`HelloWorld.java`。复杂的 WSDL 文件除了对应有一个主类外，还会有辅助类，比如主类方法中使用到的 Bean 等。
这里假设要将主类生成到 services 包目录下，那么在`src/services`（没有的自己创建一个）目录上点击右键，在出现在的菜单中选择
```
WebService
 > Genarete Java Code From Wsdl…
```
：
![](http://www.biliyu.com/wp-content/uploads/2015/07/cec019701f911e0b5d400b91a629f17a.png)
在出现的窗口是选择远程 WSDL 的地址，或者选择本地的 WSDL 文件，然后选择
```
Output
 mode（输出模式）
```
。这里作者选择成功`server`生成服务端代码，
 用于对外提供服务。当然也可以选择`client`生成客户端代码用于访问远程服务。
![](http://www.biliyu.com/wp-content/uploads/2015/07/3c3b0fc0dff2f00ff423cb2f2eb137e7.png)
下面是电信 TSM WebService 的 WSDL 文件生成的代码文件列表：
![](http://www.biliyu.com/wp-content/uploads/2015/07/b57b3edd8b6e73f4372c21de105b4715.png)
点击`OK`按钮后生成对应的文件。其中包含一个名为`deploy.wsdd`的文件，将该文件中的`service`标签复制到`web/WEB-INF/service-config.wsdd`文件的`<deployment>`标签下，用于对外发布。
运行项目的结果如下：
![](http://www.biliyu.com/wp-content/uploads/2015/07/c6523c0aad70b421de55230e9009940e.png)
## 7、输出 WSDL 文件
如果手动写的`service-config.wsdd`文件，可能不能访问
 WSDL 文件。这个时候可以在`server-config.wsdd`文件的`<deployment>`标签下手动添加`<transport>`标签。内容如下：
```xml
<deployment
        xmlns="http://xml.apache.org/axis/wsdd/"
        xmlns:java="http://xml.apache.org/axis/wsdd/providers/java">
  <transport name="http">
    <requestFlow>
      <handler type="java:org.apache.axis.handlers.http.URLMapper"/>
    </requestFlow>
  </transport>
  ...
</deployment>
```
