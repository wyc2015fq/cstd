
# ubuntu16.04下安装Scrapy - lpty的博客 - CSDN博客

2017年02月27日 23:56:07[lpty](https://me.csdn.net/sinat_33741547)阅读数：1174


一 安装环境
1 系统环境：ubuntu16.04
2 python版本：2.7.12
二 安装
1 安装pip

```python
sudo pip install python-pip
```
2 安装Scrapy依赖库
```python
sudo apt-get install python-dev
sudo apt-get install libevent-dev
sudo apt-get install libssl-dev
```
3 scarpy需求lxml，OpenSSL，Twisted库一般系统自带，也可用以下方法安装
```python
sudo pip install lxml
sudo pip install pyOpenSSL
sudo pip install Twisted
```
4 安装Scrapy
```python
sudo pip install scrapy
```
三 检查Scrapy
```python
scrapy version
```





