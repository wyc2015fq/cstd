# [Assembly] 程序的机器级表示（三）：栈指令、算术和逻辑指令 - Pangda NoSilly - CSDN博客

2017年06月02日 11:36:21[hepangda](https://me.csdn.net/hepangda)阅读数：530
个人分类：[Assembly](https://blog.csdn.net/hepangda/article/category/6946087)



# 栈指令
|指令|效果|描述|
|----|----|----|
|pushq    S|%rsp ← %rsp - 8|将四字压入栈|
|(%rsp) ← S| | |
|popq    D|D ← (%rsp)|将四字弹出栈|
|%rsp ← %rsp + 8| | |

## 需要注意的点
- pushq指令的行为等价于：`subq $8, %rsp` 与`movq %rbp, (%rsp)`两条指令的合效果。
- popq指令的行为等价于： `movq (%rsp), %rax` 与`addq $8, %rsp`两条指令的合效果。
- push/pop指令不存在其他后缀。

# 算数和逻辑指令
|指令|效果|描述|
|----|----|----|
|leaq S , D|D ← &S|加载有效地址|
|inc D|D ← D + 1|加一|
|dec D|D ← D - 1|减一|
|neg D|D ← -D|取负|
|not D|D ← ~D|取补|
|add S , D|D ← D + S|加|
|sub S , D|D ← D - S|减|
|imul S , D|D ← D * S|乘|
|div S , D|D ← D ÷ S|除|
|xor S , D|D ← D ^ S|异或|
|or S , D|D ← D | S|或|
|and S , D|D ← D & S|与|
|sal k , D|D ← D << k|左移|
|shl k , D|D ← D << k|左移（等同于sal）|
|sar k , D|D ← D >>(A) k|算数右移|
|shr k , D|D ← D >>(L) k|逻辑右移|

## 加载有效地址

**加载有效地址**指令leaq实质上是movq指令的变形。它的指令形式是从内存读取数据到寄存器，但实际上leaq没有引用内存。他的作用是将内存地址放入操作数中。**该命令的目标必须是一个寄存器**，同时leaq可以简洁的描述算数运算，所以有时它的使用与有效地址的计算无关。例如：

```
long scale(long x, long y, long z) {
    long t = x + 4 * y + 12 * z;
    return t;
}
```

```
; long scale(long x, long y, long z)
; x in %rdi, y in %rsi, z in %rdx
scale:
    leaq (%rdi, %rsi, 4), %rax    ; x + 4 * y
    leaq (%rdx, %rdx, 2), %rdx    ; z + 2 * z
    leaq (%rax, %rdx, 4), %rax    ; (x + 4 * y) + 4 * (3 * z)
```

# 特殊的算数指令
|指令|效果|描述|
|----|----|----|
|imulq S|%rdx : %rax ← S × (%rax)|有符号全乘法|
|mulq S|%rdx : %rax ← S × (%rax)|无符号全乘法|
|clto|%rdx : %rax ← 符号扩展(%rax)|扩展为八字|
|idivq S|%rdx ← (%rdx) : (%rax) mod S|有符号除法|
||%rdx ← (%rdx) : (%rax) ÷ S||
|divq S|%rdx ← (%rdx) : (%rax) mod S|无符号除法|
||%rdx ← (%rdx) : (%rax) ÷ S||

