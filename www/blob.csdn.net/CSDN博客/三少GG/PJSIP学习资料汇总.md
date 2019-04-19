# PJSIP学习资料汇总 - 三少GG - CSDN博客
2012年12月15日 15:34:55[三少GG](https://me.csdn.net/scut1135)阅读数：2955
**/////////////////////////////推荐博客/////////////////////////////////**
**liyong042**
**[http://hi.baidu.com/liyong042](http://hi.baidu.com/liyong042)**
[多线程编程 临界区，事件，信号量，互斥量 2](http://hi.baidu.com/liyong042/item/7501e1c434d7b3384494168c)
///////////////////////////////////////////////////////////////////////////
[编译pjsip2.0 + SDL 1.3](http://blog.sina.com.cn/s/blog_513f4e8401011ob4.html)
[ffmpeg 中添加264支持](http://blog.sina.com.cn/s/blog_513f4e8401011yuq.html)
[](http://blog.sina.com.cn/s/blog_513f4e8401011yuq.html)[pjsip： 修改SDP的属性](http://blog.sina.com.cn/s/blog_513f4e8401011pkg.html)
[sip的基本概念](http://blog.sina.com.cn/s/blog_513f4e840100pj2t.html)
[pjsip 2.0 ffmpeg X264  问题汇总](http://blog.sina.com.cn/s/blog_513f4e84010121if.html)
[运行FreeSWITCH](http://blog.sina.com.cn/s/blog_513f4e840101b6mv.html)
## [pjsip的语音检测](http://blog.sina.com.cn/s/blog_513f4e8401011hf9.html)
在pjsip中包含了语音静音检测的实现。
方法就是对每帧的采样点进行累加，得到的结果和门限进行比较。
语音检测分为固定模式和动态模式。固定模式就是累加值和给定的门限比较，动态模式的具体方法还不确定。
语音检测的调用关系：
g711_encode()->pjmedia_silence_det_detect（）
g711_encode：
is_silence =pjmedia_silence_det_detect(priv->vad,
      (constpj_int16_t*) input->buf,
      (input->size>> 1), NULL);
 if (is_silence&&
    (PJMEDIA_CODEC_MAX_SILENCE_PERIOD == -1 ||
     silence_period <PJMEDIA_CODEC_MAX_SILENCE_PERIOD*8000/1000))
 {
    output->type = PJMEDIA_FRAME_TYPE_NONE;
    output->buf = NULL;
    output->size = 0;
    output->timestamp =input->timestamp;
returnPJ_SUCCESS;
 } else {
    priv->last_tx =input->timestamp;
 }
    }
........
这里把代码贴一下：
PJ_DEF(pj_bool_t) pjmedia_silence_det_apply( pjmedia_silence_det*sd,
         pj_uint32_t level)
{
    intavg_recent_level;
if(gp_pjmedia_silence_det->mode ==VAD_MODE_NONE)//无模式
 returnPJ_FALSE;
if(gp_pjmedia_silence_det->mode ==VAD_MODE_FIXED)//静态模式
 return (level<gp_pjmedia_silence_det->threshold);
)//动态模式
   gp_pjmedia_silence_det->sum_level += level;
   ++gp_pjmedia_silence_det->sum_cnt;
   avg_recent_level =(gp_pjmedia_silence_det->sum_level /gp_pjmedia_silence_det->sum_cnt);
    if (level> gp_pjmedia_silence_det->threshold||
 level >=PJMEDIA_SILENCE_DET_MAX_THRESHOLD)
    {
 gp_pjmedia_silence_det->silence_timer= 0;
 gp_pjmedia_silence_det->voiced_timer+= gp_pjmedia_silence_det->ptime;
 switch(gp_pjmedia_silence_det->state){
    case STATE_VOICED:
  if(gp_pjmedia_silence_det->voiced_timer>gp_pjmedia_silence_det->recalc_on_voiced) {
     gp_pjmedia_silence_det->threshold =(avg_recent_level +gp_pjmedia_silence_det->threshold)>> 1;
     TRACE_((THIS_FILE,"Re-adjust threshold (in talk burst)"
      "to %d", gp_pjmedia_silence_det->threshold));
     gp_pjmedia_silence_det->voiced_timer = 0;
     gp_pjmedia_silence_det->sum_level =avg_recent_level;
     gp_pjmedia_silence_det->sum_cnt = 1;
  }
  break;
    case STATE_SILENCE:
  TRACE_((THIS_FILE,"Startingtalk burst (level=%d threshold=%d)",
   level,gp_pjmedia_silence_det->threshold));
    case STATE_START_SILENCE:
  gp_pjmedia_silence_det->state= STATE_VOICED;
  gp_pjmedia_silence_det->sum_level= level;
  gp_pjmedia_silence_det->sum_cnt= 1;
  break;
    default:
  pj_assert(0);
  break;
 }
    } else{
 gp_pjmedia_silence_det->voiced_timer= 0;
 gp_pjmedia_silence_det->silence_timer+= gp_pjmedia_silence_det->ptime;
 switch(gp_pjmedia_silence_det->state){
    case STATE_SILENCE:
  if(gp_pjmedia_silence_det->silence_timer>=gp_pjmedia_silence_det->recalc_on_silence) {
     gp_pjmedia_silence_det->threshold = avg_recent_level<< 1;
     TRACE_((THIS_FILE,"Re-adjust threshold (in silence)"
      "to %d", gp_pjmedia_silence_det->threshold));
     gp_pjmedia_silence_det->silence_timer = 0;
     gp_pjmedia_silence_det->sum_level =avg_recent_level;
     gp_pjmedia_silence_det->sum_cnt = 1;
  }
  break;
    case STATE_VOICED:
  gp_pjmedia_silence_det->state= STATE_START_SILENCE;
  gp_pjmedia_silence_det->sum_level= level;
  gp_pjmedia_silence_det->sum_cnt= 1;
    case STATE_START_SILENCE:
  if(gp_pjmedia_silence_det->silence_timer>=gp_pjmedia_silence_det->before_silence) {
     gp_pjmedia_silence_det->state = STATE_SILENCE;
     gp_pjmedia_silence_det->threshold = avg_recent_level<< 1;
     TRACE_((THIS_FILE,"Starting silence (level=%d "
    "threshold=%d,g_silence_nbr:%d)",level,gp_pjmedia_silence_det->threshold,g_silence_nbr));
     gp_pjmedia_silence_det->sum_level =avg_recent_level;
     gp_pjmedia_silence_det->sum_cnt = 1;
  }
  break;
    default:
  pj_assert(0);
  break;
 }
    }
    return(sd->state == STATE_SILENCE);
}
语音检测的调用关系：
/////////////////////////////////////////////////////////////////////////////////////
**【您可能对以下 6 篇相关日志也感兴趣】[开源的SIP协议栈-PJSIP介绍](http://www.zhimax.com.sixxs.org/article/vc/pjsip.html)[53]**
[2012-11-06][pjsip中播放wav文件流程](http://www.zhimax.com.sixxs.org/article/tcp/pjsua_player_destroy.html)[25][2012-09-04][如何在Visual Studio2010中打开PJSIP工程](http://www.zhimax.com.sixxs.org/article/vc/vs2010_pjsip.html)[29][2012-06-29][PJSIP中UDP通信（RTP协议）报错IP Checksum Offload 解决方法！](http://www.zhimax.com.sixxs.org/article/tcp/79.html)[83][2012-04-28][PJSIP的DTMF简单实现，回调函数:on_dtmf_digit](http://www.zhimax.com.sixxs.org/article/vc/on_dtmf_digit.html)[97][2012-04-12][PJSIP自动语音功能，WAV文件远程播放，可以与TTS整合](http://www.zhimax.com.sixxs.org/article/vc/pjsip_tts.html)[121]
# [**PJSIP简介配置和PJLIB基础库的使用**](http://www.zhimax.com.sixxs.org/article/tcp/pjlib.html)
PJSIP的实现是为了能在嵌入式设备上高效实现SIP/VOIP. 
**1.PJSIP库的主要特征:**
1).极具移植性.(Extremely portable) 
   当前可支持平台包括: 
   * Win32/x86 (Win95/98/ME, NT/2000/XP/2003, mingw). 
   * arm, WinCE and Windows Mobile. 
   * Linux/x86, (user mode and as kernel module(!)). 
   * Linux/alpha 
   * Solaris/ultra. 
   * MacOS X/powerpc 
   * RTEMS (x86 and powerpc). 
   * Symbian OS
2).非常小的足印.(Very small footprint) 
   官方宣称编译后的库<150Kb,我在PC上编译后加上strip后大概173Kb,这对于嵌入 
   式设备,是个好消息
3).高性能.(High performance) 
   这点我们后面可以看看是否如作者宣称的
**2. PJSIP的组成.**
其实说是PJSIP不是特别贴切,这个库实际上是几个部分组成的. 
1).PJSIP - Open Source SIP Stack[开源的SIP协议栈] 
2).PJMEDIA - Open Source Media Stack[开源的媒�栈] 
3).PJNATH - Open Source NAT Traversal Helper Library[开源的NAT-T****库] 
4).PJLIB-UTIL - Auxiliary Library[****工具库] 
5).PJLIB - Ultra Portable Base Framework Library[基础框架库] 
    PJLIB-UTIL****工具库：加解密MD5和CRC32的算法 
    PJNATH开源的NAT库包含ICE打洞 
    PJSUA-LIB库：最顶层的SIP库支持VOIP 
    PJMEDIA库：最顶层的支持视频的库
**3. PJLIB基础框架库提供的功能：**
1).内存的处理、数据的存储 
   .数据结构的（hash表、link表、二叉树、等） 
   .caching和pool；缓冲池和内存池 
2).OS抽象 
   .线程、互斥、临界区、锁对象、事件对象 
   .定时器 
   .pj_str_t字符串 
3).操作系统级别的函数抽象 
   .socket的抽象(tcp/udp) 
   .文件的读写 
4).使用前的初始化，使用后的清理
**4.PJSIP的安装和配置：**
**1.安装和编辑pjsip库：**
A.将pjproject-1.5.zip和DirectX-Lib.rar解压到当前目录的pjlib路径下； 
B.查看pjlib\pjproject-1.5\pjlib\include\pj下面是否有config_site.h，如果没有建一个空文件 
C.将DirectX的x86的lib库拷贝到C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib
D.在环境变量中设置DXSDK_DIR=direx中的include目录 
E.首先编译pjlib\pjproject-1.5\pjproject-vs8.sln [win32] 
F.然后打开工程开始编译
**2.脱离pjsip环境的工程的配置**
需要头文件的可以加入 
$(SolutionDir)pjlib\pjproject-1.5\pjlib\include 
$(SolutionDir)pjlib\pjproject-1.5\pjlib-util\include 
$(SolutionDir)pjlib\pjproject-1.5\pjnath\include 
$(SolutionDir)pjlib\pjproject-1.5\pjsip\include 
$(SolutionDir)pjlib\pjproject-1.5\pjmedia\include
如果需要lib库文件的加入 
$(SolutionDir)pjlib\pjproject-1.5\pjlib\lib 
$(SolutionDir)pjlib\pjproject-1.5\pjlib-util\lib 
$(SolutionDir)pjlib\pjproject-1.5\pjnath\lib 
$(SolutionDir)pjlib\pjproject-1.5\pjsip\lib 
$(SolutionDir)pjlib\pjproject-1.5\pjmedia\lib 
$(SolutionDir)pjlib\pjproject-1.5\lib
我的程序所依赖PJLIB的库有： 
**pjlib**-i386-Win32-vc8-Debug.lib 
pjlib-util-i386-Win32-vc8-Debug.lib 
**pjnath**-i386-Win32-vc8-Debug.lib 
**pjsip**-core-i386-Win32-vc8-Debug.lib
pjsip-simple-i386-Win32-vc8-Debug.lib 
pjsip-ua-i386-Win32-vc8-Debug.lib 
**pjsua**-lib-i386-Win32-vc8-Debug.lib
**pjmedia**-audiodev-i386-Win32-vc8-Debug.lib
pjmedia-codec-i386-Win32-vc8-Debug.lib 
pjmedia-i386-Win32-vc8-Debug.lib 
**libpjproject-i386-Win32-vc8-Debug.lib **
**1.使用前的初始化和使用后的清理**
PJSIP库里面封装了很多线程内存池；而且很多对象都是基于内存池创建的，所以几乎所以的库都需要初始化或创建 
下面是pjlib,pjlib-util,pjnath,pjsua-lib库的初始化和关闭
显示行号复制代码这是一段程序代码。
- pj_status_t status;
- status = pj_init();         //初始化pjlib库返回PJ_SUCCESS表示成功
- status = pjlib_util_init(); //初始化pjlib-util库
- status = pjnath_init();     //初始化pjnath库
- status = pjsua_create();    //初始化pjsua-lib库;(里面初始化了pjlib,pjlib-util,pjnath) 
- 
- pj_shutdown();    //pjlib停止
- pjsua_destroy();  //pjsua-lib库的清理- 

**2.缓冲池和内存池(caching,pool)**
pjsip的内存池的使用规则： 
每个内存池都是基于内存池工厂创建的；pjsip有一个默认的内存池工厂pj_caching_pool； 
我喜欢把它叫做缓冲池；将来清理内存池的时候，可以关闭内存池也可直接关闭缓冲池； 
    pj_caching_pool cp; 
    pj_caching_pool_init(&cp, NULL, 1024*1024 );       //缓冲池 
    pj_caching_pool_destroy(&cp); //释放工厂
    pj_pool_t pool = pj_pool_create(&cp.factory, "name", 1024, 1024, NULL); //创建内存池
    pj_pool_release(&pool); //释放内存池到工厂中，直接释放工厂可省略这一步 
    void* p = pj_pool_alloc(pool, size); //在内存中开辟一个空间
    注：适当的初始化大小；内存池只能增加不能减小 
    由于pjlib很多内部对象都用内存池，所以在对象释放之前不能释放内存池。
使用缓冲池和内存池
- //系统初始化的时候调用
- void create()
- {
- //初始化pjlib库返回PJ_SUCCESS表示成功
- pj_status_t status = pj_init(); 
- 
- pj_caching_pool_init(&this->m_caching, NULL, 0);
- this->m_pool = pj_pool_create(&this->m_caching.factory, "", 256, 256, NULL);
- 
- //初始化mutex
- pj_mutex_create(this->m_pool, "", PJ_MUTEX_SIMPLE, &this->m_pool_mutex);
- 
- //创建一个lock给定时器用
- pj_lock_create_simple_mutex(this->m_pool, "timer_lock", &this->timer_heap_lock);
- 
- //在内存池上开辟一块空间
- char *pmem = (char*)pj_pool_alloc(this->m_pool, 1024); 
- 
- //创建一个定时器堆
- pj_timer_heap_create(this->m_pool, MAX_TIMER_COUTN, &this->timer_heap);
- 
- //给定时器加锁; 将来自动删除该锁，无需手工删除
- pj_timer_heap_set_lock(this->timer_heap, this->timer_heap_lock, true);
-  }
- 
- //系统退出的时候调用
- void destroy()
-  {
- //删除mutex
- pj_mutex_destroy(this->m_pool_mutex);
- 
- //删除定时器的堆
- pj_timer_heap_destroy(this->timer_heap);
- 
- //清理内存池，这一步也可以删略；让caching_pool来清理
- pj_pool_destroy_int(this->m_pool); 
- 
- //清理缓冲池；所有在缓冲池建立的内存池都会被清理掉
- pj_caching_pool_destroy(&this->m_caching);
- 
- //pjlib停止
- pj_shutdown();
-  }
- 

**3.线程的介绍，及其线程的封装和使用**
1.外部函数或线程使用到pjsip的时候，必须注册线程 
隐藏行号复制代码注册线程
- pj_thread_desc desc;
- pj_bzero(desc, sizeof(desc));
- pj_thread_t *thread_;
- if (pj_thread_register("", desc, &thread_) != PJ_SUCCESS) 
- return 0; /* 失败*/ 
- 
function CopyCode(key){var codeElement=null;var trElements=document.all.tags("ol");var i;for(i=0;i
2.线程的创建和使用 
pj_thread_create(pool*,"", function, *arg, stack_size, flag, **pj_thread_t)//创建一个线程
pj_thread_destroy(thread); 
//注销一个线程 
pj_thread_resume(thread); 
//线程继续 
pj_thread_sleep(1500);      
//当前的线程暂停1500毫秒
pj_thread_join(thrad);       
//等待线程结束 
pj_thread_proc函数原型是：int
thread_func(void
* argv); 
3.对pjlib的thread的线程的封装
1:class CSipThread
   2: {   3://1.实现一个int ()(void*)的函数，且在结束的地方设置m_thread_t=NULL   4:     pj_thread_t *m_thread_t;   5:protected:   6:   7:/* int (pj_thread_proc)(void*);   8:    * 线程函数；子类需要使用static函数来实现   9:    * 子类函数中在结束的时候：必须将m_thread_t = NULL;  10:    */  11:     pj_thread_proc *m_thread_function;  12:  13:/* 这个是实现函数；  14:    * 子类需要实现一个run()的函数；在run函数中调用run_父类的函数  15:    */  16:virtualvoid run_(pj_pool_t *pool, pj_thread_proc *function);  17:public:  18:     CSipThread();  19:virtual ~CSipThread();  20:virtualbool thread_running();  21:  22://等待线程结束，自动释放资源，且会将m_thread_t=NULL  23:virtualvoid thread_join();  24:  25://在线程自己退出的时候，没有需要手工释放资源，和设置m_thread_t=NULL  26:virtualvoid thread_destroy();  27: };  28: CSipThread::CSipThread()  29: {  30:this->m_thread_t = NULL;  31: }  32: CSipThread::~CSipThread()  33: {  34:  35: }  36:void CSipThread::run_(pj_pool_t *pool, pj_thread_proc *function)  37: {  38:if ( this->thread_running())  39:return;  40:     pj_status_t status = pj_thread_create(pool, "", function, this,   41:         PJ_THREAD_DEFAULT_STACK_SIZE, NULL, &this->m_thread_t);  42:if (status != PJ_SUCCESS)  43:     {  44:         CFunctions::write_log(LM_ERROR, "Can't create timer thread. [result=%d]..\n", status);  45:     }  46: }  47:bool CSipThread::thread_running()  48: {  49:if (this->m_thread_t == NULL)  50:returnfalse;  51:else  52:returntrue;  53: }  54:  55:void CSipThread::thread_join()  56: {  57:if (this->m_thread_t == NULL)  58:return;  59:  60:// 等待线程结束  61:     pj_thread_join(this->m_thread_t);  62: }  63:  64:void CSipThread::thread_destroy()  65: {  66:     pj_thread_destroy(this->m_thread_t);  67:this->m_thread_t = NULL;  68: }
4.使用封装起来的thread线程对象
1:/* 使用封装后的线程注意事项   2:* 1. 必须定义一个static的静态函数   3:* 2. 在静态函数结束的时候必须调用thread_destroy();   4:*/   5:class CSendThread : public CSipThread   6: {   7://线程函数；在函数退出的时候必须调用thread_destroy();   8:staticint thread_func(void * argv);   9:public:  10:     CSendThread(void);  11:virtual ~CSendThread(void);  12:  13://再次封装了run_函数  14:void run();  15: }  16:  17:void CSendThread::run()  18: {  19:if ( this->thread_running())  20:return;  21:  22:     SIP_GUARD(CInterFace::instance()->m_pool_mutex, obj);  23:  24://调用父类的run_函数进行创建函数，开始运行；注意如果线程已经存在，那么直接返回  25://不会再创建一个线程  26:this->run_(CInterFace::instance()->m_pool, &CSendThread::thread_func);  27: }  28:  29:int CSendThread::thread_func(void * argv)  30: {  31:// 线程函数的参数默认是当前对象，等同于this指针  32:     CSendThread *this_thread = (CSendThread*)argv;  33:  34: thread_end:  35:// 在线程结束的时候，一定要调用thread_destroy();来删除线程  36:     this_thread->thread_destroy();  37:return 0;  38: }
**4.互斥和锁,及其封装(mutex)**
**pj_mutex_create**(this->m_pool, "",
PJ_MUTEX_SIMPLE, &this->m_pool_mutex);
//创建一个锁指针 
**pj_mutex_destroy**(this->m_pool_mutex);
//删除锁指针 
**pj_mutex_lock**(m_mutex);//加锁 
**pj_mutex_unlock**(m_mutex);
//解锁
1:/* 封装了mutex的使用方法   2:* 在创建对象时加锁、删除对象时解锁   3:*/   4:class Sip_Lock   5: {   6:     pj_mutex_t *m_mutex;   7:public:   8:     Sip_Lock(pj_mutex_t *mutex)   9:     {  10:this->m_mutex = mutex;  11:         pj_mutex_lock(m_mutex);  12:     }  13:virtual ~Sip_Lock()  14:     {  15:         pj_mutex_unlock(m_mutex);  16:this->m_mutex = NULL;  17:     }  18: };  19:  20:/* 封装了sip_lock的使用，直接使用宏定义进行互斥 */  21:#define SIP_GUARD(MUTEX, OBJ) Sip_Lock OBJ(MUTEX);
**5.定时器(heap,callback,thread)**
//创建定时器堆，设置定时器堆pool大小
pj_timer_heap_create(this->m_pool, MAX_TIMER_COUTN, &this->timer_heap);
pj_timer_heap_set_max_timed_out_per_poll(this->timer_heap, 20);
//对定时器加锁是lcok类型
pj_timer_heap_set_lock(this->timer_heap,
this->timer_heap_lock, 
true); 
//启动定时器轮询的线程
this->run_(this->m_pool, &CInterFace::timer_thread_fun);
//设置一个定时器
pj_timer_heap_schedule(this->timer_heap,
entry, delay); 
//取消一个定时器
pj_timer_heap_cancel(this->timer_heap,
entry); 
//定时器的入口点定义
struct pj_timer_entry 
{ 
void *user_data;
// 定时器的用户数据；C++通常用类对象；C通常用struct
int id; //
绝对的ID号；用来区分当user_data和cb都相同的情况 
pj_timer_heap_callback
*cb; // 定时器中的回调函数
}; 

//定时器回调函数的原型
void timer_callback(pj_timer_heap_t *timer_heap,structpj_timer_entry *entry);
一个定时器的使用例子： 
1. 创建一个定时器、创建一个线程 
2. 创建一个定时器入口entry 
3. 创建一个定时器的回调函数，在回调函数中必须重新将entry加入堆中
1://创建定时器堆，设置定时器堆pool大小; 加锁；   2: pj_timer_heap_create(this->m_pool, MAX_TIMER_COUTN, &this->timer_heap);   3: pj_timer_heap_set_max_timed_out_per_poll(this->timer_heap, 20);   4: pj_timer_heap_set_lock(this->timer_heap, this->timer_heap_lock, true);   5:   6://启动定时器轮询的线程   7:this->run_(this->m_pool, &CInterFace::timer_thread_fun);   8:   9:  10://定时器的线程函数  11:int CInterFace::timer_thread_fun(void* argv)  12: {  13:     CInterFace *this_thread = (CInterFace*)argv;  14:int rc;  15:while ( !CInterFace::instance()->application_exit() )  16:     {  17:         pj_thread_sleep(1);  18:  19:#if defined(PJ_SYMBIAN) && PJ_SYMBIAN!=0  20:/* On Symbian, we must use OS poll (Active Scheduler poll) since   21:        * timer is implemented using Active Object.  22:        */  23:         rc = 0;  24:while (pj_symbianos_poll(-1, 0))  25:             ++rc;  26:#else  27:         PJ_USE_EXCEPTION;  28:         PJ_TRY  29:         {  30:             rc = pj_timer_heap_poll(CInterFace::instance()->timer_heap, NULL);  31:         }  32:         PJ_CATCH_ANY  33:         {  34:  35:         }  36:         PJ_END;  37:#endif  38:     }  39:  40:// 定时器轮询的线程退出；必须手工删除pj_thread_t指针，因为它使用了内存池！  41:     CFunctions::write_log(LM_DEBUG, "pj_timer_heap_pool, ending.\n");  42:     this_thread->thread_destroy();  43:return 0;   44: }  45:  46:  47://启动一个定时器：也就是将一个entry添加到堆上面  48:void CInterFace::start_timer(pj_timer_entry* entry, pj_time_val *delay)  49: {  50:if ( !this->thread_running())  51:return;  52:  53:     SIP_GUARD(this->timer_heap_mutex, obj);  54:     pj_timer_heap_schedule(this->timer_heap, entry, delay);  55: }  56:  57://取消一个定时器：从堆上删除一个entry  58:void CInterFace::stop_timer(pj_timer_entry* entry)  59: {  60:if ( !this->thread_running())  61:return;  62:  63:     SIP_GUARD(this->timer_heap_mutex, obj);  64:     pj_timer_heap_cancel(this->timer_heap, entry);  65: }  66:  67://启动上报速度和进度的定时器  68:bool CBaseFile::start_timer_speedProgress()  69: {  70:this->m_timer_speed_tval.msec = 0;  71:this->m_timer_speed_tval.sec = 1;  72:this->m_timer_speed.user_data = this;  73:this->m_timer_speed.cb = &CBaseFile::callback_speed;  74:     SIP_GUARD(this->m_run_speed_mutex, obj);  75:this->m_run_speed = true;  76:     CInterFace::instance()->start_timer(&this->m_timer_speed, &this->m_timer_speed_tval);  77:returntrue;  78: }  79:  80://取消上报进度和速度的定时器  81:void CBaseFile::stop_timer_speedProgress()  82: {  83:     SIP_GUARD(this->m_run_speed_mutex, obj);  84:this->m_run_speed = false;  85:     CInterFace::instance()->stop_timer(&this->m_timer_speed);  86: }  87:  88://上报进度和速度的回调函数  89:void CBaseFile::callback_speed(pj_timer_heap_t *timer_heap, pj_timer_entry *entry)  90: {  91:// 上报速度和状态的的  92:if (entry->user_data != NULL)  93:     {  94:         CBaseFile *file = (CBaseFile*)entry->user_data;  95:  96:uint speed_ = 0;  97:uint progress_ = 0;  98:         file->get_speed_progress(speed_, progress_);  99: 100:if ( file->m_ice_session->is_running()) 101:         { 102:             CInterFace::instance()->report_speed( 103:                 file->get_userid().c_str(),  104:                 file->get_fileName().c_str(),  105:                 file->get_guid(),  106:                 speed_,  107:                 progress_); 108:         } 109: 110:// 由于定时select出来后，就从堆上删除了；所以需要一直触发的定时器， 111:// 就必须在回调函数中，重新想堆中添加entry！ 112:         SIP_GUARD(file->m_run_speed_mutex, obj); 113:if ( file->m_run_speed) 114:             CInterFace::instance()->start_timer(entry, &file->m_timer_speed_tval); 115:     } 116: }
**6.pj_str_t字符串**
**7.socket的封装和使用**

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PJSIP添加视频
   虽然现在 psjip2.0 已经增加了 视频的支持，但是 要真正 做好对 视频 ，云台 等 的支持， 尤其是嵌入式上 的支持 ，还是需要对psjp 内部各个模块 了解一下， 毕竟从音频 跨越到 视频 ， pjsip 2.0 的真正稳定 也不是一天两天的事 。 
    增加视频 各式各样的方法都有， 各有各的好处。 但是 做为pjsip 的设计的初衷，是考虑到各种媒体传输的， 不单只是 增加视频， 哪怕是 视频会议中的 双流， 云台控制 等 都是 可以任意增加的 。这个 可以从 pjsip1.08 版本的 sdp 测试就可以 看出来。  
     当然，要真正的实现 视频的编码解码（ 2.0 正在加入 ），以及视频和音频 同步等等， 1.x 版本 是不支持的， 但这个 并不影响 视频的加入。 
    既然 pjsip 的  Sdp 协商已经 对 视频 ，云台 ，多个视频流 等的支持  。那么 只要 在 pjsua-lib 中 对 sdp 的 构建 自己实现 就可以了（ 这一块本在 pjmeida 中实现的 ）， 然后根据协商  的 结果 打开 udp_Transport 的 就可以了 （ 当然 对 ice 的支持，需要多加考虑 ）。
对于 pjsip 的其他模块 ，有人会说 对视频可能 不支持， 我的 回答是： 是的 ，肯定不会考虑那么多。（ 但是调节一下啊 就可以了嘛 ）
对了 对于 sdp怎么构建 可以看看 字符串到 sdp  , 这样就知道 怎么从 udp_tansprot 到 sdp 了 
//创建新的sdp 注意内存需要重新分配 
pjmedia_sdp_session **p_sdp;
static  char *sd = {   "v=0\r\n"
"o=1010 418 0 IN IP4 192.168.1.103\r\n"
"s=-\r\n"
"c=IN IP4 192.168.1.103\r\n"
"b=AS:1920\r\n"
"t=0 0\r\n"
"m=audio 7000 RTP/AVP 8\r\n"
"a=rtpmap:8 PCMA/8000\r\n"
"a=sendrecv\r\n"
"m=video 24000 RTP/AVP 124\r\n"
"b=TIAS:1920000\r\n"
"a=rtpmap:124 H264/90000\r\n"
"a=fmtp:124 profile-level-id=42801e\r\n"
"a=sendrecv\r\n"
"m=application 7004 RTP/AVP 100\r\n"
"a=rtpmap:100 H224/0\r\n"
"a=sendrecv\r\n" };
std::string k_sdpstr= sd;
char* k_str = (char*) pj_pool_alloc( pool, k_sdpstr.length() +1  );
strcpy( k_str, k_sdpstr.c_str() );
//分析sdp信息
pjmedia_sdp_session *k_sdp_session ;
pj_status_t status = pjmedia_sdp_parse( pool,  k_str,  k_sdpstr.length(),   &k_sdp_session);
if ( status != PJ_SUCCESS )
{
_debug_Error(" pjmedia_sdp_parse ");
if( sip_status_code )  *sip_status_code =415;
return -40;
}
//验证sdp信息 
status = pjmedia_sdp_validate( k_sdp_session );
if( status != PJ_SUCCESS ) 
{
if( sip_status_code )  *sip_status_code =500;
_debug_Error(" pjmedia_sdp_validate ");
return -50;
}
*p_sdp = k_sdp_session;
/////////////////////////
基于PJSIP的多媒体终端的设计与实现
[http://wenku.baidu.com/view/71130fea551810a6f5248618.html](http://wenku.baidu.com/view/71130fea551810a6f5248618.html)
