# 闲聊linux中的input设备(转) - h13 - 博客园
 转自：[http://blog.csdn.net/lmm670/article/details/6080998](http://blog.csdn.net/lmm670/article/details/6080998)
  用过linux的哥们都知道，linux所有的设备都是以文件的形式实现的，要访问一个设备，我们只需要以open、read、write的形式对设备的进行操作就可以了。在linux系统的/dev目录下，罗列了当前系统支持的所有设备。运行 ls /dev一下，着实吓了一大跳，
```
[root@localhost ~]# ls /dev
adsp        full     midi      ram9        tty15  tty42  ttyS3
agpgart     fuse     mixer     ramdisk     tty16  tty43  urandom
audio       hpet     net       random      tty17  tty44  usbdev1.1_ep00
bsg         hvc0     null      root        tty18  tty45  usbdev1.1_ep81
bus         hvc1     nvram     rtc         tty19  tty46  usbmon0
cdrom       hvc2     oldmem    scd0        tty2   tty47  usbmon1
console     hvc3     parport0  sda         tty20  tty48  vcs
core        hvc4     parport1  sda1        tty21  tty49  vcs1
disk        hvc5     parport2  sda2        tty22  tty5   vcs2
dmmidi      hvc6     parport3  sda3        tty23  tty50  vcs3
dsp         hvc7     port      sequencer   tty24  tty51  vcs4
fd          initctl  ppp       sequencer2  tty25  tty52  vcs5
fd0         input    ptmx      sg0         tty26  tty53  vcs6
fd0u1040    kmsg     pts       sg1         tty27  tty54  vcs7
fd0u1120    log      ram       shm         tty28  tty55  vcs8
fd0u1440    loop0    ram0      snapshot    tty29  tty56  vcsa
fd0u1600    loop1    ram1      snd         tty3   tty57  vcsa1
fd0u1680    loop2    ram10     sr0         tty30  tty58  vcsa2
fd0u1722    loop3    ram11     stderr      tty31  tty59  vcsa3
fd0u1743    loop4    ram12     stdin       tty32  tty6   vcsa4
fd0u1760    loop5    ram13     stdout      tty33  tty60  vcsa5
fd0u1840    loop6    ram14     systty      tty34  tty61  vcsa6
fd0u1920    loop7    ram15     tty         tty35  tty62  vcsa7
fd0u360     lp0      ram2      tty0        tty36  tty63  vcsa8
fd0u720     lp1      ram3      tty1        tty37  tty7   X0R
fd0u800     lp2      ram4      tty10       tty38  tty8   XOR
fd0u820     lp3      ram5      tty11       tty39  tty9   zero
fd0u830     MAKEDEV  ram6      tty12       tty4   ttyS0
floppy      mapper   ram7      tty13       tty40  ttyS1
floppy-fd0  mem      ram8      tty14       tty41  ttyS2
```
这么多设备那么管理起来是不是很麻烦，linux内核的开发者智商自然在你我之上，他们把所有的这些设备归为三大类，即平时我们熟悉的字符设备、块设备、网络设备。运行 ls –l /dev（这里我只取一部分显示信息）
crw-rw----+ 1 root   root    14,  12 12-16 00:57 adsp
crw-------  1 root   root    10, 175 12-16 00:57 agpgart
crw-rw----+ 1 root   root    14,   4 12-16 00:57 audio
drwxr-xr-x  2 root   root         80 12-16 00:57 bsg
drwxr-xr-x  3 root   root         60 12-16 00:57 bus
lrwxrwxrwx  1 root   root          3 12-16 00:57 cdrom -> sr0
crw-------  1 lmm670 root     5,   1 12-16 00:57 console
lrwxrwxrwx  1 root   root         11 12-16 00:57 core -> /proc/kcore
drwxr-xr-x  5 root   root        100 12-16 00:57 disk
crw-rw----  1 root   root    14,   9 12-16 00:57 dmmidi
crw-rw----+ 1 root   root    14,   3 12-16 00:57 dsp
lrwxrwxrwx  1 root   root         13 12-16 00:57 fd -> /proc/self/fd
brw-r-----  1 root   floppy   2,   0 12-16 00:57 fd0
brw-r-----  1 root   floppy   2,  84 12-16 00:57 fd0u1040
brw-r-----  1 root   floppy   2,  88 12-16 00:57 fd0u1120
brw-r-----  1 root   floppy   2,  28 12-16 00:57 fd0u1440
brw-r-----  1 root   floppy   2, 124 12-16 00:57 fd0u1600
brw-r-----  1 root   floppy   2,  44 12-16 00:57 fd0u1680
brw-r-----  1 root   floppy   2,  60 12-16 00:57 fd0u1722
brw-r-----  1 root   floppy   2,  76 12-16 00:57 fd0u1743
brw-r-----  1 root   floppy   2,  96 12-16 00:57 fd0u1760
brw-r-----  1 root   floppy   2, 116 12-16 00:57 fd0u1840
brw-r-----  1 root   floppy   2, 100 12-16 00:57 fd0u1920
brw-r-----  1 root   floppy   2,  12 12-16 00:57 fd0u360
brw-r-----  1 root   floppy   2,  16 12-16 00:57 fd0u720
brw-r-----  1 root   floppy   2, 120 12-16 00:57 fd0u800
brw-r-----  1 root   floppy   2,  52 12-16 00:57 fd0u820
brw-r-----  1 root   floppy   2,  68 12-16 00:57 fd0u830
lrwxrwxrwx  1 root   root          3 12-16 00:57 floppy -> fd0
lrwxrwxrwx  1 root   root          3 12-16 00:57 floppy-fd0 -> fd0
crw-rw-rw-  1 root   root     1,   7 12-16 00:57 full
crw-rw----  1 root   fuse    10, 229 12-16 00:57 fuse
crw-rw----  1 root   root    10, 228 12-16 00:57 hpet
crw-rw----  1 root   uucp   229,   0 12-16 00:57 hvc0
crw-rw----  1 root   uucp   229,   1 12-16 00:57 hvc1
crw-rw----  1 root   uucp   229,   2 12-16 00:57 hvc2
crw-rw----  1 root   uucp   229,   3 12-16 00:57 hvc3
crw-rw----  1 root   uucp   229,   4 12-16 00:57 hvc4
crw-rw----  1 root   uucp   229,   5 12-16 00:57 hvc5
crw-rw----  1 root   uucp   229,   6 12-16 00:57 hvc6
crw-rw----  1 root   uucp   229,   7 12-16 00:57 hvc7
prw-------  1 root   root          0 12-16 00:58 initctl
drwxr-xr-x  3 root   root        200 12-16 00:57 input
crw-rw----  1 root   root     1,  11 12-16 00:57 kmsg
srw-rw-rw-  1 root   root          0 12-16 00:58 log
brw-r-----  1 root   disk     7,   0 12-16 00:57 loop0
大家可以看到，每一行的第一个字母，代表着此文件的类型。c表示字符设备，b表示块设备，s表示网络设备，细心的哥们会问，不是说只有三类设备吗，怎么还有其他类型开头的呢？比如d、l等等。对不起，这里讲的是文件类型，d表示是一个目录文件，l表示一个链接文件。至于这三者之间的区别，我就不在这啰嗦了，Google一下一大堆。我要强调的是，无论上面三个设备中的任何一种设备，要想在linux实现它的设备驱动，首先要对它进行一系列的初始化工作，然后需给它提供一个设备操作集合（或者更简单一点理解：接口函数），用来提供给我们的上层程序进行访问，比如open，read，write等等。要不然，我要你这个设备驱动干嘛。在字符设备驱动中，我们的操作集函数是这样的
```
struct file_operations {
       struct module *owner;
       loff_t (*llseek) (struct file *, loff_t, int);
       ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
       ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
       ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
       ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
       int (*readdir) (struct file *, void *, filldir_t);
       unsigned int (*poll) (struct file *, struct poll_table_struct *);
       int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
       long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
       long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
       int (*mmap) (struct file *, struct vm_area_struct *);
       int (*open) (struct inode *, struct file *);
       int (*flush) (struct file *, fl_owner_t id);
       int (*release) (struct inode *, struct file *);
       int (*fsync) (struct file *, struct dentry *, int datasync);
       int (*aio_fsync) (struct kiocb *, int datasync);
       int (*fasync) (int, struct file *, int);
       int (*lock) (struct file *, int, struct file_lock *);
       ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
       unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
       int (*check_flags)(int);
       int (*flock) (struct file *, int, struct file_lock *);
       ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
       ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
       int (*setlease)(struct file *, long, struct file_lock **);
};
```
网名为“卖血去上网”的兄弟要说了，写一个驱动接口函数怎么这么复杂，要提供这么多接口函数。其实不然，一般的设备驱动接口函数并不需要把上面的所有都实现，只需要实现里面的一些：比如我们这里：
```
static const struct file_operations evdev_fops = {
       .owner           = THIS_MODULE,
       .read              = evdev_read,
       .write             = evdev_write,
       .poll        = evdev_poll,
       .open             = evdev_open,
       .release    = evdev_release,
       .unlocked_ioctl      = evdev_ioctl,
#ifdef CONFIG_COMPAT
       .compat_ioctl  = evdev_ioctl_compat,
#endif
       .fasync           = evdev_fasync,
       .flush             = evdev_flush
};
```
兄弟们注意了，这个结构体就是前面那个结构体的实现，类似于c++中的类和对象。结构中若干个等式中，我们最终要实现的是右边的那些函数。这样做的目的大家都清楚：实现统一接口，增加程序的可移植性。上层代码每次open evdev这个设备的时候最终都会通过file_operations落实到我们的evdev_open函数。鲁迅先生曾说过：驱动代码写起来其实并不难，当接口函数多了就变难了。说了一大推好像被忽悠了，怎么没提到一点关于input设备的信息，关于input设备我现在只提一句，input设备的接口函数，linux内核已经为我们写好了。毕竟时代在进步，内核在更新，鲁先生的话也可以改一下了：设备驱动的实现本身很难，自从有了input设备子系统，就变得不难了（当然只针对input设备）。到底何谓input设备呢？
究竟何谓input设备，相信武汉跳蚤市场上卖宠物小狗的大妈都能一口答出来，你能不知道么？对，就是我们传说中的输入设备。说到输入设备，相信用过电脑的兄弟都不会陌生了，即按键、鼠标、键盘、等一系列需要我们用户“动手”产生信息，然后丢给我们聪明绝顶的pc来处理的设备。前面说了，linux内核input子系统中已经实现了input设备的接口函数，这使得我们工作量大大的减轻了。我们以akm8973芯片（用于智能手机指南针的主功能芯片，实际上就一电子罗盘）为例，来简单看一下写一个input设备我们需要做的工作。
首先，在驱动模块加载函数中申请一个input设备，并告知input子系统它支持哪些事件，如下所示：
```
akm->input_dev = input_allocate_device()；
set_bit(EV_ABS, akm->input_dev->evbit);
input_set_abs_params(akm->input_dev, ABS_RX, 0, 23040, 0, 0);
input_set_abs_params(akm->input_dev, ABS_RY, -11520, 11520, 0, 0);
input_set_abs_params(akm->input_dev, ABS_RZ, -5760, 5760, 0, 0);
input_set_abs_params(akm->input_dev, ABS_THROTTLE, -30, 85, 0, 0);
input_set_abs_params(akm->input_dev, ABS_RUDDER, 0, 3, 0, 0);
input_set_abs_params(akm->input_dev, ABS_HAT0X, -2048, 2032, 0, 0);
input_set_abs_params(akm->input_dev, ABS_HAT0Y, -2048, 2032, 0, 0);
input_set_abs_params(akm->input_dev, ABS_BRAKE, -2048, 2032, 0, 0);
```
以上这些都是为让input子系统支持的某些参数而设置的，EV_ABS表示支持绝对值坐标，后面都是针对这些坐标的一些参数访问范围设置。至于为什么这样设置，我们继续往下走，到后面我们就明白了。
接着，在驱动模块函数中注册输入设备：
err = input_register_device(akm->input_dev);
然后，报告发生的一些事件以及对应的坐标。
input_report_abs(data->input_dev, ABS_RX, rbuf[0]);
input_report_abs(data->input_dev, ABS_RY, rbuf[1]);
input_report_abs(data->input_dev, ABS_RZ, rbuf[2]);
对应的三个方向的坐标值就被驱动记录下来了。
深入里面跟踪一下：
```
static inline void input_set_abs_params(struct input_dev *dev, int axis, int min, int max, int fuzz, int flat)
{
       dev->absmin[axis] = min;
       dev->absmax[axis] = max;
       dev->absfuzz[axis] = fuzz;
       dev->absflat[axis] = flat;
 
       dev->absbit[BIT_WORD(axis)] |= BIT_MASK(axis);
}
```
这个函数用来干嘛的呢？这个留到以后讲，不过你得多个心眼，后面用得到的。
添加一个input设备，我们要做的工作就这些了。接下来我们就可以通过input内核子系统提供的接口函数来处理这些坐标值，把把他们传到用户空间。
看完这些，让我想起来了食堂门前那些招行的办信用卡活动。办一个信用卡送一个U盾。没办法，现在的社会什么事情都要收费，连上厕所也得给点小费，不给钱不让进，哪怕你当众尿裤子。所以更别说我们如此有技术含量的保护大家网上购物安全的U盾了。去银行办一个U盾，至少得花个50大洋以上。所以很多像我这样的哥们，毫不犹豫的办了一个信用卡。反正就填几张表，然后就可以免费获得一个u盾，多好啊。网名为“唐伯虎点蚊香”的兄弟马上发话了：“这不就和我们的input设备那个一样的吗，我们这些比较懒的家伙为了避免去完善那些复杂的设备接口函数集，所以干脆把它注册成一个input设备，所以你就得先申请它，注册它等一系列预备工作，（就如我们为了u盾而填的那些表格）做好这些之后，我们就实行鲁迅先生的拿来主义，直接使用input子系统的的接口函数”。
不过不是什么设备都可以注册成inpunt设备的。就好比一兄弟随便拿了一张纸，画一只小鸡，然后头上加个光圈，就号称是唐伯虎的名画“神鸟凤凰图”，然后递给招行的工作人员说“我表格填好了，给我来一个U盾”，人家会以为这哥们肯定刚从精神病院出来的。
从前一节来看，在linux内核中添加一个input设备变得很简单了。我们再也不必须去动手写那些该死的接口函数了。可是你有没有想过，是谁让我们的工作变得这么简单了呢?答案是linux内核中的input core。她总是那么痴情，默默地不求回报地为你做许许多多的事情，在你背后默默的支持你爱着你。是的，你所想到的大多数事情，我们的input core都已经为你做好。除了感动，我们还能说什么呢？（input core对应的实体在linux内核源码目录linux-2.6.29/drivers/input/input.c文件）
在正式接触我们可爱的input core之前，有必要了解一下几个重要的结构体，这几个结构体是我们这个故事的主体。
第一个数据结构 struct input_dev。悟性高的哥们马上就会想到，它就是我们input 设备在linux内核中的模拟，即里面记录了一个input设备的所有信息。定义于linux-2.6.29/include/linux/input.h中
```
struct input_dev {
 const char *name;
       const char *phys;
       const char *uniq;
       struct input_id id;
 
       unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
       unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
       unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
       unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
       unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
       unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
       unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
       unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
       unsigned long swbit[BITS_TO_LONGS(SW_CNT)];
 
       unsigned int keycodemax;
       unsigned int keycodesize;
       void *keycode;
       int (*setkeycode)(struct input_dev *dev, int scancode, int keycode);
       int (*getkeycode)(struct input_dev *dev, int scancode, int *keycode);
 
       struct ff_device *ff;
 
       unsigned int repeat_key;
       struct timer_list timer;
 
       int sync;
 
       int abs[ABS_MAX + 1];
       int rep[REP_MAX + 1];
 
       unsigned long key[BITS_TO_LONGS(KEY_CNT)];
       unsigned long led[BITS_TO_LONGS(LED_CNT)];
       unsigned long snd[BITS_TO_LONGS(SND_CNT)];
       unsigned long sw[BITS_TO_LONGS(SW_CNT)];
 
       int absmax[ABS_MAX + 1];
       int absmin[ABS_MAX + 1];
       int absfuzz[ABS_MAX + 1];
       int absflat[ABS_MAX + 1];
 
       int (*open)(struct input_dev *dev);
       void (*close)(struct input_dev *dev);
       int (*flush)(struct input_dev *dev, struct file *file);
       int (*event)(struct input_dev *dev, unsigned int type, unsigned int code, int value);
 
       struct input_handle *grab;
 
       spinlock_t event_lock;
       struct mutex mutex;
 
       unsigned int users;
       int going_away;
 
       struct device dev;
 
       struct list_head  h_list;
       struct list_head  node;
};
```
很强大的一个结构体，因为她把所有的input设备的信息都考虑到了，真的是很无私。不过对于我们的akm驱动来说只需要关注几个小细节，结构中的加粗部分。unsigned longevbit[BITS_TO_LONGS(EV_CNT)]表示此input设备支持的事件，比如前面的第二节中的set_bit(EV_ABS, akm->input_dev->evbit)设置input_dev->evbit中的相应位让它支持绝对值坐标。类似的还有以下这些事件：EV_KEY -按键， EV_REL -相对坐标EV_ABS -绝对坐标，EV_LED -  LED，EV_FF- 力反馈。unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];设置相应的位以支持某一类绝对值坐标。比如第二节中的input_set_abs_params(akm->input_dev, ABS_RX, 0, 23040, 0, 0);它的函数体如下：
```
static inline void input_set_abs_params(struct input_dev *dev, int axis, int min, int max, int fuzz, int flat)
{
       dev->absmin[axis] = min;
       dev->absmax[axis] = max;
       dev->absfuzz[axis] = fuzz;
       dev->absflat[axis] = flat;
 
       dev->absbit[BIT_WORD(axis)] |= BIT_MASK(axis);
}
```
表示支持绝对值x坐标，并设置它在坐标系中的最大值和最小值，以及干扰值和平焊位置等。
struct list_head h_list;表示的是和该设备相关的所有input handle的结构体链表（input handle为何物下文马上会讲到）。struct list_head node;所有input设备组成的链表结构（后面将会知道它对应于input_dev_list）。
Ok 马上进入第二个结构体struct input_handler（还是来自linux-2.6.29/include/linux/input.h）
```
struct input_handler {
 
       void *private;
 
       void (*event)(struct input_handle *handle, unsigned int type, unsigned int code, int value);
       int (*connect)(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id);
       void (*disconnect)(struct input_handle *handle);
       void (*start)(struct input_handle *handle);
 
       const struct file_operations *fops;
       int minor;
       const char *name;
 
       const struct input_device_id *id_table;
       const struct input_device_id *blacklist;
 
       struct list_head  h_list;
       struct list_head  node;
};
```
顾名思义：用来处理input设备的一个结构体。struct list_head h_list表示的是和该设备相关的所有input handle的结构体链表和前面那个一样；struct list_head       node所有input_handle组成的结构体连链表（后面将会知道它对应于input_handler_list）。每一个input设备在注册时，他都会遍历input_handler_list链表上的每一个input_handler，去找寻他心中的那个她，同理每一个input_handler在注册时，他也会去input_dev_list上找寻那个属于他的她。有时候事情往往不会那么尽如人意，当input_handler还没出生时，你这个input_dev就一直在那等吧，等到天荒地老，等到海枯石烂。最后来一句，我等到花儿也谢了，你丫到底还来不来啊。注意这里的input_handler和input_dev并不是一一对应的关系，有时一个input_handler对应好几个input_dev。于是乎，作为看代码的我就在想，linux内核开发者的思想怎么这么不单纯呢，这不明摆着教育我们搞一夫多妻制吗，不过管怎样，还是得记住公司的企业文化，本分点。如果你丫说你同时拥有两个马子，我将会无情的向你抛出那句话：“出来混，迟早要还的！”。
前面多次提到那个input_handle（注意区别input_handler），她到底是何方神圣。好吧，就让我们来一层一层揭开她那神秘的面纱，当你第一次看到她完完全全展现在你面前时，那时候你的满足感和兴奋度和她的害羞度是成正比的。
同样来自linux-2.6.29/include/linux/input.h
```
struct input_handle {
 
       void *private;
 
       int open;
       const char *name;
 
       struct input_dev *dev;
       struct input_handler *handler;
 
       struct list_head  d_node;
       struct list_head  h_node;
};
```
怎么啦？是不是很失望，原来就这么回事啊。嗯，没错，兄弟，就这么回事。人往往都这样，得到了某样东西，想想觉得就那么回事，没得到呢，那叫一个好奇，那叫一个盼望。好了既然看到她的庐山真面目了，就坦然面对她，作为一个负责的男人，我还是来好好研究一下。
Input_Handle其实也好理解，它就是input_dev和 input_handler粘合剂，通过Input_Handle这么一搅和，input_dev就和 input_handler发生了点关系，至于什么样的关系我们后文将会知道的一清二楚。struct input_dev *dev对应的input设备。struct input_handler *handler对应该设备的handler。struct list_head d_node和struct list_head h_node则分别关联上前面两个结构体中的struct list_head h_list。
对应input core，前面我一直在夸她的好，对于一个大家都不认识的家伙，我这样说她，是不是显的特虚，好了，为了证明她并不是那么的虚，我不得不拿出前面第二节中出现过的两行代码来看看：
akm->input_dev = input_allocate_device()；
err = input_register_device(akm->input_dev);
没有错，这正是我们要把akm实现为一个input 设备的仅有的几行代码中的两行。
第一行，申请一个input设备：在内核中分配相应的内存空间，并初始化它。
第二行，把这个input设备注册到linux内核中，从此这个设备在内核中生根发芽，快乐幸福的和他的handler过着属于自己的小日子（虽然handler不一定属于她一个人，不过她不在乎）。
作为一个男人，我还是得负责任为我们的input core说明一下，input_allocate_device()和input_register_device();都来自我们的 input core。现在知道她的伟大了吧。你看看，我们写一个input设备驱动本来就那么几行代码，而这仅有的几行代码中还调用了来自内核的函数。我不得不说，input core ，你真给力。
好了，我们先来研究一下第一个函数 input_allocate_device()。（linux内核源码目录linux-2.6.29/drivers/input/input.c文件中）
```
struct input_dev *input_allocate_device(void)
{
       1 struct input_dev *dev;
       2 dev = kzalloc(sizeof(struct input_dev), GFP_KERNEL);
       3 if (dev) {
       4     dev->dev.type = &input_dev_type;
       5     dev->dev.class = &input_class;
       6     device_initialize(&dev->dev);
       7     mutex_init(&dev->mutex);
       8     spin_lock_init(&dev->event_lock);
       9     INIT_LIST_HEAD(&dev->h_list);
       10    INIT_LIST_HEAD(&dev->node);
 
       12    __module_get(THIS_MODULE);
       }
 
       15 return dev;
}
```
第1行，申明一个input_dev结构体变量；
第2行，kzalloc（）一个陌生的家伙，它其实等于kmalloc+memset。看到kmalloc相信大家觉得眼熟，没错，他的弟弟就是malloc，而kmalloc对应于内核空间的内存分配函数。好了，第二行代码的意思相信大家也明白了：在内核空间开辟一段大小为sizeof(struct input_dev)大小的内存区，并把它初始化为0。后面的GFP_KERNEL为分配的标志，即为一个常规的内存分配，类似的还有GFP_DMA，表示分配的内存能供dma使用，GFP_ATOMIC分配内存时，不允许睡眠，一般用在中断中，大家想想，如果在一个中断处理程序中，使用GFP_KERNEL标志分配内存，发现内存不足，就一直睡在那儿等待，你受得了吗（敲了一下键盘，发现过了2分钟系统才反应过来，相信这严重影响到了你和漂亮mm网聊的兴趣了）所以在中断处理函数中我们不能使用GFP_KERNEL标志分配内存。
分配到内存后，用dev指向这段内存。
第3行，判断内存分配是否成功，若成功，则进入到4—12行的对dev的初始化工作。否则，咱们啥也别说了，说了也白说，退出，走人。
第4到6行，对input设备的内嵌dev设备进行初始化。
第7到8行，初始化该dev的互斥量和锁，为防止对dev的并发访问。
第9、10两行，对input设备中的两个链表结构头进行初始化。
好了，此函数分析到此，一旦顺利进行，则该input设备已经出落成来一个亭亭玉立的美少女了，注意了，哥们，她现在还是单身的。接下来要做的事，嘿嘿，想必大家会比我更清楚了。网名为“洞房不败”的兄弟开口了：“难道是要把她卖出去”，兄弟高雅点行不，不叫卖，那叫嫁.
故事真正要进入高潮部分了。接下来我们来了解一下，我们前面那位美少女是在哪儿被嫁了出去的。
真正的执行者乃input_register_device（）函数。
同样来自input core中，我们来看一下她的全貌：
```
int input_register_device(struct input_dev *dev)
{
       1 static atomic_t input_no = ATOMIC_INIT(0);
       2 struct input_handler *handler;
       3 const char *path;
       4 int error;
 
       5 __set_bit(EV_SYN, dev->evbit);
 
       /*
        * If delay and period are pre-set by the driver, then autorepeating
        * is handled by the driver itself and we don't do it in input.c.
        */
 
       6 init_timer(&dev->timer);
       7 if (!dev->rep[REP_DELAY] && !dev->rep[REP_PERIOD]) {
       8     dev->timer.data = (long) dev;
       9     dev->timer.function = input_repeat_key;
       10    dev->rep[REP_DELAY] = 250;
       11    dev->rep[REP_PERIOD] = 33;
       12 }
 
       13 if (!dev->getkeycode)
       14    dev->getkeycode = input_default_getkeycode;
 
       15 if (!dev->setkeycode)
       16    dev->setkeycode = input_default_setkeycode;
 
       17 snprintf(dev->dev.bus_id, sizeof(dev->dev.bus_id),
       18    "input%ld", (unsigned long) atomic_inc_return(&input_no) - 1);
 
       19 error = device_add(&dev->dev);
       20 if (error)
       21    return error;
 
       22 path = kobject_get_path(&dev->dev.kobj, GFP_KERNEL);
       23 printk(KERN_INFO "input: %s as %s/n",
       24    dev->name ? dev->name : "Unspecified device", path ? path : "N/A");
       25 kfree(path);
 
       26 error = mutex_lock_interruptible(&input_mutex);
       27 if (error) {
       28   device_del(&dev->dev);
       29    return error;
       }
 
       30 list_add_tail(&dev->node, &input_dev_list);
 
       31 list_for_each_entry(handler, &input_handler_list, node)
       32    input_attach_handler(dev, handler);
 
       33 input_wakeup_procfs_readers();
 
       34 mutex_unlock(&input_mutex);
 
       35 return 0;
}
```
这里我在给代码表明代码行数时，为了方便，只标出了那些有代码的行，对于空行，就略了。
1-4行，初始化一些基本变量，以备后文使用。这里我们重点了解一下static atomic_t input_no = ATOMIC_INIT(0); 这里atomic_t表明一个原子变量。记住对于原子变量，不能被并发的访问，比如有两个线程，都是想让nput_no加1操作，如果input_no初值为0，当两线程串行访问，当然可以得到我们想要的值2，可以如果是并发呢，两个线程同时访问它，得到的初值都为0并同时为她加1，我们最后看到的值就只增加了一次，即为1。原子变量很好的解决了此类问题。每次只能让一个线程获得它，进行操作，释放，然后第二个线程访问，然后释放，是不是让你想到了自旋锁机制，聪明，他就是一个简化版的自旋锁，不过操作的对象是一个变量，而非一段临界区代码。
5行，设置EV_SYN，让所有的设备都支持它。
6-18行是跟按键有关的，与我们这里无关。暂且不论。
19行 error = device_add(&dev->dev);和4小结中的device_initialize(&dev->dev);夫唱妇和，共同让我们的input设备中的内嵌dev结构在系统中注册，并在sysfs文件系统中形成相应的文件层次结构。
20-25行，打印此设备在文件系统中的路径。
26、34两行代码就是为了防止其中间的临界区代码被并发访问。还记得他在哪儿初始化的吗？正是在我们第4节的input_allocate_device函数中，原句如下：mutex_init(&dev->mutex);为什么要使用互斥体呢，这个问题留到后面分析中间的临界区代码再来讨论。这里mutex_lock_interruptible（）用mutex_lock取代，前者表示该可被信号打断，后者则显然不行。
27-29行代码，如果该进程没有获取到互斥量，说明此时已有另外一个程序占有了她。哥们你天生没那命，你前面的所有工作，什么买花，什么请客看电影，都白忙活了，死心吧，放弃一切（device_del(&dev->dev);），回老家，然后慢慢的躺在家里边睡边等，等待前面那位哥们，等待他放手的那一天……
30-33行，很幸运，她还是独身，也等着你的出现，所以情同意和，两小无猜，继续牵手往下走…30行，把该设备添加到input_dev_list中，31行 遍历系统中所有已注册的handler链表即input_handler_list，从中找到和自己的Mr.Right..ok,一切顺利函数返回，成功把该input设备嫁出去，任务完成，打道回府。网名叫“西门吹牛”的兄弟要说了，好像漏了点什么？哦，对！前面说到，为什么要使用互斥量了，来防止并发访问这段代码呢？如果此时你还来问我这个问题，我就要骂你，你丫到底有没有在看我文档？
看到这里，有的哥们要生气了，怎么我们美丽的input设备被嫁出去，居然一下就忽悠过去了，都不详细描述一下她的具体被嫁过程，她到底嫁给哪个handler了？又是怎么相中的？相中后他们两又一起做了些什么？好了，为了满足这位兄弟的欲望，我们来详细阅读一下前面那个input_attach_handler(dev, handler)函数。就是在这个函数中，发生了所有该发生的事情。
```
static int input_attach_handler(struct input_dev *dev, struct input_handler *handler)
{
       1 const struct input_device_id *id;
       2 int error;
 
       3 if (handler->blacklist && input_match_device(handler->blacklist, dev))
       4     return -ENODEV;
 
       5 id = input_match_device(handler->id_table, dev);
       6 if (!id)
       7     return -ENODEV;
 
       8 error = handler->connect(handler, dev, id);
       9 if (error && error != -ENODEV)
       10    printk(KERN_ERR
       11           "input: failed to attach handler %s to device %s, "
       12           "error: %d/n",
       13           handler->name, kobject_name(&dev->dev.kobj), error);
 
       14 return error;
}
```
1行，定义一个结构体struct input_device_id的变量id。该结构体是属于handler的，它为我们的handler提供了一套择偶标准。不在标准以内了，一律拒之门外，不管你是何方妖女。
2行，定义一个整形变量 error，后面要用得到。
3行，我们的handler应该来头不小，不是富一代，也至少是个富二代。不仅有前面那一堆择偶标准，还来一个黑名单列表，对于那些在黑名单以内的设备，比如说好不容易打听到一女的，电话一通，那头突然冒出凤姐的声音。别说handler兄，对于平凡的我估计也接受不了，所以直接枪毙。如果不是凤姐，证明你不是在黑名单以内，哪怕你身高只有1米48，哪怕你有一张大嘴，哪怕你还外加一口龅牙，咱好歹给个面子先见个面聊聊，然后找理由说你不符合我的择偶标准。
4行，好了，我们的input设备美少女开始和handler兄首次见面，然后互相匹配双方信息，深入input_match_device中你会发现，最终的主动权还是在我们的handler兄这里。
```
static const struct input_device_id *input_match_device(const struct input_device_id *id,
                                                 struct input_dev *dev)
{
       int i;
 
       for (; id->flags || id->driver_info; id++) {
 
              if (id->flags & INPUT_DEVICE_ID_MATCH_BUS)
                     if (id->bustype != dev->id.bustype)
                            continue;
 
              if (id->flags & INPUT_DEVICE_ID_MATCH_VENDOR)
                     if (id->vendor != dev->id.vendor)
                            continue;
 
              if (id->flags & INPUT_DEVICE_ID_MATCH_PRODUCT)
                     if (id->product != dev->id.product)
                            continue;
 
              if (id->flags & INPUT_DEVICE_ID_MATCH_VERSION)
                     if (id->version != dev->id.version)
                            continue;
 
              MATCH_BIT(evbit,  EV_MAX);
              MATCH_BIT(keybit, KEY_MAX);
              MATCH_BIT(relbit, REL_MAX);
              MATCH_BIT(absbit, ABS_MAX);
              MATCH_BIT(mscbit, MSC_MAX);
              MATCH_BIT(ledbit, LED_MAX);
              MATCH_BIT(sndbit, SND_MAX);
              MATCH_BIT(ffbit,  FF_MAX);
              MATCH_BIT(swbit,  SW_MAX);
 
              return id;
       }
 
       return NULL;
}
```
看到没，只要我们的handler兄有某一个flag设置了，你的input设备对应的条件必须具备，否则，咱俩玩完了。深入MATCH_BIT宏：
```
#define MATCH_BIT(bit, max) /
              for (i = 0; i < BITS_TO_LONGS(max); i++) /
                     if ((id->bit[i] & dev->bit[i]) != id->bit[i]) /
                            break; /
              if (i != BITS_TO_LONGS(max)) /
                     continue;
```
还是这样，只要我们的handler兄的的id中evbit、keybit等等中的某一位设置了，input设备美眉也得具备这个条件（还记不记得我们在第二节中用set_bit(EV_ABS, akm->input_dev->evbit);为我们这个input 设备美眉制造了一个条件），否则，还是枪毙。于是乎，我开始怀疑：写linux代码的这些兄弟们原来比春哥还纯爷们，所有的想来和他相亲的女的必须要服从我这些苛刻的条件，哪怕有一个不符合，你别想和他好。不过人家确实有那个资本，能够在linux内核的世界里驰骋的人一般不简单，像国内这方面的人估计还找不上几个。还好的是，我们的input设备美眉可以遍历input_handler_list上所有的handler兄，就不相信没有一个条件稍微要求低点的。那么请问条件要求很低的这样的handler现在哪里有呢？哎！这位美眉你运气真好，这里正好有一个。
看看这位仁兄的择偶标准：
```
static const struct input_device_id evdev_ids[] = {
       { .driver_info = 1 },      /* Matches all devices */
       { },                /* Terminating zero entry */
};
```
他就一个条件，而且还是一个可以说不是条件的条件，为什么会这么说呢，请继续回到我们的input_match_device函数中，看到了吗？某些兄弟可能会惊奇的大呼一声，我的妈呀！他确实是没有要求，一没设置flag，而没设置evbit、keybit等等。所以…所以…这哥们其实最有福了，就是那些所有找不到男朋友的input设备美眉，都会和他好上，（当然凤姐是看不上他的，因为他不是北大清华的经济学硕士，他长得没有金城武帅，他身高也没有1米8……）所以就出现了传说中的一夫多妻制。
这里我们这个input设备美眉因为某些条件不够，（这里申明一下，不是我们的这个input设备条件不好，是那些其他的handler针对性实在太强了，比如说，他只要嘴角下方有颗美人痣，笑起来像张曼玉，并且还得有着像张馨予一样身材的，或者是年纪20岁左右、巨蟹座的、没谈过恋爱、还长着一张张柏芝的脸的）所以也和我们这个evdev_handler结合了。evdev_handler兄偷偷地笑了，骂其他的哥们真傻，又轻松搞定一妞。
6、7行，没找到，则返回。
8行，既然找到了，就发生上关系了，马上调用我们evdev handler中的connect函数进行联姻。在此函数中，又发生什么了呢，这个我们留到下节中讲。
9-13行，如果匹配不成功，显示打印出来。
14行，打道回府。
Handler兄果然是handler兄，很给力，这不，刚和人家好上，就有了爱情的种子。有位仁兄要问了：“他是怎么做到的呢？说出来让我学习学习一下，哥这么多年了，还是一直和自己的左手生活着”，好吧，我们就来看看事情是怎样发生的：
  没有错，就在第六节的error = handler->connect(handler, dev, id);这行代码中，发生了那么点事儿，也就是那么点事儿，让他们最终走到了一起了，有时候你不得不佩服那句话的力量“生米煮成熟饭”，不过在当今社会，貌似这话也不再那么给力了……可以看到，此行代码调用了我们这位evdev handler兄的connect函数。好了，见证奇迹的时刻到了：
```
static int evdev_connect(struct input_handler *handler, struct input_dev *dev,
                      const struct input_device_id *id)
{
       1 struct evdev *evdev;
       2 int minor;
       3 int error;
 
       4 for (minor = 0; minor < EVDEV_MINORS; minor++)
       5     if (!evdev_table[minor])
       6            break;
 
       7 if (minor == EVDEV_MINORS) {
       8     printk(KERN_ERR "evdev: no more free evdev devices/n");
       9     return -ENFILE;
       10 }
 
       11 evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);
       12 if (!evdev)
       13    return -ENOMEM;
 
       14 INIT_LIST_HEAD(&evdev->client_list);
       15 spin_lock_init(&evdev->client_lock);
       16 mutex_init(&evdev->mutex);
       17 init_waitqueue_head(&evdev->wait);
       18 snprintf(evdev->name, sizeof(evdev->name), "event%d", minor);
       19 evdev->exist = 1;
       20 evdev->minor = minor;
 
       21 evdev->handle.dev = input_get_device(dev);
       22 evdev->handle.name = evdev->name;
       23 evdev->handle.handler = handler;
       24 evdev->handle.private = evdev;
 
       25 dev_set_name(&evdev->dev, evdev->name);
       26 evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);
       27 evdev->dev.class = &input_class;
       28 evdev->dev.parent = &dev->dev;
       29 evdev->dev.release = evdev_free;
       30 device_initialize(&evdev->dev);
 
       31 error = input_register_handle(&evdev->handle);
       32 if (error)
       33    goto err_free_evdev;
 
       34 error = evdev_install_chrdev(evdev);
       35 if (error)
       36    goto err_unregister_handle;
 
       37 error = device_add(&evdev->dev);
       38 if (error)
       39 goto err_cleanup_evdev;
 
       40 return 0;
 
 41 err_cleanup_evdev:
       42 evdev_cleanup(evdev);
    43 err_unregister_handle:
       44 input_unregister_handle(&evdev->handle);
    45 err_free_evdev:
       46 put_device(&evdev->dev);
       47 return error;
48 }
```
估计兄弟你看到这么长的一个函数（其实不算很长，48行代码的函数在内核中算很短的了，当然中间省略了好多的空行），马上会拖动鼠标的滑轮一直往下走，如果你真的这么干了，说明兄弟你很有远见，因为我接下来会一行行的和你来谈论这个事件的经过。如果你没有这样，说明你是一个自由探索精神很强的哥们，好好保持，linux内核开发就需要你这样的人才。闲话不多说，我们来慢慢欣赏这小两口的那点事儿。
1-3行，定义一些基本变量，以后我们会用到的，注意这里来了一新的面孔，struct evdev *evdev，他是谁呢？这儿等会我们再讨论，先来看看这个家伙长什么样：
```
struct evdev {
       int exist;
       int open;
       int minor;
       char name[16];
       struct input_handle handle;
       wait_queue_head_t wait;
       struct evdev_client *grab;
       struct list_head client_list;
       spinlock_t client_lock; /* protects client_list */
       struct mutex mutex;
       struct device dev;
};
```
现在我们不需要搞清楚里面每一个成员的含义，到了后面你自然就明白了。就是这样，有些东西你在某一个时侯怎么想也想不明白，只要一旦到了某个阶段，再一想想，其实就那么回事。怪自己当时想的多了，考虑的太复杂了，完全没必要。所以人啊，还是要活在当下，好好做好的当前的每一件事情，未来不可遇见，也不知会有什么即将发生在你身上，比如说2012。
4-6行 注意第5行中的evdev_table[minor]数组，她是一个专门用来装evdev的数组，可以把它想象成一个摇篮，只不过这个摇篮很大，一次可以装很多个孩子。注意到34行加粗部分evdev_install_chrdev(evdev)跟踪进去：
```
static int evdev_install_chrdev(struct evdev *evdev)
{
       /*
        * No need to do any locking here as calls to connect and
        * disconnect are serialized by the input core
        */
       evdev_table[evdev->minor] = evdev;
       return 0;
}
```
这个函数很简单，把一个孩子evdev放到我们的摇篮中去。现在可以来讨论evdev这个结构是个啥东东了，没有错，他正是evdev handler 和我们那个input 设备美眉两个人在那些激情燃烧的岁月里创造出来的爱情结晶。而evdev_table[minor]这个摇篮是给他们装孩子用的。注意了，前面提过，evdev handler兄比较有福气，女人多，所以linux内核给他准备了一个很大的摇篮，不过摇篮的不同位置有标号的。记住，这位仁兄和一个女人只有一个孩子（计划生意在内核里也有体现的），不过男人毕竟在社会上还是主力军，还得为社会贡献自己的价值，所以老婆不能太多，最多也只可以有EVDEV_MINORS个女人，所以给他分配的那个摇篮最多只能装EVDEV_MINORS个孩子。好了现在回过头来看第4-6行代码。遍历整个摇篮的所有位置。找到一个没有孩子的空位，等下好放孩子。
7-10行，对不起，姐姐，你来晚了，摇篮都满了，说明什么，对，说明handler兄已经有EVDEV_MINORS个老婆了，那么对不起，虽然你们两情同意和，但咱还得遵守婚姻法，走吧，姐姐，当二奶一般转正的可能性不大，何况这位handler兄女人如云。
11-13行，很幸运，姐姐你来的够早，这儿还有位子，所以你们两个可以结婚，还可以生孩子。所以这一行代码比较关键，孩子的生命体在这里诞生了。至于怎么诞生的？这还问我？相信每一个智商正常的哥们都明白如何让他诞生，不明白的话，到两性网上去好好学习一下相关知识。12-13行，很不幸，孩子这个生命体是诞生了，不够还没出娘肚，就夭折了。这是很不幸的。
14-20行，初始化evdev的一些内部成员。比如锁，互斥量，等待队列等等。这里要注意第20行：evdev->minor = minor;把前面内核给孩子分配的摇篮位置号给孩子挂上，等下好对号入座。
21-24行，前面的三大数据结构，好像一直还有一个没出现，对！就是input_handle。这里终于出现了。这三行就是来填充这个数据结构的。input_handle怎么理解呢？对，他就是把孩子和他们的爸爸妈妈绑在一起的一个户口信息本。孩子的爸爸是谁，孩子的妈妈是谁，孩子叫什么名字，从此整个孩子的信息就到我们的handle里面去了。
25-29行，每一个dev设备，最终都要在linux系统中的/dev或者它的递归子目录生成一个文件，以后我们就可以对它进行open，read….操作了，这里几行再加上37行就是专干这事的。
31-33行 提交户口信息本给相关政府单位，让我们可爱的政府知道你们两有孩子了。
后面都是一些错误处理函数，这里就不一一分析，有兴趣的哥们可以去研究一下。
好了，该讲的都讲了，不该讲的好像也讲了。整个故事貌似结束了。如意郎君找到了，婚姻也结了，孩子也生了，户口也注册了。可是当你当回到这个故事的开头，你发现好像什么都还没开始。我们现在确实创造了一个设备文件evdevX（X代表摇篮位置编号）。可是用它来干什么呢？曾哥要说话了：七月份的尾巴，我是狮子座……这还不简单！这里的evdev就是拿来给我们应用开发者操作的，她是她妈akm这个input 设备对应的设备文件实体。怎么用她？那么我们得先搞定她老爸，evdev handler叔（毕竟有女儿的人了，不能再称兄了）。
说到evdev handler这个名字，相信大家一定再熟悉不过了，就是那个妻妾成群孩子成堆的家伙，一个让世界上很多男人都望尘莫及的家伙。嫉妒吧，羡慕吧，这些或许都已不重要，为了解解恨，我们还得想办法搞定她跟akm input设备大妈生下的那个女儿了。不过想搞定他女儿，咱得先搞定她爸。只要她爸开口了，后面的事情可能就好说了一些。不管怎样，还是来熟悉一下这位evdev handle大叔。
```
static struct input_handler evdev_handler = {
       .event             = evdev_event,
       .connect  = evdev_connect,
       .disconnect     = evdev_disconnect,
       .fops              = &evdev_fops,
       .minor            = EVDEV_MINOR_BASE,
       .name             = "evdev",
       .id_table  = evdev_ids,
};
```
其实我们对他里面的几个函数已经有所了解。比如evdev_connect()（上一节刚分析过，不要告诉我你不知道？那样我会很伤心地，）那么evdev_disconnect()顾名思义就是完成相反的动作（抛妻弃女）。.id_table     = evdev_ids（择偶标准），.name              = "evdev"孩子的姓，再加一个摇篮编号就是姓名了，显示在/dev相关目录下。.minor= EVDEV_MINOR_BASE,，还记得第7节中的那一行代码吗？evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);可以看到通过这条代码生成了设备的设备号。好了还剩下两个咚咚我们还不了解的。
即evdev_event,和evdev_fops。evdev_event是么子东西，这里不讲，留个悬念先后面我会详细道来…..我们来看evdev_fops。跟踪进去：你会惊奇的发现：
```
static const struct file_operations evdev_fops = {
       .owner           = THIS_MODULE,
       .read              = evdev_read,
       .write             = evdev_write,
       .poll        = evdev_poll,
       .open             = evdev_open,
       .release    = evdev_release,
       .unlocked_ioctl      = evdev_ioctl,
#ifdef CONFIG_COMPAT
       .compat_ioctl  = evdev_ioctl_compat,
#endif
       .fasync           = evdev_fasync,
       .flush             = evdev_flush
};
```
记忆力好的哥们马上会想到我第一节所说的话了。没错，这就是input子系统为我们提供的那些文件操作集函数函数，还记得鲁迅先生说过什么了吗？不记得的话，建议回头再看看第一节的内容，让他老人家去的安心。没错，我们的应用层开发者最终都会用到它。想泡美眉的哥们绝对要记住这个结构体了，因为只有用到它，才能让这位handler叔的女儿拜倒在你的石榴裙下。
好好把握它，你就可以想打开她的心就打开她的心（open），向往她的内心写东西就可以随便写（write），想从她内心读出很多故事，她就让你读（read），最关键的是，你可以随意操作她（evdev_ioctl）。是不是很high啊。还记得我们第二节中描述的如何关于在linux中把akm这个设备注册成一个input设备吗？不记得的话，可以回头看看。关于那短短的几行代码我们基本上分析完了，不过还有一个没有分析：
```
input_report_abs(data->input_dev, ABS_RX, rbuf[0]);
input_report_abs(data->input_dev, ABS_RY, rbuf[1]);
input_report_abs(data->input_dev, ABS_RZ, rbuf[2]);
```
等几行代码。前面提过，他就是向我们的input子系统报告发生的一些事情，然后我们的input 子系统就会把它进行处理，最终提交到我们的应用层，然后就可以用上面的evdev_fops进行读写操作了。整个过程是不是有一些些明白了。
好了，关于这几个函数是如何提交我们的设备信息的，那就要跟前面evdev_handler中那个evdev_event扯上关系了。该来的它终究还是回来的，逃也逃不掉。这就是我们下一节要讨论的东东。
这里我们只以其中的一行为例来分析一下如何给我们的input子系统上报事件。
input_report_abs(data->input_dev, ABS_RX, rbuf[0]);继续跟踪：
```
static inline void input_report_abs(struct input_dev *dev, unsigned int code, int value)
{
       input_event(dev, EV_ABS, code, value);
}
```
可以看到，该函数也是只调用了一个input_event函数。网名为“拉登他爹”要骂人了：“为什么一行代码也来实现一个函数，这不是忽悠老子吗？”兄弟，你要体会我们那些写linux内核代码的哥们的用心良苦（你说你想要逃，偏偏注定要落脚 ，情灭了爱熄了 ，剩下空心要不要 ，春已走花又落 ，用心良苦却成空 ，我的痛怎么形容 ，一生爱错放你的手 。张宇大哥很经典的一首歌）他们这么做完全是希望同一个函数能同时被多个其他函数调用，达到代码复用的作用，比如这里input_event(dev, EV_ABS, code, value);根据第二个参数的不同来进行不同的处理来。下次兄弟你又要处理一个按键事件，我们就直接可以这样：
```
static inline void input_report_key(struct input_dev *dev, unsigned int code, int value)
{
        input_event(dev, EV_KEY, code, value);
}
```
好了，废话不多说，继续跟踪进来：
```
void input_event(struct input_dev *dev,  unsigned int type, unsigned int code, int value)
{
       1 unsigned long flags;
 
       2 if (is_event_supported(type, dev->evbit, EV_MAX)) {
 
       3     spin_lock_irqsave(&dev->event_lock, flags);
       4     add_input_randomness(type, code, value);
       5     input_handle_event(dev, type, code, value);
       6     spin_unlock_irqrestore(&dev->event_lock, flags);
       }
}
```
终于看到一个还像个函数的函数了，至少有那么6行代码。兄弟，其实你我都应该感到很幸福。Linux内核中达到几百行代码代码的函数比比皆是。
1行，定义一个标识符，等下会用到。
2行，判断我们的input 设备中的evbit中是否设置了EV_ABS这个标识，以表示支持绝对值坐标。回想第二节中的set_bit(EV_ABS, akm->input_dev->evbit);没有错，我们是有设置的，所以可以往下走。
3、6两行，为我们的临界区代码加个自旋锁，防止并发访问。细心的哥们可能会发现，这和我们平时的自旋锁有些不同，一般的自旋锁都是spin_lock()和spin_unlock()成对出现的。这里好像多长了个尾巴。没错，男人和女人的某些区别好像也在这里，不好意思，思想又游离了…继续回来，我们这个自旋锁条件更苛刻。一般的自旋锁保护临界区代码不受其他cpu和本cpu的抢占进程打扰。这两个长尾巴的哥们更狠，他连我们的中断也一并拒之门外。总之，通告天下，被我占有的东西，你天王老子都别想再碰，除非我释放她了。够狠吧！是的，男人就应该对自己狠一点。补充一句：前面定义的那个flag就是用来保存被我们拒之门外的那个中断的状态信息的。
4行，为内核产生随机因子。
5行，真正有悟性的哥们应该扫一眼这几行代码就会发现，这一行才是真正的国家宝藏所在，难道这就是所谓的天赋，你不得不佩服尼古拉斯凯奇的犀利，凭着那么一点点信息就可以发现那么大的宝藏。不过现实生活中这样事情发生的概率就好比张曼玉突然为了你和她的外籍男友分手，然后来到长安，为的就是能默默守侯在你身边。继续跟进input_handle_event(dev, type, code, value)：
又是一来自 input core的函数。
```
static void input_handle_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
       int disposition = INPUT_IGNORE_EVENT;
 
       switch (type) {
 
       case EV_SYN:
              switch (code) {
              case SYN_CONFIG:
                     disposition = INPUT_PASS_TO_ALL;
                     break;
 
              case SYN_REPORT:
                     if (!dev->sync) {
                            dev->sync = 1;
                            disposition = INPUT_PASS_TO_HANDLERS;
                     }
                     break;
                    
              }
              break;
 
       case EV_KEY:
              if (is_event_supported(code, dev->keybit, KEY_MAX) &&
                  !!test_bit(code, dev->key) != value) {
 
                     if (value != 2) {
                            __change_bit(code, dev->key);
                            if (value)
                                   input_start_autorepeat(dev, code);
                     }
 
                     disposition = INPUT_PASS_TO_HANDLERS;
              }
              break;
 
       case EV_SW:
              if (is_event_supported(code, dev->swbit, SW_MAX) &&
                  !!test_bit(code, dev->sw) != value) {
 
                     __change_bit(code, dev->sw);
                     disposition = INPUT_PASS_TO_HANDLERS;
              }
              break;
 
       case EV_ABS:
       1     if (is_event_supported(code, dev->absbit, ABS_MAX))
      2     {
 
      3            value = input_defuzz_abs_event(value,
      4                          dev->abs[code], dev->absfuzz[code]);
                   
      5                   if (dev->abs[code] != value)
      6                   {
                                  dev->abs[code] = value;
      7                          disposition = INPUT_PASS_TO_HANDLERS;
      8                   }
      9            }
      10   break;
 
       case EV_REL:
              if (is_event_supported(code, dev->relbit, REL_MAX) && value)
                     disposition = INPUT_PASS_TO_HANDLERS;
 
              break;
 
       case EV_MSC:
              if (is_event_supported(code, dev->mscbit, MSC_MAX))
                     disposition = INPUT_PASS_TO_ALL;
 
              break;
 
       case EV_LED:
              if (is_event_supported(code, dev->ledbit, LED_MAX) &&
                  !!test_bit(code, dev->led) != value) {
 
                     __change_bit(code, dev->led);
                     disposition = INPUT_PASS_TO_ALL;
              }
              break;
 
       case EV_SND:
              if (is_event_supported(code, dev->sndbit, SND_MAX)) {
 
                     if (!!test_bit(code, dev->snd) != !!value)
                            __change_bit(code, dev->snd);
                     disposition = INPUT_PASS_TO_ALL;
              }
              break;
 
       case EV_REP:
              if (code <= REP_MAX && value >= 0 && dev->rep[code] != value) {
                     dev->rep[code] = value;
                     disposition = INPUT_PASS_TO_ALL;
              }
              break;
 
       case EV_FF:
              if (value >= 0)
                     disposition = INPUT_PASS_TO_ALL;
              break;
 
       case EV_PWR:
              disposition = INPUT_PASS_TO_ALL;
              break;
       }
 
       if (disposition != INPUT_IGNORE_EVENT && type != EV_SYN)
              dev->sync = 0;
 
       if ((disposition & INPUT_PASS_TO_DEVICE) && dev->event)
              dev->event(dev, type, code, value);
 
       if (disposition & INPUT_PASS_TO_HANDLERS)
              input_pass_event(dev, type, code, value);
}
```
有兄弟要发话了：“好长的一堆代码“，是的，我承认，这是我这篇文档到目前为止最长的一个函数。不够在大学里面稍微学了一点点C语言的哥们应该都不觉得它复杂，否则你就将无言见江东父老，更对不起谭浩强大哥。整个函数先一个switch。然后根据不同的case进行不同的处理，得到不同的disposition变量值。然后，然后就到整个代码的最后六行代码了，是不是简单的一B。这里我们传进来的参数正好是EV_ABS。所以我们只需把注意力放到代码中的加粗部分。
1行，判断我们的input 设备中的absbit中是否设置了ABS_RX，显然前面我们已经设置了不信你回过头来看看第二节中那个曾经没有讲过的函数。相信哥们你的c语言代码阅读能力绝对在我之上，区区几行代码怎么能唬得了你。
3-4行，进行误差校准。这里我初始化时设置为0。故其实啥也没干，星星还是那颗星星哟,月亮还是那个月亮, value也还是那个value。
5-7行，把value的值付给dev->abs[code]，dev->abs[code]记录的是上一次该坐标值。如果是第一次，显然为0.所以最终我们得到了disposition = INPUT_PASS_TO_HANDLERS。默默地做了那么多前期工作为的就是得到她。现在终于如愿以偿，带着她从此开始浪迹天涯…..
浪到哪儿去了呢？聪明的你马上发现了浪到了最后两行代码。太阳依然发光，地球依然转动，你依然爱着心中的那个她，故事依然还在继续，input_pass_event(dev, type, code, value)也依然还在传值，他到底要把那个我们收集到的type、code、value传到哪儿去呢？
废话不多讲，开门见山，我们继续上节那个没有完的故事，我们收集到的那些值到底会传到哪里去呢？深入input_pass_event(dev, type, code, value)内部，看她花落谁家？
```
static void input_pass_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
       1  struct input_handle *handle;
 
       2  rcu_read_lock();
 
       3  handle = rcu_dereference(dev->grab);
       4  if (handle)
       5     handle->handler->event(handle, type, code, value);
       6   else
       7     list_for_each_entry_rcu(handle, &dev->h_list, d_node)
       8            if (handle->open)
       9                   handle->handler->event(handle,
       10                                       type, code, value);
       11    rcu_read_unlock();
}
```
1行，定义一个input_handle结构体变量，不会这么快就忘了input_handle这位兄弟吧？
2、11两行是一种 RCU形式的锁机制。它允许多个执行单元对它同时对它进行读，但是多个执行单元对它进行写操作时，并不会让你随随便便就碰她，它会要求你先复制一个副本，要改要删随你便，反正你改的不是她本身。当所有的写单元都一一改完后，再统一写回，是不是比一般的自旋锁高级多了。不过不要轻易用它，系统开销较大。
3-5行，看看dev->grab有没有设置，回顾前面dev的初始化。显然没动它。所以这几行代码就这样废了。
7-10行，遍历dev->h_list链表上的handle，看看有没有handle->open不为0的，这里在我们在用户空间通过open函数打开某个evdev的时候，相应的handle->open会++，这里显然我们要打开evdev设备来对input设备进行操作，才会发生前面那些那儿，要不然我说了这么多不是等于一个0.？
第9行代码，我们通过这个handle户口信息表找到孩子的爸爸然后调用孩子爸爸里面的函数evdev_event。
我没有食言！记性好的哥们还记不记得，我们在第8节中了解evdev_handler的时候，有一个函数没讲，正是此函数。我们再来看一下evdev_handler叔的全貌：
```
static struct input_handler evdev_handler = {
       .event             = evdev_event,
       .connect  = evdev_connect,
       .disconnect     = evdev_disconnect,
       .fops              = &evdev_fops,
       .minor            = EVDEV_MINOR_BASE,
       .name             = "evdev",
       .id_table  = evdev_ids,
};
```
现在是时候来分析这个evdev_event,的时候了，为了成功泡上他的美丽女儿，我们还是有必要来攻下这最后一座城池：
```
static void evdev_event(struct input_handle *handle,
                     unsigned int type, unsigned int code, int value)
{
       1 struct evdev *evdev = handle->private;
       2 struct evdev_client *client;
       3 struct input_event event;
       4 struct timespec ts;
 
       5 ktime_get_ts(&ts);
       6 event.time.tv_sec = ts.tv_sec;
       7 event.time.tv_usec = ts.tv_nsec / NSEC_PER_USEC;
       8 event.type = type;
       9 event.code = code;
       10 event.value = value;
 
       11 rcu_read_lock();
 
       12 client = rcu_dereference(evdev->grab);
       13 if (client)
       14    evdev_pass_event(client, &event);
       15 else
       16    list_for_each_entry_rcu(client, &evdev->client_list, node)
       17           evdev_pass_event(client, &event);
 
       18 rcu_read_unlock();
 
       19 wake_up_interruptible(&evdev->wait);
}
```
1行，大家还记不记得，那个时候，evdev handler兄和我们的akm input设备美眉喜得贵女后，带着孩子去政府部门注册户口信息时，就是把他们的女儿交给了handle->private。这里把她再提取出来，赋给我们的struct evdev *evdev，没有疑问吧！
2行，又来一个新的数据结构struct evdev_client *client;，先看看她的内部结构：
```
struct evdev_client {
       struct input_event buffer[EVDEV_BUFFER_SIZE];
       int head;
       int tail;
       spinlock_t buffer_lock; /* protects access to buffer, head and tail */
       struct fasync_struct *fasync;
       struct evdev *evdev;
       struct list_head node;
       struct wake_lock wake_lock;
};
```
眼神犀利的哥们马上会看到很关键的一行代码：struct evdev *evdev;没有错，她内嵌了一个evdev结构，怎么理解呢？这就好比我们把mtk、高通、或者Marvel厂商那儿提供给我们的整个手机设计方案搬过来，改改代码，添加点应用程序，加上一个漂亮的外壳，再来一个时尚的品牌商标，一款时尚的手机就出炉了，还卖的很好哦。对，这里的struct evdev_client就是我们那个evdev的pvt版本，她是和我们的evdev紧紧地绑定在一起的，在哪儿绑定的呢？作为一个负责任的男人，我得告诉你，还是发生在应用层调用open时发生的。关于这个open整个过程我们下一节会详细讲解。不过她确实多了几个很重要的东东：struct input_event buffer[EVDEV_BUFFER_SIZE];定义一个内核区buffer。它用来干嘛？input_event结构是什么样？请看接下来的第三行代码。
3行，世界上谁跑的最快？“当然是input_event了”凤姐很得意的回答道。凤姐智商果然超群。说input_event，input_event到。我们来看看这个家伙长什么样：
```
struct input_event {
       struct timeval time;
       __u16 type;
       __u16 code;
       __s32 value;
};
```
她的作用很明显，就是用来存储我们要传递的那些值type、code、value。不信，不信请往下看。
4-7行，跟时间片有关的一些东东，和我们的这个akm 设备无关。不过handler兄老婆多，总有一个用得上的。
8-10行，把通过参数传进来的ype、code、value放到我们的event中。是不是验证了前面的说法。
11、18两行，又是加锁操作。前面有聊过，飘过。
12-14行，看看这个client有没有被强制绑定，这里我们没绑定，故略过。
16-17行，遍历client链表，找到与我们对应的那个client，然后把刚刚那个event放到里面去。放到哪里面去呢？放到struct input_event buffer[EVDEV_BUFFER_SIZE]里面去，注意最多可放EVDEV_BUFFER_SIZE个event。
进入evdev_pass_event(client, &event);跟踪：
```
static void evdev_pass_event(struct evdev_client *client,
                          struct input_event *event)
{
       /*
        * Interrupts are disabled, just acquire the lock
        */
       spin_lock(&client->buffer_lock);
       wake_lock_timeout(&client->wake_lock, 5 * HZ);
       client->buffer[client->head++] = *event;
       client->head &= EVDEV_BUFFER_SIZE - 1;
       spin_unlock(&client->buffer_lock);
 
       kill_fasync(&client->fasync, SIGIO, POLL_IN);
}
```
等下我们的用户程序就可以通过一定的方式（read（）系统调用）来从这个buffer中拷贝数据了。这样一次从最下层驱动通过input子系统传到内核区buffer，然后再到用户空间的数据传输就这样完成了。
好了，貌似故事就这样结束了，数据终于传完了。整个handler叔全弄明白了，既然这样，那还等什么，赶紧操着家伙evdev_fops,去把玩他的美丽女儿吧。不过兄弟先别急，再去之前，有两个小小的注意事项需要交代一下，以防不测：
1，    上面的evdev_event()函数的最后一行代码wake_up_interruptible(&evdev->wait);是用来唤醒一个等待队列，他的另一半是wait_event_interruptible(&evdev->wait)，她一般位于另外一个地方。比如：
int fun1（）
{
……
……
wake_up_interruptible(&evdev->wait);
}
int fun2()
{
……
……
wait_event_interruptible(&evdev->wait)；
……
……
}
函数fun2执行到wait_event_interruptible(&evdev->wait)这一句时，她就不走了，干嘛？睡个觉先，等待她的另一半的到来，然后把她唤醒。这就需要我们的fun1函数执行到wake_up_interruptible(&evdev->wait);明白了吧！有哥们要发话了，这个机制有什么作用啊？
好吧，举个例子：两个函数对一buffer进行操作，一个读她，一个写她。要读的那个哥们是不是要先等那个要写的哥们把数据写到里面去了再读呢？否则，你去读空气啊？
好了，这里在我们这个evdev_event()函数来这么一个wake_up_interruptible(&evdev->wait);干嘛，表示你的数据已经好了，那位想读的哥们赶紧过来读吧，再不读就被别人读走了哦！
2我们通过input_report_abs(data->input_dev, ABS_RZ, rbuf[2]);最开始传输数据的，可是rbuf[2])哪来的呢？具体过程是这样的：akm8973传感器是通过I2C总线挂到系统中的，它不断地从外界收集数据（比如各坐标信息）然后把他们送到自己的寄存器中，然后我们就通过I2C总线从它的寄存器中获取数据，最终放到我们上面的rbuf[2]中。接下来就发生了上面的那一切，致使这些数据最终被传到我们的用户空间被我的应用程序处理和显示，哟！从手机的GPS导航可以发现，兄弟你现在位于广东省东莞市长安镇乌沙区大润发旁不远处的一个神秘地方，哟！显示了，是一个叫海豚湾的地方！
没错，到目前为止，akm input设备也注册了，evdev handler大叔的全貌也搞明白了，他们的女儿也亭亭玉立了……是不是该干正事的时候了？
好吧，我们就来看看如何来干这份正事：
还是列出那份操作集函数（handler大叔为我们这些笑的最后的弟兄精心准备的泡妹妹大法，不过现实生活中可没有这样的爸爸）：
```
static const struct file_operations evdev_fops = {
       .owner           = THIS_MODULE,
       .read              = evdev_read,
       .write             = evdev_write,
       .poll        = evdev_poll,
       .open             = evdev_open,
       .release    = evdev_release,
       .unlocked_ioctl      = evdev_ioctl,
#ifdef CONFIG_COMPAT
       .compat_ioctl  = evdev_ioctl_compat,
#endif
       .fasync           = evdev_fasync,
       .flush             = evdev_flush
};
```
还记得吗？这就是我们精心填表后得到的那个u盾。
要了解evdev，咱先得打开她的心扉，来了解了解她。记得一位情圣曾经说过的一句话：要获得一位美女的好感，要做的第一件事就是能打开她的话匣子，要聊那些她感兴趣的事儿，彻底打开她的心扉，这样她就会对你产生信任感….接着呢？接着就不说了，有兴趣的兄弟可以找我聊聊（研发楼二楼进门就可以看到一个长的不是很搓的瘦瘦哥们）。
好了，在应用层，我们可以通过调用open函数然后转到我们这个操作集函数中的.open  = evdev_open,。是不是很简单。嗯，确实简单的一B。继续，看看具体打开过程：
```
static int evdev_open(struct inode *inode, struct file *file)
{
       1 struct evdev *evdev;
       2 struct evdev_client *client;
       3 int i = iminor(inode) - EVDEV_MINOR_BASE;
       4 int error;
 
       5 if (i >= EVDEV_MINORS)
       6     return -ENODEV;
 
       7 error = mutex_lock_interruptible(&evdev_table_mutex);
       8 if (error)
       9 return error;
       10 evdev = evdev_table[i];
       11 if (evdev)
       12    get_device(&evdev->dev);
       13 mutex_unlock(&evdev_table_mutex);
 
       14 if (!evdev)
       15    return -ENODEV;
 
       16 client = kzalloc(sizeof(struct evdev_client), GFP_KERNEL);
       17 if (!client) {
       18    error = -ENOMEM;
       19    goto err_put_evdev;
       20 }
 
       21 spin_lock_init(&client->buffer_lock);
       22 wake_lock_init(&client->wake_lock, WAKE_LOCK_SUSPEND, "evdev");
       23 client->evdev = evdev;
       24 evdev_attach_client(evdev, client);
 
       25 error = evdev_open_device(evdev);
       26 if (error)
       27   goto err_free_client;
 
       28 file->private_data = client;
       29 return 0;
 
30 err_free_client:
       31   evdev_detach_client(evdev, client);
       32  kfree(client);
    33  err_put_evdev:
       34   put_device(&evdev->dev);
       35 return error;
}
```
俗话说：话说三遍淡如水,再说三遍打驴嘴。相信看过我前面章节的哥们要来读懂这个函数，简直是 a piece of cake。不过还是有几行代码需要提一提：
3、10两行，第3行：通过设备节点号找到摇篮位置编号；第10行：通过这个摇篮编号找到那个孩子，对了，已经不是孩子了，是个美少女了。
16行，创建struct evdev_client，还记得吗，她是在struct evdev的基础上改造出来的品牌机。再三强调：不是山寨。
24行，进行绑定。
25行，关键的25行，怎么关键了，跟踪进去：
```
static int evdev_open_device(struct evdev *evdev)
{
       int retval;
 
       retval = mutex_lock_interruptible(&evdev->mutex);
       if (retval)
              return retval;
 
       if (!evdev->exist)
              retval = -ENODEV;
       else if (!evdev->open++) {
              retval = input_open_device(&evdev->handle);
              if (retval)
                     evdev->open--;
       }
 
       mutex_unlock(&evdev->mutex);
       return retval;
}
```
眼神不是很差的哥们马上会看到，有一行代码加粗了，他就是我们这个函数的核心。就好比小时候老师经常让我们写文章段落大意，找中心句子，如果找不出来，就去操场罚跑步。天生缺乏语言细胞，那时的我步可没少跑，连操场上捡垃圾的大妈都认识我了，偶尔一天侥幸过关没去跑，第二天大妈会说“小娃子，昨天是不是没来上课啊”。估计长不胖的一个重大原因就是小时候语文课上中心句子找少了。搞得现在一看到一堆代码，就想找出那个中心来。
好了继续跟踪进来，看看input_open_device(&evdev->handle)这个中心干了什么事情：
```
int input_open_device(struct input_handle *handle)
{
       struct input_dev *dev = handle->dev;
       int retval;
 
       retval = mutex_lock_interruptible(&dev->mutex);
       if (retval)
              return retval;
 
       if (dev->going_away) {
              retval = -ENODEV;
              goto out;
       }
 
       handle->open++;
 
       if (!dev->users++ && dev->open)
              retval = dev->open(dev);
 
       if (retval) {
              dev->users--;
              if (!--handle->open) {
                     /*
                      * Make sure we are not delivering any more events
                      * through this handle
                      */
                     synchronize_rcu();
              }
       }
 
 out:
       mutex_unlock(&dev->mutex);
       return retval;
}
```
我只能说：兄弟你太有才了，马上又一眼看穿中心句了；对就这里这个加粗部分：
handle->open++;。哥们对它有没有印象，不记得的话，打开第10节，看里面的第一段函数中的加粗部分，就是因为它，我们的数据才得以传递下去……
28行 file->private_data = client;把这个client付给文件的私有数据.
前面已经打开她的心扉了。某位脑残的兄弟问：“打开之后干嘛呢”，还能干嘛呢？当然是读了，好，下来我们分析操作集中的第二个函数：evdev_read（）深入跟踪下去：
```
static ssize_t evdev_read(struct file *file, char __user *buffer,
                       size_t count, loff_t *ppos)
{
       1 struct evdev_client *client = file->private_data;
       2 struct evdev *evdev = client->evdev;
       3 struct input_event event;
       4 int retval;
 
       5 if (count < input_event_size())
       6     return -EINVAL;
 
       7 if (client->head == client->tail && evdev->exist &&
       8   (file->f_flags & O_NONBLOCK))
       9     return -EAGAIN;
 
       10 retval = wait_event_interruptible(evdev->wait,
       11    client->head != client->tail || !evdev->exist);
       12 if (retval)
       13    return retval;
 
       14 if (!evdev->exist)
       15   return -ENODEV;
 
       16 while (retval + input_event_size() <= count &&
       17       evdev_fetch_next_event(client, &event)) {
 
       18    if (input_event_to_user(buffer + retval, &event))
                     return -EFAULT;
 
       19    retval += input_event_size();
       }
 
       20 return retval;
}
```
相信在座的各位，现在分析这个函数也不会有什么困难。
1行，把在evdev_open里那个私有数据放到struct evdev_client *client中。
5-6行，如果要copy的字节数少于一个event的大小，对不起，结束吧。不完整的event对我们来说没什么用，别浪费表情。
10-11两行，wait_event_interruptible(evdev->wait,client->head != client->tail || !evdev->exist);
还记得她吗？是的，她已经在这里睡了好久，等待她的的如意郎君把她唤醒。后面的client->head != client->tail || !evdev->exist为条件，条件必须满足，她才会醒来。
16-19行，每次从client的buffer中取出一个input event数据放到我们这里的event中，然后把它传到应用层的buffer中，retval记录总共返回的字节数。深入input_event_to_user(buffer + retval, &event)：
```
int input_event_to_user(char __user *buffer,
                     const struct input_event *event)
{
       if (copy_to_user(buffer, event, sizeof(struct input_event)))
              return -EFAULT;
 
       return 0;
}
```
函数copy_to_user就是把一个内核空间buffer放到我们的用户空间buffer，他的兄弟copy_from_user完成相反的动作。
好了，数据都已经到我们的用户空间了，现在我们想干嘛就可以干嘛了……
至于函数集中的evdev_write、evdev_ioctl等函数我就不一一分析了，怎么去往她心里面写东西，怎么去操作她，这些东西听起来是不是有点yellow，我就不好意思再和你一起探讨了。这些东西哥们你要自己慢慢去摸索，俗话说的好：“自己动手，丰衣足食”。
到此，整个系列都已分析完了，发现自己理解和分析代码是一回事，写出来是另外一回事。还是那句话：兄弟们，我们要多实践啊，无论是工作还是把妹，你不去实践，光yy的话，效果还是是相差的很远的。
 转自：[http://blog.csdn.net/lmm670/article/details/6080998](http://blog.csdn.net/lmm670/article/details/6080998)
