# IAR FOR AVR 生成eep文件 - xqhrs232的专栏 - CSDN博客
2019年03月26日 15:26:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：22
原文地址::[http://www.wendangku.net/doc/00f615c15fbfc77da269b11a.html](http://www.wendangku.net/doc/00f615c15fbfc77da269b11a.html)
相关文章
1、IAR FOR AVR 生成eep文件----[https://wenku.baidu.com/view/00f615c15fbfc77da269b11a.html](https://wenku.baidu.com/view/00f615c15fbfc77da269b11a.html)
IAR FOR AVR 中如何生成*.eep文件Free_Bird
1:首先,要先勾选Project->Option->Linker->Extra Options->Use command line options并在其中添加下面三个语句:
-Oraw-binary,(CODE)=.bin
-Ointel-extended,(CODE)=.hex
-Ointel-extended,(XDATA)=.eep
则可在D:\Program Files\IAR Systems\Embedded Workbench 5.3\***\Debug\Exe分别生成*.bin\*.hex\*.eep 文件
2:被关键字__eeprom修饰的变量将被定义到EEPROM空间
__eeprom unsigned char A = 0x35;/ /默认是从eeprom的0x0000地址开始存放。
3此时,:如果程序中用到了变量A,编译器就会将其生成到*.eep文件中,可顺利下载;并且我发现,每次下载*.eep文件,都要将芯片中的程序全部擦除后再进行,并且写完*.eep文件要紧接着将EESA VE熔丝位挑上;然后再烧写Flash 文件,否则数据可能会被破坏;
4:如果程序中没有用到变量A,编译器则不会将其生成到*.eep文件中,此时向芯片中下载*.eep文件会提示“The Intel Hex input file is empty”即:eep文件为空。如下图:
![IAR FOR AVR 生成eep文件](http://www.wendangku.net/pic/view?ih=&rn=1&doc_id=00f615c15fbfc77da269b11a&o=png_6_0_0_0_0_0_0_892.979_1262.879&pn=1&iw=&ix=&sign=f5547a7e2bfd4dd2f7a1e604f2aa6b86&type=1&iy=&aimw=&app_ver=2.9.8.2&ua=bd_800_800_IncredibleS_2.9.8.2_2.3.7&bid=1&app_ua=IncredibleS&uid=&cuid=&fr=3&Bdi_bear=WIFI&from=3_10000&bduss=&pid=1&screen=800_800&sys_ver=2.3.7)
