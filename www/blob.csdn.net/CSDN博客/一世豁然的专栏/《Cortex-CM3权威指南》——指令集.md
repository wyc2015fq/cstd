# 《Cortex-CM3权威指南》——指令集 - 一世豁然的专栏 - CSDN博客





2015年05月06日 08:46:19[一世豁然](https://me.csdn.net/Explorer_day)阅读数：3736








一、汇编语言基础

 一）、汇编语言：基本语法

  1、汇编指令最典型的书写模式：

**标号**

**                   操作码        操作数1， 操作数2，... ；注释**

   1）、标号是可选的，如果有，它必须顶格写。标号的作用是让汇编器来计算程序转移的地址。

   2）、操作码是指令的助记符，它的前面必须有至少一个空白符，通常使用提个Tab键来产生。

   3）、操作码后面往往跟若干个操作数，而第一个操作数，通常都给出本指令执行结果的存储地。不同指令需要不同数目的操作数，并且对操作数的语法要求也可以不同。

   4）、注释均以**;**开头，它的有无不影响汇编操作，只是给程序员看的，让程序员更加可以理解代码。




  2、可以使用EQU指示子来定义常数，也可以使用DCB来定义一串字节常数——允许以字符串的形式表达，还可以使用DCD来定义一串32位整数。




  3、如果汇编器不能识别某些特殊指令的助记符，就需要“手工汇编”，查出该指令的确切二进制机器码，然后使用DCI编译器指示器。




  4、不同汇编器的指示字和语法都可以不同。以上以ARM汇编器说明，如使用其他汇编器，细看说明和实例代码。







 二）、汇编语言：后缀的使用

  1、在ARM处理器中，指令可以带有后缀的：

![](https://img-blog.csdn.net/20150505083644695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、在Cortex-CM3中，对条件后缀的使用有限制，只有转移指令（B指令）才可以随意使用。而对于其他指令，Cortex-CM3引入IF-THEN模块，在这个块中才可以加后缀，且

        必须加后缀。







 三）、汇编语言：统一的汇编语言

  1、为了有力支持Thumb-2，引入了一个“统一汇编语言（UAL）”语法机制。对于16位指令和32位指令均能实现一些操作，有时虽然指令的实际操作数不同，或者对立即数的

        长度有不同的限制，但是汇编器允许开发者以相同的语法格式编写，并且由汇编器来决定使用16位指令还是32位指令。




  2、如果使用了传统的Thumb语法有些指令会默认更新APSR，即使你没有加上S后缀。如果使用UAL语法，则必须使用S后缀才能更新。




  3、在Thumb-2指令集中，有些操作既可以由16位指令完成，也可以由32位指令完成。在UAL下，可以让编译器决定用哪个，也可以手工指令使用16位还是32位。

   1）、**.W**后缀指定32位指令。如果没有给出后缀，会bain其会先试着用16位指令以缩小代码体积如果不行在使用32位指令。

   2）、**.N**后缀指定16位指令。




  4、32位Thumb-2指令可以按半字节对齐。







四、指令集

  1、APSR中的5个标识位：

   1）、N：复数表示（Negative）。

   2）、Z：零结果标识（Zero）。

   3）、C：进位/借位标识（Carry）。

   4）、V：溢出标识（oVerflow）。

   5）、S：饱和标识（Saturation），它不做条件转移的依据。




  2、Cortex-CM3支持的指令集如下：

    注意：边框双粗的是从ARMv6T2才支持的指令。

                双线边框的是从Cortex-CM3才支持的指令，（v7的其他款式不一定支持）。

   1）、16位数据操作指令

![](https://img-blog.csdn.net/20150505091758547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、16位转移指令

![](https://img-blog.csdn.net/20150505114409196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   3）、16位存储器数据传送指令

![](https://img-blog.csdn.net/20150505114138670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   4）、其他16位指令

![](https://img-blog.csdn.net/20150505092714407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   5）、32位数据操作指令

![](https://img-blog.csdn.net/20150505114224066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150505114549846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   6）、32位存储器数据传送指令

![](https://img-blog.csdn.net/20150505114509629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   7）、32位转移指令

![](https://img-blog.csdn.net/20150505115114796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   8）、其他32位指令

![](https://img-blog.csdn.net/20150505115150674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150505115535438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、未支持的指令

   1）、不再是传统的架构，呆滞某些指令已失去意义

![](https://img-blog.csdn.net/20150505115637684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、不支持的协处理器相关指令

![](https://img-blog.csdn.net/20150505115824513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、不支持的CPS指令用法

![](https://img-blog.csdn.net/20150505120141387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   4）、不支持的hint指令

![](https://img-blog.csdn.net/20150505120122400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







五、近距离的检视指令

 一）、汇编语言：数据传送

  1、Cortex-CM3中的数据传输类型

   1）、两个寄存器间的传输数据。

   2）、寄存器与存储器间传输数据。

   3）、寄存器与特殊功能寄存器间传输数据。

   4）、把一个立即数加载到寄存器。




  2、用在数据传输的指令时MOV，它的另一个衍生物是MVN——把寄存器的内容取反后再传送。




  3、用于访问存储器的基础指令是“加载（load）”和“存储（store）”。

   1）、加载指令LDR把存储器中的内容加载到寄存器中，存储指令STR把寄存器的内容存储到存储器中。传送过程中数据类型也可以变通，最常见的格式有：

![](https://img-blog.csdn.net/20150505121848632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150505121901537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、如果想一次性的解决存储器访问问题，可以使用LDM/STM来进行，它相当于把若干个LDR/STR给合并起来。

![](https://img-blog.csdn.net/20150505122240068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    I、表中加粗的符合Cortex-CM3堆栈操作的LDM/STM的使用方式。并且如果Rd是R13，则与PUSH和POP等效。

    II、感叹号表示自增或自减基址存储器Rd的值，时机是在每次访问前或访问后。感叹号还可以用于单一加载或存储指令，——LDR/STR。这也就是所谓的“带预索引”的LDR和

          STR。

![](https://img-blog.csdn.net/20150505122929293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150505123135373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    III、Cortex-CM3还支持后索引。后索引也要使用一个立即数offset，但与预索引不同的是，后索引是忠实使用基址寄存器Rd的值作为数据传送的地址的。待到数据传输之后，

           在执行Rd<-Rd+offset。

![](https://img-blog.csdn.net/20150505123503290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、LDR伪指令 VS ADR伪指令

   1）、LDR和ADR都有能力产生一个地址，但是语法和行为不同。

   2）、对于LDR，如果汇编器发生要产生立即数是一个程序地址，它会自动把LSB置位，

   3）、对于ADR相反，它不会修改LSB。







 二）、汇编语言：数据处理

  1、虽然助记符都是ADD，但是二进制机器码是不同的。当使用16位加法时会自动更新APSR的标识位。然而，在使用了“.W”显式指定了32位指令后，就可以通过“S”后缀控制

        对APSR的更新。




  2、常见的算术四则运算指令

![](https://img-blog.csdn.net/20150505152354458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、Cortex-CM3还片载了硬件乘法器，支持乘加/乘减运算，并能产生64位的值。

![](https://img-blog.csdn.net/20150505152622825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、逻辑运算

![](https://img-blog.csdn.net/20150505152551400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  5、移位（支持多种组合）运算和循环运算

![](https://img-blog.csdn.net/20150505152841487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  6、带符号扩展指令

![](https://img-blog.csdn.net/20150505153507283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  7、数据序转指令

![](https://img-blog.csdn.net/20150505153547724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  8、位段处理及把玩指令

![](https://img-blog.csdn.net/20150505153521456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 三）、汇编语言：子程序呼叫与无条件转移指令

  1、最基本的无条件转移指令有两条：

     B    Label    ；转移到Label处对应的地址

     BL    reg    ；转移到有寄存器reg给出的地址




  2、呼叫子程序时，需要保存返回地址，正点的指令是：

     BL    Label    ；转移到Label处对应的地址，并且把转移前的下条指令地址保存到LR

     BLX    reg    ；转移到由寄存器reg给出的地址，根据REG的LSB切换处理器的状态，并且把转移前的下条指令地址保存到LR

   注意：、使用BLX要注意，其改变状态的功能。因此确保reg的lsb必须为1，以确保不会进入ARM状态。




  3、以PC为目的寄存器的MOV和LDR指令也可以实现转移，常见的形式有：

     MOV    PC,    R0    ;         转移地址由R0给出


     LDR     PC,    [R0]    ;      转移地址存储在R0所指向的存储器中


     POP    {...,  PC]    ;     把返回地址以弹出堆栈的方式送给PC，从而实现转移

     LDMIA    SP!,     {...,  PC    ;   POP另一种等效写法

   注意：使用这些方式必须保证送给PC的值是奇数（LSB=1）。







 四）、标志位与条件指令

  1、在应用程序状态寄存器中有5个标志位，但只有4个被条件转移指令参考。绝大多数ARM的条件转移指令根据他们来决定是否转移。

![](https://img-blog.csdn.net/20150505195625927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、在ARM中数据操作指令可以更新这4个标志位。这些标志位除了可以当条件转移的判断之外，还能再一些场合下作为指令是否执行的依据。或者在移位操作中充当各种中介

        角色。




  3、担任条件转移和条件执行的依据时，这4个标志位既可以单独使用，也可以组合使用，以产生15种判断依据：

![](https://img-blog.csdn.net/20150505200131407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、在Cortex-CM3中，下列指令可以更新PSR的标志：

   1）、16位算术逻辑指令。

   2）、32位带S后缀的算术逻辑指令。

   3）、比较指令和测试指令。

   4）、直接写PSR/APSR（MAR指令）。







 五）、汇编语言：指令隔离指令和存储器隔离指令


![](https://img-blog.csdn.net/20150505200926609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 六）、汇编指令：饱和运算

  1、Cortex-CM3的饱和运算指令分为两种：带符号的饱和运算以及无符号饱和运算。




  2、饱和运算指令

![](https://img-blog.csdn.net/20150505201346133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










六、Cortex-CM3中一些有用的新指令

  1、MSR和MRS

   1）、这两条指令是访问特殊功能寄存器的指令，必须在特权下访问，出APSR外。

   2）、指令语法如下：

          MRS    <Rn>,     <SReg>  ;   加载特殊功能寄存器的值到Rn


          MSR    <SReg>,     <Rn>  ;   存储Rn的值到特殊功能寄存器的值

   3）、SReg可以是下表中的一个

![](https://img-blog.csdn.net/20150505202050152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、IF-THEN

   1）、IF-THEN指令围成一个块，里面最多4条指令，它里面的指令可以条件执行。

   2）、IT使用形式：

![](https://img-blog.csdn.net/20150505202429607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、CBZ和CBNZ

   1）、比较并条件跳转指令专为循环结构的优化而设，它只能做向前跳转。

   2）、格式为

![](https://img-blog.csdn.net/20150505202807664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





  4、SDIV和UDIV

   1）、32位硬件除法指令。

   2）、格式

![](https://img-blog.csdn.net/20150505203220086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、运算结果是Rd = Rn / Rm。




  5、REV,REVH,REV16以及REVSH

   1）、REV反转32位整数中的字节序，REVH则以半字节为单位反转，且只反转低半字节。

   2）、REVSH在REVH的基础上，还把反转后的半子做带符号的扩展。

   3）、语法

![](https://img-blog.csdn.net/20150506080825636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  6、RBIT

   1）、RBIT比以前的REV之流更为精细，它是按位反转的，相当于把32位整数的二进制表示法水平旋转180°。此指令在处理串行比特流大有用处。

   2）、格式

**RBIT.W    Rd，    Rn**




  7、SXTB,SXTH,UXTB,UXTH

   1）、这四条指令是为了优化C的强制数据类型转换而设的，把数据宽度转换为处理器喜欢的32位长度。

   2）、语法

![](https://img-blog.csdn.net/20150506081605197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、对于SXTB/SXTH，数据带符号位扩展成32位整数。对于UXTB/UXTH，高位清0。




  8、BFC/BFI,UBFX/SBFX

   1）、这四个指令是Cortex-CM3提供的位段操作指令。

   2）、BFC（位段清零）指令把32位整数任意一段连续的二进制位S清0，语法格式为：

**  BFC    Rd，    #lsb，    #width**

   3）、BFI（位段插入指令），则把某寄存器按LSB对齐的数据，拷贝到另一个寄存器的某个位段中，其格式为：

**BFI.w    Rd，    Rn，    #lsb，    #width**

    4）、UBFX/SBFX都是位段提取指令，语法格式为：

**UBFx.w    Rd，    Rn，    #lsb，    #width**

**SBFX.w    Rd，    Rn，    #lsb，    #width**




  9、LDRD/STRD

   1）、Cortex-CM3在一定程度上支持对64位整数，其中LDRD/STRD就是为64位整数的数据传输而设置的。

   2）、格式：

![](https://img-blog.csdn.net/20150506083038269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  10、TBB,TBH

   1）、TBB（查表跳转字节范围的偏移量）指令和TBH（查表跳转半字节范围的偏移量）指令，分别用于从一个字节数组表中查找偏移地址，和从半字节数组表中查找偏移地

            址。TBB的跳转范围为255*2+4=514，TBH的跳转范围为65535*2+4=128kb+2。

   2）、TBB语法格式

** TBB.W    [Rn，   Rm]    ；PC+=Rn[Rm]*2**

   3）、TBH语法格式   

**TBH.W    [Rn，   2*Rm]    ；PC+=Rn[2*Rm]*2**




