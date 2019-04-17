# 修改uCOS_II以实现“优先级+时间片”联合调度 - xiahouzuoxin - CSDN博客





2013年08月20日 16:10:37[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4768











本文在uCOS II上增加时间片任务调度的的原理：

对设置为同优先级的任务使用时间片调度，不同优先级任务仍然使用uCOS II的优先级调度策略。在同优先级任务的时间片调度中，所有任务暂时时间片长度固定，时间片的调度使用FIFO（先进先出）队列。




整体的描述参看下图。

![](https://img-blog.csdn.net/20130820160147781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


上图中假设有3个优先级为5的任务，3个优先级为8的任务。纵向，第一列的OS_TCB形成OSTCBList双向链表（通过OSTCBNext和OSTCBPrev指针，这是原uCOS II系统已经有的部分）。横向，通过OSPSPrev和OSPSNext指针形成时间片链表（实际上是FIFO），这部分是我们在uCOS II上要增加的内容。第一列的任务每当有任务时间片用完后将挪到队列尾，从FIFO中选择下一个任务，这就是本文的时间片调度的过程。总体上看来，不同优先级使用了uCOS
 II原有的优先级调度策略，相同优先级之间增加了时间片调度策略，因此本文称为“优先级+时间片”联合调度。




下面我们列出实现时间片调度要修改的一些结构和函数，我们可以通过调试跟踪了解uCOS II的机理，从而理解为什么要这么做。

**1 修改结构体OS_TCB（ucos_ii.h文件中）**

增加4个成员变量用于时间片调度。



```cpp
struct os_tcb {
	...

	/* 增加下面4个成员 */
	struct os_tcb *OSPSNext;  /* 同一优先级下，每个任务的后向指针 */
    struct os_tcb *OSPSPrev;  /* 同一优先级下，每个任务的前向指针 */
    INT8U OSPSLen;            /* 该任务分配的时间片             */
    INT8U OSPSCurLen;         /* 该任务当前剩下的时间片          */
} OS_TCB;
```






**2 修改OS_TCBInit任务结构体初始化函数（os_core.c文件）**

在OS_TCBInit中增加时间片长度的初始化，我们可以先在os_cfg_r.h中宏定义一个时间片长度用于不同情况下的配置，



```cpp
#define TIME_SLICE_LEN            10
```



OS_TCBInit函数中增加结构体成员初始化



```cpp
ptcb->OSPSLen = TIME_SLICE_LEN;
ptcb->OSPSCurLen = TIME_SLICE_LEN;
ptcb->OSPSNext = (OS_TCB*)0;
ptcb->OSPSPrev = (OS_TCB*)0;
```



当出现同优先级的情况时，任务控制块插入到时间片链表中而不是优先级链表中，OS_TCBInit函数中要增加将任务插入到时间片链表的过程，我写的一个示例如下：



```cpp
if (ptcb != (OS_TCB *)0) {
	...
	
	OSTCBPrioTbl[prio] = ptcb;
	if (OSTCBList != (OS_TCB *) 0) { /* 不存在同优先级任务， 按照uCOS ii方法链接到OSTCBList */
		ptcb->OSTCBNext    = OSTCBList; /* Link into TCB chain                      */
		ptcb->OSTCBPrev    = (OS_TCB *)0;
		if (OSTCBList != (OS_TCB *)0) {
		    OSTCBList->OSTCBPrev = ptcb;
		}
		OSTCBList               = ptcb;
		OSRdyGrp               |= ptcb->OSTCBBitY; /* Make task ready to run */
		OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
	} else {  /* 出现同优先级情况，链接到时间片链表 */
		while(ptcb1->OSPSNext!=(OS_TCB*) 0)
		{
		   ptcb1=ptcb1->OSPSNext;   /* move to the rear of time slice list */
		}
		ptcb1->OSPSNext = ptcb;
		ptcb->OSPSPrev = ptcb1; 
		ptcb->OSPSNext=(OS_TCB*) 0;

		ptcb->OSTCBNext = (OS_TCB *)0;
		ptcb->OSTCBPrev = (OS_TCB *)0;
	}

	OSTaskCtr++;  /* Increment the #tasks counter             */
	OS_EXIT_CRITICAL();
	return (OS_ERR_NONE);
}
```





**3 修改时钟中断函数OSTimeTick（os_core.c文件）**
增加时间片调度的内容，这是时间片调度的主要部分。给出一个示例：



```cpp
OSTimeTick (void)
{
	...
	OS_TCB *ptcb1;
  
	if (OSRunning == TRUE)
	{
		ptcb = OSTCBList;		/* Point at first TCB in TCB list           */
		while (ptcb->OSTCBPrio != OS_IDLE_PRIO)
		{			/* Go through all TCBs in TCB list          */
		  	OS_ENTER_CRITICAL ();

			/* delay time out */
			...

			/* time slice count down */
			if (ptcb->OSPSCurLen != 0) {
			    --ptcb->OSPSCurLen;
			} else {  /* time slice out */
			    /* reload time slice */
			    ptcb->OSPSCurLen = ptcb->OSPSLen;

				ptcb1 = ptcb->OSPSNext;
				if (ptcb1 != (OS_TCB*) 0) {  /* there are some tasks with the same priority */
	 				/* (1) link new to priority-level list */
		            ptcb1->OSTCBNext = ptcb->OSTCBNext;
					ptcb1->OSTCBPrev = ptcb->OSTCBPrev;
					if (ptcb->OSTCBNext != (OS_TCB *)0) {
					    ptcb->OSTCBNext->OSTCBPrev = ptcb1;    
					}
					if (ptcb->OSTCBPrev != (OS_TCB *)0) {
					    ptcb->OSTCBPrev->OSTCBNext = ptcb1;
					}
					ptcb->OSTCBPrev = (OS_TCB *)0;
					ptcb->OSTCBNext = (OS_TCB *)0;

					/* (2) update OSTCBList if TCBcur is OSTCBList*/
	                if (ptcb == OSTCBList) {
					    OSTCBList = ptcb1;   
					}			

					/* (3) Compute X, Y, BitX and BitY */
					ptcb1->OSTCBY = ptcb1->OSTCBPrio >> 3;	 
					ptcb1->OSTCBBitY = OSMapTbl[ptcb1->OSTCBY]; 
					ptcb1->OSTCBX = ptcb1->OSTCBPrio & 0x07;                        
					ptcb1->OSTCBBitX = OSMapTbl[ptcb1->OSTCBX];	
					
				    /* (4) set task to be ready */
					if (ptcb1->OSTCBDly == 0) {
						OSRdyGrp |= OSMapTbl[ptcb1->OSTCBY];
						OSRdyTbl[ptcb1->OSTCBY] |= OSMapTbl[ptcb1->OSTCBX];
					}							

		            /* (5) move ptcb to the rear of queue */					    
		            while(ptcb1->OSPSNext!=(OS_TCB*) 0)
		            {
		                ptcb1=ptcb1->OSPSNext;
		            }
		            ptcb1->OSPSNext = ptcb;	   
		            ptcb->OSPSPrev = ptcb1; 
		            ptcb->OSPSNext=(OS_TCB*) 0;

					/* (6) set OSTCBPrioTbl for Context switch */
					OSTCBPrioTbl[ptcb1->OSTCBPrio]=ptcb1;  
				} else {
				    // do nothing
				}	
			}

			/* 
			 * (7) don't forgot that after time slice 
			 * schedule, ptcb->OSTCBNext==NULL 
			 */
			if (ptcb->OSTCBNext != (OS_TCB*)0) {
				ptcb = ptcb->OSTCBNext;
			} else if (ptcb1->OSTCBNext != (OS_TCB*)0) {
			    ptcb = ptcb1->OSTCBNext;
			} else {
			    break;
			}
			
			OS_EXIT_CRITICAL ();
		}
	}
}
```


OK，修改好上面的内容就大致实现了在uCOS II上增加时间片调度的过程，通过修改时间片长度TIME_SLICE_LEN可以验证时间片长度对任务调度的影响。本文最后在硬件平台STM32F103RB上测试通过，但因为时间片的引入而且未对程序做相关优化，因此时间片调度的实时性提高上还有待完善。










