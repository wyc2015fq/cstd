
# Rundeck基础：4：基础概念：操作操作Node - 知行合一 止于至善 - CSDN博客

2019年04月10日 21:23:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：24标签：[rundeck																](https://so.csdn.net/so/search/s.do?q=rundeck&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[node																](https://so.csdn.net/so/search/s.do?q=node&t=blog)[基础																](https://so.csdn.net/so/search/s.do?q=基础&t=blog)[教程																](https://so.csdn.net/so/search/s.do?q=教程&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=基础&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=node&t=blog)个人分类：[Rundeck																](https://blog.csdn.net/liumiaocn/article/category/8843151)
[
																								](https://so.csdn.net/so/search/s.do?q=node&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=rundeck&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
这篇文章介绍一下如何在Rundeck中进行Node的管理。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 缺省Node
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)缺省创建Project之后，缺省会将local作为一个节点管理起来，在Rundeck左侧的导航菜单可以确认到刚刚安装完毕之后的Node信息。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
从左可以看到hostname以及操作系统相关的信息，由于本系列文章是使用docker作为运行环境，没有设定hostname，所以hostname为上图所示, 另外进到容器之后也可以确认此hostname正是缺省的Node
[
](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@host132 ~
```
```python
]
```
```python
# docker exec -it rundeck sh
```
```python
sh-4.3$
```
```python
hostname
```
```python
426e66c11eb9
sh-4.3$
```
[
](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 创建Node
[
](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)创建Node有多种方式，这里选择使用Rundeck中xml配置文件的方式进行加入。按照如下步骤即可加入。
[
](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
## Step 1: 配置节点
[
](https://img-blog.csdnimg.cn/20190410143228654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)选择左侧导航菜单的Project Settings的Edit Nodes的菜单选项
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410143822825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
然后选择Configure Nodes之后，即可使用下图中的Add Source进行节点的添加了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410143919578.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在Rundeck中，被成为Resource Model Source，选择File的方式进行添加
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410144032910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
进行如下设定即可
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410144335812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Format主要支持xml/yaml/json三种方式，此处选择xml方式，然后按照如下方式准备xml文件
[
](https://img-blog.csdnimg.cn/20190410144335812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@host132 ~
```
```python
]
```
```python
# docker cp node-info.xml rundeck:/tmp
```
```python
[
```
```python
root@host132 ~
```
```python
]
```
```python
# docker exec -it rundeck sh
```
```python
sh-4.3$
```
```python
cat
```
```python
/tmp/node-info.xml
```
```python
<
```
```python
project
```
```python
>
```
```python
<
```
```python
node name
```
```python
=
```
```python
"host132"
```
```python
description
```
```python
=
```
```python
"host132 with ip : 192.168.163.132"
```
```python
tags
```
```python
=
```
```python
"test-env"
```
```python
osFamily
```
```python
=
```
```python
"linux"
```
```python
osName
```
```python
=
```
```python
"CentOS"
```
```python
hostname
```
```python
=
```
```python
"192.168.163.132"
```
```python
username
```
```python
=
```
```python
"rundeck"
```
```python
/
```
```python
>
```
```python
<
```
```python
/project
```
```python
>
```
```python
sh-4.3$
```
[
](https://img-blog.csdnimg.cn/20190410144335812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)设定文件说明：
[
](https://img-blog.csdnimg.cn/20190410144335812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)hostname：用于指定访问的机器
username：用于指定访问的用户，这里为了方便也使用官方镜像中相同的rundeck用户名
[
](https://img-blog.csdnimg.cn/20190410144335812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)点击save之后即保存完毕，如需修改可使用edit或者delete按钮进行操作
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410144634411.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
此时再次确认Node信息，则会发现Node已然添加进来了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410144905306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Node的详细信息如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410144846167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 其他内容
[https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)
# 参考内容
[https://www.rundeck.com/open-source](https://www.rundeck.com/open-source)

[
](https://img-blog.csdnimg.cn/20190410144905306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
