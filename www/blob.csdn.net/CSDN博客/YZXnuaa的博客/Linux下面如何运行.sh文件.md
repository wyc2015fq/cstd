# Linux下面如何运行.sh文件 - YZXnuaa的博客 - CSDN博客
2018年04月26日 16:53:29[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2196
## 方法一：当前目录执行.sh文件
- 
1
**【步骤一】cd到.sh文件所在目录**
比如以hello.sh文件为例，如下图
![Linux下面如何运行.sh文件？](https://imgsa.baidu.com/exp/w=500/sign=61261ea10f24ab18e016e13705fbe69a/4b90f603738da9770a527dacb651f8198618e33c.jpg)
- 
2
**【步骤二】给.sh文件添加x执行权限**
比如以hello.sh文件为例，chmod u+x hello.sh，如下图
![Linux下面如何运行.sh文件？](https://imgsa.baidu.com/exp/w=500/sign=080e5ca4b67eca80120539e7a1229712/a6efce1b9d16fdfa154e3c69b28f8c5494ee7b13.jpg)
- 
3
**【步骤三】./执行.sh文件**
比如以hello.sh文件为例，./hello.sh 即可执行hello.sh文件，如下图
![Linux下面如何运行.sh文件？](https://imgsa.baidu.com/exp/w=500/sign=6be4ab5b52e736d158138c08ab524ffc/b2de9c82d158ccbfbf6b50bd1fd8bc3eb0354166.jpg)
- 
4
**【步骤四】sh 执行.sh文件**
以hello.sh文件为例，sh hello.sh即可执行hello.sh文件，如下图
![Linux下面如何运行.sh文件？](https://imgsa.baidu.com/exp/w=500/sign=fda43973b14543a9f51bfacc2e168a7b/7af40ad162d9f2d31991c368afec8a136227ccb1.jpg)
END
## 方法二：绝对路径执行.sh文件
- 
1
下面三种方法都可以，如下图
./home/test/shell/hello.sh
/home/test/shell/hello.sh
sh /home/test/shell/hello.sh
![Linux下面如何运行.sh文件？](https://imgsa.baidu.com/exp/w=500/sign=ccda0c4ed12a60595210e11a1835342d/4a36acaf2edda3cce10c58a707e93901203f9249.jpg)[](http://jingyan.baidu.com/album/3f16e003e51a752591c103a4.html?picindex=5)
