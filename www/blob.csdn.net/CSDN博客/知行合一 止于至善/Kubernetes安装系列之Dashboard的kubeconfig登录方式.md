
# Kubernetes安装系列之Dashboard的kubeconfig登录方式 - 知行合一 止于至善 - CSDN博客

2019年04月02日 06:31:59[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：117标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[dashboard																](https://so.csdn.net/so/search/s.do?q=dashboard&t=blog)[kubeconfig																](https://so.csdn.net/so/search/s.do?q=kubeconfig&t=blog)[k8s																](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)[plugin																](https://so.csdn.net/so/search/s.do?q=plugin&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=k8s&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=kubeconfig&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=kubeconfig&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=kubeconfig&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=dashboard&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

在上篇文章介绍了目前最新版本的dashbaord插件的安装以及利用token登录的方法，这篇文章整理来介绍一下使用kuberconfig进行登录的方法。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# 事前准备
安装和设定dashboard插件, 详细可参看：
[https://liumiaocn.blog.csdn.net/article/details/88957901](https://liumiaocn.blog.csdn.net/article/details/88957901)
# dashboard的登录方式
Dashboard有如下两种登录方式：
Token
Kubeconfig
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401213414901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 生成kubeconfig文件
具体命令固化成了两个函数，使用如下方式即可生成
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# . ./common-util.sh
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# display_dashboard_token
```
```python
## dashboard_secrete: dashboard-admin-token-44hd5
```
```python
## dashboard_token:
```
```python
eyJhbGciOiJSUzI1NiIsImtpZCI6IiJ9.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJkYXNoYm9hcmQtYWRtaW4tdG9rZW4tNDRoZDUiLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC5uYW1lIjoiZGFzaGJvYXJkLWFkbWluIiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZXJ2aWNlLWFjY291bnQudWlkIjoiNTY5MGI2OWMtNTI1Yy0xMWU5LTkyYzQtMDgwMDI3ZjhiZTZmIiwic3ViIjoic3lzdGVtOnNlcnZpY2VhY2NvdW50Omt1YmUtc3lzdGVtOmRhc2hib2FyZC1hZG1pbiJ9.r7FfVZmCU9y3IpLV1VlKXeHGh7whI8J4gqD9r3lq4_AMKv1SSfvbryAJ3aa5bGfq5fidDlH4E2zk1tRk0YyhYjG9nQ-x_fPD7ZUz44PKQ-9an_aUjX5G-kiSz4AbIy-Vd0Ctfrq6kuAh7Nasv0cfwybuzZa8fTNQkC8RVIarcZ57AveKrLdNeCdhUYkHYYLBt11vtk6MiyFAeFy9H7v5IB9Rh82u2es9hDZCl3_cabEyIb4IH82OIBsnhE0EnowV6kEbSDkFSkYYAavTDkwvAt3HZ50Z3efo8CosO5x7sU8MTxhRfew2ZOws0Urx9EoE7GlC3kugI0QNJS_TEjCj6A
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# create_kubedonfig_bytoken "dashboard.kubeconfig" "dashboard_client" "${ENV_DASHBOARD_TOKEN}"
```
```python
Cluster
```
```python
"kubernetes"
```
```python
set.
User
```
```python
"dashboard_client"
```
```python
set.
Context
```
```python
"dashboard_client"
```
```python
modified.
Switched to context
```
```python
"dashboard_client"
```
```python
.
```
```python
## kubeconfig file created: dashboard.kubeconfig
```
```python
dashboard.kubeconfig
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
详细可参看github上的代码，此处不再粘贴。
# 使用生成的登录
选中Kubeconfig方式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402062913787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
上传生成的kubeconfig文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402062937996.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
点击SIGN IN按钮登录
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040206295652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
然后可以就可使用dashboard了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402063057880.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

[
](https://img-blog.csdnimg.cn/2019040206295652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
