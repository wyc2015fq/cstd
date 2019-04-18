# [Assembly] 程序的机器级表示（二）：数据传输指令 - Pangda NoSilly - CSDN博客

2017年05月30日 20:51:40[hepangda](https://me.csdn.net/hepangda)阅读数：1378


## 数据传输指令的形式
|指令|效果|描述|
|----|----|----|
|mov      S, D|D ← S|传送|
|movb||传送字节|
|movw||传送字|
|movl||传送双字|
|movq||传送四字|
|movabsq  I, R|D ← S|传送绝对的四字|

### 需要注意的点
- 传送指令的两操作数不可均为内存位置。
- movl在以寄存器作为目的时，会把寄存器的高位设为0 

例子：分别执行`movabsq $0xABCDEF0123456789, %r8`和`movl $0xFFFFFFFF, %r8d`，此时`%r8`寄存器的值为`00000000FFFFFFFF`
- movq仅可以操作32位补码的立即数，而movabsq仅可以以寄存器为目的地

## 符号扩展/零扩展

### 零扩展
|指令|效果|描述|
|----|----|----|
|movz     S, R|R ← (零扩展)S|以零扩展进行传送|
|movzbw||将做了零扩展的字节传送到字|
|movzbl||将做了零扩展的字节传送到双字|
|movzwl||将做了零扩展的字传送到双字|
|movzbq||将做了零扩展的字节传送到四字|
|movzwq||将做了零扩展的字传送到四字|

### 符号扩展
|指令|效果|描述|
|----|----|----|
|movs     S, R|R ← (符号扩展)S|以符号扩展进行传送|
|movsbw||将做了符号扩展的字节传送到字|
|movsbl||将做了符号扩展的字节传送到双字|
|movswl||将做了符号扩展的字传送到双字|
|movsbq||将做了符号扩展的字节传送到四字|
|movswq||将做了符号扩展的字传送到四字|
|movslq||将做了符号扩展的双字传送到四字|
|cltq|%rax ← (符号扩展)%eax|把%eax符号扩展到%rax|

### 需要注意的点
- 不存在movzlq指令，想要实现将零扩展的双字传送到四字的操作，直接使用movl即可以实现，movl会将高位置0。
- cltq指令没有操作数，会将`%eax`寄存器的内容做符号扩展到`%rax`寄存器。

## 示例

对于以下的C代码：

```
long exchange(long *xp, long y) {
    long x = *xp;
    *xp = y;
    return x;
}
```

GCC将生成以下汇编代码：

```
; long exchange(long *xp, long y)
; xp in %rdi, y in %rsi
exchange:
    movq   (%rdi), %rax
    movq   %rsi, (%rdi)
    ret
```

