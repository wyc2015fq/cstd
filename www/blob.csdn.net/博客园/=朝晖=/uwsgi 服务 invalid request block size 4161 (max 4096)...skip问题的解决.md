# uwsgi 服务 invalid request block size: 4161 (max 4096)...skip问题的解决 - =朝晖= - 博客园
# [uwsgi 服务 invalid request block size: 4161 (max 4096)...skip问题的解决](https://www.cnblogs.com/dhcn/p/7115325.html)
   问题报错：   
```
invalid request block size: 4161 (max 4096)...skip
```
    问题原因：默认的uwsgi分配一个小的buffer（4k）来接收每个请求的头信息,如果在日志中看见"invalid request block size",它意味着你需要一个大一点的buffer
    相关参数：
```
-b|--buffer-size                      set internal buffer size
```
     修改办法，可以给uwsgi启动命令家参数：
```
uwsgi -b 8192
```
或者修改uwsgi启动配置文件，比如uwsgi.ini添加以下配置：
```
buffer-size  = 8192
```
相关参考：http://longzhiwen-478.blog.163.com/blog/static/29809749201302495359155/

