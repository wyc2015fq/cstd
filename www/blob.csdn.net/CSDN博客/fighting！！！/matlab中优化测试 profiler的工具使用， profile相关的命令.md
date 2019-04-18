# matlab中优化测试 profiler的工具使用， profile相关的命令 - fighting！！！ - CSDN博客
2018年05月23日 20:46:45[dujiahei](https://me.csdn.net/dujiahei)阅读数：215
转自：[初学笔记] matlab中优化测试 profiler的工具使用， profile相关的命令 - CSDN博客 https://blog.csdn.net/Yuinfen/article/details/78212739
这里主要是整理了一下profile的工具使用的资源和步骤，里面测试使用的是我自己的代码
### **1 profile的功能是什么
http://blog.sina.com.cn/s/blog_65d4610b0100rayl.html
用profile可解决的问题：
1、避免不必要的计算
2、改动代码避免耗时的函数
3、储存一定的结果，避免重复计算
profile用作调试工具：
1、查找出没有实际运行的代码
2、You can also view the lines that did not run to help you develop test cases
that exercise that code 
3、当M文件中有错误时，从profile中查看什么运行了，什么没有运行。
profile用于理解M文件：
1、对于一个冗长的M文件，可以先用profile看看它是怎么运行的，那些行被实际调用了。
2、当你要编制一个与现存GUI或M文件相似的程序时，可以先运行profile查看那些是与你要实现的程序相似的部分。
### **2 profile 的基本命令
profile -timer ‘cpu’ 用于计算跑程序的时候，cpu花了多少时间
profile -timer 'real' 用于计算跑程序的时候，实际花了多少时间
profile on 开启
profile clear 清空数据
profile resume 不清楚之前的数据的基础上，继续计算
profile viewer 在profiler中看结果
p = profile('info'); % 将这次计算的profier的结果保存到变量中
profsave(p,'profile_results') % 保存profile 结果,会弹出一个 profile summary的窗口
profile off 关闭
### **3 具体使用 profile的步骤
http://blog.csdn.net/shaoxiaohu1/article/details/46481123
http://developer.51cto.com/art/201104/255128_1.htm
%% 这是一个关于 profile 工具的介绍
profile on, profile clear % 先进行前面跑的程序的清空，然后开启新的profier进行计算
ellipse(4); % 这是我已有的function文件
profile viewer % 这个步骤，命令窗口会弹出 Profiler，里面的code上面可以输入，m文件或者function的名称
p = profile('info'); % 将这次计算的profier的结果保存到变量中
profsave(p,'profile_results') % 保存profile 结果,会弹出一个 profile summary的窗口，那么就是所有的计算结果
% 保存的绝对路径要自己修改，这里只是相对路径，并建立新的文件夹命名为‘profile_results’
% 最后保存的格式为 html，文件名称并没有跟随改变
