# Google Protocol buffer 系列二： 常用api - 三少GG - CSDN博客
2013年11月14日 23:16:42[三少GG](https://me.csdn.net/scut1135)阅读数：2770
### [What Are Protocol Buffers?](https://developers.google.com/protocol-buffers/?hl=zh-CN)
Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data – **think XML, but smaller, faster, and simpler**. You define how you want your data to be structured once, then you can use
 special generated source code to easily write and read your structured data to and from a variety of data streams and using a variety of languages – Java, C++, or Python.
# [Protocol Buffer Basics: C++](https://developers.google.com/protocol-buffers/docs/cpptutorial?hl=zh-CN)
 a `mutable_` getter
 that lets you get a direct pointer to the string, and an extra setter. Note that you can call `mutable_email()` even
 if `email` is
 not already set; it will be initialized to an empty string automatically. If you had a singular message field in this example, it would also have a `mutable_` method
 but not a `set_` method.
**好文章一：**
[Protocol
 Buffers十分钟上手：入门示例](http://xcd.blog.techweb.com.cn/archives/173.html)
"
　　4.编译接口文件
　　调用protoc编译器进行编译：protoc x.proto –cpp_out=./oo，请注意需要指出输出文件，C++语言使用–cpp_out指出，其它语言可以查看帮助protoc –help，输出目录oo是必须事先创建好的，否则会报输出目录下存在。
　　编译成功后，将在oo目录下生成供应用调用的文件x.pb.h和x.pb.cc，编译自己代码的时候应当将x.pb.cc包含进去。
"
"
　p.SerializeToString(&str); // 将对象序列化到字符串，除此外还可以序列化到fstream等
　　printf(”%s\n”, str.c_str());
　　Person x;
　　x.ParseFromString(str); // 从字符串反序列化
　　printf(”x.name=%s\n”, [x.name](http://x.name/)().c_str()); // 这里的输出将是tom，说明反序列化正确
　　return 0;
"
**好文章二：**
**[[IBM DW] Google Protocol Buffer 的使用和原理](http://www.oschina.net/question/12_13133)**
**原理讲解！！！**
***********************************************************************************************************************************************
**如何访问、赋值 类似 vector的repeated 元素？**
Here's a snippet of my PB (detail ommited for brevity):
```
message DemandSummary
{
    required uint32 solutionIndex     = 1;
    required uint32 demandID          = 2;
}
message ComputeResponse
{
    repeated DemandSummary solutionInfo  = 3;
}
```
...and the C++ to fill up ComputeResponse::solutionInfo:
```
ComputeResponse response;
for ( int i = 0; i < demList.size(); ++i ) {
    DemandSummary* summary = response->add_solutioninfo();
    summary->set_solutionindex(solutionID);
    summary->set_demandid(demList[i].toUInt());
}
```
response.solutionInfo now contains demList.size() elements.
************************************************************************************************************
## [Protocol Buffer技术详解(C++实例)](http://www.cnblogs.com/stephen-liu74/archive/2013/01/04/2842533.html)[http://www.cnblogs.com/stephen-liu74/archive/2013/01/04/2842533.html](http://www.cnblogs.com/stephen-liu74/archive/2013/01/04/2842533.html)
**四、repeated嵌套message生成的C++代码。**
*      message BuddyInfo {          required UserInfo userInfo = 1;          required int32 groupID = 2;      }      message RetrieveBuddiesResp {          required int32 buddiesCnt = 1;          repeated BuddyInfo buddiesInfo = 2;      }*
      对于上述消息生成的代码，我们将只是针对RetrieveBuddiesResp消息所对应的C++代码进行详细说明，其余部分和前面小节的例子基本相同，可直接参照。而对于RetrieveBuddiesResp类中的代码，我们也仅仅是对buddiesInfo字段生成的代码进行更为详细的解释。
```
1     class RetrieveBuddiesResp : public ::google::protobuf::MessageLite {
 2     public:
 3         RetrieveBuddiesResp();
 4         virtual ~RetrieveBuddiesResp();
 5 
 6         ... ... //其余代码的功能性注释均可参照前面的例子。
 7             
 8         // repeated .BuddyInfo buddiesInfo = 2;
 9         static const int kBuddiesInfoFieldNumber = 2;
10         //返回数组中成员的数量。
11         inline int buddiesinfo_size() const;
12         //清空数组中的所有已初始化成员，调用该函数后，buddiesinfo_size函数将返回0。
13         inline void clear_buddiesinfo();
14         //返回数组中指定下标所包含元素的引用。
15         inline const ::BuddyInfo& buddiesinfo(int index) const;
16         //返回数组中指定下标所包含元素的指针，通过该方式可直接修改元素的值信息。
17         inline ::BuddyInfo* mutable_buddiesinfo(int index);
18         //像数组中添加一个新元素。返回值即为新增的元素，可直接对其进行初始化。
19         inline ::BuddyInfo* add_buddiesinfo();
20         //获取buddiesInfo字段所表示的容器，该函数返回的容器仅用于遍历并读取，不能直接修改。
21         inline const ::google::protobuf::RepeatedPtrField< ::BuddyInfo >&
22           buddiesinfo() const;
23         //获取buddiesInfo字段所表示的容器指针，该函数返回的容器指针可用于遍历和直接修改。
24         inline ::google::protobuf::RepeatedPtrField< ::BuddyInfo >*
25           mutable_buddiesinfo();
26     private:
27         ... ...
28     };
```
      下面是读写RetrieveBuddiesResp对象的C++测试代码和说明性注释。
```
1     void testRepeatedMessage()
 2     {
 3         printf("==================This is repeated message.================\n");
 4         RetrieveBuddiesResp retrieveResp;
 5         retrieveResp.set_buddiescnt(2);
 6         BuddyInfo* buddyInfo = retrieveResp.add_buddiesinfo();
 7         buddyInfo->set_groupid(20);
 8         UserInfo* userInfo = buddyInfo->mutable_userinfo();
 9         userInfo->set_acctid(200);
10         userInfo->set_name("user1");
11         userInfo->set_status(OFFLINE);
12     
13         buddyInfo = retrieveResp.add_buddiesinfo();
14         buddyInfo->set_groupid(21);
15         userInfo = buddyInfo->mutable_userinfo();
16         userInfo->set_acctid(201);
17         userInfo->set_name("user2");
18         userInfo->set_status(ONLINE);
19     
20         int length = retrieveResp.ByteSize();
21         char* buf = new char[length];
22         retrieveResp.SerializeToArray(buf,length);
23     
24         RetrieveBuddiesResp retrieveResp2;
25         retrieveResp2.ParseFromArray(buf,length);
26         printf("BuddiesCount = %d\n",retrieveResp2.buddiescnt());
27         printf("Repeated Size = %d\n",retrieveResp2.buddiesinfo_size());
28         //这里仅提供了通过容器迭代器的方式遍历数组元素的测试代码。
29         //事实上，通过buddiesinfo_size和buddiesinfo函数亦可循环遍历。
30         RepeatedPtrField<BuddyInfo>* buddiesInfo = retrieveResp2.mutable_buddiesinfo();
31         RepeatedPtrField<BuddyInfo>::iterator it = buddiesInfo->begin();
32         for (; it != buddiesInfo->end(); ++it) {
33             printf("BuddyInfo->groupID = %d\n", it->groupid());
34             printf("UserInfo->acctID = %I64d, UserInfo->name = %s, UserInfo->status = %d\n"
35                 , it->userinfo().acctid(), it->userinfo().name().c_str(),it->userinfo().status());
36         }
37         delete [] buf;
38     }
```
      最后需要说明的是，Protocol Buffer仍然提供了很多其它非常有用的功能，特别是针对序列化的目的地，比如文件流和网络流等。与此同时，也提供了完整的官方文档和规范的命名规则，在很多情况下，可以直接通过函数的名字便可获悉函数所完成的工作。
疑难杂症： **undefined references to `google::protobuf::GoogleOnceInitImpl**
**版本不对！原有2.4.1版本跟新安装的2.5.0有冲突！！！**
**需要调回2.4.1版本解决了！！！**
参考：[http://forums3.armagetronad.net/viewtopic.php?f=49&t=22949](http://forums3.armagetronad.net/viewtopic.php?f=49&t=22949)
 the header files (which claim the various functions exist, or the compiler would already complain)
 do not match the library file (where those functions don't exist). Have you installed protobuf from source at some earlier time by chance? 
![](https://img-blog.csdn.net/20131114233914765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGt1ZWVjc2Vy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[IBM DW] Google Protocol Buffer 的使用和原理
