# volatile的用法(转载) - xqhrs232的专栏 - CSDN博客
2009年11月14日 11:47:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：783
 volatile的本意是“易变的” 
由于访问寄存器的速度要快过RAM,所以编译器一般都会作减少存取外部RAM的优化。比如：
static int i=0;
int main(void)
{
...
while (1)
{
if (i) dosomething();
}
}
/* Interrupt service routine. */
void ISR_2(void)
{
i=1;
}
程序的本意是希望ISR_2中断产生时,在main当中调用dosomething函数,但是,由于编译器判断在main函数里面没有修改过i,因此可能只执行一次对从i到某寄存器的读操作,然后每次if判断都只使用这个寄存器里面的“i副本”,导致dosomething永远也不会被调用。如果将将变量加上volatile修饰,则编译器保证对此变量的读写操作都不会被优化（肯定执行）。此例中i也应该如此说明。
一般说来,volatile用在如下的几个地方：
1、中断服务程序中修改的供其它程序检测的变量需要加volatile;
2、多任务环境下各任务间共享的标志应该加volatile;
3、存储器映射的硬件寄存器通常也要加volatile说明,因为每次对它的读写都可能由不同意义;
另外,以上这几种情况经常还要同时考虑数据的完整性（相互关联的几个标志读了一半被打断了重写）,在1中可以通过关中断来实现,2中可以禁止任务调度,3中则只能依靠硬件的良好设计了。
