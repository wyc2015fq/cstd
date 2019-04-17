# Intel Edison 移植mjpg-stream - V__KING__的专栏 - CSDN博客





2015年12月15日 14:28:16[v__king__](https://me.csdn.net/V__KING__)阅读数：877标签：[交叉编译																[英特尔](https://so.csdn.net/so/search/s.do?q=英特尔&t=blog)](https://so.csdn.net/so/search/s.do?q=交叉编译&t=blog)
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)








参考：  
[http://blog.csdn.net/v__king__/article/details/50184347](http://blog.csdn.net/v__king__/article/details/50184347)
[https://github.com/xeecos/edi-cam](https://github.com/xeecos/edi-cam)

1. 安装上面的方法，在你的开发机上完成。 

2. 安装intel Edison 的SDK-交叉编译工具 

    - 到官网下载 

    - 解压后，执行里面的安装脚本。 

3. 修改mjpeg-stream目录下的Makefile 

    - cd mjpeg-stream 

    - source /opt/poky-edison/1.7.2/environment-setup-core2-32-poky-linux 

    - 修改 所有的Makefle 

        Makefile 

        input_control/Makefile 

        output_autofocus/Makefile 

        output_http/Makefile 

        output_viewer/Makefile 

        input_file/Makefile 

        input_testpicture  output_file/Makefile 

        output_rtsp/Makefile 

        input_gspcav1/Makefile 

        input_uvc/Makefile 

        output_udp/Makefile 

    全部修改编译工具为： 

    CC =  i586-poky-linux-gcc  -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer –sysroot=/opt/poky-edison/1.7.2/sysroots/core2-32-poky-linux 

    - make 

4. cp  mjpeg-stream.tar.gz 到 Intel Edison板子上面。 

5. 接下来的就是去执行了




