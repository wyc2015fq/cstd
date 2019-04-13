
# mac docker push : requested access to the resource is denied - Error saving credentials - 追求卓越,做到专业 - CSDN博客


2019年04月12日 13:55:05[Waldenz](https://me.csdn.net/enter89)阅读数：35标签：[docker mac																](https://so.csdn.net/so/search/s.do?q=docker mac&t=blog)[Error saving credentials																](https://so.csdn.net/so/search/s.do?q=Error saving credentials&t=blog)[requested access 																](https://so.csdn.net/so/search/s.do?q=requested access &t=blog)[resource is denied																](https://so.csdn.net/so/search/s.do?q=resource is denied&t=blog)[docker mac 报错																](https://so.csdn.net/so/search/s.do?q=docker mac 报错&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=resource is denied&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=requested access &t=blog)个人分类：[Docker																](https://blog.csdn.net/enter89/article/category/8751169)
[
																								](https://so.csdn.net/so/search/s.do?q=requested access &t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Error saving credentials&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Error saving credentials&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=docker mac&t=blog)
docker push的时候报错如下：
`denied: requested access to the resource is denied`![](https://img-blog.csdnimg.cn/20190412134206288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
是docker没有登录的原因， 使用docker login，输入dokcer id 和password，后又出现如下错误：
`Error saving credentials: error storing credentials - err: exit status 1, out: `The user name or passphrase you entered is not correct.``![](https://img-blog.csdnimg.cn/20190412134118374.png)
解决方法：
1. 找到 钥匙串.app, 右键点击登录->锁定钥匙串登录->再解锁钥匙串登录即可，解锁时要输入mac的登录密码
![](https://img-blog.csdnimg.cn/20190412135041678.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
2. 操作完成后，返回去重新输入 docker login，输入docker id和password后，显示 Login Succeeded，成功
![](https://img-blog.csdnimg.cn/20190412135213345.png)
3. 重新执行 dock push 操作，上传到dockerhub成功！
![](https://img-blog.csdnimg.cn/201904121352595.png)
4. dockerhub上查看repository
![](https://img-blog.csdnimg.cn/20190412135737664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)


