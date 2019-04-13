
# Git Github SSH key - 追求卓越,做到专业 - CSDN博客


2019年04月08日 17:35:49[Waldenz](https://me.csdn.net/enter89)阅读数：12


由于本地git仓库和github仓库之间的传输是通过SSH加密的，所以连接时需要设置SSH key
## 配置SSH key
有了这个专属的key，才可以实现身份的认证，才能正常push到Github
```python
ssh-keygen -t rsa -C w2323444@gmail.com
```
-C 后面替换成自己注册Github时申请的邮箱，然后一直enter,专属的SSH key就生成了。
ssh key就存储在 /Users/username/.ssh/id_rsa.pub中。
![](https://img-blog.csdnimg.cn/20190408165730490.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
## 复制出SSH key值
打开id_rsa.pub文件
```python
vi /Users/wadlenz/.ssh/id_rsa.pub
```
ssh key 内容格式如下：
```python
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC73aJPSg+9sX495U2Y0eYvKuisKtQitKgYZYKzJyNwG2ETt+7N6aJ+9t/LEBVwUPr+ArsMcBuU85zfLvEQH93WD+StnRj2Xu+fPVaKudYmcvKIpr6EhisAKS0oD5JPbNxQDGSa8zLWo160coIrQu3hVgItBqxsnNoxfPSD/+rh9O1a6MYlQBBJQYK28+8qFL6ukFo0BlmE3LtGhtU8QHVKxSlmYK8ATbXj2zO/LXPBVUUqk0WqP86nCElcoNgrNlu0aLGfUwOBd3y4V6xbtJFVz/1VkLUS7HyQ3gvJbyIBDOr5A3pBp5ckuwRCUabucl2UyRr************FF myaccount@gmail.com
```
## 将SSH key添加到Github上
Settings-->SSH and GPG keys-->New SSH key-> title: TheKey, key: 上述ssh key的全部内容
![](https://img-blog.csdnimg.cn/20190408172036526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
## 本地测试连接是否成功
```python
ssh -T git@github.com
```
![](https://img-blog.csdnimg.cn/20190408172548961.png)
## 本地配置Github
```python
git config --global user.name github_account_name
git config --global user.email github_account_email@gmail.com
```
此处的 github_account_name替换成你注册Github的用户名
此处的 githubb_account_email@gmail.com 替换成你注册Github时申请的邮箱

