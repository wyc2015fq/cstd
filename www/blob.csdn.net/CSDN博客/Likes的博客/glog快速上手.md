# glog快速上手 - Likes的博客 - CSDN博客
2018年10月12日 15:16:52[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：17标签：[glog																[入门																[上手																[快速																[教程](https://so.csdn.net/so/search/s.do?q=教程&t=blog)](https://so.csdn.net/so/search/s.do?q=快速&t=blog)](https://so.csdn.net/so/search/s.do?q=上手&t=blog)](https://so.csdn.net/so/search/s.do?q=入门&t=blog)](https://so.csdn.net/so/search/s.do?q=glog&t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
### 1、安装
```bash
git clone https://github.com/google/glog
cd glog
autogen.sh
./configure
make -j
make install
```
### 2、使用
- 代码
- 
```cpp
#include <glog/logging.h>
   int main(int argc, char* argv[]) {
     // Initialize Google's logging library.
     google::InitGoogleLogging("myLog");//set log name
     // ...
     LOG(INFO) << "Found " << num_cookies << " cookies";
   }
```
编译
`g++ *.cc -lglog -lpthread`- 输出
         默认将日志输出到/tmp目录下
### 3、修改日志输出目录
         在初始化日志前先设定好输出目录：FLAGS_log_dir ="./log"  (log目录需要自己创建)
```cpp
FLAGS_log_dir ="./log";
google::InitGoogleLogging("myLog");
```
### 4、四种信息类型
```cpp
google::INFO = 0,
　　google::WARNING = 1,
　　google::ERROR = 2,
　　google::FATAL = 3,
```
### 5、Debug 模式
        LOG<<  正常的日志
        DLOG<<  debug模式下才有的日志
### 6、参考
[https://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html](https://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html)
[https://github.com/google/glog/blob/master/doc/glog.html](https://github.com/google/glog/blob/master/doc/glog.html)
