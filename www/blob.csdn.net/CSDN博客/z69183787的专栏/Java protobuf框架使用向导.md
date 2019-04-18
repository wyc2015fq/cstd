# Java protobuf框架使用向导 - z69183787的专栏 - CSDN博客
2016年09月21日 16:16:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1025
调研环境：windows
1.[http://code.google.com/p/protobuf/downloads/list](http://code.google.com/p/protobuf/downloads/list) ，[https://github.com/google/protobuf/releases?after=v3.0.0-alpha-1](https://github.com/google/protobuf/releases?after=v3.0.0-alpha-1) 选择其中的win版本下载
2.下载一个protobuf-java-2.4.1.jar文件（注意，要与你刚才下的proto.exe版本相同，否则可能出现编译通不过现象）
3.在proto.exe同级目录，编写一个msg.proto文件：
```
package Feinno.Practice.Learn;  
  
option java_package = "Feinno.Practice.Learn";  
option java_outer_classname = "ProtoBufferPractice";  
  
message msgInfo  {  
  required int32 ID = 1;  
  required int64 GoodID = 2;         
  required string Url = 3;  
  required string Guid = 4;  
  required string Type = 5;  
  required int32 Order = 6;  
}
```
4.使用如下命令编译这个文件：
protoc.exe --java_out./ msg.proto
![](http://hi.csdn.net/attachment/201107/25/0_13115797750Uno.gif)
5.将生成的ProtoBufferPractice.java文件引入eclipse
6.把下载的protobuf-java-2.4.1.jar也引入工程
7.使用方法（序列化）：
```java
ProtoBufferPractice.msgInfo.Builder builder=ProtoBufferPractice.msgInfo.newBuilder();  
    builder.setGoodID(100);  
    builder.setGuid("11111-23222-3333-444");  
    builder.setOrder(0);  
    builder.setType("ITEM");  
    builder.setID(10);  
    builder.setUrl("http://xxx.jpg");  
    ProtoBufferPractice.msgInfo info=builder.build();  
  
    byte[] result=info.toByteArray() ;
```
8.反序列化：
```java
try{  
        ProtoBufferPractice.msgInfo msg = ProtoBufferPractice.msgInfo.parseFrom(result);  
        System.out.println(msg);  
    }  
    catch(Exception ex){  
        System.out.println(ex.getMessage());  
    }
```
