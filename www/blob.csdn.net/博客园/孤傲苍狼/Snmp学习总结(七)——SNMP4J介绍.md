# Snmp学习总结(七)——SNMP4J介绍 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Snmp学习总结(七)——SNMP4J介绍](https://www.cnblogs.com/xdp-gacl/p/4187089.html)



## 一、SNMP4J介绍　　

　　SNMP4J是一个用Java来实现SNMP(简单网络管理协议)协议的开源项目.它支持以命令行的形式进行管理与响应。SNMP4J是纯面向对象设计与SNMP++(用C++实现SNMPv1/v2c/v3)相类似。

SNMP4J API 提供以下下特性：
- 支持MD5和SHA验证，DES，3DES,AES128、AES192和AES256加密的SNMPv3。
- 支持MPv1,MPv2C和MPv3，带执行的可阻塞的信息处理模块。
- 全部PDU格式。
- 可阻塞的传输拓扑。支持UPD、TCP、TLS 。
- 可阻塞的超时模块。
- 同步和异步请求。
- 命令发生器以及命令应答器的支持。
- 基于Apache license的开源免费。
- JAVA 1.4.1或更高版本(2.0或更高版本需要jdk1.6及以上的支持)。
- 基于LOG4J记录日志。
- 使用GETBULK实现Row-based的有效的异步表格获取。
- 支持多线程。

## 二、SNMP4J重要的类和接口介绍

### 2.1、Snmp类

　　Snmp类：该类是SNMP4J中最为核心的类。负责SNMP报文的接受和发送。它提供了发送和接收PDU的方法，所有的PDU类型都可以采用同步或者异步的方式被发送

### 2.2、PDU类和ScopedPDU类

　　该类是SNMP报文单元的抽象，其中PDU类适用于SNMPv1和SNMPv2c。ScopedPDU类继承于PDU类，适用于SNMPv3。

### 2.3、Target接口和CommunityTarget类以及UserTarget类

　　对应于SNMP代理的地址信息，包括IP地址和端口号（161）。其中Target接口适用于SNMPv1和SNMPv2c。CommunityTarget类实现了Target接口，用于SNMPv1和SNMPv2c这两个版本，UserTarget类实现了Target接口，适用于SNMPv3。

### 2.4、TransportMapping接口

　　该接口代表了SNMP4J所使用的传输层协议。这也是SNMP4J一大特色的地方。按照RFC的规定，SNMP是只使用UDP作为传输层协议的。而SNMP4J支持管理端和代理端使用UDP或者TCP进行传输。该接口有两个子接口。

### 2.5、Snmp、Target、PDU三者的关系

**　　Target代表远程设备或者远程实体、PDU代表管理端同Target通信的数据，Snmp就代表管理者管理功能（其实就是数据的收发）的具体执行者。**

**　　打个比方：Target就是你远方的恋人，PDU就是你们之间传递的情书、而Snmp就是负责帮你寄信收信的邮差。**

## 三、SNMP4J的两种消息发送模式

**SNMP4J支持**两种消息发送模式：**同步发送模式和异步发送模式**。

**同步发送模式也称阻塞模式**。**当管理端发送出一条消息之后，线程会被阻塞，直到收到对方的回应或者时间超时**。同步发送模式编程较为简单，但是不适用于发送广播消息。

**异步发送模式也称非阻塞模式。当程序发送一条消息之后，线程将会继续执行，当收到消息的回应的时候，程序会对消息作出相应的处理。**要实现异步发送模式，需要实例化一个实现了ResponseListener接口的类的对象。ResponseListener接口中有一个名为onResponse的函数。这是一个回调函数，当程序收到响应的时候，会自动调用该函数。由该函数完成对响应的处理。

## 四、使用SNMP4J实现管理端的步骤

　　该部分说明了利用SNMP4J编写SNMP管理端的大致过程，读者在阅读之后会对SNMP4J有一个宏观上的认识。在附录部分，作者给出了一个用SNMP4J开发管理站的样例程序，如果有进一步的需要，请参考附录部分。

### 4.1 、初始化

**①、明确SNMP在传输层所使用的协议**

　　　　一般情况下，我们都使用使用UDP协议作为SNMP的传输层协议，所以我们需要实例化的是一个DefaultUdpTransportMapping接口对象；

**②、实例化一个snmp对象**

　　　　在此过程中，我们需要将1中实例化的DefaultUdpTransportMapping接口的对象作为参数，穿snmp类的构造函数中。另外，如果实现的SNMPv3协议，我们还需要设置安全机制，添加安全用户等等；

**③、监听snmp消息**

　　　　在此，我们可以调用刚刚实例化的DefaultUdpTransportMapping的接口对象的listen方法，让程序监听snmp消息；

### 4.2、 构造发送目标

　　如果实现的是SNMPv2c或者说SNMPv1，需要实例化一个CommunityTarget对象。如果实现的是SNMPv3程序，则需要实例化一个UserTarget对象。

　　之后，我们还需要对实例化的对象做一些设置。如果是CommunityTarget的对象，则需要设置使用的Snmp版本，重传时间和等待时延。如果是UserTarget对象，我们不仅需要设置版本、重传时间、等待时延，还需要设置安全级别和安全名称。

### 4.3、 构造发送报文

　　如果发送的是SNMPv2c或者说SNMPv1的报文，我们需要实例化一个PDU类的对象。如果发送的是SNMPv3的报文，我们则需要实例化一个ScopedPDU类的对象。之后，我们还需要生成一个OID对象，其中包含了我们所需要获取的SNMP对象在MIB库中的ID。然后我们需要将OID和之前生成的PDU对象或者是ScopedPDU对象绑定，并且设置PDU的报文类型（五种SNMP报文类型之一）。

### 4.4、 构造响应监听对象（异步模式）

　　当使用异步模式的时候，我们需要实例化一个实现了ResponseListener接口的对象，作为响应消息的监听对象。在构造该对象的过程中，我们需要重写ResponseListener的OnResponse函数，该函数是一个回调函数，用来处理程序收到响应后的一些操作。

### 4.5、 发送消息

　　当所有上述操作都设置完毕之后，就可以发送消息了。同步模式和异步模式发送消息调用的函数名字均为send，但是两个函数所需参数不一样。同步模式的参数仅为4.2和4.3中构造的目标对象和报文对象，而异步模式还需要4.4中构造的监听对象。

　　同步模式发送消息后便等待响应的到达，到达之后会返回一个ResponseEvent对象，该对象中包含了响应的相应信息。

　　异步模式发送消息之后便会继续执行，当收到响应消息时便会调用监听对象的OnResponse函数。该函数中的语句便是我们对响应的处理

## 五、使用SNMP4J实现管理端的编程实现

**　　①、设定远程实体**

　　　　snmp4j中，用CommunityTarget对象来表示远程实体（要进行snmp消息通信的远程主机，使用snmp的v2版本）

**　　②、指定远程实体的地址**

　　　　snmp4j中使用Address接口对象来表示，Address对象需要通过实现该接口的类的对象向上转型来实例化

**　　③、通过CommunityTarget以及其父接口中提供的setXX方法来设定远程实体的属性**，如设定远程实体的snmp共同体属性、远程实体的地址、超时时间、重传次数、snmp版本等

**　　④、设定使用的传输协议**

　　　　snmp4j中，用TransportMapping接口的对象来表示传输协议（tcp/udp）

**　　⑤、调用TransportMapping中的listen()方法，启动监听进程，接收消息，由于该监听进程是守护进程，最后应调用close()方法来释放该进程**

**　　⑥、创建SNMP对象，用于发送请求PDU**

     　　a、创建请求pdu，即创建PDU类的对象，调用PDU类中的add()方法绑定要查询的OID，调用PDU中的setType()方法来确定该pdu的类型（与snmp中五种操作想对应）

     　　b、通过PDU的构造方法  public SNMP(TransportMapping transportingMapping),或者其他构造方法来生成pdu,之后调用 ResopnseEvent send(PDU pdu,Target target)发送pdu，该方法返回一个ResponseEvent对象

**　　⑦、通过ResponseEvent对象来获得SNMP请求的应答pdu**，方法：public PDU getResponse()

**　　⑧、通过应答pdu获得mib信息（之前绑定的OID的值）**，方法：VaribleBinding get（int index）

### 5.1、获取远程计算机的名称

```
1 package me.gacl.snmp;
  2 
  3 import java.io.IOException;
  4 import org.snmp4j.CommunityTarget;
  5 import org.snmp4j.PDU;
  6 import org.snmp4j.ScopedPDU;
  7 import org.snmp4j.Snmp;
  8 import org.snmp4j.Target;
  9 import org.snmp4j.TransportMapping;
 10 import org.snmp4j.UserTarget;
 11 import org.snmp4j.event.ResponseEvent;
 12 import org.snmp4j.event.ResponseListener;
 13 import org.snmp4j.mp.MPv3;
 14 import org.snmp4j.mp.SnmpConstants;
 15 import org.snmp4j.security.AuthMD5;
 16 import org.snmp4j.security.PrivDES;
 17 import org.snmp4j.security.SecurityLevel;
 18 import org.snmp4j.security.SecurityModels;
 19 import org.snmp4j.security.SecurityProtocols;
 20 import org.snmp4j.security.USM;
 21 import org.snmp4j.security.UsmUser;
 22 import org.snmp4j.smi.Address;
 23 import org.snmp4j.smi.GenericAddress;
 24 import org.snmp4j.smi.OID;
 25 import org.snmp4j.smi.OctetString;
 26 import org.snmp4j.smi.VariableBinding;
 27 import org.snmp4j.transport.DefaultUdpTransportMapping;
 28 
 29 public class Snmp4jFirstDemo {
 30 
 31     private Snmp snmp = null;
 32     private int version ;
 33 
 34     public Snmp4jFirstDemo(int version) {
 35         try {
 36             this.version = version;
 37             TransportMapping transport = new DefaultUdpTransportMapping();
 38             snmp = new Snmp(transport);
 39             if (version == SnmpConstants.version3) {
 40                 // 设置安全模式
 41                 USM usm = new USM(SecurityProtocols.getInstance(),new OctetString(MPv3.createLocalEngineID()), 0);
 42                 SecurityModels.getInstance().addSecurityModel(usm);
 43             }
 44             // 开始监听消息
 45             transport.listen();
 46         } catch (IOException e) {
 47             e.printStackTrace();
 48         }
 49     }
 50 
 51     public void sendMessage(Boolean syn, final Boolean bro, PDU pdu, String addr)
 52             throws IOException {
 53         // 生成目标地址对象
 54         Address targetAddress = GenericAddress.parse(addr);
 55         Target target = null;
 56         if (version == SnmpConstants.version3) {
 57             // 添加用户
 58             snmp.getUSM().addUser(new OctetString("MD5DES"),new UsmUser(new OctetString("MD5DES"), AuthMD5.ID,new OctetString("MD5DESUserAuthPassword"),PrivDES.ID, new OctetString("MD5DESUserPrivPassword")));
 59             target = new UserTarget();
 60             // 设置安全级别
 61             ((UserTarget) target).setSecurityLevel(SecurityLevel.AUTH_PRIV);
 62             ((UserTarget) target).setSecurityName(new OctetString("MD5DES"));
 63             target.setVersion(SnmpConstants.version3);
 64         } else {
 65             target = new CommunityTarget();
 66             if (version == SnmpConstants.version1) {
 67                 target.setVersion(SnmpConstants.version1);
 68                 ((CommunityTarget) target).setCommunity(new OctetString("public"));
 69             } else {
 70                 target.setVersion(SnmpConstants.version2c);
 71                 ((CommunityTarget) target).setCommunity(new OctetString("public"));
 72             }
 73 
 74         }
 75         // 目标对象相关设置
 76         target.setAddress(targetAddress);
 77         target.setRetries(5);
 78         target.setTimeout(1000);
 79 
 80         if (!syn) {
 81             // 发送报文 并且接受响应
 82             ResponseEvent response = snmp.send(pdu, target);
 83             // 处理响应
 84             System.out.println("Synchronize(同步) message(消息) from(来自) "
 85                     + response.getPeerAddress() + "\r\n"+"request(发送的请求):"
 86                     + response.getRequest() + "\r\n"+"response(返回的响应):"
 87                     + response.getResponse());
 88             /**
 89              * 输出结果：
 90              * Synchronize(同步) message(消息) from(来自) 192.168.1.233/161
 91                 request(发送的请求):GET[requestID=632977521, errorStatus=Success(0), errorIndex=0, VBS[1.3.6.1.2.1.1.5.0 = Null]]
 92                 response(返回的响应):RESPONSE[requestID=632977521, errorStatus=Success(0), errorIndex=0, VBS[1.3.6.1.2.1.1.5.0 = WIN-667H6TS3U37]]
 93 
 94              */
 95         } else {
 96             // 设置监听对象
 97             ResponseListener listener = new ResponseListener() {
 98 
 99                 public void onResponse(ResponseEvent event) {
100                     if (bro.equals(false)) {
101                         ((Snmp) event.getSource()).cancel(event.getRequest(),this);
102                     }
103                     // 处理响应
104                     PDU request = event.getRequest();
105                     PDU response = event.getResponse();
106                     System.out.println("Asynchronise(异步) message(消息) from(来自) "
107                             + event.getPeerAddress() + "\r\n"+"request(发送的请求):" + request
108                             + "\r\n"+"response(返回的响应):" + response);
109                 }
110 
111             };
112             // 发送报文
113             snmp.send(pdu, target, null, listener);
114         }
115     }
116 
117     public static void main(String[] args) {
118         //Snmp的三个版本号
119         //int ver3 = SnmpConstants.version3;
120         int ver2c = SnmpConstants.version2c;
121         //int ver1 = SnmpConstants.version1;
122         Snmp4jFirstDemo manager = new Snmp4jFirstDemo(ver2c);
123         // 构造报文
124         PDU pdu = new PDU();
125          //PDU pdu = new ScopedPDU();
126         // 设置要获取的对象ID，这个OID代表远程计算机的名称
127         OID oids = new OID("1.3.6.1.2.1.1.5.0");
128         pdu.add(new VariableBinding(oids));
129         // 设置报文类型
130         pdu.setType(PDU.GET);
131         //((ScopedPDU) pdu).setContextName(new OctetString("priv"));
132         try {
133             // 发送消息 其中最后一个是想要发送的目标地址
134             //manager.sendMessage(false, true, pdu, "udp:192.168.1.229/161");//192.168.1.229 Linux服务器
135             manager.sendMessage(false, true, pdu, "udp:192.168.1.233/161");//192.168.1.233 WinServer2008服务器
136         } catch (IOException e) {
137             e.printStackTrace();
138         }
139     }
140 }
```

### 5.2、获得本机的信息

```
1 package me.gacl.snmp;
 2 
 3 import java.io.IOException;
 4 import org.snmp4j.CommunityTarget;
 5 import org.snmp4j.PDU;
 6 import org.snmp4j.Snmp;
 7 import org.snmp4j.TransportMapping;
 8 import org.snmp4j.event.ResponseEvent;
 9 import org.snmp4j.mp.SnmpConstants;
10 import org.snmp4j.smi.Address;
11 import org.snmp4j.smi.GenericAddress;
12 import org.snmp4j.smi.OID;
13 import org.snmp4j.smi.OctetString;
14 import org.snmp4j.smi.VariableBinding;
15 import org.snmp4j.transport.DefaultUdpTransportMapping;
16 
17 /**
18  * <p>ClassName: GetOID<p>
19  * <p>Description:获得本机的信息 <p>
20  * @author xudp
21  * @version 1.0 V
22  * @createTime 2014-9-15 下午04:45:12
23  */
24 public class GetOID {
25 
26     public static void main(String[] args) throws Exception{  
27         
28         try{  
29             //设定CommunityTarget   
30             CommunityTarget myTarget = new CommunityTarget();
31             //定义远程主机的地址  
32             //Address deviceAdd = GenericAddress.parse("udp:192.168.1.233/161"); 
33             //定义本机的地址   
34             Address localAdd = GenericAddress.parse("udp:localhost/161");
35             //设定远程主机的地址 
36             //myTarget.setAddress(deviceAdd); 
37             //设定本地主机的地址
38             myTarget.setAddress(localAdd);
39             //设置snmp共同体   
40             myTarget.setCommunity(new OctetString("public")); 
41             //设置超时重试次数   
42             myTarget.setRetries(2); 
43             //设置超时的时间  
44             myTarget.setTimeout(5*60);
45             //设置使用的snmp版本 
46             myTarget.setVersion(SnmpConstants.version2c);  
47              
48             //设定采取的协议   
49             TransportMapping transport = new DefaultUdpTransportMapping();//设定传输协议为UDP
50             //调用TransportMapping中的listen()方法，启动监听进程，接收消息，由于该监听进程是守护进程，最后应调用close()方法来释放该进程
51             transport.listen();  
52             //创建SNMP对象，用于发送请求PDU
53             Snmp protocol = new Snmp(transport);  
54             //创建请求pdu,获取mib   
55             PDU request = new PDU(); 
56             //调用的add方法绑定要查询的OID
57             request.add(new VariableBinding(new OID("1.3.6.1.2.1.1.1")));  
58             request.add(new VariableBinding(new OID(new int[] {1,3,6,1,2,1,1,2})));
59             //调用setType()方法来确定该pdu的类型
60             request.setType(PDU.GETNEXT);
61             //调用 send(PDU pdu,Target target)发送pdu，返回一个ResponseEvent对象
62             ResponseEvent responseEvent = protocol.send(request, myTarget);  
63             //通过ResponseEvent对象来获得SNMP请求的应答pdu，方法：public PDU getResponse()
64             PDU response=responseEvent.getResponse();  
65             //输出   
66             if(response != null){  
67                 System.out.println("request.size()="+request.size());  
68                 System.out.println("response.size()="+response.size());
69                 //通过应答pdu获得mib信息（之前绑定的OID的值），方法：VaribleBinding get(int index)
70                 VariableBinding vb1 = response.get(0);  
71                 VariableBinding vb2 = response.get(1);  
72                 System.out.println(vb1);  
73                 System.out.println(vb2);
74                 //调用close()方法释放该进程
75                 transport.close();  
76                 
77                 /**
78                  * 输出结果：
79                  * request.size()=2
80                    response.size()=2
81                     1.3.6.1.2.1.1.1.0 = Hardware: x86 Family 6 Model 58 Stepping 9 AT/AT COMPATIBLE - Software: Windows 2000 Version 5.1 (Build 2600 Multiprocessor Free)
82                     1.3.6.1.2.1.1.2.0 = 1.3.6.1.4.1.311.1.1.3.1.1
83 
84                  */
85             }  
86             
87           }catch(IOException e){  
88               e.printStackTrace();  
89           }  
90         }  
91 }
```

　　以上就是关于SNMP4J的简单介绍和入门，后面的博文中将进行更加详细的介绍。









