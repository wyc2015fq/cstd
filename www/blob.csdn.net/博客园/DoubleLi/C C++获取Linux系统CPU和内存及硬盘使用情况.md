# C/C++获取Linux系统CPU和内存及硬盘使用情况 - DoubleLi - 博客园






需求分析：

不使用Top  df  free 等命令，利用C/C++获取Linux系统CPU和内存及硬盘使用情况

实现：





**[cpp]**[view plain](http://blog.csdn.net/happycheng123/article/details/50790625#)[copy](http://blog.csdn.net/happycheng123/article/details/50790625#)



- //通过获取/proc/stat (CPU)和/proc/meminfo(内存和硬盘)文件信息  
- //proc/meminfo文件结构  
- //MemTotal:       515164 kB  
- //MemFree:         15024 kB  
- //Buffers:         13740 kB  
- //Cached:         248824 kB  
- //SwapCached:    960 kB  
- //Active:         309980 kB  
- //Inactive:       63420 kB  
- typedef struct MEMPACKED         //定义一个mem occupy的结构体  
- {  
- <span style="white-space:pre">    </span>char name1[20];      //定义一个char类型的数组名name有20个元素  
- <span style="white-space:pre">    </span>unsigned long MemTotal;  
- <span style="white-space:pre">    </span>char name2[20];  
- <span style="white-space:pre">    </span>unsigned long MemFree;  
- <span style="white-space:pre">    </span>char name3[20];  
- <span style="white-space:pre">    </span>unsigned long Buffers;  
- <span style="white-space:pre">    </span>char name4[20];  
- <span style="white-space:pre">    </span>unsigned long Cached;  
- <span style="white-space:pre">    </span>char name5[20];  
- <span style="white-space:pre">    </span>unsigned long SwapCached;  
- }MEM_OCCUPY;  
- 
- //proc/stat文件结构  
- //cpu  633666 46912 249878 176813696 782884 2859 19625 0  
- //cpu0 633666 46912 249878 176813696 782884 2859 19625 0  
- //intr 5812844  
- //ctxt 265816063  
- //btime 1455203832  
- //processes 596625  
- //procs_running 1  
- //procs_blocked 0  
- 
- typedef struct CPUPACKED         //定义一个cpu occupy的结构体  
- {  
- <span style="white-space:pre">    </span>char name[20];      //定义一个char类型的数组名name有20个元素  
- <span style="white-space:pre">    </span>unsigned int user; //定义一个无符号的int类型的user  
- <span style="white-space:pre">    </span>unsigned int nice; //定义一个无符号的int类型的nice  
- <span style="white-space:pre">    </span>unsigned int system;//定义一个无符号的int类型的system  
- <span style="white-space:pre">    </span>unsigned int idle; //定义一个无符号的int类型的idle  
- <span style="white-space:pre">    </span>unsigned int lowait;  
- <span style="white-space:pre">    </span>unsigned int irq;  
- <span style="white-space:pre">    </span>unsigned int softirq;  
- }CPU_OCCUPY;  
- 
- 
- void get_memoccupy(MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O  
- {  
- <span style="white-space:pre">    </span>FILE *fd;  
- <span style="white-space:pre">    </span>char buff[256];  
- <span style="white-space:pre">    </span>MEM_OCCUPY *m;  
- <span style="white-space:pre">    </span>m = mem;  
- 
- <span style="white-space:pre">    </span>fd = fopen("/proc/meminfo", "r");  
- <span style="white-space:pre">    </span>//MemTotal: 515164 kB  
- <span style="white-space:pre">    </span>//MemFree: 7348 kB  
- <span style="white-space:pre">    </span>//Buffers: 7892 kB  
- <span style="white-space:pre">    </span>//Cached: 241852  kB  
- <span style="white-space:pre">    </span>//SwapCached: 0 kB  
- <span style="white-space:pre">    </span>//从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里   
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);  
- <span style="white-space:pre">    </span>sscanf(buff, "%s %lu ", m->name1, &m->MemTotal);  
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);  
- <span style="white-space:pre">    </span>sscanf(buff, "%s %lu ", m->name2, &m->MemFree);  
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);  
- <span style="white-space:pre">    </span>sscanf(buff, "%s %lu ", m->name3, &m->Buffers);  
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);  
- <span style="white-space:pre">    </span>sscanf(buff, "%s %lu ", m->name4, &m->Cached);  
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);   
- <span style="white-space:pre">    </span>sscanf(buff, "%s %lu", m->name5, &m->SwapCached);  
- 
- <span style="white-space:pre">    </span>fclose(fd);     //关闭文件fd  
- }  
- 
- 
- int get_cpuoccupy(CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O  
- {  
- <span style="white-space:pre">    </span>FILE *fd;  
- <span style="white-space:pre">    </span>char buff[256];  
- <span style="white-space:pre">    </span>CPU_OCCUPY *cpu_occupy;  
- <span style="white-space:pre">    </span>cpu_occupy = cpust;  
- 
- <span style="white-space:pre">    </span>fd = fopen("/proc/stat", "r");  
- <span style="white-space:pre">    </span>fgets(buff, sizeof(buff), fd);  
- 
- <span style="white-space:pre">    </span>sscanf(buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle, &cpu_occupy->lowait, &cpu_occupy->irq, &cpu_occupy->softirq);  
- 
- 
- <span style="white-space:pre">    </span>fclose(fd);  
- 
- <span style="white-space:pre">    </span>return 0;  
- }  
- 
- 
- int cal_cpuoccupy(CPU_OCCUPY *o, CPU_OCCUPY *n)  
- {  
- <span style="white-space:pre">    </span>unsigned long od, nd;  
- <span style="white-space:pre">    </span>double cpu_use = 0;  
- 
- <span style="white-space:pre">    </span>od = (unsigned long)(o->user + o->nice + o->system + o->idle + o->lowait + o->irq + o->softirq);//第一次(用户+优先级+系统+空闲)的时间再赋给od  
- <span style="white-space:pre">    </span>nd = (unsigned long)(n->user + n->nice + n->system + n->idle + n->lowait + n->irq + n->softirq);//第二次(用户+优先级+系统+空闲)的时间再赋给od  
- <span style="white-space:pre">    </span>double sum = nd - od;  
- <span style="white-space:pre">    </span>double idle = n->idle - o->idle;  
- <span style="white-space:pre">    </span>cpu_use = idle / sum;  
- <span style="white-space:pre">    </span>printf("cpu_use1(idle) = %f\r\n", cpu_use);  
- <span style="white-space:pre">    </span>idle = n->user + n->system + n->nice - o->user - o->system - o->nice;  
- <span style="white-space:pre">    </span>cpu_use = idle / sum;  
- <span style="white-space:pre">    </span>printf("cpu_use2(user+system+nice) = %f\r\n", cpu_use);  
- <span style="white-space:pre">    </span>return 0;  
- }  
- 
- int main( int argc, char **argv )  
- {  
- <span style="white-space:pre">    </span>MEM_OCCUPY mem_stat;  
- <span style="white-space:pre">    </span>CPU_OCCUPY cpu_stat1;  
- <span style="white-space:pre">    </span>CPU_OCCUPY cpu_stat2;  
- 
- <span style="white-space:pre">    </span>CAfcLowerTools m_afcLowerTools;  
- 
- //获取内存  
- <span style="white-space:pre">    </span>get_memoccupy((MEM_OCCUPY *)&mem_stat);  
- <span style="white-space:pre">    </span>printf(" [MemTotal] = %lu \n [MemFree] = %lu \n [Buffers] = %lu \n [Cached] = %lu \n [SwapCached] = %lu \n", mem_stat.MemTotal, mem_stat.MemFree, mem_stat.Buffers, mem_stat.Cached, mem_stat.SwapCached);  
- 
- //第一次获取cpu使用情况  
- <span style="white-space:pre">    </span>get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);  
- <span style="white-space:pre">    </span>Sleep(1000);  
- //第二次获取cpu使用情况  
- <span style="white-space:pre">    </span>get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);  
- //计算cpu使用率  
- <span style="white-space:pre">    </span>cal_cpuoccupy((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);  
- 
- }  










