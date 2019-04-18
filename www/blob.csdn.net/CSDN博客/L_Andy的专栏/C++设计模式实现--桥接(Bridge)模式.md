# C++设计模式实现--桥接(Bridge)模式 - L_Andy的专栏 - CSDN博客

2014年06月18日 10:58:44[卡哥](https://me.csdn.net/L_Andy)阅读数：1860


**一. 举例**

**N年前：**

计算机最先出来时，软件和硬件是一绑在一起的，比如IBM出了一台电脑，上面有一个定制的系统，假如叫 IBM_Win，这个IBM_Win系统当然不能在HP电脑上运行，同样HP出的HP_Win系统也不能运行在IBM上。

![](https://img-my.csdn.net/uploads/201205/08/1336459191_7245.jpg)

这时如果有一家新公司Apple出了一台新电脑，那么这个公司也要开发自己的系统 Apple_Win，Apple_Linux。

其实，上面几家公司开发的软件和硬件在很多地方都有相似之处，如果每家公司都做自己的系统，这意味着资源和人员的极大浪费。

这时，无论出现新的系统，还是出现新的公司，对这种设计模式的修改都是很大的。

**N年后：**

Windows出现了，Linux出现了，软件和硬件终于分离了。这时的系统比如 Windows 既能在IBM机器上运行，也能在HP机器上运行了。

![](https://img-my.csdn.net/uploads/201205/08/1336459183_3506.jpg)

这时软件专注于软件，硬件专注于硬件。这也是推动计算机系统不断发展的原因(更加专注了嘛)，也是推动更多公司出现的原因(成本更低了嘛)。

不管是出现了新硬件平台，还是出现新的OS，大家都能专注于自己领域。也就是外界的变化很大，但对这种模式冲击却很小。

**代码实现：**


**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7546155#)[copy](http://blog.csdn.net/lwbeyond/article/details/7546155#)

- //操作系统
- class OS    
- {    
- public:    
- virtualvoid Run() {}    
- };  
- 
- //Windows
- class WindowOS: public OS    
- {  
- public:  
- void Run() { cout<<"运行 Windows"<<endl; }     
- };  
- 
- //Linux
- class LinuxOS: public OS    
- {  
- public:  
- void Run() { cout<<"运行 Linux"<<endl; }     
- };  
- 
- //计算机
- class Computer    
- {  
- public:  
- virtualvoid InstallOS(OS *os) {}    
- };  
- 
- //IBM 计算机
- class IBMComputer: public Computer    
- {  
- public:  
- void InstallOS(OS *os) { os->Run(); }    
- };  
- 
- //HP 计算机
- class HPComputer: public Computer    
- {  
- public:  
- void InstallOS(OS *os) { os->Run(); }    
- };    
- 
- 
- int main()    
- {    
-     OS *os1 = new WindowOS();  
-     OS *os2 = new LinuxOS();  
- 
-     Computer *computer1 = new IBMComputer();  
-     computer1->InstallOS(os1);  
-     computer1->InstallOS(os2);  
- 
- return 0;  
- }  

**二. 桥接模式**

定义：**将抽象部分与它的实现部分分离，使它们都可以独立地变化**。

换句话出，就是要把实现独立出来，让它们各自变化，而不会影响其它实现，这样可以降低耦合性。

为什么叫Bridge 模式，从上图中可以看出，它像一座桥，桥两边的系统相对独立，左边是抽象部分，右边是实现部分。

**三. 组合、聚合复用原则**

![](https://img-my.csdn.net/uploads/201205/08/1336459191_7245.jpg)

![](https://img-my.csdn.net/uploads/201205/08/1336459183_3506.jpg)

由上面两个种结构图的对比，我们可以引出一个原则：**尽量使用组合、聚合，尽量不要使用类继承。**

