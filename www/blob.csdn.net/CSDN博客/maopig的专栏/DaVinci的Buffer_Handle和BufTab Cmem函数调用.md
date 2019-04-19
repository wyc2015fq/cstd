# DaVinci的Buffer_Handle和BufTab Cmem函数调用 - maopig的专栏 - CSDN博客
2012年01月08日 17:48:40[maopig](https://me.csdn.net/maopig)阅读数：1704
DaVinci的dvsdk里面，所有的内存用的都是CMEM，而比较上层的一个封装就是BufTab和Buffer_Handle了
每个Tab可以控制好多块内存块。
具体到内存块有两种，一种是创建的时候指定他的大小啊，物理地址啥的。另外一种就是仅仅创建出这个
结构的一个实例，而物理地址啊啥的都留空，等到回头用的时候，用指针搞定。
第一类，对应的配置里面有一个叫reference的变量，配成false的话，创建的时候就直接创建出来内存块
了，reference这点BufTab和Buffer_Handle的创建配置是一样的。
第二类，reference配置成true，这样的话，创建之后，指针没有指向，而需要后面重新指定。指定的时
候，要用Buffer_setUserPtr，同时还要设一下大小，Buffer_setNumBytesUsed，同时用这两个，不然程
序会down掉的。
我试验了，使用CMEM手动分配物理空间，用reference=TRUE的配置，声明一个Buffer_handle的结构，使
用cmem函数直接申请空间，然后再配置Buffer_handle的指针。
CMEM_AllocParams cmem_params;
Int8 *ptr_area_fifo;
cmem_params.type = CMEM_POOL;
cmem_params.flags = CMEM_NONCACHED;
for(loop_num = 0; loop_num < fifo_total; loop_num++) {
ptr_area_fifo = (Int8 *)CMEM_alloc(bufSize, &cmem_params);
//hBufTab is created while its reference=TRUE.
hDstBuf = BufTab_getBuf(hBufTab, loop_num);
Buffer_setNumBytesUsed(hDstBuf, bufSize);
if (Buffer_setUserPtr(hDstBuf, ptr_area_fifo) < 0) {
printf("error while set prt into bufTab, id=%d\n", loop_num);
}
}
