# Java数据通讯中使用Google Protobuf实现序列化与反序列化 - 零度的博客专栏 - CSDN博客
2017年01月11日 10:05:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：3335
### 一、.什么是protocol buffer
       ProtocolBuffer是用于结构化数据串行化的灵活、高效、自动的方法，有如XML，不过它更小、更快、也更简单。你可以定义自己的数据结构，然后使用代码生成器生成的代码来读写这个数据结构。你甚至可以在无需重新部署程序的情况下更新数据结构。
      l  Google定义的一种序列化的协议格式；
      l  Google内部几乎所有的RPC调用及文件格式；
（据称当前google已经定义了12，183个.proto文件，共有48，162种不同的message类型。它们用于RPC系统或各种存储系统中进行数据的存储）
l  目标：
Ø  简单性
Ø  兼容性
Ø  高性能
### 二、如何使用
 1. google官方连接地址  [http://code.google.com/p/protobuf/downloads/list](http://code.google.com/p/protobuf/downloads/list)
2. 选择proto.exe压缩包protoc-2.4-win32.zip和protobuf-java-2.4.1.jar压缩包protobuf-2.4.zip
3.下载一个protobuf-java-2.4.1.jar文件（注意，要与你刚才下的proto.exe版本相同，否则可能出现编译通不过现象）
4.在proto.exe同级目录，编写一个netbean.proto文件,为需要序列化的数据接口加入一个message属性，为每一个字段指定名称和类型，如下所示（message 的名称不能与java_outer_classname 名称相同，不然编译不过 ）:
package com.leehongee.netserver.net.bean;
option java_package = "com.leehongee.netserver.net.bean";
option java_outer_classname = "Request";
message request_Net {
 required string cmd = 1;
 optional int32 keycode = 2;      
 optional sint32 posX = 3;
 optional sint32 posY = 4;
 optional bool onOrOff = 5;
 optional bool ontouch = 6;
}
备注：限定修饰符 required ,optional ,repeated
required: 表示是一个必须字段，必须相对于发送方，在发送消息之前必须设置该字段的值，对于接收方，必须能够识别该字段的意思。
optional：表示是一个可选字段，可选对于发送方，在发送消息时，可以有选择性的设置或者不设置该字段的值。
repeated：表示该字段可以包含0~N个元素。其特性和optional一样，但是每一次可以包含多个值。
5.使用CMD命令编译这个netbean.proto文件:
D:\protoc-2.4.1-win32>protoc.exe  --java_out=./  netbean.proto
6.在目录com\leehongee\netserver\net\bean 目录下将生成一个Request.java源文件，并将其引入到Myeclipse中
7.把jar包protobuf-java-2.4.1.jar也引入到工程libs目录中
8.使用方法调用序列化：
Request .request_Net .Builder builder=Request .request_Net ..newBuilder();
builder.setCmd("ontouch");
Request .request_Net  request=builder.builder();
byte[] buffer=request.toByteAarray();
9.使用反序列化：
try{
    Request .request_Net  request     = Request .request_Net .parseFrom(buffer);
   }
   catch(Exception ex){
       System.out.println(ex.getMessage());
   }
### 三、具体实例实现
1、编写 test.proto文件：
**[plain]**[view
 plain](http://blog.csdn.net/qyf_5445/article/details/43793067#)[copy](http://blog.csdn.net/qyf_5445/article/details/43793067#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/604078/fork)
- option java_package = "com.proto";   
- option java_outer_classname = "FirstProtobuf";   
- message TestBuf  {   
-   required int32 ID = 1;   
-   required string Url = 2;   
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
2、利用protoc.exe 生成java文件：
**[plain]**[view
 plain](http://blog.csdn.net/qyf_5445/article/details/43793067#)[copy](http://blog.csdn.net/qyf_5445/article/details/43793067#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/604078/fork)
- protoc.exe --java_out=./ test.proto  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
3、编写测试类：
**[java]**[view
 plain](http://blog.csdn.net/qyf_5445/article/details/43793067#)[copy](http://blog.csdn.net/qyf_5445/article/details/43793067#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/604078/fork)
- package com.test;  
- 
- import java.io.IOException;  
- import com.proto.FirstProtobuf;  
- import com.proto.FirstProtobuf.TestBuf;  
- 
- publicclass TestProtoBuf {  
- 
- publicstaticvoid main(String[] args) throws IOException {  
- //序列化过程 
- //FirstProtobuf是生成类的名字，即proto文件中的java_outer_classname 
- //testBuf是里面某个序列的名字，即proto文件中的message testBuf 
- 
-         FirstProtobuf.TestBuf.Builder builder = FirstProtobuf.TestBuf.newBuilder();  
- 
-         builder.setID( 123 );  
-         builder.setUrl("http://www.21cn.com");  
- 
-         TestBuf info = builder.build();  
- byte[] buf = info.toByteArray();  
- 
-         TestBuf tb = TestBuf.parseFrom(buf);  
-         System.out.println( tb.getID());  
-         System.out.println( tb.getUrl());  
-     }  
- }  
