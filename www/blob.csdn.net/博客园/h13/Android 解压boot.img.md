# Android 解压boot.img - h13 - 博客园
其实解压、打包boot.img没什么难度一看就会咯！！
   1.先下附件：工具。*[点击打开链接](http://code.lardcave.net/entries/2010/11/24/164025/split_bootimg.pl)**6.0 KB, 下载次数: 60)*      解压到bin文件夹里，方便以后使用。
   2.解压boot.img文件
     去到boot.img所在的文件夹，输入下面的命令：
split.pl boot.img         
       Page size: 2048 (0x00000800)
        Kernel size: 2240184 (0x00222eb8)
        Ramdisk size: 221504 (0x00036140)
        Second size: 0 (0x00000000)
        Board name:
        Command line:androidboot.hardware=blade console=null g_android.product_id=0x1354 g_android.serial_number=Blade-CM7
        Writing boot.img-kernel ... complete.
        Writing boot.img-ramdisk.gz ... complete.
        完了之后，你就会发现多了boot.img-kernel 和 boot.img-ramdisk.gz文件，这就是解压出来的boot.img，其中boot.img-kernel 就是内核，boot.img-ramdisk.gz就是ramdisk。
       3.打包boot.img文件
             在替换和内核后，就要对boot.img打包了！！下是代码:
mkbootimg --cmdline 'androidboot.hardware=blade console=null g_android.product_id=0x1354 g_android.serial_number=Blade-CM7'  --kernel boot.img-kernel --ramdisk boot.img-ramdisk.gz -o new.img
             命令中-cmdline‘xxx’，xxx就是解压boot.img是Command line：xxx
new.img文件就是你修改后的boot.img文件了。改个名字替换到ROM里面就可以了。！！
             new.img文件就是你修改后的boot.img文件了。改个名字替换到ROM里面就可以了。！！
# ./split_bootimg.pl boot.img Page size: 2048 (0x00000800) Kernel size: 1388548 (0x00153004) Ramdisk size: 141518 (0x000228ce) Second size: 0 (0x00000000) Board name: Command line: no_console_suspend=1 Writing boot.img-kernel ... complete. Writing boot.img-ramdisk.gz ... complete.
Extract the ramdisk.
# mkdir ramdisk # cd ramdisk # gzip -dc ../*-ramdisk.gz | cpio -i # cd ..
Make any changes necessary (e.g., set ro.secure=0 in default.prop).
Recreate the cpio archive using the mkbootfs binary produced from building the Android source code (The cpio utility in OS X does not recognize the newc format, therefore mkbootfs is the best option for OS X users).
# mkbootfs ./ramdisk | gzip > ramdisk-new.gz
Recreate the image file using the mkbootimg binary produced from building the Android source code.
# mkbootimg --cmdline 'no_console_suspend=1 console=null' --kernel boot.img-kernel --ramdisk ramdisk-new.gz -o boot-new.img --base 0x20000000
=======================================================================
Make any changes necessary (e.g., set ro.secure=0 in default.prop).
Recreate the cpio archive using the mkbootfs binary produced from building the Android source code (The cpio utility in OS X does not recognize the newc format, therefore mkbootfs is the best option for OS X users).
# mkbootfs ./ramdisk | gzip > ramdisk-new.gz
Recreate the image file using the mkbootimg binary produced from building the Android source code.
# mkbootimg --cmdline 'no_console_suspend=1 console=null' --kernel boot.img-kernel --ramdisk ramdisk-new.gz -o boot-new.img --base 0x20000000
打包ramdisk.img :  find . |cpio -ov -H newc |gzip > ../ramdisk.img
[http://blog.csdn.net/tody_guo/article/details/7515151](http://blog.csdn.net/tody_guo/article/details/7515151)
