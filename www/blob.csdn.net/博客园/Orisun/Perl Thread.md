# Perl Thread - Orisun - 博客园







# [Perl Thread](https://www.cnblogs.com/zhangchaoyang/articles/2057178.html)





perldoc threads　　----　　英文的... ...

网上找不到中文全面介绍的，无奈，哥今天就来翻译一下。

<<=========================threads===========================>>

#!/usr/bin/perl
use threads ('yield',
            'stack_size' => 64*4096,
            'exit' => 'threads_only',
            'stringify');

sub start_thread {
   my @args = @_;
   print('Thread started: ', join(' ', @args), "\n");
}

##创建线程的方法
# my $thr = threads->create('func_name', ...);
# my $thr = threads->create(sub { ... }, ...);
# my $thr = threads->create(\&func, ...);
# The "->new()" method is an alias for "->create()".
my $thr = threads->create('start_thread', 'argument1', 'argument2');		#通过create创建线程。返回线程实例
$thr->join();				#等待线程结束
threads->create(sub { print("I am a thread\n"); })->join();					#创建一个线程，没有返回值。那这个线程实例如何访问呢？

my $thr2 = async { foreach (@ARGS) { print"$_\n"; } };								#通过async使用匿名子例程创建线程
$thr2->join();
if (my $err = $thr2->error()) {
   warn("Thread error: $err\n");
}

# 在隐式的列表环境中调用thread
my $thr3 = threads->create(sub { return (qw/a b c/); });
# 在显式的列表环境中调用thread
my $thr4 = threads->create({'context' => 'list'},
                         sub { return (qw/a b c/); });
# 由于创建线程时使用的子例程返回的是列表，所以这里的join函数返回的也是列表
my @results = $thr3->join();
print "@results\n";
# 把线程从主线程中分离出来
# $thr->detach();		##报错：Cannot detach a joined thread,因为$thr已经调用过join()
$thr4->detach();		##
$tid = $thr4->tid();
print "线程4ID：$tid\n";

# Get a thread's object
$thr6 = threads->self();
$thr7 = threads->object($tid);

# Get a thread's ID
$tid = threads->tid();
$tid = "$thr7";		#根据线程实例获得线程ID

# 给其他线程一个运行的机会
threads->yield();
yield();

# 返回未分离的线程列表
my @threads = threads->list();
my $thread_count = threads->list();

my @running = threads->list(threads::running);
my @joinable = threads->list(threads::joinable);

# 判断两个线程是相同
if ($thr4 == $thr2) {
   print "thread4 equals to thread2.\n";
}

# 管理线程栈大小
$stack_size = threads->get_stack_size();
$old_size = threads->set_stack_size(32*4096);

# Create a thread with a specific context and stack size
my $thr5 = threads->create({ 'context'    => 'list',
                           'stack_size' => 32*4096,
                           'exit'       => 'thread_only' },
                         \&start_thread);

# Get thread's context
my $wantarray = $thr->wantarray();
print $wantarray,"\n";

# Check thread's state
if ($thr5->is_running()) {
   sleep(1);
}
if ($thr5->is_joinable()) {
   $thr5->join();
}

# Send a signal to a thread
$thr5->kill('SIGUSR1');

# Exit a thread
threads->exit();										




<<=========================Thread========================>>

$thread = Thread->new(\&start_sub)

$thread = Thread->new(\&start_sub,@args)

start_sub指定线程要执行的子例程，args是传给子例程的参数。

lock  VARIABLE

给变量加锁，直到锁超出范围。给变量加锁只影响到lock函数的调用--即一个线程lock var1后，另一个线程再调用lovk var1时线程就会阻塞，但lock  VARIABLE并不影响正常的对变量的访问。

如果锁往的是一个容器（如哈希或数组），那么其中的每一个元素并没有全部被锁住。比如一个线程中调用lock  @arr，在另一个线程中调用lock $arr[3]时并不会阻塞。

async  BLOCK;

async函数创建并返回一个线程实例，该线程要执行的代码全在BLOCK中，这里BLOCK是个匿名子例程，所以其后一定加分号。

Thread->self

返回调用Thread->self函数的线程实例。

Thread->list

返回non-joined和non-detached线程实例。

cond_waitLOCKED_VARIALBLE

cond_signal  LOCKED_VARIALBLE

cond_broadcast  LOCKED_VARIALBLE

上面3个函数主要用于线程问同步，都以一个已加锁的变量作为输入参数。当一个线程调用cond_wait后阻塞自己；当一个线程发出cond_broadcast后所有阻塞的线程得救；当一个线程发出cond_signal后只有一个阻塞的线程得救，至于是哪一个由系统内部决定。当然只有LOCKED_VARIALBLE参数相同时才为一组，大家才可以在一起玩同步。

yield

把CPU控制权交给另外一个线程，至于是哪个线程依赖于当时的运行环境。

join

等待一个线程结束并返回该线程结束时的返回值。

detach

分离的线程不允许被join。

equal

判断两个线程是否相同。

tid

返回线程的tid。tid是递增的，main线程的tid为0。

done

判断线程是否已经结束。

下面这3个函数在5005threads中还可以用，但是在ithreads中已经不可用了。

lock(\&sub)　　　　eval　　　　flags

<<============================threads::shared============================>>

默认下数据都是线程私有的，新创建的线程获得已有变量的一份私有拷贝。threads::shared用于在线程之间共享数据结构，可共享的数据类型只有6种，标量数据、数组、散列、以及它们的引用。

声明共享变量：

my ($scalar, @array, %hash);             

share($scalar);             

share(@array);           

 share(%hash);

share函数返回共享的值，这通常是一个引用。

也可以在编译时标记变量为共享变量：

my ($var, %hash, @array) :shared;

my ($var, %hash, @array) :shared;
             my $bork;

             # Storing scalars
             $var = 1;
             $hash{'foo'} = 'bar';
             $array[0] = 1.5;

             # Storing shared refs
             $var = \%hash;
             $hash{'ary'} = \@array;
             $array[1] = \$var;

             # 不能把非共享变量的引赋给一个共享变量，下面这3句是错误的
             #   $var = \$bork;                    # ref of non-shared variable
             #   $hash{'bork'} = [];               # non-shared array ref
             #   push(@array, { 'x' => 1 });       # non-shared hash ref


shared_clone REF

 my $obj = {'foo' => [qw/foo bar baz/]};           

 bless($obj, 'Foo');             

my $cpy = shared_clone($obj); 

# Object status (i.e., the class an object is blessed into) is also  cloned.           

print(ref($cpy), "\n");         # Outputs 'Foo'

对于克隆空的数组或散列，下面用法是等价的：

$var = &share([]);   # Same as $var = shared_clone([]);             

$var = &share({});   # Same as $var = shared_clone({});

is_shared VARIABLE

判断变量是否为共享变量，如果是则返回变量的内部ID（类似于refaddr函数），如果不是返回undef。

如果is_shared参数是数组或散列，它并不检查容器中的元素是否为共享变量。如下

my %hash :shared;
             if (is_shared(%hash)) {
                 print("\%hash is shared\n");
             }

             $hash{'elem'} = 1;
             if (is_shared($hash{'elem'})) {                          ##返回undef
                 print("\$hash{'elem'} is in a shared hash\n");
             }


 lock VARIABLE

不能对容器内部的变量进行加锁：

 my %hash :shared;             

$hash{'foo'} = 'bar';           

 #lock($hash{'foo'});          # Error           

 lock(%hash);                  # Works

cond_wait VARIABLE

cond_signal VARIABLE

cond_broadcast VARIABLE

这3个函数就不说了，跟threads里的一样。

cond_wait CONDVAR, LOCKVAR

当有其他线程signal第一个参数变量CONDVAR时，第二个参数变量LOCKVAR被解锁。

cond_timedwait VARIABLE, ABS_TIMEOUT       

cond_timedwait CONDVAR, ABS_TIMEOUT, LOCKVAR

如果signal未到达，而timeout了，同样会把变量解锁。

 　　#  创建一个共享的'Foo' object
         my $foo :shared = shared_clone({});
         bless($foo, 'Foo');

         # 创建一个共享的 'Bar' object
         my $bar :shared = shared_clone({});
         bless($bar, 'Bar');

         # 把'bar' 放到 'foo'里面
         $foo->{'bar'} = $bar;

         # 通过线程重新bless the objects
         threads->create(sub {
             # Rebless the outer object
             bless($foo, 'Yin');

             # 不能直接 rebless the inner object
             #bless($foo->{'bar'}, 'Yang');

             # 重新取回然后 rebless the inner object
             my $obj = $foo->{'bar'};
             bless($obj, 'Yang');
             $foo->{'bar'} = $obj;

         })->join();

         print(ref($foo),          "\n");    # Prints 'Yin'
         print(ref($foo->{'bar'}), "\n");    # Prints 'Yang'
         print(ref($bar),          "\n");    # Also prints 'Yang'


注意：如果你还想使用threads，那么你必须在"use threads::shared"之前就"use threads"，否则会报告异常。

如果你把一个数组、散列或它们的引用share以后，那么容器中的元素都会丢失。

 my @arr = qw(foo bar baz);
         share(@arr);
         # @arr is now empty (i.e., == ());

         # Create a 'foo' object
         my $foo = { 'data' => 99 };
         bless($foo, 'foo');

         # Share the object
         share($foo);        # Contents are now wiped out
         print("ERROR: \$foo is empty\n")
             if (! exists($foo->{'data'}));


所以正确的做法是你应该先把一个空的容器share，然后再往里面添加元素。

<<========================Thread::Semaphore=============================>>

use Thread::Semaphore;
           my $s = Thread::Semaphore->new();
           $s->down();   # P操作
           # The guarded section is here
           $s->up();     # V操作

           # Decrement the semaphore only if it would immediately succeed.
           if ($s->down_nb()) {
               # 邻界区在此
               $s->up();
           }

           # 强制降低信号量即使他成为负数
           $s->down_force();

           # 创建信号量时指定·初始值
           my $s = Thread::Semaphore->new($initial_value);
           $s->down($down_value);
           $s->up($up_value);
           if ($s->down_nb($down_value)) {
               ...
               $s->up($up_value);
           }
           $s->down_force($down_value);


<<===========================Thread::Queue===================================>>

直接看程序是学习语言的快速方法，注释得很清楚：

use strict;
           use warnings;

           use threads;
           use Thread::Queue;

           my $q = Thread::Queue->new();    # 创建一个空的线程队列

           # Worker线程
           my $thr = threads->create(sub {
                                       while (my $item = $q->dequeue()) {
                                           #处理$item
                                       }
                                    })->detach();

           # 向线程发送 work
           $q->enqueue($item1, ...);


           # 计算队列中有多少项
           my $left = $q->pending();

           # 非阻塞地出队
           if (defined(my $item = $q->dequeue_nb())) {
               # 处理 $item
           }

           # 获取队列中的第2项，注意并没有进行出几队操作
           my $item = $q->peek(1);

           # 在队头后面插入两个元素
           $q->insert(1, $item1, $item2);

           # 提取队列中的最后两个元素
           my ($item1, $item2) = $q->extract(-2, 2);


上面代码中出现过的函数我就不介绍了。

下面的数据类型可以放入队列：
普通标题数据;

标量引用;

数组引用;

哈希引用;

以上对象的组合。

my @ary = qw/foo bar baz/;         

 $q->enqueue(\@ary); 　　　　##copy the elements 'foo', 'bar' and 'baz' from @ary into $q。

而对于共享变量，是它的引用进入队列，而没有发生元素的深复制。

my @ary :shared = qw/foo bar baz/;
           $q->enqueue(\@ary);

           my $obj = &shared({});
           $$obj{'foo'} = 'bar';
           $$obj{'qux'} = 99;
           bless($obj, 'My::Class');
           $q->enqueue($obj);


->new()　　　　##创建新队列

->new(LIST)　　##创建队列时压入元素

->enqueue(LIST)　　　　#入队

->dequeue()　　　　#从队中取出一个元素

->dequeue(COUNT)　　　　#从队中取出COUNT个元素，如果COUNT大于队列长度，则阻塞，下面的方法不会阻塞。

->dequeue_nb()       

->dequeue_nb(COUNT)

->pending()

返回队列中元素的个数。

         {
               lock($q);   # 销往队列，以防止其他线程中修改和删除队列元素
               my $item = $q->peek();
               if ($item ...) {
                   ...
               }
           }
           # 离开块之后，队列变量自动解锁


->peek()　　　　　　#取出队首元素，并没有出险

->peek(INDEX)　　　　#取出指定下标的队列元素，INDEX为负数时是从队尾开始数起

->insert(INDEX,LIST)　　　　#在指定的位置上插入一组元素，队首元素的INDEX为0

->extract()

->extract(INDEX)       

->extract(INDEX, COUNT)

删除并返回指定的元素。












