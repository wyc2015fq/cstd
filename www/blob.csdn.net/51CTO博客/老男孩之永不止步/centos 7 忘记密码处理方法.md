# centos 7 忘记密码处理方法-老男孩之永不止步-51CTO博客
1、重启进去GRUB菜单，按e
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/87a42e1a5f0ee450b0c43ad1709f09ec.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2、然后再使用方向的下方向键按到linux16开头那行，确保使用的语言，若是中文需要更改为英文的，否则后期更改密码出现乱码，另外将ro更改为rw,末行输入enforcing=0,init=/bin/bash
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/d0921edadff4950cc4f55362aeed465c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/73685829b2fd0911105f422fe42be5ff.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
3、按ctrl+x进入下一步
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/5f4db85f77e27fcf4520bacf63638b9b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
4、进入bash设置密码：passwd root
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/852e8beb3f4249c4cdeaeb56c6f054c7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
5、检查是否更改生效，重启：exec /sbin/init
![centos 7 忘记密码处理方法](https://s1.51cto.com/images/blog/201904/03/a87c3b04f047bb460d25426bfb7765ad.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
