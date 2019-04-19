# nginx中配置跨域支持功能 - Ms.Fox的专栏 - CSDN博客
2019年04月04日 15:47:15[lulu-lu](https://me.csdn.net/smbluesky)阅读数：6
# [nginx中配置跨域支持功能](https://www.cnblogs.com/wenhainan/p/8630377.html)
在我们访问文件的时候，会出现  
No 'Access-Control-Allow-Origin' header is present on the requested resource.之类的提示，遇到这种问题最简单的方式就是在服务器进行配置，当然客户端的方式就是jsonp，但是麻烦，还是下面的解决方式比较简单
vi /etc/nginx/nginx.conf
加入如下代码
http {
  ###start####
  add_header Access-Control-Allow-Origin *;
  add_header Access-Control-Allow-Headers X-Requested-With;
  add_header Access-Control-Allow-Methods GET,POST,OPTIONS;
  ###end ###
}
:wq    保存
service nginx restart 重启nginx即可。
