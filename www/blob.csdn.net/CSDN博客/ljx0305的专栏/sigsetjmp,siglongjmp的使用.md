# sigsetjmp,siglongjmp的使用 - ljx0305的专栏 - CSDN博客
2008年09月09日 14:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：10401
![b.gif](http://imgcache.qq.com/ac/b.gif)
sigsetjmp,siglongjmp的使用
![b.gif](http://imgcache.qq.com/ac/b.gif)相关函数：longjmp, siglongjmp, setjmp 
表头文件：#include <setjmp.h> 
函数定义：int sigsetjmp(sigjmp_buf env, int savesigs) 
函数说明：sigsetjmp()会保存目前堆栈环境，然后将目前的地址作一个记号，而在程序其他地方调用siglongjmp()时便会直接跳到这个记号位置，然后还原堆栈，继续程序好执行。 
参数env为用来保存目前堆栈环境，一般声明为全局变量 
参数savesigs若为非0则代表搁置的信号集合也会一块保存 
当sigsetjmp()返回0时代表已经做好记号上，若返回非0则代表由siglongjmp（）跳转回来。 
返回值  ：返回0代表局促存好目前的堆栈环境，随时可供siglongjmp()调用， 若返回非0值则代表由siglongjmp()返回 
附加说明：setjmp()和sigsetjmp()会令程序不易令人理解，请尽量不要使用
//测试代码
#include <unistd.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdlib.h>
#include <signal.h>
static jmp_buf env_alrm;
void alarm_fun()
{
printf("time out/n");
siglongjmp(env_alrm,1);
}
int test_alrm()
{
int test_value = 0;
signal(SIGALRM,alarm_fun);
if ( sigsetjmp(env_alrm, 1) != 0 )
    {
        alarm( 0 );
        signal( SIGALRM, SIG_IGN );
        return -1;    /* timeout */
    }
alarm(5);//5 sec;
sleep(10);//改变这里看效果
alarm(0);
signal( SIGALRM, SIG_IGN );//忽略信号
return 1;
}
int main()
{
printf("%d",test_alrm());
return 0;
}
![b.gif](http://imgcache.qq.com/ac/b.gif)
