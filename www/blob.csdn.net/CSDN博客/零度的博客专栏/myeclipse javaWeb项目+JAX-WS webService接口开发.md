# myeclipse javaWeb项目+JAX-WS webService接口开发 - 零度的博客专栏 - CSDN博客
2016年05月09日 09:16:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：881
使用myeclipse javaWeb项目开发webService
这里我使用的是myeclipse8.6,webService架包我使用的是myeclipse集成的JAX-WS
一,Service服务端
首先创建一个[Java Web](http://lib.csdn.net/base/13)项目,建好包名,建一个普通类,然后写入以下内容:
package com.shich.webservice;
public class FristService {
public String sayHello(String name){
System.out.println(name+"在调用sayHello...");
if(name==null || "".equals(name)){
return "请输入name.";
}
return "hello,"+name;
}
}
点击工具栏new web service,创建一个web service
![](https://img-blog.csdn.net/20140929121201532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后弹出
![](https://img-blog.csdn.net/20140929121142765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择JAX-WS,下面选择第二个单选按钮,next
![](https://img-blog.csdn.net/20140929121505800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Finsh 后就可以看到项目中多出了一些东西 ,wsdl文件
![](https://img-blog.csdn.net/20140929121559183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在jax-ws架包即可,具体是哪些架包我也不知道,就将所有jax-ws架包导入吧
整个项目的结构如下:
![](https://img-blog.csdn.net/20140929123004673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后部署项目运行,在浏览器上输入地址
http://localhost:8088/testService/FristServicePort?wsdl
就会看到刚才发布的Service
![](https://img-blog.csdn.net/20140929123033502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
二,测试Service客户端
新建一个项目,创建好包名
然后点击,注意这里选择的是New web Service Client
![](https://img-blog.csdn.net/20140929121201532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后弹出
![](https://img-blog.csdn.net/20140929123125856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
next
![](https://img-blog.csdn.net/20140929123440622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
next-->next-->finsh
然后你会发现项目中刚创建的包中多出一些类
测试Service客户端整体结构
![](https://img-blog.csdn.net/20140929123950798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后创建一个测试类,测试整个接口结果
package test;
import com.shich.client.FristServiceDelegate;
import com.shich.client.FristServiceService;
public class TestClient {
public static void main(String[] args) {
FristServiceDelegate frist=new FristServiceService().getFristServicePort();
String result=frist.sayHello("123");
System.out.println(result);
}
}
运行这个测试类,你会看到客服端控制台输出以下结果:
![](https://img-blog.csdn.net/20140929124326733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
webservice服务端控制台会输出以下结果:
![](https://img-blog.csdn.net/20140929124808828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3NoZW5nX3BhcGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好了,到这里整个webservice服务端,客服端就写完了.
