# SSM实现图片上传 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月27日 20:26:18[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：2346


SSM配置在此不多说，之前写过。

1、导包

![](https://img-blog.csdn.net/20180327195510288)

2、想要实现图片上传，form表单必须是 post请求，并且需要加上enctype="multipart/form-data"

![](https://img-blog.csdn.net/20180327195609331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3、springmvc.xml 配置文件需要 配置multipartResolver   节点

![](https://img-blog.csdn.net/20180327195350995)

4、controller中 

参数通过MultipartFile file获取表单图片对象

通过 file.getOriginalFilename()  获取图片名

通过String realPath = res.getServletContext().getRealPath("/"); 拿到项目跟路径

然后就是 new File() 拼接要存的位置， 但注意 不能放在WEB-INF 下，因为图片都是get请求，在WEB-INF下，无法通过get请求获取到。

同时把 图片的url地址 （我这里使用的就是图片名） 存入数据库当中。

5、jsp显示页面

jsp中取的时候 就通过 http：//ip地址：端口号// 项目名 （这一堆可以封装起来放入request域）+自定义的路径+图片名  来取

![](https://img-blog.csdn.net/20180327202534289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、springmvc.xml

同时 得在 springmvc.xml中配置过滤器，否则会被mvc 的servlet 拦截，导致无法获取

![](https://img-blog.csdn.net/20180327200836573?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

mapping 代表 url 输入 /resource/a.jpg   /resource/a.png   /resource/img/a.jpg   /resource/img/a.png  都不会拦截

 ** 就代表多层目录 均可

location 就是 资源的 位置

6、维护sql语句的 xml 文件中动态查询

![](https://img-blog.csdn.net/20180327201351387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于 修改操作，可能会 不换图片，但是如果不换图片<input type="file" >这个标签默认的值为null，最终导致重定向到列表首页的时候  没有图片显示。 所以需要动态查询。

7、add.jsp页面中  为了实现 选完上传的图片以后 可以查看到自己所选的 图片

通过 onload事件 ， 动态向img标签的src 属性赋值。

![](https://img-blog.csdn.net/20180327201810757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

8、edit.jsp 为了实现 将 原来的信息带入到 修改页面。 

需要给显示图片的 img 标签 的src 赋一个初始值，初始值 通过 id来查询。


