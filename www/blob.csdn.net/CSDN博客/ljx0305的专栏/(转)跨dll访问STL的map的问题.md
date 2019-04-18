# (转)跨dll访问STL的map的问题  - ljx0305的专栏 - CSDN博客
2011年01月24日 15:46:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：878
## [(转)跨dll访问STL的map的问题](http://www.cppblog.com/fwxjj/archive/2009/06/16/87810.html)
原因分析：
一句话-----如果任何**STL**类使用了静态变量（无论是直接还是间接使用），那么就不要再写出跨执行单元访问它的代码。 除非你能够确定两个动态库使用 的都是同样的**STL**实现，比如都使用VC同一版本的**STL**，编译选项也一样。强烈建议，不要在动态库**接口**中传递**STL**容器！！
**STL**不一定不能在**DLL**间传递，但你必须彻底搞懂它的内部实现，并懂得为何会出问题。
微软的解释：
http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b172396
微软给的解决办法：
http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b168958
1、微软的解释：
**大部分C++标准库里提供的类直接或间接地使用了静态变量。由 于这些类是通过模板扩展而来的，因此每个可执行映像（通常是.**dll**或.exe文件）就会存在一份只属于自己的、给定类的静态数据成员。当一个需要访问这 些静态成员的类方法执行时，它使用的是“这个方法的代码当前所在的那份可执行映像”里的静态成员变量。由于两份可执行映像各自的静态数据成员并未同步，这 个行为就可能导致访问违例，或者数据看起来似乎丢失或被破坏了。**
可能不太好懂，我举个例子：假如类A<T>有个静态变量m_s，那么当1.exe使用了2.**dll**中提供的某个A<int>对象时，由于模板扩展机制，1.exe和2.**dll**中会分别存在自己的一份类静态变量A<int>.m_s。
这样，假如1.exe中从2.**dll**中取得了一个的类A<int>的实例对象a，那么当在1.exe中直接访问a.m_s时，其实访问的是 1.exe中的对应拷贝（正确情况应该是访问了2.**dll**中的a.m_s）。这样就可能导致非法访问、应当改变的数据没有改变、不应改变的数据被错误地更 改等异常情形。
原文：
**Most classes in the Standard C++ Libraries use static data members directly or indirectly. Since these classes are generated through template instantiation, each executable image (usually with **DLL** or EXE file name extensions) will contain its own copy of the static data member for a given class. When a method of the class that requires the static data member is executed, it uses the static data member in the executable image in which the method code resides. Since the static data members in the executable images are not in sync, this action could result in an access violation or data may appear to be lost or corrupted.**
1、保证资源的分配/删除操作对等并处于同一个执行单元；
   比如，可以把这些操作（包括构造/析构函数、某些容器自动扩容{这个需要特别注意}时的内存再分配等）隐藏到**接口**函数里面。换句话说：尽量不要直接从**dll**中输出**stl**对象；如果一定要输出，给它加上一层包装，然后输出这个包装**接口**而不是原始**接口**。
2、保证所有的执行单元使用同样版本的**STL**运行库。
   比如，全部使用release库或debug库，否则两个执行单元扩展出来的**STL**类的内存布局就可能会不一样。
只要记住关键就是：如果任何**STL**类使用了静态变量（无论是直接还是间接使用），那么就不要再写出跨执行单元访问它的代码。
解决方法：
1. 一个可以考虑的方案
比如有两个动态库L1和L2，L2需要修改L1中的一个map，那么我在L1中设置如下**接口**
int modify_map(int key, int new_value);
如果需要指定“某一个map”，则可以考虑实现一种类似于句柄的方式，比如可以传递一个DWORD
不过这个DWORD放的是一个地址
那么modify_map就可以这样实现：
int modify_map(DWORD map_handle, int key, int new_value)
{
    std::map<int, int>& themap = *(std::map<int, int>*)map_handle;
    themap[key] = new_value;
}
map_handle的值也首先由L1“告诉”L2:
DWORD get_map_handle();
L2可以这样调用:
DWORD h = get_map_handle();
modify_map(h, 1, 2);
2. 加入一个额外的层，就可以解决问题。所以，你需要将你的Map包装在**dll**内部，而不是让它出现在**接口**当中。动态库的**接口**越简单越好，不好去传太过复杂的东东是至理名言：） 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
我自己遇到的问题：
vector在跨越进程的时候返回出来的值是一堆乱七八糟的东西。
在一个exe中，将一个vector<wstring>的引用传递一个Dll的函数，期望DLL将vector里面的值填充后给exe使用，结果发现是一堆乱七八糟的东西，显然内存已经乱了。
**解决办法：**
**不要传vector<wstring>，改用传一个类似二维数组的结构体。**
引用:http://hi.baidu.com/coxy/blog/item/f020828eed9897e0f11f36bd.html
