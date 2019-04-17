# 在VS2008中移植uCOS-II - xiahouzuoxin - CSDN博客





2012年06月24日 12:35:46[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：5138








**在VS2008中移植uCOS-II**

目的：我也是初学者，只为初学者建立环境，大家一起学习

参考资源：所有相关资源已经上传到CSDN下载版块[http://download.csdn.net/detail/xiahouzuoxin/4389537](http://download.csdn.net/detail/xiahouzuoxin/4389537)

1、新建工程

![](https://img-my.csdn.net/uploads/201206/24/1340512015_5314.JPG)

![](https://img-my.csdn.net/uploads/201206/24/1340512035_3421.JPG)

2、拷贝uCOS-II文件夹到工程目录下；

将“..\uCOS-II\Other_Headers”下的“include.h”和“SystemConfig.h”两个头文件剪切到..\uCOS_Demo\uCOS_Demo目录下；

3、配置工程

在工程上单击右键“属性”，弹出uCOS_Demo属性页对话框。

（1）附加包含目录配置

![](https://img-my.csdn.net/uploads/201206/24/1340512135_5321.JPG)

（2）编译代码类型配置为C代码

![](https://img-my.csdn.net/uploads/201206/24/1340512153_9730.JPG)

（3）添加依赖项”WinMM.lib”

![](https://img-my.csdn.net/uploads/201206/24/1340512171_9943.JPG)

4、在工程中添加源文件

![](https://img-my.csdn.net/uploads/201206/24/1340512184_2939.JPG)

5、编译试试，出现如下Error

1>ucos_ii.obj : error LNK2019: 无法解析的外部符号_OSDebugInit，该符号在函数_OSInit 中被引用

这是由于OSDebugInit没有定义导致，因此在os_trace.c中添加如下代码

/* 移植时所需定义*/

void OSDebugInit()

{

}

6、再编译工程试试，OK

7、创建3个任务：MianTask、App1Task、App2Task

（1）“SystemCOnfig.h”代码如下：

#ifndef SYSTEM_CONFIG_H

#define SYSTEM_CONFIG_H



#define _CRT_SECURE_NO_WARNINGS 



#include
<stdio.h>

#include
<assert.h>

#include
<includes.h>



/*

*********************************************************************

*                         TASK PRIORITIES

*********************************************************************

*/



#define MainTask_Prio               5

#define App1Task_Prio               7

#define App2Task_Prio               9



/*

*********************************************************************

*                         TASK STACK SIZES

*********************************************************************

*/



#define MainTask_StkSize            1024

#define App1Task_StkSize            1024

#define App2Task_StkSize            1024



#define OS_TASK_TMR_PRIO        65533

#define OS_IDLE_PRIO            OS_TASK_IDLE_PRIO

#define OS_STAT_PRIO            OS_TASK_STAT_PRIO



/*

*********************************************************************

*                         TASK 

*********************************************************************

*/

externvoid MainTask(void *p_arg);

externvoid App1Task(void *p_arg);

externvoid App2Task(void *p_arg);



#endif

（2）“uCOS_Demo.cpp”代码如下

//uCOS_Demo.cpp : 定义控制台应用程序的入口点。

#include
"stdafx.h"

#include
"SystemConfig.h"



/* 定义堆栈*/

OS_STKMainTask_Stk[MainTask_StkSize];

OS_STKApp1Task_Stk[App1Task_StkSize];

OS_STK App2Task_Stk[App2Task_StkSize];



int _tmain(int argc, _TCHAR*argv[])

{

    OSInit();  /* 系统初始化*/



/* 创建主任务*/

    OSTaskCreate(MainTask, (void *)0,&MainTask_Stk[MainTask_StkSize-1], MainTask_Prio);



    OSStart();  /* 开始任务调度*/



return 0;

}



/* 主任务*/

void MainTask(void *p_arg)

{

    p_arg = p_arg;



    OSStatInit();  /* 统计任务初始化*/

while(1)

    {

/* 创建其他任务*/

       OSTaskCreate(App1Task, (void*)0, &App1Task_Stk[App1Task_StkSize-1], App1Task_Prio);

       OSTaskCreate(App2Task, (void*)0, &App2Task_Stk[App2Task_StkSize-1], App2Task_Prio);

       OSTimeDlyHMSM(0,0,1,0); /* 任务调度*/

//OSTaskDel(MainTask_Prio);  
/* 删除主任务*/ 

    }

}



/* App1Task */

void App1Task(void *p_arg)

{

    p_arg = p_arg;



while(1)

    {

       printf("Hello,I am PC!\n");



       OSTimeDlyHMSM(0, 0, 1, 0); /* 任务调度*/

    }

}



/* App2Task */

void App2Task(void *p_arg)

{

    p_arg = p_arg;



while(1)

    {

       printf("Hello,uCOS-II!\n");



       OSTimeDlyHMSM(0, 0, 2, 0); /* 任务调度*/

    }

}

运行结果如下

![](https://img-my.csdn.net/uploads/201206/24/1340512243_4483.JPG)

【注】所有移植过程过程参考网上资源



