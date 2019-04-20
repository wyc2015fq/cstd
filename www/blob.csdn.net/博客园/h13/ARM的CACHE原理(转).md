# ARM的CACHE原理(转) - h13 - 博客园
对大量典型程序运行情况的分析结果表明，在一个较短的时间间隔内，由程序产生的地址往往集中在存储器逻辑地址空间的很小范围内。指令地址的分布本来就是连 续的，再加上循环程序段和子程序段要重复执行多次。
Cache的工作原理是基于程序访问的局部性。 
对大量典型程序运行情况的分析结果表明，在一个较短的时间间隔内，由程序产生的地址往往集中在存储器逻辑地址空间的很小范围内。指令地址的分布本来就是连 续的，再加上循环程序段和子程序段要重复执行多次。因此，对这些地址的访问就自然地具有时间上集中分布的倾向。 
数据分布的这种集中倾向不如指令明显，但对数组的存储和访问以及工作单元的选择都可以使存储器地址相对集中。这种对局部范围的存储器地址频繁访问，而对此范围以外的地址则访问甚少的现象，就称为程序访问的局部性。
根据程序的局部性原理，可以在主存和CPU通用寄存器之间设置一个高速的容量相对较小的存储器，把正在执行的指令地址附近的一部分指令或数据从主存调入这 个存储器，供CPU在一段时间内使用。这对提高程序的运行速度有很大的作用。这个介于主存和CPU之间的高速小容量存储器称作高速缓冲存储器 (Cache)。 
系统正是依据此原理，不断地将与当前指令集相关联的一个不太大的后继指令集从内存读到Cache，然后再与CPU高速传送，从而达到速度匹配。
CPU对存储器进行数据请求时，通常先访问Cache。由于局部性原理不能保证所请求的数据百分之百地在Cache中，这里便存在一个命中率。即CPU在任一时刻从Cache中可靠获取数据的几率。 
命中率越高，正确获取数据的可靠性就越大。一般来说，Cache的存储容量比主存的容量小得多，但不能太小，太小会使命中率太低；也没有必要过大，过大不仅会增加成本，而且当容量超过一定值后，命中率随容量的增加将不会有明显地增长。
只要Cache的空间与主存空间在一定范围内保持适当比例的映射关系，Cache的命中率还是相当高的。 
一般规定Cache与内存的空间比为4：1000，即128kB Cache可映射32MB内存；256kB Cache可映射64MB内存。在这种情况下，命中率都在90％以上。至于没有命中的数据，CPU只好直接从内存获取。获取的同时，也把它拷进 Cache，以备下次访问。
**Cache的基本结构 **
Cache通常由相联存储器实现。相联存储器的每一个存储块都具有额外的存储信息，称为标签(Tag)。当访问相联存储器时，将地址和每一个标签同时进行比较，从而对标签相同的存储块进行访问。Cache的3种基本结构如下：
**DCACHE**
ARM cache架构由cache存储器和写缓冲器（write-buffer）组成，其中写缓冲器是CACHE按照FIFO原则向主存写的缓冲处理器。
一般来说CACHEABILITY和BUFFERABILITY都是可以配置的，所以，一块存储区域可以配置成下面4种方式：NCNB CNB NCB CB。 在实际应用当中，可以根据需要对主存进行配置。对I/O MAP来说，一般都需要采用NCNB方式，以保证对I/O的正确操作。而其他的存储区域一般都可以配置成CB方式，以获取最好的性能。
引入CACHE和WRITE BUFFER是为了提高存储访问的 速度，提供系统性能。如果CACHE打开的话，CPU读写主存的时候，都是通 
过CACHE进行的。进行读操作的时候，如果在CACHE里面找到了所需的内容 (CACHE HIT)，直接从CACHE里读取；如果要读的内容不在CACHE上的时候 (CACHE MISS)，先把所需的内容装载到CACHE里，在从CACHE上读取。进行 写操作的时候，数据先写到CACHE上。具体又可以分为WRITE THROUGH和 WRITE BACK两种方式。如果是WRITE THROUGH方式的话，每次写操作都通 
过CACHE+WRITE BUFFER把数据直接写到主存当中去；如果是WRITE BACK 方式的话，数据最初只是写到CACHE上，必要的时候(CACHE REPLACEMENT) 在将CACHE上的数据通过WRITE BUFFER实际回写到主存当中去。
DCaches使用的是虚拟地址，它的大小是16KB,它被分成512行（entry），每行8个字（8 words,32Bits）。每行有两个修改标志位（dirty bits），第一个标志位标识前4个字，第二个标志位标识后4个字，同时每行中还有一个TAG 地址（标签地址）和一个valid bit。 
    与ICaches一样，系统上电或重起（Reset）的时候，DCaches功能是被关闭的，我们必须往Ccr bit置1去开启它，Ccr bit在CP15协处理器中控制寄存器1的第2位（关闭DCaches功能则是往该位置0）。与ICaches不同，DCaches功能是必须在MMU开 启之后才能被使用的。
我们现在讨论的都是DCaches,你可能会问那Write Buffer呢？他和DCaches区别是什么呢？ 其实DCaches和Write Buffer两者间的操作有着非常紧密的联系，很抱歉，到目前为止我无法说出他们之间有什么根本上的区别（-_-!!!），但我能告诉你什么时候使用的是 DCaches,什么时候使用的是Write Buffer.系统可以通过Ccr bit对Dcaches的功能进行开启与关闭的设定，但是在s3c2410中却没有确定的某个bit可以来开启或关闭Write Buffer… 你可能有点懵…我们还是来看一张表吧，这张表说明了DCaches,Write Buffer和CCr,Ctt (descriptor中的C bit),Btt(descriptor中的B bit)之间的关系，其中“Ctt and Ccr”一项里面的值是 Ctt与Ccr进行逻辑与之后的值（Ctt&&Ccr）.
![](http://www.rosoo.net/Files/allimg/2011/0910304L5-0.jpg)
从上面的表格中我们可以清楚的知道系统什么时候使用的是DCaches,什么时候使用的是Write Buffer，我们也可以看到DCaches的写回方式是怎么决定的（write-back or write-througth）。
在这里我要对Ctt and Ccr=0进行说明，能够使Ctt and Ccr=0的共有三种情况，分别是
Ctt =0， Ccr=0
Ctt =1， Ccr=0
Ctt =0， Ccr=1
我们分别对其进行说明。
情况1（Ctt =0， Ccr=0）：这种情况下CPU的DCaches功能是关闭的（Ccr=0），所以CPU存取数据的时候不会从DCaches里进行数据地查询，CPU直接去内存存取数据。 
情况2（Ctt =1， Ccr=0）：与情况1相同。 
情况3（Ctt =0， Ccr=1）：这种情况下DCaches功能是开启的，CPU读取数据的时候会先从DCaches里进行数据地查询，若DCaches中没有合适的数据， 则CPU会去内存进行读取，但此时由于Ctt =0（Ctt 是descriptor中的C bit,该bit决定该descriptor所描述的内存区域是否可以被Cache）,所以CPU不会把读取到的数据Cache到DCaches(不发生 linefill). 
到此为止我们用两句话总结一下DCaches与Write Buffer的开启和使用：
1．DCaches与Write Buffer的开启由Ccr决定。
2．DCaches与Write Buffer的使用规则由Ctt和Btt决定。
**ICACHE**
当系统上电或重起（Reset）的时候，ICaches功能是被关闭的，我们必须往lcr bit置1去开启它，lcr bit在CP15协处理器中控制寄存器1的第12位（关闭ICaches功能则是往该位置0）。ICaches功能一般是在MMU开启之后被使用的（为了 降低MMU查表带来的开销）,但有一点需要注意，并不是说MMU被开启了ICaches才会被开启，正如本段刚开始讲的，ICaches的开启与关闭是由 lcr bit所决定的，无论MMU是否被开启，只要lcr bit被置1了，ICaches就会发挥它的作用。
大家是否还记得discriptor（描述符）中有一个C bit我们称之为Ctt,它是指明该描述符描述的内存区域内的内容（可以是指令也可以是数据）是否可以被Cache，若Ctt=1,则允许Cache,否 则不允许被Cache。于是CPU读取指令出现了下面这些情况：
如果CPU从Caches中读取到所要的一条指令（cache hit）且这条指令所在的内存区域是Cacheble的（该区域所属描述符中Ctt=1）,则CPU执行这条指令并从Caches中返回（不需要从内存中读取）。 
若CPU从Caches中读取不到所要的指令（cache miss）而这条指令所在的内存区域是Cacheble的（同第1点），则CPU将从内存中读取这条指令，同时，一个称为“8-word linefill”的动作将发生，这个动作是把该指令所处区域的8个word写进ICaches的某个entry中，这个entry必须是没有被锁定的 （对锁定这个操作感兴趣的朋友可以找相关的资料进行了解） 
若CPU从Caches中读取不到所要的指令（cache miss）而这条指令所在的内存区域是UnCacheble的（该区域所属描述符中Ctt=0），则CPU将从内存读取这条指令并执行后返回（不发生linefill）
通过以上的说明，我们可以了解到CPU是怎么通过ICaches执行指令的。你可能会有这个疑问，ICaches总共只有512个条目（entry），当 512个条目都被填充完之后，CPU要把新读取近来的指令放到哪个条目上呢？答案是CPU会把新读取近来的8个word从512个条目中选择一个对其进行 写入，那CPU是怎么选出一个条目来的呢？这就关系到ICaches的替换法则（replacemnet algorithm）了。 ICaches的replacemnet algorithm有两种，一种是Random模式另一种Round-Robin模式，我们可以通过CP15协处理器中寄存器1的RR bit对其进行指定（0 = Random replacement 1 = Round robin replacement），如果有需要你还可以进行指令锁定（INSTRUCTION CACHE LOCKDOWN）。
**虚拟cache**
Cache 位于MMU前面靠近CPU称为逻辑CACHE又叫虚拟Cache。CPU可以直接访问CACHE的数据，而ARM 11（ARMV6）的结构是CACHE   在MMU后面CPU访问CACHE要通过MMU地址转换
在DM6446的core用的是哈佛结构，即把CACHE分为8K的D-CACHE（数据CACHE）和16K的I-cache(指令CACHE)
一个完整的CACHE分为CACHE控制器和CACHE存储器
**例子**
Davinci DM6446 D-cache行应为512行 ，cache存储器主要分为三个部分：目录存储段（driectory-story），状态信息段（status information）和数据项段（data section）每一行cache都包括这三部分。Cache用目录存储段来存储主存的地址，数据项段存放的是主存的数据，在cache中用状态信息段来 记录状态信息，其中v表示有效位，d表示脏位，有效位记录当前cache行是活动的，cache行的数据和主存中的数据是一致的，处理器可以读取。脏位则 表示cache行的数据和主存中的数据不一致。
在读写请求到达存储器前会被CACHE捕获，cache存储器将该请求分成三部分标签，组索引和数据索引域，cache通过组索引域确定可能包含地 址和数据cache的行，cache存储器检查匹配的CACHE       行的状态标签，如果是V表示（cache hit）命中，否则cache失效（cache miss）在cache失效时从主存吧cache行考到CACHE存储器
主存中的部分内容存放在cache中的最简单方式是直接映射，在一个直接映射中，主存的地址唯一对应cache行，因为主存容量很大所以主存的很多地址映射到同一个cache行
见下图：
在DM6446 中内存为128M （bootargs 设为128M）8K的D-CACHE则128×1024/8 = 16384映射一个cache行。由于cache的速度大大大于低速的主存速度，因此需要写缓冲器。
**Cache与DRAM存取的一致性**
在CPU与主存之间增加了Cache之后，便存在数据在CPU和Cache及主存之间如何存取的问题。读写各有2种方式。
**贯穿读出式(Look Through)**
该方式将Cache隔在CPU与主存之间，CPU对主存的所有数据请求都首先送到Cache，由Cache自行在自身查找。如果命中，则切断CPU对主存的请求，并将数据送出；不命中，则将数据请求传给主存。 
该方法的优点是降低了CPU对主存的请求次数，缺点是延迟了CPU对主存的访问时间。 
旁路读出式(Look Aside) 
在这种方式中，CPU发出数据请求时，并不是单通道地穿过Cache，而是向Cache和主存同时发出请求。由于Cache速度更快，如果命中，则 Cache在将数据回送给CPU的同时，还来得及中断CPU对主存的请求；不命中，则Cache不做任何动作，由CPU直接访问主存。
它的优点是没有时间延迟，缺点是每次CPU对主存的访问都存在，这样，就占用了一部分总线时间。
**写穿式(Write Through)**
任一从CPU发出的写信号送到Cache的同时，也写入主存，以保证主存的数据能同步地更新。
它的优点是操作简单，但由于主存的慢速，降低了系统的写速度并占用了总线的时间。
**回写式(Copy Back)**
为了克服贯穿式中每次数据写入时都要访问主存，从而导致系统写速度降低并占用总线时间的弊病，尽量减少对主存的访问次数，又有了回写式。
它是这样工作的：数据一般只写到Cache，这样有可能出现Cache中的数据得到更新而主存中的数据不变(数据陈旧)的情况。但此时可在Cache 中设一标志地址及数据陈旧的信息，只有当Cache中的数据被再次更改时，才将原更新的数据写入主存相应的单元中，然后再接受再次更新的数据。这样保证了 Cache和主存中的数据不致产生冲突。
**ARM cache 策略**
Cache的写策略分为直写策略和回写策略。同时向cache行和相应的主存位置写数据，同时更新这两个地方的数据的方法称为直写策略 （writethrough），把数据写入cache行，不写入主存的或者只有当cache被替换时或清理cache行时才写入主存的策略称为回写策略 （writeback）。采用回写策略时，当处理器cache命中，只向cache存储器写数据，不写入主存，主存里的数据就和cache里不一 致，cache里的数据是最新的，主存里的数据是早前的。这就用cache存储器信息状态标志位了，当向cache存储器里某行写数据时，置相应行的信息 标志脏位为1，那么主控制器下次访问cache存储器就知道cache里有主存没有的数据了，把数据写回到主存中去。
当一个cache访问失效时，cache控制器必须从当前有效行中取出一个cache行存储从主存中取到的信息，被选中替换的cache行称为丢弃者，如 果这个cache行中脏位为1则应把该cache行中的数据回写到主存中，而替换策略决定了那个cache行会被替换，在arm926ejs中ARM支持 两种策略：轮转策略和伪随机策略。轮转策略就是取当前cache行的下一行，伪随机策略是控制器随机产生一个值。
当cache失效时，ARM采取两种方式分配cache行，一种是读操作（read-allocate）还有一种是读-写分配策略（read- write-allocate），当cache未命中时对于读操作策略，在对cache存储器读操作时才会分配cache行
**全相联Cache**
在全相联Cache中，存储的块与块之间，以及存储顺序或保存的存储器地址之间没有直接的关系。程序可以访问很多的子程序、堆栈和段，而它们是位于主存储器的不同部位上。
因此，Cache保存着很多互不相关的数据块，Cache必须对每个块和块自身的地址加以存储。当请求数据时，Cache控制器要把请求地址同所有地址加以比较，进行确认。
这种Cache结构的主要优点是，它能够在给定的时间内去存储主存器中的不同的块，命中率高；缺点是每一次请求数据同Cache中的地址进行比较需要相当的时间，速度较慢。
**直接映像Cache**
直接映像Cache不同于全相联Cache，地址仅需比较一次。 
    在直接映像Cache中，由于每个主存储器的块在Cache中仅存在一个位置，因而把地址的比较次数减少为一次。其做法是，为Cache中的每个块位置分配一个索引字段，用Tag字段区分存放在Cache位置上的不同的块。
    单路直接映像把主存储器分成若干页，主存储器的每一页与Cache存储器的大小相同，匹配的主存储器的偏移量可以直接映像为Cache偏移量。Cache的Tag存储器(偏移量)保存着主存储器的页地址(页号)。 
    以上可以看出，直接映像Cache优于全相联Cache，能进行快速查找，其缺点是当主存储器的组之间做频繁调用时，Cache控制器必须做多次转换。 
**组相联Cache **
    组相联Cache是介于全相联Cache和直接映像Cache之间的一种结构。这种类型的Cache使用了几组直接映像的块，对于某一个给定的索引号，可以允许有几个块位置，因而可以增加命中率和系统效率。

