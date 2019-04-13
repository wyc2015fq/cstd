
# Docker 阿里云加速器 - sinat_33731745的博客 - CSDN博客

2017年12月24日 22:31:01[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：74标签：[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)个人分类：[docker																](https://blog.csdn.net/sinat_33731745/article/category/7256766)



您可以通过修改daemon配置文件/etc/docker/daemon.json来使用加速器：
```python
sudo
```
```python
mkdir -p /etc/docker
```
```python
sudo
```
```python
tee /etc/docker/daemon.json <<-
```
```python
'EOF'
```
```python
{
```
```python
"registry-mirrors"
```
```python
: [
```
```python
"https://******.mirror.aliyuncs.com"
```
```python
]
}
EOF
```
```python
sudo
```
```python
systemctl daemon-reload
```
```python
sudo
```
```python
systemctl restart docker
```

