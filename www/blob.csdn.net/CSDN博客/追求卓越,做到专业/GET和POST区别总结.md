
# GET和POST区别总结 - 追求卓越,做到专业 - CSDN博客


2012年07月23日 13:56:28[Waldenz](https://me.csdn.net/enter89)阅读数：426


1.get是从服务器上获取数据,post是像服务器上传数据
2.get把参数数据列加到url中,值和表单内各个字段一一对应,在url中可以看到
post是将表单内各个字段和其内容放置在http header内一起传到url地址
(因此get安全性比post低)
3.get方式,服务器用Request.QueryString获取变量的值
post方式,用Request.Form获取提交的数据
4.get传送数据量小(IE对URL长度的限制是2083字节(2K+35)
post传输数据不受限制
(参考:[ http://blog.csdn.net/gideal_wang/article/details/4316691\#comments](http://blog.csdn.net/gideal_wang/article/details/4316691#comments))


