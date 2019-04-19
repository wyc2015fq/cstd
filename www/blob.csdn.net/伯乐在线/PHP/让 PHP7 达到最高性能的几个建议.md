# 让 PHP7 达到最高性能的几个建议 - 文章 - 伯乐在线
原文出处： [惠新宸（@Laruence）](http://www.laruence.com/2015/12/04/3086.html)
[PHP7 已经发布了](http://top.jobbole.com/30974/)，作为PHP十年来最大的版本升级，最大的性能升级，PHP7在多放的测试中都表现出很明显的性能提升，然而，为了让它能发挥出最大的性能，我还是有几件事想提醒下。
![](http://ww4.sinaimg.cn/large/63918611gw1eyovhm41ilj21200ladhn.jpg)
PHP7 VS PHP5.6
### 1. Opcache
记得启用Zend Opcache，因为PHP7即使不启用Opcache速度也比PHP-5.6启用了Opcache快，所以之前测试时期就发生了有人一直没有启用Opcache的事情。启用Opcache非常简单，在php.ini配置文件中加入:

Shell
```
zend_extension=opcache.so
opcache.enable=1
opcache.enable_cli=1"
```
### 2. 使用新的编译器
使用新一点的编译器，推荐GCC 4.8以上，因为只有GCC 4.8以上PHP才会开启Global Register for opline and execute_data支持，这个会带来5%左右的性能提升(Wordpres的QPS角度衡量)
其实GCC 4.8以前的版本也支持，但是我们发现它支持的有Bug，所以必须是4.8以上的版本才会开启这个特性。
### 3. HugePage
我之前的文章也介绍过: [让你的PHP7更快之Hugepage](http://www.laruence.com/2015/10/02/3069.html) ，首先在系统中开启HugePages，然后开启Opcache的huge_code_pages。
以我的CentOS 6.5为例，通过:

Shell
```
$sudo sysctl vm.nr_hugepages=512
```
分配512个预留的大页内存:

Shell
```
$ cat /proc/meminfo  | grep Huge
AnonHugePages:    106496 kB
HugePages_Total:     512
HugePages_Free:      504
HugePages_Rsvd:       27
HugePages_Surp:        0
Hugepagesize:       2048 kB
```
然后在php.ini中加入:

Shell
```
opcache.huge_code_pages=1
```
这样一来，PHP会把自身的text段，以及内存分配中的huge都采用大内存页来保存，减少TLB miss，从而提高性能。
### 4. Opcache file cache
开启Opcache File Cache(实验性)，通过开启这个，我们可以让Opcache把opcode缓存缓存到外部文件中，对于一些脚本，会有很明显的性能提升。
在php.ini中加入:

Shell
```
opcache.file_cache=/tmp
```
这样PHP就会在/tmp目录下Cache一些Opcode的二进制导出文件，可以跨PHP生命周期存在。
### 5. PGO
我之前的文章: [让你的PHP7更快(GCC PGO)](http://www.laruence.com/2015/06/19/3063.html) 也介绍过，如果你的PHP是专门为一个项目服务，比如只是为你的Wordpress，或者drupal，或者其他什么，那么你就可以尝试通过PGO，来提升PHP，专门为你的这个项目提高性能。
具体的，以wordpress 4.1为优化场景。首先在编译PHP的时候首先:

Shell
```
$ make prof-gen
```
然后用你的项目训练PHP，比如对于Wordpress:

Shell
```
$ sapi/cgi/php-cgi -T 100 /home/huixinchen/local/www/htdocs/wordpress/index.php >/dev/null
```
也就是让php-cgi跑100遍wordpress的首页，从而生成一些在这个过程中的profile信息。
最后:

Shell
```
$ make prof-clean
$ make prof-use
```
这个时候你编译得到的PHP7，就是为你的项目量身打造的最高性能的编译版本。
暂时就这么多吧，以后想起来再加，欢迎大家尝试，thanks。
