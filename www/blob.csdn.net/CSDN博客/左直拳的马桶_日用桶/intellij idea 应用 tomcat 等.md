# intellij idea 应用 tomcat 等 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月08日 11:44:00[左直拳](https://me.csdn.net/leftfist)阅读数：699
[我越来越希望能掌握java](http://blog.csdn.net/leftfist/article/details/79286868)。可是，java不好学啊。
就拿这个开发工具来说吧。
之前断断续续用过eclipse。但这次想编译开源项目cas，人家的说明文件明确说了， elipse对gradle支持得不够好，加上本来就觉得eclipse怎么用怎么不舒服，所以我就冒险用intellij idea。这是捷克人搞的开发工具，咦，居然不是老美搞的。可以啊，捷克人。以前他们叫捷克斯洛伐克，现在拆分了吧。
结果怎么一个折腾了得。这是java的优点之一，就是不够傻瓜，不够友好，让你被迫加强动手能力，生命不息，折腾不止。相比之下，你看看.net的visual studio，不愧为宇宙级的开发工具，那叫一个爽。
怎么折腾法呢？
一、spring 
社区版（community）不支持，收费版（untimate）才支持。我刚开始下载安装的是社区版，新增项目时才发现与众不同，只好卸载重装收费版，破解又费了一番工夫。
二、spring initializr 
死活连不上 [https://start.spring.io](https://start.spring.io)。因为该网站加入了百度安全验证。只好手动在该网站手动生成一个框架，然后再在idea里导入。
三、加入jar包 
使用了gradle。对我来说是个新玩意，java玩的就是各种概念。当然，.net有nuget，node.js有npm，都差不多吧。依赖包改gradel脚本：
build.gradle
```
dependencies {
    compile('org.springframework.boot:spring-boot-starter')
    compile('org.springframework.boot:spring-boot-starter-web')//手动加的，支持import org.springframework.web.bind.annotation.RequestMapping;
    testCompile('org.springframework.boot:spring-boot-starter-test')
}
```
加了之后编译就OK了。
四、使用tomcat
1、首先要装一个tomcat
2、操作系统要配置环境变量 
比如 
![这里写图片描述](https://img-blog.csdn.net/20180208111453002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、run 该web项目 
系统会自动打开tomcat 
![这里写图片描述](https://img-blog.csdn.net/20180208112721962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180208112738316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180208112820870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码结构： 
![这里写图片描述](https://img-blog.csdn.net/20180208112949887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Spring6Application.java
```java
package com.landtool.spring6;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.support.SpringBootServletInitializer;
@SpringBootApplication
public class Spring6Application extends SpringBootServletInitializer{
    @Override
    protected SpringApplicationBuilder configure(SpringApplicationBuilder application) {
        return application.sources(Spring6Application.class);
    }
    public static void main(String[] args) {
        SpringApplication.run(Spring6Application.class, args);
    }
}
```
HelloController.java
```java
package com.landtool.spring6;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
@RestController
public class HelloController {
    @RequestMapping("hello")
    public String say(){
        return "Hello java!";
    }
}
```
4、run on tomcat 
明确地指定使用tomcat可能更好一点。 
run – edit configurations，+号 - tomcat server，然后配置。 
![这里写图片描述](https://img-blog.csdn.net/20180228182429792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
众所周知，tomcat部署是将war包或相关站点扔到tomcat/webapps/下。那这里要指定将相应的Artifacts部署到tomcat下： 
![这里写图片描述](https://img-blog.csdn.net/20180228182759615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180228182820481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
主要是编译这个Artifacts要注意： 
菜单栏file–Project Structure–Artifacts 
两种方式都可以： 
![这里写图片描述](https://img-blog.csdn.net/20180208113913410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
主要是有问号这个地方，一定要双击，选中生成的内容（有病），否则war包根本不会生成 
![这里写图片描述](https://img-blog.csdn.net/20180208114113329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然代码也要有相应修改 
![这里写图片描述](https://img-blog.csdn.net/20180208114321824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
