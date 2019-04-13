
# Kubernetes设定之TLS客户端访问 - 知行合一 止于至善 - CSDN博客

2019年04月01日 20:53:55[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：68标签：[kubernetes																](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)[TLS																](https://so.csdn.net/so/search/s.do?q=TLS&t=blog)[客户端																](https://so.csdn.net/so/search/s.do?q=客户端&t=blog)[pkcs12																](https://so.csdn.net/so/search/s.do?q=pkcs12&t=blog)[openssl																](https://so.csdn.net/so/search/s.do?q=openssl&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=pkcs12&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=客户端&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=客户端&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=客户端&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=TLS&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=kubernetes&t=blog)

使用TLS方式进行访问时，在客户端需要将证书进行导入才能进行访问，这篇文章通过具体示例进行简单说明。
# 环境准备
使用easypacack上提供的一键安装参数传入install all即可，目前缺省状态会打开6443端口，如果使用客户端直接访问，会提示如下错误信息。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
注：此处127.0.0.1的原因是因为使用了Virtual Box，此6443端口是运行与Virtual Box中的host131的6443与宿主机之间的端口映射。
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)创建一个浏览器可以使用 PKCS\#12/PFX 格式的证书
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@host131 ca
```
```python
]
```
```python
# ls
```
```python
admin-csr.json  ca-csr.json  ca.csr  kubeadmin-key.pem  kubeadmin.pem
ca-config.json  ca-key.pem   ca.pem  kubeadmin.csr      kubectl.kubeconfig
```
```python
[
```
```python
root@host131 ca
```
```python
]
```
```python
# openssl pkcs12 -export -out kubectladmin.pfx -inkey kubeadmin-key.pem -in kubeadmin.pem -certfile ca.pem
```
```python
Enter Export Password:
Verifying - Enter Export Password:
```
```python
[
```
```python
root@host131 ca
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)因为此处需要输入密码，不利于嵌入脚本之中，可以使用如下方式进行密码的缺省指定(此处指定密码为liumiaocn)
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@host131 ca
```
```python
]
```
```python
# openssl pkcs12 -export -out kubectladmin.pfx -inkey kubeadmin-key.pem -in kubeadmin.pem -certfile ca.pem -password pass:liumiaocn
```
```python
[
```
```python
root@host131 ca
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 客户端设定示例
[
](https://img-blog.csdnimg.cn/20190401142448149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)以下以Mac端设定为例进行说明。首先打开Keychain Access
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401130240704.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
导入生成的kubectladmin.pfx，过程中需要输入设定时输入的密码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401130358304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
由于证书是使用我们自己创建的CA进行的签名，这里需要将其设定为Alays Trust
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401130510885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
再次使用6443端口进行访问，然后选择我们设定的证书
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401130829348.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
此处需要使用MacOS的管理用户名和密码进行相关keychain的设定
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401130909825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
可以看到此时使用6443访问已经正常动作了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190401131045738.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

[
](https://img-blog.csdnimg.cn/20190401130909825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
