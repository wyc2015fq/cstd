# 作业——Windows核心编程学习手札系列之五 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年12月15日 17:25:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2365








# 作业

### ——Windows核心编程学习手札系列之五

Windows提供作业内核对象，可以将进程组合在千毫，并创建一个“沙框”以便限制进程能够进行的操作。作业可视为进程的容器，windows98不支持作业的操作。创建作业内核对象的函数是Handle CraeteJobObject(PSECURITY_ATTRIBUTES psa,PCTSTR pszName)；与其他内核对象一样，第一个参数将安全信息与作业对象关联起来，并告诉系统是否使返回的句柄成为可继承的句柄；第二个参数是给对象命名，便于另一个进程通过函数HANDLE OpenJobObject(DWORD dwDesiredAccess,BOOL bInheritHandle,PCTSTR pszName)进行访问；如不再访问代码中的作业对象，通过调用CloseHandle来关闭它的句柄。

进程创建后，通常需要设置“沙框”，限制其只能进行一定范围内的限制，对作业可以加上如下一些类型的限制：基本限制和扩展基本限制，用于防止作业中的进程垄断系统的资源；基本的UI限制，用于防止作业中的进程改变用户界面；安全性限制，用于防止作业中的进程访问保密资源（文件、注册表子关键字等）。调用函数

HANDLE SetInformationJobObject(HANDLE hJob,

JOBOBJECTINFOCLASS jobObjectInformationClass,

PVOID pJobObjectInformation,

DWORD cbJobObjectInformationLength)，可以给作业加上各种限制。

第一个参数用于标识要限制的作业，第二个参数是枚举类型，用于指明要使用的限制类型，第三个参数包含限制设置值的数据结构的地址，第四个参数指明该结构的大小。一旦作业设置了条件，可以通过函数

BOOL QueryInformationJobObject(HANDLE hJob,

JOBOBJECTINFOCLASS JobObjectInformationClass,

PVOID pvJobObjectInformation,

DWORD cbJobObjectInformationLength,

PDWORD pdwReturnLength)查询已设置的限制。要为该函数传递作业的句柄，包括用于指明限制信息的枚举类型、函数初始化的数据结构地址、以及包含该结构的数据块大小，最后一个参数告诉你有多少字节将放入缓存。

函数BOOL AssignProcessToJobObject(HANDLE hJob,HANDLE hProcess)用于告诉系统将进程（由hProcess标识）视为现有作业（由hJob标识）的一部分。该函数只运行将尚未被赋予任何作业的进程赋予给一个作业。一旦一个进程成为一个作业的组成部分，就不能转到另一个作业，并且不能是无作业的进程，此外当作为作业一部分的进程生成另一个进程时，新进程将自动成为父作业的组成部分。要撤消作业中的进程，需要调用BOOL TerminateJobObject(HANDLE hJob,UINT uExitCode)函数，类似为每个进程调用TerminateProcess函数，将它们所有退出代码设置为uExitCode。

下面的代码演示了创建作业并设置基本和UI两种设置，并将CMD进程加入到沙框，具体代码如下：

#define_WIN32_WINNT0x0500

#include <Windows.h> 

voidStartRestrictedProcess(void)

{

//Createajobkernelobject.

HANDLEhjob=CreateJobObject(NULL,NULL);



//Placesomerestrictionsonprocessesinthejob.

//First,setsomebasicrestrictions.

JOBOBJECT_BASIC_LIMIT_INFORMATIONjobli={0};

//Theprocessalwaysrunsintheidlepriorityclass.

jobli.PriorityClass=IDLE_PRIORITY_CLASS;



//Thejobcannotusemorethan1secondofCPUtime.

jobli.PerJobUserTimeLimit.QuadPart=10000000;

//1secin100-nsintervals 



//Thesearetheonly2restrictionsIwantplacedonthejob(process). 

jobli.LimitFlags=JOB_OBJECT_LIMIT_PRIORITY_CLASS|

JOB_OBJECT_LIMIT_JOB_TIME;



SetInformationJobObject(hjob,

JobObjectBasicLimitInformation,

&jobli,sizeof(jobli));



//Second,setsomeUIrestrictions. 

JOBOBJECT_BASIC_UI_RESTRICTIONSjobuir;

jobuir.UIRestrictionsClass=JOB_OBJECT_UILIMIT_NONE;

//Afancyzero



//Theprocesscan'tlogoffthesystem.

jobuir.UIRestrictionsClass|=JOB_OBJECT_UILIMIT_EXITWINDOWS; 

//Theprocesscan'taccessUSERobjects

//(suchasotherwindows)inthesystem.

jobuir.UIRestrictionsClass|=JOB_OBJECT_UILIMIT_HANDLES;



SetInformationJobObject(hjob,JobObjectBasicUIRestrictions,

&jobuir,sizeof(jobuir)); 



//Spawntheprocessthatistobeinthejob.

//Note:Youmustfirstspawntheprocessandthenplacetheprocessin

//thejob.Thismeansthattheprocess'sthreadmustbeinitially

//suspendedsothatitcan'texecuteanycodeoutside

//ofthejob'srestrictions.

STARTUPINFOsi={sizeof(si)};

PROCESS_INFORMATIONpi;

CreateProcess(NULL,"CMD",NULL,NULL,FALSE,

CREATE_SUSPENDED,NULL,NULL,&si,&pi);

//Placetheprocessinthejob.

//Note:ifthisprocessspawnsanychildren,thechildrenare

//automaticallypartofthesamejob.

AssignProcessToJobObject(hjob,pi.hProcess);



//Nowwecanallowthechildprocess'sthreadtoexecutecode.

ResumeThread(pi.hThread);

CloseHandle(pi.hThread);



//Waitfortheprocesstoterminateorforallthejob's

//allottedCPUtimetobeused.

HANDLEh[2];

h[0]=pi.hProcess;

h[1]=hjob;

DWORDdw=WaitForMultipleObjects(2,h,FALSE,INFINITE);

switch(dw-WAIT_OBJECT_0)

{ 

case0:

//Theprocesshasterminated... 

printf("the process has terminated!");

break;

case1:

//Allofthejob'sallottedCPUtimewasused...

printf("CPU time has used!");

break;

}

//Cleanupproperly.

CloseHandle(pi.hProcess);

CloseHandle(hjob);

}

<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />







int main(int argc, char* argv[])

{

//printf("Hello World!/n");

StartRestrictedProcess();

return 0;

}

如非<?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />2008-12-15



