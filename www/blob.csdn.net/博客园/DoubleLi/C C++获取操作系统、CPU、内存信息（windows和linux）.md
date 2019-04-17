# C/C++获取操作系统、CPU、内存信息（windows和linux） - DoubleLi - 博客园






有时候需要在工程里面获取一些系统或者硬件的信息，比如系统版本，cpu，内存，显卡，硬盘等，作为后续软件功能判断的依据，甚至参与性能算法自适应建模

Windows

操作系统和内存信息在windows下通过系统的API来获取，CPU信息则需要需要通过底层CPUID指令取得

代码：





**[cpp]**[view plain](http://blog.csdn.net/u012234115/article/details/71056957#)[copy](http://blog.csdn.net/u012234115/article/details/71056957#)

[print](http://blog.csdn.net/u012234115/article/details/71056957#)[?](http://blog.csdn.net/u012234115/article/details/71056957#)

- #include <iostream>   
- #include <string>  
- #include <windows.h>    
- 
- #pragma warning(disable: 4996) // avoid GetVersionEx to be warned  
- 
- // ***** global macros ***** //  
- static const int kMaxInfoBuffer = 256;  
- #define  GBYTES  1073741824    
- #define  MBYTES  1048576    
- #define  KBYTES  1024    
- #define  DKBYTES 1024.0    
- 
- // ---- get os info ---- //  
- void getOsInfo()  
- {  
- // get os name according to version number  
-     OSVERSIONINFO osver = {sizeof(OSVERSIONINFO)};  
-     GetVersionEx(&osver);  
-     std::string os_name;  
- if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)  
-         os_name = "Windows 2000";  
- else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
-         os_name = "Windows XP";  
- else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)  
-         os_name = "Windows 2003";  
- else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
-         os_name = "windows vista";  
- else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)  
-         os_name = "windows 7";  
- else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)  
-         os_name = "windows 10";  
- 
-     std::cout << "os name: " << os_name << std::endl;  
-     std::cout << "os version: " << osver.dwMajorVersion << '.' << osver.dwMinorVersion << std::endl;  
- 
- }  
- 
- // ---- get cpu info ---- //  
- // save 4 register variables  
- DWORD deax;  
- DWORD debx;  
- DWORD decx;  
- DWORD dedx;  
- 
- // init cpu in assembly language  
- void initCpu(DWORD veax)    
- {  
-     __asm  
-     {  
-         mov eax, veax  
-         cpuid  
-             mov deax, eax  
-             mov debx, ebx  
-             mov decx, ecx  
-             mov dedx, edx  
-     }  
- }  
- 
- long getCpuFreq()         
- {  
- int start, over;  
-     _asm  
-     {  
-         RDTSC  
-         mov start, eax  
-     }  
-     Sleep(50);  
-     _asm  
-     {  
-         RDTSC  
-         mov over, eax  
-     }  
- return (over - start) / 50000;  
- }  
- 
- std::string getManufactureID()      
- {  
- char manuID[25];  
-     memset(manuID, 0, sizeof(manuID));  
- 
-     initCpu(0);          
-     memcpy(manuID + 0, &debx, 4); // copy to array  
-     memcpy(manuID + 4, &dedx, 4);  
-     memcpy(manuID + 8, &decx, 4);  
- 
- return manuID;  
- }  
- 
- std::string getCpuType()  
- {  
- const DWORD id = 0x80000002; // start 0x80000002 end to 0x80000004  
- char cpuType[49];  
-     memset(cpuType, 0, sizeof(cpuType));  
- 
- for (DWORD t = 0; t < 3; t++)  
-     {  
-         initCpu(id + t);  
- 
-         memcpy(cpuType + 16 * t + 0, &deax, 4);  
-         memcpy(cpuType + 16 * t + 4, &debx, 4);  
-         memcpy(cpuType + 16 * t + 8, &decx, 4);  
-         memcpy(cpuType + 16 * t + 12, &dedx, 4);  
-     }  
- 
- return cpuType;  
- }  
- 
- void getCpuInfo()  
- {  
-     std::cout << "CPU main frequency: " << getCpuFreq() << "MHz" << std::endl;  
-     std::cout << "CPU manufacture: " << getManufactureID() << std::endl;  
-     std::cout << "CPU type: " << getCpuType() << std::endl;  
- }  
- 
- // ---- get memory info ---- //  
- void getMemoryInfo()  
- {  
-     std::string memory_info;  
-     MEMORYSTATUSEX statusex;  
-     statusex.dwLength = sizeof(statusex);  
- if (GlobalMemoryStatusEx(&statusex))  
-     {  
-         unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;  
- double decimal_total = 0, decimal_avl = 0;  
-         remain_total = statusex.ullTotalPhys % GBYTES;  
-         total = statusex.ullTotalPhys / GBYTES;  
-         avl = statusex.ullAvailPhys / GBYTES;  
-         remain_avl = statusex.ullAvailPhys % GBYTES;  
- if (remain_total > 0)  
-             decimal_total = (remain_total / MBYTES) / DKBYTES;  
- if (remain_avl > 0)  
-             decimal_avl = (remain_avl / MBYTES) / DKBYTES;  
- 
-         decimal_total += (double)total;  
-         decimal_avl += (double)avl;  
- char  buffer[kMaxInfoBuffer];  
-         sprintf_s(buffer, kMaxInfoBuffer, "total %.2f GB (%.2f GB available)", decimal_total, decimal_avl);  
-         memory_info.append(buffer);  
-     }  
-     std::cout << memory_info << std::endl;  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     std::cout << "===os information===" << std::endl;  
-     getOsInfo();  
- 
-     std::cout << "===cpu infomation===" << std::endl;  
-     getCpuInfo();  
- 
-     std::cout << "===memory information===" << std::endl;  
-     getMemoryInfo();  
- 
-     system("pause");  
- return 0;  
- }  






结果：





**[plain]**[view plain](http://blog.csdn.net/u012234115/article/details/71056957#)[copy](http://blog.csdn.net/u012234115/article/details/71056957#)

[print](http://blog.csdn.net/u012234115/article/details/71056957#)[?](http://blog.csdn.net/u012234115/article/details/71056957#)

- ===os information===  
- os name: windows 10  
- os version: 6.2  
- ===cpu infomation===  
- CPU main frequency: 2612MHz  
- CPU manufacture: GenuineIntel  
- CPU type:        Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz  
- ===memory information===  
- total 7.86 GB (1.38 GB available)  






Linux
linux下很多信息都是存放着系统的/proc目录下，因此读文件就可以获取到了

代码：





**[cpp]**[view plain](http://blog.csdn.net/u012234115/article/details/71056957#)[copy](http://blog.csdn.net/u012234115/article/details/71056957#)

[print](http://blog.csdn.net/u012234115/article/details/71056957#)[?](http://blog.csdn.net/u012234115/article/details/71056957#)

- #include <stdio.h>  
- #include <stdlib.h>  
- #include <string.h>  
- 
- // read the linux config files to get system info  
- 
- void getOsInfo()  
- {  
- FILE *fp = fopen("/proc/version", "r");    
- if(NULL == fp)     
-         printf("failed to open version\n");     
- char szTest[1000] = {0};    
- while(!feof(fp))    
-     {    
-         memset(szTest, 0, sizeof(szTest));    
-         fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n  
-         printf("%s", szTest);    
-     }    
-     fclose(fp);   
- }  
- 
- void getCpuInfo()  
- {  
- FILE *fp = fopen("/proc/cpuinfo", "r");    
- if(NULL == fp)     
-         printf("failed to open cpuinfo\n");     
- char szTest[1000] = {0};     
- // read file line by line   
- while(!feof(fp))    
-     {    
-         memset(szTest, 0, sizeof(szTest));    
-         fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n  
-         printf("%s", szTest);    
-     }    
-     fclose(fp);   
- }  
- 
- void getMemoryInfo()  
- {  
- FILE *fp = fopen("/proc/meminfo", "r");    
- if(NULL == fp)     
-         printf("failed to open meminfo\n");     
- char szTest[1000] = {0};    
- while(!feof(fp))    
-     {    
-         memset(szTest, 0, sizeof(szTest));    
-         fgets(szTest, sizeof(szTest) - 1, fp);   
-         printf("%s", szTest);    
-     }    
-     fclose(fp);   
- }  
- 
- int main(int argc, char **argv)  
- {  
-     printf("===os information===\n");  
-     getOsInfo();  
- 
-     printf("===cpu infomation===\n");  
-     getCpuInfo();  
- 
-     printf("===memory information===\n");  
-     getMemoryInfo();  
- 
- return 0;  
- }  







结果：





**[plain]**[view plain](http://blog.csdn.net/u012234115/article/details/71056957#)[copy](http://blog.csdn.net/u012234115/article/details/71056957#)

[print](http://blog.csdn.net/u012234115/article/details/71056957#)[?](http://blog.csdn.net/u012234115/article/details/71056957#)

- ===os information===  
- Linux version 4.8.6-300.fc25.x86_64 (mockbuild@bkernel02.phx2.fedoraproject.org) (gcc version 6.2.1 20160916 (Red Hat 6.2.1-2) (GCC) ) #1 SMP Tue Nov 1 12:36:38 UTC 2016  
- ===cpu infomation===  
- processor   : 0  
- vendor_id   : GenuineIntel  
- cpu family  : 6  
- model       : 58  
- model name  : Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz  
- stepping    : 9  
- microcode   : 0x1b  
- cpu MHz     : 2599.755  
- cache size  : 3072 KB  
- ===memory information===  
- MemTotal:        3061552 kB  
- MemFree:           89584 kB  
- MemAvailable:     596876 kB  
- Buffers:           70372 kB  
- Cached:           707548 kB  
- SwapCached:          392 kB  







PS：硬盘和网络参数也可以通过这种方法拿到，显卡参数可以用制造商提供的驱动API来取得









