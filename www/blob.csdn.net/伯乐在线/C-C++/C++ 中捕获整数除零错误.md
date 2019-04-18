# C++ 中捕获整数除零错误 - 文章 - 伯乐在线
原文出处： [Bit Focus](http://blog.bitfoc.us/?p=100)
继承自 C 的优良传统, C++ 也是一门非常靠近底层的语言, 可是实在是太靠近了, 很多问题语言本身没有提供解决方案, 可执行代码贴近机器, 运行时没有虚拟机来反馈错误, 跑着跑着就毫无征兆地崩溃了, 简直比过山车还刺激.
虽然 C++ 加入了异常机制来处理很多运行时错误, 但是异常机制的功效非常受限, 很多错误还没办法用原生异常手段捕捉, 比如整数除 0 错误. 下面这段代码


```
#include <iostream>
int main()
{
    try {
        int x, y;
        std::cin >> x >> y;
        std::cout << x / y << std::endl;
    } catch (...) {
        std::cerr << "attempt to divide integer by 0." << std::endl;
    }
    return 0;
}
```
输入 “1 0” 则会导致程序挂掉, 而那对 try-catch 还呆在那里好像什么事情都没发生一样. 像 Python 一类有虚拟机环境支持的语言, 都会毫无悬念地捕获除 0 错误.
**使用信号**
不过, 底层自然有底层的办法, 而且有虚拟机的环境也并非在每个整数除法指令之前都添上一句 if 0 == divisor: raise 之类的挫语句来触发异常. 这得益于硬件体系中的中断机制. 简而言之, 当发生整数除 0 之类的错误时, 硬件会触发中断, 这时操作系统会根据上下文查出是哪个进程不给力了, 然后给这个进程发出一个信号. 某些时候也可以手动给进程发信号, 比如恼怒的用户发现某个程序卡死的时候果断 kill 掉这个进程, 这也是信号的一种.
这次就不是 C 标准了, 而是 POSIX 标准. 它规定了哪些信号进程不处理也不会有太大问题, 有些信号进程想处理也是不行的, 还有一些信号是错误中断, 如果程序处理了它们, 那么程序能继续执行, 否则直接杀掉.
不过, 这些错误处理默认过程都是不存在的, 需要通过调用 signal 函数配置. 方法类似下面这个例子


```
#include <csignal>
#include <cstdlib>
#include <iostream>
void handle_div_0(int)
{
    std::cerr << "attempt to divide integer by 0." << std::endl;
    exit(1);
}
int main()
{
    if (SIG_ERR == signal(SIGFPE, handle_div_0)) {
        std::cerr << "fail to setup handler." << std::endl;
        return 1;
    }
    int x, y;
    std::cin >> x >> y;
    std::cout << x / y << std::endl;
    return 0;
}
```
可以看出, signal 接受两个参数, 分别是信号编号和信号处理函数. 成功设置了针对 SIGFPE (吐槽: 为什么是浮点异常 FPE 呢?) 的处理函数 handle_div_0, 如果再发生整数除 0 的惨剧, handle_div_0 就会被调用.
handle_div_0 的参数是信号码, 也就是 SIGFPE, 忽略它也行.
**底层机制**
虽然说 handle_div_0 是异常处理过程, 但毕竟是函数都会有调用栈, 能返回. 假如在 handle_div_0 中不调用 exit 自寻死路, 而是选择返回, 那么程序会怎么样呢? 运行一下, 当出现错误时, stderr 会死循环般地刷屏.
实际上, 当错误发生时, 操作系统会在当前错误出现处加载信号处理函数的调用栈帧, 并且把它的返回地址设置为出错的那条指令之前, 这样看起来就像是出错之前的瞬间调用了信号处理函数. 当信号处理函数返回时, 则又会再次执行那条会出错的指令, 除非信号处理函数能通过某些特别的技巧修复指令, 否则退出时会重蹈覆辙.
上面提到的 “修复指令” 指的是修复 CPU 级别的指令码或者操作数. 把除数 y 变成全局变量, 然后在 handle_div_0 中设置 y 为 1, 这样做是于事无补的.
**使用异常处理机制**
修复指令这种事情简直是天方夜谭, 所以选择输出一跳错误语句并退出也算是不错的方法. 在 C 语言时代, 还可以通过 setjmp 和 longjmp 来跳转程序流程. 不过 setjmp 和 longjmp 操作起来太不方便了, 相比之下 try-catch 要好得多.
刚才说过, 错误处理函数的调用栈帧直接位于错误发生处所在函数栈帧之上, 因此, 抛出异常能够被外部设置的 try-catch 捕获. 现在定义一个异常类型, 然后在 handle_div_0 中抛出就行.


```
#include <csignal>
#include <iostream>
struct div_0_exception {};
void handle_div_0(int)
{
    throw div_0_exception();
}
int main()
{
    if (SIG_ERR == signal(SIGFPE, handle_div_0)) {
        std::cerr << "fail to setup handler." << std::endl;
        return 1;
    }
    try {
        int x, y;
        std::cin >> x >> y;
        std::cout << x / y << std::endl;
    } catch (div_0_exception) {
        std::cerr << "attempt to divide integer by 0." << std::endl;
    }
    return 0;
}
```
**更精准的信号处理**
上述方法的缺陷在于, 只要发生 SIGFPE 中断, 无论是整数除 0 错误, 还是其它浮点异常, 处理方式是统一的. 不过, POSIX 还规定了一组更精细的信号处理接口, 它们是 sigaction.
呃… 对它们都是 sigaction. 这又是一个雷死人的东西. 在 csignal 中定义了两个同名的东西, 分别是


```
struct sigaction;
int sigaction(int sig
            , struct sigaction const* restrict act
            , struct sigaction* restrict old_act);
```
前面那个结构体在设置信号处理函数时用到, 里面存放了一些标志位和信号处理函数指针. 而后面那个函数就是设置信号处理的入口 (如果函数的第三个参数并非 NULL, 并且之前设置过信号处理结构体, 那么会将之前的处理方法写入第三个参数所指向的结构中, 这一点并不需要, 所以后面的例子中这个参数直接传入 NULL, 详情请见 man 3 sigaction).
结构 sigaction 中会有两个函数入口地址, 它们分别是


```
void (* sa_handler)(int);
void (* sa_sigaction)(int, siginfo_t*, void*);
```
sa_handler 也就是之前所演示的轻便型信号处理函数; 而 sa_sigaction, 从它接受的参数就能看出, 它能获得更多的上下文信息 (然而, 一看第三个参数的类型是 void* 就知道没有好事, 信息都在第二个参数指向的结构体中).
既然有两个处理函数, 那么如何决定使用哪一个呢? 在 struct sigaction 中有一个标志位成员 sa_flags, 如果为它置上 SA_SIGINFO 位, 那么就使用 sa_sigaction 作为处理函数.
siginfo_t 类型中有一个叫做 si_code 的成员, 它为信号类型提供进一步的细分, 比如在 SIGFPE 信号下, si_code 可能有 FPE_INTOVF (整数溢出), FPE_FLTUND (浮点数下溢), FPE_FLTOVF (浮点数上溢) 等各种相关取值, 当然还有现在最关心的整数除 0 信号码 FPE_INTDIV. 如果陷入 SIGFPE 的窘境中, 而 si_code 又恰好是 FPE_INTDIV 那么就要果断抛出 0 异常了.
由于原生的 struct sigaction 居然跟函数重名, 所以下面的例子中会对其包装一下, 提供合适的初始化过程.


```
#include <csignal>
#include <cstring>
#include <iostream>
struct my_sig_action {
    typedef void (* handler_type)(int, siginfo_t*, void*);
    explicit my_sig_action(handler_type handler)
    {
        memset(&_sa, 0, sizeof(struct sigaction));
        _sa.sa_sigaction = handler;
        _sa.sa_flags = SA_SIGINFO;
    }
    operator struct sigaction const*() const
    {
        return &_sa;
    }
protected:
    struct sigaction _sa;
};
struct div_0_exception {};
void handle_div_0(int sig, siginfo_t* info, void*)
{
    if (FPE_INTDIV == info->si_code)
        throw div_0_exception();
}
int main()
{
    my_sig_action sa(handle_div_0);
    if (0 != sigaction(SIGFPE, sa, NULL)) {
        std::cerr << "fail to setup handler." << std::endl;
        return 1;
    }
    try {
        int x, y;
        std::cin >> x >> y;
        std::cout << x / y << std::endl;
    } catch (div_0_exception) {
        std::cerr << "attempt to divide integer by 0." << std::endl;
    }
    return 0;
}
```
