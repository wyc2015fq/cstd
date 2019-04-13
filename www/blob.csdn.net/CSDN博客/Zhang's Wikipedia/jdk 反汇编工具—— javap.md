
# jdk 反汇编工具—— javap - Zhang's Wikipedia - CSDN博客


2018年07月18日 23:01:19[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1170


javap：对由 javac 编译出的 .class 文件进行反汇编；javap -c HelloWorld：.class 文件不需加 .class 后缀；

## 1. 常见参数
-c：对代码进行反编译；
-v（–verbose）：
## 2. 反汇编结果
函数局部变量赋值：
void func() {
	int i = 2;
	int j = 3;
}对其反编译的结果为：
Code:
  0:   iconst_2    //把2放到栈顶，这里的栈指的是操作数栈，此时的 _2 表示的是数值
  1:   istore_1    //把栈顶的值放到局部变量1中，即i中，这里的局部变量指的是局部变量表
  				// 此时的 _1 表示的是局部变量的序号；
  2:   iconst_3    //把3放到栈顶
  3:   istore_2    //把栈顶的值放到局部变量1中，即j中
  4:   returnvoid func() {
	int i = 2;
	int j = i;
}对其进行反编译：
Code:
   0:   iconst_2    //把2放到栈顶
   1:   istore_1    //把栈顶的值放到局部变量1中，即i中
   2:   iload_1     //把局部变量1的值放到栈顶，也就是说此时栈顶的值是2
   3:   istore_2    //把栈顶的值放到局部变量2中，即j中
   4:   return

