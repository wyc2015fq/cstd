# 纯 C# 实现 Hook 功能 - 文章 - 伯乐在线
原文出处： [zhao.yj](http://www.cnblogs.com/zhyj/p/5533657.html)
发布一个自己写的用于Hook .Net方法的类库，代码量不大，完全的C#代码实现，是一个比较有趣的功能，分享出来希望能和大家共同探讨
安装：Install-Package DotNetDetour
源码：[https://github.com/bigbaldy1128/DotNetDetour](https://github.com/bigbaldy1128/DotNetDetour)
**1.为何想做这个**
说到hook大家都应该不陌生，就是改变函数的执行流程，让本应该执行的函数跑到另一个函数中执行，这是个很有用也很有趣的功能（例如获取函数参数信息，改变函数执行流程，计算函数执行时间等等），杀软中主防的原理就是hook，通过hook拦截函数获取参数信息来判断是否是危险行为，但这类程序大多是C++的，一直以来我都想实现可以hook .net函数的库，网上搜索了很多，但都不理想，所以想自己实现一个
**2.实现原理**
我采用的是inline hook的方式，因为我对.net虚拟机以及一些内部的结构并不是很熟悉，并且有些东西的确找不到任何文档，所以就采用原生代码的inline hook的方式来实现。
首先说一下inline hook的基本原理，它是通过修改函数的前5字节指令为jmp xxxxxxxx来实现的，例如一个C#方法：
![](http://jbcdn2.b0.upaiyun.com/2016/06/6d7e456427635b5a958cb27a24ddf38a.png)
用windbg调试查看方法信息：
![](http://jbcdn2.b0.upaiyun.com/2016/06/9ee6418a8da9793ed96d8892558aa177.png)
查看已经jit了的原生代码:
![](http://jbcdn2.b0.upaiyun.com/2016/06/52e2f2e6b9b11accc89344b8facffe4f.png)
这里的地址(0x008c0640)可以通过MethodInfo.MethodHandle.GetFunctionPointer().ToPointer()方法获取
到了这里，我们就知道了修改从push ebp开始的5个字节为jmp跳转指令，跳入我们自己的函数就可以达到hook的目的，但执行到我们的函数后，如果我们并不是要拦截执行流程，那么我们最终是需要再调用原函数的，但原函数已经被修改了，这会想到的办法就是恢复那修改的5字节指令。
但这又会引发另一个问题，就是当我们恢复时，正好另一个线程调用到这个函数，那么程序将会崩溃，或者说漏掉一次函数调用，修改时暂停其他线程并等待正跑在其中的CPU执行完这5字节再去恢复指令也许是个不错的办法，但感觉并不容易实现，而且影响性能，所以我放弃了这种办法
那么如何才能调用修改前的函数呢，我首先想到是C中写裸函数的方式，即自己用汇编拼出来一个原函数再执行：
原函数前5字节指令+jmp跳转指令
但其实这也是不可行的，聪明的人已经发现，图中所示的函数的前5字节并不是一个完整的汇编指令，不同的函数，长度都不一样，.net的函数并不像某些原生函数那样，会预留mov edi,edi这样的正好5字节的指令，我先想到的是复制函数的所有汇编指令生成新的函数，但这样也会出问题，因为像E8,E9这样的相对跳转指令，如果指令地址变了，那么跳转的位置也就变了，程序就会崩溃，所以这也不可行。
到了这里，我有些不耐烦了，毕竟我是要hook所有函数的，而不是某个固定的函数，而函数入口的指令又不相同，这可怎么办，难道我需要计算出大于等于5字节的最小完整汇编指令长度？
按照这个思路，最终找到了一个用C写的反汇编库（BlackBone），其中提供了类似的方法，我稍作了修改后试用了下，的确不错，可以准确求出汇编指令长度，例如
push ebp
mov ebp,esp
mov eax,dword ptr ds:[33F22ACh]
求出值是9，这样我根据求出的值动态拼接一个函数出来即可，哈哈，到了这里，感觉实现的差不多了，但没想到64位下又给了我当头一棒，之前的原函数指令可以写成：
大于等于5字节的最小完整汇编指令+jmp跳转指令即可构成我们的原函数
但我们知道，C#中要想执行汇编，是需要用Marshal.AllocHGlobal来分配非托管空间的，而这样分配的地址与我们要跳转到的原函数的地址在64位下是超过2GB地址范围的，一般的跳转指令是无法实现的，所以想到了用ret指令实现，而64位地址又不能直接push，所以最后写出如下汇编：
push rax
mov rax,target_addr
push rax
mov rax,qword ptr ss:[rsp+8]
ret 8
由于某些C#函数竟然第一行就是修改rax寄存器的值，所以只能是先保存rax，推入堆栈后再恢复，这里汇编操作就方便多了，之前实现另一个东西，用到IL指令，但发现只有dup这种复制栈顶元素的指令，却没有获取堆栈中某个非栈顶元素值的指令，所以说还是汇编灵活啊，想怎么写就怎么写，啥都能实现。
最后就是这个原函数的调用过程了，因为是动态拼接的函数，所以想到的就是用Marshal.GetDelegateForFunctionPointer转成委托来执行，后来发现不对，因为我虽然拼接的是汇编，而这个汇编是C#方法jit后的汇编，这个并不是C方法编译后的汇编，通过把非托管指针转换为委托的方式运行函数是会添加很多不需要的操作的
例如托管类型与非托管类型的转换，但我拼接出的函数是不需要这些过程的，这个怎么办，看来只能用调用C#普通函数的方式调用，这个怎么实现呢，其实很好办，只需写一个空壳函数，然后修改这个函数的方法表中的原生指令指针即可，具体方法如下：


C#
```
*((ulong*)((uint*)method.MethodHandle.Value.ToPointer() + 2)) = (ulong)ptr;
```
method是空壳函数的MethodInfo，         ptr是动态拼接的原函数的地址
好，到了这里就基本完成核心功能了，最不好处理的就是这个原函数调用，我的完整的64位原函数指令拼接就实现了，代码很少，如下所示：


C#
```
byte[] jmp_inst =
{
    0x50,                                              //push rax
    0x48,0xB8,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90, //mov rax,target_addr
    0x50,                                              //push rax
    0x48,0x8B,0x44,0x24,0x08,                          //mov rax,qword ptr ss:[rsp+8]
    0xC2,0x08,0x00                                     //ret 8
};
 
protected override void CreateOriginalMethod(MethodInfo method)
{
    uint oldProtect;
    var needSize = NativeAPI.SizeofMin5Byte(srcPtr);
    byte[] src_instr = new byte[needSize];
    for (int i = 0; i < needSize; i++)
    {
        src_instr[i] = srcPtr[i];
    }
    fixed (byte* p = &jmp_inst[3])
    {
        *((ulong*)p) = (ulong)(srcPtr + needSize);
    }
    var totalLength = src_instr.Length + jmp_inst.Length;
    IntPtr ptr = Marshal.AllocHGlobal(totalLength);
    Marshal.Copy(src_instr, 0, ptr, src_instr.Length);
    Marshal.Copy(jmp_inst, 0, ptr + src_instr.Length, jmp_inst.Length);
    NativeAPI.VirtualProtect(ptr, (uint)totalLength, Protection.PAGE_EXECUTE_READWRITE, out oldProtect);
    RuntimeHelpers.PrepareMethod(method.MethodHandle);
    *((ulong*)((uint*)method.MethodHandle.Value.ToPointer() + 2)) = (ulong)ptr;
}
```
**3.类库开发所用到的语言**
之前我说，我的这个库是完全用C#实现的，但其中的确用到了一个C写的反汇编库，于是我用C#把那个库重写了一遍，说来也简单，C的代码粘过来，C#启用unsafe代码，改了10分钟就好了，真心是非常方便，毕竟C#是支持指针和结构体的，而且基础类型非常丰富，这里得给C#点个赞！
**4.具体使用**
使用非常简单，首先新建控制台程序并添加一个类，继承接口IMethodMonitor，Get是你自己的函数，Ori是原函数会在运行时动态生成，在Get中你可以干你想干的任何事情


C#
```
public class CustomMonitor : IMethodMonitor //自定义一个类并继承IMethodMonitor接口
{
    [Monitor("TargetNamespace", "TargetClass")] //你要hook的目标方法的名称空间，类名
    public string Get() //方法签名要与目标方法一致
    {
        return "B" + Ori();
    }
 
    [MethodImpl(MethodImplOptions.NoInlining)]
    [Original] //原函数标记
    public string Ori() //方法签名要与目标方法一致
    {
        return null; //这里写什么无所谓，能编译过即可
    }
}
```
然后定义目标函数，例如


C#
```
public string Get()
 {
    return "A";
 }
```
最后调用Monitor.Install()安装监视器,例如：


C#
```
Console.WrtieLine(Get());
Monitor.Install()
Console.WrtieLine(Get());
```
你会发现第一次调用Get输出的值是”A”,第二次是”BA”
当然这个库只是hook，但hook一般都需要dll注入来配合，因为hook自身进程没什么意义，hook别人的进程才有意义，我之后会发布一个用于.net程序远程注入的类库，注入的是.net的dll哦，不是C++的
好了，讲了这么多，其实这个库代码量并不大，但主要是自己研究的一个成果，很多东西都是自己琢磨出来的，所以觉得这个过程很有意思，也希望高手能指出改进方案，毕竟感觉目前这种方法虽然实现了功能，但是并不是很好，总觉得以hook .net虚拟机的方式来实现会更简单一些，或者网络上已经有了现成的解决方案我没有找到，总之，抛砖引玉，希望大家能共同探讨
