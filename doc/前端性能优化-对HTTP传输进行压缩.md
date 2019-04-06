# 前端性能优化-对HTTP传输进行压缩

#  

# 简介

资深的前端开发人员都知道，在web开发中，对js、css、图片、font等都要进行压缩，尽可能的减小文件的大小，减少前端下载的时间，从而提高网页响应的时间。特别是在移动端，这对用户的流量还有影响。不过本文中所提的压缩并不是指这情况，而是在js，css、图片、font等资源已经压缩了的基础上（当然，这一步非必要条件，压不压缩看你心情，资源文件的压缩跟http传输过程的压缩没关系），在http传输过程中的再次压缩。

## HTTP1.1中的Accept-Encoding

在HTTP1.1开始，Web客户端可以通过Acceppt-Encoding头来标识对压缩的支持。客户端HTTP请求头声明浏览器支持的压缩方式，服务端配置启用压缩，压缩的文件类型，压缩方式。当客户端请求到达服务器，服务器响应时对请求资源进行压缩，返回给客户端浏览器，浏览器按照相应的方式进行解析。

先来看下天猫网站的案例。





![img](https://upload-images.jianshu.io/upload_images/3020614-4e20f97ed55d1e68.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

天猫首页index.js的HTTP过程

在HTTP请求中，accept-encoding: gzip,  deflate, sdch,  br是指客户端浏览器（这里是我的chrome浏览器）支持的压缩方式。在HTTP响应中，content-encoding:gzip  是指服务端使用了gzip这种压缩方式。





![img](https://upload-images.jianshu.io/upload_images/3020614-de275659a9bda482.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)





![img](https://upload-images.jianshu.io/upload_images/3020614-471642d10fae56f0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/407)

在google的network中我们可以看到，index.js是5kb,这是传输过程中的大小。当我们把它的文件下载下来查看时发现他是9.62kb，压缩了44%。

不仅仅只是天猫，在国内大多数网站上都能看到使用这个方法来优化前端性能的身影。

## 压缩方式选择哪一种？

上面提到的压缩方式有这么多种，我们应该使用哪一个呢？事实上这点不用我们操心，选择gzip就可以了，参考Yahoo！工程师的说法，支持gzip的浏览器范围最广，使用gzip的压缩效果最佳。实际上经过我亲自考证，www.tmall.com、www.taobao.com、www.jd.com、www.suning.com、www.qq.com、www.baidu.com  这些大型网站都无一例外的使用了gzip这种压缩方式。

## 如何使用gzip进行压缩

怎么样才能使用HTTP的这个属性来压缩呢？这才是本文的重点。其实很简单，客户端不用任何配置，在服务端配置即可，服务器不同，配置方法也不尽相同。本文以tomcat为例，用我亲自测试的过程来展现。

### （1）tomcat的配置

以我电脑tomcat安装目录为例，找到F:\software\apache-tomcat-7.0.54\conf文件夹下的server.xml





![img](https://upload-images.jianshu.io/upload_images/3020614-619b55c7f4d93dcf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/504)





![img](https://upload-images.jianshu.io/upload_images/3020614-4929f89dcd2d52af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

compress="on" 表示开启压缩。

compressionMinSize="2048"表示会对大于2KB的文件进行压缩。一般按照如此配置，可根据自己需求而定。

compressableMimeType是指将进行压缩的文件类型，对js，css，图片的压缩可以大大提升压缩效果。

tomcat的server.xml的配置按照上述进行配置，重启tomcat服务器。

### （2）压缩前后对比，效果明显！  



![img](https://upload-images.jianshu.io/upload_images/3020614-09f4f6636c43b622.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

在我的demo中，有js，css，jpg图片，font，mp4等基本涵盖所有常用的资源类型。上面是未压缩前的情况，值得注意的是上面的status都是200，都是未缓存的情况。size一列显示了传输过程中的大小。页面整体响应时间大概在800ms左右。

接下来我们查看一下使用gzip压缩后的情况（特别值得注意的是，修改了server.xml后重启tomcat是无效的。需要在eclipse中删除，然后在新创建一个tomcat服务器，需要指向我们刚刚配置的tomcat）。





![img](https://upload-images.jianshu.io/upload_images/3020614-d3f9847eb9beb76c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

上图是使用gzip压缩后的情况，非常明显的可以看出，document从原来的8.5KB压缩到现在的1.5KB,压缩了82%，angular.js压缩了75%，yitalaUI压缩了61.5%，jpg的图片几乎没有怎么压缩，微乎其微，可见我们上面的配置需要修改，只需要对document，js，css压缩即可。我们没有对font文件和mp4文件进行压缩，所以上面大小不变。

另外特别值得注意的是整体响应时间减少了200ms，提高了25%？真的是这样吗？错了，因为传输过程的时间跟网络有关系，我在本地电脑测试，网络是极好的，上面实际上是看不出传输时间的改善。减少了200ms是其他不确定的因素起的作用。实际上如果网站的document的大小和css，js文件数量越多则效果越明显。如果有条件，你可以自行搭建环境测试。

## gzip的浏览器兼容性

在8年前，即2008年，yahoo团队的《High.Performance.Web.Sites》一书中就指出大部分浏览器都支持gzip。现在浏览器都已更新换代好几代了，基本上都支持gzip。因此我们不需要考虑客户端不支持的情况（不支持的情况是非常严重的）。

下面是浏览器对gzip的支持情况：

Netscape 6+ (Netscape 4-5 does, but with some bugs).

Internet Explorer 5.5+ (July 2000) and IE 4 if set to HTTP/1.1.

Opera 5+ (June 2000)

Lynx 2.6+ (some time before 1999)

Firefox 0.9.5+ (October 2001)

Chrome since forever

Safari since forever (as far as I can tell)

## 弊端

对HTTP传输内容进行压缩是改良前端响应性能的可用方法之一，大型网站都在用。但是也有缺点，就是压缩过程占用cpu的资源，客户端浏览器解析也占据了一部分时间。但是随着硬件性能不断的提高，这些问题正在不断的弱化。

本文中提到的方法，读者可大胆的使用。国内国外大型的网站都在使用，前端的性能优化必须对这些细节非常注意，变量引起质变，如果每个地方你都极尽最佳性能，你开发出来的网站性能将会尤其突出。