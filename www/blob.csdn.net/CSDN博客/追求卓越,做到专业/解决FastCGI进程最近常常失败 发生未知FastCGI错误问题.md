
# 解决FastCGI进程最近常常失败 发生未知FastCGI错误问题 - 追求卓越,做到专业 - CSDN博客


2019年01月10日 14:29:49[Waldenz](https://me.csdn.net/enter89)阅读数：439标签：[FastCGI进程																](https://so.csdn.net/so/search/s.do?q=FastCGI进程&t=blog)[FastCGI进程最近常常失败																](https://so.csdn.net/so/search/s.do?q=FastCGI进程最近常常失败&t=blog)[发生未知FastCGI错误问题																](https://so.csdn.net/so/search/s.do?q=发生未知FastCGI错误问题&t=blog)[iis																](https://so.csdn.net/so/search/s.do?q=iis&t=blog)[an unknown fastcgi error 																](https://so.csdn.net/so/search/s.do?q=an unknown fastcgi error &t=blog)[
							](https://so.csdn.net/so/search/s.do?q=iis&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=发生未知FastCGI错误问题&t=blog)个人分类：[系统框架设计																](https://blog.csdn.net/enter89/article/category/1094534)
[
																								](https://so.csdn.net/so/search/s.do?q=发生未知FastCGI错误问题&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=FastCGI进程最近常常失败&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=FastCGI进程最近常常失败&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=FastCGI进程&t=blog)
问题如图：
![](https://img-blog.csdnimg.cn/20190110134230767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
**解决方法：**
应用程序池->高级设置->标识： 将默认的ApplicationPoolIdentify修改为LocalSystem。
参考：[https://github.com/Microsoft/PTVS/issues/2024](https://github.com/Microsoft/PTVS/issues/2024)
![](https://img-blog.csdnimg.cn/2019011014530927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)


