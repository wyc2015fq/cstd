# Google protobuf的安装及使用 - DoubleLi - 博客园







**[cpp]**[view plain](http://blog.csdn.net/zaitianaoxiang/article/details/7639125)[copy](http://blog.csdn.net/zaitianaoxiang/article/details/7639125)



- 最近应为工作的需要，合作的部门提供了protobuf的接口，总结了一下使用的过程和方法如下：  
- 
- 
- 
- 
- 
- 下载protobuf-2.3.0：  
-     http://protobuf.googlecode.com/files/protobuf-2.3.0.zip  
- 安装：   
- unzip protobuf-2.3.0.zip  
- cd protobuf-2.3.0  
- ./configure  
- make   
- make check   
- make install  
- 
- 结果：  
- Libraries have been installed in:  
-    /usr/local/lib  
- Head files hava been installed in:  
- /usr/local/include/google/  
- protobuf/  
- 
- 
- 开始写.proto文件：  
- BaseMessage.proto：  
- message MessageBase  
- {  
-     required int32 opcode = 1;  
- // other: sendMgrId, sendId, recvMgrId, recvId, ...  
- }  
- 
- message BaseMessage  
- {  
-     required MessageBase msgbase = 1;  
- }  
- 
- BaseMessage.proto是其它消息proto文件的基础，以容器模块的C2S_GetContainerInfo为例：  
- ContainerMessage.proto：  
- import "BaseMessage.proto";  
- 
- message C2SGetContainerInfoMsg  
- {  
-     required MessageBase msgbase = 1;  
-     optional int32 containerType = 2;  
- }  
- 
- .proto文件编写规则：  
- 1）所有消息都需要包含msgbase这项，并编号都为1，即：  
-   required MessageBase msgbase = 1;  
- 2）除了msgbase这项写成required外，其它所有项都写成optional。  
- 
- 编译 .proto 文件  
- protoc -I=. --cpp_out=. BaseMessage.proto  
- protoc -I=. --cpp_out=. ContainerMessage.proto  
- 生成BaseMessage.pb.h、BaseMessage.pb.cc  
-     ContainerMessage.pb.h、ContainerMessage.pb.cc  
- 将它们添加到工程文件中。  
- 
- 编写C++代码：  
- 1）发送消息：  
- C2SGetContainerInfoMsg msg;  
- msg.mutable_msgbase()->set_opcode(C2S_GetContainerInfo);  
- msg.set_containertype(1);  
- std::string out = msg.SerializeAsString();  
- send(sockfd, out.c_str(), out.size(), 0);  
- 2）接收消息  
- char buf[MAXBUF + 1];  
- int len;  
- bzero(buf, MAXBUF + 1);  
- len = recv(new_fd, buf, MAXBUF, 0);  
- if (len > 0)  
- {  
-     printf("%d接收消息成功:'%s'，共%d个字节的数据/n",  
-             new_fd, buf, len);  
- 
-     BaseMessage baseMsg;  
-     std::string data = buf;  
-     baseMsg.ParseFromString(data);  
- 
- int opcode = baseMsg.mutable_msgbase()->opcode();  
- 
-     printf("opcode=%d/n", opcode);  
- 
- switch (opcode)  
-     {  
- case C2S_GetContainerInfo:  
-     {  
-         C2SGetContainerInfoMsg msg;  
-         msg.ParseFromString(data);  
- 
-         printf("containerType=%d/n", msg.containertype());  
- 
- break;  
-     }  
- default:  
-     {  
- break;  
-     }  
-     }  
- }  
- else  
- {  
- if (len < 0)  
-         printf("消息接收失败！错误代码是%d，错误信息是'%s'/n",  
-              errno, strerror(errno));  
-     close(new_fd);  
- return -1;  
- }  
- 
- 
- 编译C++代码：  
- Need to link lib:  
- protobuf  
- pthread  
- 
- 
- 参考：   
- 1，http://www.360doc.com/content/10/0822/16/11586_47942017.shtml  
- 2，http://code.google.com/p/protobuf/  
- 
- 



原文地址：http://blog.csdn.net/ganghust/article/details/6115283







项目主页：[http://code.google.com/p/protobuf/](http://code.google.com/p/protobuf/)

下载：[http://code.google.com/p/protobuf/downloads/list](http://code.google.com/p/protobuf/downloads/list)[protobuf-2.4.1.tar.gz](http://code.google.com/p/protobuf/downloads/detail?name=protobuf-2.4.1.tar.gz&can=2&q=)



1、./configure（注：默认可能会安装在/usr/local目录下，可以加--prefix=/usr来指定安装到/usr/lib下，可以免去路径的设置，路径设置见[Linux命令pkg-config](http://www.cnblogs.com/coderyoyo/archive/2011/08/23/Linux_pkg_config.html)）

2、make

3、make check

4、make install(需要超级用户root权限)









