# IDA反汇编工具初探 - xqhrs232的专栏 - CSDN博客
2013年10月29日 15:39:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1263
个人分类：[PC/Windows/Office技术](https://blog.csdn.net/xqhrs232/article/category/906929)
原文地址::[http://blog.csdn.net/pankun/article/details/11841](http://blog.csdn.net/pankun/article/details/11841)
相关文章
1、源码恢复反汇编静态分析工具IDA pro V5.5.0.925 汉化破解版----[http://www.cr173.com/soft/14313.html](http://www.cr173.com/soft/14313.html)
2、[反汇编stepldr.exe——第一次使用IDA反汇编](http://blog.csdn.net/gooogleman/article/details/3760731)----[http://blog.csdn.net/gooogleman/article/details/3760731](http://blog.csdn.net/gooogleman/article/details/3760731)
3、[ARM指令机器码学习——反汇编必学](http://blog.csdn.net/gooogleman/article/details/3758555)----[http://blog.csdn.net/gooogleman/article/details/3758555](http://blog.csdn.net/gooogleman/article/details/3758555)

看过<<笑傲江湖>>的朋友一定知道吸星大法吧,这是一种可以吸取他人内功来使自己功力增长的神功,(段誉的北冥神功也是这样, 扯远了...).对于程序员来说,增长自己编程功力的一个好方法是阅读其它人开发的程序的源码,从而把别人的技术来消化成为自己知识,这是不是很象吸星大法?
但开源的程序毕竟是在少数,大多数程序都只会分发可执行文件及相关文件,这时我们要想查看此程序的代码,就只有把它反汇编,当然这需要一定的汇编功底,但是一个好的反汇编工具能为你阅读反汇编出来的程序提供非常大的帮助.
了解反汇编的朋友也一定知道WINDASM这个有名的反汇编工具,比如我们用WINDASM反汇编一个程序,在其程序入口点反汇编得到如下代码:
//*********************** Program Entry Point *****************
:00401000 6A00                 push 00000000
:00401002 E8FF050000      call 00401606
:00401007 A316304000      mov [00403016], eax
:00401007 E8EF050000      call 00401600
:00401011 A30E304000      mov [0040300E], eax
:00401016 6A0A                 push 0000000A
:00401018 FF350E304000   push dword ptr [0040300E]
:0040101E 6A00                 push 00000000
:00401020 EF3516304000   push dword ptr [00403016]
:00401026 E806000000       call 00401031
:0040102B 50                     push eax
:0040102c E8C9050000      call 004015FA
如果不联系上下文及知道这是程序入口的话,很难看出来这一段代码到底是干什么的,但IDA就不一样了,它不但会反汇编程序,并会尽量分析程序,并加上相应的注释(正因为这样,IDA反汇编一个大的程序会花非常长的时间),请看下面一段IDA反汇编出来的代码,是不是明了多了?
.text:00401000                 push     0               ; lpModuleName
.text:00401002                 call      GetModuleHandleA
.text:00401007                 mov     hInstance, eax
.text:0040100C                call      GetCommandLineA
.text:00401011                 mov    dword_0_40300E, eax
.text:00401016                 push    0Ah
.text:00401018                 push    dword_0_40300E
.text:0040101E                 push    0
.text:00401020                 push    hInstance
.text:00401026                 call      sub_0_401031
.text:0040102B                 push    eax             ; uExitCode
.text:0040102C                 call      ExitProcess
IDA反汇编程序后,会生成一个 .idb文件,里面保存了反汇编出来的代码及注释及IDA的一些其它相关数据,我们可以直接在IDA中写自己的分析结果和注释并保存,下次直接打开.idb文件就可以了,例如上面
.text:00401000                 push    0               ; lpModuleName
.text:00401002                 call      GetModuleHandleA
.text:00401007                 mov     hInstance, eax
我们可以看出来实际上就是hInstance = GetModuleHandleA(nil);我们可以在后面直接加上注释,在.text:00401007这一行最后面的空白处点右键,在弹出的菜单中选择"注释",然后在弹出的窗口中填上"取得当前模块实例句柄",这一行就会变为
.text:00401007                 mov     hInstance, eax  ; 取得当前模块实例句柄
这样就为我们的反汇编出的代码增加了可读性.
IDA不但可以在当前代码中加注释,还可以更改其默认的符号名,比如
.text:00401011                 mov     dword_0_40300E, eax
其中的dwrd_0_40300E可以看出来是存放取得的命令行的缓冲区指针(可以双击符号名,函数名跳到其定义处),在dword_0_40300E上面点右键,选取"重命名",然后在弹出的窗口中填入lpCommandline,点确定,这样程序中所有使用到了dword_0_40300E这个变量的地方都会将dword_0_40300E替换为lpCommandline.如下所示:
.text:00401011                 mov     lpCommandline, eax
.text:00401016                 push    0Ah
.text:00401018                 push    lpCommandline
我们再来看.text:00401026                 call    sub_0_401031这一行
可以从上面的代码看出来,这是调用的WinMain函数,在sub_0_401031上面点右键,选取"重命名",然这个函数命名为WinMain,这时IDA就将所有sub_0_401031符号变为WinMain, 并且自动加上函数定义,并会在函数调用时入栈的参数后面加上其对应的变量注释,这时我们反汇编出来的这一段代码就成了下面这个样子的了:
.text:00401000 start           proc    near
.text:00401000                  push    0               ; lpModuleName
.text:00401002                  call      GetModuleHandleA
.text:00401007                  mov     hInstance, eax  ; 取得当前模块实例句柄
.text:0040100C                 call      GetCommandLineA
.text:00401011                  mov     lpCommandline, eax
.text:00401016                  push    0Ah             ; nShowCmd
.text:00401018                  push    lpCommandline   ; lpCmdLine
.text:0040101E                  push    0               ; hPrevInstance
.text:00401020                  push    hInstance       ; hInstance
.text:00401026                  call      WinMain
.text:0040102B                  push    eax             ; uExitCode
.text:0040102C                  call      ExitProcess
是不是一目了解了呢?
当我们通过阅读源码,能确定某一个子函数的作用及传入的参数类型时,我们可以双击这个函数名,跳到函数定义处,在函数定义处点右键,使用"设置函数类型"功能来编辑函数定义(C++语法),这样所有调用到这个函数的地方都会在入栈的参数后面加上其对应的变量注释. 还可以通过在函数定义处后面空白处点右键加上"可重复注释",这样所有调用此函数的地方都会在后面加上这个重复的注释.
如果想查看某个变量或函数被调用的情况,可以通过在函数或变量名上点右键,点击"查看操作数交叉索引处"功能,就可以在打开的窗口中查看到所有调用其的代码,并可通过双击跳到这段代码处.这是一个很有用的功能,能帮助你快速的搞清函数及变量的调用关系.
按下F12还可以查看到程序的流程图,CTRL+12可以查看到函数的调用图.
IDA还拥有符号调试技术,能识别常见编释器编释的程序,例如下面反汇编出的VC6.0的程序代码段:
.text:00405427                 push    edx
.text:00405428                 call    _swscanf
.text:0040542D                 lea     eax, [esp+38h+arg_40]
.text:00405431                 push    offset unk_0_5DB1A4 ; const wchar_t *
.text:00405436                 push    eax             ; const wchar_t *
.text:00405437                 call    _wcscmp
.text:0040543C                 add     esp, 1Ch
.text:0040543F                 test    eax, eax
.text:00405441                 jz      short loc_0_405459
.text:00405443                 lea     ecx, [esp+24h+arg_40]
.text:00405447                 push    offset unk_0_5DB18C ; const wchar_t *
.text:0040544C                 push    ecx             ; const wchar_t *
.text:0040544D                 call    _wcscmp
就检查到了其调用了MFC类库中的函数,并把它们替换成了相应的函数名.
还可以调用IDA导出.MAP文件,来配合其它动态调试工具如SOFT-ICE来进行代码分析.
IDA是一个非常强大的反汇编工具,这里只是讨论了一下它的一些基本的应用,希望能起到抛砖引玉的作用,如果对IDA应用有兴趣的朋友可以来信讨论,或参考段钢先生的<<加密与解密>>一书,里面对IDA的应用有较祥细的讲解.
我的Mail:akunspy@sina.com
