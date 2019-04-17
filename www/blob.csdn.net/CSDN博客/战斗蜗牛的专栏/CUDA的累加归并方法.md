# CUDA的累加归并方法 - 战斗蜗牛的专栏 - CSDN博客





2012年03月21日 07:09:09[vbskj](https://me.csdn.net/vbskj)阅读数：3727








把一个n元素数组里面的值全部加起来的程序，大家都会写，一个for循环搞定。但是如果在cuda代码里面也这么写的话，速度会比[cp](http://www.linuxso.com/command/cp.html)u上还慢，毕竟人家一对一对拼的话，肯定是干不过cpu的。

在cuda里面的算法是n个线程一起参与这个运算。

nv[id](http://www.linuxso.com/command/id.html)ia貌似有个官方的算法，几乎所有看到的cuda代码里面都是用的它：
for(int i=(blockDim.x>>1);i>0;i>>=1){
    if(threadIdx.x < i){
        pis[threadIdx.x] += pis[threadIdx.x+i];
    }
    __sy[nc](http://www.linuxso.com/command/nc.html)threads();
}

if(threadIdx.x == 0)    r[0] = pis[0];
就是把数组的内容都对应地读到shared内存中，然后二叉归并。



不过这个算法只支持n是2的幂的情况，如果不是，则会漏掉很多元素。比如n = 3,第三个元素就被漏掉了。

但是还原到单线程算法么，实在太慢，非常不甘心。想了一个新的算法。
for(int i = 1;i < blockDim.x ; i <<= 1){
    if(threadIdx.x % (i<<1) == i){
        tmp[threadIdx.x - i] += tmp[threadIdx.x];
    }
    __[sync](http://www.linuxso.com/command/sync.html)threads();        
}    

if(threadIdx.x == 0)    r[0] = tmp[0];
当n不为2的幂时也照常工作，当需要累加的只是线程中的一部分的时候，只需要在算法前把超出的线程return掉就可以了。鲁棒性非常好。原理比较简单，但是有点抽象，基本就是把奇数累加到前面一个位置，然后作为结果的偶数们形成新的数组，继续。到只剩下0为止。

算法复杂度两个都是lgN,（如果N比sp的数量少的话），新算法应该会稍微慢一些，因为取模算法应该会耗时一些。

虽然是原创，不过之前肯定已经有人也这么写过了，不过暂时google不到。记录一下，让google能找到它。



