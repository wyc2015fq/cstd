# /usr/lib/uwsgi/plugins/python_plugin.so: cannot open shared object file: No such file or directory - =朝晖= - 博客园
# [/usr/lib/uwsgi/plugins/python_plugin.so: cannot open shared object file: No such file or directory](https://www.cnblogs.com/dhcn/p/7116212.html)
 Django uwsgi部署方式下产生这个Bug，后来发现把uwsgi配置ini文件里面的
```
#plugins = python
```
把上面这句配置语句注释掉，uwsgi就可以运行了，当然，是正常可用运行状态。

