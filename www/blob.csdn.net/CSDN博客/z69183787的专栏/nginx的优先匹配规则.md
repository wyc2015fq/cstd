# nginx的优先匹配规则 - z69183787的专栏 - CSDN博客
2017年11月24日 21:56:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：632
nginx的优先匹配规则
以=开头表示精确匹配
如 A 中只匹配根目录结尾的请求，后面不能带任何字符串。
^~ 开头表示uri以某个常规字符串开头，不是正则匹配
~ 开头表示区分大小写的正则匹配;
~* 开头表示不区分大小写的正则匹配
/ 通用匹配, 如果没有其它匹配,任何请求都会匹配到
顺序不等于优先级：
(location =) > (location 完整路径) > (location ^~ 路径) > (location ~,~* 正则顺序) > (location 部分起始路径) > (/)
# 第一个必选规则
一般会nginx代理多个tomcat，有一个作为首页，其余的只作为接口相互调用。
```
```bash
location
```
```bash
/cms
```
```bash
{
```
```bash
```
```bash
index.html html
```
```bash
```
```bash
proxy_pass http:
```
```bash
//tomcat
```
```bash
:8080;
```
```bash
}
```
```
第二种是静态，当然也有使用静态做首页的，都是死的，点了也不会跳转的，直接映射本地目录。
```
```bash
location ^~
```
```bash
/static/
```
```bash
{
```
```bash
```
```bash
root
```
```bash
/webroot/static/
```
```bash
;
```
```bash
}
```
```bash
location ~* \.(gif|jpg|jpeg|png|css|js|ico)$ {
```
```bash
```
```bash
root
```
```bash
/webroot/res/
```
```bash
;
```
```bash
}
```
```
第三种就是动静分离：
静态服务器放静态资源，nginx做反代：静态服务器访问本地静态资源，开启80端口。
```
```bash
location = / {
```
```bash
```
```bash
root
```
```bash
/data/www
```
```bash
;
```
```bash
}
```
```bash
nginx web service做代理：
```
```bash
location = / {
```
```bash
index.html html
```
```bash
```
```bash
proxy_pass http:
```
```bash
//static
```
```bash
server:80;
```
```bash
}
```
```
第四种就是接口调用，nginx代理tomcat。
```
```bash
location
```
```bash
/api
```
```bash
{
```
```bash
```
```bash
proxy_pass http:
```
```bash
//tomcat
```
```bash
:8080/;
```
```bash
}
```
```
举一个简单的例子优先匹配规则，不能完全模仿生产，开发需求访问www.xxx/cms,如果是*.html的都跳到静态资源，其他的都跳到tomcat pool；当然静态资源和tomcat pool下面都有index.html文件。在自己的测试机试了下可以做到。
```
```bash
[root@www conf.d]
```
```bash
# cat admin.conf
```
```bash
server {
```
```bash
```
```bash
listen       80;
```
```bash
```
```bash
server_name  localhost;
```
```bash
location
```
```bash
/cms
```
```bash
{
```
```bash
```
```bash
root
```
```bash
/data/www
```
```bash
;
```
```bash
```
```bash
}
```
```bash
location ~*
```
```bash
/cms/
```
```bash
.*\.html$ {
```
```bash
```
```bash
root
```
```bash
/data/222
```
```bash
;
```
```bash
}
```
```bash
}
```
```
目录下面的文件内容如下：
```
```bash
[root@www conf.d]
```
```bash
# ls
```
```bash
admin.conf  default.conf.bak
```
```bash
[root@www conf.d]
```
```bash
# cat /data/222/cms/index.html
```
```bash
2
```
```bash
[root@www conf.d]
```
```bash
# cat /data/www/cms/index.html
```
```bash
index1
```
```
![wKioL1kCuQGiCkmmAABYafJIyV4330.png](https://s2.51cto.com/wyfs02/M02/92/C1/wKioL1kCuQGiCkmmAABYafJIyV4330.png)
测试访问url：[http://172.16.2.24/cms/,看它默认跳转。](http://172.16.2.24/cms/,%E7%9C%8B%E5%AE%83%E9%BB%98%E8%AE%A4%E8%B7%B3%E8%BD%AC%E3%80%82)
![wKiom1kCuOmAbsyJAABqHmxskz4658.png](https://s5.51cto.com/wyfs02/M02/92/C2/wKiom1kCuOmAbsyJAABqHmxskz4658.png)
测试成功，跟书写顺序无关，优先匹配。
