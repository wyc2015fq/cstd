
# Windows环境配置xdebug调试PHP - 无界 - CSDN博客

2017年02月01日 14:20:02[21aspnet](https://me.csdn.net/21aspnet)阅读数：11757个人分类：[PHP本质																](https://blog.csdn.net/21aspnet/article/category/929611)



**1.版本对应**
php版本，TS 和NTS，VC9和VC11，32位和64位都需要正确才可以。
首先用phpinfo查看PHP安装版本
![](https://img-blog.csdn.net/20170201140858032)

**2.下载对应版本**
[https://xdebug.org/download.php](https://xdebug.org/download.php)
![](https://img-blog.csdn.net/20170201141053608)
比如下载的是32位的TS版本：php_xdebug-2.4.1-5.4-vc9.dll，这个文件复制进任意目录都可以。

**3.配置php.ini文件**

```python
[Xdebug]  
;指定Xdebug扩展文件的绝对路径  
zend_extension=D:/phpext/php_xdebug-2.4.1-5.4-vc9.dll 
;启用性能检测分析  
xdebug.profiler_enable=On
;启用代码自动跟踪  
xdebug.auto_trace=on
;允许收集传递给函数的参数变量  
xdebug.collect_params=On
;允许收集函数调用的返回值  
xdebug.collect_return=On
;指定堆栈跟踪文件的存放目录  
xdebug.trace_output_dir="D:/debug"
;指定性能分析文件的存放目录  
xdebug.profiler_output_dir="D:/debug"
xdebug.profiler_output_name = "cachegrind.out.%p"
xdebug.remote_enable = On
xdebug.remote_port=9000
xdebug.remote_handler = dbgp
xdebug.remote_host = localhost
xdebug.var_display_max_depth = 10
```
**4.重启php**查看phpinfo()有无加载好xdebug扩展
![](https://img-blog.csdn.net/20170201141409567)
说明：如果没有这个说明配置不对或者版本不对

**5.配置netbean**
![](https://img-blog.csdn.net/20170201141643411)

**6.打断点开始调试**
![](https://img-blog.csdn.net/20170201141939760)


