
# ubuntu16.04下安装CRF++ - lpty的博客 - CSDN博客

2018年03月16日 15:01:12[lpty](https://me.csdn.net/sinat_33741547)阅读数：683个人分类：[杂七杂八																](https://blog.csdn.net/sinat_33741547/article/category/7513215)



### 一、环境
1、ubuntu16.04 LTS
2、CRF++-0.58
### 二、下载源文件
在这里下载CRF++包：[地址](https://drive.google.com/folderview?id=0B4y35FiV1wh7fngteFhHQUN2Y1B5eUJBNHZUemJYQV9VWlBUb3JlX0xBdWVZTWtSbVBneU0&usp=drive_web#list)
### 三、安装CRF
#### 1、解压，到目录中执行
`sudo ./configure  
sudo make  
sudo make install`
#### 2、安装CRFPP
进入CRF++-0.58/python路径下
`python setup.py build
sudo python setup.py install`
#### 3、import CRFPP出现错误
`ImportError: libcrfpp.so.0: cannot open shared object file: No such file or directory`首先，编辑文件
`sudo vim /etc/ld.so.conf`加入以下命令
`include /usr/local/lib`保存后执行
`sudo /sbin/ldconfig -v`

