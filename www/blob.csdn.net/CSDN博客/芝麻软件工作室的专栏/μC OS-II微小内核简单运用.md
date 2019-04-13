
# μC-OS-II微小内核简单运用 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:56:40[seven-soft](https://me.csdn.net/softn)阅读数：268个人分类：[uCos-II																](https://blog.csdn.net/softn/article/category/6305029)



对于μC/OS-II来说，大多数API都是成对的，而且一部分必须配对使用。






中断服务程序不能调用可能会导致任务调度的函数，它们主要是一些等待事件的函数，这些函数及其替代函数见下表。

注意：未列入表中的函数OSTaskCreate()、OSTaskCreateExt()、OSTaskDel()、OSTaskResume()、 OSTaskChangePrio()、OSTaskSuspend()、OSTimeDly()、OSTimeDlyHMSM() 、OSTimeResume()都属于在中断服务程序中禁止调用的函数，一些函数虽然没有明确地规定不能被中断服务程序调用，但因为中断服务程序的特性， 一般不会使用。





















