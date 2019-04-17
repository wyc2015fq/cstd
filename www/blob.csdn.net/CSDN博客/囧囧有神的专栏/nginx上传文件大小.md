# nginx上传文件大小 - 囧囧有神的专栏 - CSDN博客





2014年03月03日 18:03:34[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：14292








采用nginx作反向代理，出现了一个诡异的问题，小文件可以提交，大文件会报500内部错误。这个是什么原因导致的呢？

    查wiki可知，上传文件大小相关的有三个配置

client_body_buffer_size 配置请求体缓存区大小, 不配的话，


client_body_temp_path 设置临时文件存放路径。只有当上传的请求体超出缓存区大小时，才会写到临时文件中

client_max_body_size 设置上传文件的最大值




所以查出来，问题出现的原因是

1.文件大小超过了client_body_buffer_size

2.client_body_temp_path的临时文件路径居然没有写权限

以上两个原因导致了返回500错误。


如果上传文件大小超过client_max_body_size时，会报413 entity too large的错误。




原因知道了，修正就简单了。

1.client_body_buffer_size 尽量设置的大点，这是基于速度的考虑，如果因为设置的过小，导致上传的文件老要写磁盘，那速度就太慢了。

2.client_body_temp_path 路径要有可写权限，这个是明显的错误了。改正了就好

3.client_max_body_size 设置上传文件的最大值，这个是基于安全的考虑，我们认为正常用户不会或者基本不会上传太大的文件。

可以设置为client_max_body_size 100m;  或者按照自己的业务来设置这个值。
























