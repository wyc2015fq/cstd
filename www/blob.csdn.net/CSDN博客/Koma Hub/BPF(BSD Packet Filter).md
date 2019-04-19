# BPF(BSD Packet Filter) - Koma Hub - CSDN博客
2019年03月14日 21:23:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：13
个人分类：[Linux kernel																[Linux driver																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8645170)](https://blog.csdn.net/Rong_Toa/article/category/7348890)
源地址：[BPF(BSD Packet Filter)--应用和理念扩展](https://blog.csdn.net/dog250/article/details/5876607#commentsedit)
**BPF是一个过滤机制，它用于过滤送往特定地点比如用户空间的数据包，它被设计成一种类似汇编语言的语言，可以称之为伪汇编码。**虽然被设计用来过滤数据包，但这种设计方式**更适合用于操作硬件**，特别用来编写需要写少量固定序列的**硬件驱动程序**。不管用于什么，BPF的设计是优秀的，是状态机实现控制逻辑的完美实例。BPF实际上是一组基于状态机的匹配过滤序列，用于简单的数据包模式匹配。每个匹配包含四个元素，定义为一个结构体：
```cpp
struct socket_filter
{
    __u16    code;  //操作码，可以实现数值运算，加载，比较等操作
    __u8    jt;     //如果匹配跳转到哪里
    __u8    jf;     //如果不匹配跳转到哪里
    __u32    k;     //参数字段，对于不同的操作码有不同的用途。
                    //比如在操作码是比较时存放比较键，
                    //操作码为加载时存放载入数据在数据包(链路帧/数据报)的偏移
}
```
**匹配序列**很像一个汇编程序，有其自身的操作码，操作数以及分支跳转功能，于是这段匹配序列的执行过程自然就类似一个冯诺依曼机器上单进程的执行绪了，它的本质从执行上讲是一个状态机(从数据角度讲，进程又是一个过滤器，它的名字恰就是过滤器...)，很显然其实现应该是一个状态驱动的循环：
```cpp
while(序列中还有匹配){
    switch(当前操作码)
        case 加减乘除:
            ...
        case 加载:
            载入当前匹配项的k值便宜的数据，设为d
            下一个匹配项
        case 比较跳转:
            程序计数器 += 比较结果?当前匹配项的jt字段:jf字段
        ...
}
```
看看linux实现的代码，它基本就是这么实现的：
```cpp
int sk_run_filter(struct sk_buff *skb, struct sock_filter *filter, int flen)
{
    ...//定义中间变量，保存临时计算结果
    int k;
    int pc; //程序计数器，用于分支跳转
    for (pc = 0; pc < flen; pc++) {
        fentry = &filter[pc];
        switch (fentry->code) {
        case BPF_ALU|BPF_ADD|BPF_X:
            A += X;
            continue;
        ...//类似实现减法，乘法，除法，取反，与，或..等操作
        case BPF_JMP|BPF_JA: //涉及分支跳转
            pc += fentry->k;
            continue;
        case BPF_JMP|BPF_JGT|BPF_K:  //大于
            pc += (A > fentry->k) ? fentry->jt : fentry->jf;
            continue;
        ...//类似实现小于等于等比较操作，然后分支跳转
 load_w:    //加载操作，类似x86汇编中的mov，这些load操作也是要区分大小的，比如是load一个字还是双字，还是字节...
            if (k >= 0 && (unsigned int)(k+sizeof(u32)) <= len) {
                A = ntohl(*(u32*)&data[k]);
                continue;
            }
    ...
}
```
**BPF用于很多抓包程序**，在linux中，一般内核自动编译进了**af_packet**这个驱动，因此只需要在用户态配制一个PACKET类型的socket，然后将filter配制进内核即可--使用setsockopt的SO_ATTACH_FILTER命令，这个filter是在用户空间配制的，比如tcpdump应用程序，tcpdump和内核BPF过滤器的关系类似iptables和netfilter的关系，只是netfilter实现了match/target的复杂配合，而BPF的target仅仅是“该数据包要”和“该数据包不要”。当在用户态配制
`tcpdump -i eth0 host 1.2.3.4 ...`
的时候，实际上进入内核的filter就是以下的序列，每个{}中的都是一个socket_filter：
```
...
n:    {加载,0,0,源ip地址在以太帧中的偏移},
n+1:    {比较跳转,n+3,n+2,"1.2.3.4"},
n+2:    {加载,0,0,目标ip地址在以太帧中的偏移},
n+3:    {比较跳转,n+4,n+m,"1.2.3.4"},
n+4:    {...},
...
n+m:    {返回...}
```
然后当有数据包进来的时候，由于tcpdump的socket事先注册进了ptype_all这个list，那么数据包将会复制一份给了tcpdump的socket，然后在其packet_type的func函数中调用run_filter来进行数据包过滤，确定到底需不需要将这个包交给tcpdump。
在windows中，由于其羸弱的网络处理能力以及过渡的分层，或者说为了创立业界标准而导致过度接口化的实现，其内核并没有直接包含BPF，需要一个NDIS过滤驱动来实现，这个实现起来也是蛮简单的，很模块化的。在上面盖一个类似libpcap的接口，这样就可以实现ethereal了。不管在什么操作系统上，如果能将这种伪汇编指令及时编译成机器指令，利用冯诺依曼机器cpu状态机的本质来代替软件函数--比如sk_run_filter，那性能将会有很大的提升。
最后看看BPF的设计理念用于硬件驱动程序的情形，首先定义一个结构体，类似linux的BPF中的socket_filter，但是更加紧凑冗余了，实际上没有必要实现这么多的字段，不过那样的话driver函数就要更复杂了，总之理念一致即可：
```cpp
struct sequence_item {
        int opt;    //操作码：读/写/加减乘除，取反...
        int data;    //操作数
        int port;    //第二操作数，可以为端口
        int flag;    //标志，可存储是否使用中间结果
        char reverse[0] //预留
};
int driver(struct sequence_item *sequence, unsigned int len)
{
    int i = 0;
    int result = -1;
    struct sequence_item si;
    for (; i < len; i++) {
        si = sequence[i];
         if (si.opt == 0) {
            outb_p(si.flag?result:si.data, si.port);
        } else if (si.opt == 1){
            result = inb_p(si.port);
        } else {
            switch (si.opt) {
                case '~':
                    result ~= si.data;
                    break;
                case '^':
                    result ^= si.data;
                    break;
                ...
            }
        }
    }
    return 1;
}
```
[PS]：这个代码是从很早之前(3 years ago)我写的一个驱动程序中抽出来的，所使用的思想竟然和BPF(2 years ago)的一致。
--------------------- 
以上内容是简单我帮大神整理了一下格式，如有侵权请联系删除。
> 
作者：dog250 
来源：CSDN 
原文：https://blog.csdn.net/dog250/article/details/5876607 
版权声明：本文为博主原创文章，转载请附上博文链接！
