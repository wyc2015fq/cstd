# httpclient ＋protobuf 实现数据传输 - z69183787的专栏 - CSDN博客
2016年09月21日 17:07:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9148
最近项目要接入某联盟广告，采用protobuf作为传输协议，以前没弄过，这次刚好使用到了，整理下
一、环境准备：（mac下）
1.1 下载protobuf2.5安装包
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- http://pan.baidu.com/s/1o6v4Sae  
1.2 解压安装包
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- tar -zxf  protobuf-2.5.0.tar.gz  
1.3 进入软件目录
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- cd protobuf-2.5.0  
1.4 设置编译目录
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- ./configure --prefix=/User/jack/software/tools/protobuf  
-  /User/jack/software/tools/protobuf  为自己设定的编译安装目录  
1.5 安装
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- make  
- make install  
1.6 配置环境变量
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- sudo vi .bash_profile  
- export PROTOBUF=/Users/jack/software/tools/protobuf  
- export PATH=$PROTOBUF/bin:$PATH  
1.7 验证
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- jack-3:bin jack$ protoc --version  
- libprotoc 2.5.0  
二、代码编写
2.1 查看.proto文件，只贴出了部分文件内容
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- package mobads.apiv5;  
- // 本文件描述API接口版本：5.0.0  
- 
- // 版本号信息  
- message Version {  
-     optional uint32 major = 1[default = 0]; // 必填！  
-     optional uint32 minor = 2[default = 0]; // 选填！  
-     optional uint32 micro = 3[default = 0]; // 选填！  
- };  
2.2 下载 protobuf-java-2.5.0.jar
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- http://download.csdn.net/download/zinc2008/8128155  
2.3 生成.java文件
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- protoc --java_out=./ baidu_mobads_api_5.0.proto  
同样只贴出部分java源文件 ( 注意java文件不要修改哦)
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- // Generated by the protocol buffer compiler.  DO NOT EDIT!  
- // source: baidu_mobads_api_5.0.proto  
- 
- package mobads.apiv5;  
- 
- public final class BaiduMobadsApi50 {  
-   private BaiduMobadsApi50() {}  
- }  
2.4 代码编写，只贴出关键部分
**[html]**[view
 plain](http://blog.csdn.net/codetomylaw/article/details/47281311#)[copy](http://blog.csdn.net/codetomylaw/article/details/47281311#)
- MobadsRequest adrequest = MobadsRequest.newBuilder().setRequestId(requestId).setAdslot(adslot).build();  
- 
- byte[] content = adrequest.toByteArray();  
- 
- HttpClient client = new HttpClient();   
- PostMethod postMethod = new PostMethod(URL);    
- postMethod.addRequestHeader("Content-Type", "application/octet-stream;charset=utf-8");  
- postMethod.setRequestEntity(new ByteArrayRequestEntity(content ));    
- client.executeMethod(postMethod);    
- 
![](http://static.blog.csdn.net/images/save_snippets.png)
注意content-type 设置为application/octet-stream。
到此，就完成了通过httpclient的post请求传输protobuf二进制流。
使用protobuf，所有的参数和返回都是对象的形式，这点还是用着比较爽的。
String byte 切换方法
服务端返回字符串
方式一：
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/41648905#)[copy](http://blog.csdn.net/linminqin/article/details/41648905#)
- Arrays.toString(personInfo.toByteArray())  
方式二：
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/41648905#)[copy](http://blog.csdn.net/linminqin/article/details/41648905#)
- toByteString().toStringUtf8()  
客户端解析字符串
方式一：
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/41648905#)[copy](http://blog.csdn.net/linminqin/article/details/41648905#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/538458/fork)
- privatestaticbyte[] fromString(String string) {  
-     String[] strings = string.replace("[", "").replace("]", "").split(", ");  
- byte[] result = newbyte[strings.length];  
- for (int i = 0; i < result.length; i++) {  
-         result[i] = Byte.parseByte(strings[i]);  
-     }  
- return result;  
- }  
- 
- XXX.parseFrom(fromString(byteString))  
方式二：
**[java]**[view
 plain](http://blog.csdn.net/linminqin/article/details/41648905#)[copy](http://blog.csdn.net/linminqin/article/details/41648905#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/538458/fork)
- XXX.parseFrom(ByteString.copyFromUtf8(utf8ByteString))  
- 
protobuf已经出来好多年了，原谅我最近才了解到google这个高性能的用于传输的格式。
从各方面来看，它无论从序列化的性能还是从序列化的压缩比都是优于当前各种传输格式的。如json、xml、hessian，java原生的Serializable。具体对比结果参见：http://agapple.iteye.com/blog/859052
网上也看了些资料，并有所了解，看了下，这玩意儿性能上来说真的没什么可说，但初始化和使用的时候感觉不是特别方便。而且网上的例子多数都是序列化过程，没有很多关于演示网络传输的demo，于是自己简单写了一个，将完整的例子摆在这里。
先定义一个Person.proto
Java代码  ![收藏代码](http://fableking.iteye.com/images/icon_star.png)
- option java_package = "com.example.protobuf";  
- option java_outer_classname = "PersonProbuf";  
- 
- message Person {  
-   required string name = 1;  
-   required int32 id = 2;  
-   optional string email = 3;  
-   repeated PhoneNumber phone = 4;  
- 
- enum PhoneType {  
-     MOBILE = 0;  
-     HOME = 1;  
-     WORK = 2;  
-   }  
- 
-   message PhoneNumber {  
-     required string number = 1;  
-     optional PhoneType type = 2 [default = HOME];  
-   }  
- }  
 使用protoc生成对应的PersonProtobuf类
Java代码  ![收藏代码](http://fableking.iteye.com/images/icon_star.png)
- protoc.exe Person.proto --java_out=.  
请求客户端代码如下：
Java代码  ![收藏代码](http://fableking.iteye.com/images/icon_star.png)
- PersonProbuf.Person.Builder personRequest = PersonProbuf.Person.newBuilder();  
- personRequest.setId(1);  
- personRequest.setName("jesse");  
- personRequest.setEmail("xx@xx.com");  
-         personRequest.addPhone(PersonProbuf.Person.PhoneNumber.newBuilder().setNumber("1234567890").setType(PersonProbuf.Person.PhoneType.HOME));  
- 
- //使用java原生URL连接代码生成请求并获得返回值打印        
- URL url = new URL("http://localhost:90/protobuf.jsp");  
- URLConnection connection = url.openConnection();  
- connection.setDoOutput(true);  
- personRequest.build().writeTo(connection.getOutputStream());  
- 
- PersonProbuf.Person personResponse = PersonProbuf.Person.parseFrom(connection.getInputStream());  
- System.out.println(personResponse.getId());  
- System.out.println(personResponse.getName());  
- System.out.println(personResponse.getEmail());  
- System.out.println(personResponse.getPhone(0));  
- System.out.println(personResponse.getPhone(1));  
服务端protobuf.jsp代码如下：
Java代码  ![收藏代码](http://fableking.iteye.com/images/icon_star.png)
- <%@ page pageEncoding="UTF-8"%>  
- <%@pageimport="com.example.protobuf.PersonProbuf"%>  
- <%  
- PersonProbuf.Person person = PersonProbuf.Person.parseFrom(request.getInputStream());  
- System.out.println(person.getId());  
- System.out.println(person.getName());  
- System.out.println(person.getEmail());  
- System.out.println(person.getPhone(0));  
- PersonProbuf.Person.Builder personBuilder = person.newBuilder(person);  
- personBuilder.setId(2);  
- personBuilder.setName("tiger");  
- personBuilder.setEmail("yy@yy.com");  
- personBuilder.addPhone(PersonProbuf.Person.PhoneNumber.newBuilder().setNumber("0987654321").setType(PersonProbuf.Person.PhoneType.HOME));  
- personBuilder.build().writeTo(response.getOutputStream());  
- %>  
执行客户端代码后，服务端/客户端输出：
Java代码  ![收藏代码](http://fableking.iteye.com/images/icon_star.png)
- ------服务端输出------  
- 1
- jesse  
- xx@xx.com  
- number: "1234567890"
- type: HOME  
- 
- ------客户端输出------  
- 2
- tiger  
- yy@yy.com  
- number: "1234567890"
- type: HOME  
- 
- number: "0987654321"
- type: MOBILE  
所以protobuf只是一种序列化的格式，并不是传输协议，需要传输的，只要使用输入输出流，就可以做任何保存，传输的操作。
另外百度写了个jprotobuf开源，使用起来挺方便，只要使用注解放在javabean上就可以了
github：https://github.com/jhunters/jprotobuf
不过这个开源工具对集合没有适合的注解，而且也没有说明和其他语言传输的时候，如何保证一致性，所以需要使用的人考虑清楚，需要慎用。
