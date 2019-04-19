# jemalloc 优化nginx、mysql - walkabc - 博客园
# [jemalloc 优化nginx、mysql](https://www.cnblogs.com/hitfire/articles/6368464.html)
在高性能、高强大的运算下，内存的优化，可以让服务性能再度提升，现在大量的web环境采用了PHP+MYSQL+NGINX的搭配，而查过很多第三方的评测，jemalloc对MYSQL的优化是最强力的：
> 
在单线程下，彼此相差不多，dlmalloc(Doug Lea’s malloc, been around forever)稍占优势。
在多线程下，dlmalloc和pkgmalloc性能急剧下降，而jemalloc在线程数和处理器数一致的时候，性能达到最高，线程数继续增加能保持稳定。
　　个人认为选用jemalloc最合适不过了，那么使用简单的方法，给ng和mysql插上鸟人的翅膀，首先安装jemalloc ：
```bash;gutter
https://github.com/jemalloc/jemalloc/releases/download/3.6.0/jemalloc-3.6.0.tar.bz2
tar xjf jemalloc-3.6.0.tar.bz2
cd jemalloc-3.6.0
./configure
make && make install
echo '/usr/local/lib' > /etc/ld.so.conf.d/local.conf
ldconfig
```
MYSQL（安装目录位置改成mysql的路径）：
```bash;gutter
sed -i 's@executing mysqld_safe@executing mysqld_safe\nexport LD_PRELOAD=/usr/local/lib/libjemalloc.so@' /mysql的安装目录/mysql/bin/mysqld_safe
```
重启即可。
NGINX需要重新编译，在编译时候增加以下参数：
```bash;gutter
--with-ld-opt="-ljemalloc"
```
查看jemalloc状态：
```bash;gutter
lsof -n | grep jemalloc
```
