
# 用C语言写PHP扩展 - 无界 - CSDN博客

置顶2012年03月22日 15:20:13[21aspnet](https://me.csdn.net/21aspnet)阅读数：14269


**用C语言写PHP扩展**
**1：预定义**
在home目录，也可以其他任意目录，写一个文件，例如caleng_module.def
内容是你希望定义的函数名以及参数：
int a(int x,int y)
string b(string str,int n)
**2：到php源码目录的ext目录**
\#cd /usr/local/php-5.4.0/ext/
执行命令，生成对应扩展目录
\#./ext_skel --extname=caleng_module --proto=/home/hm/caleng_module.def
**3：修改config.m4**
去掉dnl的注释
PHP_ARG_ENABLE(caleng_module, whether to enable caleng_module support,
Make sure that the comment is aligned:
[  --enable-caleng_module           Enable caleng_module support])
**4：修改caleng_module.c**
```python
/* {{{ proto int a(int x, int y)
    */
PHP_FUNCTION(a)
{
	int argc = ZEND_NUM_ARGS();
	int x;
	int y;
    int z;
	if (zend_parse_parameters(argc TSRMLS_CC, "ll", &x, &y) == FAILURE) 
		
		return;
z=x+y;
		RETURN_LONG(z);
}
/* }}} */
/* {{{ proto string b(string str, int n)
    */
PHP_FUNCTION(b)
{
	char *str = NULL;
    int argc = ZEND_NUM_ARGS();
    int str_len;
    long n;
    char *result;
    char *ptr;
    int result_length;
 
    if (zend_parse_parameters(argc TSRMLS_CC, "sl", &str, &str_len, &n) == FAILURE)
        return;
    result_length = str_len * n;
    result = (char *) emalloc(result_length + 1);
    ptr = result;
    while (n--) {
        memcpy(ptr, str, str_len);
        ptr += str_len;
    }
    *ptr = '\0';
    RETURN_STRINGL(result, result_length, 0);
}
/* }}} */
```
**5：生成扩展库**
\#cd ./caleng_module
\#/usr/local/php/bin/phpize
\#./configure --with-php-config=/usr/local/php/bin/php-config
\#make
\#make install
![](http://hi.csdn.net/attachment/201203/22/0_1332400698hvNa.gif)
**6：到php的对应extensions目录**
如上图所示
\#cd /usr/local/php/lib/php/extensions/no-debug-non-zts-20100525/
改目录下有生成的caleng_module.so文件
**7：修改php.ini**
php.ini如果找不到可以从phpinfo()打出的信息看到
\#cd /usr/local/php/lib/
php.ini增加扩展信息
extension=caleng_module.so
**8:重启Apache**
\# /usr/local/apache2/bin/apachectl restart
**9:检查加载**
/usr/local/php/bin/php -m
**10:PHP调用**
```python
echo a(1,2);
```
输出 3  就说明成功了！
=================================
下面是原文

### Linux下用C开发PHP扩展
一、首先下载PHP源码包，假设源码包目录为：/software/php-5.2.13
一、首先下载PHP源码包，假设源码包目录为：/software/php-5.2.13
\#> cd /software/php-5.2.13/ext
二、假设我们要[开发](http://www.itokit.com/tag.php?tag=%BF%AA%B7%A2)一个名为caleng_module的扩展，该扩展包含两个函数：a--处理两个整型相加和b-处理字符串重复输出；
1、首先编写一个函数定义文件，该文件编写函数原型后缀为def，假设为：caleng_module.def
int a(int x, int y)
string b(string str, int n)
2、通过扩展骨架生成器，将在ext目录下自动建立扩展目录caleng_module
\#> ./ext_skel --extname=caleng_module --proto=caleng_module.def
3、修改配置文件: \#> vim /software/php-5.2.13/ext/caleng_module/config.m4，将如下行的注释标签"dnl"去掉，修改后如下所示：
PHP_ARG_ENABLE(myfunctions, whether to enable myfunctions support,
Make sure that the comment is aligned:
[  --enable-myfunctions           Enable myfunctions support])
4、完善函数a和b的功能: \#> vim /software/php-5.2.13/ext/caleng_module/caleng_module.c
PHP_FUNCTION(a)
{
int x, y, z;
int argc = ZEND_NUM_ARGS();
if (zend_parse_parameters(argc TSRMLS_CC, "ll", &x, &y) == FAILURE)
return;
z = x + y;
RETURN_LONG(z);
}
PHP_FUNCTION(b)
{
char *str = NULL;
int argc = ZEND_NUM_ARGS();
int str_len;
long n;
char *result;
char *ptr;
int result_length;
if (zend_parse_parameters(argc TSRMLS_CC, "sl", &str, &str_len, &n) == FAILURE)
return;
result_length = str_len * n;
result = (char *) emalloc(result_length + 1);
ptr = result;
while (n--) {
memcpy(ptr, str, str_len);
ptr += str_len;
}
*ptr = '\0';
RETURN_STRINGL(result, result_length, 0);
}
三、编译安装，假设php的安装目录为:/usr/localhost/webserver/php
\#> cd /software/php-5.2.13/ext/caleng_module
\#> /usr/localhost/webserver/php/bin/phpize
\#> ./configure --with-php-config=/usr/localhost/webserver/php/bin/php-config
\#> make
\#> make install
现在将在/usr/local/webserver/php/lib/php/extensions/no-debug-non-zts-20060613目录下生成caleng_module.so文件
在php.ini配置文件中加入: extension=caleng_module.so.
搞定收工
更多参考：[http://blog.csdn.net/heiyeshuwu/article/details/3453854](http://blog.csdn.net/heiyeshuwu/article/details/3453854)
[http://blog.csdn.net/taft/article/details/596291](http://blog.csdn.net/taft/article/details/596291)
[http://blog.sina.com.cn/s/blog_638f9da701012zk3.html](http://blog.sina.com.cn/s/blog_638f9da701012zk3.html)

