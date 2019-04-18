# 【Java】【web】【计算机网络】cookie学习总结 2018-9-28 - csdn_baotai的博客 - CSDN博客

2018年09月28日 10:05:53[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：44


# cookie学习总结

## Cookie的操作:

> - 
创建Cookie对象.

> 
Cookie cookie = new Cookie(String name,String value);

参数:

name:  该当前Cookie取一个唯一的名字.

value: 存储在Cookie的共享数据,只能是String类型.

​      Cookie cookie = new Cookie(“currentName”,“will”);

- 
把Cookie放入响应中,响应给浏览器,把共享的数据存储在浏览器中.

> 
response.addCookie(cookie);

- 
获取Cookie以及获取Cookie中的数据.

> 
因为Cookie存在在请求头中,所以应该通过request去获取.

Cookie[] cs = req.getCookies();

> 
获取当前Cookie的名字: String name = cookie对象.getName();

获取当前Cookie的值:   String value= cookie对象.getValue();

- 
Cookie的中文问题.

> 
在Cookie中属性名和属性值都不能使用中文.

- 
修改Cookie中指定属性名的属性值.

> 
需求:修改Cookie cookie = new Cookie(“currentName”,“will”);
- 创建一个同名的新的Cookie.

Cookie c = new Cookie(“currentName”,“Lucy”);
- 获取该Cookie对象,通过setValue方法,重新设置新的value值.

Cookie对象.setValue(“新的值”);

注意:别忘了,重新把该Cookie放入响应中:resp.addCookie©;

- 
Cookie的分类(会话Cookie和持久化Cookie):

> 
会话Cookie:   关闭浏览器之后,Cookie就销毁了. 缺省情况.

持久化Cookie: Cookie可以保存指定的时间段(3天,一周,一个月).

设置Cookie的最大存活时间: cookie对象.setMaxAge(int seconds);
- seconds  == 0:  删除Cookie.
- seconds  <  0:  会话
- Cookie.seconds  >  0:  存储指定的秒数

- 
删除Cookie: cookie对象.setMaxAge(0);

## Cookie的domain:

> 
Cookie的path是在同一主机中指定共享Cookie，如果主机不同那么就一定不能共享Cookie，无论path是什么。

如果希望不同的二级域名中可以共享Cookie，那么就要设置Cookie的domain和path了。

例如：[music.baidu.com](http://music.baidu.com)、[map.baidu.com](http://map.baidu.com)、[tieba.baidu.com](http://tieba.baidu.com)，它们的域名不同，但百度希望它们之间可以共享    	Cookie，那么就要设置domain了。

1). 设置Cookie的path为“/”，例如：cookie.setPath("/");在整个baidu.com中都能传递.

2). 设置Cookie的domain，例如：cookie.setDomain(".baidu.com")，其中domain中没有指定域名前缀！

## Cookie的缺陷:

> - 多个人使用同一台电脑的时候,可以查看浏览器的Cookie,不安全.
- Cookie存储中文比较麻烦(得编码,再解码).
- Cookie的value是String类型,一个Cookie就只能存储一个数据,如果需要存储多	个数据,就得死还有N个Cookie.
- 一个站点对Cookie有限制:

​      Cookie大小限制在4KB之内；

​      一台服务器在一个客户端最多保存20个Cookie；

​      一个浏览器最多可以保存300个Cookie；
- 在设计上就有问题,考虑生活中问题.

​     Cookie是浏览器和服务器之间数据交互的凭证.

​     在生活中,我们是把识别数据存储到服务端

