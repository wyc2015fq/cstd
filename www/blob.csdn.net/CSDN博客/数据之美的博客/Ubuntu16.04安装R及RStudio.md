# Ubuntu16.04安装R及RStudio - 数据之美的博客 - CSDN博客
2017年07月15日 16:47:56[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1660
主要内容：
- 介绍在Ubuntu16.04上安装R及RStudio
## 安装R
安装RStudio之前需要先安装R。
### <1>添加镜像源
- 添加镜像源到/etc/apt/sources.list
- 注意Ubuntu版本（本人Ubuntu版本是xenial，即16.04。根据版本改变对应的名字。）
```bash
# Ctrl+Alt+T打开终端
$ sudo echo "deb http://cran.rstudio.com/bin/linux/ubuntu xenial/" | sudo tee -a /etc/apt/sources.list
```
- 1
- 2
- 1
- 2
### <2>下载公钥
```
$ gpg --keyserver keyserver.ubuntu.com --recv-key 51716619E084DAB9
$ gpg -a --export 51716619E084DAB9 | sudo apt-key add -
```
- 1
- 2
- 1
- 2
### <3>安装r-base
```
$ sudo apt-get update
$ sudo apt-get install r-base r-base-dev
```
- 1
- 2
- 1
- 2
验证安装是否成功：打开终端，输入“R”，出现R的信息则安装成功。如： 
![这里写图片描述](https://img-blog.csdn.net/20161213163821104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV4dTI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 安装RStudio
直接从[RStudio](https://www.rstudio.com/)官网下载所需要的版本。（本人下载的rstudio-1.0.44-amd64.deb）
```
$ sudo apt-get install gdebi-core
$ sudo gdebi -n rstudio-1.0.44-amd64.deb
$ rm rstudio-1.0.44-amd64.deb
```
- 1
- 2
- 3
- 1
- 2
- 3
若安装成功，打开应用程序，会看到R和RStudio的图标。 
RStudio界面： 
![这里写图片描述](https://img-blog.csdn.net/20161213165625583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV4dTI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
