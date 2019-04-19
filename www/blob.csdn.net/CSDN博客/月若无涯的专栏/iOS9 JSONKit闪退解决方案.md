# iOS9 JSONKit闪退解决方案 - 月若无涯的专栏 - CSDN博客
2015年09月17日 17:46:26[月若无涯](https://me.csdn.net/u010124617)阅读数：4471
升级到iOS9之后，使用XCode7编译时，发现JSONKit不幸的又出现问题了，那么如何解决呢？
解决方案1： 
等待JSONKit更新，同时等待所有使用JSONKit的第三方SDK更新（好吧这方法是逗你玩的）
解决方案2： 
使用系统的！！ 
但是使用系统JSON解析很麻烦，项目改动又比较多，怎么办？
这时候可以试试我之前写的小库，完美兼容JSONKit的常用方法，并且采用系统JSON库实现。
YRJSONAdapter：[https://github.com/YueRuo/YRJSONAdapter](https://github.com/YueRuo/YRJSONAdapter)
使用方法非常简单： 
1.删除JSONKit 
2.引入YRJSONAdapter文件 
3.把之前的import “JSONKit.h”全部替换成import “YRJSONAdapter.h”
好了搞定了，完美切换回系统JSON，希望对大家有帮助。
2015.9.18注： 
有同学发现了，由于截止今天，部分第三方的SDK里面还在使用JSONKit，导致项目仍然有闪退情况，为了解决这个问题，那么就要使出Method Swizzling大法。
原理如下：替换常用JSONKit的category方法调用，拦截后转到可执行过的系统调用方法上。
YRJSONAdapter已经做过拦截处理，希望能解决所有JSONKit导致的闪退问题，有兴趣的可以看看源码，算是比较简单的。
另外有同学再问，JSON输出的格式不想要换行符和空格，那只需要把库中方法的
```
[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted error:nil];
```
改成
```
[NSJSONSerialization dataWithJSONObject:object options:kNilOptions error:nil];
```
目前最新的github版本我已经改成无换行符和空格的了。
