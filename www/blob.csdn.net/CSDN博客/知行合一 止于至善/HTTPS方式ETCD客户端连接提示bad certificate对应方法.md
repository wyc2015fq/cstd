
# HTTPS方式ETCD客户端连接提示bad certificate对应方法 - 知行合一 止于至善 - CSDN博客

2019年03月23日 14:34:25[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：237标签：[etcd																](https://so.csdn.net/so/search/s.do?q=etcd&t=blog)[https																](https://so.csdn.net/so/search/s.do?q=https&t=blog)[证书																](https://so.csdn.net/so/search/s.do?q=证书&t=blog)[hosts																](https://so.csdn.net/so/search/s.do?q=hosts&t=blog)[bad certificate																](https://so.csdn.net/so/search/s.do?q=bad certificate&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hosts&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=证书&t=blog)个人分类：[Kubernetes																](https://blog.csdn.net/liumiaocn/article/category/6328275)
[
																					](https://so.csdn.net/so/search/s.do?q=证书&t=blog)所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)[
							](https://so.csdn.net/so/search/s.do?q=证书&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=https&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=etcd&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=etcd&t=blog)

本文memo一下HTTPS方式下ETCD证书生成时的一个小错误。
# 现象：提示bad certificate错误
证书正常生成与设定，etcd能够正常启动，但是客户端连接2379端口时报错，报错示例：
```python
Mar 23 10:58:32 host131 etcd
```
```python
[
```
```python
7421
```
```python
]
```
```python
: rejected connection from
```
```python
"192.168.163.131:50958"
```
```python
(
```
```python
error
```
```python
"remote error: tls: bad certificate"
```
```python
, ServerName
```
```python
""
```
```python
)
```
# 原因
证书设定问题，需检查证书生成时csr文件中是否正确设定，IP是否设定到hosts中
# 对应方法
设定正确之后重新生成证书并部署，连接即可成功
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
# export ETCDCTL_API=3; etcdctl --endpoints=https://192.168.163.131:2379 --cacert=/etc/ssl/ca/ca.pem --cert=/etc/ssl/etcd/cert-etcd.pem --key=/etc/ssl/etcd/cert-etcd-key.pem endpoint health
```
```python
https://192.168.163.131:2379 is healthy: successfully committed proposal: took
```
```python
=
```
```python
1.528611ms
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

