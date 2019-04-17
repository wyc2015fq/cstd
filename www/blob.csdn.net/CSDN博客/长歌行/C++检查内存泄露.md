# C++检查内存泄露 - 长歌行 - CSDN博客





2013年01月11日 12:31:12[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2900








说明，我使用的ide是vs2008







1. 工程设置为debug

内存泄露的检测一般在debug模式下进行




2.在需要检查内存泄露的cpp头部加上



```cpp
#ifdef	_DEBUG
 #define _CRTDBG_MAP_ALLOC
 #include <stdlib.h>
 #include <crtdbg.h>
 	#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
 #endif
```

3.代码中插入这么一句话
EnableMemLeakCheck();




```cpp
inline void EnableMemLeakCheck()
 {
 	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
 }
```


4.然后就可以在输出中看泄露情况了

举个例子，例子中我用newEx表示的上述宏定义中的new



```cpp
int _tmain(int argc, _TCHAR* argv[])
 {
 	EnableMemLeakCheck();
 	int num = 10;
 	byte **p = newEx byte *[num];
 	for (int i = 0; i < num; i ++)
 	{
 		Sleep(1);
 		*p = newEx byte[i];
 	}
 
 	long *pl = newEx long[100];
 
 	while(1)
 	{
 		Sleep(100);
 	}
 	return 0;
 }
```


输出中显示的内容（debug下运行程序，然后点叉叉关闭程序）

memory leaks!

 Dumping objects ->

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(101) : {124} normal block at 0x00295CB8, 400 bytes long.

  Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {122} normal block at 0x00294C30, 9 bytes long.

  Data: <         > CD CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {121} normal block at 0x00294BE8, 8 bytes long.

  Data: <        > CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {120} normal block at 0x00299F88, 7 bytes long.

  Data: <       > CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {119} normal block at 0x00299F40, 6 bytes long.

  Data: <      > CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {118} normal block at 0x00299EF8, 5 bytes long.

  Data: <     > CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {117} normal block at 0x00299EB8, 4 bytes long.

  Data: <    > CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {116} normal block at 0x00299E78, 3 bytes long.

  Data: <   > CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {115} normal block at 0x00299E38, 2 bytes long.

  Data: <  > CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {114} normal block at 0x00299DF8, 1 bytes long.

  Data: < > CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : {113} normal block at 0x00299DB8, 0 bytes long.

  Data: <> 励p

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(88) : {112} normal block at 0x00299D50, 40 bytes long.

  Data: <0L)             > 30 4C 29 00 CD CD CD CD CD CD CD CD CD CD CD CD 

 Object dump complete.





有文件名和行号，先申请的后释放。




扩展：

实际上是使用了另外的new，在dbgnew.cpp中，列一个例子说明：






```cpp
void *__CRTDECL operator new[](
         size_t cb,
         int nBlockUse,
         const char * szFileName,
         int nLine
         )
         _THROW1(_STD bad_alloc)
 {
     void *res = operator new(cb, nBlockUse, szFileName, nLine );
 
     RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));
 
     return res;
 }
```




注意，const char *szFileName可以使用__file__也可以使用自定义的类，经过重载为char*（感谢老马提供代码）

例如：



```cpp
char new_index_recorder_file_name[1024 * 1024][256];
 
 
 
 class new_index_recorder{
 
 public:
 
 	new_index_recorder(char* file, int line) : file(file), line(line){
 
 
 
 	} 
 
 	operator char*(){
 
 		static unsigned int index;
 
 		sprintf(new_index_recorder_file_name[index], "%s(%d) : %d ", file, line, index++);
 
 		return  new_index_recorder_file_name[index];
 
 	}
 
 private:
 
 	char* file;
 
 	int line; 
 
 };
```



当然，也可以用函数来返回char*指针



```cpp
int g_count = 0;
 
 class OperNew
 {
 public:
 	OperNew()
 	{	
 		g_count ++;
 		pC = new char[20];
 		memset(pC, 0, 10);
 		sprintf(pC, "No.%d", g_count);
 	}
 	char* GetChar()
 	{
 		return pC;
 	}
 private:
 	char * pC ;
 };
```


这样就能返回自定义的内容了。本次的返回加上了一个构造时候的序号，当然也可以添加时间等。结果如下：

e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(95) : 11 (95) : {123} normal block at 0x002B4C78, 400 bytes long.

  Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 10 (92) : {122} normal block at 0x002B4C30, 9 bytes long.

  Data: <         > CD CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 9 (92) : {121} normal block at 0x002B4BE8, 8 bytes long.

  Data: <        > CD CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 8 (92) : {120} normal block at 0x002B9F88, 7 bytes long.

  Data: <       > CD CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 7 (92) : {119} normal block at 0x002B9F40, 6 bytes long.

  Data: <      > CD CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 6 (92) : {118} normal block at 0x002B9EF8, 5 bytes long.

  Data: <     > CD CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 5 (92) : {117} normal block at 0x002B9EB8, 4 bytes long.

  Data: <    > CD CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 4 (92) : {116} normal block at 0x002B9E78, 3 bytes long.

  Data: <   > CD CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 3 (92) : {115} normal block at 0x002B9E38, 2 bytes long.

  Data: <  > CD CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 2 (92) : {114} normal block at 0x002B9DF8, 1 bytes long.

  Data: < > CD 

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(92) : 1 (92) : {113} normal block at 0x002B9DB8, 0 bytes long.

  Data: <> h鴌

 e:\project\test\test_mem_leak\test_mem_leak\test_mem_leak.cpp(88) : 0 (88) : {112} normal block at 0x002B9D50, 40 bytes long.

  Data: <0L+             > 30 4C 2B 00 CD CD CD CD CD CD CD CD CD CD CD CD 

 Object dump complete.

 程序“[8796] test_MEM_LEAK.exe: 本机”已退出，返回值为 -1073741510 (0xc000013a)。





小提示：

将输出文件拷贝到UE中，然后查找泄露行号出现的次数，可以计算出泄露的数目。









