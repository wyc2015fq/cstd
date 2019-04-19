# Ubuntu下配置Anaconda、Pycharm编译环境 - 刘炫320的博客 - CSDN博客
2017年05月03日 17:09:59[刘炫320](https://me.csdn.net/qq_35082030)阅读数：4149
# 1. 必要性
Linux比Windows做开发实在是要强太多了。自带的Python编译器，命令行模式，让程序员都非常的喜欢。而且最重要的是，使用Linux,你感觉自己真的是在使用编程的方式运用这台计算机。而使用Windows，你真觉得一切都是Window…
当然，我并不会因为逼格，才去Linux的，而是太多的优秀的库Word2vec、fasttext等都需要C或者Python环境。虽然Windows也能勉强（经过一系列填坑以后，而且是能填好。通常是填不好）运行C语言版本的这些库。但是Linux下就可以很完美的运行（好吧，经过这篇以后，我觉得Linux也不是很容易了，可能是我打开的方式不对）。
总而言之，我们从无到有，总共分为这么几步：
- 安装Ubuntu
- 安装java
- 安装pycharm
- 安装anaconda
- 安装g++、gcc
- 安装fasttext
下面我们将会一步步进行讲解。
# 2. 安装java
第一步安装Ubuntu，不用我讲了吧，对于执意要装实体Ubuntu的大神不用看我的小白帖子，对于装VM的Ubuntu，使用VM的简易安装程序，完全无人值守即可完成安装。
下面我们重点讲解安装java，其实这话不准确，应该是jdk，也就是java开发包。为什么要装java呢，是因为pycharm需要它，所以必须先装java，再装pycharm，不然pycharm运行不起来。
网上是这样教我们的，只需要三个步骤：
```
sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install Oracle-java8-installer
```
当我们小白一样，按照这个命令一个个执行的时候，发现第二个开始就开始警告，到第三个的时候，完全执行不了。
当然，你也可以使用下载好的jdk包，然后安装，具体的可以参阅：[《Ubuntu下配置java》](http://blog.sina.com.cn/s/blog_53a99cf30102v6o4.html)
不过无论你如何挣扎，都会出现第二个的错误，为`E: Some index files failed to download, they have been ignored, or old ones used instead.`
这个错误，网上有提示说要修改DNS，但是实际上修改DNS并不能解决，至少我没有解决。还需要删除库后，然后修改更新的镜像，这里推荐使用163的镜像，因为阿里的镜像也被ign了。
```
sudo rm /var/cache/apt/archives/lock
sudo rm /var/lib/dpkg/lock
```
上面这两个是删除lock的，应该是没有用，但是为了安全起见，我也做了。这个的结果就是最后更新的时候，需要多做很多。
具体的可以参考[163镜像修改](http://www.cnblogs.com/lixiaolun/p/5495911.html)、[Update失败处理](http://blog.csdn.net/ronghua_liu/article/details/8609450)、[解决方案](http://www.2cto.com/os/201410/341871.html)
至此，基本上都解决完错误了。下面我们安装pycharm。
# 3. 安装pycharm
只需要去官网下载一个最新的pycharm即可，然后具体的就看这里[《安装pycharm》](http://blog.csdn.net/kevinelstri/article/details/57413791)。值得要提的一点是，一定要看其内容评论，修改hosts文件之后才能使用：
找到etc/hosts，在终端中打开`$sudo gedit hosts`，加上一行 
`0.0.0.0 account.jetbrains.com`就可以了。
# 4. 安装anaconda
Anaconda的安装就没什么好讲的，主要就是下载比较缓慢，这里建议从清华镜像下载，并且使用迅雷下载。这里要提醒的是Anaconda3 - 4.2之前还是python3.5版本，之后就是3.6版本了。因此我们这里选择Anaconda3 - 4.2安装，因为有很多库并不支持python3.6。
# 5. 安装g++、gcc
Fasttext包需要C语言的支持，因此，需要安装g++、gcc。 
这里，使用`g++ -v`和`gcc -v`来查看自身的系统的版本，如果没有的话，还是需要安装的，这里可以使用以下命令： 
`sudo apt-get install build-essential`
这样就把依赖包都安装了，这里可以参考[《安装G++、Gcc》](http://blog.csdn.net/yunfenglw/article/details/46911471)。 
当然你也可以使用：
```
apt-get install gcc-4.8
apt-get install g++-4.8
```
来安装指定版本的，这里是有一些奇怪的地方，例如有可能安装的和Ubuntu内核不一致。建议使用与Ubuntu内核一致的编译器。
# 6. 安装fasttext
Pycharm的包安装里当时并未找到fasttext的包。命令行下的pip install安装时，在运行时，由于一个`***i385-cython.so`的文件，这个文件是fasttext的用户层的动态库，在编译时总是出现一个错误`__cxa_throw_bad_array_new_length。`
这个错误连StackOverFlow上面都没有解决办法，它总是会指向第五步里的错误，认为是编译器4.9版本以上才会出现的错误，但是我们第五步已经修改为4.8并且与内核相同了。
因此，我就想直接从外部包直接安装这个，因为那样的话，其编译过程，也就是生成动态库的过程是由我们系统自己编译的，应该不会出现这个问题，结果就这样解决了。
具体的，解决办法如下： 
首先下载好fasttext包，然后再fasttext.tar.gz包所在的目录下执行：
```
tar -zxvf fasttext-0.8.3.tar.gz
cd fasttext-0.8.3
python setup.py build
python setup.py install
```
因为pip的安装不一定有用，毕竟我们之前用过了，有可能仍然出现问题。
# 7. 小结
至此，我们的安装这个的艰难之路就结束了。因为网上很多这个过程，都是以顺利的过程结束的，但是实际安装中，仍然会存在很多问题。我也仅仅是列举了我所遇到的问题以及解决办法。如果出现了其他问题，还是需要另查资料来解决。
