# Red5开发第一步-Hello World - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月09日 23:39:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：39
个人分类：[Flash相关																[RTMP](https://blog.csdn.net/leixiaohua1020/article/category/8432890)](https://blog.csdn.net/leixiaohua1020/article/category/1653525)









安装好Red5服务器之后，接下来就是测试运行众人皆知的Hello World了。按照教程上一步一步来做，可惜一直没运行起来，老是NetConnection.Connect.Rejected，NetConnection.Connect.Closed，郁闷了半天，最后通过与Red5自带的例子SOSample对比，终于给运行起来了。其过程如下：

第一步：把Red5安装目录中doc/templates文件夹下的myapp文件夹copy到webapps目录下，并改名为hello（或任意名称）。

第二步：把hello目录下的log4j.properties文件删掉，修改web.xml，删除不需要的配置信息，修改后的文件内容如下图。



[![webxml](http://images.cnblogs.com/cnblogs_com/flexer/WindowsLiveWriter/Red5HelloWorld_1190/webxml_thumb.jpg)](http://images.cnblogs.com/cnblogs_com/flexer/WindowsLiveWriter/Red5HelloWorld_1190/webxml.jpg)



修改red5-web.xml，修改后的文件内容如下图。



[![red5web](http://images.cnblogs.com/cnblogs_com/flexer/WindowsLiveWriter/Red5HelloWorld_1190/red5web_thumb_1.jpg)](http://images.cnblogs.com/cnblogs_com/flexer/WindowsLiveWriter/Red5HelloWorld_1190/red5web_1.jpg)



修改red5-web.properties，将webapp.contextPath的值改为/hello。

第三步：写Java代码

打开Eclipse，建立一个Java Project，命名为hello，并将该Project地址设为我们刚才在Red5安装目录的webapps目录下新建的hello文件夹，点击Next按钮，在这个界面中点击Browse按钮，在WEB-INF中新增classes目录，将其设置为output folder，然后点击Finish。（此处要注意，output folder一定要设置在WEB-INF目录下）。建好Project后别忘了引入位于Red5安装目录下的red5.jar。Java代码如下。



```java
package com.flexer;
import org.red5.server.adapter.ApplicationAdapter;
public class Hello extends ApplicationAdapter{
    public String hello() 
    { 
        return "Hello World!"; 
    } 
}
```







然后编译，编译完成后重启Red5服务。

第四步：写Flex代码。

打开Flex Builder 3，建立Flex Project，代码如下。



```
<?xml version="1.0" encoding="utf-8"?> 
<mx:Application xmlns:mx="http://www.adobe.com/2006/mxml" layout="absolute" initialize="init()" fontSize="12"> 
    <mx:Script> 
        <![CDATA[ 
            import mx.controls.Alert; 
            private var nc:NetConnection=null; 
            private var responder:Responder=null; 
            private function init():void 
            { 
                nc=new NetConnection(); 
                nc.addEventListener(NetStatusEvent.NET_STATUS,ncHandler); 
                nc.connect("rtmp://localhost/hello"); 
            } 
            private function ncHandler(event:NetStatusEvent):void 
            { 
                if(event.info.code=="NetConnection.Connect.Success") 
                { 
                    responder=new Responder(result,error); 
                    nc.call("hello",responder); 
                } 
            } 
            private function result(obj:Object):void 
            { 
                Alert.show(obj.toString()); 
            } 
            private function error(err:Object):void 
            { 
                Alert.show(err.toString()); 
            } 
        ]]> 
    </mx:Script> 
</mx:Application>
```







完成后，编译运行。



至此，Hello World完工！



原文地址：[http://blog.csdn.net/wuguoyao/article/details/4385667](http://blog.csdn.net/wuguoyao/article/details/4385667)





