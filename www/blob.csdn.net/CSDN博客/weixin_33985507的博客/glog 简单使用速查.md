# glog 简单使用速查 - weixin_33985507的博客 - CSDN博客
2013年01月11日 17:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
debug模式：是否定义了**NDEBUG**宏，没有定义就是debug模式，可以使用如下打印调试信息：
DLOG(INFO) << "Found cookies"; (最好使用DLOG（WARNING）)
