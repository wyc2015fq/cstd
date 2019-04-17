# bootm命令中地址参数，内核加载地址以及内核入口地址 - DoubleLi - 博客园






bootm命令只能用来引导经过mkimage构建了镜像头的内核镜像文件以及根文件镜像，对于没有用mkimage对内核进行处理的话，那直接把内核下载到连接脚本中指定的加载地址0x30008000再运行就行，内核会自解压运行（不过内核运行需要一个tag来传递参数，而这个tag是由bootloader提供的，在u-boot下默认是由bootm命令建立的）。



通过mkimage可以给内核镜像或根文件系统镜像加入一个用来记录镜像的各种信息的头。同样通过mkimage也可以将内核镜像进行一次压缩(指定-C none/gzip/bzip2),所以这里也就引申出了两个阶段的解压缩过程：第一个阶段是u-boot里面的解压缩，也就是将由mkimage压缩的镜像解压缩得到原始的没加镜像头的内核镜像。第二个阶段是内核镜像的自解压， u-boot 里面的解压实际上是bootm 实现的 ， 把 mkimage -C bzip2或者gzip 生成的 uImage进行解压 ； 而kernel的自解压是对zImage进行解压，发生在bootm解压之后。



下面通过cmd_bootm.c文件中对bootm命令进行解析以及执行的过程来分析，这三种不同地址的区别：



ulong load_addr = CFG_LOAD_ADDR;  /* Default Load Address */



int do_bootm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

       ......

        if (argc < 2) {
             addr = load_addr;//当bootm命令后面不带地址参数时，将默认的加载地址赋值给addr
        } else {
             addr = simple_strtoul(argv[1], NULL, 16); //如果bootm命令后面带了加载地址，则将该地址赋值给addr,所以最终有用的地址还是bootm命令后附带的地址
        }



        ......



        //

        switch (hdr->ih_comp) { //开始判断利用mkimage命令后是否对内核镜像进行了压缩
        case IH_COMP_NONE:  //如果没有被压缩，只是在原有的内核镜像前加了一个头
                 if(ntohl(hdr->ih_load) == addr) { //这步很重要，涉及到我们要讨论的两个地址，我们知道在利用mkimage时指定-a选项后面就是指定的内核加载的地址，这个地址会被存放到镜像头结构的ih_load成员变量中如在smdk2410中ih_load为0x30008000,在这里开始作地址的判断，如果指定的加载地址和之前的addr也就是bootm后面附带的地址相同，则不用将内核镜像搬到其他地方，就在这个地址上执行，这样内核的入口地址就要在加载地址之后的64个字节(因为镜像头占了64个字节)，所以入口地址为0x30008040
                       printf ("   XIP %s ... ", name);
                 } else {//如果指定的加载地址和bootm命令后的附加地址不相同，我们看看下面data此时表示的是什么地址：data = addr + sizeof(image_header_t);可以看到如果指定加载地址与bootm命令后地址不相同，则从bootm命令后面地址所在地取出内核镜像的头进行检验，检验完后data指向真正的内核，然后将内核拷贝到指定的加载地址处来进行自解压运行，这个时候内核的入口地址就和加载地址一样，不需要加上40个字节，因为内核镜像前面的40个字节的头已经被取出来了。

                       memmove ((void *) ntohl(hdr->ih_load), (uchar *)data, len);

                  }

        case IH_COMP_GZIP://如果利用mkimage时对内核镜像进行了压缩，则需要在u-boot内进行第一阶段的解压缩，将解压后的内核镜像存放到指定的加载地址ih_load，然后内核镜像自解压启动
                 printf ("   Uncompressing %s ... ", name);
                 if (gunzip ((void *)ntohl(hdr->ih_load), unc_len,(uchar *)data, &len) != 0) {
                       puts ("GUNZIP ERROR - must RESET board to recover/n");
                       SHOW_BOOT_PROGRESS (-6);
                      do_reset (cmdtp, flag, argc, argv);
                 }
                 break;



}





所以如果使用mkimage生成内核镜像文件的话，会在内核的前头加上了64byte的信息，供建立tag之用。bootm命令会首先判断bootm xxxx 这个指定的地址xxxx是否与-a指定的加载地址是否相同。
(1)如果不同的话会从这个地址开始提取出这个64byte的头部，对其进行分析，然后把去掉头部的内核复制到-a指定的load地址中去运行之
(2)如果相同的话那就让其原封不同的放在那，但-e指定的入口地址会推后64byte，以跳过这64byte的头部。





我们来看看这三个地址的不同情况：

1> mkimage -A arm -O linux -T kernel -C none -a 30008000 -e 
30008040 -n linux-2.6.18.8 -d zImage uImage2.6.18.8-8040 
这种情况 ，只能把 uImage download到 30008000的位置上，否则 从 30008040
是启动不了的。

原因：如果将uImage(加了头的镜像文件)下载到不同于指定加载地址的地方，则会进行上面的操作，将去掉头部的内核拷贝到指定的加载地址，此时加载地址和入口地址需要是相同的，因为已经没有镜像头了，所以此时入口地址也应该为30008000，而不应该再加上64个字节

所以在构建镜像头部中的加载地址和入口地址时千万要考虑下载的地址，否则将会启动不了。
2> mkimage -A arm -O linux -T kernel -C none -a 30008000 -e 
30008000 -n linux-2.6.18.8 -d zImage uImage2.6.18.8-8000 
这种情况download地址随便。 还是按上面说的，因为将加载地址和入口地址设置成同样的地址，在下载到任意地址时，将去掉头部的内核镜像拷贝到指定加载地址后，可以直接从加载地址开始启动。但是要是下载地址和指定加载地址相同呢？也就是下面的：


如果 tftp 下载地址==0x30008000 ， 此时因为下载地址和指定加载地址相同，所以就不会搬动，内核直接从指定加载地址自解压啦，但是因为指定的入口地址也是0x30008000,还是在镜像头处，可以看到上面的代码，如果相同没有做任何事，只是打印了提示信息，所以还得将入口地址往后推后64个字节还是从 0x30008040 启动就肯定OK 。



所以在制作镜像头以及下载地址就有两种情况：

1，mkimage -n 'linux-2.6.14' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -d zImage zImage.img

加载地址和入口地址相同

tftp 0x31000000 zImage.img

bootm 0x31000000

下载地址可以任意放。



2,mkimage -n 'linux-2.6.14' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d zImage zImage.img

入口地址在加载地址后面64个字节

tftp 0x30008000 zImage.img

bootm 0x30008000

下载地址一定要在指定的加载地址上。









