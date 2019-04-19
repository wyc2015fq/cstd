# Android下Vold守护进程分析 - xqhrs232的专栏 - CSDN博客
2011年12月05日 17:45:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：835标签：[android																[socket																[action																[struct																[存储																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.chineselinuxuniversity.net/freesky/viewthread.php?tid=41](http://www.chineselinuxuniversity.net/freesky/viewthread.php?tid=41)
vold的全称是volume daemon。实际上是负责完成系统的CDROM, USB大容量存储，MMC卡等扩展存储的挂载任务自动完成的守护进程。它提供的主要特点是支持这些存储外设的热插拔。这里有GNU/Linux vold的介绍［[http://vold.sourceforge.net/](http://vold.sourceforge.net/)］。在Android上的这个vold系统和GNU/Linux的之间存在很大的差异，这里我们主要是分析Android上的vold系统的处理过程。
Vold处理过程大致分为三步：
1.创建链接：
在vold作为一个守护进程，一方面接受驱动的信息，并把信息传给应用层；另一方面接受上层的命令并完成相应。所以这里的链接一共有两条：
（1）vold socket: 负责vold与应用层的信息传递；
（2）访问udev的socket: 负责vold与底层的信息传递；
这两个链接都是在进程的一开始完成创建的。
2.引导：
这里主要是在vold启动时，对现有外设存储设备的处理。首先，要加载并解析vold.conf，
并检查挂载点是否已经被挂载（注：这里检查挂载点的用意不是很清楚！）； 其次，执行MMC卡挂载； 最后，处理USB大容量存储。
3.事件处理：
这里通过对两个链接的监听，完成对动态事件的处理，以及对上层应用操作的响应。
我们来具体分析一下代码过程。我们以带着mmc卡开机这种情况为例，看看vold的启动和处理过程。我们从vold的主函数的部分实现开始：
首先是创建两个socket。首先创建的是vold与上层应用程序的链接。
    // Socket to listen on for incomming framework connections
    if ((door_sock = android_get_control_socket(VOLD_SOCKET)) < 0) {
        LOGE("Obtaining file descriptor socket '%s' failed: %s",
             VOLD_SOCKET, strerror(errno));
        exit(1);
    }
door_sock就是对VOLD_SOCKET监听的文件描述符。这里VOLD_SOCKET就是在init.rc中启动vold服务的时候创建的 socket设备文件。上层的应用层——MountListener，这个服务会连接vold socket——作为客户端，然后通过这个连接和vold之间进行信息和命令的交互。当有client连接进这个vold socket时，监听door_sock的vold就会生成自己的socket描述符fw_sock，并通过它来传送和接受上层信息。
然后，创建vold与底层的信息交互的socket。
    if ((uevent_sock = socket(PF_NETLINK,
                             SOCK_DGRAM,NETLINK_KOBJECT_UEVENT)) < 0) {
        LOGE("Unable to create uevent socket: %s", strerror(errno));
        exit(1);
    }
这里使用 NETLINK_KOBJECT_UEVENT的socket去访问并获得udev的信息。关于udev，这是一个Linux内核的设备管理模块，网上有详细资料。这里我们只要知道设备的热插拔信息是从这个模块中获得的。
下边介绍引导（Boot Strap）过程。 首先，执行volmgr_bootstrap()，对vold.conf文件进行解析。这个文件主要包含了如下的信息，我们使用模拟器中的默认的 vold.conf为例。
volume_sdcard {
    ## This is the direct uevent device path to the SD slot on the device
    emu_media_path /devices/platform/goldfish_mmc.0/mmc_host/mmc0
    media_type     mmc
    mount_point    /sdcard
    ums_path       /devices/platform/usb_mass_storage/lun0
}
然后把解析之后的信息保存在一个全局的结构变量里，并读取/proc/mounts信息，检查挂载点是否已经被挂载。这里只有一个mmc卡槽，所以只定义了一个挂载点。
接着是mmc_bootstrap()函数。这里有一连串调用和处理，主要是针对目录来作的。调用关系如下，
mmc_bootstrap() → mmc_bootstrap_controller() → mmc_bootstrap_card()
经过这几步之后，在mmc_bootstrap_card()调用中，我们已经进入文件夹 /sys/devices/platform/goldfish_mmc.0/mmc_host/mmc0/mmc0:e118下边了。这里，就是我们当前插槽中的mmc卡存放设备信息的地方，然后获得一些相关的信息，这些信息为之后的uevent的生成作准备。我们简单说一下uevent事件处理系统。下面是代码中的dispatch_table全局变量。
static struct uevent_dispatch dispatch_table[] = {
    { "switch", handle_switch_event },
    { "battery", handle_battery_event },
    { "mmc", handle_mmc_event },
    { "block", handle_block_event },
    { "bdi", handle_bdi_event },
    { "power_supply", handle_powersupply_event },
    { NULL, NULL }
};
这里主要存储了不同的设备的uevent的不同的处理方式。通过dispatch_event()函数，来获得相应事件的名字，并找到，执行相应的操作。 dispatch_table全局变量保存了所有事件与处理句柄的对应。dispatch_event()中，正是遍历这个全局表来完成相应的调用。而 dispatch_event()，需要一个struct uevent*，也就是uevent指针作为参数。这里，uevent就是事件的结构体，定义如下。
struct uevent {
    char *path;
    enum uevent_action action;
    char *subsystem;
    char *param[UEVENT_PARAMS_MAX];
    unsigned int seqnum;
};
对 dispatch_event()的调用在vold主要有两种方式，一种是通过捕捉udev的底层消息，然后执行 process_uevent_message()来执行dispatch_event()；另一种就是在Boot strap期间调用的simulate_uevent()函数，开辟内存并通过参数生成一个uevent，然后执行dispatch_event()。在我们的mmc卡的引导过程中，一共需要调用若干次的simulate_uevent()函数。这个函数中，会根据参数，生成并初始化一个uevent实例，再把这个实例作为参数传给dispatch_event()函数，来完成事件的执行过程。下面我们回到mmc卡的引导过程，结合这个过程看看
 simulate_uevent()函数的工作。
我们把虚拟机的文件目录下的配置看一下。
# pwd
/sys/devices/platform/goldfish_mmc.0/mmc_host/mmc0/mmc0:e118
# ls -l
-rw-r--r-- root     root         4096 2009-06-09 11:01 uevent
-r--r--r-- root     root         4096 2009-06-09 11:10 cid
-r--r--r-- root     root         4096 2009-06-09 11:10 csd
-r--r--r-- root     root         4096 2009-06-09 11:10 scr
-r--r--r-- root     root         4096 2009-06-09 11:10 date
-r--r--r-- root     root         4096 2009-06-09 11:10 fwrev
-r--r--r-- root     root         4096 2009-06-09 11:10 hwrev
-r--r--r-- root     root         4096 2009-06-09 11:10 manfid
-r--r--r-- root     root         4096 2009-06-09 11:01 name
-r--r--r-- root     root         4096 2009-06-09 11:10 oemid
-r--r--r-- root     root         4096 2009-06-09 11:01 serial
-r--r--r-- root     root         4096 2009-06-09 11:01 type
lrwxrwxrwx root     root              2009-06-09 11:10 subsystem -> ../../../../../../bus/mmc
drwxr-xr-x root     root              2009-06-09 11:01 power
lrwxrwxrwx root     root              2009-06-09 11:10 driver -> ../../../../../../bus/mmc/drivers/mmcblk
drwxr-xr-x root     root              2009-06-09 11:01 block
这里边的文件存放的各种mmc卡的类型，名字等信息。我们再看一段mmc_bootstrap_card()函数的一段代码：
// file: mmc.c
static int mmc_bootstrap_card(char *sysfs_path)
｛
    … ...
    sprintf(tmp, "DEVPATH=%s", devpath);
    uevent_params[0] = (char *) strdup(tmp);
    sprintf(filename, "/sys%s/type", devpath);
    p = read_file(filename, &sz);
    p[strlen(p) - 1] = '\0';
    sprintf(tmp, "MMC_TYPE=%s", p);
    free(p);
    uevent_params[1] = (char *) strdup(tmp);
    sprintf(filename, "/sys%s/name", devpath);
    p = read_file(filename, &sz);
    p[strlen(p) - 1] = '\0';
    sprintf(tmp, "MMC_NAME=%s", p);
    free(p);
    uevent_params[2] = (char *) strdup(tmp);
    uevent_params[3] = (char *) NULL;
    if (simulate_uevent("mmc", devpath, "add", uevent_params) < 0) {
        LOGE("Error simulating uevent (%m)");
        return -errno;
    }
    … …
｝
通过simulate_uevent()的参数，传入uevent结构体中。这里，执行的是一个mmc卡的加载信息。我们看到char* uevent_params[4]中存储了mmc卡的路径，类型，名字等信息。
并将其传入simulate_uevent()。我们进入simulate_uevent()函数看看。
int simulate_uevent(char *subsys, char *path, char *action, char **params)
{
    struct uevent *event;
    char tmp[255];
    int i, rc;
    if (!(event = malloc(sizeof(struct uevent)))) {
        LOGE("Error allocating memory (%s)", strerror(errno));
        return -errno;
    }
    memset(event, 0, sizeof(struct uevent));
    event->subsystem = strdup(subsys);
    if (!strcmp(action, "add"))
        event->action = action_add;
    else if (!strcmp(action, "change"))
        event->action = action_change;
    else if (!strcmp(action, "remove"))
        event->action = action_remove;
    else {
        LOGE("Invalid action '%s'", action);
        return -1;
    }
    event->path = strdup(path);
    for (i = 0; i < UEVENT_PARAMS_MAX; i++) {
        if (!params)
            break;
        event->param = strdup(params);
    }
    rc = dispatch_uevent(event);
    free_uevent(event);
    return rc;
}
我们看到，simulate_uevent()函数生成并根据参数初始化，最后，调用dispatch_uevent()去执行这个模拟事件。
处理函数dispatch_uevent()调用会根据名字，这里会调用 handle_mmc_event()进行处理。实际上，这个处理过程并没有加载mmc卡到/sdcard挂载点上。而挂载过程，还在下边。:-) 我们继续分析。
处理完这里之后，mmc_bootstrap_card()过程继续往下走，
mmc_bootstrap_card() → mmc_bootstrap_block() → mmc_bootstrap_mmcblk() → mmc_bootstrap_mmcblk_partition()
这是执行过程。mmc_bootstrap_mmcblk_partition()函数总共执行了两次。两次的差别主要是参数上的不同，第一次的调用参数是：
／sys/devices/platform/goldfish_mmc.0/mmc_host/mmc0/mmc0:e118/block/mmcblk0
执行一次simulate_uevent()，添加block的信息； 第二次调用的参数是：
/sys/devices/platform/goldfish_mmc.0/mmc_host/mmc0/mmc0:e118/block/mmcblk0/mmcblk0p1
我们来看一下这个函数的实现：
static int mmc_bootstrap_mmcblk(char *devpath)
{
… ...
    if ((rc = mmc_bootstrap_mmcblk_partition(devpath))) {
    … ...
    }
    … ...
    for (part_no = 0; part_no < 4; part_no++) {
    … ...
            if (mmc_bootstrap_mmcblk_partition(part_devpath))
    … ...
        }
    }
… ...
}
两次调用的路径参数，我们已经给出了。 mmc_bootstrap_mmcblk_partition()函数第一次调用会添加mmc卡的DISK信息进去，然后创建一个block device把些信息记录下来，一个主要的信息是这个DISK的Partition信息，这个对后来的挂载起着决定的作用。挂载实际上只是对 Partition才进行的。mmc_bootstrap_mmcblk_partition()通过调用simulate_uevent()函数进行事件的模拟，最后完成操作。值得一说的是，挂载是通过单独线程异步执行的。到这里，关于启动时的分析就介绍到这儿，具体的代码调用，比较复杂，大家可以追踪代码来分析具体的实现。
由于USB大容量存储的挂载还没有实现,ums_bootstrap()是个空函数，所以这一部分可以跳过。还有就是 switch_bootstrap()，这个似乎也是处理USB存储方面的东西，具体代码，还没有仔细的阅读，以后有更新了，我会继续update。
这样我们再次回到vold主函数内部。接下来就是进入while循环阻塞，要对两个链接描述符进行监听，并执行各自的请求了，这里使用了我们熟悉的 select系统调用。当应用层有链接vold socket的请求进来时，这个应用层和vold之间的链接描述符fw_sock就会获得值
==========
在init.rc中启动VOLD这个守护线程和创建socket的命令如下：
service vold /system/bin/vold
    socket vold stream 0660 root mount
================
有一点需要注意，以前使用mountd的时候，也采用了同样的原理，也建立了一个socket
现在这些在init.rc中已经被注释掉了。
#service mountd /system/bin/mountd
#    socket mountd stream 0660 root mount
