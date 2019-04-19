# 汇编：ADD - Koma Hub - CSDN博客
2019年03月21日 22:30:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：28

The add instruction, like it’s x86 counterpart, adds two values on the 80x86. This instruction takes several forms. There are five forms that concern us here. They are：
```
add reg, reg
add reg, memory
add memory, reg
add reg, constant
add memory, constant
```
All these instructions add the second operand to the first leaving the sum in the first operand. For example, add bx,5 computes bx := bx + 5.
