# Flex调用Servlet - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月05日 14:16:32[boonya](https://me.csdn.net/boonya)阅读数：1769









如果您把 Flex 中 mxml 和 Action Script 的关系理解为 Html 和 JavaScript 的关系，您会忽然发现您对 Flex 变的如此熟悉！


Action Script 语言是面向对象的脚本语言，它连编写方式都和 JavaScript 非常的相似。除了可以嵌套在 mxml 里面之外，它还可以像 JavaScript 写在单独的 .js 文件里面那样写在单独的 .as 文件里面，然后在 mxml 里面引入它。


下面我们新建一个 Action Script 的类 J2eeServer.as 如清单 2 所示：

##### 清单 2：J2eeServer.as

package com.ibm.flex
{
    import flash.events.EventDispatcher;
    import mx.rpc.AsyncToken;
    import mx.rpc.events.FaultEvent;
    import mx.rpc.events.ResultEvent;
    import mx.rpc.http.HTTPService;
    
    public class J2eeServer extends EventDispatcher
    {
        public function J2eeServer()
        {
        }

        public function sendRequest(locale:String):void
        {
            var httpObject:HTTPService = new HTTPService();
            httpObject.resultFormat = "text";
            httpObject.url = 
                "http://localhost:8080/FlexSample/SampleServlet?locale="+locale;
            var responder:mx.rpc.Responder = new mx.rpc.Responder(onSuccess, onFault);
            var call:AsyncToken = httpObject.send();
            call.addResponder(responder);
        }
        private function onSuccess(event:ResultEvent):void
        {
            this.dispatchEvent(event);   
        }
        
		private function onFault(event:FaultEvent):void
        {
            trace("communication failed!");
            this.dispatchEvent(event); 
        } 
    }
}


在这个类定义里面，熟悉 Java 的开发人员可以看出其编码规范和 Java 非常类似。在其中我们定义了一个 sendRequest() 方法， 使用 HTTPService 对象发起一个 http 的 get 请求 , 并且对于不同的返回结果我们定义了 onSuccess() 和 onFault() 两个方法去处理。在这两个结果处理方法中，我们将事件 dispatch 出去。

### 与 Servlet 集成


在 J2eeServer.as 中，我们通过 Action Script 发出 http 请求，现在我们定义一个服务器端的 servlet 来处理该请求。如清单 3 所示：

##### 清单 3：SampleServlet.java

package com.ibm.sample;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class SampleServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;

    public SampleServlet() {
        super();
    }

    protected void doGet(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        doPost(request, response);
    }

    protected void doPost(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        String locale = request.getParameter("locale");
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        out.println("The locale you selected is " + locale);
    }
}


和普通 web 应用一样配置 web.xml, 如清单 4

##### 清单 4：web.xml

<?xml version="1.0" encoding="UTF-8"?>
<web-app id="WebApp_ID" version="2.4" xmlns="http://java.sun.com/xml/ns/j2ee" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee 
        http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
    <display-name>FlexSample</display-name>
    <servlet>
        <display-name>SampleServlet</display-name>
        <servlet-name>SampleServlet</servlet-name>
        <servlet-class>com.ibm.sample.SampleServlet</servlet-class>
    </servlet>
    <servlet-mapping>
        <servlet-name>SampleServlet</servlet-name>
        <url-pattern>/SampleServlet</url-pattern>
    </servlet-mapping>
    <welcome-file-list>
        <welcome-file>FlexSample.html</welcome-file>
    </welcome-file-list>
</web-app>

### 事件驱动与数据绑定


从 J2eeServer.as 中我们依稀可以看出 Flex 事件驱动的影子。的确，Flex 的业务流程大多是靠事件来驱动的。某一个方法结束后，dispatch 出去一个事件。事件的监听者监听到这个事件后就会采取相应的动作来处理事件。如清单 5 所示：我们在 init() 方法中进行了事件绑定，绑定了两个监听方法 successHandler () 和 errorHandler () 到 J2eeServer 上。当 J2eeServer 完成和服务器端 servlet 通信 dispatch 出相应的事件后，两个监听方法便会监听到相应的事件并进行处理。


Flex 的另外一个重要的特点就是支持数据绑定，在另一篇文章[《使用 Eclipse
 和 JavaFX 开发 Mashup 应用》](http://www.ibm.com/developerworks/cn/edu/j-dw-java-jfx-mashup.html)中我们曾介绍了 JavaFx 的数据绑定特性，同样 Flex 也支持数据绑定。当数据源被绑定到另外一个对象的某属性上时，数据源的数据如果发生变化，开发人员不需要写额外的代码该对象的属性值就可以相应的变化。如清单 5 中所示：如果要进行数据绑定，则要先定义一个可绑定的数据源，例如我们在变量`private`
```
`var`
```
serverResponse:String
 =
```
`""``;`的上面设置
 [Bindable] 属性，表示其为可绑定变量。然后我们在 resultLable 中使用花括号 {`serverResponse`}
 将变量`serverResponse`绑定到 resultLable 的 text
 属性上。
```


##### `清单 5：FlexSample.mxml`


```
<?xml version="1.0" encoding="utf-8"?>
<mx:Application xmlns:mx="http://www.adobe.com/2006/mxml" layout="absolute"
     creationComplete="init()">
     <mx:Script>
         <![CDATA[
             import mx.rpc.events.ResultEvent;
             import mx.rpc.events.FaultEvent;
             import com.ibm.flex.J2eeServer;
             import mx.collections.ArrayCollection;
             
            private var jserver:J2eeServer = new J2eeServer();
            //可绑定变量
            [Bindable]
            private var serverResponse:String = "";
            //可绑定变量
            [Bindable]
            private var locales:ArrayCollection = new ArrayCollection(
                                                      [ {label:"en_us", data:"en_us"}, 
                                                         {label:"zh_cn", data:"zh_cn"}]); 
             
             private function init():void
             {
                 //事件绑定
                jserver.addEventListener(ResultEvent.RESULT, successHandler);
                jserver.addEventListener(FaultEvent.FAULT, errorHandler);
             }
             private function localeComboxChangeHandler(event:Event):void
             {
                 jserver.sendRequest(localeCombox.selectedItem.data);
             }
             
             private function successHandler(event:ResultEvent):void
             {
                 serverResponse = event.result.toString();
                 trace(event.result);
             }
             private function errorHandler(event:FaultEvent):void
             {
                 serverResponse = event.message.toString();
                 trace(event.message);
             }
         ]]>
     </mx:Script>
     <mx:VBox>
        <mx:Label text="Select your locale:" />
        <mx:ComboBox 
		    id="localeCombox" 
		    width="160" 
		    dataProvider="{locales}" 
		    change="localeComboxChangeHandler(event)"/>
        <mx:Label 
		    id="resultLabel"
		    text="The response from server is: {serverResponse}" />        
     </mx:VBox>
</mx:Application>
```


文章出处：[http://www.ibm.com/developerworks/cn/web/wa-lo-flexdev/](http://www.ibm.com/developerworks/cn/web/wa-lo-flexdev/)



