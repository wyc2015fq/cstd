# 编译原子哥的SYSTEM出现错误 Inline assembler not permitted when generating Thumb code - xqhrs232的专栏 - CSDN博客
2015年11月20日 21:23:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5431
原文地址::[http://www.openedv.com/posts/list/0/55803.htm;jsessionid=BF32A849CB8EF733FEEC85010EBCE48C](http://www.openedv.com/posts/list/0/55803.htm;jsessionid=BF32A849CB8EF733FEEC85010EBCE48C)
void WFI_SET(void)
{
__ASM volatile("wfi");  
}
//关闭所有中断
void INTX_DISABLE(void)
{  
__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
__ASM volatile("cpsie i");  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 //set Main Stack value
    BX r14
}
//=========================================修改如下就可以了==============================================
|`01`|`//THUMB指令不支持汇编内联`|
|`02`|`//采用如下方法实现执行汇编指令WFI `|
|`03`|`__asm ``void``WFI_SET(``void``)`|
|`04`|`{`|
|`05`|`    ``WFI;         `|
|`06`|`}`|
|`07`|`//关闭所有中断`|
|`08`|`__asm ``void``INTX_DISABLE(``void``)`|
|`09`|`{`|
|`10`|`    ````CPSID I;```|
|`11`|`}`|
|`12`|`//开启所有中断`|
|`13`|`__asm ``void``INTX_ENABLE(``void``)`|
|`14`|`{`|
|`15`|`    ````CPSIE I;```|
|`16`|`}`|
|`17`|`//设置栈顶地址`|
|`18`|`//addr:栈顶地址`|
|`19`|`__asm ``void````MSR_MSP(u32 addr)```|
|`20`|`{`|
|`21`|`    ````MSR MSP, r0``````//set Main Stack value```|
|`22`|`    ````BX r14```|
|`23`|`}`|
