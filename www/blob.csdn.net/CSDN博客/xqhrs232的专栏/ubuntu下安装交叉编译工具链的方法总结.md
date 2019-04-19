# ubuntu下安装交叉编译工具链的方法总结 - xqhrs232的专栏 - CSDN博客
2016年03月21日 11:57:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：487
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://blog.csdn.net/yinjiabin/article/details/7369071](http://blog.csdn.net/yinjiabin/article/details/7369071)
相关文章
1、ubuntu下安装交叉编译工具链 
 ----[http://407672744.blog.163.com/blog/static/13007915820109264250985/](http://407672744.blog.163.com/blog/static/13007915820109264250985/)
2、Ubuntu下安装ARM交叉编译器图文教程----[http://www.linuxidc.com/Linux/2014-09/106637.htm](http://www.linuxidc.com/Linux/2014-09/106637.htm)
版权声明：本文为博主原创文章，未经博主允许不得转载。
1.下载arm-linux-gcc-3.4.1.tar.bz2到任意的目录下，我把它下载到了我的个人文件夹里 /home/wrq  
arm-linux-gcc-3.4.1.tar.bz2 的下载地址如下：
http://www.handhelds.org/download/projects/toolchain/arm-linux-gcc-3.4.1.tar.bz2
2. 解压 arm-linux-gcc-3.4.1.tar.bz2
   #tar -jxvf arm-linux-gcc-3.4.1.tar.bz2
   解压过程需要一段时间，解压后的文件形成了 usr/local/ 文件夹，进入该文件夹，将arm文件夹拷贝到/usr/local/下
# cd usr/local/ 
#cp -rv arm /usr/local/
   现在交叉编译程序集都在/usr/local/arm/3.4.1/bin下面了
3. 修改环境变量，把交叉编译器的路径加入到PATH。(有三种方法，强烈推荐使用方法一)
       方法一：修改/etc/bash.bashrc文件
#vim /etc/bash.bashrc
在最后加上：
export PATH＝$PATH:/usr/local/arm/3.4.1/bin
export PATH  
        方法二：修改/etc/profile文件：
 # vim /etc/profile
增加路径设置，在末尾添加如下,保存/etc/profil文件：
export PATH＝$PATH:/usr/local/arm/3.4.1/bin
4. 立即使新的环境变量生效，不用重启电脑：
                 对应方法一：#source /root/.bashrc
                 对应方法二：# source /etc/profile
5. 检查是否将路径加入到PATH：
                 # echo $PATH
  显示的内容中有/usr/local/arm/bin，说明已经将交叉编译器的路径加入PATH。至此，交
叉编译环境安装完成。
6. 测试是否安装成功
   # arm-linux-gcc -v  
上面的命令会显示arm-linux-gcc信息和版本，显示的信息：
Reading specs from /usr/local/arm/3.4.1/lib/gcc/arm-linux/3.4.1/specs Configured with: /work/crosstool-0.27/build/arm-linux/gcc-3.4.1-glibc-2.3.2/gcc-
3.4.1/configure --target=arm-linux --host=i686-host_pc-linux-gnu
--prefix=/usr/local/arm/3.4.1 --with-headers=/usr/local/arm/3.4.1/arm
-linux/include --with-local-prefix=/usr/local/arm/3.4.1/arm-linux --disable
-nls --enable-threads=posix --enable-symvers=gnu --enable-__cxa_atexit --enable-
languages=c,c++ --enable-shared --enable-c99 --enable-long-long
Thread model: posix  
gcc version 3.4.1
7.编译
Hello World程序，测试交叉工具链
写下下面的Hello World程序，保存为
hello.c
   #include 
    int main()
    {
    printf("Hello World!\n");
       return 0;
    }
执行下面的命令：
    # arm-linux-gcc -o hello hello.c  
源程序有错误的话会有提示，没有任何提示的话，就是通过了，就可以下载到ARM目标板上运行了！接着可以输入file hello的命令，查看生成的hello文件的类型，要注意的是生成的可执行文件只能在ARM体系下运行，不能在其于X86的PC机上运行。
