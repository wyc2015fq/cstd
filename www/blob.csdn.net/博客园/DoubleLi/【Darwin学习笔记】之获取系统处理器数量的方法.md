# 【Darwin学习笔记】之获取系统处理器数量的方法 - DoubleLi - 博客园






阅读Darwin源码的时候看到这个方法，感觉挺有用处，且考虑了多种平台下的实现方式，直接贴代码，以后说不定会用到~

单一种平台下的实现方法可能很容易，但是把这些个系统都收集在一起，在一个函数中实现还是极好的

【转载请注明出处】：http://blog.csdn.net/longlong530


- 



UInt32 OS::GetNumProcessors()


- 



{


- 



#if (__Win32__)


- 



SYSTEM_INFO theSystemInfo;


- 



::GetSystemInfo(&theSystemInfo);


- 




- 



return (UInt32)theSystemInfo.dwNumberOfProcessors;


- 



#endif


- 




- 



#if (__MacOSX__ || __FreeBSD__)


- 



int numCPUs = 1;


- 



size_t len = sizeof(numCPUs);


- 



int mib[2];


- 



mib[0] = CTL_HW;


- 



mib[1] = HW_NCPU;


- 



(void) ::sysctl(mib,2,&numCPUs,&len,NULL,0);


- 



if (numCPUs < 1) 


- 



numCPUs = 1;


- 



return (UInt32) numCPUs;


- 



#endif


- 




- 



#if(__linux__ || __linuxppc__)


- 




- 



char cpuBuffer[8192] = "";


- 



StrPtrLen cpuInfoBuf(cpuBuffer, sizeof(cpuBuffer));


- 



FILE *cpuFile = ::fopen( "/proc/cpuinfo", "r" );


- 



if (cpuFile)


- 



{ cpuInfoBuf.Len = ::fread(cpuInfoBuf.Ptr, sizeof(char), cpuInfoBuf.Len, cpuFile);


- 



::fclose(cpuFile);


- 



}


- 




- 



StringParser cpuInfoFileParser(&cpuInfoBuf);


- 



StrPtrLen line;


- 



StrPtrLen word;


- 



UInt32 numCPUs = 0;


- 




- 



while( cpuInfoFileParser.GetDataRemaining() != 0 ) 


- 



{


- 



cpuInfoFileParser.GetThruEOL(&line); // Read each line 


- 



StringParser lineParser(&line);


- 



lineParser.ConsumeWhitespace(); //skip over leading whitespace


- 




- 



if (lineParser.GetDataRemaining() == 0) // must be an empty line


- 



continue;


- 




- 



lineParser.ConsumeUntilWhitespace(&word);


- 




- 



if ( word.Equal("processor") ) // found a processor as first word in line


- 



{ numCPUs ++;


- 



}


- 



}


- 




- 



if (numCPUs == 0)


- 



numCPUs = 1;


- 




- 



return numCPUs;


- 



#endif


- 




- 



#if(__solaris__)


- 



{


- 



UInt32 numCPUs = 0;


- 



char linebuff[512] = "";


- 



StrPtrLen line(linebuff, sizeof(linebuff));


- 



StrPtrLen word;


- 




- 



FILE *p = ::popen("uname -X","r");


- 



while((::fgets(linebuff, sizeof(linebuff -1), p)) > 0)


- 



{


- 



StringParser lineParser(&line);


- 



lineParser.ConsumeWhitespace(); //skip over leading whitespace


- 




- 



if (lineParser.GetDataRemaining() == 0) // must be an empty line


- 



continue;


- 




- 



lineParser.ConsumeUntilWhitespace(&word);


- 




- 



if ( word.Equal("NumCPU")) // found a tag as first word in line


- 



{


- 



lineParser.GetThru(NULL,'=');


- 



lineParser.ConsumeWhitespace(); //skip over leading whitespace


- 



lineParser.ConsumeUntilWhitespace(&word); //read the number of cpus


- 



if (word.Len > 0)


- 



::sscanf(word.Ptr, "%"_U32BITARG_"", &numCPUs);


- 




- 



break;


- 



}


- 



}


- 



if (numCPUs == 0)


- 



numCPUs = 1;


- 




- 



::pclose(p);


- 




- 



return numCPUs;


- 



}


- 



#endif


- 




- 



#if(__sgi__) 


- 



UInt32 numCPUs = 0;


- 




- 



numCPUs = sysconf(_SC_NPROC_ONLN);


- 




- 



return numCPUs;


- 



#endif 


- 




- 




- 



return 1;


- 



}




PS:  函数摘自Darwin源码：\StreamServer\CommonUtilitiesLib\OS.cpp









