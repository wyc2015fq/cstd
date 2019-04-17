# nginx静态编译lua[jit] - 囧囧有神的专栏 - CSDN博客





2013年12月24日 15:51:20[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：4489








原淘宝工程师清无(王晓哲)和春来(章亦春)开发的lua nginx模块非常强大。现在网上的安装方法主要都是动态编译lua的，但是总有种种原因动态编译有诸多不便，这时候就需要将luajit静态编译进nginx里




第一种方法就是直接使用 openresty，并利用openresty提供的方式静态编译luajit




安装nginx nginx_lua_module luajit nginx_dev_kit步骤略




第二种方法就是在nginx  configure执行过后，修改生成的objs/Makefile文件

在第一个target  “objs/nginx: objs/src/core/nginx.o \”下面，添加luajit的静态文件

把

-Wl,-E -lpthread -lcrypt -L  /xxx/xxx/lua/libs -lluajit-5.1 -lm -ldl /xxx/xxx/pcre-8.02/.libs/libpcre.a -lssl -lcrypto -ldl -lz

改成

-Wl,-E -lpthread -lcrypt /xxx/xxx/libluajit-5.1.a -lm -ldl /xxx/xxx/pcre-8.02/.libs/libpcre.a -lssl -lcrypto -ldl -lz




改完后 再执行 make && make install 就行了

我们用ldd nginx命令可以看到，已经依赖luajit动态链接库了




第三种方法

移除luajit的so文件，只保留.a文件，这样ld在连接时找不到so文件，就只好链接.a文件了。




这样改了之后，还需要修nginx_lua的config文件，否则configure会失败，失败原因是gcc需要-ldl参数

修改nginx_lua的config 

将

ngx_feature_libs="-R$LUAJIT_LIB -L$LUAJIT_LIB -lluajit-5.1 -lm"

改为

ngx_feature_libs="-R$LUAJIT_LIB -L$LUAJIT_LIB -lluajit-5.1 -lm -ldl"

即可

相应位置的ngx_feature_libs 都需要添加-ldl这个参数




改完后，重新编译nginx，就可以成功了。



