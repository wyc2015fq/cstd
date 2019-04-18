# img src的特殊使用 - z69183787的专栏 - CSDN博客
2013年01月25日 21:58:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10345
img标签的 src属性，一般都是图片的路径。不过src也可以这样使用：
Html代码  ![收藏代码](http://ganqing.iteye.com/images/icon_star.png)
- <imgsrc="${contextPath}/album/image.do?r=2&id=${imgId}">
 这时当页面加载图片时，会调用后台的方法去生成图片。后台的方法必须要输出一个图片。
Java代码  ![收藏代码](http://ganqing.iteye.com/images/icon_star.png)
- String path = getPathById(imgId); // 得到图片路径
- File file = new File(path);  
- ....  
- response.setHeader("Pragma", "No-cache");  
- response.setHeader("Cache-Control", "no-cache");  
- response.setDateHeader("Expires", 0L);  
- response.setHeader("Content-Length", file.length() + "");  
- 
- InputStream in = new FileInputStream(file);  
- OutputStream out = response.getOutputStream();  
- ...// 从in读，写到out里
 这样就可以动态的加载图片了。
现在知道这其实并不特殊，我们正常的使用img的src的图片路径是在服务器可以读取的地方，如使用tomcat的话，图片如存在web-inf父目录下，tomcat会自动帮我们读取图片写入到response的输出流中。但是如果图片存在其他地方，tomcat无法找到，那就需要我们在程序中自己去读取图片写入输出流，如上面所示。
在输出图片时可以设置response的header中Cache-Control、dateheader的Expires来控制图片的缓存，可以避免每次加载图片都要做读取，写入输出流的操作。提高性能。关于Cache-Control和Expires的设置可参考下面的链接：
[http://tongjian.iteye.com/blog/234715](http://tongjian.iteye.com/blog/234715)
[http://www.cnblogs.com/cuixiping/archive/2008/05/04/1181056.html](http://www.cnblogs.com/cuixiping/archive/2008/05/04/1181056.html)
设置图片缓存后，会导致图片更新后仍然显示缓存图片的问题，这种情况可为图片添加一个版本号，将版本号写入img的src属性里，这样图片变化后img的src地址就会变化，浏览器会重新加载图片，保证图片及时得到更新。
