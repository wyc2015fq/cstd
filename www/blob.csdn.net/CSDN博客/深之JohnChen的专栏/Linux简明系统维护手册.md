# Linux简明系统维护手册 - 深之JohnChen的专栏 - CSDN博客

2005年12月23日 11:35:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4821


**Linux简明系统维护手册**
摘要
    　　本文是长期工作经验的总结，说明了Linux下各种主要网络应用的配置实例，对接触Linux时间不长的同学们很有参考价值。注意：其中有些应用还是很复杂的呀。文中VPN技术部分要感谢老井无私的提供支持，这是我们共同的完成的。(2002-11-01 16:50:49)

By atan

作者：阿谭

前言

　　有同事已经通过艰苦的工作完成了长达好几百页的关于Linux的文档。事实上，在网上还有超过好几万页的各种关于Linux的技术精华，还不包括英文的。有数万程序员和网络专家在为这个软件努力工作。这就是自由的魅力。

　　我使用Linux有不少时间了，期间也经历过无数的挫折，同时也得到过很多高手的帮助。我一向喜欢言简意赅的文字。通过下面的简单句子和代码，我希望能够表达出我使用Linux的体会。也许本文的结构比较凌乱，但我希望它的每一句话都是对你有用的。当然，你要知道一些更详细的东西，请参阅我同事写的文档。

　　当然，能够通过查阅这个文档，使你的工作顺利是我最大的心愿。

一、关于Linux的误区

　　1、Linux干什么用？

　　首先，很少有人用Linux做桌面系统，因为Windows桌面系统已经足够好了。我们主要把Linux用作网络服务器的操作系统。如果你和你的应用符合下面的几条，那么你合适使用Linux：

　　*不愿意花钱在操作系统和网络应用上。

　　*用D版系统受良心谴责或害怕承担责任。

　　*想得到和花钱的系统差不多的性能。

　　*在用户面前显得水平较高。

　　*认识几个弄Linux的高手，或者知道在哪里能够找到问题答案。

　　2、每种Linux发行包差别很多吗？

　　如果你在上世纪80年代末到95年之前这段时间大量使用PC机的话，那对于Linux你应该很好理解：它就象一个有着32位抢先多任务核心的 DOS，其区别在于外表看起来像是一种UNIX。Linux是好多人用智慧堆砌起来的，初学者经常问：“RedHat好还是Slackware好？”。这个人同时还面临上百种选择，这的确让人头痛。大部分Linux发行包实际上很简单，几乎所有的发行包都有相同的内核（也许是不同版本），外部命令和数据文件的差别也仅仅局限于在文件系统中的存放位置不同（这导致了某些兼容性问题，因为在某些地方找不到特定的文件嘛）。另外就是有些发行包为了显示自己的卓而不群，在发行包中增加了一些自己的实用程序。实际上动手修改内核的厂商不算太多，因为这样会内核官方版本的不断更新而手忙脚乱。当然有一些嵌入式系统开发商在Linux基础上做了自己的系统另说。

　　拥有众多的发行包是Linux的一个很讨厌的问题，在这一点上，我更喜欢FreeBSD，因为它只有一个官方发行版本，也运行得比较稳定。可给 Linux做软件的程序员太多了，Linux上面的资料也更多。记住：当一个家伙跟你说用哪个发行版本无所谓的时候，那他就是专家。

　　3、Linux业余爱好者的东西吗？

　　这样的宣传给了很多人这样的印象。其实，Linux是一个严肃的操作系统，拥有一切操作系统共有的特性。它可以用在很多严肃的场合。

　　4、Linux性能比Windows更好吗？

　　这里的答案会让你失望，据我以往的实际测试，2.2版本的Linux核心的性能比windows2000要低。值得欣慰的是2.4版本的核心性能有大幅度提高基本上接近了Windows2000。但Windows2000复杂的图形界面占用了相当的系统资源。

　　5、Linux比Windows更复杂更专业吗？

　　从复杂度上说，Windows2000要比Linux复杂很多。在核心技术上，Windows也更先进。

　　6、Linux更可靠吗？

　　应该从两个角度评价系统的可靠性。（1）系统自身的可靠性，两者象差不多。人们普遍认为的Windwos不如Linux可靠的印象来源于 Windows众多的良莠不齐的软件。当然，Linux比Windows结构简单，在一定程度上也可以提高可靠性。（2）从安全性角度来说，Linux远远高于Windows系统，道理很简单：树大招风嘛。另外，Linux的源代码开放机制使得漏洞发现和消灭得很快。而Windows相对就显得比较官僚了。这里补充一点，我用过的最可靠的系统是BSD类的OS，可靠性和安全性都可圈可点。

　　7、Linux真的不用花一分钱吗？

　　大部分的发行包需要花少量的钱购买。当然你也可以从网络上DOWN。其实这些发行包未必符合你的应用，一般都需要经过一些改动和定制才能运行好。当然，花的钱跟Windows没法比。

二、Linux安装注意事项

　　现在的大部分发行包都很容易安装，基本上都是图形向导式的，每个发行包都不太相同，也无法一一描述，如果你连安装都不明白，那就别用Linux了。这里仅仅说明几个注意的问题。

　　1、如果要和Windows混装，一定要先装Windows，后装Linux。

　　2、分区的时候，根据计划安装的软件灵活决定分区。一般分区有：/，/root，/usr，/var和swap分区，其中swap分区类型和其他不同需要专门选择一下，它的空间一般是物理内存的两倍即可。/root分区是紧急情况root用的，有64兆就行了。

　　3、当然，为了方便，你可以安装发行包给你做好的软件。但我安装Linux的习惯做法是开始就安装一个最小系统，然后安装所有的编译工具。安装编译工具的方法一般都通过你用的发行包的包管理工具实现，这样最方便。比如在TurboLinux中，运行/usr/sbin/Turbopkg就可以了。在管理工具中选择所有关于Developoment相关的选项或gcc相关选项，一般就可以了。

　　4、lilo一定要装。否则一旦出现分区问题都不好办。

　　5、如果和Windows混装的时候，装完了系统起不来了。可以用98启动盘启动，然后用FDISK /MBR回复主引导分区来保住Windows分区。

　　在安装了最小系统和全套的编译器后，我们应该得到一个比较干净的系统。在这个系统上，可以下载和安装各种应用软件了。

三、基本使用

　　1、文件系统

　　无论有多少个分区，整个文件系统都是一棵完整的树结构。经常用到的目录有：

　　(1)/usr：存放各种应用程序文件用。其中的/usr/local用来存放后安装的软件

　　(2)/var：用来存放所有的数据文件

　　(3)/proc：里面的文件用来表示系统内核的各种配置和状态。这一块不是真正的文件系统，而是内存里面的各种数据。可以从这里得到一些常用的系统信息。比如CPU内存什么的。

　　(4)/etc：这里是放置所有的系统配置文件。一般情况下，后安装的软件的配置文件不会放到这里。除非你是用RPM安装的随发行包配的软件或者故意这样做。我不喜欢把不同软件的文件混在一起，因此通常让不同的软件的配置文件放在各自的目录中。

　　2、常用的命令

　　这里我列出一些经常用的命令，具体的用法可以参见相关手册页（希望你的英文不象我这么差）。

　　vi(一定要会用这个你非常讨厌的东西，如果你屈服了，可以选择另一个叫做pico的编辑器，它有点象DOS的edit)

　　head(用来看很长的文件用)

　　tail(同上)

　　netstat(看网络状态)

　　tar(解开.tar.gz的压缩包)

　　ps(察看进程)

　　kill(干掉进程)

　　top(看系统状况)

　　shutdown(关闭系统)

　　cat(看文件内容)

　　ping (看网络连通状况)

　　ftp(传输文件)

　　man(手册)

　　上面这些命令都是最最常用的，一定要把基本用法记牢。事实上，每人能记住全部命令或一个命令的全部参数，记住几个常用的就行了。其他的查书。

　　3、内核升级

　　如果你不愿意冒险，可以跳过这个部分。不过根据我实际使用的经验，2.4.x版本的内核性能比2.2.x的性能至少提高40%，还是很值得一试的。现在最新的发行包几乎都带有新的2.4内核，但是版本号想对保守。这里，我的建议是在决定升级任何部分的时候，一定要首先看一下所谓的 “currect version release note”信息，里面告诉你本次版本升级都有些什么改动。如果改动的内容不涉及你当前的环境或需求，只是增加了一些邪门设备的驱动，你大可不必急着升级。补充一句话：Linux的内核版本号的中间一位是奇数的是非稳定版本，比如2.3，是偶数的才是稳定版本。

　　升级内核的步骤如下：

　　(1)首先在网上找到你打算升级的内核文件，一般名字是：linux-2.x.xx.tar.gz，把这个文件复制到/usr/src。(不知道在哪里找？看看：[http://www.kernel.org](http://www.kernel.org/) ，最好找到.tar.gz格式的，比如这里：[http://www.kernel.org/pub/linux/kernel/v2.x/](http://www.kernel.org/pub/linux/kernel/v2.x/)）。下面的升级举例是从2.2.18升级到2.5.7（这是最新的内核了，你使用其它的版本别忘了改变版本号）

　　(2)解压缩：tar zxvf linux-2.5.7.tar.gz后生成一个目录：linux-2.5.7。

　　(3)进入/usr/src，用ls –l 命令可以看到src里面有一个连接，类似于：linux -> linux-2.2.18/ （连接指向你当前用的内核版本）。首先删除这个连接（rm linux），用ln -s linux-2.5.7 linux 命令重建连接。我想你一定不会连这里的版本号都照抄吧？

　　(4)进入linux-2.5.7 目录，如果不是第一次编译这个内核，最好用命令：make mrproper 删掉上次编译产生的.o文件等，当然以前保存的配置也丢了。

　　(5)用命令：make menuconfig命令调整内核配置以适应你目前的环境，记住，不懂的配置不要乱动。首要任务是调整各种硬件的环境，比如SCSI卡：SCSI support/SCSI low-level drivers，不知道现在的SCSI卡型号？在/proc/scsi中可以找到。还有网卡，在Network Device Support 里面。不知道现在的网卡型号？在这个文件里面写着呢：/proc/net/PRO_LAN_Adapters/eth0.info或/proc/pci文件中也可以找到 。在menuconfig中用空格键改变选项状态，前面的<*>表示该功能编译在内核中，主要是运行速度快。< >表示不需要该功能。表示把该功能编译成模块，通常为了减小尺寸，和更换方便而把某些驱动编译成模块。

　　(6)下面的事情比较程序化，照着做：make dep （检查文件的完整性，过程很复杂）

　　(7)make bzImage 注意大小写（真正开始编译了耶！我觉着这个最过瘾了，屏幕“哗啦哗啦”的，这时候你会感觉到，多少不知名的程序员在这个复杂的系统中贡献了自己的一份力量）

　　(8)make modules (编译那些被标记为的功能模块或驱动程序)

　　(9)make modules_install（把编译好的模块复制到指定地点，一般是：/lib/modules/ 。注意：不同版本号的模块是完全分开在不同目录中的，因为modules跟内核是紧密相关的，混用容易导致系统崩溃）

　　(10)cd /usr/src/linux-2.5.7/arch/i386/boot ，用命令：cp /usr/src/linux2.5.7/arch/i386/boot/bzImage /boot/vmlinuz-2.5.7 把文件bzImage文件改名成vmlinuz-2.5.7复制到/boot/

　　(11)cp /usr/src/linux-2.5.7/System.map /boot/System.map-2.5.7

　　(12)进入/boot目录，rm System.map

　　(13)在/boot目录中运行ln System.map-2.5.7 System.map 10-13两个步骤每次重新编译都要做。

　　(14)我开始提醒过的，但愿你安装了lilo （否则你就高唱：“神啊，救救我吧！”），编辑文件：/etc/lilo.conf ，做下面的编辑：

　　boot=/dev/sda
　　map=/boot/map
　　install=/boot/boot.b
　　prompt
　　timeout=50
　　lba32
　　default=linux-2.5.7

　　image=/boot/vmlinuz
        label=linux
        initrd=/boot/initrd
        read-only
        root=/dev/sda5
　　image=/boot/vmlinuz-2.5.7
        label=linux-2.5.7
        initrd=/boot/initrd
        read-only
        root=/dev/sda5

　　黑体字部分是后加的和修改的。万一你没有安装lilo，你只能修改/boot下面的连接指向你新的System.map和vmlinuz ， 而不用修改lilo了。这样也能启动，但是一旦出现问题，你的机器就启动不了了。

　　(15)执行命令：lilo (用来更新lilo数据)，注意输出的结果：带星号的是缺省的。

　　(16)祈祷，然后reboot 系统重新启动后，用uname –a 可看到新的内核起作用了。如果出现任何问题不能引导了，你需要在lilo的引导界面选择原来的内核启动，进入系统后重新改变内核参数，调整硬件或其他配置。然后重复前面所有过程。

四、主要的软件安装

　　前面说过，我的习惯是，装系统的时候就安装一个内核和编译系统，得到了一个干净的系统后，再安装其他的软件就比较清晰。另一点，除非搞不到源代码版本，一般情况下我都会下载官方版本的源代码文件，来重新编译之。同样的，选择软件版本要根据该软件的Release Note来决定，并非越新越好。这就是为什么很多软件都是几个版本齐头并进的发展之原因。这些软件不同版本之间的安装方法会有少许不同，安装前一定要仔细阅读目录中的README文件和INSTALL文件，否则这些少许不同会造成不少得麻烦。下面描述的安装过程都核版本号紧密相关。另外，所有的软件都把程序文件的包拷贝到/usr/local/src目录中再解压缩，这是习惯。因为在Linux中习惯于把后来的软件装在/usr/local中。

　　(1)安装DNS服务器

　　1、从[www.isc.org](http://www.isc.org/)下载bind 域名服务器软件。我们这里用的是bind8.3.0，bind8分支和bind9分支是并行发展的。根据自己的实际情况选择合适的bind版本。

　　2、mkdir /usr/src/bind83

　　3、下载的文件叫bind-src.tar.gz，复制到/usr/local/src/bind83目录下。

　　4、cd /usr/local/src/bind83

　　5、tar zxvf bind-src.tar.gz

　　6、cd src

　　7、make stdlinks

　　8、make clean

　　9、make depend

　　10、make all

　　11、make install

　　12、编辑配置文件：/etc/named.conf内容如下，需要更改的部分用黑体标出：

　　/*
　　 * This is a worthless, nonrunnable example of a named.conf file that has
　　 * every conceivable syntax element in use.  We use it to test the parser.
　　 * It could also be used as a conceptual template for users of new features.
　　 */

　　/*
　　 * C-style comments are OK
　　 */

　　// So are C++-style comments

　　# So are shell-style comments

　　// watch out for ";" -- it's important!

　　options {
       directory "/var/named";                 
　　　　　　// use current directory
       named-xfer "/usr/libexec/named-xfer";   
　　　　　　// _PATH_XFER
       dump-file "named_dump.db";             
　　　　　　// _PATH_DUMPFILE
      pid-file "/var/run/named.pid";          
　　　　　  // _PATH_PIDFILE
      statistics-file "named.stats";          
　　　　　　// _PATH_STATS
      memstatistics-file "named.memstats";     
　　　　　　// _PATH_MEMSTATS
      check-names master fail;
      check-names slave warn;
      check-names response ignore;
      host-statistics no;
      deallocate-on-exit no;                  
　　　　　　// Painstakingly deallocate all
　　　　　　// objects when exiting instead of
　　　　　　// letting the OS clean up for us.
　　　　　　// Useful a memory leak is suspected.
            // Final statistics are written to the
            // memstatistics-file.
      datasize default;
      stacksize default;
      coresize default;
      files unlimited;
      recursion yes;
      fetch-glue yes;
      fake-iquery no;
      notify yes;    
　　　 　// send NOTIFY messages.  You can set
         // notify on a zone-by-zone
         // basis in the "zone" statement
         // see (below)
      serial-queries 4;               
　 　　　// number of parallel SOA queries
         // we can have outstanding for master
         // zone change testing purposes
      auth-nxdomain yes;             
         // always set AA on NXDOMAIN.
         // don't set this to 'no' unless
         // you know what you're doing -- older
         // servers won't like it.
      multiple-cnames no;             
　　     // if yes, then a name my have more
        // than one CNAME RR.  This use
        // is non-standard and is not
        // recommended, but it is available
        // because previous releases supported
        // it and it was used by large sites
        // for load balancing.
     allow-query { any; };
     allow-transfer { any; };
      transfers-in 10;               
　　　　 // DEFAULT_XFERS_RUNNING, cannot be
　　    // set > than MAX_XFERS_RUNNING (20)
      transfers-per-ns 2;             
         // DEFAULT_XFERS_PER_NS
      transfers-out 0;                
        // not implemented
      max-transfer-time-in 120;     
        // MAX_XFER_TIME; the default number
        // of minutes an inbound zone transfer
        // may run.  May be set on a per-zone
        // basis.
     transfer-format one-answer;
     query-source address * port *;
　　/*
     * The "forward" option is only meaningful if you've defined
     * forwarders.  "first" gives the normal BIND
     * forwarding behavior, i.e. ask the forwarders first, and if that
     * doesn't work then do the full lookup.  You can also say
     * "forward only;" which is what used to be specified with
     * "slave" or "options forward-only".  "only" will never attempt
　　 * a full lookup; only the forwarders will be used.
　　 */
    forward first;
    forwarders { };                
　　　　// default is no forwarders
      topology { localhost; localnets; };     
　　　　// prefer local nameservers
      listen-on port 53 { any; };     
　　　　// listen for queries on port 53 on
　　    // any interface on the system
　　    // (i.e. all interfaces).  The
　　    // "port 53" is optional; if you
　　    // don't specify a port, port 53
　　    // is assumed.
    /*
     * Interval Timers
     */
    cleaning-interval 60;          
　　　 // clean the cache of expired RRs
　　    // every 'cleaning-interval' minutes
    interface-interval 60;          
　　　　// scan for new or deleted interfaces
　　    // every 'interface-interval' minutes
    statistics-interval 60;         
　　　　// log statistics every 
　　    // 'statistics-interval' minutes
    maintain-ixfr-base no;   
　　　　// If yes, keep transaction log file for IXFR
    max-ixfr-log-size 20;        
　　　　// Not implemented, maximum size the 
　　    // IXFR transaction log file to grow
　　};

　　/*
　　 * Control listeners, for "ndc".  Every nameserver needs at least one.
　　 */
　　controls {
        inet * port 52 allow { any; };
             // a bad idea
        unix "/var/run/ndc" perm 0600 owner 0 group 0; 
　　　　　　 // the default
　　};

　　zone "rd.xxx.com" in {
        type master;                    
　　　　　　// what used to be called "primary" 
        file "rd.xxx.com.db";
        check-names fail;
        allow-update { none; };
        allow-transfer { any; };
        allow-query { any; };
        // notify yes;                  
　　　　　　// send NOTIFY messages for this
            // zone?  The global option is used
            // if "notify" is not specified
            // here.
        also-notify { };                
　　　　　　// don't notify any nameservers other
            // than those on the NS list for this
            // zone
　　};

　　zone "223.99.211.in-addr.arpa" in {
        type master;         
           // what used to be called "secondary" 
        file "21.9.22.db";
        };

　　zone "0.0.127.in-addr.arpa" in {
        type master;
        file "127.0.0.db";
　　};

　　zone "." in {
        type hint;                     
　　　　　　 // used to be specified w/ "cache"
        file "named.root";
　　};

　　logging {
        /*
         * All log output goes to one or more "channels"; you can make as
         * many of them as you want.
         */

     channel syslog_errors {         
　　　　　　// this channel will send errors or
        syslog user;            
　　　　　　// or worse to syslog (user facility)
        severity error;
     };

    category parser {
       syslog_errors;
          // you can log to as many channels
       default_syslog;     
　　　　  // as you want
    };

    category lame-servers { null; };       
　　　　 // don't log these at all

    channel moderate_debug {
       severity debug 3;              
　　　　 // level 3 debugging to file
       file "foo";                     
　　　　 // foo
       print-time yes;                 
　　　　// timestamp log entries
       print-category yes;            
　　　　 // print category name
       print-severity yes;             
　　　　 // print severity level
         /*
          * Note that debugging must have been turned on either
          * on the command line or with a signal to get debugging
          * output (non-debugging output will still be written to
          * this channel).
          */
        };

    /*
     * If you don't want to see "zone XXXX loaded" messages but do
     * want to see any problems, you could do the following.
     */

    channel no_info_messages {
            syslog;
            severity notice;
    };

    category load { no_info_messages; };

    /*
     * You can also define category "default"; it gets used when no
     * "category" statement has been given for a category.
     */

    category default {
          default_syslog;
          moderate_debug;
    };

};

　　13、在/var/named/中生成/etc/named.conf中标记的文件：rd.xxx.com.db，内容如下，需要修改和调整相应部分：

　　;Authoriative data for rd.xxx.com
　　;
　　$TTL 3600
　　@  IN  SOA  compaq.rd.xxx.com. tandongyu.rd.xxx.com. (
　　　　20020101        ;Serial
　　　　3600            ;Refresh 1 hour
　　　　900             ;Retry 15 mins
　　　　604800          ;Expire 7 days
　　　　86400)          ;Mini 24 hours
　　;Name server NS records
　　@  IN    NS      compaq.rd.xxx.com.
　　;Mail Exchange (MX) records
　　rd.xxx.com.   IN  MX      0       compaq
　　;Address (A) records.
　　localhost       IN      A       127.0.0.1
　　compaq          IN      A       21.9.22.9
　　tls65           IN      A       21.9.22.8
　　fbsd            IN      A       21.9.22.7

　　14、在/var/named/中生成/etc/named.conf中标记的文件：21.9.22.db，内容如下，你需要修改相应部分：

　　;
　　;
　　$TTL 3600
　　@  IN SOA  compaq.rd.xxx.com.         tandongyu.rd.xxx.com. (
　　　　20020101        ;Serial
　　　　3600            ;Refresh
　　　　900             ;Retry 15 mins
　　　　604800          ;Expire 7 days
　　　　86400)          ;Mini 24 hours
　　;NameServer (NS) records
　　@       IN      NS      compaq.rd.xxx.com.
　　;Address Point to Name (PTR) records
　　9       IN      PTR     compaq.rd.xxx.com.
　　8       IN      PTR     tls65.rd.xxx.com.
　　7       IN      PTR     fbsd.rd.xxx.com.

　　15、在/var/named/中生成/etc/named.conf中标记的文件：127.0.0.db，内容如下，你需要修改相应部分：

　　; 0.0.127.in-addr.arpa
　　$TTL 3600
　　@    IN   SOA     compaq.rd.xxx.com. tandongyu.rd.xxx.com. (
　　　　 20020101;serial
         3600    ;refresh
         1800    ;retry
         604800  ;expiration
         3600 )  ;minimum
    IN     NS      compaq.rd.xxx.com.
　　1        IN     PTR     localhost.

　　16、在/var/named/中生成/etc/named.conf中标记的文件：named.root，内容大致如下。该文件标记了14个域名服务器。可以从ftp.rs.internic.net获得该文件的最新样本：named.hosts，然后改名成你需要的名字，比如： named.root

　　;       This file holds the information on root name servers needed to
　　;       initialize cache of Internet domain name servers
　　;       (e.g. reference this file in the "cache  .  "
　　;       configuration file of BIND domain name servers).
　　;
　　;    This file is made available by InterNIC registration services
　　;    under anonymous FTP as
　　;    file                /domain/named.root
　　;    on server           FTP.RS.INTERNIC.NET
　　;       -OR- under Gopher at    RS.INTERNIC.NET
　　;           under menu          InterNIC Registration Services (NSI)
　　;              submenu          InterNIC Registration Archives
　　;           file                named.root
　　;
　　;       last update:    Aug 22, 1997
　　;       related version of root zone:   1997082200
　　;
　　;
　　; formerly NS.INTERNIC.NET
　　;
　　　　.                        3600000  IN  NS    A.ROOT-SERVERS.NET.
　　　　A.ROOT-SERVERS.NET.      3600000      A     198.41.0.4
　　;
　　; formerly NS1.ISI.EDU
　　;
　　　　.                        3600000      NS    B.ROOT-SERVERS.NET.
　　　　B.ROOT-SERVERS.NET.      3600000      A     128.9.0.107
　　;
　　; formerly C.PSI.NET
　　;
　　　　.                        3600000      NS    C.ROOT-SERVERS.NET.
　　　　C.ROOT-SERVERS.NET.      3600000      A     192.33.4.12
　　;
　　; formerly TERP.UMD.EDU
　　;
　　　　.                        3600000      NS    D.ROOT-SERVERS.NET.
　　　　D.ROOT-SERVERS.NET.      3600000      A     128.8.10.90
　　;
　　; formerly NS.NASA.GOV
　　;
　　　　.                        3600000      NS    E.ROOT-SERVERS.NET.
　　　　E.ROOT-SERVERS.NET.      3600000      A     192.203.230.10
　　;
　　; formerly NS.ISC.ORG
　　;
　　　　.                        3600000      NS    F.ROOT-SERVERS.NET.
　　　　F.ROOT-SERVERS.NET.      3600000      A     192.5.5.241
　　;
　　; formerly NS.NIC.DDN.MIL
　　;
　　　　.                        3600000      NS    G.ROOT-SERVERS.NET.
　　　　G.ROOT-SERVERS.NET.      3600000      A     192.112.36.4
　　;
　　; formerly AOS.ARL.ARMY.MIL
　　;
　　　　.                        3600000      NS    H.ROOT-SERVERS.NET.
　　　　H.ROOT-SERVERS.NET.      3600000      A     128.63.2.53
　　;
　　; formerly NIC.NORDU.NET
　　;
　　　　.                        3600000      NS    I.ROOT-SERVERS.NET.
　　　　I.ROOT-SERVERS.NET.      3600000      A     192.36.148.17
　　;
　　; temporarily housed at NSI (InterNIC)
　　;
　　　　.                        3600000      NS    J.ROOT-SERVERS.NET.
　　　　J.ROOT-SERVERS.NET.      3600000      A     198.41.0.10
　　;
　　; housed in LINX, operated by RIPE NCC
　　;
　　　　.                        3600000      NS    K.ROOT-SERVERS.NET.
　　　　K.ROOT-SERVERS.NET.      3600000      A     193.0.14.129 
　　;
　　; temporarily housed at ISI (IANA)
　　;
　　　　.                        3600000      NS    L.ROOT-SERVERS.NET.
　　　　L.ROOT-SERVERS.NET.      3600000      A     198.32.64.12
　　;
　　; housed in Japan, operated by WIDE
　　;
　　　　.                        3600000      NS    M.ROOT-SERVERS.NET.
　　　　M.ROOT-SERVERS.NET.      3600000      A     202.12.27.33
　　; End of File

　　17、我们还需要配置/etc/resolv.conf、/etc/hosts、/etc/hosts.conf文件以适应新的状况。

　　18、一切都结束后，用/usr/sbin/ndc start命令启动bind,同样的可用stop、restart、reload等命令参数操作。

　　19、启动后用nslookup命令(有的系统推荐使用dig命令)检验是否正确。如果出现错误，该命令将不能启动。一般的错误都是数据库文件或配置文件笔误所至。比如少个“.”或者文件明不正确等等。

　　(2) 安装sendmail服务器

　　1、从[www.sendmail.org](http://www.sendmail.org/)下载最新的版本(这个snedmail倒是有必要升级为最新的版本，因为它的升级主要是安全漏洞问题)。这里说明的是用的sendmail-8.12.2.tar.gz

　　2、cd /usr/local/src/

　　3、把文件下载到：/usr/local/src中

　　4、tar zxvf sendmail-8.12.2.tar.gz

　　5、cd /usr/local/src/sendmail-8.12.2

　　6、chmod go-w / /etc /etc/mail /usr /var /var/spool /var/spool/mqueue

　　7、chown root / /etc /etc/mail /usr /var /var/spool /var/spool/mqueue

　　8、cd /usr/local/src/sendmail-8.12.2/sendmail

　　9、sh Build

　　10、cd /usr/local/src/sendmail-8.12.2/cf/cf

　　11、建立文件sendmail.mc内容如下，你可根据需要修改相应部分。

　　divert(-1)
　　dnl This is the macro config file used to generate the /etc/sendmail.cf
　　dnl file. If you modify thei file you will have to regenerate the
　　dnl /etc/sendmail.cf by running this macro config through the m4
　　dnl preprocessor:
　　dnl m4 /etc/sendmail.mc > /etc/sendmail.cf
　　dnl You will need to have the Sendmail-cf pacage installed for this to work.
　　include(`/usr/local/src/sendmail-8.12.2/cf')
    define(`confDEF_USER_ID',`8:12')
    OSTYPE(`linux')
    undefine(`UUCP_RELAY')
    undefine(`BITNET_RELAY')
    define(`confTO_CONNECT', `1m')
    define(`confTRY_NULL_MX_LIST',true)
    define(`confDONT_PROBE_INTERFACES',true)
    define(`PROCMAIL_MAILER_PATH',`/usr/bin/procmail')
    define(`SMART_HOST',compaq.rd.xxx.com)
　　 　<---这条用于(非HUB)缺省使用HUB发送邮件
    MASQUERADE_AS(`rd.xxx.com')        
       <-------------------------
    FEATURE(`masquerade_entire_domain')     
　　　　   <---这三条用于邮件地址伪装
    FEATURE(`masquerade_envelope')            
　　　　　　<-------------------------
    FEATURE(`smrsh',`/usr/sbin/smrsh')
    FEATURE(`mailertable',`hash -o /etc/mail/mailertable')
    FEATURE(`virtusertable',`hash -o /etc/mail/virtusertable')
    FEATURE(redirect)
    FEATURE(always_add_domain)
    FEATURE(use_cw_file)
    FEATURE(local_procmail)
    FEATURE(`access_db')
    FEATURE(`blacklist_recipients')
    FEATURE(`accept_unresolvable_domains')
    MAILER(smtp)
    MAILER(procmail)
    dnl We strongly recommend to comment this one out if you want to protect
    dnl yourself from spam. However, the laptop and users on computers that do
    dnl not hav 24x7 DNS do need this.
    dnl FEATURE(`relay_based_on_MX')   

　　12、sh Build install-cf

　　13、groupadd smmsp

　　14、useradd smmsp

　　15、cd cd /usr/local/src/sendmail-8.12.2/sendmail

　　16、sh Build install

　　17、cd /usr/local/src/sendmail-8.12.2/makemap

　　18、sh Build clean

　　19、sh Build all

　　20、sh Build install

　　21、cd /usr/local/src/sendmail-8.12.2/

　　22、在本域DNS主数据库文件中增加MX纪录：

　　rd.xxx.com.        IN      MX      0       compaq

　　注意修改相应部分。那个0是有几个邮件集中器的时候用于标记先后顺序的。当有好几个MX的时候，建议顺序写为10、20、30…

　　23、在/etc/mail目录下创建access文件，内容类似如下：

        127.0.0.1 RELAY
        21.9.22 RELAY
        211.99.221.238 RELAY

　　然后：makemap hash access.db < access

　　24、创建文件/etc/mail/local-host-names，其内容为本机的拥有的域名信息。

　　rd.xxx.com
　　compaq.rd.xxx.com

　　25、创建文件/etc/mail/aliases，内容类似：

　　MAILER-DAEMON: postmaster
        postmaster: root
        bin: root
        daemon: root
        nobody: root

　　运行newaliases创建数据库。

　　创建别名文件的意义之一在于当邮件发往域中其他邮件服务器的用户而不是mail HUB用户的时候用。

　　比如增加一条：

　　atan: atan@fbsd

　　则导致邮件发往mail HUB的时候自动转发到[atan@fbsd.rd.xxx.com](mailto:atan@fbsd.rd.xxx.com)

　　26、启动sendmail： /usr/sbin/sendmail -bd -q30m

　　排错：如果有问题导致启动不了，大部分问题和DNS配置有关，可以使用nslookup检查DNS是否正常。挨个检查/etc/mail中的文件内容也是排错的好办法。另外，修改配置，不建议直接编辑sendmail.cf文件，建议使用m4宏编译工具，因为有些带有安全漏洞或过时的宏在编译的时候会有提示，这样以免造成相关安全问题。

[](http://www.ut365.com/bbs/misc.php?action=viewratings&tid=66&pid=94)(3)安装DHCP服务器

　　1、源程序下载地址是：[ftp://ftp.isc.org/isc/dhcp/dhcp-3.0.tar.gz](ftp://ftp.isc.org/isc/dhcp/dhcp-3.0.tar.gz) ，我们的版本是：dhcpd-3.0

　　2、复制到：/usr/local/scr后解压：tar zxvf dhcp-3.0.tar.gz

　　3、cd /usr/local/src/dhcp-3.0.tar.gz

　　4、./configure

　　5、make(如果不是第一次编译，先make clean)

　　6、make install

　　7、cp ./server/dhcpd.conf /etc

　　8、编辑这个文件，看起来类似下面这样：（要更改的地方用粗体标出）

　　# dhcpd.conf
　　#
　　# Sample configuration file for ISC dhcpd
　　#

　　# option definitions common to all supported networks...
　　option domain-name "rd.xxx.com";
　　option domain-name-servers compaq.rd.xxx.com;

　　default-lease-time 86400;
　　max-lease-time 172800;

　　ddns-update-style ad-hoc;

　　# If this DHCP server is the official DHCP server for the local
　　# network, the authoritative directive should be uncommented.
　　#authoritative;

　　# Use this to send dhcp log messages to a different log file (you also
　　# have to hack syslog.conf to complete the redirection).
　　log-facility local7;

　　# No service will be given on this subnet, but declaring it helps the
　　# DHCP server to understand the network topology.

　　# This is a very basic subnet declaration.

　　subnet 21.9.22.0 netmask 255.255.255.224 {
　　　　range 21.9.22.2 21.9.22.6;
　　　　option routers 21.9.22.1;
　　}

　　# This declaration allows BOOTP clients to get dynamic addresses,
　　# which we don't really recommend.

　　#subnet 10.254.239.32 netmask 255.255.255.224 　　{
　　　　#  range dynamic-bootp 10.254.239.40 10.254.239.60;
　　　　#  option broadcast-address 10.254.239.31;
　　　　#  option routers rtr-239-32-1.example.org;
　　#}

　　# A slightly different configuration for an internal subnet.
　　#subnet 10.5.5.0 netmask 255.255.255.224 {
　　#  range 10.5.5.26 10.5.5.30;
　　#  option domain-name-servers ns1.internal.example.org;
　　#  option domain-name "internal.example.org";
　　#  option routers 10.5.5.1;
　　#  option broadcast-address 10.5.5.31;
　　#  default-lease-time 600;
　　#  max-lease-time 7200;
　　#}

　　# Hosts which require special configuration options can be listed in
　　# host statements.   If no address is specified, the address will be
　　# allocated dynamically (if possible), but the host-specific information
　　# will still come from the host declaration.

　　#host passacaglia {
　　#  hardware ethernet 0:0:c0:5d:bd:95;
　　#  filename "vmunix.passacaglia";
　　#  server-name "toccata.fugue.com";
　　#}

　　# Fixed IP addresses can also be specified for hosts.   These addresses
　　# should not also be listed as being available for dynamic assignment.
　　# Hosts for which fixed IP addresses have been specified can boot using
　　# BOOTP or DHCP.   Hosts for which no fixed address is specified can only
　　# be booted with DHCP, unless there is an address range on the subnet
　　# to which a BOOTP client is connected which has the dynamic-bootp flag
　　# set.
　　#host fantasia {
　　#  hardware ethernet 08:00:07:26:c0:a5;
　　#  fixed-address fantasia.fugue.com;
　　#}

　　# You can declare a class of clients and then do address allocation
　　# based on that.   The example below shows a case where all clients
　　# in a certain class get addresses on the 10.17.224/24 subnet, and all
　　# other clients get addresses on the 10.0.29/24 subnet.

　　#class "foo" {
　　#  match if substring (option vendor-class-identifier, 0, 4) = "SUNW";
　　#}

　　#shared-network 224-29 {
　　#  subnet 10.17.224.0 netmask 255.255.255.0 {
　　#    option routers rtr-224.example.org;
　　#  }
　　#  subnet 10.0.29.0 netmask 255.255.255.0 {
　　#    option routers rtr-29.example.org;
　　#  }
　　#  pool {
　　#    allow members of "foo";
　　#    range 10.17.224.10 10.17.224.250;
　　#  }
　　#  pool {
　　#    deny members of "foo";
　　#    range 10.0.29.10 10.0.29.230;
　　#  }

　　上面这个文件中后面都是注释，是原来系统给你的配置做的例子，可以不要。当然除非你用到它们，并且清楚它们的含义。

　　9、建立空文件：touch /var/state/dhcp/dhcpd.leases空文件

　　10、检查内核编译选项：Networking options-----Packet socket: mmapped IO和Socket Filtering选项是否被选中（不知道怎么看？看看前面的文章，用make menuconfig呀），如果没有就需要重新编译内核了。

　　11、route add -host 255.255.255.255 dev eth0

　　12、roote add -host localhost dev eth0

　　13、在/etc/rc.d/rc.local最后增加一行route add -host 255.255.255.255 dev eth0

　　14、reboot系统

　　15、用dhcpd命令启动DHCP，用其他的机器试试看能不能自动配置网络了。

(4) 安装apache+mysql+php+gd+png+zlib+jpeg+freetype+sslmod

　　这么一大堆东西为什么要一块说呢？因为他们中间是有紧密联系的，尤其是那个PHP，用到其他所有的模块。要想PHP功能全，只能一步步来了。

　　1、首先安装MYSQL：从[http://www.mysql.com/downloads/index.html](http://www.mysql.com/downloads/index.html)下载自己觉得合适的版本。这里用3.23.42版本。

　　2、进入目录/usr/local/src(这个以后就不用说了吧？)解压缩：tar zxvf mysql-3.23.42.tar.gz

　　3、cd mysql-3.23.42/

　　4、configure --prefix=/usr/local/mysql

　　5、make

　　6、make install

　　7、useradd mysql ; groupadd mysql ; su mysql ;(建立命名为mysql的用户和组，并切换为mysql身份)

　　8、scripts/mysql_install_db (建立数据库结构)

　　9、cd /usr/local/mysql/bin

　　10、./safe_mysqld & （启动了）

　　11、./mysqladmin -u root password "new-password"（你的新密码，记住MYSQL的密码核系统密码是独立的，缺省是没有密码）

　　12、用mysql程序试试看。

　　下面的步骤都很程式化，我尽量简洁地写出。

　　13、下载相关的软件包：地址表格如下

　　软件　　　　           版本　　　　        地址
jpegsrc.v6b.tar.gz　　6b        [ftp://ftp.uu.net/graphics/jpeg](ftp://ftp.uu.net/graphics/jpeg)
zlib.tar.gz            1.1.3        [ftp://ftp.uu.net/graphics/png/src](ftp://ftp.uu.net/graphics/png/src)
freetype-2.0.8.tar.gz  2.0.8       [http://www.freetype.org](http://www.freetype.org/)
libpng-1.0.10.tar.gz   1.0.10        [ftp://ftp.uu.net/graphics/png/src](ftp://ftp.uu.net/graphics/png/src)
gd-1.8.4.tar.gz             1.8.4         [http://www.boutell.com/gd](http://www.boutell.com/gd)
apache_1.3.23.tar.gz   1.3.23         [http://www.apache.org](http://www.apache.org/)
php-4.1.1.tar.gz             4.1.1         [http://www.php.net](http://www.php.net/)
number4.tar.gz              4        [http://www.php.net/extra](http://www.php.net/extra)
rsaref20.tar.Z              2.0        [ftp://ftp.ai.mit.edu/pub/deberg](ftp://ftp.ai.mit.edu/pub/deberg)
openssl-engine-0.9.6c.tar.gz  0.9.6c         [http://www.openssl.org/source](http://www.openssl.org/source)
mod_ssl-2.8.6-1.3.23.tar.gz   2.8.6      [http://www.modssl.org](http://www.modssl.org/)

　　下齐了就放在/usr/local/src目录下，除了number4.tar.gz和rsaref20.tar.Z以外，其他一律用tar zxvf XXXX.tar.gz解压缩。（过瘾吧？）

　　特殊的，rsaref20.tar.Z 需要先建立一个rsaref-2.0目录，然后把rsaref20.tar.Z复制到改目录下用tar zxvf rsaref30.tar.Z释放压缩。Number4.tar.gz需要复制到php-4.1.1目录内，然后用tar zxvf number4.tar.gz解压缩。

　　下面的描述我想简单一点，只写出进入目录后执行的命令。那些目录都是释放的时候自己建立的，如果你这些操作都在/usr/local/src下面执行，所有的目录就都在这里面。最好按照顺序执行。再罗嗦一句：如果不是第一次编译，一般情况请执行一次make clean后再编译。

　　14、[jpeg-6b]：生成JPEG图像的函数库。

　　./configure
　　make
　　make test
　　make install

　　15、[zlib-1.1.3]：压缩算法库，是PNG和JPEG需要的

　　make test
　　make install

　　16、[freetype-2.0.8]：字体库，在图像上使用字体的时候需要用到，

　　./configure --prefix=/usr/local
　　make
　　make install

　　17、[libpng-1.0.10]：生成PNG图像的函数库

　　cp scripts/makefile.linux makefile
　　make test
　　pngtest pngnow.png 
　　(如果都通过测试了，请继续。否则，看看错误提示，改正错误)
　　make install

　　18、[gd-1.8.4]：GD库，用来动态生成图像用，这里主要是配合PHP生成图像。

　　编辑Makefile 文件：

　　把下列行取消注释符号（#）：

　　CFLAGS=-O -DHAVE_LIBXPM -DHAVE_LIBPNG -DHAVE_LIBJPEG 
    　　-DHAVE_LIBFREETYPE -DHAVE_LIBTTF
　　LIBS=-lgd -lpng -lz -ljpeg -lfreetype -lm -lttf

　　注释下列行（增加#）：
　　CFLAGS=-O -DHAVE_LIBPNG -DHAVE_LIBJPEG
　　LIBS=-lgd -lpng -lz -lm

　　修改下列行：
　　INCLUDEDIRS=-I. -I/usr/local/include/freetype2 -I/usr/include/X11 
　　　　-I/usr/X11R6/include/X11 -I/usr/local/include
　　　　 -I/usr/include/freetype

　　保存后执行：

　　make
　　make install

　　19、[apache_1.3.23]：步骤一 ./configure --prefix=/usr/local/apache

　　20、[php-4.1.1]：

　　如果不是第一次安装，请删除：config.cache文件，然后执行先执行make clean

　　接下来配置环境，输入一个比较长的命令：

　　./configure 
　　　　--with-apache=/usr/local/src/apache_1.3.23 
　　　　--with-config-file-path=/local/apache/conf 
　　　　--with-enable-versioning 
　　　　--with-mysql=/usr/local/mysql 
　　　　--with-ftp 
　　　　--with-gd=/usr/local/src/gd-1.8.4 
　　　　--with-enabled-bcmath=yes 
　　　　--with-disable-debug 
　　　　--enable-memory-limit=yes 
　　　　--enable-track-vars 
　　　　--with-zlib 
　　　　--with-jpeg-dir=/usr/local/src/jpeg-6b 
　　　　--with-png-dir=/usr/local/src/libpng-1.0.10 
　　　　--with-freetype-dir=/usr//local/src/freetype-2.0.8

　　当然你也可以实现准备一个含有上述命令的sh文件，这样改起来方便些。我就是这样做的。

　　make
　　make install

　　21、[rsaref-2.0]：RSA加密模块

　　前面说过，这个目录的释放是特殊的，先见目录后释放，希望你是这样做的。

　　cp -rp install/unix local
　　cd local
　　make
　　mv rsaref.a librsaref.a

　　22、[openssl-engine-0.9.6c]：openssl引擎

　　./config -prefix=/usr/local/ssl -L`pwd`/../rsaref-2.0/local/ rsaref –fPIC
　　make
　　make test
　　make install

　　23、[mod_ssl-2.8.6-1.3.23]

　　./configure --with-apache=../apache_1.3.23

　　24、[apache_1.3.23]步骤二

　　./configure --prefix=/usr/local/apache --enable-shared=ssl
　　　　 --enable-module=ssl --activate-module=src/modules/php4/libphp4.a
　　make
　　make certificate TYPE=custom（回答一些问题）
　　make install

　　25、收尾工作：

　　编辑/usr/local/apache/conf/httpd.conf文件，增加一行：

　　AddType application/x-httpd-php .php

　　26、用命令/usr/local/apache/bin/apachectl configtest检查一下配置文件的正确性。

　　27、用/usr/lcoal/apache/bin/apachectl start启动apache服务器

　　28、在/usr/local/apache/htdocs建立一个test.php文件包含下列内容：

　　　　phpinfo();
　　?>

　　29、在别的机器上用浏览器看一下这个机器：[http://xxx.xxx.xxx.xxx/test.php](http://xxx.xxx.xxx.xxx/test.php)会出现php的配置总汇，看看前面编译的模块都在不在。

　　30、检验ssl很麻烦，需要用apachectl startssl启动apache然后还要生成一大堆证书。这里就不讨论了。

　　31、把/usr/local/apache/bin/apachectl文件复制到/etc/rc.d/init.d中，然后在/etc/rc.d/rd3.d中建立个符号连接指向/etc/rc.d/init.d中的上述文件：

　　ln -s ../init.d/apachectl S70apachectl

　　这样系统启动的时候就可以自动启动apache和她的哪一大堆模块了。

　　呼~~~~（喘口气）到这里，就基本上完成了一个功能很全的WEB服务器系统了。

　　(5)安装 ftp 服务器

　　(6)安装 pop3服务器

　　有的邮件服务器自己带pop3功能（比如Qmail、Xmail），如果你用这几种邮件服务器软件，当然就不用安装pop3了。目前，常用的邮件服务器里面不带pop3的好象只有sendmail和postfix等少数几种。这几种邮件服务器软件使用的都是系统用户，而我们安装的pop3也只是对系统用户的邮件进行弹出操作的。这里我们使用的版本和下载地址是：

　　qpopper4.0.3.tar.gz 　　
[http://www.eudora.com/qpopper_general/](http://www.eudora.com/qpopper_general/)

　　同样的，把文件下载到/usr/local/src中，按照下列步骤安装：

　　tar zxvf qpopper4.0.3.tar.gz
　　cd qpopper4.0.3
　　./configure
　　make
　　make install

　　完成后，qpopper安装在/usr/local/sbin中（如果你愿意把它放在其他地方，可以在前面用./configure –prefix=/your_path指定），文件名叫作popper。通常情况下，popper是作为一个inetd（超级网络服务进程）的子进程加载的。这样需要编辑文件/etc/inetd.conf并做下面改动： ...

　　# POP3 mail server

　　#pop-3   stream  tcp     nowait  root    /usr/sbin/tcpd ipop3d 
　　pop-3 stream tcp nowait root /usr/local/sbin/popper qpopper –s
　　...

　　增加黑体的这一行，注释掉原来的（上面这一行），注意，如果你的安装路经不同，记着改过来呀。然后，用下面的命令得到inetd的进程号：

　　ps –ax | grep inetd

　　得到结果类似：

　　248 ?        S      0:00 inetd

　　什么？哪个是进程号？就是最前面那个数字。然后用下面的命令重新启动inetd:

　　kill –HUP 248

　　用下面的命令看看pop3是不是启动了。

　　netstat -ln|grep 110

　　如果结果是这样，就恭喜了。

　　tcp      0      0 0.0.0.0:110       0.0.0.0:*               LISTEN

　　这里还要补充一点，少数情况下，pop3进程需要独立的监听端口，也就是不作为inetd的子进程装载，上面过程需要做如下改动：

　　./configure的时候加上--enable-standalone参数。

　　编辑/etc/inetd.conf不增加黑体一行，但需要注释掉原来的pop-3一行（如果有的话）。

　　启动的时候，直接运行/usr/local/sbin/popper xxx.xxx.xxx.xxx:110即可。后面的参数是本地需要几监听的IP地址和端口。注意，没有特殊需要，端口必须是110。最后要说的是：其实最简单的办法就是用你使用的Linux发行包带的pop3软件，安装的时候直接选上即可。
(7)安装imap服务器

　　几乎所有的发行包都带有imap服务器软件（一般是华盛顿大学版本），最好的办法是用你的发行包安装IMAP服务器。事实上，我个人觉得 IMAP服务器很少被使用。下载最常规的IMAP服务器源码地址是：ftp: //ftp.cac.washington.edu/imap/imap.tar.Z

　　tar zxvf imap.tar.Z
　　cd imap-2001a
　　make slx

　　(如果不行依次试验一下其他你觉得可能的系统类型，用vi看一下Makefile中列的名称)

　　然后配置/etc/inetd.conf，让inetd允许imap服务。[本节内容没有试验成功，编译总是过不去]

　　(8)安装squid服务器

　　SQUID是一个优秀的代理服务器软件，它可以很灵活的被配置为各种应用形式，其中包括正向代理，反向加速模式和透明代理等。但是SQUID目前只能代理HTTP协议，代理FTP协议需要配置浏览器仿真主动FTP协议。下面的步骤是安装SQUID的过程。

　　1、下载反向代理服务器软件采用squid，下载地址：[http://www.squid-cache.org/](http://www.squid-cache.org/)

　　下载后存放在/usr/local/src目录里，文件名是squid-2.4.STABLE2-src.tar.gz

　　2、tar zxvf squid-2.4.STABLE2-src.tar.gz 解压缩

　　3、cd /usr/local/src/squid-2.4.STABLE2 进入目录

　　4、./configure --prefix=/usr/local/squid --enable-heap-replacement --disable-internal-dns建立环境，把squid安装/usr/local/squid中。第二个参数是指定使用更先进的缓冲算法。第三个参数是取消内部DNS解析（如果使用在远程高速缓存模式，比如GSLB，需要增加选项：--disable-internal-dns，目的是关闭内部DNS.否则内部DNS不理睬你在etc/hosts中的设定，直接寻找域名服务器，这样就会造成转发循环。就是cache发送给物理服务器的更新请求（因为用域名）也会被用户的GSLN设备解析回来而形成循环。）

　　5、make 开始编译

　　6、make install 安装到刚才--prefix=指定的路径当中

　　7、安装完成后，会在您指定的安装路径里产生一个squid目录，squid目录下有四个目录：bin/ etc/ libexec/ logs/。其中etc里面是配置文件，bin里面是执行文件，logs里面是日志文件。

　　8、安装结束后就是调试服务器，使其按照您的要求工作。Squid的配置文件只有一个，在etc目录里，名字是squid.conf，所有的配置选项都在这个文件里面。而且每个配置项目都有注释说明。首先，在squid文件里面找到下列配置项：

　　cache_mem ---这里可以添上您准备给squid作为高速缓存使用的内存大小。注意，如果您的机器有N兆内存，那么，推荐您在这里添的数字是N/3。

　　cache_dir /usr/local/squid/cache 100 16 256 这里的第一个数字100是您准备给squid作为cache使用的硬盘空间大小，单位是兆。如果您想划100M空间当作cache,那么这里就写100。

　　cache_mgr [webmaster@test.com.cn](mailto:webmaster@test.com.cn)这里填写cache管理员的Email地址，系统出错会自动提醒cache管理员。

　　cache_replacement_policy和memory_replacement_policy的参数由于使用了--enable-heap-replacement编译参数，就不能使用缺省的lru算法了，你可以在下列三种里面选一个：

　　heap GDSF : Greedy-Dual Size Frequency

　　heap LFUDA: Least Frequently Used with Dynamic Aging

　　heap LRU : LRU policy implemented using a heap

　　例如：

　　cache_replacement_policy heap LRU
　　memory_replacement_policy heap LRU

　　下面设定ACL访问控制列表：为了简单起见，我们在这里开放所有的权限。ACL分为两个部分：ACL定义部分，和http_access部分。在access_http部分用到前面的定义。前面定义了：

　　acl all src 0.0.0.0/0.0.0.0 

　　我们注释掉所有的http_access行加一句：

　　http_access allow all ---注意：all是前面acl定义的。

　　这样就开放了所有的权限。以后有需要的，再继续往上加各种限制即可。

　　9、以root身份创建组nogroup ：

　　%su root （如果当前不是root）
　　#groupadd nogroup
　　#useradd nobody (如果没有这个用户)

　　10、进行目录/usr/local ，以root身份执行下面的命令，改变整个Squid目录的所有者为nobody.nogroup

　　#cd /usr/local 
　　#chown nobody.nogroup -R squid

　　11、su 作为nobody,进行/usr/local/squid/bin目录，执行#squid -z 创建cache交换目录

　　#su nobody 
　　$ cd /usr/local/squid/bin 
　　$./squid -z 

　　12、成功后，测试一下：/usr/local/squid/bin/squid -NCd1 该命令正式启动Squid。如果一切正常，你会看到一行输出 ：Ready to serve requests.

　　13、用ctrl-c退出前台测试。

　　14、把squid作为守护进程启动就直接运行：/usr/local/squid/bin/squid

　　15、检查状态用squid –k check

　　16、停止squid用squid -k shutdown

　　如果是配置基本的正向代理，上面的已经可以使用了。下面的步骤用于配置支持多域名的反向代理服务器。还好，SQUID的所有配置都在/usr/local/squid/etc下面，和反向代理有关的几项介绍如下：

　　17、http_port 80 “http_port”参数指定Squid监听浏览器客户请求的端口号。

　　18、icp_port 0 “icp_port”参数指定Squid从邻居（neighbour）服务器缓冲内发送和接收ICP请求的端口号。这里设置为0是因为这里配置Squid为内部Web服务器的加速器，所以不需要使用邻居服务器的缓冲。

　　19、emulate_httpd_log on 打开“emulate_httpd_log”选项，将使Squid仿照Web服务器的格式创建访问记录。如果希望使用Web访问记录分析程序，就需要设置这个参数。

　　20、redirect_rewrites_host_header off 缺省地，Squid将改写任何重定向请求的主机头部。若系统运行Squid为加速器模式，则这时不需要重定向特性。该参数在负载过重的情况下要旁路重定向器时才打开

　　21、httpd_accel_host vartual 此处设置反向代理的主机名，如果对后面多个域名进行缓冲，请使用虚拟主机模式(如此)。

　　22、httpd_accel_port 80 此处设置反向代理的WEB服务端口号。

　　23、#httpd_accel_with_proxy off 把这行注释掉，此处设置开反向代理的同时，是否开普通代理缓存服务。如果这行不注释掉，就没有高速缓存功能。

　　24、定义访问控制列表：

　　acl port80 port 80
　　acl accel_host1 dstdomain .test.com
　　acl accel_host2 dstdomain .test.net
　　… …
　　http_access allow accel_host1 port80
　　http_access allow accel_host2 port80
　　http_access deny all;

　　25、完成后，用squid –k reconfigure重新装载配置文件。

　　(9)安装SSH

　　---

　　(10)配置Linux为路由器

　　---

　　(11)配置Linux网关和安装ipchains/iptables防火墙

　　在Linux上面的防火墙，最最常用的是ipchains，而且通常情况下是作为网关的附加部分安装的。Ipchains的规则是很复杂的，灵活性也很强，可以配制成各种五花八门的样子。这些都需要和你自己的实际情况相结合。这里，我们只介绍一种基于网关的简单配置。

　　安装IPCHAINS一般都不用你操心，因为几乎所有的Linux发行包都把该软件作为必须安装的却省配置。另一个原因是ipchains是跟内核（kernel）有很大的关系，因此最好在安装系统的时候选上相关选项（如果有）。在标题里面我们还提到了iptables，这个工具在表面上等同于 ipchains，只不过是用于2.4内核的（2.4内核在这方面的代码几乎是重新写过的，功能有了长足的提高）。关于iptables工具的配置我们在后面介绍。你只需要记住2.2核心下使用ipchains，2.4核心下使用iptables即可。

　　首先，你的服务器需要两块网卡（或更多），这种机器叫做“多宿主主机”，是专门的用于做网关或路由器的。这里插一句：一般情况下，作为普通服务器的主机即使负载再重就需要一块网卡就够了，只有做网关或路由器的时候才需要多宿主主机。这并不象一般人认为得那样增加一块网卡可以增加一份带宽，事实上，一块网卡就可以提供足够的带宽。并且，还有人错误的把两块网卡接在同一个交换机上分配两个地址，这更是错误的，因为这样产生了额外的循环路由，会产生大量的内部警告错误，某些系统就会报警。

　　1、制作一个双界面（双宿主）的主机。

　　通常，现在的发行包的安装程序都可以识别两块网卡，这样就省事了。但是也有不少发行包只识别第一块网卡（也许是出于前面说的原因），或者你要在一台在用的机器上加一块网卡（因为你不愿意重装系统），那么就按照下面的办法处理。

　　1．1、我们就说PCI网卡。安装前首先看看网卡芯片，记住芯片的型号（希望你自己攒过机器）。

　　1．2、安装MAN手册（用发行包）

　　1．3、在/usr/doc/HOWTO/english/txt/Ethernet-HOWTO文件(如果是压缩的就释放)中搜索你的网卡型号，找到对应的驱动模块名称。

　　1．4、如果模块没有，还需要重新编译核心。在menuconfig网络设备一栏选中你的型号然后标记为*或M，编译完核心后别忘了编译模块：make modules;make modules_install。(不会的话再仔细温习一下前面编译内核的部分)

　　1．5、用depmod –a命令建立/etc/modules.conf（如果已经有就不用了），也有的发行包叫做conf.modules

　　1．6、编辑该文件加上一行：alias eth1 XXXX，其中XXXX是你刚才查到的模块名称。一般该模块文件位于：/lib/modules/内核版本号/net…中的XXXX.o文件。这个文件是你编译内核模块的时候产生的，你在内核配置的时候，凡是标记为M的都会被编译成.o文件放在这里。同样的，你选择的网卡驱动也是会编译成模块在这里的。

　　1．7、运行modprobe eth1使模块有效。

　　1．8、修改/etc/sysconfig/network文件中需要修改的部分。

　　1．9、建立或修改/etc/sysconfig/network-script/ifcfg-eth1文件（照抄那个eth0的即可），设定地址是你的真实情况，比如设为eth1要接的网段。这两个文件（eth0/eth1是启动的时候的脚本参数文件）

　　1．10、重新启动一下网络：/etc/rc.d/init.d/network restart

　　1．11、用ifconfig看看是否eth0/eth1都启动了。

　　1．12、大功告成

　　2、调整和编译核心：如果作为网关，有些核心选项需要配置。注意：这里的内核编译选项仅仅适合于2.2.X版本，2.4版本完全不同。

　　2．1、在/usr/src/linux中运行make menuconfig配置核心下列选项：

　　Networking options中

Network firewalls 
　　 IP: advanced router 
　　 IP: firewalling 
　　 IP: firewall packet netlink device 
　　 IP: transparent proxy support 
　　 IP: masquerading 
　　 IP: ICMP masquerading 
　　 IP: masquerading special modules support 
　　 IP: ipautofw masq support (EXPERIMENTAL) 
　　 IP: ipportfw masq support (EXPERIMENTAL) 
　　 IP: ip fwmark masq-forwarding support (EXPERIMENTAL) 
　　 IP: masquerading virtual server support (EXPERIMENTAL) 
　　(12) IP masquerading VS table size (the Nth power of 2)    

　　具体选项不同版本的内核不尽相同，你看着差不多就可以了。编译成模块或编进核心 可以自由选择。

　　2．2、按照前面说的编译内核，重新启动后就具有网关功能了。启动后验证一下文件：/proc/sys/net/ipv4/ip_forward内容是否为1。（别告诉我不知道怎么看8-））

　　3、设置ipchains

　　本来这项工作是非常复杂的，但是我们这里讲的简单，让网关通了就行。

　　3．1、先看一下目前我们目前的假想环境：你的机器有两块网卡，一块连接在内网交换机配内部网络地址，另一块在外网交换机配公网地址。在这个机器ping 内网和外网的机器应该都能通。并且在内网的机器ping 该机器的外网地址也通，在外网的机器ping该机器内网的地址也同样通。这说明转发功能是有效的。

　　3．2、再试验一下：在内部网络把网关设为你的机器的内网地址，然后ping 外网的任一台机器（不是这台机器的外网地址），结果是不通。原因是ICMP包发出去后外网的机器不知道怎么回答，那里没有合适的路由，因为这个包的回应地址是内网。

　　3．3、现在设置ipchains：(ipchains软件适用2.2.X的内核，在2.4.X的内核中不适用。2.4.X的内核推荐使用iptables，关于该软件的用法在下面介绍)

　　ipchains –A forward –s 192.168.1.0/24 –j MASQ

　　黑体部分是内网地址。这句话的意思是，把所有内网发送的包都伪装成外部网的地址（那个机器的外网地址），这样，外部其他机器回应的时候就会发送到这台机器，经过转发就回来内网了。好了，一个简单的网关做好了。如果你需要实现防火墙（包过滤功能），则需要配置更加复杂的过滤规则，这些规则应该同时作用于input/output/forward链。

　　3．4、自动启动：设定了复杂的链，需要保存。在/etc/rc.d中建立一个文件：rc.ipfwadm把你的链脚本加进去就可以了。最后给这个文件+x属性(chmod +x rc.ipfwadm)。

　　4．在2.4.X内核中使用iptables建立Linux防火墙(网关)。

　　4.1 说明和下载

　　前面已经提到过2.4 内核中要使用iptables来做类似于ipchains在2.2内核中实现的事情。事实上，在2.4内核中的iptables包含了两个完全不同的部分：包过滤和地址转换。这两个功能在逻辑上是分开的，考虑到操作习惯上的问题才集成在一个配置程序(iptables)上。如果你安装了带有2.4内核的发行包，那么一般都带有iptables，并且在内核配置上也配置好了。如果要编译新的内核或者原来的内核没有支持iptables，需要选上相关的选项。使用make menuconfig 配置内核的时候操作：进入：Networking Options至少选择上：

　　 Network packet filtering (replaces ipchains)
　　    Network packet filtering debugging (NEW)
　　进入：IP: Netfilter Configuration  --->
　　 Connection tracking (required for masq/NAT) (NEW)
       FTP protocol support (NEW)
       IRC protocol support (NEW)     
　　等等……
　　把下面的选项全部标记<*>

　　逐级返回上面菜单后保存配置，然后按照编译内核的那一套程序进行即可（参见前面的内容）。这里需要说明的是，内核支持了iptables功能，还需要有外部程序才行。如果你是直接安装的带有2.4内核的发行包，应该已经带有iptables实用程序了。但是如果你是从2.2核心的发行包直接升级到2.4内核，尽管按照上面的做法配置的核心，但是还不能使用iptables。还必须下载和编译iptables外部程序才能使用iptables功能。下载的地址是：[http://www.netfilter.org](http://www.netfilter.org/) ，下载的文件是iptables-1.2.6a.tar.bz2或更新的版本。

　　4.2 安装：

　　这个.bz2文件比较奇怪吧？把文件复制到/usr/local/src中，然后用下列命令释放编译和安装： bzip2 -dc iptables-1.1.2.tar.bz2 |tar vxf - （最后那个‘- ’别忘了）

　　你的系统可能没有bzip2工具，如果没有，你就去下一个：

　　进入新生成的目录 ：cd iptables-1.2.6a 编译和安装：make ; make install

　　4.3 配置

　　如果你需要配置一个简单的网关，你仅仅需要配置一个转发加地址转换功能即可，我提供的参考脚本如下：

　　#begin
　　echo 1 > /proc/sys/net/ipv4/ip_forward
　　modprobe iptable_nat
　　modprobe ip_conntrack
　　modprobe ip_conntrack_ftp
　　modprobe ip_nat_ftp
　　iptables -F INPUT
　　iptables -F FORWARD
　　iptables –F OUTPUT
　　iptables -F POSTROUTING -t nat
　　iptables -t nat -F
　　iptables -P FORWARD DROP
　　iptables -A FORWARD -s 192.168.1.0/24 -j ACCEPT
　　iptables -A FORWARD -i eth1 -m state --state ESTABLISHED,RELATED -j ACCEPT 
　　iptables -t nat -A POSTROUTING -o eth1 -s 192.168.1.0/24 
　　　　-j SNAT --to 21.9.22.2
　　#end

　　其中3行modprobe是当你在内核中把iptables相关功能编译成模块(选成)的时候才用到的(也可能不止3行)。其中插入模块（modprobe）的几行比较重要，有时候当你作完NAT后，发现FTP不好用了，就要手工插入ip_nat_ftp模块（语法是：modprobe ip_nat_ftp）。黑体的地址部分可能是需要根据你网络的实际情况来决定的。最后一句话的意思是把凡是源地址是192.168.1.x的包做地址转换，转换为网关外侧地址21.9.22.2。好了，你已经建立起一个基本的网关了。其实iptables的功能及其强大，配置也非常灵活。其中的防火墙功能通过-t filter参数实现，地址转换功能通过-t nat实现（就像上面一样）。防火墙功能（packet filter）主要是3个链：INPUT,FORWARD,OUTPUT，地址转换功（nat）主要是3个链：PREROUTING, POSTROUTING,OUTPUT。观察当前的状态可以使用下面的命令：iptables –L –t filter 或 iptables –L –t nat。

　　好了，要配置出更灵活的、更强大的防火墙功能，全靠你自己了。我推荐你两个手册：《Linux 2.4 Packet Filter HOWTO》和《Linux 2.4 NAT HOWTO》。这两个文档是iptables的原作者写的，相当经典。英文不好？没关系，这两个文档都有中文版本。推荐你到：永远的UNIX(http: //www.fanqiang.com)下载。

　　(12)配置samba服务器
(13)构建基于Linux的VPN网络

　　构建VPN几乎是Linux的最高级应用之一了，学会了这项技术，是足以使你自豪的资本。VPN的主要用途就是建立一个加密的通信机制，然后通过把所有的你的子网的信息按照特定的方式加密传输，构成一个逻辑上的虚拟的网络。简单的说，就是一个Linux系统的IP层加密解决方案。这里面需要用到不少组件，下面一一介绍。

　　1、准备工作和安装

　　1．1 从[http://www.kernel.org](http://www.kernel.org/)下载2.4.X的内核，除了2.4.15版本（该版本有一个致命错误）。然后把该内核放置到： /usr/src中。(这里我们使用Linux-2.4.18.tar.gz)，然后释放：tar zxvf linux-2.4.18.tar.gz

　　1．2 删除原来的连接rm linux

　　1．3 ln –s linux-2.4.18.tar.gz linux

　　1．4 检查当前的网卡和SCSI的型号（参见内核升级一章）

　　1．5 cd linux (进入linux-2.4.18目录)

　　1．6 make menugonfig

　　1．7 make dep

　　1．8 make bzImage

　　1．9 编译工作做到这里就打住！

　　1．10 从[http://www.swox.com/gmp](http://www.swox.com/gmp) 下载gmp库的最新版本到/usr/local/src。

　　1．11 tar zxvf gmp-4.0.1.tar.gz

　　1．12 cd gmp-4.0.1

　　1．13 ./configure

　　1．14 make

　　1．15 make install

　　1．16 从[http://www.freeswan.org](http://www.freeswan.org/)下载freeswan-1.97.tar.gz（我们这里使用的版本）到/usr/local/src

　　1．17 tar zxvf freeswan-1.97.tar.gz

　　1．18 从[http://www.strongsec.com/freeswan/](http://www.strongsec.com/freeswan/) 下载x509patch-0.9.11-freeswan-1.97.tar.gz ，这个是补丁文件。释放，进入x509补丁目录，复制freeswan.diff到外面的freeswan源目录，然后回到freeswan源目录中运行：patch –p1 < freswan.diff

　　1．19 从[http://www.openssl.org](http://www.openssl.org/)下载openssl-0.9.6b版本到/usr/local/src，释放，进入目录

　　1．20 ./config (如果原来系统中有openssl需要先uninstall再安装，不过一般情况下你可能uninstall不下来8-)。如果无法uninstall，找到他们的路径，在这里通过—prefix=参数指定路径覆盖旧版本的文件。这里严重建议：如果你没有把握确定路径，最好这样做：到你用的发行包的开发商的FTP站点（如果你用turbolinux就去ftp.turbolinux.com用redhat就去ftp.redhat.com），用 anonymous用户和随便一个电子邮件作为密码登陆，找到你用的发行包版本的生机目录，然后下载相应的RPM包，注意：这里的包至少应该是 0.9.6b1以上的i386版本。下载完毕后用rpm –Uvh更新。这样做完了很干净。

　　1．21 make （如果你用rpm包升级就不用这个步骤了）

　　1．22 make test （如果你用rpm包升级就不用这个步骤了）

　　1．23 make install （如果你用rpm包升级就不用这个步骤了）安装完了以后执行openssl命令，输入version看看是不是你刚刚安装的版本。如果不是，可能没有覆盖原来安装的旧版本。从1.20重新来过

　　1．24 然后回到freeswan的源目录，运行：make menugo，在networking options中选择关于iptables和ipsec相关的所有选项。其中ipsec是freeswan加上的，最好把前面括号中M（模块方式）换成* （编译进内核）。下列内核选项应该选上：

　　进入：Networking Options至少选择上：

　　 Network packet filtering (replaces ipchains)
　　    Network packet filtering debugging (NEW)
　　进入：IP: Netfilter Configuration  --->
　　<*> Connection tracking (required for masq/NAT) (NEW)
      FTP protocol support (NEW)
      IRC protocol support (NEW)

　　注意：如果你需要使用DHCP功能，需要增加Pachet Socket mmapped IO和Socket Filtering两个选项，参见DHCP一章。

　　等等……

　　把下面的选项全部标记<*>

　　返回上一层菜单后，把列表选项最下面的凡是IPSEC相关的选项全部选为<*>。另外的，如果你打算使用拨号连接请在网络设备支持菜单选择ppp支持(注意，你用的ppp程序一定要2.4版本以上的)

　　1．25 然后检查网卡和硬盘选项是否正确，如果没有问题就逐层退出，然后保存配置。

　　1．26 退出后将自动编译内核，等待……

　　1．27 编译完成后，来到/usr/src/linux目录，运行：make modules;make modules_install

　　1．28 cp System..map /boot/System.map-2.4.18-vpn

　　1．29 cd arch/i386/boot

　　1．30 cp bzImage /boot/vmlinuz-2.4.18-vpn

　　1．31 cd /boot

　　1．32 rm System.map

　　1．33 ln –s System.map-2.4.18-vpn System.map

　　1．34 vi /etc/lilo.conf

　　增加一段：

　　boot=/dev/sda
　　map=/boot/map
　　install=/boot/boot.b
　　prompt
　　timeout=50
　　lba32
　　default=linux-vpn

　　image=/boot/vmlinuz
        label=linux
        initrd=/boot/initrd
        read-only
        root=/dev/sda5
　　image=/boot/vmlinuz-2.4.18-vpn
        label=linux-vpn
        initrd=/boot/initrd
        read-only
        root=/dev/sda5

　　1．35 运行lilo更新数据

　　1．36 reboot

　　1．37 启动后，运行：ipsec setup restart 应该不报任何错误而正常出现freeswan的版本。

　　注意：还有一些必要的内核参数配置，这些配置可以在rc.local中实现。他们是：

　　echo 0 > /proc/sys/net/ipv4/conf/eth0/rp_filter
　　echo 1 > /proc/sys/net/ipv4/ip_forward

　　如果你把下面两项编译成模块（前面扩号是M而不是*）：

　　   FTP protocol support (NEW)     
       IRC protocol support (NEW)

　　你需要在rc.local中加上:

　　modprobe ip_nat_ftp

　　安装完了，接下来我们将说明几种VPN的玩法。

　　2、配置Frees/wan用于支持双网关通信。也就是两个异地的子网通过一对ipsec的VPN网关进行交互访问。第一种玩法是网络对网络的VPN。一般的，某企业在甲乙两地（距离相当远）各有一个办公室，每个办公室各有一套LAN，他们分别通过专线连接到 internet网络上。甲LAN上是企业的管理中心，运行着企业的管理系统。而乙LAN上的用户也需要使用该管理系统，尽管乙LAN可以通过 internet（公网）连接过去，但是企业的老板说不行！我们的数据不能暴露在公网上，必须加密！好了，我们的VPN网络就可以应用于该企业的这种需求。首先在甲乙两个LAN的出口各放置一台我们的Linux服务器，他们都安装好了ipsec（前面说的安装步骤一个都不少），两个LAN的数据分别通过各自的这台机器（ipsec gateway）进入公网，凡是经过该网关的数据全部都是加密的。在效果上，两个LAN的用户可以互相ping到对方的机器，尽管他们可能一个是 192.168.1.0/24网段，另一个是192.168.10.0/24网段的。他们好像在同一个局域网中工作，没有界限。公共网络的加密部分对他们来说也是透明的。而两个LAN在公共网络上交换的数据是密文的。这就是虚拟专用网VPN。

　　但愿你已经按照前面的步骤顺利的安装好了两台机器，下面我告诉你怎样配置成网对网的环境。

　　2．1 我们先配置甲网的ipsec网关（该网关有两个网卡，我们配置他们的地址分别为eth1:192.168.1.231和eth0:21.9.22.22）。安装完成后，我们首先要做的事情是生成CA证书。（用到刚才安装的openssl）

　　2．2 找到openssl.cnf文件，一般在/etc/ssl/中，也可能在/var/ssl中或/usr/ssl中（实在不行你就find / -name “openssl.cnf”找找嘛！），要是有好几个，你要搞清楚哪个是你安装的版本。改动其中的default_bits选项的值从1024到 2048，然后改动default_days的值到3650。让期限为10年！保存退出。

　　2．3 在/var/中建立一个目录：/var/sslca，改变该目录的权限为700（chmod 700 /var/sslca）

　　2．4 在你安装的openssl目录中找到CA.sh脚本。注意，应该是你当前运行的openssl版本的CA.sh

　　2．5 cd /var/sslca 进入到你刚才建立的目录

　　2．6 比如你刚才找到的CA.sh在/usr/lib/ssl/misc/，那么就输入/usr/lib/ssl/misc/CA.sh –newca，接下来你会被问到一系列问题。问题和回答类似于下面的样子。如果你确认哪些你有把握更改就改，比如公司名称、邮件、密码等。不能确定的就按照下面的样子抄上即可。

　　~/sslca#/usr/lib/ssl/misc/CA.sh -newca
　　CA certificate filename (or enter to create)
(enter)
　　Making CA certificate ...
　　Using configuration from /usr/lib/ssl/openssl.cnf
　　Generating a 2048 bit RSA private key
　　........................................+++
　　........................................+++
　　writing new private key to './demoCA/private/./cakey.pem'
　　Enter PEM pass phrase![](http://www.ut365.com/bbs/images/smilies/sad.gif)enter password)
　　Verifying password - Enter PEM pass phrase![](http://www.ut365.com/bbs/images/smilies/sad.gif)enter same password again)
　　-----
　　You are about to be asked to enter 　　
　　information that will be incorporated
　　into your certificate request.
　　What you are about to enter is what is called a Distinguished Name or a DN.
　　There are quite a few fields but you can leave some blank
　　For some fields there will be a default value,
If you enter '.', the field will be left blank.
　　-----
　　Country Name (2 letter code) [AU]:US(enter)
　　State or Province Name (full name) [Some-State]:State(enter)
　　Locality Name (eg, city) []:City(enter)
　　Organization Name (eg, company) [Internet Widgits Pty Ltd]:21vianet(enter)
　　Organizational Unit Name (eg, section) []![](http://www.ut365.com/bbs/images/smilies/sad.gif)enter)
　　Common Name (eg, YOUR name) []:CA(enter)
　　Email Address []:ca@xxx.com(enter)
　　~/sslca#

　　2．7 下一步是给网关生成证书:

　　命令和要回答的问题如下：

　　~/sslca# /usr/lib/ssl/misc/CA.sh -newreq

　　Using configuration from /usr/lib/ssl/openssl.cnf
　　Generating a 2048 bit RSA private key
　　...................................+++
　　...............................+++
　　writing new private key to 'newreq.pem'
　　Enter PEM pass phrase:(enter password)
　　Verifying password - Enter PEM pass phrase:(repeat password)
　　-----
　　You are about to be asked to enter information that will be incorporated
　　into your certificate request.
　　What you are about to enter is what is called a Distinguished Name or a DN.
　　There are quite a few fields but you can leave some blank
　　For some fields there will be a default value,
　　If you enter '.', the field will be left blank.
　　-----
　　Country Name (2 letter code) [AU]:US(enter)
　　State or Province Name (full name) [Some-State]:State(enter)
　　Locality Name (eg, city) []:City(enter)
　　Organization Name (eg, company) [Internet Widgits Pty Ltd]:ExampleCo(enter)
　　Organizational Unit Name (eg, section) []:(enter)
　　Common Name (eg, YOUR name) []:vpnserver.rd.xxx.com(enter)
　　Email Address []:user@xxx.com(enter)

　　Please enter the following 'extra' attributes
　　to be sent with your certificate request
　　A challenge password []:(enter)
　　An optional company name []:(enter)
　　Request (and private key) is in newreq.pem
　　natecars@buzzword:~/sslca$ /usr/lib/ssl/misc/CA.sh -sign
　　Using configuration from /usr/lib/ssl/openssl.cnf
　　Enter PEM pass phrase:(password you entered for the ca certificate)
　　Check that the request matches the signature
Signature ok
　　The Subjects Distinguished Name is as follows
　　countryName           ![](http://www.ut365.com/bbs/images/smilies/tongue.gif)RINTABLE:'US'
　　stateOrProvinceName   ![](http://www.ut365.com/bbs/images/smilies/tongue.gif)RINTABLE:'State'
　　localityName          ![](http://www.ut365.com/bbs/images/smilies/tongue.gif)RINTABLE:'City'
　　organizationName      :PRINTABLE:'21vianet'
　　commonName            :PRINTABLE:'vpnserver.rd.xxx.com'
　　emailAddress          :IA5STRING:'[rd@xxx.com](mailto:rd@xxx.com)'
　　Certificate is to be certified until Feb 13 16:28:40 2012 GMT (3650 days)
　　Sign the certificate? [y/n]:y(enter)

　　1 out of 1 certificate requests certified, commit? [y/n]y(enter)
　　Write out database with 1 new entries
Data Base Updated
　　(certificate snipped)
　　Signed certificate is in newcert.pem

　　在以上步骤中一定要记住你输入的密码。如果哪一不错了，想重新来过的话，记住删除/var/sslca目录下面的所有子目录即可。

　　2．8 把文件名字改为你需要的

　　~/sslca# mv newcert.pem vpnserver.rd.xxx.com.pem
　　~/sslca# mv newreq.pem vpnserver.rd.xxx.com.key

　　2．9 编辑.key文件，删除'-----BEGIN CERTIFICATE REQUEST-----'后面所有的东西，之后，这个文件应该从'-----BEGIN RSA PRIVATE KEY-----'至'-----END RSA PRIVATE KEY-----'结束。

　　2．10 如果正常的安装了x.509补丁，你应该可以看到/etc/ipsec.d及其下面的目录。如果没有，你就自己建立。然后按照下面的位置复制合适的文件：(此时你仍然在/var/sslca目录中)

　　# cp vpnserver.rd.xxx.com.key /etc/ipsec.d/private
　　# cp vpnserver.rd.xxx.com.pem /etc/ipsec.d
　　# openssl x509 -in demoCA/cacert.pem -outform der -out rootca.der
　　# cp rootca.der /etc/ipsec.d/cacerts/RootCA.der
　　# openssl x509 -in host.example.com.pem -outform der -out /etc/x509cert.der
　　# openssl ca -gencrl -out crl.pem
　　# cp crl.pem /etc/ipsec.d/crls

　　2．11在/etc/ipsec.secrets中加入一行：: RSA vpnserver.rd.xxx.com.key "password"，然后删除其他所有行。其中的password是你前面生成密要的时候回答问题输入的密码。

　　2．12编辑ipsec.conf文件类似下面的样子：

　　# basic configuration
　　config setup
        interfaces=%defaultroute
        klipsdebug=none
        plutodebug=none
        plutoload=%search
        plutostart=%search
        uniqueids=yes

　　conn %default
        keyingtries=1
        compress=yes
        disablearrivalcheck=no
        authby=rsasig
        leftrsasigkey=%cert
        rightrsasigkey=%cert
        left=%defaultroute
        leftcert=vpnserver.rd.xxx.com.pem
        auto=add
        pfs=yes

　　conn roadwarrior
        right=%any

　　conn roadwarrior-net
        leftsubnet=192.168.1.0/255.255.255.0
        right=%any

　　conn net-net
        leftsubnet=192.168.1.0/255.255.255.0
        right=%any
        rightsubnet=192.168.10.0/255.255.255.0

　　conn world-net
        leftsubnet=*
        right=%any
        rightsubnet=192.168.10.0/255.255.255.0

　　黑体字的部分是你需要根据你实际的环境更改的。我们用的两端的子网是192.168.1.0/24和192.168.10.0/24。文件中 vpnserver.rd.xxx.com.pem是CA证书。这个配置是通用的，也就是说可以适用LAN-LAN方式的俩接和远端客户端两种方式。

　　2．13 下面配置分支机构的网关。这个机器也要事先安装完全freeswan等软件。

　　重新执行上面的2.7-2.9步骤生成分支机构的证书，注意：在回答问题的过程中，关于主机名称的部分的输入改变为你分支机构的网关机器名称，比如：vpncliet.rd.xxx.com。

　　2．14 复制下列生成的文件到分支机构网关上的相应位置(比如通过软盘复制)：

　　cp vpnserver.rd.xxx.com.pem /etc/ipsec.d
　　cp vpnclient.rd.xxx.com.key /etc/ipsec.d/private
　　cp vpnclient.rd.xxx.com.pem /etc/ipsec.d
　　执行命令：openssl x509 –in vpnclient.rd.xxx.com.pem –
　　　　outform der –out /etc/x509cert.de
　　cp rootca.der /etc/ipsec.d/cacerts/RootCA.der
　　cp crl.pem /etc/ipsec.d/crls

　　2．15 配置分支机构的/etc/ipsec.secrets 写上一行：

　 RSA vpnclient.rd.xxx.com.key "password"。

　　其他行删除。Password就是前面回答问题的时候输入的密码。

　　2．16 配置/etc/ipsec.conf

　　# basic configuration
　　config setup
        interfaces=%defaultroute
        klipsdebug=none
        plutodebug=none
        plutoload=%search
        plutostart=%search
        uniqueids=yes

　　conn %default
        keyingtries=0
        compress=yes
        disablearrivalcheck=no
        authby=rsasig
        rightrsasigkey=%cert
        leftrsasigkey=%cert
        right=%defaultroute
        rightcert=vpnclient.rd.xxx.com.pem
        auto=add
        pfs=yes

　　conn roadwarrior
        left=21.9.22.22
        leftcert=vpnserver.rd.xxx.com.pem

　　conn roadwarrior-net
        left=21.9.22.22
        leftcert=vpnserver.rd.xxx.com.pem
        leftsubnet=192.168.1.0/255.255.255.0

　　conn net-net
        left=21.9.22.22
        leftcert=vpnserver.rd.xxx.com.pem
        leftsubnet=192.168.1.0/255.255.255.0
        rightsubnet=192.168.10.0/255.255.255.0

　　其中黑体的部分是你可以根据实际情况修改的。

　　2．17 首先启动server端的ipsec：ipsec setup restart，然后同样启动客户端的ipsec

　　2．18 建立通道：ipsec auto –up net-net然后在Server端可以用命令ipsec whack status应该可以看到新建立的几个通道。此时，你在两个子网中应该可以互相ping 通。

　　3、配置Frees.wan用于支持远程客户端访问。也就是允许一个Windows客户端，来通过VPN和公司内部的网络进行通讯。

　　第2种VPN玩法其实就是把分支机构的LAN换成一个单独的，地址不固定的机器。这样的应用适合总经理出差的时候从外地ISP拨号上网连接到本部网络的情况。期间，非但有认证，而且通过ISP和公网的数据全部是加密的。这种方式在服务器端的配置和上面完全一样（记得吗？我们在行面给出的配置已经是兼顾了两种用法）。这里需要做的是把总经理的笔记本配成VPN客户端。他的笔记本应该是windows2000的并且升级到sp2。

　　3．1 首先重复2.7-2.9步骤生成证书，其中有关主机名的部分可以输入你总经理的机器名。类似于：win.rd.xxx.com。当然，总经理的机器要是同名的。

　　3．2 在服务器端生成windows可以认的p12格式的密钥。

　　openssl pkcs12 -export 
        -in win.rd.xxx.com.pem 
　　　　-inkey win.rd.xxx.com.key 
　　　　-certfile demoCA/cacert.pem 
　　　　-out win.rd.xxx.com.p12

　　3．3 用命令察看环境：最好把结果输出到文件记住，以后用得到。

　　openssl x509 -in demoCA/cacert.pem -noout -subject

　　3．4 把上面生成的p12文件传送到总经理的机器上，放在一个正规的地方（这个文件很重要）。

　　3．5 在总经理的机器上从[http://vpn.ebootis.de](http://vpn.ebootis.de/)站点下载：ipsec.exe

　　3．6 在总经理的机器上从： [http://agent.microsoft.com/windo ... sting/ipsecpolo.asp](http://agent.microsoft.com/windows2000/techinfo/reskit/tools/existing/ipsecpolo.asp)站点下载windwos2000的ipsec资源工具。

　　3．7 安装上述两个软件，并且把他们放在同一个目录中。

　　3．8 建立一个ipsec的MMC：（希望你知道MMC是什么）

　　依次进入Start/Run/MMC，

　　File (or Console) - Add/Remove Snap-in
　　单击 'Add'
　　选 'Certificates', 然后选 'Add'
　　选 'Computer Account', 然后点 'Next'.
　　选 'Local computer', 然后点 'Finish'.
　　选 'IP Security Policy Management', 然后点 'Add'.
　　选 'Local Computer', 然后点 'Finish'
　　选 'Close' 然后点 'OK'

　　3．9 增加一个证书

　　展开左侧窗口中 'Certificates (Local Computer)'

　　右键 'Personal', 选 'All Tasks' 然后点 'Import'

　　点 Next

　　输入哪个.p12 文件的路径 (就是刚才你从服务器网关复制过来的,浏览选择也可), 然后点'Next' 输入export password（密码）, 然后点Next 选'Automatically select the certificate store based on the type of certificate', 然后点Next 点Finish, 如果有任何提示窗口弹出都选yes 退出MMC, 保存当前配置到管理工具中，这样就不用每次都重新来过了。以上所做就增加了一个证书到总经理的机器上。

　　3.10设置ipsec工具：

　　编辑总经理机器上的ipsec.conf文件，把rightca的=后面写成刚才openssl x509 -in demoCA/cacert.pem -noout –subject命令生成的结果。类似下面这样：

　　conn roadwarrior
        left=%any
        right=(ip_of_remote_system)
        rightca="C=US,S=State,L=City,O=21vianet,
　　　　　　CN=CA,Email=ca@xxx.com"
        network=auto
        auto=start
        pfs=yes

　　conn roadwarrior-net
        left=%any
        right=(ip_of_remote_system)
        rightsubnet=192.168.1.0/24
        rightca="C=US,S=State,L=City,O=21vianet,
　　　　　　CN=CA,Email=ca@xxx.com"
        network=auto
        auto=start
        pfs=yes

　　黑体部分要注意配置正确。

　　3.12运行ipsec.exe有下面输出：

　　C:ipsec>ipsec
　　IPSec Version 2.1.4 (c) 2001,2002 Marcus Mueller
　　Getting running Config ...
　　Microsoft's Windows XP identified
　　Host name is: (local_hostname)
　　No RAS connections found.
　　LAN IP address: (local_ip_address)
　　Setting up IPSec ...

        Deactivating old policy...
        Removing old policy...

　　Connection roadwarrior:
        MyTunnel     : (local_ip_address)
        MyNet   : (local_ip_address)/255.255.255.255
        PartnerTunnel: (ip_of_remote_system)
        PartnerNet   : (ip_of_remote_system)/255.255.255.255
        CA (ID)      : C=US,S=State,L=City,O=ExampleCo,...
        PFS          : y
        Auto         : start
        Auth.Mode    : MD5
        Rekeying     : 3600S/50000K
        Activating policy...

　　Connection roadwarrior-net:
        MyTunnel     : (local_ip_address)
        MyNet        : (local_ip_address)/255.255.255.255
        PartnerTunnel: (ip_of_remote_system)
        PartnerNet   : (remote_subnet)/(remote_netmask)
        CA (ID)      : C=US,S=State,L=City,O=ExampleCo,...
        PFS          : y
        Auto         : start
        Auth.Mode    : MD5
        Rekeying     : 3600S/50000K
        Activating policy...

　　C:ipsec>

　　这时候你从客户端ping服务器后面的内网得到几个'Negotiating IP Security'之后就可以ping通了。这样总经理带着这台笔记本到有互联网络的地方就可以象在办公室一样连接到公司里了。

　　值的注意的是，出于安全性的问题，我们建议你关闭VPN网关上面的所有其他服务，并仔细配置防火墙。通常的，如果你希望把所有的流量都发送给主站网关，在从站就不需要增加iptables策略。否则，需要增加这样一条策略：

　　iptables –t nat –A POSTROUTING –o eth0 –j MASQUERADE

　　在主站由于路由的原因，需要增加下面的策略：

　　iptables –t nat –A POSTROUTING –o ipsec0 –s ! 192.168.10.0/255.255.255.0 –
　　　　j SNAT –to 192,168.1.231
　　iptables –t nat –A POSTROUTING –o eth1 –s ! 192.168.10.0/255.255.255.0 –
　　　　j SNAT –to 192,168.1.231

　　每行中前面的黑体是子网网段，后面的是本网关的内侧出口。

　　注：原版资料在[http://vpn.ebootis.de/](http://vpn.ebootis.de/) 详细资料在：[http://www.natecarlson.com/inclu ... amp;page=ipsec-x509](http://www.natecarlson.com/include/showpage.php?cat=linux&page=ipsec-x509)

　　(14)安装另一种邮件系统postfix

　　任何一种知名的事物似乎一定有至少一个跟风之作（微软的Windows系列似乎例外，跟风者无一例外的夭折了），并且这些后来者往往有他们更出色的方面才能立足。在UNIX世界的邮件服务器领域，没有哪个产品能够有sendmail程序这么大的名气，尽管它有一个非常晦涩的配置文件。同样的，有不少人为了改进Sendmail的缺点做了很多其他的邮件服务器产品，并且的它们都有各自鲜明的特色，并且吸引了不少用户。其中佼佼者当属Qmail和 Postfix邮件系统了。这里我们之所以选择Postfix介绍，主要基于下面两个原因：其一、Postfix是一个和Sendmail一样的为高负载邮件服务器设计的MTA(邮件传输代理)，而Qmail处理能力要差一些（它比较适合中小型的应用场合）。在某些情况下Postfix甚至比 Sendmail速度快3倍。其二、Postfix是按照兼容Sendmail的设计路子来做的，相当的配置文件都可以直接使用。这样原有的 Sendmail用户可很容易的升级的Postfix。这是一个很“优惠”的升级条件，吸引了很多的原来的Sendmail用户。

　　安装和配置Postfix邮件系统的最基本步骤如下：

　　1、从[http://www.postfix.org](http://www.postfix.org/)下载Postfix的最新版本。我们这里使用的版本是postfix1.1.5，文件名字叫作postfix-1.1.5.tar.gz，把这个文件下载到/usr/local/src里面

　　2、cd /usr/local/src

　　3、tar zxvf postfix-1.1.5.tar.gz释放压缩文件

　　4、cd postfix-1.1.5

　　5、vi INSTALL仔细阅读该安装文件和注意事项。

　　6、make clean

　　7、make

　　8、useradd postfix （或者是adduser postfix）增加一个新的用户。然后用：vi /etc/passwd 编辑用户文件的postfix一行，让该行看起来类似：postfix:*:2126:2128:postfix:/no/where: /no/shell 其中号码部分不要改动。这主要是为了安全性考虑。

　　9、groupadd postdrop 建立一个组，但是这个组不能包括任何一个用户。/etc文件中的相关行类似于：postdrop:*:54321:

　　10、检查/etc/mail/aliases里面有没有postfix: root一行，没有就加上。

　　11、# mv /usr/sbin/sendmail /usr/sbin/sendmail.OFF

　　12、# mv /usr/bin/newaliases /usr/bin/newaliases.OFF

　　13、# mv /usr/bin/mailq /usr/bin/mailq.OFF

　　14、# chmod 755 /usr/sbin/sendmail.OFF /usr/bin/newaliases.OFF /usr/bin/mailq.OFF

　　15、make install （新安装）

　　16、make upgrade （升级）

　　在执行上述两步（选择其中一个）的时候，脚本会提问许多路径什么的，建议不要做改动一路回车下来，除非你确定你知道改动默认值的必要性。

　　17、启动是postfix start

　　注意：以上步骤10-14是从sendmail升级的时候用的。

　　很可能的，这样安装完了系统还不能收发邮件。首先，你应该安装一个pop3邮件接收协议服务（参见pop3安装）。另外的，需要按照下列步骤检查一下相关的配置文件。

　　1、/etc/mail/access是允许访问的控制文件，类似下面内容，注意：地址就别照抄了。

　　127.0.0.1 RELAY
　　21.9.22 RELAY
　　211.151.194.14 RELAY

　　事实上这个文件可以定义得相当复杂。模板和说明可以参照：/etc/postfix/access

　　2、编辑完成/etc/access文件，还要把它编译成数据库格式：makemap hash access.db < access

　　3、/etc/mail/aliases是别名文件，类似下面的内容：

　　MAILER-DAEMON: postmaster
　　postmaster: root
　　bin: root
　　daemon: root
　　nobody: root
　　postfix: root

　　这个文件的模板和说明可以参见/etc/postfix/aliases。编辑完成后，用newaliases aliases转换成数据库格式。

　　4、/etc/mail/local-host-names写了本地主机的名字，内容类似于：

　　rd.xxx.com
　　tls65.rd.xxx.com

　　5、主配置文件：/etc/postfix/main.cf ，看一下内容，比sendmial.cf强多了吧？每一项都有详细的说明和示例。最主要的（仅能使运行的）配置项目在载面介绍一下。

　　5．1 mydomain

　　指明你的域名，在这里我们指定：

　　mydomain = test.com
　　5．2 myorigin

　　myorigin参数指明发件人所在的域名。如果你的用户的邮件地址为[user@domain.com](mailto:user@domain.com),则 该参数指定@后面的域名。在这里我们指定：

　　myorigin = $mydomain 
　　5．3 mydestination

　　mydestination参数指定postfix接收邮件时收件人的域名，换句话说，也就是你的postfix 系统要接收什么样的邮件。通常与myorigin一样：

　　mydestination = $mydomain
　　5．4 mynetworks_style

　　设置网络类型，我们指定：

　　mynetworks_style = subnet
　　5．5mynetworks

　　定义可以使用的此smtp服务器发信的客户ip地址，因为公司的ip范围已知，所以我们指定：

　　mynetworks = 192.168.1.0/24

　　6、设定DNS服务器的MX记录。例如，在域test.com中，安装postfix的服务器名字叫mail.test.com，则在本域的DNS中设定MX记录为mail.test.com

　　7、更改设定后不必重新启动postfix，只需要用postfix reload命令重新加载配置即可。

　　8、post还支持虚拟域名，配置方法如下：

　　8．1 首先把otherdomain.com（要用的虚拟域名）的MX记录指向mail.test.com（本域的邮件服务器），这个配置在负责解析otherdomain.com的DNS做。

　　8．2 为了让用户可以使用[username@otherdomain.com](mailto:username@otherdomain.com)收发邮件，我们必须配置虚拟域，在main.cf文件中，我们添加以下内容：

　　virtual_maps = hash:/etc/postfix/virtual

　　这里假定用户tom、test、white需要使用这个虚拟域名，当然首先要在系统中添加这几个用户（用useradd或adduser填加系统用户），然后建立/etc/postfix/virtual文件并添加以下内容：

　　othername.com anything
[tom@othername.com](mailto:tom@othername.com) tom
[test@othername.com](mailto:test@othername.com) test
[white@othername.com](mailto:white@othername.com) white

　　之后，用postmap命令生成虚拟域数据库：

　　postmap /etc/postfix/virtual

　　明白上面配置文件的例子，你就可以非常灵活的多域名邮件系统了。

五、日常维护

