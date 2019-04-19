# 全球有多少 C/C++ 程序员？Java 和 Python 又有多少？ - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [黄利民](http://www.jobbole.com/members/huanglimin) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
【伯乐在线导读】：[2015年4月15日 JetBrains 发布了 CLion，一个跨平台的 C/C++ IDE](http://blog.jobbole.com/86032/)。现在快要出 1.2 版了。在发表 CLion 1.0 后，收到了很多反馈。结果引发他们讨论 C/C++ 的市场。比如： C/C++ 程序员有多少？他们分布在哪些地方？他们在用哪些工具？
正好他们有个能做出些结果令人振奋的研究团队。于是就给该团队分派了任务，调研 C/C++ 的市场情况。最后研究团队给出了一份答卷，伯乐在线摘编如下：
### 1. C/C++ 程序员的数量
全球大约 440 万 C++ 程序员；大约 190 万 C 程序员。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1exf0n8ig05j20er0e30vr.jpg)
### 2. C++ 比 Python 的程序员数量高一些，C 和 Ruby 的程序员数量相当
JetBrains 估算了全球使用热门编程语言的开发者数量：
- Java 有 900 万；
- C# 大约 780 万；
- JavaScript 大约 760 万；
- PHP 将近 600 万；
- C++ 大约 440 万；
- Python 有 400 万；
- Objective-C 大约 330 万；
- C 大约 190 万；
- Ruby 大约 180 万；
![](http://ww2.sinaimg.cn/large/7cc829d3gw1exf0nfdw7aj20eq0ifwh0.jpg)
### 3. C++ 程序员在全球的分布
- EMEA 地区有 150 万；*（伯乐在线注：EMEA 是 Europe, the Middle East and Africa 的字母缩写，为欧洲、中东、非洲三地区的合称，通常是用作政府行政或商业上的区域划分方式，这种用法较常见于北美洲的企业。）*
- 亚太地区有 150 万；
- 北美地区有 100 万；
- 拉丁美洲约 40 万；
![](http://ww4.sinaimg.cn/large/7cc829d3gw1exf0ngbxvfj20ev0gpdix.jpg)
### 4. C++ 更为流行的国家
相比其他语言，C++ 在俄罗斯、捷克、匈牙利、德国、法国、芬兰、以色列和新加坡更流行。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1exf0ngu0blj20f00gmtbo.jpg)
### 5. 哪些行业在用 C++ 语言
分析 C++ 的招聘广告信息得出的结果：前三的行业是：**金融、银行和游戏**。接下来是：Front Office、通信、电子、投行、市场、制造业、零售业。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1exf0nognrgj20er0fdjsp.jpg)
*【伯乐在线补充】：Front Office 是相对于 Back Office 而言。所谓 Front Office 软件即指应用在销售部门、客户服务、电话中心(call center)、和技术支援方面的软件，Front Office软件的目的在提升企业的销售、行销、与客户服务的效率。*
### 6. 哪些编程语言和 C++ 一起在用
根据分析的招聘广告信息，经常和 C++ 一起在用的语言有：Java、C、C#、Python、SQL 和 JS。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1exf0nox272j20ek0bu76d.jpg)
### 7. C++ 程序员所用的操作系统
Linux 44%、Windows 39%、OS X 17%；
![](http://ww3.sinaimg.cn/large/7cc829d3gw1exf0nqndw5j20eq0ezabn.jpg)
### 8. C++ 的版本
已经有 34% 的 C++ 程序员在用 C++11；
![](http://ww4.sinaimg.cn/large/7cc829d3gw1exf0npw458j20eq0c7wfj.jpg)
### 9. 最流行的 C++ 编译器
GCC 遥遥领先，65% 份额；
![](http://ww4.sinaimg.cn/large/7cc829d3gw1exf0nqs1yyj20et0eamyg.jpg)
### 10. Windows 平台上最流行的 C++ 编译器
Windows 平台上，VC++ 虽然是第一（36%），但只比 GCC 领先 2%。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1exf0nvabopj20eg0gndhv.jpg)
> 
插播一条简讯：微软在11月释出的Visual C++更新将整合[Clang开源C和C++编译器](http://www.theregister.co.uk/2015/10/21/microsoft_promises_clang_for_windows_in_november_visual_c_update/)，开发者将可以用Clang编译Windows应用程序。Visual Studio 2015 已经整合了Clang编译器，但它是被用于Android和 iOS上的应用开发，而这一次整合是用之开发Windows程序。
Visual C++由多部分构成，其中包括前端编译器，用于生成代表程序内容的抽象语法树；后端编译器则读取抽象语法树生成执行代码。前端编译器称为C1，后端称为 C2，由微软的不同团队开发。最新的整合将用Clang取代C1，后端继续用C2。微软推出Clang/C2 for Windows的目的是改进跨平台支持。（中文摘要来自[Solidot](http://www.solidot.org/story?sid=45898)）
### 11. 最流行的 C++ 调试器
如果不划分开发平台，GDB7 占据了 62%，甩了其他调试器很多条街了。不过在 OS X 平台上的数据就完全不同了，LLDB 是 39% 领先，GDB7 32% 落后。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1exf0nvjnrhj20et0flq50.jpg)
### 12. 最流行的 C++ 构建系统
CMake 和 GNU Make 夺走了 68% C++ 的心；
![](http://ww3.sinaimg.cn/large/7cc829d3gw1exf0o2rju9j20em0fi760.jpg)
### 13. 编译器 + 调试器 + 构建工具组合
GCC + CMake + GDB7 组合是 C++ 程序员的最爱。不过在 OS X 平台，这个荣誉要让给 Clang + CMake + LLDB。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1exf0o3iec5j20ee0giwgd.jpg)
### JetBrains 此次调查的数据来源有：
![](http://ww2.sinaimg.cn/large/7cc829d3gw1exf0o47ygoj20du0ar3z5.jpg)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/ad6d90a05c268f775d371bbea374e1561.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/fa163d96822aabcfda67f64fc702685f1.jpg)
