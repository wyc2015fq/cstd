# 信号量(semaphore)入门 - 建建的博客 - CSDN博客
2017年03月07日 15:30:38[纪建](https://me.csdn.net/u013898698)阅读数：254
**信号量(semaphore) 入门**
    国内的计算机类教材太垃圾了，大多数都是把国外的帮助文档拿来翻译一下就拿去卖钱了，可谓适应知识经济的发展，呵呵。计算机类的书籍不像文学类的书籍，只 要识字就能看懂，所以这些教材编得不好，也没有多少人能看得出来( 背后N 个人在@!%@#%&* ）。对于很多计算机知识，入门最好的方法还是从简 单的开始理解，然后那些复杂的东西就会迎刃而解。
--------------------------------------------------------------------------------
    今天我来说说[Linux](http://lib.csdn.net/base/linux)/Unix 下信号量使用的入门吧。信号量是什么？就是设置在内核当中用户解决资源冲突东东。举个简单的例子，一张凳子一个人已经坐 了，第二个人就不能坐，但如果第二个人不知道这张凳子已经给人坐了，还坐上去，就会出现问题，所以如果我们约定凳子被占了就登记到一个本子上，每个人想坐
 这张凳子之前都先看看这个本子有没写下东西，就不会出现上述情况了。我说的这些，只要上过[操作系统](http://lib.csdn.net/base/operatingsystem)的同学都知道（除非经常逃课，期末考试碰巧又混过去 了:P ），但是在实际编程当中，新手根本不知道Linux/Unix 的信号量怎么使用。信号量从哪里来，怎么用呢？查那些教材又让人一头雾水，FT ！
     所以我用自己实验的例子来说说，Linux/Unix 下怎么使用信号量。
要使用信号量当然就要创建啦，怎么弄？
用semid=semget(KEY,3,0777 | IPC_CREAT);
其中KEY 就是给信号量的键，系统会根据信号量的键分配一 个信号量ID ，一个键一个信号量ID ，所以如果你想不同程序都使用同一个信号量的话，只要这个KEY 的值相等就行了。第二个是信号量元素的数量，其实我们 创建的是信号量集，里面有包含着元素，我的例子当中是3 个元素，0777 是信号量的权限，这是一个8 进制数字，为什么使用它？因为权限刚好分为写、读、执 行三种，用1 、2、4 代表刚好，方便快捷。0777 就是，根用户、用户组、一般用户都有读、写、执行的权力，IPC_CREAT 是FLAG ，表示创建这个
 信号量，当然，如果KEY 对应的信号量已经创建，则会返回已创建信号量的ID. 其他的FLAG 可以参考man semget. （不要告诉我你不知道man semget 是什么哦，FT ！就是Linux 下的帮助文档啊，man 就是manual 的简写，记住哦。）。
好了，创建好信号量( 我们用semid ，一个整型变量保存信号量ID) 后又怎么样呢？哦！还没有给这三个信号量元素赋初值！那要怎么做？
用semctl(semid,0,SETALL,semun_init)
semid 不用说了吧，第二个参数是从第几个元素开始赋值，这里是0 ，即从第一个元素开始，SETALL是FLAG ，这里的意思是：对所有的元素赋 值，最后一个参数是union semun 类型，里面包含每个元素应赋的值，要使用union semun 类型，记得在你的程序里面要加上：
 #if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
 /* union semun is defined by including */
 #else
 /* according to X/OPEN we have to define it ourselves */
 union semun {
  int val;                  /* value for SETVAL */
  struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
  unsigned short *array;    /* array for GETALL, SETALL */
     /* Linux specific part: */
  struct seminfo *__buf;    /* buffer for IPC_INFO */
       };
       #endif
注释上说了，根据X/OPEN ，即有些系统下我们要自己定义union semun 。
有了的定义之后，我们就可以定义变量semun_init
union semun semun_init;
因为我们的信号量有3 个元素，所以我们会使用semun_init 里面的unsigned short *array 。如果只有一个元素，就使用int val 。
再定义一个变量数组
unsigned short val[3]={1,1,1};//3 个元素的值均为1
然后：
semun_init.array = val;
好了，准备好后就可以调用刚才的semctl(semid,0,SETALL,semun_init) ，信号量初始化完毕。
将程序拼在一起:
int semid;
semid=semget(KEY,3,0777 | IPC_CREAT);
union semun semun_init;
unsigned short val[3]={1,1,1};
semun_init.array = val;
semctl(semid,0,SETALL,semun_init);
semctl(semid,0,IPC_RMID);// 最后不要忘了删除信号量哦。
还有一些零碎的东西，如返回值的处理啊等等，有了上面的基础，大家可以参看信号量的文档就很容易理解了。按我以上的例子，我简单举例说明一下：
int a = semctl(semid,2,GETVAL); 获取第三个元素的值到变量a 中，元素需要从0 开始，刚好2 就是第三个元素。
struct sembuf op_down={0,-1,IPC_NOWAIT};// 设置信号量操作的结构体变量
semop(semid,&op_down,1);// 注意！第二个参数是指针，第三个参数表示该指针指向多少个这种结构体。用这种方法我们可以一次性对几个元素进行操作。
就是将第一个元素进行-1 操作，IPC_NIWAIT 明显是FLAG 啦，表示如果操作不成功程序不等待，而直接返回。
怎样简单否？
国内和国外的学术最大的不同就是，国外把很难的东西变得很简单，国内则把很简单的东西变得很难，以显示作者的高深。呵呵，好在，我这篇东西与学术是一个地一个天，没法比！^_^
原文出处：[http://blog.ednchina.com/marshallemon/243738/message.aspx#](http://blog.ednchina.com/marshallemon/243738/message.aspx)
