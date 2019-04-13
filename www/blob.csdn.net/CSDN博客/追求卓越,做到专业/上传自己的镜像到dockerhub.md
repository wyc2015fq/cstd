
# 上传自己的镜像到dockerhub - 追求卓越,做到专业 - CSDN博客


2019年04月12日 14:21:39[Waldenz](https://me.csdn.net/enter89)阅读数：6标签：[本地镜像上传到dockerhub																](https://so.csdn.net/so/search/s.do?q=本地镜像上传到dockerhub&t=blog)[上传 dockerhub																](https://so.csdn.net/so/search/s.do?q=上传 dockerhub&t=blog)[dockerhub 本地镜像																](https://so.csdn.net/so/search/s.do?q=dockerhub 本地镜像&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=上传 dockerhub&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=本地镜像上传到dockerhub&t=blog)个人分类：[Docker																](https://blog.csdn.net/enter89/article/category/8751169)
[
																								](https://so.csdn.net/so/search/s.do?q=本地镜像上传到dockerhub&t=blog)


1. dockerhub上创建repository。
其实也可以省略这步，执行push操作时，如果dockerhub上不存在该repository，则会自动创建。
![](https://img-blog.csdnimg.cn/20190412102926796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
2. 本地的image名称push到docker时，repository 需要符合格式： dockerid/repository_name,
如，本地之前创建了一个friendlyhello的image, 需要通过 docker tag 变为可push的repository
`docker tag local-image:tagname new-repo:tagname
eg: docker tag friendlyhello waldenz/friendlyhello:v1`![](https://img-blog.csdnimg.cn/20190412140413267.png)
![](https://img-blog.csdnimg.cn/20190412140849158.png)
![](https://img-blog.csdnimg.cn/20190412140908741.png)
3. docker push 到dockerhub上
`docker push waldenz/friendlyhello:v1
// waldenz是本人的dockerhub  id号， friendlyhello是要在传到dockerhub上名称为friendlyhello的repository，
// 如dockerhub上没有，则自动创建`![](https://img-blog.csdnimg.cn/2019041214100516.png)
**注意：**mac 执行docker push命令如果出现denied: requested access to the resource is denied,需要再mac的钥匙串重新登录一次，再docker login即可。
详解参考本人的另一篇文章：[mac docker push : requested access to the resource is denied](https://blog.csdn.net/enter89/article/details/89234961)
4. 在dockerhub上查看
![](https://img-blog.csdnimg.cn/20190412141206948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

