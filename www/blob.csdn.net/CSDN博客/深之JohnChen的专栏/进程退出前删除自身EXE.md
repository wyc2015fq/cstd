# 进程退出前删除自身EXE  - 深之JohnChen的专栏 - CSDN博客

2005年11月26日 18:03:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4460


下面的代码由Gary Nebbett写就.Gary Nebbett乃是WINDOWS NT/2000 NATIVE API REFERENCE的作者.乃NT系统一等一的高手.下面就分析一些他的这段代码. 
这段代码在PROCESS没有结束前就将启动PROCESS的EXE文件删除了. 
int main(int argc, char *argv[]) 
{ 
HMODULE module = GetModuleHandle(0); 
CHAR buf[MAX_PATH]; 
GetModuleFileName(module, buf, sizeof buf); 
CloseHandle(HANDLE(4)); 
__asm { 
lea eax, buf 
push 0 
push 0 
push eax 
push ExitProcess 
push module 
push DeleteFile 
push UnmapViewOfFile 
ret 
} 
return 0; 
} 
现在,我们先看一下堆栈中的东西 

偏移 内容 
24 0 
20 0 
16 offset buf 
12 address of ExitProcess 
8 module 
4 address of DeleteFile 
0 address of UnmapViewOfFile 

调用RET返回到了UnmapViewOfFile,也就是栈里的偏移0所指的地方.当进入UnmapViewOfFile的流程时,栈里见到的是返回地址DeleteFile和HMODUL module.也就是说调用完毕后返回到了DeleteFile的入口地址.当返回到DeleteFile时,看到了ExitProcess的地址,也就是返回地址.和参数EAX,而EAX则是buffer.buffer存的是EXE的文件名.由GetModuleFileName(module, buf, sizeof buf)返回得到.执行了DeleteFile后,就返回到了ExitProcess的函数入口.并且参数为0而返回地址也是0.0是个非法地址.如果返回到地址0则会出错.而调用ExitProcess则应该不会返回. 
这段代码的精妙之处在于: 
1.如果有文件的HANDLE打开,文件删除就会失败,所以,CloseHandle(HANDLE(4));是十分巧妙的一手.HANDLE4是OS的硬编码,对应于EXE的IMAGE.在缺省情况下,OS假定没有任何调用会关闭IMAGE SECTION的HANDLE,而现在,该HANDLE被关闭了.删除文件就解除了文件对应的一个句柄. 
2.由于UnmapViewOfFile解除了另外一个对应IMAGE的HANDLE,而且解除了IMAGE在内存的映射.所以,后面的任何代码都不可以引用IMAGE映射地址内的任何代码.否则就OS会报错.而现在的代码在UnmapViewOfFile后则刚好没有引用到任何IMAGE内的代码. 
3.在ExitProcess之前,EXE文件就被删除了.也就是说,进程尚在,而主线程所在的EXE文件已经没了.(WINNT/9X都保护这些被映射到内存的WIN32 IMAGE不被删除.)

