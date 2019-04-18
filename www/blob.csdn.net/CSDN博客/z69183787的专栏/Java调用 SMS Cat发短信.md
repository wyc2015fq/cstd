# Java调用 SMS Cat发短信 - z69183787的专栏 - CSDN博客
2014年07月30日 16:54:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2704

居然不能发表SMS Cat的中文名?!
这个是 SMS Cat设备
![](http://dl.iteye.com/upload/attachment/0075/9986/ae31d131-dc06-30b3-a457-b4b66f51e932.png)
需要插入SIM卡，所以，是需要付短信费的。
插好USB和电源后，会安装好驱动，可以在设备管理可以查看到端口号
![](http://dl.iteye.com/upload/attachment/0075/9992/fb42a77b-03c2-3779-ad85-b553e062edcd.png)
也有一种软件可以测试端口是否可连接：secureCRT
![](http://dl.iteye.com/upload/attachment/0076/0009/1542d309-d90f-38ba-8758-1a92cbc90b00.png)
新建好connection后，输入AT测试，如果OK就OK。
![](http://dl.iteye.com/upload/attachment/0076/0013/6ad76c59-63a9-331b-90ad-257a6a847282.png)
好了，端口确定没问题了。
接着，在你的JDK的bin路径下放一个win32com.dll
![](http://dl.iteye.com/upload/attachment/0075/9990/4fa501d5-4462-3aa3-8fc7-aa6571d9f84d.png)
在JDK的lib中放一个comm.jar和javax.comm.properties
![](http://dl.iteye.com/upload/attachment/0076/0033/d8f64f5c-5028-3801-9884-4283b7a3205e.png)
需要的文件都在附件SMSCat.rar中。
OK，所有都准备完了，现在开始代码测试。
demo测试需要用到的jar
![](http://dl.iteye.com/upload/attachment/0076/0025/d7cbda17-00e5-356d-827a-2abe78ae6c1d.png)
sms.properties#sms properties
Java代码  ![收藏代码](http://royal2xiaose.iteye.com/images/icon_star.png)
- Message.comId=modem.com19  
- Message.com=COM19  
- Message.baudRate=9600
- Message.manufacturer=wavecom  
- Message.model=  
- Message.simPin=0000
注意：这里的端口COM19必须和你上面的端口对应。
SMSService.java
Java代码  ![收藏代码](http://royal2xiaose.iteye.com/images/icon_star.png)
- /**
-  * Copyright(C) 2012 GZ ISCAS ALL Rights Reserved
-  */
- package com.royal.SMSCat;  
- 
- import java.util.Properties;  
- 
- import org.smslib.Message.MessageEncodings;  
- import org.smslib.OutboundMessage;  
- import org.smslib.Service;  
- import org.smslib.modem.SerialModemGateway;  
- 
- import com.royal.utils.PropertiesUtil;  
- 
- /**
-  * 描述：SMS Cat服务类
-  */
- publicclass SMSService {  
- 
- /**
-      * 私有静态实例
-      */
- privatestatic SMSService instance = null;  
- 
- /**
-      * 是否开启服务
-      */
- privateboolean isStartService = false;  
- 
- /**
-      * 私有构造方法
-      */
- private SMSService() {  
-     }  
- 
- /**
-      * 获取实例（单例模式）
-      * 
-      * @return
-      */
- publicstatic SMSService getInstance() {  
- if (instance == null) {  
-             instance = new SMSService();  
-         }  
- return instance;  
-     }  
- 
- /**
-      * 开启短信服务
-      * 
-      * @param path
-      *            配置文件路径
-      */
- publicvoid startService(String path) {  
-         System.out.println("开始初始化SMS服务！");  
- 
- // 加载文件属性
-         Properties p = null;  
- try {  
-             p = PropertiesUtil.getProperties(path);  
-         } catch (Exception e) {  
-             System.out.println("加载属性文件出错：" + e.getMessage());  
- return;  
-         }  
- 
- // 初始化网关，参数信息依次为：COMID,COM号,比特率,制造商,Modem模式
-         SerialModemGateway gateway = new SerialModemGateway(p.getProperty("Message.comId"), p.getProperty("Message.com"), Integer.parseInt(p.getProperty("Message.baudRate")), p.getProperty("Message.manufacturer"), p.getProperty("Message.model"));  
- 
-         gateway.setInbound(true);  
-         gateway.setOutbound(true);  
-         gateway.setSimPin(p.getProperty("Message.simPin"));  
- 
-         OutboundNotification outboundNotification = new OutboundNotification();  
- 
-         Service service = Service.getInstance();  
- if (service == null) {  
-             System.out.println("初始化SMS服务失败！");  
- return;  
-         }  
- 
-         service.setOutboundMessageNotification(outboundNotification);  
- try {  
-             service.addGateway(gateway);  
- // 开启服务
-             service.startService();  
-             System.out.println("初始化SMS服务成功！");  
-             isStartService = true;  
-         } catch (Exception e) {  
-             System.out.println("开启SMS服务异常:" + e.getMessage());  
-         }   
-     }  
- 
- /**
-      * 停止SMS服务
-      */
- publicvoid stopService() {  
- try {  
-             Service.getInstance().stopService();  
-         } catch (Exception e) {  
-             System.out.println("关闭SMS服务异常:" + e.getMessage());  
-         }   
-         isStartService = false;  
-     }  
- 
- /**
-      * 发送短信
-      * 
-      * @param toNumber
-      *            手机号码
-      * @param message
-      *            短信内容
-      */
- publicvoid sendMessage(String toNumber, String message) {  
- if (!isStartService) {  
-             System.out.println("尚未开启SMS服务！");  
- return;  
-         }  
- 
- // 封装信息
-         OutboundMessage msg = new OutboundMessage(toNumber, message);  
-         msg.setEncoding(MessageEncodings.ENCUCS2);  
- try {  
- // 发送信息
-             Service.getInstance().sendMessage(msg);  
-         } catch (Exception e) {  
-             System.out.println("SMS服务发送信息发生异常:" + e.getMessage());  
-             isStartService = false;  
-         }   
-     }  
- 
- }  
OutboundNotification.java
Java代码  ![收藏代码](http://royal2xiaose.iteye.com/images/icon_star.png)
- package com.royal.SMSCat;  
- 
- import org.smslib.AGateway;  
- import org.smslib.IOutboundMessageNotification;  
- import org.smslib.OutboundMessage;  
- 
- /**
-  * 封装发送短信类
-  */
- publicclass OutboundNotification implements IOutboundMessageNotification {  
- 
- publicvoid process(AGateway gateway, OutboundMessage msg) {  
-         System.out.println("Outbound handler called from Gateway: " + gateway.getGatewayId());  
-     }  
- }  
SMSCatClient.java
Java代码  ![收藏代码](http://royal2xiaose.iteye.com/images/icon_star.png)
- package com.royal.SMSCat;  
- 
- publicclass SMSCatClient {  
- 
- /**
-      * 测试
-      * 
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         String path = "D:\\sms.properties";  
-         SMSService.getInstance().startService(path);  
-         SMSService.getInstance().sendMessage("13800138000", "测试 Test！");  
- //没必要的时候没停止服务，因为端口占用着
-         SMSService.getInstance().stopService();  
-     }  
- 
- }  
测试结果自己找个手机号测吧
![](http://dl.iteye.com/upload/attachment/0076/0047/bd6898ae-139f-368b-a337-144908cb7412.png)
看见了吗？控制台中的服务（红色标识）还在跑着，也就是端口还在占用着；服务没断，可以不用重新初始化。
- [SMSCat.rar](http://dl.iteye.com/topics/download/90a11e30-934a-3ef4-a377-73f8ea5330ef) (1.3 MB)
- 下载次数: 3
- [查看图片附件](http://royal2xiaose.iteye.com/blog/1714582#)
- 
