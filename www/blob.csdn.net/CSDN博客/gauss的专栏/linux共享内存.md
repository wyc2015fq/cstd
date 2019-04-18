# linux共享内存 - gauss的专栏 - CSDN博客
2015年04月17日 02:03:48[gauss](https://me.csdn.net/mathlmx)阅读数：251
个人分类：[linux](https://blog.csdn.net/mathlmx/article/category/2303095)
2013-02-25 10:52
335人阅读[评论](http://blog.csdn.net/turkeyzhou/article/details/8608603#comments)(0)收藏[举报](http://blog.csdn.net/turkeyzhou/article/details/8608603#report)
共享内存是系统出于多个进程之间通讯的考虑，而预留的的一块内存区。在/proc/sys/kernel/目录下，记录着共享内存的一些限制，如一个共享内存区的最大字节数shmmax，系统范围内最大共享内存区标识符数shmmni等，可以手工对其调整，但不推荐这样做。
一、应用
共享内存的使用，主要有以下几个API：ftok()、shmget()、shmat()、shmdt()及shmctl()。
1)用ftok()函数获得一个ID号.
应用说明:
在IPC中，我们经常用用key_t的值来创建或者打开信号量，共享内存和消息队列。
函数原型:
key_t ftok(const char *pathname, int proj_id);
Keys：
1）pathname一定要在系统中存在并且进程能够访问的
3）proj_id是一个1－255之间的一个整数值，典型的值是一个ASCII值。
当成功执行的时候，一个key_t值将会被返回，否则－1被返回。我们可以使用strerror(errno)来确定具体的错误信息。
考虑到应用系统可能在不同的主机上应用，可以直接定义一个key，而不用ftok获得：
#define IPCKEY 0x344378
2)shmget()用来开辟/指向一块共享内存的函数
应用说明:
shmget（）用来获得共享内存区域的ID，如果不存在指定的共享区域就创建相应的区域。
函数原型:
int shmget(key_t key, size_t size, int shmflg);
key_t key 是这块共享内存的标识符。如果是父子关系的进程间通信的话，这个标识符用IPC_PRIVATE来代替。如果两个进程没有任何关系，所以就用ftok()算出来一个标识符（或者自己定义一个）使用了。
int size 是这块内存的大小．
int flag 是这块内存的模式(mode)以及权限标识。
模式可取如下值：        
IPC_CREAT 新建（如果已创建则返回目前共享内存的id）
IPC_EXCL   与IPC_CREAT结合使用，如果已创建则则返回错误
然后将“模式” 和“权限标识”进行“或”运算，做为第三个参数。
如：    IPC_CREAT | IPC_EXCL | 0640   
例子中的0666为权限标识,4/2/1 分别表示读/写/执行3种权限，第一个0是UID,第一个6（4+2）表示拥有者的权限，第二个4表示同组权限，第3个0表示他人的权限。
这个函数成功时返回共享内存的ID，失败时返回-1。
关于这个函数，要多说两句。
创建共享内存时，shmflg参数至少需要 IPC_CREAT | 权限标识，如果只有IPC_CREAT 则申请的地址都是k=0xffffffff，不能使用；
获取已创建的共享内存时，shmflg不要用IPC_CREAT（只能用创建共享内存时的权限标识，如0640），否则在某些情况下，比如用ipcrm删除共享内存后，用该函数并用IPC_CREAT参数获取一次共享内存（当然，获取失败），则即使再次创建共享内存也不能成功，此时必须更改key来重建共享内存。
3) shmat()将这个内存区映射到本进程的虚拟地址空间。
函数原型:
void    *shmat( int shmid , char *shmaddr , int shmflag );
shmat()是用来允许本进程访问一块共享内存的函数。
int shmid是那块共享内存的ID。
char *shmaddr是共享内存的起始地址，如果shmaddr为0，内核会把共享内存映像到调用进程的地址空间中选定位置；如果shmaddr不为0，内核会把共享内存映像到shmaddr指定的位置。所以一般把shmaddr设为0。
int shmflag是本进程对该内存的操作模式。如果是SHM_RDONLY的话，就是只读模式。其它的是读写模式
成功时，这个函数返回共享内存的起始地址。失败时返回-1。
4) shmdt()函数删除本进程对这块内存的使用，shmdt()与shmat()相反，是用来禁止本进程访问一块共享内存的函数。
函数原型:
int shmdt( char *shmaddr );
参数char *shmaddr是那块共享内存的起始地址。
成功时返回0。失败时返回-1。
5) shmctl() 控制对这块共享内存的使用
函数原型：
int     shmctl( int shmid , int cmd , struct shmid_ds *buf );
int shmid是共享内存的ID。
int cmd是控制命令，可取值如下：
        IPC_STAT        得到共享内存的状态
        IPC_SET         改变共享内存的状态
        IPC_RMID        删除共享内存
struct shmid_ds *buf是一个结构体指针。IPC_STAT的时候，取得的状态放在这个结构体中。如果要改变共享内存的状态，用这个结构体指定。
返回值：        成功：0
                失败：-1
示例程序:
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define IPCKEY 0x366378
typedef struct{
char agen[10];
unsigned char file_no;
} st_setting;
int main(int argc, char** argv)
{ 
    int shm_id;
    key_t key;
    st_setting *p_setting;
    //首先检查共享内存是否存在，存在则先删除
    shm_id = shmget(IPCKEY ,1028,0640);     
    if(shm_id != -1)
    {
        p_setting = (st_setting*)shmat(shm_id,NULL,0);
      if ( p_setting != (void *)-1)
      {
      shmdt(p_setting);
          shmctl(shm_id,IPC_RMID,0) ;
      }
    }
    shm_id=shmget(IPCKEY,1028,0640|IPC_CREAT|IPC_EXCL); 
    if(shm_id==-1)
    {
        printf("shmget error\n");
        return -1;
    }
    //将这块共享内存区附加到自己的内存段
    p_setting=(st_setting*)shmat(shm_id,NULL,0);
    strncpy(p_setting->agen,"jinyh",10); 
    printf( "agen:%s\n",p_setting->agen );
    p_setting->file_no = 1;
    printf( "file_no:%d\n",p_setting->file_no );
    system("ipcs -m");//此时可看到有进程关联到共享内存的信息，nattch为1
    //将这块共享内存区从自己的内存段删除出去
    if(shmdt(p_setting) == -1)
       perror(" detach error ");
    system("ipcs -m");//此时可看到有进程关联到共享内存的信息，nattch为0
    //删除共享内存
    if (shmctl( shm_id , IPC_RMID , NULL ) == -1)
      perror(" delete error ");
     //exit(0);
}
注意：在使用共享内存，结束程序退出后。如果你没在程序中用shmctl()删除共享内存的话，一定要在命令行下用ipcrm命令删除这块共享内存。你要是不管的话，它就一直在那儿放着了。
简单解释一下ipcs命令和ipcrm命令。
取得ipc信息：
ipcs [-m|-q|-s]
-m      输出有关共享内存(shared memory)的信息
-q      输出有关信息队列(message queue)的信息
-s      输出有关“遮断器”(semaphore)的信息
%ipcs -m
删除ipc
ipcrm -m|-q|-s shm_id
%ipcrm -m 105
二、陷阱（参考http://www.ibm.com/developerworks/cn/aix/library/au-cn-sharemem/）
1）ftok陷阱
采用ftok来生成key的情况下，如果ftok的参数pathname指定文件被删除后重建，则文件系统会赋予这个同名文件（或目录）新的i节点信息，于是这些进程所调用的ftok虽然都能正常返回，但得到的键值却并不能保证相同。
2）3. AIX中shmat的问题
AIX系统中，System V各类进程间通信机制在使用中均存在限制。区别于其它UNIX操作系统对IPC机制的资源配置方式，AIX使用了不同的方法；在AIX中定义了 IPC 机制的上限, 且是不可配置的。就共享内存机制而言，在4.2.1及以上版本的AIX系统上，存在下列限制：
对于64位进程，同一进程可连接最多268435456个共享内存段； 
对于32位进程，同一进程可连接最多11个共享内存段，除非使用扩展的shmat； 
上述限制对于64位应用不会带来麻烦，因为可供连接的数量已经足够大了；但对于32位应用，却很容易带来意外的问题，因为最大的连接数量只有11个。
下面的例程test02.c演示了这个问题，为了精简代码，它反复连接的是同一个共享内存对象；实际上，无论所连接的共享内存对象是否相同，该限制制约的是连接次数：
#include <stdio.h>
#include <errno.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAX_ATTACH_NUM 15
void main(int argc, char* argv[])
{
    key_t       mem_key;
    long        mem_id;
    void*       mem_addr[MAX_ATTACH_NUM];
    int          i;
    if ( ( mem_key = ftok("/tmp/mykeyfile", 1) ) == (key_t)(-1) ) {
            printf("Failed to generate shared memory access key, ERRNO=%d\n",
    errno);
            goto MOD_EXIT;
    }
    if ( ( mem_id = shmget(mem_key, 256, IPC_CREAT) ) == (-1) ) {
            printf("Failed to obtain shared memory ID, ERRNO=%d\n", errno);
            goto MOD_EXIT;
    }
    for ( i=1; i<=MAX_ATTACH_NUM; i++ ) {
   if ( ( mem_addr[i] = (void *)shmat(mem_id, 0, 0) ) == (void *)(-1) )
    printf("Failed to attach shared memory, times [%02d], errno:%d\n", i,
    errno);
   else
    printf("Successfully attached shared memory, times [%02d]\n", i);
    }
MOD_EXIT:
    shmctl(mem_id, IPC_RMID, NULL);
}
在AIX系统上，我们将其编译为test02，并运行，可以看到如下输出：
Successfully attached shared memory, times [01] 
Successfully attached shared memory, times [02] 
Successfully attached shared memory, times [03] 
Successfully attached shared memory, times [04] 
Successfully attached shared memory, times [05] 
Successfully attached shared memory, times [06] 
Successfully attached shared memory, times [07] 
Successfully attached shared memory, times [08] 
Successfully attached shared memory, times [09] 
Successfully attached shared memory, times [10] 
Successfully attached shared memory, times [11] 
Failed to attach shared memory, times [12], errno:24
Failed to attach shared memory, times [13], errno:24
Failed to attach shared memory, times [14], errno:24
Failed to attach shared memory, times [15], errno:24
说明超出11个连接之后，所有后续的共享内存连接都将无法建立。错误码24的定义是EMFILE，AIX给予的解释是：
The number of shared memory segments attached to the calling process exceeds the system-imposed limit。
解决这个问题的方法是，使用扩展的shmat；具体而言就是，在运行相关应用之前（确切地说，是在共享内存被创建之前），首先在shell中设置EXTSHM环境变量，通过它扩展shmat，对于源代码本身无需作任何修改：
   export EXTSHM=ON
值得注意的是，虽然设置环境变量，在程序中也可通过setenv函数来做到，比如在程序的开始，加入下列代码：
   setenv("EXTSHM", "ON", 1);
但实践证明这样的方法在解决这个问题上是无效的；也就是说唯一可行的办法，就是在shell中设置EXTSHM环境变量，而非在程序中。
在AIX上配置32位DB2实例时，也要求确保将环境变量 EXTSHM 设为 ON，这是运行 Warehouse Manager 和 Query Patroller 之前必需的操作：
export EXTSHM=ON
db2set DB2ENVLIST=EXTSHM
db2start
其原因即来自我们刚刚介绍的AIX中32位应用连接共享内存时，存在最大连接数限制。这个问题同样普遍存在于AIX平台上Oracle等软件产品中。
3）HP-UX中shmget和shmat的问题
3.1 32位和64位应用兼容问题
在HP-UX平台上，如果同时运行32位应用和64位应用，而且它们访问的是一个相同的共享内存区，则会遇到兼容性问题。
在HP-UX中，应用程序设置IPC_CREAT标志调用shmget，所创建的共享内存区，只可被同类型的应用所访问；即32位应用程序所创建的共享内存区只可被其它的32位应用程序访问，同样地，64位应用程序所创建的共享内存区只可被其它的64位应用程序访问。
如果，32位应用企图访问一个由64位应用创建的共享内存区，则会在调用shmget时失败，得到EINVAL错误码，其解释是：
A shared memory identifier exists for key but is in 64-bit address space and the process performing the request has been compiled as a 32-bit executable.
解决这一问题的方法是，当64位应用创建共享内存时，合并IPC_CREAT标志，同时给定IPC_SHARE32标志：
shmget(mem_key, size, 0666 | IPC_CREAT | IPC_SHARE32)
对于32位应用，没有设定IPC_SHARE32标志的要求，但设置该标志并不会带来任何问题，也就是说无论应用程序将被编译为32位还是64位模式，都可采用如上相同的代码；并且由此解决32位应用和64位应用在共享内存访问上的兼容性问题。
3.2 对同一共享内存的连接数限制
在HP-UX上，应用进程对同一个共享内存区的连接次数被限制为最多1次；区别于上面第3节所介绍的AIX上的连接数限制，HP-UX并未对指向不同共享内存区的连接数设置上限，也就是说，运行在HP-UX上的应用进程可以同时连接很多个不同的共享内存区，但对于同一个共享内存区，最多只允许连接1次；否则，shmat调用将失败，返回错误码EINVAL，在shmat的man帮助中，对该错误码有下列解释：
shmid is not a valid shared memory identifier, (possibly because the shared memory segment was already removed using shmctl(2) with IPC_RMID), or the calling process is already attached to shmid.
这个限制会对多线程应用带来无法避免的问题，只要一个应用进程中有超过1个以上的线程企图连接同一个共享内存区，则都将以失败而告终。
解决这个问题，需要修改应用程序设计，使应用进程具备对同一共享内存的多线程访问能力。相对于前述问题的解决方法，解决这个问题的方法要复杂一些。
作为可供参考的方法之一，以下介绍的逻辑可以很好地解决这个问题：
基本思路是，对于每一个共享内存区，应用进程首次连接上之后，将其键值（ftok的返回值）、系统标识符（shmid，shmget调用的返回值）和访问地址（即shmat调用的返回值）保存下来，以这个进程的全局数组或者链表的形式留下记录。在任何对共享内存的连接操作之前，程序都将先行检索这个记录列表，根据键值和标志符去匹配希望访问的共享内存，如果找到匹配记录，则从记录中直接读取访问地址，而无需再次调用shmat函数，从而解决这一问题；如果没有找到匹配目标，则调用shmat建立连接，并且为新连接上来的共享内存添加一个新记录。
记录条目的数据结构，可定义为如下形式：
typedef struct _Shared_Memory_Record
{
key_t   mem_key;   // key generated by ftok()   
int    mem_id;    // id returned by shmget()   
void*   mem_addr;   // access address returned by shmat() 
int    nattach;    // times of attachment    
} Shared_
4）Solaris中的shmdt函数原型问题
Solaris系统中的shmdt调用，在原型上与System V标准有所不同，
    Default
     int shmdt(char *shmaddr);
即形参shmaddr的数据类型在Solaris上是char *，而System V定义的是void * 类型；实际上Solaris上shmdt调用遵循的函数原型规范是SVID-v4之前的标准；以Linux系统为例，libc4和libc5 采用的是char * 类型的形参，而遵循SVID-v4及后续标准的glibc2及其更新版本，均改为采用void * 类型的形参。
如果仍在代码中采用System V的标准原型，就会在Solaris上编译代码时造成编译错误；比如：
Error: Formal argument 1 of type char* in call to shmdt(char*) 
is being passed void*.
解决方法是，引入一个条件编译宏，在编译平台是Solaris时，采用char * 类型的形参，而对其它平台，均仍采用System V标准的void * 类型形参，比如：
#ifdef _SOLARIS_SHARED_MEMORY         
shmdt((char *)mem_addr); 
#else                 
shmdt((void *)mem_addr); 
#endif
5）通过shmctl删除共享内存的风险
如果共享内存已经与所有访问它的进程断开了连接，则调用IPC_RMID子命令后，系统将立即删除共享内存的标识符，并删除该共享内存区，以及所有相关的数据结构； 
如果仍有别的进程与该共享内存保持连接，则调用IPC_RMID子命令后，该共享内存并不会被立即从系统中删除，而是被设置为IPC_PRIVATE状态，并被标记为"已被删除"；直到已有连接全部断开，该共享内存才会最终从系统中消失。
需要说明的是：一旦通过shmctl对共享内存进行了删除操作，则该共享内存将不能再接受任何新的连接，即使它依然存在于系统中！所以，可以确知，在对共享内存删除之后不可能再有新的连接，则执行删除操作是安全的；否则，在删除操作之后如仍有新的连接发生，则这些连接都将失败！
