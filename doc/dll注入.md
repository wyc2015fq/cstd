# dll注入

本词条缺少**概述图**，补充相关内容使词条更完整，还能快速升级，赶紧来编辑吧！

所谓DLL [1]  注入就是将一个DLL放进某个进程的[地址空间](https://baike.baidu.com/item/%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4)里，让它成为那个进程的一部分。要实现DLL注入，首先需要打开目标进程。



- 中文名

  dll注入

- 外文名

  hRemoteProcess

- 意    义

  将一个DLL放进进程的[地址空间](https://baike.baidu.com/item/%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4)里

- 方    法

  打开目标进程

**例：**

```cpp
hRemoteProcess = OpenProcess(　PROCESS_CREATE_THREAD | //允许远程创建线程
PROCESS_VM_OPERATION　| //允许远程VM操 [2]  作
PROCESS_VM_WRITE,　//允许远程VM写
FALSE, dwRemoteProcessId )
```

由于我们后面需要写入远程进程的[内存地址](https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E5%9C%B0%E5%9D%80)空间并建立远程线程，所以需要申请足够的权限（PROCESS_CREATE_THREAD、VM_OPERATION、VM_WRITE）。

如果进程打不开，以后的操作就别想了。进程打开后，就可以建立远[线程](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B)了，不过别急，先想想这个远线程的线程函数是什么？我们的目的是注入一个DLL。而且我们知道用[LoadLibrary](https://baike.baidu.com/item/LoadLibrary/9598035)可以加载一个DLL到本进程的[地址空间](https://baike.baidu.com/item/%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4)。于是，自然会想到如果可以在目标进程中调用LoadLibrary，不就可以把DLL加载到目标进程的地址空间了吗？对！就是这样。远线程就在这儿用了一次，建立的远线程的线程函数就是LoadLibrary，而参数就是要注入的DLL的文件名。(这里需要自己想一想，注意到了吗，线程函数[ThreadProc](https://baike.baidu.com/item/ThreadProc/6454209)和LoadLibrary函数非常相似，[返回值](https://baike.baidu.com/item/%E8%BF%94%E5%9B%9E%E5%80%BC/9629649)，参数个数都一样) 还有一个问题，[LoadLibrary](https://baike.baidu.com/item/LoadLibrary/9598035)这个函数的地址在哪儿？也许你会说，这个简单，[GetProcAddress](https://baike.baidu.com/item/GetProcAddress/1633633)就可以得出。于是代码就出来了。

```cpp
char *pszLibFileRemote="my.dll";

PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");

CreateRemoteThread( hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL);
```

但是不对！不要忘了，这是远[线程](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B)，不是在你的进程里，而pszLibFileRemote指向的是你的进程里的数据，到了目标进程，这个[指针](https://baike.baidu.com/item/%E6%8C%87%E9%92%88)都不知道指向哪儿去了，同样pfnStartAddr这个地址上的代码到了目标进程里也不知道是什么了，不知道是不是你想要的LoadLibraryA了。但是，问题总是可以解决的，Windows有些很强大的[API函数](https://baike.baidu.com/item/API%E5%87%BD%E6%95%B0/9554990)，他们可以在目标进程里分配内存，可以将你的进程中的数据拷贝到目标进程中。因此pszLibFileRemote的问题可以解决了。

```cpp
char *pszLibFileName="my.dll";//注意，这个一定要是全路径文件名，除非它在系统目录里；原因大家自己想想。

//计算DLL路径名需要的内存空间

int cb = (1 + lstrlenA(pszLibFileName)) * sizeof(char);

//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名缓冲区

pszLibFileRemote = (char *) VirtualAllocEx( hRemoteProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);

//使用WriteProcessMemory函数将DLL的路径名复制到远程进程的内存空间

iReturnCode = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (PVOID) pszLibFileName, cb, NULL);
```

OK，现在目标进程也认识pszLibFileRemote了，但是pfnStartAddr好像不好办，我怎么可能知道LoadLibraryA在目标进程中的地址呢？其实Windows为我们解决了这个问题，LoadLibraryA这个函数是在Kernel32.dll这个核心DLL里的，而这个[DLL](https://baike.baidu.com/item/DLL)很特殊，不管对于哪个进程，Windows总是把它加载到相同的地址上去。因此你的进程中LoadLibraryA的地址和目标进程中LoadLibraryA的地址是相同的(其实，这个DLL里的所有函数都是如此)。至此，DLL注入结束了。

## 参考资料

1.[  ](https://baike.baidu.com/item/dll%E6%B3%A8%E5%85%A5/8420208?fr=aladdin#ref_[1]_2052068)[DLL动态链接库编写详解 ](https://baike.baidu.com/reference/8420208/76dc6AFTpfEn4lUdjVM6TCck8uXwDd-48m6DcVou-MpiebSSeojDLaVGzflZPH2GrIoM4YIMBo5hwA-KiY8iAK0QCA9CIMHOHMQJ13fD8kNA4myMLmPm) ．李岩的博客[引用日期2013-03-20]

2.[  ](https://baike.baidu.com/item/dll%E6%B3%A8%E5%85%A5/8420208?fr=aladdin#ref_[2]_2052068)[经典DLL注入实例 ](https://baike.baidu.com/reference/8420208/73a0CfcsOPNoABZ2JoFLcYA_8yLWurh3Kkk7Lky4fAfJFRTHJPydjvV6YUtHVQk8DwgNjGipgOSIcj0c) ．Rabbit's Blog[引用日期2012-07-14]

## 学术论文内容来自

- [赵北庚，孙楠.    远程线程DLL注入的实现与逆向分析侦查． ](https://xueshu.baidu.com/usercenter/paper/show?paperid=b5d4ec7c56bf8ac62ba6b347fb87d643&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&site=baike)《 网络安全技术与应用 》 ， 2015 

- [Hack.    利用目标资源实现进程的DLL注入． ](https://xueshu.baidu.com/usercenter/paper/show?paperid=7ebb49aa567eef4ec91381a042bde180&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&site=baike)《 黑客防线 》 ， 2008  

- [冰河洗剑.    DLL注入木马． ](https://xueshu.baidu.com/usercenter/paper/show?paperid=2edce41da75c0f8ed7f0e2a634644004&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&site=baike)《 电脑迷 》 ， 2006 

-  [张钟.    在远程进程中注入DLL钩挂IAT的方法． ](https://xueshu.baidu.com/usercenter/paper/show?paperid=3dceba8fafafa1403c5a29fb4a20ac09&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&site=baike)《 计算机与现代化 》 ， 2014 

-  [阙海忠.    使用DLL远程注入对记事本换肤． ](https://xueshu.baidu.com/usercenter/paper/show?paperid=c5f2449fd70278232182379bbdb1c5cb&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&site=baike)《 电脑编程技巧与维护 》 ， 2007

  [查看全部 ](https://xueshu.baidu.com/s?wd=intitle%3A%28dll%E6%B3%A8%E5%85%A5%29&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_from=pingtai7&site=baike)