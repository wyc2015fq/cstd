# 64K 色模式下的快速 Alpha混合算法 - xqhrs232的专栏 - CSDN博客
2012年10月26日 22:57:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：737
原文地址::[http://dev.gameres.com/Program/Visual/2D/64Kalpha.htm](http://dev.gameres.com/Program/Visual/2D/64Kalpha.htm)
在 32/64k 色模式下,由于每个点的 RGB 值是放在一个字里,以 16bit 色为例, 一般是按 RGB 或 BGR 565 存放. 传统的软件 Alpha 混合算法是先将 RGB 分离出来, 分开运算,然后再合成. 这造成了 16bit 模式下的 alpha 混合比 24bit 模式下慢的现象,但使用
 16bit color 真的那么慢吗? 我认为如果不使用 MMX 指令, 15/16 的比 24bit 的快. 因为我们可以使用一个小的技巧来同时计算 RGB. 而 24 bit 颜色,除非使用 MMX 指令,否则必须分开计算 R G B.
先设 color 是 RGB 565 的, 那么按 2 进制看, 这个颜色字是这样分布的:
|RRRRR|GGGGGG|BBBBB|
|----|----|----|
|5位|6位|5位|
而 386 以上 CPU 都有 32 位的寄存器,我们只需要将 16bit RGB 变形为
|00000|GGGGGG|00000|RRRRR|000000|BBBBB|
|----|----|----|----|----|----|
|5位|6位|5位|5位|6位|5位|
储存在 32 位寄存器中,(就是把绿色提到 前 16 位里) 由于64k 色下颜色深度是 32 级的,所以 alpha 也只用分 32 级就能满足需要. 那么对上面变形过的双字处理,可以同时算 RGB 了. (Color1*Alpha+Color2*(32-Alpha))/32 能不能简化为 (Color1-Color2)*Alpha/32+Color2.? 我思考过这个问题,以为减法将产生负数,这样再算乘法时有可能出问题,
 但是经过测试,这样简化似乎又没有问题. 毕竟极小的误差是可以忽略的
最近温习了一下汇编,今天用 Nasm 写了个 C 可调用的 Alpha 混合函数 (32bit 模式, 我针对 Djgpp 写的) 并进行了 Pentium 优化(针对双流水线,有错请指出) 大家看看,有 bug,还能优化或有更快的方法也请一定[告诉我](mailto:cloudwu@263.net) btw,
 上面提到的化简没有体现到下面的程序中, 而且,使用乘法本身是个错误. 只是看看吧 ;-) 如果你想实际运用,请参考 Allegro 的做法.
; 对 16bit 的 color1 和 color2 进行 Alpha 混合
; R=(r1*alpha+r2*(32-alpha))/32
; G=(g1*alpha+g2*(32-alpha))/32
; B=(b1*alpha+b2*(32-alpha))/32
; C 语言调用函数 (32 位保护模式) Pentium 双流水线优化
; By Cloud Wu (cloudwu@263.net)
; (http://member.netease.com/~cloudwu)
; -------------------------------------------------------------------------
; unsigned long alpha (unsigned long c1,unsigned long c2,unsigned long alpha);
; -------------------------------------------------------------------------
; c1: 颜色1 RGB (565), c2: 颜色2 RGB (565), alpha: Alpha值 (0~31)
; NASM 编译通过
[BITS 32]
[GLOBAL _alpha]
[SECTION .text]
_alpha:
    ; 初始化代码
    push ebp ; ebp 压栈 
    mov ebp,esp ; 保存 esp 到 ebp 
    mov edi,0x7e0f81f ; dx=00000111111000001111100000011111
    add esp,8 ; esp 指向参数 c1 
    pop eax ; 弹出 c1 到 ax 
    pop ebx ; 弹出 c2 到 bx 
    ; 处理颜色
    mov cx,ax ; cx=r1..b1
    mov dx,ax ; dx=r2..b2
    sal eax,16 ; eax=r1g1b1......
    sal ebx,16 ; ebx=r2g2b2......
    mov ax,cx ; eax=r1g1b1r1g1b1
    mov bx,dx ; ebx=r2g2b2r2g2b2
    and eax,edi ; eax=..g1..r1..b1 
    pop esi ; 弹出 alpha
    mul esi ; eax*=alpha
    neg esi ; -alpha
    and ebx,edi ; ebx=..g2..r2..b2 
    add esi,0x20 ; 32-alpha
    xchg eax,ebx ; 交换 eax,ebx
    mul esi ; c2*=(32-alpha)
    add eax,ebx ; c1*alpha+c2*(32-alpha)
    mov esp,ebp 
    sar eax,5 ；color=(c1*alpha+c2*(32-alpha))/32
    ;还原成 RGB 形式
    pop ebp 
    and eax,edi ; color=..g..r..b
    mov cx,ax ;
    sar eax,16 ; 
    or ax,cx ; color=rgb (eax)
    ret
如果建一张 256K 的表来查表预处理RGB怎样? 经过尝试,发现速度不仅没有提高,反而降低了 :-( 分析的结论是, 256K 的表太大了,以至于不能放到 Cache 里,反而没有计算的方法快, 毕竟计算的话,每行的代码都很快,而不必和内存打交道. 真正加速的方法是什么?借鉴 Allegro 里的方法,建立 32 个函数分别计算每个 alpha 值的情况. 这样,alpha 值变成固定的,从而可以使用 LEA, ADD, SUB, SAL, SAR 来替代 缓慢的 MUL.
