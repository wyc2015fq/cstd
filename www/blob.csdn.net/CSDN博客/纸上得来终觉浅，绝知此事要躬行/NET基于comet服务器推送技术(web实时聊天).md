# .NET基于comet服务器推送技术(web实时聊天) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月26日 16:43:40[boonya](https://me.csdn.net/boonya)阅读数：6926








原文地址：[http://www.cnblogs.com/zengqinglei/archive/2013/03/31/2991189.html](http://www.cnblogs.com/zengqinglei/archive/2013/03/31/2991189.html)

[](http://www.cnblogs.com/zengqinglei/archive/2013/03/31/2991189.html)
作者：[曾庆雷](http://www.cnblogs.com/zengqinglei/)
出处：[http://www.cnblogs.com/zengqinglei](http://www.cnblogs.com/zengqinglei/)
Comet 也称反向 Ajax 或服务器端推技术.其思想很简单：将数据直接从服务器推到浏览器，而不必等到浏览器请求数据。



主要思想：服务器端将数据推送到客户端(浏览器)

本人做了简单的web实时聊天系统：[服务器推送(聊天).zip](http://files.cnblogs.com/zengqinglei/%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%8E%A8%E9%80%81%28%E8%81%8A%E5%A4%A9%29.zip)

系统简单说明如下：

{

系统所用数据库：sqlite数据库

　　UserInfo：用户信息表

　　UserRelation:用户关系表

　　MessageInfo：聊天信息表

开发结构：

　　结构图：

![](http://images.cnitblog.com/blog/451346/201303/31001200-58be99828e7748d68769d6122267dfd7.png)

　　三层开发：

　　　　数据层--与sqlite数据库之间的交互

　　　　逻辑层--数据转换

　　　　实体层--数据对象

　　Web服务：利用WebService对外提供公共的接口，主要提供客户端与服务器端数据之间的交互。


　　客户端(asp.net)：利用comet中ajax反向技术推送聊天信息。

项目操作演示效果：

　　1.用户登录(测试账户{liger_zql,12345},{漠然,12345})

　　说明：用浏览器开启两个选项卡浏览Login.aspx页面分别如下

![](http://images.cnitblog.com/blog/451346/201303/30235720-a6299acc415d4a4986d937b6664cc88e.png)

![](http://images.cnitblog.com/blog/451346/201303/30235943-4ad28c606b1a4ef084c16072e608c3ff.png)

　　2.聊天界面

　　登录成功后，两选项卡分别跳转到如下界面：双击在线好友中的行，则可开始进行聊天。

　　liger_zql聊天界面：

![](http://images.cnitblog.com/blog/451346/201303/31000322-6e53369502a6486aba0e0f3052a72eaa.png)

　　漠然聊天界面：

![](http://images.cnitblog.com/blog/451346/201303/31000456-09b4ec7861ae4e27b8cf258e1850dbf7.png)

　　其中聊天过程是使用了comet推送技术。

　　3.用户注册界面

![](http://images.cnitblog.com/blog/451346/201303/31001837-4b8893ee6c2c436e84ce836355fccadc.png)

}

项目中comet推送技术

　　客户端

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

//ajax长轮询
function PostSubmit(params, success) {
    $.post("comet_broadcast.asyn", params,
    success, "json");
}
function Keepline() {
    var array = new PHPArray("Keepline");
    var success = function (data, status) {
        if (data.ResponseStatus == 1) {
            ShowMessage(data.ResponseData, "recive");
        }
        Keepline();
    }
    PostSubmit(array.ToJson(), success);
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

　　服务器端：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

　　　　#region 发送消息
        private void SendMsg()
        {
            MessageInfo message = new MessageInfo()
            {
                SendUserId = m_Context.Request["UserId"],
                ReciveUserId = m_Context.Request["ReciveUserId"],
                Content = m_Context.Request["Content"]
            };
            //获取服务器端处理结果
            string result = sdk.Send_Msg(message);
            if (dict.ContainsKey(message.ReciveUserId))
            {
                //向对发送信息
                dict[message.ReciveUserId].Result = result;
                dict[message.ReciveUserId].Send();
            }
            //向自己发送处理结果
            _IAsyncResult.Result = result;
            _IAsyncResult.Send();
        }
        #endregion

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

其中(项目网站--WebChat_ServerPush和服务器端处理--ServerPushHttpHandler)需要在Web.config配置文件中做出如下配置：

```
<httpHandlers>
      <!--comet长连接配置字节-->
      <add verb="*" path="comet_broadcast.asyn" type="ServerPushHttpHandler.ServerPush,ServerPushHttpHandler"/>
    </httpHandlers>
```

Comet是一个web应用模型，在该模型中，请求被发送到服务器端并保持一个很长的存活期，直到超时或是有服务器端事件发生。在该请求完成后，另一个长生存期的Ajax请求就被送去等待另一个服务器端事件。使用Comet的话，web服务器就可以在无需显式请求的情况下向客户端发送数据。

Comet的一大优点是，每个客户端始终都有一个向服务器端打开的通信链路。服务器端可以通过在事件到来时立即提交（完成）响应来把事件推给客户端，或者它甚至可以累积再连续发送。因为请求长时间保持打开的状态，故服务器端需要特别的功能来处理所有的这些长生存期请求。



