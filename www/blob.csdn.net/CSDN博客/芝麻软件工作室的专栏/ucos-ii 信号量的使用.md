
# ucos-ii 信号量的使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 10:26:55[seven-soft](https://me.csdn.net/softn)阅读数：602


信号量使用主要涉及三个函数： OSSemCreat(), OSSemPend(), OSSemPost(); 第一个负责建立一个信号量，同时需要的参数是设定信号量的值；第二个是等待该信号量，当信号量值为0时，将调用改函数的任务加入该信号量的等待任务列表 中，当信号量值大于0时，该函数紧紧将信号量值减1；第三个函数负责置位指定的信号量，当指定的信号量是0或者大于0时，该函数将信号量值加1然后返回。 此时在该信号量的等待任务列表中优先级最高的任务进入就绪态，任务调度函数进行任务调度。
void DemoTask(void *pdata)
{
INT32U i;
pdata = pdata;
while(1)
{
for(i=0; i<3000000; i++); //delay time
LED_off;   //control the led
OSSemPost(FirstSem);
}
}
void TestTask(void *pdata)
{
INT8U err;
INT32U i;
while (1)
{
OSSemPend(FirstSem, 0, &err);
for(i=0; i<3000000; i++); //delay time
LED_on; //control the led
}
}
首先要先建立这两个任务和FirstSem信号量，同时将信号量值设为0，然后该段代码可以实现LED闪烁。

