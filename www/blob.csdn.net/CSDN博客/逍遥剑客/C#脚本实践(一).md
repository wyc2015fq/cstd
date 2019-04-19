# C#脚本实践(一) - 逍遥剑客 - CSDN博客
2010年11月27日 02:12:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：15386
个人对脚本语言认识不深, 尤其是动态语言...
但是问了好多人为什么逻辑用脚本写, 答案都是不用编译
这也只能怪DT的C++当初没设计好
由于WOW的成功, 让lua几乎成了游戏脚本的标准
原来的python/ruby/tcl啥的一下子就没落了, 都怪暴雪啊~
虽然lua做为一个脚本语言来说, 非常的完美
但是我对它不是很感冒, 可能习惯了C系语言的语法了
对于C/C++/C#/Java/JS/AS类的我才看着比较顺眼
而且, 从学校开始, 好像自己就中了C#的毒一般, 虽然一直没机会大量应用, 但心中一直念念不忘
这也导致了我把引擎工具用WPF来开发的念头
今年Unity这个名字在眼前和耳边不断地飘过, 其理念非常不错
而它的脚本是基于Mono的, 冒似实际效果很不错
这也让我有了使用C#做为脚本的冲动
如果嫌C#不是动态语言的话, Mono还支持JavaScript和Boo
本来想如果不跨平台的话是不是可以直接使用MS的.Net实现
Google一下到是真有个叫CS-Script的实现, 不过想嵌入NativeC++看来不是那么好整
所以, 一切还是要从Mono下手, 虽然它的运行效率比MS的要打个折扣, 但再怎么说也是lua的好多倍
Mono在Windows下的编译不是很顺利, 因为官方的压缩包里的sln压根文件就不全
搜索也没什么头绪, 看来大家都是在linux下搞这玩意
后来从github上下载了最新的代码, 好不容易在修改了部分路径下的情况下编译通过
其中有些.lib是在mono的windows安装版本中的
Embedded Samples就是嵌入mono的示例程序
以teste为例, 运行时需要依赖mscorlib.dll, 而且那个路径很奇怪, 竟然是mono/msvc/Win32_Debug/lib/mono/2.0/
这个之后再搞懂怎么配置
test.cs载入之前要先编译成test.exe, 然后才能载入运行
这个可以看成是脚本的预编译, 怎么直接载入文本执行呢? 这个待研究
P/Invoke方式的调用显然不实用, 要是能这样用还不如直接MS的.Net
另外一种脚本调用底层的方法是internal call, 大体看了下好像必须要在C#中再做一次extern声明
这个看看能不能做自动生成的, 要不然每次改动两边都要改多不爽
C++调脚本是通过Mono提供的一系列API来完成的, 如mono_runtime_invoke
另外有个类似luabind的库, 叫monobind, 可以参考一下
![](http://hi.csdn.net/attachment/201011/26/0_1290795232sfmS.gif)
