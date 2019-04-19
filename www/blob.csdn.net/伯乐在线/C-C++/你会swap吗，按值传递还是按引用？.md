# 你会swap吗，按值传递还是按引用？ - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [foreach_break](http://www.jobbole.com/members/foreach_break) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## **问题**
1、Java到底是按值传递(Call by Value)，还是按引用传递(Call by Reference)？
2、如下面的代码，为什么不能进行交换？

Java
```
public CallBy swap2(CallBy a,CallBy b) {
    CallBy t = a;
    a = b;
    b = t;
    return b;
}
```
3、如下面的代码，为什么能够交换成功？

Java
```
public int swap2(CallBy a,CallBy b) {
	int t = a.value;
	a.value = b.value;
	b.value  = t;
	return t;
}
```
## **简单的C++例子**
为了解决上面的三个问题，我们从简单的例子开始，为什么是C++的例子呢？看完了你就会明白。
假设我们要交换两个整形变量的值，在C++中怎么做呢？
我们来看多种方式，哪种能够做到.

C++
```
#include <iostream>
using namespace std;
// 可以交换的例子
void call_by_ref(int &p,int &q) {
	int t = p;
	p = q;
	q = t;
}
// 不能交换的例子
void call_by_val_ptr(int * p,int * q) {
	int * t = p;
	p = q;
	q = t;
}
// 不能交换的例子
void call_by_val(int p,int q){
	int t = p ;
	p = q;
	q = t;
}
int main() {
	int a = 3;
	int b = 4;
	cout << "---------- input ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl << endl;
	call_by_val(a,b);
	cout << "---------- call_by_val ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl << endl;
	call_by_val_ptr(&a,&b);
	cout << "---------- call_by_val_ptr ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl << endl;
	call_by_ref(a,b);
	cout << "---------- call_by_ref ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl;
}
```
因为例子非常简单，看代码即可知道只有call_by_ref这个方法可以成功交换。这里，**你一定还知道一种可以交换的方式**，别着急，慢慢来，我们先看看为什么只有call_by_ref可以交换。
### **1、call_by_ref**
![](http://ww1.sinaimg.cn/mw690/b254dc71gw1euvjxursrjj20bo0dit9u.jpg)

C++
```
void call_by_ref(int &p,int &q) {
	push   %rbp
	mov    %rsp,%rbp
	mov    %rdi,-0x18(%rbp)
	mov    %rsi,-0x20(%rbp)
    //int t = p;
	mov    -0x18(%rbp),%rax
	//关键点：rax中存放的是变量的实际地址，将地址处存放的值取出放到eax中
	mov    (%rax),%eax
	mov    %eax,-0x4(%rbp)
    //p = q;
	mov    -0x20(%rbp),%rax
	//关键点：rax中存放的是变量的实际地址，将地址处存放的值取出放到edx
	mov    (%rax),%edx
	mov    -0x18(%rbp),%rax
	mov    %edx,(%rax)
    //q = t;
	mov    -0x20(%rbp),%rax
	mov    -0x4(%rbp),%edx
	//关键点：rax存放的也是实际地址，同上.
	mov    %edx,(%rax)
}
```
上面这段汇编的逻辑非常简单，我们看到里面的关键点都在强调：
> 
将值存放在实际地址中.
上面这句话虽然简单，但很重要，可以拆为两点：
1、**要有实际地址**.
2、**要有将值存入实际地址的动作**.
从上面的代码中，我们看到已经有“存值”这个动作，那么传入的是否实际地址呢？

C
```
// c代码
call_by_val_ptr(&a,&b);
// 对应的汇编代码
lea    -0x18(%rbp),%rdx
lea    -0x14(%rbp),%rax
mov    %rdx,%rsi
mov    %rax,%rdi
callq  4008c0 <_Z11call_by_refRiS_>
```
注意到，**lea操作是取地址**，那么就能确定这种“**按引用传递**“的方式，实际是传入了实参的实际地址。
那么，满足了上文的两个条件，就能交换成功。
### **2、call_by_val**
![这里写图片描述](http://ww2.sinaimg.cn/mw690/b254dc71gw1euvjxv9qb7j20bo0cmgmr.jpg)
call_by_val的反汇编代码如下：

C
```
void call_by_val(int p,int q){
	push   %rbp
	mov    %rsp,%rbp
	mov    %edi,-0x14(%rbp)
	mov    %esi,-0x18(%rbp)
	//int t = p ;
	mov    -0x14(%rbp),%eax
	mov    %eax,-0x4(%rbp)
	//p = q;
	mov    -0x18(%rbp),%eax
	mov    %eax,-0x14(%rbp)
	//q = t;
	mov    -0x4(%rbp),%eax
	mov    %eax,-0x18(%rbp)
}
```
可以看到，上面的代码中在赋值时，仅仅是将某种”值“放入了寄存器，再观察下传参的代码：

C++
```
// c++代码
call_by_val(a,b);
// 对应的汇编代码
mov    -0x18(%rbp),%edx
mov    -0x14(%rbp),%eax
mov    %edx,%esi
mov    %eax,%edi
callq  400912 <_Z11call_by_valii>
```
可以看出，仅仅是将变量a、b的值存入了寄存器，而非”**地址**“或者能找到其”**地址**“的东西。
那么，因为不满足上文的两个条件，所以不能交换。
这里还有一点有趣的东西，也就是我们常听说的**拷贝（Copy）**：
> 
当一个值，被放入寄存器或者堆栈中，其拥有了新的地址，那么这个值就和其原来的实际地址没有关系了，这种行为，是不是很像一种拷贝？
但实际上，在我看来，这是一个很误导的术语，因为上面的按引用传递的call_by_ref实际上也是拷贝一种值，它是个地址，而且是实际地址。
所以，应该记住的是那两个条件，在你还不能真正理解拷贝的意义之前最好不要用这个术语。
### **2、call_by_val_ptr**
![这里写图片描述](http://ww3.sinaimg.cn/mw690/b254dc71gw1euvjxvpghfj20c60e80ur.jpg)
这种方式，本来是可以完成交换的，因为我们可以用指针来指向实际地址，这样我们就满足了条件1：
> 
要有实际地址。
别着急，我们先看下上文的实现中，为什么没有完成交换：

C++
```
void call_by_val_ptr(int * p,int * q) {
	push   %rbp
	mov    %rsp,%rbp
	mov    %rdi,-0x18(%rbp)
	mov    %rsi,-0x20(%rbp)
	//int * t = p;
	mov    -0x18(%rbp),%rax
	mov    %rax,-0x8(%rbp)
	//p = q;
	mov    -0x20(%rbp),%rax
	mov    %rax,-0x18(%rbp)
	//q = t;
	mov    -0x8(%rbp),%rax
	mov    %rax,-0x20(%rbp)
}
```
可以看到，上面的逻辑和call_by_val非常类似，也只是做了将值放到寄存器这件事，那么再看下传给它的参数：

C++
```
// c++代码
call_by_val_ptr(&a,&b);
// 对应的汇编代码
lea    -0x18(%rbp),%rdx
lea    -0x14(%rbp),%rax
mov    %rdx,%rsi
mov    %rax,%rdi
callq  4008ec <_Z15call_by_val_ptrPiS_>
```
注意到，lea是取地址，所以这里实际也是将地址传进去了，但为什么没有完成交换？
因为不满足条件2：
> 
将值存入实际地址。
call_by_val_ptr中的交换，从汇编代码就能看出，只是交换了指针指向的地址，而**没有通过将值存入这个地址而改变地址中的值**。
## **Java**
通过上面的例子，我们掌握了要完成交换的两个条件，也了解了什么是传引用，什么是传值，从**实际效果**来讲：
> 
如果传入的值，是实参的实际地址，那么就可以认为是按引用传递。否则，就是按值传递。而实际上，传值和传引用在汇编层面或者机器码层面没有语义，因为都是将某个”值“丢给了寄存器或者堆栈。
所以，类似`Java是按值传递还是按引用传递`这种问题，通常没有任何意义，因为要看站在哪个抽象层次上看。
如果非要定义一下，好吧，也许你会在面试中碰到这种问题，那么最好这样回答：
> 
Java是按值传递的，但可以达到按引用传递的效果。
那么，什么是有意义的呢？
上文的那两个条件。
但从编译的角度讲，引用和地址有很强的关系，却不是一回事。
### **Java按值传递的行为**
我们回顾下开头的三个问题中的第二个问题：
**如下面的代码，为什么不能进行交换？**

C++
```
public CallBy swap2(CallBy a,CallBy b) {
    CallBy t = a;
    a = b;
    b = t;
    return b;
}
```
我们首先从比较简单的bytecode看起：

C++
```
public com.haoran.CallBy swap2(com.haoran.CallBy, com.haoran.CallBy);
    descriptor: (Lcom/haoran/CallBy;Lcom/haoran/CallBy;)Lcom/haoran/CallBy;
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=4, args_size=3
         0: aload_1
         1: astore_3
         2: aload_2
         3: astore_1
         4: aload_3
         5: astore_2
         6: aload_2
         7: areturn
      LineNumberTable:
        line 45: 0
        line 46: 2
        line 47: 4
        line 48: 6
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       8     0  this   Lcom/haoran/CallBy;
            0       8     1     a   Lcom/haoran/CallBy;
            0       8     2     b   Lcom/haoran/CallBy;
            2       6     3     t   Lcom/haoran/CallBy;
```
集中精力看这一块：

C++
```
//t = a
0: aload_1
1: astore_3
//a = b
2: aload_2
3: astore_1
//b = t
4: aload_3
5: astore_2
6: aload_2
```
代码很简单，注释也表明了在做什么， 那么需要不需要看传递给它什么参数呢？
不需要，先看下汇编代码，满足不满足”将值放到实际地址“这个条件，我们截取a = b这一句来观察：


```
// a = b bytecode
2: aload_2
3: astore_1
```


```
/***************************************
* aload_2对应的汇编（未优化）
****************************************/
mov    -0x10(%r14),%rax\n
//----------------------------------------
movzbl 0x1(%r13),%ebx
inc    %r13
movabs $0x7ffff71ad900,%r10
jmpq   *(%r10,%rbx,8)
```


```
/***************************************
* astore_1对应的汇编（未优化）
****************************************/
pop    %rax
mov    %rax,-0x8(%r14)
//----------------------------------------
movzbl 0x1(%r13),%ebx
inc    %r13
movabs $0x7ffff71ae100,%r10
jmpq   *(%r10,%rbx,8)
```
如果将上面的代码和c++实例中的call_by_ref和call_by_val对比，就会发现，上面的代码缺失了这样一种语义：
> 
将值放入实际地址中。
其仅仅是将值放入寄存器或者堆栈上，并没有将值放入**实际地址**这个操作。
为什么不需要观察给这个方法传参的过程？
这是一个很简单的必要条件问题，所以，不需要观察。
从上面的过程来看，Java的行为是Call by Value的。
### **Java按引用传递的行为**
![这里写图片描述](http://ww2.sinaimg.cn/mw690/b254dc71gw1euvjxwdihmj20au0js767.jpg)
现在来讨论第三个问题：
**如下面的代码，为什么能够交换成功？**

C++
```
public int swap2(CallBy a,CallBy b) {
	int t = a.value;
	a.value = b.value;
	b.value  = t;
	return t;
}
```
还是从bytecode先看起：


```
public com.haoran.CallBy swap2(com.haoran.CallBy, com.haoran.CallBy);
    descriptor: (Lcom/haoran/CallBy;Lcom/haoran/CallBy;)Lcom/haoran/CallBy;
    flags: ACC_PUBLIC
    Code:
      stack=2, locals=4, args_size=3
         0: aload_1
         1: getfield      #2                  // Field value:I
         4: istore_3
         5: aload_1
         6: aload_2
         7: getfield      #2                  // Field value:I
        10: putfield      #2                  // Field value:I
        13: aload_2
        14: iload_3
        15: putfield      #2                  // Field value:I
        18: aload_1
        19: areturn
      LineNumberTable:
        line 41: 0
        line 42: 5
        line 43: 13
        line 44: 18
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      20     0  this   Lcom/haoran/CallBy;
            0      20     1     a   Lcom/haoran/CallBy;
            0      20     2     b   Lcom/haoran/CallBy;
            5      15     3     t   I
```
聚焦putfield这句字节码，其对应的是在b.value的值放到操作数栈顶后，拿下这个值，**赋** 给a.value：


```
a.value = b.value;
```
因为putfield字节码对应的汇编代码非常长，我们进一步聚焦其宏汇编，因为CallBy类的value字段，是个int型，所以我们关注itos：

C++
```
// itos
{
	__ pop(itos);
	if (!is_static) pop_and_check_object(obj);
	// 关键点：只看这里
	__ movl(field, rax);
	if (!is_static) {
	  patch_bytecode(Bytecodes::_fast_iputfield, bc, rbx, true, byte_no);
	}
	__ jmp(Done);
}
```
其中关键的一句：`__ movl(field, rax);`

C++
```
void Assembler::movl(Address dst, Register src) {
  InstructionMark im(this);
  prefix(dst, src);
  emit_int8((unsigned char)0x89);
  emit_operand(src, dst);
}
```
movl对应的汇编代码：

C++
```
// 对应的关键汇编代码
...
mov    %eax,(%rcx,%rbx,1)
...
```
上面的汇编代码的意思是：
> 
将eax中的值存入rcx + rbx*1所指向的地址处。
其中,eax的值就是b.value的值，而rcx+rbx*1所指向的地址就是a.value的地址。
上面的过程满足了这样的语义：
> 
将值存入实际地址中.
所以，a.value和b.value可以交换，类似这样，**Java的按值传递方式也可以表现出按引用传递的行为**。
## **另一种交换值的方式**
还记得在C++的实例中，我们提到还有一种交换值的方式，是什么呢？
### **call_by_WHAT**

C++
```
void call_by_WHAT(int * p,int * q) {
	int t = *p;
	*p = *q;
	*q = t;
}
```
这样传参：

C++
```
int main() {
	int a = 3;
	int b = 4;
	cout << "---------- input ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl << endl;
	call_by_WHAT(&a,&b);
	cout << "---------- call_by_WHAT ------------" << endl;
	cout << "a = " << a << ", b = " << b << endl;
}
```
会不会交换呢？

C++
```
// 输出
---------- input ------------
a = 3, b = 4
---------- call_by_WHAT ------------
a = 4, b = 3
```
从这种方式中，我们也看到了所有能够交换值的方式的统一性：
1、指针p、q或者对象引用objectRef，能够直接指向对象的实际地址。
2、要有一个将值放入实际地址的操作：

C++
```
// C/C++
*p = *q;
...
// Java
putField -> a.value = b.value
...
// 汇编
mov reg_src , (reg_dst)
mov reg_src , (addr_dst)
...
```
## **结语**
![这里写图片描述](http://ww1.sinaimg.cn/mw690/b254dc71gw1euvjxxknn7j20lx0ae40d.jpg)
老生常谈，并无LUAN用，终。
