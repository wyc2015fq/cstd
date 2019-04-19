# NGINX开启GZIP压缩 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2019年01月28日 18:39:44[dutong0321](https://me.csdn.net/dutong0321)阅读数：21
### 为什么要开启GZIP？
主要是在于HTTP传输主要以文本为主，尤其是大量的JS/CSS/HTML还有图片等等，那么开启了GZIP后，就可以在传输的过程中对内容进行压缩，既减少了对于带宽的压力，还可以提升用户打开网页的速度，何乐而不为呢？
### 我为什么要开启GZIP？
没错，我开启GZIP的根本原因和上面没有半毛钱关系，以上带来的好处最多也仅仅是算是附属品吧，我遇到的难题是：使用百度的开源编辑器umeditor插件，但是其中的umeditor.js/umeditor.min.js无法完整的加载，而且只有这2个文件不正常，其他均正常，但是压缩后就变得好用了。
[百度umeditor编辑器中js的坑爹问题](https://blog.csdn.net/dutong0321/article/details/86681628)
### 在NGINX中开启GZIP
其实，在NGINX的默认配置文件中，已经写了一部分，不过被注释掉了，就是在http中的server上面有一行：
```
#gzip  on;
```
那么仅仅把注释去掉也是不可以的，只是开启了一部分，我们还需要更多的设置。
```
gzip  on;
        gzip_min_length 1k;
        gzip_buffers 4 16k;
        gzip_comp_level 3;
        gzip_types text/plain text/css application/xml application/javascript application/x-javascript text/javascript;
```
第一行，必须开启，开启了以后下面的内容才生效。
第二行，大于1K的文件才会压缩，小文件压缩也节省不了多少带宽，所以就没有这个必要。
第三行，压缩的缓存区，可以不用管，照写就是。
第四行，压缩的级别，越高，压缩的文件越小，但是压缩的时间比较长，我选择3.
第五行，压缩的类型，我都说了我最主要的目的就是使js进行正常的加载，所以就没有配置图片，如果有需要可以对PNG/BMP等图片进行压缩一下，还是可以节省很多的空间的，这里因人而异吧。不过需要注意的是，压缩javascript文件必须上面三个内容都必须写，而且顺序不能乱，否则没有效果。
最后，重启或者重新加载nginx的配置文件。
```
nginx -s reload
```
然后打开[网页GZIP压缩检测](http://tool.chinaz.com/Gzips/)的这个网页，对其进行检测就可以了。
