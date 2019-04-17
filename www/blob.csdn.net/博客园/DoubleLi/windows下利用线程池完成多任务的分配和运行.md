# windows下利用线程池完成多任务的分配和运行 - DoubleLi - 博客园






在做项目的过程中有时候为了提升效率，用了多线程的方法来对任务进行分割和应用，后来发现，采用线程池的方法能更好的利用线程资源来计算任务，网上有很多关于如何运行线程池的例子，msdn上也给出了对应的例子：https://msdn.microsoft.com/en-us/library/windows/desktop/ms686980(v=vs.85).aspx

感兴趣的话大家可以去看看，这里我给出一个简单的demo，利用线程池单次调用多次调用，例子如下：





**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50178339)[copy](http://blog.csdn.net/xiaxiazls/article/details/50178339)



- #include<Windows.h>  
- #include<iostream>  
- #include<cstdlib>  
- 
- using namespace std;  
- 
- #define THREAD_NUM 10  
- 
- struct tempStruct  
- {  
- int a;  
- int b;  
- };  
- 
- VOID CALLBACK SimpleCallback(PTP_CALLBACK_INSTANCE Instance,void* Context);  
- VOID CALLBACK WorkCallbackTemp(PTP_CALLBACK_INSTANCE Instance, void* Context, PTP_WORK Work);  
- 
- void main()  
- {  
-     PTP_WORK tpWork[THREAD_NUM];  
-     tempStruct transferStruct[THREAD_NUM];  
- 
- for ( int i = 0; i< THREAD_NUM; i++ )  
-     {  
-         transferStruct[i].a = i;  
-         transferStruct[i].b = i+1;  
-     }  
- 
- //单次工作提交  
-     TrySubmitThreadpoolCallback(SimpleCallback,&transferStruct[2],NULL);  
- 
- for ( int i = 0; i< THREAD_NUM; i++ )  
-         tpWork[i] = CreateThreadpoolWork(WorkCallbackTemp,&transferStruct[i],NULL);  
- 
- //提交工作  
- for ( int i = 0; i< THREAD_NUM; i++ )  
-         SubmitThreadpoolWork(tpWork[i]);  
- 
- //等待工作结束  
- for ( int i = 0; i< THREAD_NUM; i++ )  
-         WaitForThreadpoolWorkCallbacks(tpWork[i],false);  
- 
- //关闭工作对象  
- for ( int i = 0; i< THREAD_NUM; i++ )  
-         CloseThreadpoolWork(tpWork[i]);   
- 
-     system("pause");  
- }  
- 
- VOID CALLBACK SimpleCallback(PTP_CALLBACK_INSTANCE Instance,void* Context)  
- {  
-     tempStruct *pt = (tempStruct *)Context;  
- 
- int pruduct = pt->a * pt->b;  
-     cout <<"Simple struct a: "<<pt->a<<" struct_temp.b: "<<pt->b<<" pruduct: "<<pruduct<<endl;  
- }  
- 
- VOID CALLBACK WorkCallbackTemp(PTP_CALLBACK_INSTANCE Instance, void* Context, PTP_WORK Work)  
- {  
-     tempStruct *pt = (tempStruct *)Context;  
- 
- int sum = pt->a + pt->b;  
-     cout <<"Work struct a: "<<pt->a<<" struct_temp.b: "<<pt->b<<" sum: "<<sum<<endl;  
- }  




PS有个网站的教程写得很好，收益很多大家也可以去看看：http://www.cnblogs.com/kzloser/archive/2013/03/11/2909221.html

做个mark,希望对需要的朋友有帮助！









