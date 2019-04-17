# appweb-esp原理 - V__KING__的专栏 - CSDN博客





2018年11月08日 00:25:01[v__king__](https://me.csdn.net/V__KING__)阅读数：224
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)









**问题1**： 当在浏览器中加载hello.esp时，如何才能不让esp去编译view_961a91577424cf374750e5b8f5c21138.c(这个.c文件由hello.esp文件生成)。

方案1：可以试试修改esp-compile.json

方案2：既然是在浏览器访问的时候去compile的，那么，可不可以预先就编译好呢?

答案：动态加载hello.esp的时候，必须要编译的。why？因为首先hello.esp中嵌入的c代码需要生效，就必须生成真正的c代码，既然生成了新的c代码，那么就必须编译。

所以， 在嵌入式设备中需要gcc编译器！！

问题2：假设我不想在嵌入式设备中放gcc编译器，怎么办?

答案2：使用esp compile命令预先编译好so文件。

参考：[https://www.embedthis.com/esp/doc/users/pages.html](https://www.embedthis.com/esp/doc/users/pages.html)
### 安装：

按照 [https://www.embedthis.com/esp/doc/start/quick.html](https://www.embedthis.com/esp/doc/start/quick.html) 进行安装
- makeme，类似于makefile的编译管理
- Pak ，包管理软件，类似于apt-get
- Expansive ，静态layout的，类似于go语言中的一些静态打包程序。

### 编译并且运行esp

[https://www.embedthis.com/esp/doc/start/tour.html](https://www.embedthis.com/esp/doc/start/tour.html)

这里我并没奖esp 安装linux系统中。而只是在build/bin目录中去创建hello.esp,并且执行。

注意：如何了解esp的运行过程: ./esp运行之后，不需要重启./esp。

执行./esp之后，修改了hello.esp文件(修改里面嵌入的c代码，故意留个c语法错误)。查看错误如下。

```
gcc -shared -g -Wall -Wno-unused-result -DPIC -fPIC -I. -Isrc
-I/home/vk/esp/build/linux-x86-default/inc -L/home/vk/esp/build/linux-x86-default/bin -Wl,--enable-new-dtags -Wl,-rpath,$ORIGIN/    -o /home/vk/esp/build/linux-x86-default/bin/cache/view_961a91577424cf374750e5b8f5c21138.so /home/vk/esp/build/linux-x86-default/bin/cache/view_961a91577424cf374750e5b8f5c21138.c,
```

看到上面的错误，后查看对应的代码，你会发现：
- esp是一个主程序，就是web server程序。
- 在build/bin/cache目录下生成的view_279264239472839429.so 应该是一个so库插件。
- 当在浏览器刷新加载动态的hello.esp的时候，第一次动态加载hello.esp时，esp会去编译cache/view_961a91577424cf374750e5b8f5c21138.c 代码(该c文件是中间产物，最终生成so文件)

### 安装esp后

sudo make install

去运行esp --home 

出现错误。

运行esp --listen 

还是监听的127.0.0.1
### FAQ

问题1：出现404的错误。

原因：执行esp后，home 目录是dist
- The `pak.json` file to describe the name and version of your application with any installed packages.
- The `esp.json` file to configure ESP and HTTP.
- The `dist` directory is the final distribution directory for web content after rendering.





